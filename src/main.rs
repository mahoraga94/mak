mod ast;
mod cgen;
mod lexer;
mod obfuscate;
mod parser;
mod sema;

use crate::lexer::Span;
use ast::*;
use std::collections::{HashSet, VecDeque};
use std::env;
use std::fs;
use std::io::{self, IsTerminal};
use std::path::{Path, PathBuf};
use std::process::{Command, Stdio};
use std::time::{Duration, Instant};

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
    -v, --verbose             Print compiler phases and the exact backend command.
    -q, --quiet               Suppress normal success messages.
        --dry-run             Do everything except invoke the backend compiler.
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

    mak main.mak -o optimized.mLIR
        → optimized MAK LIR
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

    if matches!(target.as_str(), "native" | "host") {
        // no target flag
    } else {
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

    let status = Command::new(&program)
        .args(&args)
        .stdin(Stdio::inherit())
        .stdout(Stdio::inherit())
        .stderr(Stdio::inherit())
        .status()?;

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

    let phase_start = Instant::now();
    info(theme, format!("load {}", input.display()));
    let prog = load_program(theme, input)?;
    let load_time = phase_start.elapsed();

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

    let opt_start = Instant::now();
    // println!("+=+=+++\nast being  sent to obfuscate:\n{:?}",&prog);

    let prog = obfuscate::run(prog);
    // println!("ast being sent  to cgen:\n{:?}\n+=+=+++\n",&prog);
    let opt_time = opt_start.elapsed();

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
        return Ok(());
    }

    let binary = binary_path.expect("compile mode always has a binary path");
    let cc_start = Instant::now();
    execute_backend(theme, &cli, &c_path, &binary)?;
    let cc_time = cc_start.elapsed();

    if cli.dry_run {
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

    Ok(())
}

fn main() {
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
