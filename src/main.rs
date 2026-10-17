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
use std::sync::{Mutex, OnceLock};
use std::thread;
use std::time::{Duration, Instant};

static LOGGER: OnceLock<Logger> = OnceLock::new();

struct DebugTarget {
    multi: MultiProgress,
    stage: ProgressBar,
    process: ProgressBar,
    active_section: String,
    active_iteration: usize,
    active_step: usize,
    active_total: usize,
    active_pass: String,
    active_description: String,
    source_name: String,
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
                return;
            }
            let trimmed = message.trim_start();
            if trimmed.starts_with('→')
                || trimmed.starts_with('✓')
                || trimmed.starts_with("timing:")
                || trimmed.starts_with("summary:")
            {
                return;
            }
            let _ = target.multi.println(&message);
            return;
        }
    }

    let _ = writeln!(io::stderr().lock(), "{message}");
}

fn unicode_ui() -> bool {
    if env::var_os("MAK_ASCII").is_some() {
        return false;
    }
    let locale = env::var("LC_ALL")
        .or_else(|_| env::var("LC_CTYPE"))
        .or_else(|_| env::var("LANG"))
        .unwrap_or_default()
        .to_ascii_lowercase();
    locale.is_empty() || locale.contains("utf-8") || locale.contains("utf8") || cfg!(windows)
}

fn root_prefix() -> &'static str {
    if unicode_ui() { "╭─" } else { "+-" }
}

fn stage_prefix() -> &'static str {
    if unicode_ui() { "├─" } else { "|-" }
}

fn process_prefix() -> &'static str {
    if unicode_ui() { "└─" } else { "`-" }
}

fn bar_chars() -> &'static str {
    if unicode_ui() { "━◆╌" } else { "=> " }
}

fn spinner_chars() -> &'static str {
    if unicode_ui() { "⠋⠙⠹⠸⠼⠴⠦⠧⠇⠏" } else { "|/-\\" }
}

fn spinner_style(color: &'static str) -> ProgressStyle {
    ProgressStyle::with_template(&format!("{{prefix:.bold.{color}}} {{spinner}} {{msg}} · {{elapsed_precise}}"))
        .expect("valid spinner template")
        .tick_chars(spinner_chars())
}

fn stage_bar_style() -> ProgressStyle {
    ProgressStyle::with_template("{prefix:.bold.green} [{bar:24.green}] {msg} · {elapsed_precise}")
        .expect("valid stage progress template")
        .progress_chars(bar_chars())
}

fn process_bar_style() -> ProgressStyle {
    ProgressStyle::with_template("{prefix:.bold.magenta} [{bar:24.magenta}] {pos}/{len} {msg} · {elapsed_precise}")
        .expect("valid process progress template")
        .progress_chars(bar_chars())
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
            target.stage.set_style(spinner_style("green"));
            target.stage.set_prefix(stage_prefix());
            target.stage.set_message(clip_end(message, 88));
            target.stage.enable_steady_tick(Duration::from_millis(90));
            target.process.set_style(spinner_style("magenta"));
            target.process.set_prefix(process_prefix());
            target.process.reset_elapsed();
            target.process.set_message(clip_end(message, 94));
            target.process.enable_steady_tick(Duration::from_millis(90));
        }
    }
}

pub fn optimizer_subtask(message: &str) {
    let Some(logger) = LOGGER.get() else {
        return;
    };
    if let Ok(mut target_guard) = logger.target.lock() {
        if let Some(target) = target_guard.as_mut() {
            target.active_description = message.to_string();
            target.process.set_style(spinner_style("magenta"));
            target.process.set_prefix(process_prefix());
            let text = if target.active_pass.is_empty() {
                message.to_string()
            } else {
                format!("{} · {}: {message}", target.source_name, target.active_pass)
            };
            target.process.set_message(clip_end(&text, 108));
            target.process.enable_steady_tick(Duration::from_millis(90));
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
    _changed_in_iteration: bool,
) {
    let Some(logger) = LOGGER.get() else {
        return;
    };
    if let Ok(mut target_guard) = logger.target.lock() {
        let Some(target) = target_guard.as_mut() else {
            return;
        };
        let total = total_steps.max(1);
        target.active_section = section.to_string();
        target.active_iteration = iteration;
        target.active_step = step.min(total);
        target.active_total = total;
        target.active_pass = pass.to_string();
        target.active_description = description.to_string();
        target.stage.set_style(stage_bar_style());
        target.stage.set_prefix(stage_prefix());
        target.stage.set_length(total as u64);
        target.stage.set_position(step.saturating_sub(1).min(total) as u64);
        target.stage.set_message(clip_end(&format!(
            "{section} {iteration:02} · pass {step:02}/{total:02} {pass} · {current_cost}→{best_cost} · +{accepted}/−{rejected}"
        ), 108));
        target.process.set_style(spinner_style("magenta"));
        target.process.set_prefix(process_prefix());
        target.process.reset_elapsed();
        target.process.set_message(clip_end(&format!(
            "{} · {pass}: {description} · {nodes} nodes / {functions} fn",
            target.source_name
        ), 108));
        target.process.enable_steady_tick(Duration::from_millis(90));
    }
}

pub fn optimizer_node_progress(function: &str, depth: usize, visited: usize, total: usize) {
    let Some(logger) = LOGGER.get() else {
        return;
    };
    if let Ok(mut target_guard) = logger.target.lock() {
        if let Some(target) = target_guard.as_mut() {
            if total > 0 {
                target.process.set_style(process_bar_style());
                target.process.set_prefix(process_prefix());
                target.process.set_length(total as u64);
                target.process.set_position(visited.min(total) as u64);
                target.process.set_message(clip_end(&format!(
                    "{} · {} · fn {} · depth {depth}",
                    target.active_pass,
                    target.active_description,
                    compact_label(function, 28)
                ), 104));
            } else {
                target.process.set_style(spinner_style("magenta"));
                target.process.set_prefix(process_prefix());
                target.process.set_message(clip_end(&format!(
                    "{} · {} · fn {} · depth {depth}",
                    target.active_pass,
                    target.active_description,
                    compact_label(function, 28)
                ), 104));
            }
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
            target.stage.set_position(target.active_step.min(target.active_total) as u64);
            target.stage.set_message(clip_end(&format!(
                "{} {:02} · pass {:02}/{:02} {pass} · {} · {current_cost}→{best_cost} · +{accepted}/−{rejected}",
                target.active_section,
                target.active_iteration,
                target.active_step,
                target.active_total,
                if accepted_pass { "accepted" } else { "no improvement" },
            ), 108));
            target.process.set_style(spinner_style("magenta"));
            target.process.set_prefix(process_prefix());
            target.process.set_message(clip_end(&format!(
                "{pass} · {} · {nodes} nodes",
                if accepted_pass { "candidate verified" } else { "kept best candidate" },
            ), 108));
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
            let mut routed = false;
            if let Some(logger) = LOGGER.get() {
                if let Ok(target_guard) = logger.target.lock() {
                    if let Some(target) = target_guard.as_ref() {
                        target.multi.println(text.as_str()).map_err(|err| {
                            io::Error::new(io::ErrorKind::Other, err.to_string())
                        })?;
                        routed = true;
                    }
                }
            }
            if !routed {
                let mut stdout = io::stdout().lock();
                stdout.write_all(&bytes)?;
                stdout.flush()?;
            }
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
    Three live lines show phase completion, optimizer pass progress, and active work.
    The build line advances only when a phase completes; optimizer counters come from
    measured AST traversals and accepted/rejected rewrite candidates. No ETA is guessed.
    Backend diagnostics are streamed without corrupting the display. Use -v for the exact
    backend command and phase timings. Set MAK_ASCII=1 for ASCII-only progress glyphs.
    --debug-log captures internal stderr and backend stdout/stderr with elapsed timestamps.
"#
}

fn print_banner(theme: Theme) {
    let title = theme.paint(BOLD, "◆ MAK");
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
    stage: ProgressBar,
    process: ProgressBar,
    input: PathBuf,
    output: PathBuf,
    target: String,
    debug_log: Option<PathBuf>,
    total_stages: u64,
    finished: bool,
}

impl ProgressUi {
    fn new(
        input: &Path,
        output: &Path,
        target: &str,
        debug_log: Option<PathBuf>,
        total_stages: u64,
    ) -> Option<Self> {
        if !io::stderr().is_terminal() {
            return None;
        }

        let multi = MultiProgress::new();
        let overall = multi.add(ProgressBar::new(total_stages.max(1)));
        overall.set_style(
            ProgressStyle::with_template("{prefix:.bold.cyan} [{bar:24.cyan}] {pos}/{len} {msg} · {elapsed_precise}")
                .expect("valid overall progress template")
                .progress_chars(bar_chars()),
        );
        overall.set_prefix(root_prefix());
        overall.enable_steady_tick(Duration::from_millis(100));

        let stage = multi.add(ProgressBar::new_spinner());
        stage.set_style(spinner_style("green"));
        stage.set_prefix(stage_prefix());
        stage.set_message("preparing");
        stage.enable_steady_tick(Duration::from_millis(90));

        let process = multi.add(ProgressBar::new_spinner());
        process.set_style(spinner_style("magenta"));
        process.set_prefix(process_prefix());
        process.set_message("starting");
        process.enable_steady_tick(Duration::from_millis(90));

        if let Some(logger) = LOGGER.get() {
            if let Ok(mut target_guard) = logger.target.lock() {
                *target_guard = Some(DebugTarget {
                    multi: multi.clone(),
                    stage: stage.clone(),
                    process: process.clone(),
                    active_section: String::new(),
                    active_iteration: 0,
                    active_step: 0,
                    active_total: 1,
                    active_pass: String::new(),
                    active_description: String::new(),
                    source_name: input.display().to_string(),
                });
            }
        }

        let mut ui = Self {
            overall,
            stage,
            process,
            input: input.to_path_buf(),
            output: output.to_path_buf(),
            target: target.to_string(),
            debug_log,
            total_stages: total_stages.max(1),
            finished: false,
        };
        ui.set_stage("resolve input", 0);
        Some(ui)
    }

    fn set_stage(&mut self, stage: &str, completed_stages: u64) {
        let input = compact_label(&self.input.display().to_string(), 22);
        let output = compact_label(&self.output.display().to_string(), 22);
        let target = compact_label(&self.target, 10);
        let log = self.debug_log.as_ref().map(|p| {
            format!("log {}", compact_label(&p.display().to_string(), 18))
        }).unwrap_or_else(|| "log off".to_string());
        self.overall.set_length(self.total_stages);
        self.overall.set_position(completed_stages.min(self.total_stages));
        self.overall.set_message(clip_end(&format!(
            "{input} → {output} · {} · {target} · {log}",
            stage.to_ascii_uppercase()
        ), 100));

        self.stage.set_style(spinner_style("green"));
        self.stage.set_prefix(stage_prefix());
        self.stage.set_length(1);
        self.stage.set_position(0);
        self.stage.set_message(stage.to_string());
        self.stage.enable_steady_tick(Duration::from_millis(90));

        self.process.set_style(spinner_style("magenta"));
        self.process.set_prefix(process_prefix());
        self.process.reset_elapsed();
        self.process.set_message(stage.to_string());
        self.process.enable_steady_tick(Duration::from_millis(90));

        if let Some(logger) = LOGGER.get() {
            if let Ok(mut target_guard) = logger.target.lock() {
                if let Some(target) = target_guard.as_mut() {
                    target.active_section.clear();
                    target.active_pass.clear();
                    target.active_description = stage.to_string();
                }
            }
        }
    }

    fn set_process(&self, message: impl AsRef<str>) {
        self.process.set_style(spinner_style("magenta"));
        self.process.set_prefix(process_prefix());
        self.process.set_message(clip_end(message.as_ref(), 112));
        self.process.enable_steady_tick(Duration::from_millis(90));
    }


    fn finish(&mut self, message: &str) {
        if self.finished {
            return;
        }
        self.overall.set_length(self.total_stages);
        self.overall.set_position(self.total_stages);
        self.overall.set_message(clip_end(&format!(
            "{} → {} · complete",
            compact_label(&self.input.display().to_string(), 22),
            compact_label(&self.output.display().to_string(), 22),
        ), 100));
        self.overall.finish();
        self.stage.set_style(
            ProgressStyle::with_template("{prefix:.bold.green} {msg}")
                .expect("valid final stage template"),
        );
        self.stage.finish_with_message(if unicode_ui() { "✓ complete" } else { "complete" });
        self.process.set_style(
            ProgressStyle::with_template("{prefix:.bold.magenta} {msg}")
                .expect("valid final process template"),
        );
        self.process.finish_with_message(clip_end(message, 104));
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
        if !self.finished {
            self.overall.abandon_with_message("failed");
            self.stage.set_style(
                ProgressStyle::with_template("{prefix:.bold.green} {msg}")
                    .expect("valid interrupted stage template"),
            );
            self.stage.abandon_with_message("failed");
            self.process.set_style(
                ProgressStyle::with_template("{prefix:.bold.magenta} {msg}")
                    .expect("valid interrupted process template"),
            );
            self.process.abandon_with_message("failed");
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

fn execute_backend(
    theme: Theme,
    cli: &Cli,
    c_path: &Path,
    binary: &Path,
    progress: Option<&ProgressUi>,
) -> io::Result<String> {
    let (program, args) = build_backend_command(cli, c_path, binary)
        .map_err(|msg| io::Error::new(io::ErrorKind::InvalidInput, msg))?;

    if let Some(ui) = progress {
        let target = target_alias(&cli.target);
        ui.set_process(format!(
            "driver {} · compile C + link binary · -std=c11 -O3 · target {} · {} argument(s)",
            Path::new(&program).file_name().and_then(|name| name.to_str()).unwrap_or(&program),
            target,
            args.len()
        ));
    }

    if cli.verbose {
        eprintln!(
            "{} {}",
            theme.paint(DIM, "backend:"),
            pretty_command(&program, &args)
        );
    }

    if cli.dry_run {
        if !cli.quiet {
            ok(theme, "dry-run: backend not invoked");
        }
        return Ok(program);
    }

    if !command_in_path(&program) {
        return Err(io::Error::new(
            io::ErrorKind::NotFound,
            format!(
                "backend `{program}` was not found in PATH\n  hint: install it or choose another backend with `-bcc=clang`, `-bcc=gcc`, or `-bcc=zig`"
            ),
        ));
    }

    let capture_backend_output = progress.is_some() || cli.debug_log.is_some();
    let status = if capture_backend_output {
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

    Ok(program)
}

#[derive(Default)]
struct LoadStats {
    files_parsed: usize,
    source_bytes: u64,
    import_requests: usize,
    duplicate_imports: usize,
}

#[derive(Default, Clone, Copy)]
struct AstMetrics {
    declarations: usize,
    type_declarations: usize,
    constructors: usize,
    externals: usize,
    expressions: usize,
    function_expressions: usize,
    max_depth: usize,
}

fn count_expr_metrics(expr: &Expr, depth: usize, metrics: &mut AstMetrics) {
    metrics.expressions = metrics.expressions.saturating_add(1);
    metrics.max_depth = metrics.max_depth.max(depth);
    match &expr.kind {
        ExprKind::Let { value, body, .. } => {
            count_expr_metrics(value, depth + 1, metrics);
            count_expr_metrics(body, depth + 1, metrics);
        }
        ExprKind::Fn { body, .. } => {
            metrics.function_expressions = metrics.function_expressions.saturating_add(1);
            count_expr_metrics(body, depth + 1, metrics);
        }
        ExprKind::App { fun, args } => {
            count_expr_metrics(fun, depth + 1, metrics);
            for arg in args {
                count_expr_metrics(arg, depth + 1, metrics);
            }
        }
        ExprKind::Case { scrut, arms } => {
            count_expr_metrics(scrut, depth + 1, metrics);
            for arm in arms {
                count_expr_metrics(&arm.body, depth + 1, metrics);
            }
        }
        ExprKind::Block { stmts, tail } => {
            for stmt in stmts {
                count_expr_metrics(stmt, depth + 1, metrics);
            }
            count_expr_metrics(tail, depth + 1, metrics);
        }
        ExprKind::Bin { l, r, .. } => {
            count_expr_metrics(l, depth + 1, metrics);
            count_expr_metrics(r, depth + 1, metrics);
        }
        ExprKind::Un { e, .. } | ExprKind::Quote(e) => {
            count_expr_metrics(e, depth + 1, metrics);
        }
        ExprKind::Int(_) | ExprKind::Float(_) | ExprKind::Str(_) | ExprKind::Var(_) => {}
    }
}

fn ast_metrics(program: &Program) -> AstMetrics {
    let mut metrics = AstMetrics {
        declarations: program.decls.len(),
        ..AstMetrics::default()
    };
    for decl in &program.decls {
        match decl {
            Decl::Type { ctors, .. } => {
                metrics.type_declarations = metrics.type_declarations.saturating_add(1);
                metrics.constructors = metrics.constructors.saturating_add(ctors.len());
            }
            Decl::Let { value, .. } => count_expr_metrics(value, 1, &mut metrics),
            Decl::External(_) => metrics.externals = metrics.externals.saturating_add(1),
            Decl::Bring(_) => {}
        }
    }
    count_expr_metrics(&program.body, 1, &mut metrics);
    metrics
}

fn human_bytes(bytes: u64) -> String {
    const UNITS: [&str; 5] = ["B", "KiB", "MiB", "GiB", "TiB"];
    let mut value = bytes as f64;
    let mut unit = 0usize;
    while value >= 1024.0 && unit + 1 < UNITS.len() {
        value /= 1024.0;
        unit += 1;
    }
    if unit == 0 {
        format!("{} B", bytes)
    } else {
        format!("{value:.1} {}", UNITS[unit])
    }
}

fn verify_artifact(path: &Path, description: &str) -> io::Result<u64> {
    let metadata = fs::metadata(path).map_err(|err| {
        io::Error::new(
            err.kind(),
            format!("{description} was not produced at `{}`: {err}", path.display()),
        )
    })?;
    if !metadata.is_file() {
        return Err(io::Error::new(
            io::ErrorKind::InvalidData,
            format!("{description} path is not a regular file: `{}`", path.display()),
        ));
    }
    if metadata.len() == 0 {
        return Err(io::Error::new(
            io::ErrorKind::InvalidData,
            format!("{description} is empty: `{}`", path.display()),
        ));
    }
    Ok(metadata.len())
}

fn load_program(
    theme: Theme,
    path: &Path,
    stats: &mut LoadStats,
    progress: Option<&ProgressUi>,
) -> io::Result<Program> {
    let mut visited = HashSet::new();
    let mut final_decls = Vec::new();
    let mut queue = VecDeque::new();

    let root_canon = path.canonicalize().unwrap_or_else(|_| path.to_path_buf());
    visited.insert(root_canon.clone());

    if let Some(ui) = progress {
        ui.set_process(format!("read source · {}", path.display()));
    }
    let src = fs::read(path)?;
    stats.files_parsed = stats.files_parsed.saturating_add(1);
    stats.source_bytes = stats.source_bytes.saturating_add(src.len() as u64);
    let root_bytes = src.len();
    let mut src_buf = src;
    src_buf.push(0);
    src_buf.push(0);
    if let Some(ui) = progress {
        ui.set_process(format!("lex + parse · {} · {}", path.display(), human_bytes(root_bytes as u64)));
    }

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
                        stats.import_requests = stats.import_requests.saturating_add(1);
                        let raw = Path::new(&import);
                        let abs = if raw.is_absolute() {
                            raw.to_path_buf()
                        } else {
                            dir.join(raw)
                        };
                        let canon = abs.canonicalize().unwrap_or(abs);
                        if visited.insert(canon.clone()) {
                            if let Some(ui) = progress {
                                ui.set_process(format!(
                                    "resolve import · file {} · {}",
                                    stats.files_parsed.saturating_add(1),
                                    canon.display()
                                ));
                            }
                            let sub_src = fs::read(&canon).map_err(|err| {
                                io::Error::new(
                                    err.kind(),
                                    format!("failed to read imported source `{}`: {err}", canon.display()),
                                )
                            })?;
                            stats.files_parsed = stats.files_parsed.saturating_add(1);
                            stats.source_bytes = stats.source_bytes.saturating_add(sub_src.len() as u64);
                            let sub_bytes = sub_src.len();
                            let mut sub_src_buf = sub_src;
                            sub_src_buf.push(0);
                            sub_src_buf.push(0);
                            if let Some(ui) = progress {
                                ui.set_process(format!(
                                    "lex + parse import · {} · {}",
                                    canon.display(),
                                    human_bytes(sub_bytes as u64)
                                ));
                            }

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
                        } else {
                            stats.duplicate_imports = stats.duplicate_imports.saturating_add(1);
                        }
                    }
                }
                other => final_decls.push(other),
            }
        }
    }

    let program = Program {
        decls: final_decls,
        body,
    };
    if let Some(ui) = progress {
        ui.set_process(format!(
            "import graph ready · {} file(s) · {} edge(s) · {} repeated edge(s)",
            stats.files_parsed,
            stats.import_requests,
            stats.duplicate_imports
        ));
    }
    Ok(program)
}

fn compile(cli: Cli) -> io::Result<()> {
    let theme = Theme::new();
    if !cli.quiet && !io::stderr().is_terminal() {
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
        Some(path) => path.clone(),
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
    let total_stages = if cli.compile && !cli.dry_run { 6 } else { 5 };
    let mut progress = if cli.quiet {
        None
    } else {
        ProgressUi::new(input, progress_output, &target, cli.debug_log.clone(), total_stages)
    };

    if let Some(ui) = progress.as_mut() {
        ui.set_stage("read · lex · parse · resolve imports", 0);
        ui.set_process(format!("open source · {}", input.display()));
    }

    let load_start = Instant::now();
    if !cli.quiet {
        info(theme, format!("load {}", input.display()));
    }
    let mut load_stats = LoadStats::default();
    let loaded_program = load_program(
        theme,
        input,
        &mut load_stats,
        progress.as_ref(),
    )?;
    let load_time = load_start.elapsed();
    let loaded_metrics = ast_metrics(&loaded_program);
    if let Some(ui) = progress.as_ref() {
        ui.set_process(format!(
            "parsed {} file(s) · {} source · {} import edge(s) / {} repeated · {} declarations · {} expressions",
            load_stats.files_parsed,
            human_bytes(load_stats.source_bytes),
            load_stats.import_requests,
            load_stats.duplicate_imports,
            loaded_metrics.declarations,
            loaded_metrics.expressions
        ));
    }

    if let Some(ui) = progress.as_mut() {
        ui.set_stage("semantic analysis · reachability + validation", 1);
        ui.set_process(format!(
            "analyze declarations · {} decl · {} type · {} extern · {} ctor",
            loaded_metrics.declarations,
            loaded_metrics.type_declarations,
            loaded_metrics.externals,
            loaded_metrics.constructors
        ));
    }
    let sema_start = Instant::now();
    let main_src = fs::read(input).unwrap_or_default();
    let program = match sema::analyze(loaded_program) {
        Ok(program) => program,
        Err(errors) => {
            for error in &errors {
                print_error(
                    theme,
                    &input.display().to_string(),
                    &main_src,
                    error.span,
                    &error.msg,
                    error.note.as_deref(),
                );
            }
            return Err(io::Error::new(
                io::ErrorKind::InvalidData,
                format!("semantic analysis failed with {} error(s)", errors.len()),
            ));
        }
    };
    let sema_time = sema_start.elapsed();
    let sema_metrics = ast_metrics(&program);
    if let Some(ui) = progress.as_ref() {
        ui.set_process(format!(
            "validated · {} declarations remain · {} expressions · {} functions",
            sema_metrics.declarations,
            sema_metrics.expressions,
            sema_metrics.function_expressions
        ));
    }

    if let Some(ui) = progress.as_mut() {
        ui.set_stage("optimization · whole-program + rewrite search", 2);
        ui.set_process(format!(
            "input IR · {} expressions · {} functions · max depth {}",
            sema_metrics.expressions,
            sema_metrics.function_expressions,
            sema_metrics.max_depth
        ));
    }
    let opt_start = Instant::now();
    let program = obfuscate::run(program);
    let opt_time = opt_start.elapsed();
    let optimized_metrics = ast_metrics(&program);
    if let Some(ui) = progress.as_ref() {
        ui.set_process(format!(
            "optimized IR · {} expressions · {} functions · depth {} · {:+} expressions",
            optimized_metrics.expressions,
            optimized_metrics.function_expressions,
            optimized_metrics.max_depth,
            optimized_metrics.expressions as i64 - sema_metrics.expressions as i64
        ));
    }

    if let Some(ui) = progress.as_mut() {
        ui.set_stage(
            if kind == OutputKind::Lir { "lower + optimize IR · emit mLIR" } else { "lower + optimize IR · emit C" },
            3,
        );
        ui.set_process(format!(
            "codegen · register symbols · analyze reps · lower · optimize IR · emit {}",
            if kind == OutputKind::Lir { "mLIR" } else { "C11" }
        ));
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
        let codegen_start = Instant::now();
        let lir = cgen::generate_mlir(&program);
        fs::write(&output, lir.as_bytes())?;
        let codegen_time = codegen_start.elapsed();
        if let Some(ui) = progress.as_ref() {
            ui.set_process(format!("mLIR emitted · {} · {}", output.display(), human_bytes(lir.len() as u64)));
        }
        if let Some(ui) = progress.as_mut() {
            ui.set_stage("verify generated mLIR artifact", 4);
            ui.set_process(format!("validate file · {}", output.display()));
        }
        let output_bytes = verify_artifact(&output, "mLIR output")?;
        if !cli.quiet {
            ok(
                theme,
                format!(
                    "generated mLIR {} ({}; {})",
                    output.display(),
                    human_bytes(output_bytes),
                    fmt_duration(codegen_time)
                ),
            );
        }
        if cli.verbose {
            eprintln!(
                "{} output={} bytes={} total={}",
                theme.paint(DIM, "summary:"),
                output.display(),
                output_bytes,
                fmt_duration(total.elapsed())
            );
        }
        if let Some(ui) = progress.as_mut() {
            ui.finish(&format!("mLIR verified · {} · {}", output.display(), human_bytes(output_bytes)));
        }
        return Ok(());
    }

    let cgen_start = Instant::now();
    let c_source = cgen::generate(&program);
    fs::write(&c_path, c_source.as_bytes())?;
    let cgen_time = cgen_start.elapsed();
    let generated_c_bytes = c_source.len() as u64;
    if let Some(ui) = progress.as_ref() {
        ui.set_process(format!(
            "C11 emitted · {} · {} · {} line(s)",
            c_path.display(),
            human_bytes(generated_c_bytes),
            c_source.lines().count()
        ));
    }
    if !cli.quiet {
        ok(
            theme,
            format!(
                "generated C {} ({}; {})",
                c_path.display(),
                human_bytes(generated_c_bytes),
                fmt_duration(cgen_time)
            ),
        );
    }

    if !cli.compile {
        if let Some(ui) = progress.as_mut() {
            ui.set_stage("verify generated C artifact", 4);
            ui.set_process(format!("validate file · {}", c_path.display()));
        }
        let c_bytes = verify_artifact(&c_path, "generated C source")?;
        if let Some(ui) = progress.as_mut() {
            ui.finish(&format!("C verified · {} · {}", c_path.display(), human_bytes(c_bytes)));
        }
        if cli.verbose {
            eprintln!(
                "{} load={} sema={} optimize={} codegen={} total={}",
                theme.paint(DIM, "summary:"),
                fmt_duration(load_time),
                fmt_duration(sema_time),
                fmt_duration(opt_time),
                fmt_duration(cgen_time),
                fmt_duration(total.elapsed())
            );
        }
        return Ok(());
    }

    let c_bytes = verify_artifact(&c_path, "generated C source")?;

    let binary = binary_path.expect("compile mode always has a binary path");
    if let Some(ui) = progress.as_mut() {
        ui.set_stage(if cli.dry_run { "backend command · dry run" } else { "backend compiler · compile C + link" }, 4);
        ui.set_process(format!(
            "prepare backend · C {} ({}) · output {} · target {}",
            c_path.display(),
            human_bytes(c_bytes),
            binary.display(),
            target
        ));
    }
    let backend_start = Instant::now();
    let backend_program = execute_backend(theme, &cli, &c_path, &binary, progress.as_ref())?;
    let backend_time = backend_start.elapsed();
    let backend_name = Path::new(&backend_program)
        .file_name()
        .and_then(|name| name.to_str())
        .unwrap_or(&backend_program)
        .to_string();

    if cli.dry_run {
        if let Some(ui) = progress.as_mut() {
            ui.finish(&format!("dry run · {} command prepared · {}", backend_name, fmt_duration(backend_time)));
        }
        return Ok(());
    }

    if let Some(ui) = progress.as_mut() {
        ui.set_stage("verify linked executable", 5);
        ui.set_process(format!("inspect linker output · {}", binary.display()));
    }
    let binary_bytes = verify_artifact(&binary, "linked executable")?;
    let total_time = total.elapsed();
    if !cli.quiet {
        ok(theme, format!(
            "built {} ({}; backend {}; compile/link {}; total {})",
            binary.display(),
            human_bytes(binary_bytes),
            backend_name,
            fmt_duration(backend_time),
            fmt_duration(total_time)
        ));
    }

    if cli.verbose {
        eprintln!(
            concat!(
                "{} total={} load={} sema={} optimize={} codegen={} backend={} ",
                "source_files={} source_bytes={} input_exprs={} optimized_exprs={} ",
                "functions={} output_bytes={} target={} backend={}"
            ),
            theme.paint(DIM, "summary:"),
            fmt_duration(total_time),
            fmt_duration(load_time),
            fmt_duration(sema_time),
            fmt_duration(opt_time),
            fmt_duration(cgen_time),
            fmt_duration(backend_time),
            load_stats.files_parsed,
            load_stats.source_bytes,
            sema_metrics.expressions,
            optimized_metrics.expressions,
            optimized_metrics.function_expressions,
            binary_bytes,
            target,
            backend_name,
        );
    }

    if let Some(ui) = progress.as_mut() {
        ui.finish(&format!(
            "linked + verified · {} · {} · {}",
            binary.display(),
            human_bytes(binary_bytes),
            fmt_duration(total_time)
        ));
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
