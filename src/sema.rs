use crate::ast::*;
use crate::lexer::Span;
use std::collections::{HashMap, HashSet};

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
    let mut errors = Vec::new();

    // =========================================================================
    // PHASE 1: Reachability (Remove unused externals/lets)
    // =========================================================================
    let mut used = HashSet::new();
    let mut top_level: HashMap<String, &Decl> = HashMap::new();

    for d in &p.decls {
        match d {
            Decl::Let { name, .. } | Decl::External(Extern { name, .. }) => {
                top_level.insert(name.clone(), d);
            }
            Decl::Type { name, ctors, .. } => {
                top_level.insert(name.clone(), d);
                for c in ctors {
                    top_level.insert(c.name.clone(), d);
                }
            }
            Decl::Bring(_) => {}
        }
    }

    fn walk_expr_used(e: &Expr, used: &mut HashSet<String>) {
        match &e.kind {
            ExprKind::Var(n) => {
                used.insert(n.clone());
            }
            ExprKind::Let { value, body, .. } => {
                walk_expr_used(value, used);
                walk_expr_used(body, used);
            }
            ExprKind::Fn { body, .. } => walk_expr_used(body, used),
            ExprKind::App { fun, args } => {
                walk_expr_used(fun, used);
                for a in args {
                    walk_expr_used(a, used);
                }
            }
            ExprKind::Case { scrut, arms } => {
                walk_expr_used(scrut, used);
                for arm in arms {
                    walk_expr_used(&arm.body, used);
                }
            }
            ExprKind::Block { stmts, tail } => {
                for s in stmts {
                    walk_expr_used(s, used);
                }
                walk_expr_used(tail, used);
            }
            ExprKind::Bin { l, r, .. } => {
                walk_expr_used(l, used);
                walk_expr_used(r, used);
            }
            ExprKind::Un { e, .. } | ExprKind::Quote(e) => walk_expr_used(e, used),
            _ => {}
        }
    }

    walk_expr_used(&p.body, &mut used);

    let mut changed = true;
    while changed {
        changed = false;
        for name in used.clone() {
            if let Some(d) = top_level.get(&name) {
                if let Decl::Let { value, .. } = d {
                    let before = used.len();
                    walk_expr_used(value, &mut used);
                    if used.len() > before {
                        changed = true;
                    }
                }
            }
        }
    }

    p.decls.retain(|d| match d {
        Decl::Let { name, .. } | Decl::External(Extern { name, .. }) => used.contains(name),
        Decl::Type { name, ctors, .. } => {
            used.contains(name) || ctors.iter().any(|c| used.contains(&c.name))
        }
        Decl::Bring(_) => true,
    });

    // =========================================================================
    // PHASE 2: Scope Checking (Throw errors for undefined variables)
    // =========================================================================
    let mut globals = HashSet::new();
    for d in &p.decls {
        match d {
            Decl::Let { name, .. } | Decl::External(Extern { name, .. }) => {
                globals.insert(name.clone());
            }
            Decl::Type { ctors, .. } => {
                for c in ctors {
                    globals.insert(c.name.clone());
                }
            }
            Decl::Bring(_) => {}
        }
    }

    fn check_expr(
        e: &Expr,
        scope: &mut Vec<HashSet<String>>,
        globals: &HashSet<String>,
        errors: &mut Vec<SemaErr>,
    ) {
        match &e.kind {
            ExprKind::Var(n) => {
                let in_scope = scope.iter().rev().any(|s| s.contains(n)) || globals.contains(n);
                if !in_scope {
                    let mut suggestions: Vec<String> = globals
                        .iter()
                        .filter(|g| levenshtein(n, g) <= 2 && levenshtein(n, g) > 0)
                        .cloned()
                        .collect();
                    suggestions.sort_by_key(|g| levenshtein(n, g));

                    let note = if suggestions.is_empty() {
                        Some("if this is a function, ensure it is defined before use or imported via `bring`".into())
                    } else {
                        Some(format!("did you mean `{}`?", suggestions[0]))
                    };

                    errors.push(SemaErr {
                        msg: format!("undefined variable `{}`", n),
                        span: e.span,
                        note,
                    });
                }
            }
            ExprKind::Let { name, value, body } => {
                check_expr(value, scope, globals, errors);
                scope.last_mut().unwrap().insert(name.clone());
                check_expr(body, scope, globals, errors);
                scope.last_mut().unwrap().remove(name);
            }
            ExprKind::Fn { params, body } => {
                scope.push(HashSet::new());
                for p in params {
                    scope.last_mut().unwrap().insert(p.clone());
                }
                check_expr(body, scope, globals, errors);
                scope.pop();
            }
            ExprKind::App { fun, args } => {
                check_expr(fun, scope, globals, errors);
                for a in args {
                    check_expr(a, scope, globals, errors);
                }
            }
            ExprKind::Case { scrut, arms } => {
                check_expr(scrut, scope, globals, errors);
                for arm in arms {
                    scope.push(HashSet::new());
                    collect_pat_vars(&arm.pat, scope.last_mut().unwrap());
                    check_expr(&arm.body, scope, globals, errors);
                    scope.pop();
                }
            }
            ExprKind::Block { stmts, tail } => {
                scope.push(HashSet::new());
                for s in stmts {
                    check_expr(s, scope, globals, errors);
                    if let ExprKind::Let { name, .. } = &s.kind {
                        scope.last_mut().unwrap().insert(name.clone());
                    }
                }
                check_expr(tail, scope, globals, errors);
                scope.pop();
            }
            ExprKind::Bin { l, r, .. } => {
                check_expr(l, scope, globals, errors);
                check_expr(r, scope, globals, errors);
            }
            ExprKind::Un { e, .. } | ExprKind::Quote(e) => check_expr(e, scope, globals, errors),
            _ => {}
        }
    }

    fn collect_pat_vars(p: &Pat, out: &mut HashSet<String>) {
        match p {
            Pat::Bind(n) => {
                out.insert(n.clone());
            }
            Pat::Ctor { args, .. } | Pat::Tuple(args) => {
                for a in args {
                    collect_pat_vars(a, out);
                }
            }
            _ => {}
        }
    }

    for d in &p.decls {
        if let Decl::Let { value, .. } = d {
            let mut scope = vec![HashSet::new()];
            check_expr(value, &mut scope, &globals, &mut errors);
        }
    }

    let mut scope = vec![HashSet::new()];
    check_expr(&p.body, &mut scope, &globals, &mut errors);

    if errors.is_empty() {
        Ok(p)
    } else {
        Err(errors)
    }
}
