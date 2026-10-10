
use crate::ast::*;
use crate::lexer::Span;
use std::collections::{HashMap, HashSet};

/// Internal names emitted by the parser for syntax that does not have
/// its own AST node. These are compiler intrinsics, not user variables.
const BUILTIN_INTRINSICS: &[&str] = &["$mak.index"];

#[inline]
fn is_builtin_intrinsic(name: &str) -> bool {
    BUILTIN_INTRINSICS.contains(&name)
}

/// Enable semantic-analysis tracing by setting MAK_DEBUG_SEMA=1.
///
/// Tracing is disabled by default. Values such as 0, false, off, and no
/// leave it disabled.
fn sema_debug_enabled() -> bool {
    std::env::var("MAK_DEBUG_SEMA")
        .map(|value| {
            let value = value.trim();
            !value.is_empty()
                && !matches!(
                    value.to_ascii_lowercase().as_str(),
                    "0" | "false" | "off" | "no"
                )
        })
        .unwrap_or(false)
}

#[inline]
fn sema_debug(enabled: bool, message: impl std::fmt::Display) {
    if enabled {
        eprintln!("[sema] {message}");
    }
}

fn decl_label(decl: &Decl) -> String {
    match decl {
        Decl::Let { name, .. } => format!("let {name}"),
        Decl::External(Extern { name, .. }) => format!("external {name}"),
        Decl::Type { name, .. } => format!("type {name}"),
        Decl::Bring(paths) => format!("bring {} path(s)", paths.len()),
    }
}

#[derive(Debug, Clone)]
pub struct SemaErr {
    pub msg: String,
    pub span: Span,
    pub note: Option<String>,
}

fn levenshtein(a: &str, b: &str) -> usize {
    let mut d = vec![vec![0; b.len() + 1]; a.len() + 1];

    for i in 0..=a.len() {
        d[i][0] = i;
    }

    for j in 0..=b.len() {
        d[0][j] = j;
    }

    for i in 1..=a.len() {
        for j in 1..=b.len() {
            let cost = if a.as_bytes()[i - 1] == b.as_bytes()[j - 1] {
                0
            } else {
                1
            };

            d[i][j] = (d[i - 1][j] + 1)
                .min(d[i][j - 1] + 1)
                .min(d[i - 1][j - 1] + cost);
        }
    }

    d[a.len()][b.len()]
}

pub fn analyze(mut p: Program) -> Result<Program, Vec<SemaErr>> {
    let debug = sema_debug_enabled();
    let mut errors = Vec::new();

    sema_debug(
        debug,
        format!(
            "starting analysis: {} top-level declaration(s), body span {}..{}",
            p.decls.len(),
            p.body.span.lo,
            p.body.span.hi
        ),
    );

    if debug {
        for (index, decl) in p.decls.iter().enumerate() {
            eprintln!(
                "[sema] input declaration #{index}: {}",
                decl_label(decl)
            );
        }
    }

    // =====================================================================
    // PHASE 1: Reachability
    //
    // Remove unused top-level declarations while preserving the declarations
    // referenced by the main expression and their dependencies.
    //
    // IMPORTANT:
    // Parser-generated intrinsics such as $mak.index do not name declarations.
    // They must not become roots in the declaration-reachability analysis.
    // =====================================================================

    let mut used = HashSet::new();
    let mut top_level: HashMap<String, &Decl> = HashMap::new();

    for decl in &p.decls {
        match decl {
            Decl::Let { name, .. } | Decl::External(Extern { name, .. }) => {
                top_level.insert(name.clone(), decl);
            }

            Decl::Type { name, ctors, .. } => {
                top_level.insert(name.clone(), decl);

                for ctor in ctors {
                    top_level.insert(ctor.name.clone(), decl);
                }
            }

            Decl::Bring(_) => {}
        }
    }

    fn walk_expr_used(expr: &Expr, used: &mut HashSet<String>) {
        match &expr.kind {
            ExprKind::Var(name) => {
                // Intrinsics are handled by the compiler and do not correspond
                // to source-level declarations that can be retained or removed.
                if !is_builtin_intrinsic(name) {
                    used.insert(name.clone());
                }
            }

            ExprKind::Let { value, body, .. } => {
                walk_expr_used(value, used);
                walk_expr_used(body, used);
            }

            ExprKind::Fn { body, .. } => {
                walk_expr_used(body, used);
            }

            ExprKind::App { fun, args } => {
                walk_expr_used(fun, used);

                for arg in args {
                    walk_expr_used(arg, used);
                }
            }

            ExprKind::Case { scrut, arms } => {
                walk_expr_used(scrut, used);

                for arm in arms {
                    walk_expr_used(&arm.body, used);
                }
            }

            ExprKind::Block { stmts, tail } => {
                for stmt in stmts {
                    walk_expr_used(stmt, used);
                }

                walk_expr_used(tail, used);
            }

            ExprKind::Bin { l, r, .. } => {
                walk_expr_used(l, used);
                walk_expr_used(r, used);
            }

            ExprKind::Un { e, .. } | ExprKind::Quote(e) => {
                walk_expr_used(e, used);
            }

            ExprKind::Int(_) | ExprKind::Float(_) | ExprKind::Str(_) => {}
        }
    }

    walk_expr_used(&p.body, &mut used);

    if debug {
        let mut names: Vec<String> = used.iter().cloned().collect();
        names.sort();

        sema_debug(debug, format!("initial reachable names: {names:?}"));
    }

    // Follow dependencies of reachable top-level let bindings until no new
    // names are discovered.
    let mut changed = true;

    while changed {
        changed = false;

        let currently_used: Vec<String> = used.iter().cloned().collect();

        for name in currently_used {
            if let Some(decl) = top_level.get(&name) {
                if let Decl::Let { value, .. } = decl {
                    let before = used.len();

                    walk_expr_used(value, &mut used);

                    if used.len() > before {
                        changed = true;
                    }
                }
            }
        }
    }

    let before_reachability = p.decls.len();

    p.decls.retain(|decl| match decl {
        Decl::Let { name, .. } | Decl::External(Extern { name, .. }) => {
            used.contains(name)
        }

        Decl::Type { name, ctors, .. } => {
            used.contains(name)
                || ctors.iter().any(|ctor| used.contains(&ctor.name))
        }

        // Keep bring declarations, because they may affect how the source
        // program is loaded or interpreted.
        Decl::Bring(_) => true,
    });

    if debug {
        sema_debug(
            debug,
            format!(
                "reachability retained {} of {} declaration(s)",
                p.decls.len(),
                before_reachability
            ),
        );

        for decl in &p.decls {
            eprintln!("[sema] retained: {}", decl_label(decl));
        }

        let mut names: Vec<String> = used.iter().cloned().collect();
        names.sort();

        sema_debug(debug, format!("final reachable names: {names:?}"));
    }

    // =====================================================================
    // PHASE 2: Build the set of globally available source-level names.
    // =====================================================================

    let mut globals = HashSet::new();

    for decl in &p.decls {
        match decl {
            Decl::Let { name, .. } | Decl::External(Extern { name, .. }) => {
                globals.insert(name.clone());
            }

            Decl::Type { ctors, .. } => {
                for ctor in ctors {
                    globals.insert(ctor.name.clone());
                }
            }

            Decl::Bring(_) => {}
        }
    }

    if debug {
        let mut names: Vec<String> = globals.iter().cloned().collect();
        names.sort();

        sema_debug(debug, format!("global names: {names:?}"));
    }

    // =====================================================================
    // PHASE 3: Scope checking
    //
    // Report truly undefined variables. Recognize compiler intrinsics as
    // built-ins, without making arbitrary unknown names valid.
    // =====================================================================

    fn check_expr(
        expr: &Expr,
        scope: &mut Vec<HashSet<String>>,
        globals: &HashSet<String>,
        errors: &mut Vec<SemaErr>,
        debug: bool,
    ) {
        match &expr.kind {
            ExprKind::Var(name) => {
                let is_local = scope.iter().rev().any(|s| s.contains(name));
                let is_global = globals.contains(name);
                let is_builtin = is_builtin_intrinsic(name);

                let in_scope = is_local || is_global || is_builtin;

                if debug {
                    let resolution = if is_local {
                        "local"
                    } else if is_global {
                        "global"
                    } else if is_builtin {
                        "built-in intrinsic"
                    } else {
                        "UNDEFINED"
                    };

                    eprintln!(
                        "[sema] resolve `{name}` at {}..{} => {resolution}",
                        expr.span.lo,
                        expr.span.hi
                    );
                }

                if !in_scope {
                    let mut suggestions: Vec<String> = globals
                        .iter()
                        .filter(|candidate| {
                            let distance = levenshtein(name, candidate);
                            distance > 0 && distance <= 2
                        })
                        .cloned()
                        .collect();

                    suggestions.sort_by_key(|candidate| levenshtein(name, candidate));

                    let note = if suggestions.is_empty() {
                        Some(
                            "if this is a function, ensure it is defined or imported via `bring`"
                                .into(),
                        )
                    } else {
                        Some(format!("did you mean `{}`?", suggestions[0]))
                    };

                    errors.push(SemaErr {
                        msg: format!("undefined variable `{name}`"),
                        span: expr.span,
                        note,
                    });
                }
            }

            ExprKind::Let { name, value, body } => {
                // A local let value cannot see the name being introduced.
                check_expr(value, scope, globals, errors, debug);

                scope.last_mut().unwrap().insert(name.clone());

                check_expr(body, scope, globals, errors, debug);

                scope.last_mut().unwrap().remove(name);
            }

            ExprKind::Fn { params, body } => {
                scope.push(HashSet::new());

                for param in params {
                    scope.last_mut().unwrap().insert(param.clone());
                }

                check_expr(body, scope, globals, errors, debug);

                scope.pop();
            }

            ExprKind::App { fun, args } => {
                check_expr(fun, scope, globals, errors, debug);

                for arg in args {
                    check_expr(arg, scope, globals, errors, debug);
                }
            }

            ExprKind::Case { scrut, arms } => {
                check_expr(scrut, scope, globals, errors, debug);

                for arm in arms {
                    scope.push(HashSet::new());

                    collect_pat_vars(&arm.pat, scope.last_mut().unwrap());

                    check_expr(&arm.body, scope, globals, errors, debug);

                    scope.pop();
                }
            }

            ExprKind::Block { stmts, tail } => {
                scope.push(HashSet::new());

                for stmt in stmts {
                    check_expr(stmt, scope, globals, errors, debug);

                    // Block-local let declarations become visible to following
                    // statements, not to their own initializers.
                    if let ExprKind::Let { name, .. } = &stmt.kind {
                        scope.last_mut().unwrap().insert(name.clone());
                    }
                }

                check_expr(tail, scope, globals, errors, debug);

                scope.pop();
            }

            ExprKind::Bin { l, r, .. } => {
                check_expr(l, scope, globals, errors, debug);
                check_expr(r, scope, globals, errors, debug);
            }

            ExprKind::Un { e, .. } | ExprKind::Quote(e) => {
                check_expr(e, scope, globals, errors, debug);
            }

            ExprKind::Int(_) | ExprKind::Float(_) | ExprKind::Str(_) => {}
        }
    }

    fn collect_pat_vars(pattern: &Pat, out: &mut HashSet<String>) {
        match pattern {
            Pat::Bind(name) => {
                out.insert(name.clone());
            }

            Pat::Ctor { args, .. } | Pat::Tuple(args) => {
                for arg in args {
                    collect_pat_vars(arg, out);
                }
            }

            Pat::Wild | Pat::Int(_) | Pat::Str(_) => {}
        }
    }

    // Check top-level let expressions. External declarations contain type
    // metadata rather than a MAK expression body, so they need no scope walk.
    for decl in &p.decls {
        if let Decl::Let { value, .. } = decl {
            let mut scope = vec![HashSet::new()];

            if debug {
                sema_debug(
                    debug,
                    format!(
                        "checking top-level let value at {}..{}",
                        value.span.lo, value.span.hi
                    ),
                );
            }

            check_expr(value, &mut scope, &globals, &mut errors, debug);
        }
    }

    let mut scope = vec![HashSet::new()];

    sema_debug(
        debug,
        format!(
            "checking program body at {}..{}",
            p.body.span.lo, p.body.span.hi
        ),
    );

    check_expr(&p.body, &mut scope, &globals, &mut errors, debug);

    if debug {
        sema_debug(
            debug,
            format!("scope check found {} error(s)", errors.len()),
        );

        for error in &errors {
            eprintln!(
                "[sema] error at {}..{}: {}{}",
                error.span.lo,
                error.span.hi,
                error.msg,
                error
                    .note
                    .as_deref()
                    .map(|note| format!(" (note: {note})"))
                    .unwrap_or_default()
            );
        }
    }

    if errors.is_empty() {
        sema_debug(debug, "semantic analysis succeeded");
        Ok(p)
    } else {
        sema_debug(debug, "semantic analysis failed");
        Err(errors)
    }
}