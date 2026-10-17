macro_rules! eprintln {
    () => {{
        crate::emit_stderr(module_path!(), String::new())
    }};
    ($($arg:tt)*) => {{
        crate::emit_stderr(module_path!(), format!($($arg)*))
    }};
}

mod ast;
mod cgen;
mod lexer;
mod obfuscate;
mod parser;
mod sema;

use crate::lexer::Span;
use ast::*;
use indicatif::{MultiProgress, ProgressBar, ProgressStyle};
use std::collections::{HashSet, VecDeque};
use std::env;
use std::fs::{self, File, OpenOptions};
use std::io::{self, BufRead, IsTerminal, Write};
use std::path::{Path, PathBuf};
use std::process::{Command, Stdio};
use std::sync::atomic::{AtomicBool, Ordering};
use std::sync::{Arc, Mutex, OnceLock};
use std::thread;
use std::time::{Duration, Instant};

static LOGGER: OnceLock<Logger> = OnceLock::new();

struct DebugTarget {
    multi: MultiProgress,
    overall: ProgressBar,
    operation: ProgressBar,
    layer_bar: ProgressBar,
    scope: ProgressBar,
    metrics: ProgressBar,
    event_line: ProgressBar,
    active_section: String,
    active_iteration: usize,
    active_step: usize,
    active_total: usize,
    active_pass: String,
    active_description: String,
    last_step_started: Instant,
    average_sweep_seconds: f64,
    average_wpo_seconds: f64,
    active_functions: usize,
    active_pass_running: bool,
}

struct Logger {
    file: Mutex<Option<File>>,
    target: Mutex<Option<DebugTarget>>,
    started: Instant,
}

fn debug_log_path(args: &[String]) -> Option<PathBuf> {
    let mut path = None;
    let mut i = 0;
    while i < args.len() {
        if args[i] == "--debug-log" {
            if let Some(value) = args.get(i + 1) {
                path = Some(PathBuf::from(value));
                i += 2;
                continue;
            }
        } else if let Some(value) = args[i].strip_prefix("--debug-log=") {
            path = Some(PathBuf::from(value));
        }
        i += 1;
    }
    path
}

fn initialize_logger() -> io::Result<()> {
    let args = env::args().skip(1).collect::<Vec<_>>();
    let path = debug_log_path(&args);
    let file = if let Some(path) = path {
        if let Some(parent) = path.parent() {
            if !parent.as_os_str().is_empty() {
                fs::create_dir_all(parent)?;
            }
        }
        Some(
            OpenOptions::new()
                .create(true)
                .truncate(true)
                .write(true)
                .open(path)?,
        )
    } else {
        None
    };
    let _ = LOGGER.set(Logger {
        file: Mutex::new(file),
        target: Mutex::new(None),
        started: Instant::now(),
    });
    Ok(())
}

fn is_debug_message(module: &str, message: &str) -> bool {
    if module.ends_with("::sema")
        || module.ends_with("::cgen")
        || module.ends_with("::obfuscate")
    {
        return true;
    }
    let line = message.trim_start();
    [
        "[sema]",
        "[CGEN:",
        "[MAK:CGEN:",
        "[opt-debug]",
        "[opt-final]",
        "[scoped-debug]",
        "[alpha-debug]",
        "[WPO trace]",
        "[debug]",
        "[DEBUG]",
    ]
    .iter()
    .any(|prefix| line.starts_with(prefix))
}

fn write_debug_log(module: &str, message: &str) {
    let Some(logger) = LOGGER.get() else {
        return;
    };
    if let Ok(mut file_guard) = logger.file.lock() {
        if let Some(file) = file_guard.as_mut() {
            let elapsed = logger.started.elapsed().as_secs_f64();
            let _ = writeln!(file, "[+{elapsed:09.3}s] [{module}] {message}");
            let _ = file.flush();
        }
    }
}

fn emit_stderr(module: &str, message: String) {
    let Some(logger) = LOGGER.get() else {
        let _ = writeln!(io::stderr().lock(), "{message}");
        return;
    };
    write_debug_log(module, &message);

    if let Ok(target_guard) = logger.target.lock() {
        if let Some(target) = target_guard.as_ref() {
            if is_debug_message(module, &message) {
                if let Some(latest) = message.lines().rev().find(|line| !line.trim().is_empty()) {
                    target.event_line.set_message(clip_end(latest.trim(), 118));
                }
            } else {
                let _ = target.multi.println(&message);
            }
            return;
        }
    }

    let _ = writeln!(io::stderr().lock(), "{message}");
}

fn pretty_seconds(seconds: f64) -> String {
    if !seconds.is_finite() || seconds < 0.0 {
        return "calculating".to_string();
    }
    if seconds < 1.0 {
        return format!("{:.1}s", seconds.max(0.1));
    }
    let total = seconds.ceil() as u64;
    let hours = total / 3600;
    let minutes = (total % 3600) / 60;
    let seconds = total % 60;
    if hours > 0 {
        format!("{hours:02}:{minutes:02}:{seconds:02}")
    } else if minutes > 0 {
        format!("{minutes:02}:{seconds:02}")
    } else {
        format!("{seconds:02}s")
    }
}

fn live_layer_message(target: &DebugTarget) -> String {
    if target.active_pass.is_empty() {
        return "waiting for optimizer work".to_string();
    }
    let total = target.active_total.max(1);
    let end_label = if target.active_section == "SWEEP" {
        "sweep end"
    } else {
        "WPO round end"
    };
    if !target.active_pass_running {
        if target.active_step >= total {
            return format!("{total}/{total} passes done · checking convergence");
        }
        return "pass finished · preparing next pass".to_string();
    }
    let average = if target.active_section == "SWEEP" {
        target.average_sweep_seconds
    } else {
        target.average_wpo_seconds
    };
    if average <= 0.0 {
        return "ETA calibrating from completed passes".to_string();
    }
    let elapsed = target.last_step_started.elapsed().as_secs_f64();
    let later_passes = total.saturating_sub(target.active_step) as f64;
    if elapsed < average {
        let remaining = (average - elapsed) + average * later_passes;
        format!("ETA ~{} to {end_label}", pretty_seconds(remaining))
    } else if later_passes > 0.0 {
        let lower_bound = average * later_passes;
        format!(
            "ETA ≥~{} to {end_label} · active pass exceeds recent average",
            pretty_seconds(lower_bound)
        )
    } else {
        "active pass exceeds recent average · ETA still updating".to_string()
    }
}

pub fn optimizer_phase(message: &str) {
    let Some(logger) = LOGGER.get() else {
        return;
    };
    if let Ok(mut target_guard) = logger.target.lock() {
        if let Some(target) = target_guard.as_mut() {
            target.active_pass.clear();
            target.active_description.clear();
            target.active_section.clear();
            target.active_pass_running = false;
            target.last_step_started = Instant::now();
            target.layer_bar.reset();
            target.layer_bar.reset_elapsed();
            target.layer_bar.set_message("waiting for the next measured pass");
            target.operation.reset_elapsed();
            target.operation.set_message(clip_end(message, 94));
            target.scope.set_message("Waiting for the current pass to expose its function and expression cursor".to_string());
        }
    }
}

pub fn optimizer_subtask(message: &str) {
    let Some(logger) = LOGGER.get() else {
        return;
    };
    if let Ok(target_guard) = logger.target.lock() {
        if let Some(target) = target_guard.as_ref() {
            if target.active_pass.is_empty() {
                target.operation.set_message(clip_end(message, 94));
            } else {
                let action = format!(
                    "src/obfuscate.rs · {} {:03}/{:03} · {message}",
                    target.active_pass,
                    target.active_step,
                    target.active_total
                );
                target.operation.set_message(clip_end(&action, 94));
            }
        }
    }
}

pub fn optimizer_layer_progress(
    section: &str,
    iteration: usize,
    step: usize,
    total_steps: usize,
    pass: &str,
    description: &str,
    nodes: usize,
    functions: usize,
    current_cost: u64,
    best_cost: u64,
    accepted: usize,
    rejected: usize,
    changed_in_iteration: bool,
) {
    let Some(logger) = LOGGER.get() else {
        return;
    };
    if let Ok(mut target_guard) = logger.target.lock() {
        let Some(target) = target_guard.as_mut() else {
            return;
        };
        let total = total_steps.max(1);
        if target.active_section != section
            || target.active_iteration != iteration
            || target.active_total != total
        {
            target.layer_bar.reset();
            target.layer_bar.set_length(total as u64);
            target.layer_bar.set_prefix(format!("{section} {iteration:02}"));
        }
        target.active_section = section.to_string();
        target.active_iteration = iteration;
        target.active_step = step;
        target.active_total = total;
        target.active_pass = pass.to_string();
        target.active_description = description.to_string();
        target.active_pass_running = true;
        target.last_step_started = Instant::now();
        target.active_functions = functions;
        target.layer_bar.set_length(total as u64);
        target.layer_bar.set_position(step.saturating_sub(1) as u64);
        target.layer_bar.set_prefix(format!("{section} {iteration:02}"));
        target.layer_bar.reset_elapsed();
        target.layer_bar.set_message(live_layer_message(target));
        target.operation.reset_elapsed();
        target.operation.set_message(clip_end(&format!(
            "src/obfuscate.rs · {section} {iteration:02} · pass {step:03}/{total:03} `{pass}` — {description}"
        ), 94));
        target.scope.set_message(clip_end(&format!(
            "Program-wide operation · {nodes} AST nodes / {functions} functions · expression cursor appears when this pass walks the tree"
        ), 108));
        let progress_state = match (section, changed_in_iteration) {
            ("WPO", true) => "this whole-program round changed the candidate; another round may follow",
            ("WPO", false) => "no accepted whole-program rewrite in this round yet",
            (_, true) => "this sweep has accepted rewrites; convergence will be checked after it",
            _ => "no accepted rewrite yet in this sweep",
        };
        target.metrics.set_message(clip_end(&format!(
            "Estimated cost {current_cost} · best {best_cost} · AST {nodes} nodes / {functions} functions · {accepted} committed · {rejected} rejected · {progress_state}"
        ), 108));
    }
}

pub fn optimizer_node_progress(function: &str, depth: usize, visited: usize, total: usize) {
    let Some(logger) = LOGGER.get() else {
        return;
    };
    if let Ok(target_guard) = logger.target.lock() {
        if let Some(target) = target_guard.as_ref() {
            let function = clip_end(function, 54);
            target.scope.set_message(format!(
                "Cursor `{function}` · expr {visited}/{total} · depth {depth}"
            ));
        }
    }
}

pub fn optimizer_result_progress(
    pass: &str,
    accepted_pass: bool,
    current_cost: u64,
    best_cost: u64,
    nodes: usize,
    accepted: usize,
    rejected: usize,
) {
    let Some(logger) = LOGGER.get() else {
        return;
    };
    if let Ok(mut target_guard) = logger.target.lock() {
        if let Some(target) = target_guard.as_mut() {
            let elapsed = target.last_step_started.elapsed().as_secs_f64();
            if elapsed.is_finite() && elapsed >= 0.0001 {
                if target.active_section == "SWEEP" {
                    target.average_sweep_seconds = if target.average_sweep_seconds <= 0.0 {
                        elapsed
                    } else {
                        target.average_sweep_seconds * 0.78 + elapsed * 0.22
                    };
                } else {
                    target.average_wpo_seconds = if target.average_wpo_seconds <= 0.0 {
                        elapsed
                    } else {
                        target.average_wpo_seconds * 0.78 + elapsed * 0.22
                    };
                }
            }
            target.active_pass_running = false;
            target.layer_bar.set_position(target.active_step.min(target.active_total) as u64);
            target.layer_bar.set_message(live_layer_message(target));
            target.metrics.set_message(clip_end(&format!(
                "{pass}: {} · cost {current_cost} · best {best_cost} · {nodes} nodes / {} fns · {accepted} committed / {rejected} rejected · pass {}",
                if accepted_pass { "accepted" } else { "no safe improvement" },
                target.active_functions,
                fmt_duration(Duration::from_secs_f64(elapsed)),
            ), 108));
            if !accepted_pass {
                target.event_line.set_message(clip_end(&format!("{pass}: no safe improvement; current best remains {best_cost}"), 108));
            } else {
                target.event_line.set_message(clip_end(&format!("{pass}: candidate validated; estimated cost is {current_cost}"), 108));
            }
        }
    }
}

fn stream_backend_pipe<R: BufRead + Send + 'static>(
    mut reader: R,
    module: &'static str,
    to_stderr: bool,
) -> io::Result<()> {
    let mut bytes = Vec::new();
    loop {
        bytes.clear();
        if reader.read_until(b'\n', &mut bytes)? == 0 {
            break;
        }
        let mut text = String::from_utf8_lossy(&bytes).into_owned();
        while text.ends_with('\n') || text.ends_with('\r') {
            text.pop();
        }
        if to_stderr {
            emit_stderr(module, text);
        } else {
            write_debug_log(module, &text);
            let mut stdout = io::stdout().lock();
            stdout.write_all(&bytes)?;
            stdout.flush()?;
        }
    }
    Ok(())
}

const VERSION: &str = env!("CARGO_PKG_VERSION");

#[derive(Clone, Copy)]
struct Theme {
    enabled: bool,
}
impl Theme {
    fn new() -> Self {
        let enabled = env::var_os("NO_COLOR").is_none() && io::stderr().is_terminal();
        Self { enabled }
    }
    fn paint(self, code: &'static str, text: impl AsRef<str>) -> String {
        if self.enabled {
            format!("{code}{}{RESET}", text.as_ref())
        } else {
            text.as_ref().to_string()
        }
    }
}

const RESET: &str = "\x1b[0m";
const DIM: &str = "\x1b[2m";
const BOLD: &str = "\x1b[1m";
const RED: &str = "\x1b[31m";
const GREEN: &str = "\x1b[32m";
const CYAN: &str = "\x1b[36m";
const BLUE: &str = "\x1b[34m";

#[derive(Debug, Clone)]
struct Cli {
    input: PathBuf,
    output: Option<PathBuf>,
    compile: bool,
    target: String,
    backend: Option<String>,
    linker: Option<String>,
    cflags: Vec<String>,
    link_args: Vec<String>,
    defines: Vec<String>,
    includes: Vec<PathBuf>,
    lib_paths: Vec<PathBuf>,
    libs: Vec<String>,
    verbose: bool,
    quiet: bool,
    dry_run: bool,
    debug_log: Option<PathBuf>,
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
enum OutputKind {
    C,
    Lir,
    Binary,
}

fn usage() -> &'static str {
    r#"MAK — low-level compiler driver

USAGE
    mak [OPTIONS] <INPUT.mak>

OUTPUT
    -o, --output <PATH>       Output artifact / binary name.
                              No -cc: writes PATH.c unless PATH is .c/.mLIR/.LIR.
                              With -cc: PATH is the binary; PATH.c is kept beside it.
                              .LIR/.lir/.mlir are normalized to .mLIR and emit MAK LIR.

COMPILATION
    -cc, --compile            Generate C and invoke the backend compiler.
    -bcc=<NAME>               Backend compiler: clang, gcc, cc, zig. `zig` means `zig cc`.
        --backend <NAME>      Same as -bcc=NAME.
    -t, --target <TARGET>     Target name or triple. `native` uses the host.
                              Easy names: x64, arm64, x86, win64, arm, riscv64.

LINKING / C
    --linker <NAME>           Linker passed through -fuse-ld=<NAME> (mold, lld, ...).
    -C, --cflag <FLAG>        Extra C compiler flag; repeatable.
    --link-arg <ARG>          Extra linker argument; repeatable.
    -D, --define <NAME[=V]>   Add a C preprocessor definition.
    -I, --include <DIR>       Add a header search directory.
    -L, --lib-path <DIR>      Add a library search directory.
    -l, --lib <NAME>          Link a library.

DRIVER
    -v, --verbose             Print compiler phases, timings, and backend command.
    -q, --quiet               Suppress normal success messages.
        --dry-run             Run all compiler phases without invoking the backend.
        --debug-log <PATH>    Write timestamped internal stderr/debug output and
                              captured backend stdout/stderr to this file.
    -h, --help                Show this help.
    -V, --version             Show MAK version.

EXAMPLES
    mak main.mak
        → main.c

    mak main.mak -o game
        → game.c

    mak main.mak -cc -o game
        → game.c + game

    mak main.mak -cc -o game -t x86_64-linux-gnu -bcc=clang
        → cross-compiled game

    mak main.mak -cc -o game -bcc=zig -t x64 -ld=mold
        → Zig C driver + mold

    mak main.mak --debug-log logs/build.log
        → generate C and save the complete internal log

    mak main.mak -cc -o game --debug-log logs/build.log
        → compile, link, and capture backend output

    mak main.mak -o optimized.mLIR
        → optimized MAK LIR

TUI
    BUILD    Current pipeline phase and total elapsed time. It stays indeterminate
             rather than showing a misleading percentage while optimization runs.
    SOURCE   Input file and bytes, output path, target, and debug-log destination.
    ACTION   src/obfuscate.rs, sweep/round, exact pass index/name, purpose,
             live pass runtime, and any current subtask. Local position 12 is
             `inline`; position 18 is `adce`.
    SWEEP    Completed-pass percentage/count out of 128 (or whole-program passes)
             and rolling ETA to sweep/round end based on observed pass durations.
             Overlong passes show a lower-bound/refreshing message, never a false 0s.
    SCOPE    Current source function, expressions visited/total, and AST depth.
    STATS    Estimated runtime cost, best verified cost, AST size, accepted and
             rejected candidates, and the duration/result of the current pass.
    UPDATE   Most recent optimization result or notable compiler event.

    The optimizer searches until a stable, verified result is reached. Its total
    finish time cannot be known in advance; ETA refers to the current pass sweep
    and updates as measured pass durations become available. Use --debug-log to
    preserve every intercepted internal stderr message and backend output.
"#
}

fn print_banner(theme: Theme) {
    let title = theme.paint(BOLD, "MAK");
    let sub = theme.paint(DIM, "low-level compiler driver");
    eprintln!("{}  {}", title, sub);
}

fn info(theme: Theme, msg: impl AsRef<str>) {
    eprintln!("{} {}", theme.paint(CYAN, "→"), msg.as_ref());
}

fn ok(theme: Theme, msg: impl AsRef<str>) {
    eprintln!("{} {}", theme.paint(GREEN, "✓"), msg.as_ref());
}

fn fmt_duration(d: Duration) -> String {
    if d.as_secs() > 0 {
        format!("{:.2}s", d.as_secs_f64())
    } else if d.as_millis() > 0 {
        format!("{}ms", d.as_millis())
    } else {
        format!("{}µs", d.as_micros())
    }
}

fn compact_label(value: &str, max_chars: usize) -> String {
    let chars = value.chars().collect::<Vec<_>>();
    if chars.len() <= max_chars {
        return value.to_string();
    }
    let tail = chars[chars.len().saturating_sub(max_chars.saturating_sub(1))..]
        .iter()
        .copied()
        .collect::<String>();
    format!("…{tail}")
}

fn clip_end(value: &str, max_chars: usize) -> String {
    let chars = value.chars().collect::<Vec<_>>();
    if chars.len() <= max_chars {
        return value.to_string();
    }
    let mut clipped = chars
        .into_iter()
        .take(max_chars.saturating_sub(1))
        .collect::<String>();
    clipped.push('…');
    clipped
}

struct ProgressUi {
    overall: ProgressBar,
    status: ProgressBar,
    operation: ProgressBar,
    layer_bar: ProgressBar,
    scope: ProgressBar,
    metrics: ProgressBar,
    event_line: ProgressBar,
    input: PathBuf,
    output: PathBuf,
    target: String,
    debug_log: Option<PathBuf>,
    source_bytes: u64,
    started: Instant,
    finished: bool,
    eta_stop: Arc<AtomicBool>,
    eta_thread: Option<thread::JoinHandle<()>>,
}

impl ProgressUi {
    fn new(
        input: &Path,
        output: &Path,
        target: &str,
        debug_log: Option<PathBuf>,
    ) -> Option<Self> {
        if !io::stderr().is_terminal() {
            return None;
        }

        let multi = MultiProgress::new();
        let overall = multi.add(ProgressBar::new_spinner());
        overall.set_style(
            ProgressStyle::with_template(
                "{spinner:.bold.cyan} {prefix:.bold.cyan} {msg} · total elapsed {elapsed_precise}"
            )
            .expect("valid build status template"),
        );
        overall.set_prefix("BUILD  │");
        overall.set_message("preparing");
        overall.enable_steady_tick(Duration::from_millis(100));

        let status = multi.add(ProgressBar::new_spinner());
        status.set_style(
            ProgressStyle::with_template("{spinner:.cyan} {prefix:.bold} {msg}")
                .expect("valid source template"),
        );
        status.set_prefix("SOURCE │");
        status.enable_steady_tick(Duration::from_millis(100));

        let operation = multi.add(ProgressBar::new_spinner());
        operation.set_style(
            ProgressStyle::with_template(
                "{spinner:.bold.magenta} {prefix:.bold.magenta} {msg} · active {elapsed_precise}"
            )
            .expect("valid operation template"),
        );
        operation.set_prefix("ACTION │");
        operation.enable_steady_tick(Duration::from_millis(120));

        let layer_bar = multi.add(ProgressBar::new(128));
        layer_bar.set_style(
            ProgressStyle::with_template(
                "{prefix:.bold.green} [{bar:24.green/blue}] {percent:>3}% · {pos:>3}/{len} passes done · {msg}"
            )
            .expect("valid optimizer template")
            .progress_chars("█▓░"),
        );
        layer_bar.set_prefix("SWEEP");
        layer_bar.set_message("waiting for the optimizer");
        layer_bar.enable_steady_tick(Duration::from_millis(200));

        let scope = multi.add(ProgressBar::new_spinner());
        scope.set_style(
            ProgressStyle::with_template("{prefix:.bold.yellow} {msg}")
                .expect("valid scope template"),
        );
        scope.set_prefix("SCOPE  │");

        let metrics = multi.add(ProgressBar::new_spinner());
        metrics.set_style(
            ProgressStyle::with_template("{prefix:.bold.blue} {msg}")
                .expect("valid metrics template"),
        );
        metrics.set_prefix("STATS  │");

        let event_line = multi.add(ProgressBar::new_spinner());
        event_line.set_style(
            ProgressStyle::with_template("{prefix:.dim} {msg}")
                .expect("valid event template"),
        );
        event_line.set_prefix("UPDATE │");

        if let Some(logger) = LOGGER.get() {
            if let Ok(mut target) = logger.target.lock() {
                *target = Some(DebugTarget {
                    multi: multi.clone(),
                    overall: overall.clone(),
                    operation: operation.clone(),
                    layer_bar: layer_bar.clone(),
                    scope: scope.clone(),
                    metrics: metrics.clone(),
                    event_line: event_line.clone(),
                    active_section: String::new(),
                    active_iteration: 0,
                    active_step: 0,
                    active_total: 1,
                    active_pass: String::new(),
                    active_description: String::new(),
                    last_step_started: Instant::now(),
                    average_sweep_seconds: 0.0,
                    average_wpo_seconds: 0.0,
                    active_functions: 0,
                    active_pass_running: false,
                });
            }
        }

        let eta_stop = Arc::new(AtomicBool::new(false));
        let thread_stop = Arc::clone(&eta_stop);
        let eta_thread = thread::spawn(move || {
            while !thread_stop.load(Ordering::Relaxed) {
                if let Some(logger) = LOGGER.get() {
                    if let Ok(target_guard) = logger.target.lock() {
                        if let Some(target) = target_guard.as_ref() {
                            target.layer_bar.set_message(live_layer_message(target));
                        }
                    }
                }
                thread::sleep(Duration::from_millis(200));
            }
        });

        let mut ui = Self {
            overall,
            status,
            operation,
            layer_bar,
            scope,
            metrics,
            event_line,
            input: input.to_path_buf(),
            output: output.to_path_buf(),
            target: target.to_string(),
            debug_log,
            source_bytes: fs::metadata(input).map(|m| m.len()).unwrap_or(0),
            started: Instant::now(),
            finished: false,
            eta_stop,
            eta_thread: Some(eta_thread),
        };
        ui.set_stage("preparing source and output paths", 0);
        Some(ui)
    }

    fn set_stage(&mut self, stage: &str, position: u64) {
        let stage_label = match position {
            0 => "1/5 · loading and parsing",
            1 => "2/5 · semantic analysis",
            2 => "3/5 · optimizing and obfuscating",
            3 => "4/5 · generating output",
            _ => "5/5 · finalizing build",
        };
        let estimate_note = if position == 2 {
            " · total ETA depends on optimizer convergence"
        } else {
            ""
        };
        self.overall.set_message(format!("{stage_label} · {stage}{estimate_note}"));
        let input = compact_label(&self.input.display().to_string(), 26);
        let output = compact_label(&self.output.display().to_string(), 26);
        let target = compact_label(&self.target, 12);
        let log = self
            .debug_log
            .as_ref()
            .map(|p| compact_label(&p.display().to_string(), 22))
            .unwrap_or_else(|| "off".to_string());
        self.status.set_message(format!(
            "{input} ({} B) → {output} · {target} · log {log}",
            self.source_bytes
        ));
        self.operation.reset_elapsed();
        self.operation.set_message(clip_end(stage, 98));
    }

    fn stop_eta_thread(&mut self) {
        self.eta_stop.store(true, Ordering::Relaxed);
        if let Some(handle) = self.eta_thread.take() {
            let _ = handle.join();
        }
    }

    fn finish(&mut self, message: &str) {
        if self.finished {
            return;
        }
        self.stop_eta_thread();
        self.overall.finish_with_message(format!("5/5 · {message}"));
        self.status.finish_with_message(format!(
            "{} → {} · elapsed {} · {message}",
            self.input.display(),
            self.output.display(),
            fmt_duration(self.started.elapsed())
        ));
        self.operation.finish();
        self.layer_bar.finish_with_message("optimizer phase complete");
        self.scope.finish();
        self.metrics.finish();
        self.event_line.finish();
        self.clear_debug_target();
        self.finished = true;
    }

    fn clear_debug_target(&self) {
        if let Some(logger) = LOGGER.get() {
            if let Ok(mut target) = logger.target.lock() {
                *target = None;
            }
        }
    }
}

impl Drop for ProgressUi {
    fn drop(&mut self) {
        self.stop_eta_thread();
        if !self.finished {
            let elapsed = fmt_duration(self.started.elapsed());
            self.overall.abandon_with_message(format!("interrupted after {elapsed}"));
            self.status.finish_with_message("BUILD INTERRUPTED");
            self.operation.finish();
            self.layer_bar.abandon_with_message("optimizer interrupted");
            self.scope.finish();
            self.metrics.finish();
            self.event_line.finish();
            self.clear_debug_target();
        }
    }
}

fn line_col(src: &[u8], offset: u32) -> (u32, u32) {
    let off = (offset as usize).min(src.len());
    let (mut line, mut col) = (1u32, 1u32);
    for &b in &src[..off] {
        if b == b'\n' {
            line += 1;
            col = 1;
        } else {
            col += 1;
        }
    }
    (line, col)
}

fn get_line(src: &[u8], line: u32) -> (usize, usize) {
    let (mut n, mut start) = (1u32, 0usize);
    for (i, &b) in src.iter().enumerate() {
        if n == line {
            if b == b'\n' {
                return (start, i);
            }
        } else if b == b'\n' {
            n += 1;
            start = i + 1;
        }
    }
    if n == line {
        (start, src.len())
    } else {
        (0, 0)
    }
}

fn print_error(theme: Theme, path: &str, src: &[u8], span: Span, msg: &str, note: Option<&str>) {
    let (line, col) = line_col(src, span.lo);
    let (ls, le) = get_line(src, line);
    let line_bytes = &src[ls..le.min(src.len())];
    let line_str = String::from_utf8_lossy(line_bytes);

    let mut caret = String::new();
    for i in 0..(col as usize).saturating_sub(1) {
        caret.push(if line_bytes.get(i).copied() == Some(b'\t') {
            '\t'
        } else {
            ' '
        });
    }
    let width = (span.hi.saturating_sub(span.lo)).max(1) as usize;
    caret.extend(std::iter::repeat('^').take(width));

    let line_num = line.to_string();
    let pad = " ".repeat(line_num.len());
    let loc = format!("{path}:{line}:{col}");

    eprintln!("{}: {}", theme.paint(RED, "error"), theme.paint(BOLD, msg));
    eprintln!(" {} {}", theme.paint(BLUE, "-->"), theme.paint(CYAN, loc));
    eprintln!(
        " {}{}{}",
        theme.paint(BLUE, pad.as_str()),
        theme.paint(BLUE, " |"),
        ""
    );
    eprintln!(
        " {}{} {}",
        theme.paint(BLUE, &format!("{line_num:>w$}", w = line_num.len())),
        theme.paint(BLUE, " |"),
        line_str
    );
    eprintln!(
        " {}{} {}",
        theme.paint(BLUE, pad.as_str()),
        theme.paint(BLUE, " |"),
        theme.paint(RED, caret)
    );
    eprintln!(
        " {}{} {}",
        theme.paint(BLUE, pad.as_str()),
        theme.paint(BLUE, " |"),
        theme.paint(DIM, format!("span {}..{}", span.lo, span.hi))
    );
    if let Some(note) = note {
        eprintln!("  {} {}", theme.paint(CYAN, "note:"), note);
    }
    eprintln!();
}

fn parse_cli(theme: Theme) -> Result<Cli, String> {
    let args = env::args().skip(1).collect::<Vec<_>>();
    if args.iter().any(|a| a == "--help" || a == "-h") {
        println!("{}", usage());
        std::process::exit(0);
    }
    if args.iter().any(|a| a == "--version" || a == "-V") {
        println!("mak {VERSION}");
        std::process::exit(0);
    }

    if args.is_empty() {
        return Err("no input file supplied\n\nRun `mak --help` for usage.".into());
    }

    let mut input = None;
    let mut output: Option<PathBuf> = None;
    let mut compile = false;
    let mut target = "native".to_string();
    let mut backend = None;
    let mut linker = None;
    let mut cflags = Vec::new();
    let mut link_args = Vec::new();
    let mut defines = Vec::new();
    let mut includes = Vec::new();
    let mut lib_paths = Vec::new();
    let mut libs = Vec::new();
    let mut verbose = false;
    let mut quiet = false;
    let mut dry_run = false;
    let mut debug_log = None;

    let mut i = 0;
    while i < args.len() {
        let raw = args[i].clone();
        if raw == "--" {
            i += 1;
            if i < args.len() {
                if input.is_some() {
                    return Err("multiple input files are not supported yet".into());
                }
                input = Some(PathBuf::from(&args[i]));
                i += 1;
            }
            if i != args.len() {
                return Err("unexpected arguments after input file".into());
            }
            break;
        }

        match raw.as_str() {
            "-cc" | "--compile" => compile = true,
            "-v" | "--verbose" => verbose = true,
            "-q" | "--quiet" => quiet = true,
            "--dry-run" => dry_run = true,
            "--debug-log" => {
                i += 1;
                debug_log = Some(PathBuf::from(required(&args, i, "--debug-log")?));
            }
            "-o" | "--output" => {
                i += 1;
                output = Some(PathBuf::from(required(&args, i, "-o/--output")?));
            }
            "-t" | "--target" => {
                i += 1;
                target = required(&args, i, "-t/--target")?;
            }
            "--backend" | "-bcc" => {
                i += 1;
                backend = Some(required(&args, i, "--backend/-bcc")?);
            }
            "--linker" | "-ld" | "-lld" | "-ldd" => {
                i += 1;
                linker = Some(required(&args, i, "--linker/-ld")?);
            }
            "-C" | "--cflag" => {
                i += 1;
                cflags.push(required(&args, i, "-C/--cflag")?);
            }
            "--link-arg" => {
                i += 1;
                link_args.push(required(&args, i, "--link-arg")?);
            }
            "-D" | "--define" => {
                i += 1;
                defines.push(required(&args, i, "-D/--define")?);
            }
            "-I" | "--include" => {
                i += 1;
                includes.push(PathBuf::from(required(&args, i, "-I/--include")?));
            }
            "-L" | "--lib-path" => {
                i += 1;
                lib_paths.push(PathBuf::from(required(&args, i, "-L/--lib-path")?));
            }
            "-l" | "--lib" => {
                i += 1;
                libs.push(required(&args, i, "-l/--lib")?);
            }
            _ if raw.starts_with("--debug-log=") => {
                let value = raw.split_once('=').unwrap().1;
                if value.is_empty() {
                    return Err("--debug-log expects a path".into());
                }
                debug_log = Some(PathBuf::from(value));
            }
            _ if raw.starts_with("-o=") || raw.starts_with("--output=") => {
                output = Some(PathBuf::from(raw.split_once('=').unwrap().1));
            }
            _ if raw.starts_with("-t=") || raw.starts_with("--target=") => {
                target = raw.split_once('=').unwrap().1.to_string();
            }
            _ if raw.starts_with("-bcc=") || raw.starts_with("--backend=") => {
                backend = Some(raw.split_once('=').unwrap().1.to_string());
            }
            _ if raw.starts_with("-ld=")
                || raw.starts_with("--linker=")
                || raw.starts_with("-lld=")
                || raw.starts_with("-ldd=") =>
            {
                linker = Some(raw.split_once('=').unwrap().1.to_string());
            }
            _ if raw.starts_with("-C=") || raw.starts_with("--cflag=") => {
                cflags.push(raw.split_once('=').unwrap().1.to_string());
            }
            _ if raw.starts_with("--link-arg=") => {
                link_args.push(raw.split_once('=').unwrap().1.to_string());
            }
            _ if raw.starts_with("-D=") || raw.starts_with("--define=") => {
                defines.push(raw.split_once('=').unwrap().1.to_string());
            }
            _ if raw.starts_with("-D") && raw.len() > 2 => defines.push(raw[2..].to_string()),
            _ if raw.starts_with("-I") && raw.len() > 2 => includes.push(PathBuf::from(&raw[2..])),
            _ if raw.starts_with("-L") && raw.len() > 2 => lib_paths.push(PathBuf::from(&raw[2..])),
            _ if raw.starts_with("-l") && raw.len() > 2 => libs.push(raw[2..].to_string()),
            _ if raw.starts_with('-') => {
                return Err(format!("unknown option `{raw}`"));
            }
            _ => {
                if input.is_some() {
                    return Err("multiple input files are not supported yet".into());
                }
                input = Some(PathBuf::from(args[i].clone()));
            }
        }
        i += 1;
    }

    let input = input.ok_or_else(|| "no input file supplied".to_string())?;
    if target.trim().is_empty() {
        return Err("target cannot be empty".into());
    }
    if linker.as_deref() == Some("") {
        linker = None;
    }
    Ok(Cli {
        input,
        output,
        compile,
        target,
        backend,
        linker,
        cflags,
        link_args,
        defines,
        includes,
        lib_paths,
        libs,
        verbose,
        quiet,
        dry_run,
        debug_log,
    })
}

fn required(args: &[String], index: usize, flag: &str) -> Result<String, String> {
    args.get(index)
        .cloned()
        .ok_or_else(|| format!("{flag} expects a value"))
}

fn target_alias(input: &str) -> String {
    let t = input.trim().to_ascii_lowercase();
    match t.as_str() {
        "native" | "host" | "local" => "native".into(),
        "x64" | "amd64" | "x86_64" | "linux-x64" | "linux-amd64" | "x64-linux" | "x86_64-linux"
        | "x64-linux-gnu" => "x86_64-linux-gnu".into(),
        "arm64" | "aarch64" | "linux-arm64" | "arm64-linux" | "aarch64-linux"
        | "arm64-linux-gnu" => "aarch64-linux-gnu".into(),
        "win64" | "windows-x64" | "windows-amd64" | "x64-windows" | "x86_64-windows"
        | "x64-windows-gnu" => "x86_64-windows-gnu".into(),
        "x86" | "i386" | "i686" | "linux-x86" | "x86-linux" | "i686-linux" => {
            "i686-linux-gnu".into()
        }
        "arm" | "arm32" | "armv7" | "armv7-linux" => "armv7-linux-gnueabihf".into(),
        "riscv64" | "rv64" | "riscv64-linux" => "riscv64-linux-gnu".into(),
        other => other.to_string(),
    }
}

fn output_kind(path: &Path, compile: bool, explicit: bool) -> OutputKind {
    let ext = path
        .extension()
        .and_then(|x| x.to_str())
        .unwrap_or("")
        .to_ascii_lowercase();

    if ext == "mlir" || ext == "lir" {
        OutputKind::Lir
    } else if ext == "c" {
        OutputKind::C
    } else if compile {
        OutputKind::Binary
    } else if explicit {
        OutputKind::C
    } else {
        OutputKind::C
    }
}

fn normalize_lir_path(mut path: PathBuf) -> PathBuf {
    path.set_extension("mLIR");
    path
}

fn default_base(input: &Path) -> PathBuf {
    input.with_extension("")
}

fn derive_c_path(output: &Path) -> PathBuf {
    output.with_extension("c")
}

fn derive_binary_from_c(c_path: &Path) -> PathBuf {
    c_path.with_extension("")
}

fn ensure_parent(path: &Path) -> io::Result<()> {
    if let Some(parent) = path.parent() {
        if !parent.as_os_str().is_empty() && parent != Path::new(".") {
            fs::create_dir_all(parent)?;
        }
    }
    Ok(())
}

fn command_in_path(program: &str) -> bool {
    let p = Path::new(program);
    if p.components().count() > 1 {
        return p.is_file();
    }
    let Some(path) = env::var_os("PATH") else {
        return false;
    };
    env::split_paths(&path).any(|dir| dir.join(program).is_file())
}

fn shell_split(s: &str) -> Result<Vec<String>, String> {
    let mut out = Vec::new();
    let mut cur = String::new();
    let mut quote = None;
    let mut escaped = false;
    let mut started = false;

    for ch in s.chars() {
        if escaped {
            cur.push(ch);
            escaped = false;
            started = true;
            continue;
        }
        match quote {
            Some(q) if ch == q => quote = None,
            Some(_) => cur.push(ch),
            None if ch == '\\' => escaped = true,
            None if ch == '\'' || ch == '"' => {
                quote = Some(ch);
                started = true;
            }
            None if ch.is_whitespace() => {
                if started {
                    out.push(std::mem::take(&mut cur));
                    started = false;
                }
            }
            None => {
                cur.push(ch);
                started = true;
            }
        }
    }
    if escaped {
        return Err("unterminated escape in backend command".into());
    }
    if quote.is_some() {
        return Err("unterminated quote in backend command".into());
    }
    if started {
        out.push(cur);
    }
    if out.is_empty() {
        return Err("backend command cannot be empty".into());
    }
    Ok(out)
}

fn default_backend() -> String {
    if command_in_path("clang") {
        "clang".into()
    } else if command_in_path("gcc") {
        "gcc".into()
    } else if command_in_path("cc") {
        "cc".into()
    } else if command_in_path("zig") {
        "zig".into()
    } else {
        "cc".into()
    }
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
enum BackendFamily {
    Clang,
    Gcc,
    Zig,
    Unknown,
}

fn backend_family(parts: &[String]) -> BackendFamily {
    if parts.iter().any(|p| {
        Path::new(p)
            .file_name()
            .and_then(|x| x.to_str())
            .unwrap_or(p)
            .eq_ignore_ascii_case("zig")
    }) {
        return BackendFamily::Zig;
    }
    for p in parts.iter().rev() {
        let n = Path::new(p)
            .file_name()
            .and_then(|x| x.to_str())
            .unwrap_or(p)
            .to_ascii_lowercase();
        if n == "clang" || n.starts_with("clang-") || n == "clang++" {
            return BackendFamily::Clang;
        }
        if n == "gcc" || n.starts_with("gcc-") || n.ends_with("-gcc") || n == "cc" {
            return BackendFamily::Gcc;
        }
    }
    BackendFamily::Unknown
}

fn find_gcc_cross(target: &str) -> Option<String> {
    let mut candidates = vec![format!("{target}-gcc")];
    if let Some(rest) = target.strip_prefix("x86_64-unknown-") {
        candidates.push(format!("x86_64-{rest}-gcc"));
    }
    if let Some(rest) = target.strip_prefix("aarch64-unknown-") {
        candidates.push(format!("aarch64-{rest}-gcc"));
    }
    candidates.into_iter().find(|x| command_in_path(x))
}

fn pretty_command(program: &str, args: &[String]) -> String {
    let mut out = quote_arg(program);
    for arg in args {
        out.push(' ');
        out.push_str(&quote_arg(arg));
    }
    out
}

fn quote_arg(s: &str) -> String {
    if s.chars()
        .all(|c| c.is_ascii_alphanumeric() || "-_./:=+,@%".contains(c))
    {
        return s.to_string();
    }
    let escaped = s.replace('\\', "\\\\").replace('"', "\\\"");
    format!("\"{escaped}\"")
}

fn build_backend_command(
    cli: &Cli,
    c_path: &Path,
    binary: &Path,
) -> Result<(String, Vec<String>), String> {
    let requested = cli.backend.clone().unwrap_or_else(default_backend);
    let mut parts = shell_split(&requested)?;
    if parts.len() == 1
        && Path::new(&parts[0])
            .file_name()
            .and_then(|x| x.to_str())
            .map_or(false, |x| x.eq_ignore_ascii_case("zig"))
    {
        parts.push("cc".into());
    }
    let family = backend_family(&parts);
    let target = target_alias(&cli.target);

    if !matches!(target.as_str(), "native" | "host") {
        match family {
            BackendFamily::Clang => {
                parts.push(format!("--target={target}"));
            }
            BackendFamily::Zig => {
                parts.push("-target".into());
                parts.push(target.clone());
            }
            BackendFamily::Gcc => {
                if let Some(cross) = find_gcc_cross(&target) {
                    if parts.len() == 1 {
                        parts[0] = cross;
                    } else {
                        return Err(format!(
                            "GCC cross target `{target}` needs `{target}-gcc`. Try `-bcc=clang` or `-bcc=zig` for target-aware compilation."
                        ));
                    }
                } else {
                    return Err(format!(
                        "backend `{requested}` does not provide a portable `-t {target}` interface. \
                         Install `{target}-gcc`, use `-bcc=clang`, or use `-bcc=\"zig cc\"`."
                    ));
                }
            }
            BackendFamily::Unknown => {
                return Err(format!(
                    "cannot apply target `{target}` to backend `{requested}`. Use clang, gcc/cross-gcc, or zig."
                ));
            }
        }
    }

    parts.push("-std=c11".into());
    parts.push("-O3".into());
    parts.push("-DNDEBUG".into());

    if let Some(linker) = cli.linker.as_deref() {
        if linker.eq_ignore_ascii_case("lld")
            || linker.eq_ignore_ascii_case("mold")
            || linker.starts_with("ld")
            || linker.contains('/')
            || linker
                .chars()
                .all(|c| c.is_ascii_alphanumeric() || c == '-' || c == '_' || c == '.')
        {
            parts.push(format!("-fuse-ld={linker}"));
        }
    }

    for flag in &cli.cflags {
        parts.push(flag.clone());
    }
    for def in &cli.defines {
        parts.push(format!("-D{def}"));
    }
    for dir in &cli.includes {
        parts.push(format!("-I{}", dir.display()));
    }
    for dir in &cli.lib_paths {
        parts.push(format!("-L{}", dir.display()));
    }

    parts.push(c_path.to_string_lossy().into_owned());
    parts.push("-o".into());
    parts.push(binary.to_string_lossy().into_owned());

    for arg in &cli.link_args {
        parts.push(arg.clone());
    }
    for lib in &cli.libs {
        if lib.starts_with("-l") {
            parts.push(lib.clone());
        } else {
            parts.push(format!("-l{lib}"));
        }
    }

    Ok((parts.remove(0), parts))
}

fn execute_backend(theme: Theme, cli: &Cli, c_path: &Path, binary: &Path) -> io::Result<()> {
    let (program, args) = build_backend_command(cli, c_path, binary)
        .map_err(|msg| io::Error::new(io::ErrorKind::InvalidInput, msg))?;

    if cli.verbose {
        eprintln!(
            "{} {}",
            theme.paint(DIM, "backend:"),
            pretty_command(&program, &args)
        );
    }

    if cli.dry_run {
        ok(theme, format!("dry-run: backend not invoked"));
        return Ok(());
    }

    if !command_in_path(&program) {
        return Err(io::Error::new(
            io::ErrorKind::NotFound,
            format!(
                "backend `{program}` was not found in PATH\n  hint: install it or choose another backend with `-bcc=clang`, `-bcc=gcc`, or `-bcc=zig`"
            ),
        ));
    }

    let status = if cli.debug_log.is_some() {
        let mut child = Command::new(&program)
            .args(&args)
            .stdin(Stdio::inherit())
            .stdout(Stdio::piped())
            .stderr(Stdio::piped())
            .spawn()?;
        let stdout = child.stdout.take().ok_or_else(|| {
            io::Error::new(io::ErrorKind::Other, "backend stdout pipe was unavailable")
        })?;
        let stderr = child.stderr.take().ok_or_else(|| {
            io::Error::new(io::ErrorKind::Other, "backend stderr pipe was unavailable")
        })?;
        let stdout_thread = thread::spawn(move || {
            stream_backend_pipe(io::BufReader::new(stdout), "backend::stdout", false)
        });
        let stderr_thread = thread::spawn(move || {
            stream_backend_pipe(io::BufReader::new(stderr), "backend::stderr", true)
        });
        let status = child.wait()?;
        stdout_thread
            .join()
            .map_err(|_| io::Error::new(io::ErrorKind::Other, "backend stdout reader panicked"))??;
        stderr_thread
            .join()
            .map_err(|_| io::Error::new(io::ErrorKind::Other, "backend stderr reader panicked"))??;
        status
    } else {
        Command::new(&program)
            .args(&args)
            .stdin(Stdio::inherit())
            .stdout(Stdio::inherit())
            .stderr(Stdio::inherit())
            .status()?
    };

    if !status.success() {
        return Err(io::Error::new(
            io::ErrorKind::Other,
            format!("backend `{program}` failed with {status}"),
        ));
    }

    if !binary.is_file() {
        return Err(io::Error::new(
            io::ErrorKind::Other,
            format!(
                "backend exited successfully but did not produce `{}`",
                binary.display()
            ),
        ));
    }

    Ok(())
}

fn load_program(theme: Theme, path: &Path) -> io::Result<Program> {
    let mut visited = HashSet::new();
    let mut final_decls = Vec::new();
    let mut queue = VecDeque::new();

    let root_canon = path.canonicalize().unwrap_or_else(|_| path.to_path_buf());
    visited.insert(root_canon.clone());

    let src = fs::read(path)?;
    let mut src_buf = src;
    src_buf.push(0);
    src_buf.push(0);

    let p = parser::Parser::parse(&src_buf).map_err(|errs| {
        for e in &errs {
            print_error(
                theme,
                path.to_str().unwrap_or("<input>"),
                &src_buf,
                e.span,
                &e.msg,
                e.note.as_deref(),
            );
        }
        io::Error::new(io::ErrorKind::InvalidData, "parse error")
    })?;

    let base_dir = path.parent().unwrap_or(Path::new(".")).to_path_buf();
    queue.push_back((p.decls, base_dir));
    let body = p.body;

    while let Some((decls, dir)) = queue.pop_front() {
        for d in decls {
            match d {
                Decl::Bring(paths) => {
                    for import in paths {
                        let raw = Path::new(&import);
                        let abs = if raw.is_absolute() {
                            raw.to_path_buf()
                        } else {
                            dir.join(raw)
                        };
                        let canon = abs.canonicalize().unwrap_or(abs);
                        if visited.insert(canon.clone()) {
                            let sub_src = fs::read(&canon)?;
                            let mut sub_src_buf = sub_src;
                            sub_src_buf.push(0);
                            sub_src_buf.push(0);

                            let sub_p = parser::Parser::parse(&sub_src_buf).map_err(|errs| {
                                for e in &errs {
                                    print_error(
                                        theme,
                                        canon.to_str().unwrap_or("<import>"),
                                        &sub_src_buf,
                                        e.span,
                                        &e.msg,
                                        e.note.as_deref(),
                                    );
                                }
                                io::Error::new(io::ErrorKind::InvalidData, "parse error")
                            })?;

                            let sub_dir = canon.parent().unwrap_or(Path::new(".")).to_path_buf();
                            queue.push_back((sub_p.decls, sub_dir));
                        }
                    }
                }
                other => final_decls.push(other),
            }
        }
    }

    Ok(Program {
        decls: final_decls,
        body,
    })
}

fn compile(cli: Cli) -> io::Result<()> {
    let theme = Theme::new();
    if !cli.quiet {
        print_banner(theme);
    }

    let total = Instant::now();
    let input = &cli.input;

    if !input.is_file() {
        return Err(io::Error::new(
            io::ErrorKind::NotFound,
            format!(
                "source file not found: {}\n  hint: pass a `.mak` source file, e.g. `mak main.mak`",
                input.display()
            ),
        ));
    }

    let target = target_alias(&cli.target);
    let explicit_output = cli.output.is_some();
    let mut output = match &cli.output {
        Some(p) => p.clone(),
        None if cli.compile => default_base(input),
        None => derive_c_path(input),
    };

    let kind = output_kind(&output, cli.compile, explicit_output);
    if kind == OutputKind::Lir && cli.compile {
        return Err(io::Error::new(
            io::ErrorKind::InvalidInput,
            "`-cc` cannot be combined with an LIR output; choose a binary output name instead",
        ));
    }
    if kind == OutputKind::Lir {
        output = normalize_lir_path(output);
    }

    let (c_path, binary_path) = if cli.compile {
        match kind {
            OutputKind::Lir => unreachable!(),
            OutputKind::C => (output.clone(), Some(derive_binary_from_c(&output))),
            OutputKind::Binary => (derive_c_path(&output), Some(output.clone())),
        }
    } else {
        match kind {
            OutputKind::Lir => (PathBuf::new(), None),
            OutputKind::C => (output.clone(), None),
            OutputKind::Binary => (derive_c_path(&output), None),
        }
    };

    if kind == OutputKind::Lir {
        ensure_parent(&output)?;
    } else {
        ensure_parent(&c_path)?;
        if let Some(binary) = &binary_path {
            ensure_parent(binary)?;
        }
    }

    let progress_output = binary_path.as_deref().unwrap_or(&output);
    let mut progress = ProgressUi::new(
        input,
        progress_output,
        &target,
        cli.debug_log.clone(),
    );
    if let Some(ui) = progress.as_mut() {
        ui.set_stage("loading / parsing / imports", 0);
    }

    let phase_start = Instant::now();
    info(theme, format!("load {}", input.display()));
    let prog = load_program(theme, input)?;
    let load_time = phase_start.elapsed();

    if let Some(ui) = progress.as_mut() {
        ui.set_stage("semantic analysis", 1);
    }
    let sema_start = Instant::now();
    let main_src = fs::read(input).unwrap_or_default();
    let prog = match sema::analyze(prog) {
        Ok(p) => p,
        Err(errs) => {
            for e in &errs {
                print_error(
                    theme,
                    &input.display().to_string(),
                    &main_src,
                    e.span,
                    &e.msg,
                    e.note.as_deref(),
                );
            }
            return Err(io::Error::new(
                io::ErrorKind::InvalidData,
                "semantic analysis failed",
            ));
        }
    };
    let sema_time = sema_start.elapsed();

    if let Some(ui) = progress.as_mut() {
        ui.set_stage("optimizing / obfuscating", 2);
    }
    let opt_start = Instant::now();
    let prog = obfuscate::run(prog);
    let opt_time = opt_start.elapsed();

    if let Some(ui) = progress.as_mut() {
        ui.set_stage(
            if kind == OutputKind::Lir {
                "generating mLIR"
            } else {
                "generating C"
            },
            3,
        );
    }

    if cli.verbose {
        eprintln!(
            "{} load={} sema={} optimize={}",
            theme.paint(DIM, "timing:"),
            fmt_duration(load_time),
            fmt_duration(sema_time),
            fmt_duration(opt_time),
        );
    }

    if kind == OutputKind::Lir {
        let lir_start = Instant::now();
        let lir = cgen::generate_mlir(&prog);
        fs::write(&output, lir.as_bytes())?;
        let t = lir_start.elapsed();
        ok(
            theme,
            format!(
                "generated mLIR {} ({} bytes, {})",
                output.display(),
                lir.len(),
                fmt_duration(t)
            ),
        );
        if let Some(ui) = progress.as_mut() {
            ui.set_stage("mLIR written; finalizing output", 4);
            ui.finish("mLIR generated");
        }
        return Ok(());
    }

    let c_start = Instant::now();
    let c = cgen::generate(&prog);
    fs::write(&c_path, c.as_bytes())?;
    let c_time = c_start.elapsed();
    ok(
        theme,
        format!(
            "generated C {} ({} bytes, {})",
            c_path.display(),
            c.len(),
            fmt_duration(c_time)
        ),
    );

    if !cli.compile {
        if let Some(ui) = progress.as_mut() {
            ui.set_stage("C written; finalizing output", 4);
            ui.finish("C generated");
        }
        return Ok(());
    }

    let binary = binary_path.expect("compile mode always has a binary path");
    if let Some(ui) = progress.as_mut() {
        ui.set_stage("backend compile / link", 4);
    }
    let cc_start = Instant::now();
    execute_backend(theme, &cli, &c_path, &binary)?;
    let cc_time = cc_start.elapsed();

    if cli.dry_run {
        if let Some(ui) = progress.as_mut() {
            ui.finish("dry-run complete");
        }
        return Ok(());
    }

    ok(
        theme,
        format!(
            "built {} via {} ({}; target={})",
            binary.display(),
            cli.backend.as_deref().unwrap_or("auto"),
            fmt_duration(cc_time),
            target
        ),
    );

    if cli.verbose {
        let total_time = total.elapsed();
        eprintln!(
            "{} total={} source={} target={} backend={}",
            theme.paint(DIM, "summary:"),
            fmt_duration(total_time),
            input.display(),
            target,
            cli.backend.as_deref().unwrap_or("auto"),
        );
    }

    if let Some(ui) = progress.as_mut() {
        ui.finish("build complete");
    }
    Ok(())
}

fn main() {
    if let Err(err) = initialize_logger() {
        let _ = writeln!(io::stderr().lock(), "error: could not initialize debug log: {err}");
        std::process::exit(2);
    }
    let theme = Theme::new();
    match parse_cli(theme) {
        Ok(cli) => {
            if let Err(err) = compile(cli) {
                eprintln!("{}: {}", theme.paint(RED, "error"), err);
                std::process::exit(1);
            }
        }
        Err(msg) => {
            eprintln!("{}: {}\n\n{}", theme.paint(RED, "error"), msg, usage());
            std::process::exit(2);
        }
    }
}
