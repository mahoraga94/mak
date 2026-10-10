#![allow(unused_variables, unused_mut, unreachable_patterns, dead_code)]

use crate::ast::*;
use crate::lexer::Span;
use std::collections::{HashMap, HashSet};

/// Parser-generated names for syntax that has no dedicated AST variant.
/// These are compiler intrinsics, not user-level references. Keep this list
/// in sync with `sema.rs` and `parser.rs`.
#[inline]
fn is_builtin_intrinsic(name: &str) -> bool {
    matches!(name, "$mak.index")
}


const NULL_SPAN: Span = Span { lo: 0, hi: 0 };
const LOCAL_LAYERS: usize = 128;

macro_rules! opt_debug {
    ($($arg:tt)*) => {{
        #[cfg(debug_assertions)]
        {
            eprintln!("  [opt-debug] {}", format_args!($($arg)*));
        }
    }};
}

macro_rules! opt_debug_delta {
    ($label:expr, $before:expr, $after:expr) => {{
        #[cfg(debug_assertions)]
        {
            debug_delta($label, $before, $after);
        }
    }};
}

#[cfg(debug_assertions)]
fn debug_delta(label: &str, before: &Program, after: &Program) {
    let b = measure(before);
    let a = measure(after);
    let bc = wpo_cost(before);
    let ac = wpo_cost(after);
    let dn = a.nodes as isize - b.nodes as isize;
    let df = a.fns as isize - b.fns as isize;
    let db = a.binders as isize - b.binders as isize;
    let dc = ac as isize - bc as isize;
    eprintln!(
        "  [opt-debug] {} candidate shape: nodes {} -> {} ({:+}), fns {} -> {} ({:+}), binders {} -> {} ({:+}), cost {} -> {} ({:+}, {:+.2}%), hash {:016x} -> {:016x}",
        label,
        b.nodes,
        a.nodes,
        dn,
        b.fns,
        a.fns,
        df,
        b.binders,
        a.binders,
        db,
        bc,
        ac,
        dc,
        pct_delta(bc, ac),
        fingerprint(before),
        fingerprint(after),
    );

    let names_before = before
        .decls
        .iter()
        .filter_map(|d| match d {
            Decl::Let { name, .. } => Some(name.as_str()),
            _ => None,
        })
        .collect::<HashSet<_>>();
    let names_after = after
        .decls
        .iter()
        .filter_map(|d| match d {
            Decl::Let { name, .. } => Some(name.as_str()),
            _ => None,
        })
        .collect::<HashSet<_>>();

    let mut removed = names_before
        .difference(&names_after)
        .copied()
        .collect::<Vec<_>>();
    let mut added = names_after
        .difference(&names_before)
        .copied()
        .collect::<Vec<_>>();
    removed.sort_unstable();
    added.sort_unstable();

    if removed.is_empty() && added.is_empty() {
        eprintln!(
            "  [opt-debug] {} declarations: no top-level function/value names added or removed",
            label
        );
    } else {
        eprintln!(
            "  [opt-debug] {} declarations: -[{}] +[{}]",
            label,
            removed.join(", "),
            added.join(", "),
        );
    }
}

#[derive(Default)]
struct OptStat {
    hits: usize,
    delta: isize,
}

#[derive(Default)]
struct OptStats {
    rounds: usize,
    restarts: usize,
    passes: HashMap<String, OptStat>,
    rejected: usize,
    quarantined: HashSet<String>,
}

#[derive(Default, Clone, Copy)]
struct Measure {
    nodes: usize,
    fns: usize,
    binders: usize,
}

fn measure(p: &Program) -> Measure {
    fn pat(p: &Pat, m: &mut Measure) {
        match p {
            Pat::Bind(_) => m.binders += 1,
            Pat::Ctor { args, .. } => {
                for a in args {
                    pat(a, m);
                }
            }
            Pat::Tuple(xs) => {
                for x in xs {
                    pat(x, m);
                }
            }
            _ => {}
        }
    }

    fn expr(e: &Expr, m: &mut Measure) {
        m.nodes += 1;
        match &e.kind {
            ExprKind::Let { value, body, .. } => {
                m.binders += 1;
                expr(value, m);
                expr(body, m);
            }
            ExprKind::Fn { params, body } => {
                m.fns += 1;
                m.binders += params.len();
                expr(body, m);
            }
            ExprKind::App { fun, args } => {
                expr(fun, m);
                for a in args {
                    expr(a, m);
                }
            }
            ExprKind::Case { scrut, arms } => {
                expr(scrut, m);
                for arm in arms {
                    pat(&arm.pat, m);
                    expr(&arm.body, m);
                }
            }
            ExprKind::Block { stmts, tail } => {
                for s in stmts {
                    expr(s, m);
                }
                expr(tail, m);
            }
            ExprKind::Bin { l, r, .. } => {
                expr(l, m);
                expr(r, m);
            }
            ExprKind::Un { e, .. } | ExprKind::Quote(e) => expr(e, m),
            ExprKind::Int(_) | ExprKind::Float(_) | ExprKind::Str(_) | ExprKind::Var(_) => {}
        }
    }

    let mut m = Measure::default();
    for d in &p.decls {
        m.nodes += 1;
        match d {
            Decl::Let { value, .. } => expr(value, &mut m),
            Decl::External(_) | Decl::Type { .. } | Decl::Bring(_) => {}
        }
    }
    expr(&p.body, &mut m);
    m
}

fn pct_delta(before: usize, after: usize) -> f64 {
    if before == 0 {
        0.0
    } else {
        ((after as f64 - before as f64) * 100.0) / before as f64
    }
}

fn print_pass_stats(s: &OptStats) {
    let order = [
        "beta",
        "known-case",
        "tree",
        "const",
        "global-const",
        "global-sccp",
        "param-drop",
        "inline",
        "mega",
        "advanced",
        "jump-thread",
        "sink",
        "word-sccp",
        "dce",
        "whole-dce",
    ];
    let mut parts = Vec::new();
    for name in order {
        if let Some(v) = s.passes.get(name) {
            if v.hits != 0 {
                let n = if v.delta >= 0 {
                    format!("-{} nodes", v.delta)
                } else {
                    format!("+{} nodes", -v.delta)
                };
                parts.push(format!("{}={} ({})", name, v.hits, n));
            }
        }
    }
    if parts.is_empty() {
        eprintln!("  passes: no effective rewrites");
    } else {
        eprintln!("  passes: {}", parts.join("  "));
    }
    if s.rejected != 0 {
        eprintln!("  safety guards: {} rejected", s.rejected);
    }
    if !s.quarantined.is_empty() {
        let mut q = s.quarantined.iter().cloned().collect::<Vec<_>>();
        q.sort_unstable();
        eprintln!("  quarantined: {}", q.join(", "));
    }
}

const WPO_ROUNDS: usize = 20;
const WPO_MAX_COMMITS: usize = 160;
const WPO_COST_GROWTH_NUM: usize = 3;
const WPO_COST_GROWTH_DEN: usize = 2;

#[derive(Default)]
struct WpoStats {
    rounds: usize,
    commits: usize,
    rejected: usize,
    const_folds: usize,
    phase_hits: HashMap<String, usize>,
}

#[derive(Clone)]
struct WpoSummary {
    functions: HashMap<String, (Vec<String>, Expr)>,
    graph: HashMap<String, HashSet<String>>,
    recursive: HashSet<String>,
    pure: HashSet<String>,
    calls: HashMap<String, usize>,
    sizes: HashMap<String, usize>,
}

fn wpo_function_map(p: &Program) -> HashMap<String, (Vec<String>, Expr)> {
    let mut out = HashMap::new();
    for d in &p.decls {
        if let Decl::Let { name, value, .. } = d {
            if let ExprKind::Fn { params, body } = &value.kind {
                out.insert(name.clone(), (params.clone(), (**body).clone()));
            }
        }
    }
    out
}

fn wpo_graph_and_calls(
    p: &Program,
    funcs: &HashMap<String, (Vec<String>, Expr)>,
) -> (HashMap<String, HashSet<String>>, HashMap<String, usize>) {
    let names: HashSet<String> = funcs.keys().cloned().collect();
    let mut graph = HashMap::<String, HashSet<String>>::new();
    let mut calls_total = HashMap::<String, usize>::new();

    fn count_expr(
        e: &Expr,
        names: &HashSet<String>,
        graph_out: &mut HashSet<String>,
        total: &mut HashMap<String, usize>,
    ) {
        match &e.kind {
            ExprKind::App { fun, args } => {
                if let ExprKind::Var(n) = &fun.kind {
                    if names.contains(n) {
                        graph_out.insert(n.clone());
                        *total.entry(n.clone()).or_default() += 1;
                    }
                }
                count_expr(fun, names, graph_out, total);
                for a in args {
                    count_expr(a, names, graph_out, total);
                }
            }
            ExprKind::Let { value, body, .. } => {
                count_expr(value, names, graph_out, total);
                count_expr(body, names, graph_out, total);
            }
            ExprKind::Fn { body, .. } => count_expr(body, names, graph_out, total),
            ExprKind::Case { scrut, arms } => {
                count_expr(scrut, names, graph_out, total);
                for a in arms {
                    count_expr(&a.body, names, graph_out, total);
                }
            }
            ExprKind::Block { stmts, tail } => {
                for s in stmts {
                    count_expr(s, names, graph_out, total);
                }
                count_expr(tail, names, graph_out, total);
            }
            ExprKind::Bin { l, r, .. } => {
                count_expr(l, names, graph_out, total);
                count_expr(r, names, graph_out, total);
            }
            ExprKind::Un { e, .. } | ExprKind::Quote(e) => count_expr(e, names, graph_out, total),
            ExprKind::Int(_) | ExprKind::Float(_) | ExprKind::Str(_) | ExprKind::Var(_) => {}
        }
    }

    for (name, (_, body)) in funcs {
        let mut edges = HashSet::new();
        count_expr(body, &names, &mut edges, &mut calls_total);
        graph.insert(name.clone(), edges);
    }

    let mut body_edges = HashSet::new();
    count_expr(&p.body, &names, &mut body_edges, &mut calls_total);
    let _ = body_edges;
    (graph, calls_total)
}

fn wpo_sccs(graph: &HashMap<String, HashSet<String>>) -> Vec<Vec<String>> {
    fn dfs(
        v: &str,
        graph: &HashMap<String, HashSet<String>>,
        seen: &mut HashSet<String>,
        order: &mut Vec<String>,
    ) {
        if !seen.insert(v.to_string()) {
            return;
        }
        if let Some(next) = graph.get(v) {
            for n in next {
                dfs(n, graph, seen, order);
            }
        }
        order.push(v.to_string());
    }

    fn rev_graph(graph: &HashMap<String, HashSet<String>>) -> HashMap<String, HashSet<String>> {
        let mut rev = HashMap::<String, HashSet<String>>::new();
        for n in graph.keys() {
            rev.entry(n.clone()).or_default();
        }
        for (a, bs) in graph {
            for b in bs {
                rev.entry(b.clone()).or_default().insert(a.clone());
            }
        }
        rev
    }

    fn collect(
        v: &str,
        graph: &HashMap<String, HashSet<String>>,
        seen: &mut HashSet<String>,
        comp: &mut Vec<String>,
    ) {
        if !seen.insert(v.to_string()) {
            return;
        }
        comp.push(v.to_string());
        if let Some(next) = graph.get(v) {
            for n in next {
                collect(n, graph, seen, comp);
            }
        }
    }

    let mut seen = HashSet::new();
    let mut order = Vec::with_capacity(graph.len());
    for n in graph.keys() {
        dfs(n, graph, &mut seen, &mut order);
    }

    let rev = rev_graph(graph);
    let mut seen_rev = HashSet::new();
    let mut sccs = Vec::new();
    while let Some(v) = order.pop() {
        let mut comp = Vec::new();
        collect(&v, &rev, &mut seen_rev, &mut comp);
        if !comp.is_empty() {
            sccs.push(comp);
        }
    }
    sccs
}

fn wpo_recursive_from_graph(graph: &HashMap<String, HashSet<String>>) -> HashSet<String> {
    let mut out = HashSet::new();
    for comp in wpo_sccs(graph) {
        if comp.len() > 1 {
            out.extend(comp);
            continue;
        }
        if let Some(n) = comp.first() {
            if graph.get(n).map_or(false, |xs| xs.contains(n)) {
                out.insert(n.clone());
            }
        }
    }
    out
}

fn flatten_application_for_opt<'a>(fun: &'a Expr, args: &'a [Expr]) -> (&'a Expr, Vec<&'a Expr>) {
    let mut base = fun;
    let mut flattened = Vec::new();
    if let ExprKind::App { fun: inner_fun, args: inner_args } = &base.kind {
        let (inner_base, inner_flat) = flatten_application_for_opt(inner_fun, inner_args);
        base = inner_base;
        flattened.extend(inner_flat);
    }
    flattened.extend(args.iter());
    (base, flattened)
}

fn wpo_pure_expr(e: &Expr, pure: &HashSet<String>, local_scc: &HashSet<String>) -> bool {
    match &e.kind {
        ExprKind::Int(_) | ExprKind::Float(_) | ExprKind::Str(_) | ExprKind::Var(_) => true,
        ExprKind::Fn { body, .. } => wpo_pure_expr(body, pure, local_scc),
        ExprKind::Quote(_) => true,
        ExprKind::Let { value, body, .. } => {
            wpo_pure_expr(value, pure, local_scc) && wpo_pure_expr(body, pure, local_scc)
        }
        ExprKind::App { fun, args } => {
            let (base, flat_args) = flatten_application_for_opt(fun, args);
            if matches!(&base.kind, ExprKind::Var(n) if n == "$mak.index") {
                // Dereferencing unknown memory is not referentially transparent:
                // an external call may mutate the pointed-to object.
                let _ = flat_args;
                return false;
            }
            let callee_ok = match &fun.kind {
                ExprKind::Var(n) => local_scc.contains(n) || pure.contains(n),
                ExprKind::Fn { body, .. } => wpo_pure_expr(body, pure, local_scc),
                _ => wpo_pure_expr(fun, pure, local_scc),
            };
            callee_ok && args.iter().all(|a| wpo_pure_expr(a, pure, local_scc))
        }
        ExprKind::Case { scrut, arms } => {
            wpo_pure_expr(scrut, pure, local_scc)
                && arms.iter().all(|a| wpo_pure_expr(&a.body, pure, local_scc))
        }
        ExprKind::Block { stmts, tail } => {
            stmts.iter().all(|s| wpo_pure_expr(s, pure, local_scc))
                && wpo_pure_expr(tail, pure, local_scc)
        }
        ExprKind::Bin { l, r, .. } => {
            wpo_pure_expr(l, pure, local_scc) && wpo_pure_expr(r, pure, local_scc)
        }
        ExprKind::Un { e, .. } => wpo_pure_expr(e, pure, local_scc),
    }
}

fn wpo_collect_pure(p: &Program, graph: &HashMap<String, HashSet<String>>) -> HashSet<String> {
    let mut pure = HashSet::<String>::new();

    for d in &p.decls {
        match d {
            Decl::External(e) if e.pure => {
                pure.insert(e.name.clone());
            }
            Decl::Type { ctors, .. } => {
                for c in ctors {
                    pure.insert(c.name.clone());
                }
            }
            _ => {}
        }
    }

    let sccs = wpo_sccs(graph);
    loop {
        let mut changed = false;
        for comp in &sccs {
            let local: HashSet<String> = comp.iter().cloned().collect();
            let mut ok = true;
            for d in &p.decls {
                let Decl::Let { name, value, .. } = d else {
                    continue;
                };
                if !local.contains(name) {
                    continue;
                }
                let ExprKind::Fn { body, .. } = &value.kind else {
                    ok = false;
                    break;
                };
                if !wpo_pure_expr(body, &pure, &local) {
                    ok = false;
                    break;
                }
            }
            if ok {
                for n in comp {
                    changed |= pure.insert(n.clone());
                }
            }
        }
        if !changed {
            break;
        }
    }
    pure
}

fn wpo_summary(p: &Program) -> WpoSummary {
    let functions = wpo_function_map(p);
    let (graph, calls) = wpo_graph_and_calls(p, &functions);
    let recursive = wpo_recursive_from_graph(&graph);
    let pure = wpo_collect_pure(p, &graph);
    let sizes = functions
        .iter()
        .map(|(n, (_, body))| (n.clone(), size(body)))
        .collect();
    WpoSummary {
        functions,
        graph,
        recursive,
        pure,
        calls,
        sizes,
    }
}

fn wpo_pat_int(p: &Pat, value: u64, binds: &mut Vec<(String, u64)>) -> bool {
    match p {
        Pat::Wild => true,
        Pat::Bind(n) => {
            binds.push((n.clone(), value));
            true
        }
        Pat::Int(n) => *n == value,
        _ => false,
    }
}

fn wpo_eval_int(
    e: &Expr,
    env: &mut Vec<(String, u64)>,
    funcs: &HashMap<String, (Vec<String>, Expr)>,
    pure: &HashSet<String>,
    fuel: &mut usize,
) -> Option<u64> {
    if *fuel == 0 {
        return None;
    }
    *fuel -= 1;
    match &e.kind {
        ExprKind::Int(n) => Some(*n),
        ExprKind::Var(n) => env.iter().rev().find(|(k, _)| k == n).map(|(_, v)| *v),
        ExprKind::Un { op, e } => {
            wpo_eval_int(e, env, funcs, pure, fuel).and_then(|v| eval_un(*op, v))
        }
        ExprKind::Bin { op, l, r } => {
            let a = wpo_eval_int(l, env, funcs, pure, fuel)?;
            let b = wpo_eval_int(r, env, funcs, pure, fuel)?;
            eval_bin(*op, a, b)
        }
        ExprKind::Let { name, value, body } => {
            let v = wpo_eval_int(value, env, funcs, pure, fuel)?;
            env.push((name.clone(), v));
            let out = wpo_eval_int(body, env, funcs, pure, fuel);
            env.pop();
            out
        }
        ExprKind::Case { scrut, arms } => {
            let v = wpo_eval_int(scrut, env, funcs, pure, fuel)?;
            for arm in arms {
                let mut binds = Vec::new();
                if wpo_pat_int(&arm.pat, v, &mut binds) {
                    let old = env.len();
                    for b in binds {
                        env.push(b);
                    }
                    let out = wpo_eval_int(&arm.body, env, funcs, pure, fuel);
                    env.truncate(old);
                    return out;
                }
            }
            None
        }
        ExprKind::Block { stmts, tail } => {
            for s in stmts {
                let _ = wpo_eval_int(s, env, funcs, pure, fuel);
            }
            wpo_eval_int(tail, env, funcs, pure, fuel)
        }
        ExprKind::App { fun, args } => {
            let ExprKind::Var(name) = &fun.kind else {
                return None;
            };
            if !pure.contains(name) {
                return None;
            }
            let (params, body) = funcs.get(name)?;
            if params.len() != args.len() {
                return None;
            }
            let mut values = Vec::with_capacity(args.len());
            for a in args {
                values.push(wpo_eval_int(a, env, funcs, pure, fuel)?);
            }
            let old = env.len();
            for (param, value) in params.iter().zip(values) {
                env.push((param.clone(), value));
            }
            let out = wpo_eval_int(body, env, funcs, pure, fuel);
            env.truncate(old);
            out
        }
        ExprKind::Fn { .. } | ExprKind::Float(_) | ExprKind::Str(_) | ExprKind::Quote(_) => None,
    }
}

fn wpo_fold_word_calls_expr(
    e: Expr,
    summary: &WpoSummary,
    changed: &mut bool,
    folds: &mut usize,
) -> Expr {
    let sp = e.span;
    let e = match e.kind {
        ExprKind::Let { name, value, body } => Expr {
            span: sp,
            kind: ExprKind::Let {
                name,
                value: Box::new(wpo_fold_word_calls_expr(*value, summary, changed, folds)),
                body: Box::new(wpo_fold_word_calls_expr(*body, summary, changed, folds)),
            },
        },
        ExprKind::Fn { params, body } => Expr {
            span: sp,
            kind: ExprKind::Fn {
                params,
                body: Box::new(wpo_fold_word_calls_expr(*body, summary, changed, folds)),
            },
        },
        ExprKind::App { fun, args } => Expr {
            span: sp,
            kind: ExprKind::App {
                fun: Box::new(wpo_fold_word_calls_expr(*fun, summary, changed, folds)),
                args: args
                    .into_iter()
                    .map(|a| wpo_fold_word_calls_expr(a, summary, changed, folds))
                    .collect(),
            },
        },
        ExprKind::Case { scrut, arms } => Expr {
            span: sp,
            kind: ExprKind::Case {
                scrut: Box::new(wpo_fold_word_calls_expr(*scrut, summary, changed, folds)),
                arms: arms
                    .into_iter()
                    .map(|a| Arm {
                        pat: a.pat,
                        body: wpo_fold_word_calls_expr(a.body, summary, changed, folds),
                        span: a.span,
                    })
                    .collect(),
            },
        },
        ExprKind::Block { stmts, tail } => Expr {
            span: sp,
            kind: ExprKind::Block {
                stmts: stmts
                    .into_iter()
                    .map(|s| wpo_fold_word_calls_expr(s, summary, changed, folds))
                    .collect(),
                tail: Box::new(wpo_fold_word_calls_expr(*tail, summary, changed, folds)),
            },
        },
        ExprKind::Bin { op, l, r } => Expr {
            span: sp,
            kind: ExprKind::Bin {
                op,
                l: Box::new(wpo_fold_word_calls_expr(*l, summary, changed, folds)),
                r: Box::new(wpo_fold_word_calls_expr(*r, summary, changed, folds)),
            },
        },
        ExprKind::Un { op, e } => Expr {
            span: sp,
            kind: ExprKind::Un {
                op,
                e: Box::new(wpo_fold_word_calls_expr(*e, summary, changed, folds)),
            },
        },
        ExprKind::Quote(x) => Expr {
            span: sp,
            kind: ExprKind::Quote(Box::new(wpo_fold_word_calls_expr(
                *x, summary, changed, folds,
            ))),
        },
        other => Expr {
            span: sp,
            kind: other,
        },
    };

    if let ExprKind::App { fun, args } = &e.kind {
        if let ExprKind::Var(name) = &fun.kind {
            if summary.pure.contains(name)
                && summary.functions.contains_key(name)
                && args.iter().all(|a| matches!(a.kind, ExprKind::Int(_)))
                && args.len() <= 12
            {
                let mut env = Vec::new();
                let mut fuel = 4096usize;
                if let Some(v) =
                    wpo_eval_int(&e, &mut env, &summary.functions, &summary.pure, &mut fuel)
                {
                    *changed = true;
                    *folds += 1;
                    return Expr {
                        span: sp,
                        kind: ExprKind::Int(v),
                    };
                }
            }
        }
    }
    e
}

fn wpo_fold_word_calls(
    mut p: Program,
    summary: &WpoSummary,
    changed: &mut bool,
    folds: &mut usize,
) -> Program {
    p.decls = p
        .decls
        .into_iter()
        .map(|d| match d {
            Decl::Let { name, value, span } => Decl::Let {
                name,
                value: Box::new(wpo_fold_word_calls_expr(*value, summary, changed, folds)),
                span,
            },
            other => other,
        })
        .collect();
    p.body = Box::new(wpo_fold_word_calls_expr(*p.body, summary, changed, folds));
    p
}

fn wpo_adaptive_inline(p: Program, pure: &HashSet<String>, changed: &mut bool) -> Program {
    let summary = wpo_summary(&p);
    if summary.functions.is_empty() {
        return p;
    }
    let mut used = identifiers(&p);
    let mut serial = 0usize;

    fn inline_one(
        params: &[String],
        body: &Expr,
        args: &[Expr],
        pure: &HashSet<String>,
        used: &mut HashSet<String>,
        serial: &mut usize,
    ) -> Option<Expr> {
        if params.len() != args.len() || !distinct(params) {
            return None;
        }
        let mut b = body.clone();
        let mut lets = Vec::<(String, Expr)>::new();
        for (param, arg) in params.iter().zip(args) {
            let uses = count_var(&b, param);
            if uses == 0 {
                if is_pure(arg, pure) {
                    continue;
                }
            }
            if uses <= 1 || is_pure(arg, pure) {
                b = subst(b, param, arg);
            } else {
                let fresh = fresh_opt_name(used, serial, "ip");
                b = subst(
                    b,
                    param,
                    &Expr {
                        span: arg.span,
                        kind: ExprKind::Var(fresh.clone()),
                    },
                );
                lets.push((fresh, arg.clone()));
            }
            if count_var(&b, param) != 0 {
                return None;
            }
        }
        for (name, value) in lets.into_iter().rev() {
            b = Expr {
                span: b.span,
                kind: ExprKind::Let {
                    name,
                    value: Box::new(value),
                    body: Box::new(b),
                },
            };
        }
        Some(b)
    }

    fn go(
        e: Expr,
        summary: &WpoSummary,
        pure: &HashSet<String>,
        changed: &mut bool,
        used: &mut HashSet<String>,
        serial: &mut usize,
    ) -> Expr {
        let sp = e.span;
        let e = match e.kind {
            ExprKind::Let { name, value, body } => Expr {
                span: sp,
                kind: ExprKind::Let {
                    name,
                    value: Box::new(go(*value, summary, pure, changed, used, serial)),
                    body: Box::new(go(*body, summary, pure, changed, used, serial)),
                },
            },
            ExprKind::Fn { params, body } => Expr {
                span: sp,
                kind: ExprKind::Fn {
                    params,
                    body: Box::new(go(*body, summary, pure, changed, used, serial)),
                },
            },
            ExprKind::App { fun, args } => Expr {
                span: sp,
                kind: ExprKind::App {
                    fun: Box::new(go(*fun, summary, pure, changed, used, serial)),
                    args: args
                        .into_iter()
                        .map(|a| go(a, summary, pure, changed, used, serial))
                        .collect(),
                },
            },
            ExprKind::Case { scrut, arms } => Expr {
                span: sp,
                kind: ExprKind::Case {
                    scrut: Box::new(go(*scrut, summary, pure, changed, used, serial)),
                    arms: arms
                        .into_iter()
                        .map(|a| Arm {
                            pat: a.pat,
                            body: go(a.body, summary, pure, changed, used, serial),
                            span: a.span,
                        })
                        .collect(),
                },
            },
            ExprKind::Block { stmts, tail } => Expr {
                span: sp,
                kind: ExprKind::Block {
                    stmts: stmts
                        .into_iter()
                        .map(|x| go(x, summary, pure, changed, used, serial))
                        .collect(),
                    tail: Box::new(go(*tail, summary, pure, changed, used, serial)),
                },
            },
            ExprKind::Bin { op, l, r } => Expr {
                span: sp,
                kind: ExprKind::Bin {
                    op,
                    l: Box::new(go(*l, summary, pure, changed, used, serial)),
                    r: Box::new(go(*r, summary, pure, changed, used, serial)),
                },
            },
            ExprKind::Un { op, e } => Expr {
                span: sp,
                kind: ExprKind::Un {
                    op,
                    e: Box::new(go(*e, summary, pure, changed, used, serial)),
                },
            },
            ExprKind::Quote(x) => Expr {
                span: sp,
                kind: ExprKind::Quote(Box::new(go(*x, summary, pure, changed, used, serial))),
            },
            other => Expr {
                span: sp,
                kind: other,
            },
        };

        if let ExprKind::App { fun, args } = &e.kind {
            if let ExprKind::Var(name) = &fun.kind {
                if let Some((params, body)) = summary.functions.get(name) {
                    if !summary.recursive.contains(name)
                        && params.len() == args.len()
                        && !params.is_empty()
                        && size(body) <= 384
                    {
                        let uses = *summary.calls.get(name).unwrap_or(&1);
                        let limit = if uses <= 1 {
                            384
                        } else if pure.contains(name) {
                            192
                        } else {
                            96
                        };
                        if size(body) <= limit {
                            let constant_arg = args.iter().any(|a| {
                                matches!(
                                    a.kind,
                                    ExprKind::Int(_) | ExprKind::Float(_) | ExprKind::Str(_)
                                )
                            });
                            let hot_or_const = uses <= 1 || constant_arg || pure.contains(name);
                            if hot_or_const {
                                if let Some(out) =
                                    inline_one(params, body, args, pure, used, serial)
                                {
                                    *changed = true;
                                    return out;
                                }
                            }
                        }
                    }
                }
            }
        }
        e
    }

    let mut out = p;
    out.decls = out
        .decls
        .into_iter()
        .map(|d| match d {
            Decl::Let { name, value, span } => Decl::Let {
                name,
                value: Box::new(go(*value, &summary, pure, changed, &mut used, &mut serial)),
                span,
            },
            other => other,
        })
        .collect();
    out.body = Box::new(go(
        *out.body,
        &summary,
        pure,
        changed,
        &mut used,
        &mut serial,
    ));
    out
}

fn wpo_cost(p: &Program) -> usize {
    let m = measure(p);
    m.nodes
        .saturating_add(m.binders.saturating_mul(3))
        .saturating_add(m.fns.saturating_mul(5))
}

#[inline]
fn opt_cost(p: &Program) -> usize {
    let m = measure(p);
    m.nodes
        .saturating_add(m.binders.saturating_mul(3))
        .saturating_add(m.fns.saturating_mul(5))
}

fn wpo_commit<F>(
    p: Program,
    pure: &HashSet<String>,
    label: &str,
    stats: &mut WpoStats,
    f: F,
) -> (Program, bool)
where
    F: FnOnce(Program, &HashSet<String>, &mut bool) -> Program,
{
    let before_hash = fingerprint(&p);
    let before_cost = wpo_cost(&p);
    let before_measure = measure(&p);
    let mut changed = false;

    opt_debug!(
        "[WPO] pass={} BEGIN: hash={:016x}, nodes={}, fns={}, binders={}, cost={}",
        label,
        before_hash,
        before_measure.nodes,
        before_measure.fns,
        before_measure.binders,
        before_cost
    );

    let candidate = f(p.clone(), pure, &mut changed);
    let after_hash = fingerprint(&candidate);

    if !changed {
        opt_debug!(
            "[WPO] pass={} KEEP: transform reported changed=false; candidate discarded without AST checks",
            label
        );
        return (p, false);
    }

    opt_debug_delta!(&format!("[WPO] {}", label), &p, &candidate);

    if after_hash == before_hash {
        stats.rejected += 1;
        opt_debug!(
            "[WPO] pass={} REVERT: transform claimed a rewrite, but structural fingerprint stayed {:016x}; reason=NO_STRUCTURAL_CHANGE",
            label,
            before_hash
        );
        return (p, false);
    }

    let _ = (before_cost, wpo_cost(&candidate));

    let scope_ok = scoped(&candidate);
    if !scope_ok {
        stats.rejected += 1;
        opt_debug!(
            "[WPO] pass={} REVERT: scope validation failed; reason=SCOPE_INVARIANT",
            label
        );
        return (p, false);
    }

    *stats.phase_hits.entry(label.to_string()).or_default() += 1;
    stats.commits += 1;
    opt_debug!(
        "[WPO] pass={} COMMIT: candidate accepted; commits={} rejects={}",
        label,
        stats.commits,
        stats.rejected
    );
    (candidate, true)
}

fn whole_program_optimize(mut p: Program) -> (Program, WpoStats) {
    let mut stats = WpoStats::default();
    let mut last = fingerprint(&p);

    for round in 0..WPO_ROUNDS {
        if stats.commits >= WPO_MAX_COMMITS {
            opt_debug!(
                "[WPO] STOP: commit limit reached ({} >= {})",
                stats.commits,
                WPO_MAX_COMMITS
            );
            break;
        }
        stats.rounds += 1;
        opt_debug!(
            "[WPO] ROUND {} BEGIN: hash={:016x}, nodes={}, fns={}, binders={}",
            round + 1,
            fingerprint(&p),
            measure(&p).nodes,
            measure(&p).fns,
            measure(&p).binders
        );
        let summary = wpo_summary(&p);
        let _ = (&summary.graph, &summary.calls, &summary.sizes);

        let before_round = fingerprint(&p);

        let mut folds = 0usize;
        let (next, ok) = wpo_commit(
            p,
            &summary.pure,
            "word-sccp",
            &mut stats,
            |p, _, changed| wpo_fold_word_calls(p, &summary, changed, &mut folds),
        );
        p = next;
        stats.const_folds += folds;
        if !ok && folds != 0 {
            stats.const_folds = stats.const_folds.saturating_sub(folds);
        }

        let phases: [(&str, fn(Program, &HashSet<String>, &mut bool) -> Program); 14] = [
            ("beta", beta),
            ("known-case", known_cases),
            ("instcombine", tree_opt),
            ("sccp", const_opt),
            ("global-sccp", global_const_opt),
            ("jump-thread", adv_opt),
            ("gvn-cse", adv_opt),
            ("arg-dce", drop_params),
            ("sink-sroa", sink),
            ("mega-local", mega_opt),
            ("case-fold", flatten_cases),
            ("closure-strip", strip_closure_surface),
            ("adce", dce_changed),
            ("cleanup", tree_opt),
        ];

        for (label, pass) in phases {
            if stats.commits >= WPO_MAX_COMMITS {
                break;
            }
            let summary = wpo_summary(&p);
            let before = fingerprint(&p);
            let (next, ok) = wpo_commit(p, &summary.pure, label, &mut stats, pass);
            p = next;
            if label == "gvn-cse" {
                let summary = wpo_summary(&p);
                let (next, _) = wpo_commit(
                    p,
                    &summary.pure,
                    "partial-inline",
                    &mut stats,
                    wpo_adaptive_inline,
                );
                p = next;
            }
            if ok && fingerprint(&p) != before {
                continue;
            }
        }

        let summary = wpo_summary(&p);
        let (next, _) = wpo_commit(p, &summary.pure, "whole-dce", &mut stats, dce_changed);
        p = next;

        let now = fingerprint(&p);
        if now == before_round || now == last {
            opt_debug!(
                "[WPO] ROUND {} END: fixed point reached; before={:016x}, after={:016x}, previous={:016x}",
                round + 1,
                before_round,
                now,
                last
            );
            break;
        }
        opt_debug!(
            "[WPO] ROUND {} END: state changed; {:016x} -> {:016x}; continuing whole-program analysis",
            round + 1,
            before_round,
            now
        );
        last = now;

        if round + 1 == WPO_ROUNDS {
            break;
        }
    }

    (p, stats)
}

fn print_wpo_stats(before: &Measure, after: &Measure, s: &WpoStats, p: &Program) {
    let summary = wpo_summary(p);
    let mut phase_parts = Vec::new();
    let order = [
        "word-sccp",
        "beta",
        "known-case",
        "instcombine",
        "sccp",
        "global-sccp",
        "jump-thread",
        "gvn-cse",
        "partial-inline",
        "arg-dce",
        "sink-sroa",
        "mega-local",
        "case-fold",
        "closure-strip",
        "adce",
        "cleanup",
        "whole-dce",
    ];
    for name in order {
        if let Some(n) = s.phase_hits.get(name) {
            if *n != 0 {
                phase_parts.push(format!("{}={}", name, n));
            }
        }
    }
    eprintln!(
        "  IPO: {} functions, {} recursive, {} pure, {} SCCs, {} call edges",
        summary.functions.len(),
        summary.recursive.len(),
        summary
            .pure
            .iter()
            .filter(|n| summary.functions.contains_key(*n))
            .count(),
        wpo_sccs(&summary.graph).len(),
        summary.graph.values().map(HashSet::len).sum::<usize>(),
    );
    eprintln!(
        "  rounds: {}  commits: {}  rejected: {}  word folds: {}",
        s.rounds, s.commits, s.rejected, s.const_folds
    );
    if phase_parts.is_empty() {
        eprintln!("  passes: no effective WPO rewrites");
    } else {
        eprintln!("  passes: {}", phase_parts.join("  "));
    }
    let node_delta = after.nodes as isize - before.nodes as isize;
    if node_delta <= 0 {
        eprintln!(
            "  AST: {} -> {} nodes (-{})",
            before.nodes, after.nodes, -node_delta
        );
    } else {
        eprintln!(
            "  AST: {} -> {} nodes (+{})",
            before.nodes, after.nodes, node_delta
        );
    }
}

pub fn run(p: Program) -> Program {
    let start = measure(&p);
    eprintln!("→ optimize: normalize");
    let p_original = p.clone();
    let p = alpha(p);
    let norm = measure(&p);
    let norm_decls = p.decls.len();
    eprintln!(
        "  normalized: {} nodes, {} functions, {} binders",
        norm.nodes, norm.fns, norm.binders
    );

    if !scoped(&p) {
        eprintln!("✗ optimize: initial scope validation failed; original program kept");
        eprintln!("  [run-debug] returning the PRE-alpha program (the alpha'd one is broken)");
        return p_original;
    }

    eprintln!("→ optimize: analyze");
    let analysis = wpo_summary(&p);
    let pure_fns = analysis
        .pure
        .iter()
        .filter(|name| analysis.functions.contains_key(*name))
        .count();
    eprintln!(
        "  whole-program: {} functions, {} SCCs, {} recursive, {} pure, {} call edges",
        analysis.functions.len(),
        wpo_sccs(&analysis.graph).len(),
        analysis.recursive.len(),
        pure_fns,
        analysis.graph.values().map(HashSet::len).sum::<usize>(),
    );

    eprintln!("→ optimize: whole-program / LLVM-class AST");
    let wpo_before = measure(&p);
    let (wpo_p, wpo_stats) = whole_program_optimize(p);
    let wpo_after = measure(&wpo_p);
    print_wpo_stats(&wpo_before, &wpo_after, &wpo_stats, &wpo_p);
    let mut p = wpo_p;
    let mut seen = HashSet::<u64>::new();
    let mut stats = OptStats::default();
    let mut quarantined = HashSet::<String>::new();
    let mut nodes = wpo_after.nodes;
    let mut state = fingerprint(&p);
    seen.insert(state);
    let pass_count = 15usize;

    eprintln!("→ optimize: simplify");
    eprintln!(
        "  pipeline: {} global layers, {} local rewrite layers",
        pass_count, LOCAL_LAYERS
    );
    loop {
        stats.rounds += 1;
        opt_debug!(
            "[LOCAL] SWEEP {} BEGIN: hash={:016x}, nodes={}, fns={}, binders={}",
            stats.rounds,
            fingerprint(&p),
            measure(&p).nodes,
            measure(&p).fns,
            measure(&p).binders
        );
        let summary = wpo_summary(&p);
        let pure = summary.pure;
        let mut restarted = false;

        for pass in 0..pass_count {
            let (label, protect_rec) = match pass {
                0 => ("beta", false),
                1 => ("known-case", false),
                2 => ("tree", false),
                3 => ("const", false),
                4 => ("global-const", false),
                5 => ("param-drop", true),
                6 => ("inline", false),
                7 => ("mega", false),
                8 => ("advanced", false),
                9 => ("sink", false),
                10 => ("dce", false),
                11 => ("global-sccp", false),
                12 => ("jump-thread", false),
                13 => ("word-sccp", false),
                14 => ("whole-dce", false),
                _ => unreachable!(),
            };

            if quarantined.contains(label) {
                opt_debug!(
                    "[LOCAL] pass={} SKIP: already quarantined after a prior failed validation",
                    label
                );
                continue;
            }

            let (candidate, accepted, next_state, next_nodes) = match label {
                "beta" => guard_pass(
                    p,
                    &pure,
                    &mut stats,
                    label,
                    protect_rec,
                    state,
                    nodes,
                    &seen,
                    &mut quarantined,
                    beta,
                ),
                "known-case" => guard_pass(
                    p,
                    &pure,
                    &mut stats,
                    label,
                    protect_rec,
                    state,
                    nodes,
                    &seen,
                    &mut quarantined,
                    known_cases,
                ),
                "tree" => guard_pass(
                    p,
                    &pure,
                    &mut stats,
                    label,
                    protect_rec,
                    state,
                    nodes,
                    &seen,
                    &mut quarantined,
                    tree_opt,
                ),
                "const" => guard_pass(
                    p,
                    &pure,
                    &mut stats,
                    label,
                    protect_rec,
                    state,
                    nodes,
                    &seen,
                    &mut quarantined,
                    const_opt,
                ),
                "global-const" => guard_pass(
                    p,
                    &pure,
                    &mut stats,
                    label,
                    protect_rec,
                    state,
                    nodes,
                    &seen,
                    &mut quarantined,
                    global_const_opt,
                ),
                "param-drop" => guard_pass(
                    p,
                    &pure,
                    &mut stats,
                    label,
                    protect_rec,
                    state,
                    nodes,
                    &seen,
                    &mut quarantined,
                    drop_params,
                ),
                "inline" => guard_pass(
                    p,
                    &pure,
                    &mut stats,
                    label,
                    protect_rec,
                    state,
                    nodes,
                    &seen,
                    &mut quarantined,
                    inline,
                ),
                "mega" => guard_pass(
                    p,
                    &pure,
                    &mut stats,
                    label,
                    protect_rec,
                    state,
                    nodes,
                    &seen,
                    &mut quarantined,
                    mega_opt,
                ),
                "advanced" => guard_pass(
                    p,
                    &pure,
                    &mut stats,
                    label,
                    protect_rec,
                    state,
                    nodes,
                    &seen,
                    &mut quarantined,
                    adv_opt,
                ),
                "sink" => guard_pass(
                    p,
                    &pure,
                    &mut stats,
                    label,
                    protect_rec,
                    state,
                    nodes,
                    &seen,
                    &mut quarantined,
                    sink,
                ),
                "dce" => guard_pass(
                    p,
                    &pure,
                    &mut stats,
                    label,
                    protect_rec,
                    state,
                    nodes,
                    &seen,
                    &mut quarantined,
                    dce_changed,
                ),
                "global-sccp" => guard_pass(
                    p,
                    &pure,
                    &mut stats,
                    label,
                    protect_rec,
                    state,
                    nodes,
                    &seen,
                    &mut quarantined,
                    global_const_opt,
                ),
                "jump-thread" => guard_pass(
                    p,
                    &pure,
                    &mut stats,
                    label,
                    protect_rec,
                    state,
                    nodes,
                    &seen,
                    &mut quarantined,
                    adv_opt,
                ),
                "word-sccp" => guard_pass(
                    p,
                    &pure,
                    &mut stats,
                    label,
                    protect_rec,
                    state,
                    nodes,
                    &seen,
                    &mut quarantined,
                    const_opt,
                ),
                "whole-dce" => guard_pass(
                    p,
                    &pure,
                    &mut stats,
                    label,
                    protect_rec,
                    state,
                    nodes,
                    &seen,
                    &mut quarantined,
                    dce_changed,
                ),
                _ => unreachable!(),
            };

            p = candidate;
            if accepted {
                opt_debug!(
                    "[LOCAL] pass={} COMMIT: restart optimizer from new state hash={:016x}, nodes={} -> {}; accepted_restarts={}",
                    label,
                    next_state,
                    nodes,
                    next_nodes,
                    stats.restarts + 1
                );
                state = next_state;
                nodes = next_nodes;
                stats.restarts += 1;
                restarted = true;
                break;
            } else {
                opt_debug!(
                    "[LOCAL] pass={} KEEP: original state preserved; next pass may still examine it",
                    label
                );
            }
        }

        if !restarted {
            opt_debug!(
                "[LOCAL] SWEEP {} END: no pass committed; fixed point at hash={:016x}",
                stats.rounds,
                fingerprint(&p)
            );
            break;
        }
        opt_debug!(
            "[LOCAL] SWEEP {} END: committed pass forced restart; new hash={:016x}",
            stats.rounds,
            fingerprint(&p)
        );
    }

    stats.quarantined = quarantined;
    eprintln!("  sweeps: {}", stats.rounds);
    eprintln!("  restarts: {}", stats.restarts);
    eprintln!("  fixed point: no further rewrites");
    print_pass_stats(&stats);

    eprintln!("→ optimize: finalize");

    let p = match crate::sema::analyze(p.clone()) {
        Ok(p) => p,
        Err(errs) => {
            for e in &errs {
                eprintln!(
                    "  [opt-final] semantic invariant: {} (span {}..{})",
                    e.msg, e.span.lo, e.span.hi
                );
            }
            return p;
        }
    };
    let final_ok = scoped(&p);
    let end = measure(&p);
    opt_debug!(
        "[VERIFY] final scope={} hash={:016x} nodes={} fns={} binders={} decls={}",
        final_ok,
        fingerprint(&p),
        end.nodes,
        end.fns,
        end.binders,
        p.decls.len()
    );
    if final_ok {
        let delta = end.nodes as isize - norm.nodes as isize;
        let pct = pct_delta(norm.nodes, end.nodes).abs();
        if delta <= 0 {
            eprintln!(
                "✓ optimize: reduced {} -> {} nodes (-{} / {:.1}%), {} -> {} functions, {} -> {} declarations",
                norm.nodes,
                end.nodes,
                -delta,
                pct,
                norm.fns,
                end.fns,
                norm_decls,
                p.decls.len()
            );
        } else {
            eprintln!(
                "✓ optimize: grew {} -> {} nodes (+{} / {:.1}%), {} -> {} functions, {} -> {} declarations",
                norm.nodes,
                end.nodes,
                delta,
                pct,
                norm.fns,
                end.fns,
                norm_decls,
                p.decls.len()
            );
        }
        if start.nodes != norm.nodes {
            eprintln!("  normalization: {} -> {} nodes", start.nodes, norm.nodes);
        }
    } else {
        eprintln!("✗ optimize: final scope invariant failed; returning current AST");
    }
    p
}

const HASH_OFFSET: u64 = 0xcbf29ce484222325;
const HASH_PRIME: u64 = 0x100000001b3;

#[inline(always)]
fn hash_byte(h: &mut u64, b: u8) {
    *h ^= b as u64;
    *h = h.wrapping_mul(HASH_PRIME);
}

#[inline(always)]
fn hash_u64(h: &mut u64, x: u64) {
    for b in x.to_le_bytes() {
        hash_byte(h, b);
    }
}

#[inline(always)]
fn hash_str(h: &mut u64, s: &str) {
    hash_u64(h, s.len() as u64);
    for &b in s.as_bytes() {
        hash_byte(h, b);
    }
}

fn hash_pat(h: &mut u64, p: &Pat) {
    match p {
        Pat::Wild => hash_byte(h, 0),
        Pat::Bind(n) => {
            hash_byte(h, 1);
            hash_str(h, n);
        }
        Pat::Int(v) => {
            hash_byte(h, 2);
            hash_u64(h, *v);
        }
        Pat::Str(v) => {
            hash_byte(h, 3);
            hash_u64(h, v.len() as u64);
            for &b in v {
                hash_byte(h, b);
            }
        }
        Pat::Ctor { name, args } => {
            hash_byte(h, 4);
            hash_str(h, name);
            hash_u64(h, args.len() as u64);
            for a in args {
                hash_pat(h, a);
            }
        }
        Pat::Tuple(xs) => {
            hash_byte(h, 5);
            hash_u64(h, xs.len() as u64);
            for x in xs {
                hash_pat(h, x);
            }
        }
    }
}

fn hash_ty(h: &mut u64, t: &Ty) {
    match t {
        Ty::Prim(p) => {
            hash_byte(h, 0);
            hash_u64(h, *p as u64);
        }
        Ty::Named(n) => {
            hash_byte(h, 1);
            hash_str(h, n);
        }
        Ty::App(n, xs) => {
            hash_byte(h, 2);
            hash_str(h, n);
            hash_u64(h, xs.len() as u64);
            for x in xs {
                hash_ty(h, x);
            }
        }
        Ty::Array(x) => {
            hash_byte(h, 3);
            hash_ty(h, x);
        }
        Ty::Tuple(xs) => {
            hash_byte(h, 4);
            hash_u64(h, xs.len() as u64);
            for x in xs {
                hash_ty(h, x);
            }
        }
        Ty::Fun(a, b) => {
            hash_byte(h, 5);
            hash_ty(h, a);
            hash_ty(h, b);
        }
        Ty::Bottom => hash_byte(h, 6),
    }
}

fn hash_foreign(h: &mut u64, t: &ForeignTy) {
    match t {
        ForeignTy::Atom(a) => {
            hash_byte(h, 0);
            hash_u64(
                h,
                match a {
                    ForeignAtom::I8 => 0,
                    ForeignAtom::I16 => 1,
                    ForeignAtom::I32 => 2,
                    ForeignAtom::I64 => 3,
                    ForeignAtom::U8 => 4,
                    ForeignAtom::U16 => 5,
                    ForeignAtom::U32 => 6,
                    ForeignAtom::U64 => 7,
                    ForeignAtom::F32 => 8,
                    ForeignAtom::F64 => 9,
                    ForeignAtom::Ptr => 10,
                    ForeignAtom::Void => 11,
                    ForeignAtom::Named(_) => 12,
                    ForeignAtom::PtrTo(_) => 13,
                },
            );
            match a {
                ForeignAtom::Named(n) => hash_str(h, n),
                ForeignAtom::PtrTo(x) => hash_foreign(h, x),
                _ => {}
            }
        }
        ForeignTy::Fun { args, ret } => {
            hash_byte(h, 1);
            hash_u64(h, args.len() as u64);
            for a in args {
                match a {
                    ForeignArg::Ty(t) => {
                        hash_byte(h, 0);
                        hash_foreign(h, t);
                    }
                    ForeignArg::VarArgs => hash_byte(h, 1),
                }
            }
            hash_foreign(h, ret);
        }
    }
}

fn hash_expr(h: &mut u64, e: &Expr) {
    match &e.kind {
        ExprKind::Int(v) => {
            hash_byte(h, 0);
            hash_u64(h, *v);
        }
        ExprKind::Float(v) => {
            hash_byte(h, 1);
            hash_u64(h, v.to_bits());
        }
        ExprKind::Str(v) => {
            hash_byte(h, 2);
            hash_u64(h, v.len() as u64);
            for &b in v {
                hash_byte(h, b);
            }
        }
        ExprKind::Var(n) => {
            hash_byte(h, 3);
            hash_str(h, n);
        }
        ExprKind::Let { name, value, body } => {
            hash_byte(h, 4);
            hash_str(h, name);
            hash_expr(h, value);
            hash_expr(h, body);
        }
        ExprKind::Fn { params, body } => {
            hash_byte(h, 5);
            hash_u64(h, params.len() as u64);
            for p in params {
                hash_str(h, p);
            }
            hash_expr(h, body);
        }
        ExprKind::App { fun, args } => {
            hash_byte(h, 6);
            hash_expr(h, fun);
            hash_u64(h, args.len() as u64);
            for a in args {
                hash_expr(h, a);
            }
        }
        ExprKind::Case { scrut, arms } => {
            hash_byte(h, 7);
            hash_expr(h, scrut);
            hash_u64(h, arms.len() as u64);
            for a in arms {
                hash_pat(h, &a.pat);
                hash_expr(h, &a.body);
            }
        }
        ExprKind::Block { stmts, tail } => {
            hash_byte(h, 8);
            hash_u64(h, stmts.len() as u64);
            for s in stmts {
                hash_expr(h, s);
            }
            hash_expr(h, tail);
        }
        ExprKind::Bin { op, l, r } => {
            hash_byte(h, 9);
            hash_u64(h, *op as u64);
            hash_expr(h, l);
            hash_expr(h, r);
        }
        ExprKind::Un { op, e } => {
            hash_byte(h, 10);
            hash_u64(h, *op as u64);
            hash_expr(h, e);
        }
        ExprKind::Quote(e) => {
            hash_byte(h, 11);
            hash_expr(h, e);
        }
    }
}

fn hash_program(p: &Program) -> u64 {
    let mut h = HASH_OFFSET;
    hash_u64(&mut h, p.decls.len() as u64);
    for d in &p.decls {
        match d {
            Decl::Type {
                name,
                params,
                ctors,
                ..
            } => {
                hash_byte(&mut h, 0);
                hash_str(&mut h, name);
                hash_u64(&mut h, params.len() as u64);
                for p in params {
                    hash_str(&mut h, p);
                }
                hash_u64(&mut h, ctors.len() as u64);
                for c in ctors {
                    hash_str(&mut h, &c.name);
                    hash_u64(&mut h, c.args.len() as u64);
                    for a in &c.args {
                        hash_ty(&mut h, a);
                    }
                }
            }
            Decl::Let { name, value, .. } => {
                hash_byte(&mut h, 1);
                hash_str(&mut h, name);
                hash_expr(&mut h, value);
            }
            Decl::External(e) => {
                hash_byte(&mut h, 2);
                hash_str(&mut h, &e.name);
                hash_ty(&mut h, &e.mak_ty);
                hash_str(&mut h, &e.symbol);
                hash_byte(&mut h, e.is_global as u8);
                hash_foreign(&mut h, &e.foreign);
                hash_byte(&mut h, e.pure as u8);
                match &e.from {
                    Some(x) => {
                        hash_byte(&mut h, 1);
                        hash_str(&mut h, x);
                    }
                    None => hash_byte(&mut h, 0),
                }
                match &e.abi {
                    Some(x) => {
                        hash_byte(&mut h, 1);
                        hash_str(&mut h, x);
                    }
                    None => hash_byte(&mut h, 0),
                }
            }
            Decl::Bring(xs) => {
                hash_byte(&mut h, 3);
                hash_u64(&mut h, xs.len() as u64);
                for x in xs {
                    hash_str(&mut h, x);
                }
            }
        }
    }
    hash_expr(&mut h, &p.body);
    h
}

fn fingerprint(p: &Program) -> u64 {
    hash_program(p)
}

fn rec_shape(p: &Program) -> HashMap<String, usize> {
    let recursive = recursive_fns(p);
    let mut out = HashMap::new();
    for d in &p.decls {
        let Decl::Let { name, value, .. } = d else {
            continue;
        };
        if !recursive.contains(name) {
            continue;
        }
        if let ExprKind::Fn { params, .. } = &value.kind {
            out.insert(name.clone(), params.len());
        }
    }
    out
}

fn guard_pass<F>(
    p: Program,
    pure: &HashSet<String>,
    stats: &mut OptStats,
    label: &str,
    protect_rec: bool,
    before_hash: u64,
    before_nodes: usize,
    seen: &HashSet<u64>,
    quarantined: &mut HashSet<String>,
    f: F,
) -> (Program, bool, u64, usize)
where
    F: FnOnce(Program, &HashSet<String>, &mut bool) -> Program,
{
    let original = p;
    let before_rec = if protect_rec {
        Some(rec_shape(&original))
    } else {
        None
    };
    let mut pass_changed = false;
    let candidate = f(original.clone(), pure, &mut pass_changed);

    opt_debug!(
        "[LOCAL] pass={} BEGIN: hash={:016x}, nodes={}, protect_rec={}, quarantined=false",
        label,
        before_hash,
        before_nodes,
        protect_rec
    );

    if !pass_changed {
        opt_debug!(
            "[LOCAL] pass={} KEEP: transform reported changed=false; no candidate AST checks performed",
            label
        );
        return (original, false, before_hash, before_nodes);
    }

    let after_hash = fingerprint(&candidate);
    opt_debug_delta!(&format!("[LOCAL] {}", label), &original, &candidate);

    let quarantine =
        |stats: &mut OptStats, quarantined: &mut HashSet<String>, label: &str, reason: &str| {
            stats.rejected += 1;
            quarantined.insert(label.to_string());
            opt_debug!(
                "[LOCAL] pass={} REVERT + QUARANTINE: reason={}; total_rejected={} ",
                label,
                reason,
                stats.rejected
            );
        };

    if after_hash == before_hash {
        quarantine(stats, quarantined, label, "NO_STRUCTURAL_CHANGE");
        return (original, false, before_hash, before_nodes);
    }

    if !scoped(&candidate) {
        quarantine(stats, quarantined, label, "SCOPE_INVARIANT");
        return (original, false, before_hash, before_nodes);
    }

    let _ = (opt_cost(&original), opt_cost(&candidate));

    if protect_rec {
        let before_rec = before_rec.as_ref().unwrap();
        let after_rec = rec_shape(&candidate);
        if after_rec != *before_rec {
            quarantine(stats, quarantined, label, "RECURSION_SHAPE_CHANGED");
            #[cfg(debug_assertions)]
            eprintln!(
                "  [opt-debug] [LOCAL] pass={} recursion-shape changed: before={:?} after={:?}",
                label, before_rec, after_rec
            );
            return (original, false, before_hash, before_nodes);
        }
    }

    if seen.contains(&after_hash) {
        quarantine(stats, quarantined, label, "ALREADY_SEEN_STATE_CYCLE");
        return (original, false, before_hash, before_nodes);
    }

    let after_nodes = measure(&candidate).nodes;
    let entry = stats.passes.entry(label.to_string()).or_default();
    entry.hits += 1;
    entry.delta += before_nodes as isize - after_nodes as isize;
    opt_debug!(
        "[LOCAL] pass={} ACCEPT: AST invariants ok; cycle=none; delta_nodes={:+}; pass_hits={}",
        label,
        after_nodes as isize - before_nodes as isize,
        entry.hits
    );
    (candidate, true, after_hash, after_nodes)
}

fn scoped(p: &Program) -> bool {
    let mut globals = HashSet::new();
    let mut ctors = HashSet::new();

    for d in &p.decls {
        match d {
            Decl::Let { name, .. } => {
                globals.insert(name.clone());
            }
            Decl::External(e) => {
                globals.insert(e.name.clone());
            }
            Decl::Type { ctors: cs, .. } => {
                for c in cs {
                    ctors.insert(c.name.clone());
                }
            }
            Decl::Bring(_) => {}
        }
    }

    {
        let mut gs: Vec<&String> = globals.iter().collect();
        gs.sort();
        eprintln!("  [scoped-debug] globals ({}): {:?}", gs.len(), gs);

        let mut cs: Vec<&String> = ctors.iter().collect();
        cs.sort();
        eprintln!("  [scoped-debug] ctors   ({}): {:?}", cs.len(), cs);
    }

    fn pat_binds(p: &Pat, bound: &mut HashSet<String>) {
        match p {
            Pat::Bind(n) => {
                bound.insert(n.clone());
            }
            Pat::Ctor { args, .. } => {
                for a in args {
                    pat_binds(a, bound);
                }
            }
            Pat::Tuple(xs) => {
                for x in xs {
                    pat_binds(x, bound);
                }
            }
            Pat::Wild | Pat::Int(_) | Pat::Str(_) => {}
        }
    }

    fn expr_ok(
        e: &Expr,
        bound: &mut HashSet<String>,
        globals: &HashSet<String>,
        ctors: &HashSet<String>,
    ) -> bool {
        match &e.kind {
            ExprKind::Int(_) | ExprKind::Float(_) | ExprKind::Str(_) | ExprKind::Quote(_) => true,
            ExprKind::Var(n) => {
                // `$mak.index` is emitted by the parser for `base[index]`.
                // It deliberately has no Decl entry and must survive alpha
                // renaming as an intrinsic. Do not make other `$mak.*` names
                // valid: only explicitly listed compiler intrinsics bypass
                // ordinary scope lookup.
                let is_intrinsic = is_builtin_intrinsic(n);
                let in_scope = bound.contains(n);
                let in_globals = globals.contains(n);
                let in_ctors = ctors.contains(n);
                let ok = is_intrinsic || in_scope || in_globals || in_ctors;

                if !ok {
                    eprintln!(
                        "  [scoped-debug] FAIL: unbound var `{}` at span {}..{} \
                         (in_scope={} in_globals={} in_ctors={} is_intrinsic={} local_bound_count={})",
                        n,
                        e.span.lo,
                        e.span.hi,
                        in_scope,
                        in_globals,
                        in_ctors,
                        is_intrinsic,
                        bound.len(),
                    );
                } else if is_intrinsic {
                    eprintln!(
                        "  [scoped-debug] OK: compiler intrinsic `{}` at span {}..{}",
                        n, e.span.lo, e.span.hi
                    );
                }

                ok
            }
            ExprKind::Let { name, value, body } => {
                // A let name is not in scope within its own initializer.
                if !expr_ok(value, bound, globals, ctors) {
                    return false;
                }

                let was = bound.insert(name.clone());
                let ok = expr_ok(body, bound, globals, ctors);

                if !was {
                    bound.remove(name);
                }

                ok
            }
            ExprKind::Fn { params, body } => {
                // Reject duplicate parameters rather than letting the scope
                // validator silently collapse them into one set entry.
                let mut param_names = HashSet::with_capacity(params.len());
                if params.iter().any(|p| !param_names.insert(p.clone())) {
                    eprintln!("  [scoped-debug] FAIL: duplicate function parameter");
                    return false;
                }

                let mut added = Vec::new();
                for p in params {
                    if bound.insert(p.clone()) {
                        added.push(p.clone());
                    }
                }

                let ok = expr_ok(body, bound, globals, ctors);
                for n in added {
                    bound.remove(&n);
                }
                ok
            }
            ExprKind::App { fun, args } => {
                expr_ok(fun, bound, globals, ctors)
                    && args.iter().all(|a| expr_ok(a, bound, globals, ctors))
            }
            ExprKind::Case { scrut, arms } => {
                if !expr_ok(scrut, bound, globals, ctors) {
                    return false;
                }

                for arm in arms {
                    let mut arm_bound = bound.clone();
                    pat_binds(&arm.pat, &mut arm_bound);
                    if !expr_ok(&arm.body, &mut arm_bound, globals, ctors) {
                        return false;
                    }
                }
                true
            }
            ExprKind::Block { stmts, tail } => {
                stmts.iter().all(|s| expr_ok(s, bound, globals, ctors))
                    && expr_ok(tail, bound, globals, ctors)
            }
            ExprKind::Bin { l, r, .. } => {
                expr_ok(l, bound, globals, ctors) && expr_ok(r, bound, globals, ctors)
            }
            ExprKind::Un { e, .. } => expr_ok(e, bound, globals, ctors),
        }
    }

    for d in &p.decls {
        if let Decl::Let { value, name, .. } = d {
            let mut bound = HashSet::new();
            if !expr_ok(value, &mut bound, &globals, &ctors) {
                eprintln!("  [scoped-debug] declaration `{}` failed scope validation", name);
                return false;
            }
        }
    }

    expr_ok(&p.body, &mut HashSet::new(), &globals, &ctors)
}

// =============================================================================
// Alpha-renaming (obfuscator)
// =============================================================================
//
// Design:
//
//   1. INJECTIVE per namespace.  `alpha_name` is a genuine bijection
//      N -> nonempty strings over a fixed alphabet (bijective base
//      conversion).  Two distinct counters therefore never collide.
//
//   2. DISJOINT across namespaces.  Values live over the lowercase
//      consonant alphabet, types are that alphabet prefixed with '_',
//      constructors live over the uppercase consonant alphabet.  The
//      three output sets are pairwise disjoint, so no value can shadow
//      a type, no type can shadow a ctor, etc. even after cgen.rs
//      mangles the names again.
//
//   3. RESERVED-KEYWORD SAFETY.  The lowercase alphabet excludes every
//      vowel, which already rules out every vowel-containing keyword
//      ("int", "if", "for", "fn", "as", "and", ...).  The three
//      remaining letter-only keywords that our alphabet could spell
//      are filtered explicitly.
//
//   4. TRANSPARENT INTO QUOTE.  cgen.rs lowers `ExprKind::Quote(x)` as
//      an ordinary expression, so identifiers inside a quote must be
//      renamed consistently with the rest of the program.  We recurse.
//
//   5. GLOBALLY FRESH BINDERS.  Every local binder (Let, Fn parameter,
//      Case-arm binder) draws from the SAME value namespace as the
//      top-level values, so shadowing disappears on output and no
//      downstream pass has to be scope-aware.

/// Bijection N -> nonempty strings over the lowercase consonants.
/// Never returns a C or Rust reserved keyword.
fn alpha_name(mut n: usize) -> String {
    // Bijective base-26 over the full lowercase alphabet.  Emits, in order:
    //     a, b, c, ..., z, aa, ab, ..., az, ba, bb, ..., zz, aaa, ...
    // No vowels are excluded anymore, so we must filter every name that
    // could land on a keyword in MAK, Rust, or C.  When a candidate is
    // reserved we simply advance n by one and retry.
    const A: &[u8] = b"abcdefghijklmnopqrstuvwxyz";
    const RESERVED: &[&str] = &[
        // --- MAK surface keywords -----------------------------------------
        "fn", "as", "and", "or", "not", "if", "else", "case", "of", "let", "in", "type", "external",
        "bring", "pure", "from", "true", "false", "then", "do", "return",
        // --- Rust keywords (safety net if cgen inlines anything) ----------
        "use", "mod", "pub", "crate", "self", "super", "move", "ref", "mut", "static", "const",
        "where", "unsafe", "async", "await", "dyn", "box", "match", "loop", "while", "for",
        "break", "continue", "struct", "enum", "impl", "trait", "yield",
        // --- C keywords (cgen emits C) ------------------------------------
        "int", "char", "long", "short", "float", "double", "void", "signed", "unsigned", "register",
        "auto", "extern", "volatile", "restrict", "inline", "goto", "switch", "default", "sizeof",
        "typedef", "union",
    ];
    let base = A.len();

    loop {
        let original = n;
        let mut rev = Vec::with_capacity(8);
        let mut m = n;
        loop {
            rev.push(A[m % base]);
            if m < base {
                break;
            }
            m = m / base - 1;
        }
        rev.reverse();
        let candidate = String::from_utf8(rev).expect("static alphabet");
        if !RESERVED.iter().any(|&kw| kw == candidate) {
            return candidate;
        }
        n = original + 1;
    }
}

/// Type namespace.  The leading '_' guarantees disjointness from the
/// value namespace (which never contains '_').
fn type_name(n: usize) -> String {
    format!("_{}", alpha_name(n))
}

/// Constructor namespace.  Entirely uppercase, disjoint from both the
/// value namespace and the type namespace.
fn ctor_name(n: usize) -> String {
    // Bijective base-26 over the full uppercase alphabet.  Emits, in order:
    //     A, B, C, ..., Z, AA, AB, ..., ZZ, AAA, ...
    const A: &[u8] = b"ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    let base = A.len();
    let mut rev = Vec::with_capacity(8);
    let mut m = n;
    loop {
        rev.push(A[m % base]);
        if m < base {
            break;
        }
        m = m / base - 1;
    }
    rev.reverse();
    String::from_utf8(rev).expect("static alphabet")
}

/// Monotone fresh-name allocator for the value namespace.
///
/// Invariant: calling `fresh()` k times yields k pairwise-distinct
/// strings, all distinct from anything previously passed to `reserve`.
/// Because `alpha_name` is bijective this never loops more than once
/// (modulo the three filtered reserved keywords).
struct ValueNames {
    counter: usize,
    used: HashSet<String>,
}

impl ValueNames {
    fn new() -> Self {
        Self {
            counter: 0,
            used: HashSet::new(),
        }
    }

    fn reserve(&mut self, name: String) {
        self.used.insert(name);
    }

    fn fresh(&mut self) -> String {
        loop {
            let n = self.counter;
            self.counter = self
                .counter
                .checked_add(1)
                .expect("alpha name counter overflow");
            let candidate = alpha_name(n);
            if self.used.insert(candidate.clone()) {
                return candidate;
            }
        }
    }
}

fn alpha(p: Program) -> Program {
    {
        eprintln!(
            "  [alpha-debug] ========== INPUT ({} decls) ==========",
            p.decls.len()
        );
        for d in &p.decls {
            match d {
                Decl::Type { name, ctors, .. } => {
                    eprintln!(
                        "  [alpha-debug]   type `{}` {{ {} }}",
                        name,
                        ctors
                            .iter()
                            .map(|c| c.name.as_str())
                            .collect::<Vec<_>>()
                            .join(", "),
                    );
                }
                Decl::Let { name, .. } => eprintln!("  [alpha-debug]   let `{}`", name),
                Decl::External(e) => eprintln!("  [alpha-debug]   external `{}`", e.name),
                Decl::Bring(_) => eprintln!("  [alpha-debug]   bring ..."),
            }
        }
    }

    // Collect all reserved names (externals) to prevent collisions
    let mut reserved: HashSet<String> = HashSet::new();
    for d in &p.decls {
        if let Decl::External(e) = d {
            reserved.insert(e.name.clone());
        }
    }

    let mut values = ValueNames::new();
    // Reserve external names so they are never generated by fresh()
    for r in &reserved {
        values.reserve(r.clone());
    }

    let mut top = HashMap::<String, String>::new();
    let mut type_map = HashMap::<String, String>::new();
    let mut ctor_map = HashMap::<(String, String), String>::new();
    let mut next_type = 0usize;
    let mut next_ctor = 0usize;
    let mut used_type_names = HashSet::<String>::new();
    let mut used_ctor_names = HashSet::<String>::new();

    // -----------------------------------------------------------------
    // Pass 1: allocate a fresh name for every top-level declaration.
    //
    // `entry(..).or_insert_with(..)` (not `insert`) is deliberate: if
    // two declarations ever share a source name (legal in some MAK
    // encodings for constructors across types), we keep the FIRST
    // allocation and reuse it, rather than silently rewriting it on
    // the second occurrence.
    // -----------------------------------------------------------------
    for d in &p.decls {
        match d {
            Decl::Let { name, .. } => {
                top.entry(name.clone()).or_insert_with(|| values.fresh());
            }
            Decl::External(e) => {
                // Externals keep their original name or get a fresh one if
                // we wanted to rename them, but here we just ensure they
                // are in the map. Since we reserved them, values.fresh()
                // won't return them. However, we usually want externals
                // to keep their MAK name if it's not conflicting, or
                // just map them to themselves if they are already safe.
                // The current logic maps them to a fresh name.
                // To keep external names stable, we can do:
                top.entry(e.name.clone()).or_insert_with(|| e.name.clone());
            }
            Decl::Type { name, ctors, .. } => {
                type_map.entry(name.clone()).or_insert_with(|| {
                    loop {
                        let tn = type_name(next_type);
                        next_type += 1;
                        if used_type_names.insert(tn.clone()) {
                            return tn;
                        }
                    }
                });
                for c in ctors {
                    ctor_map
                        .entry((name.clone(), c.name.clone()))
                        .or_insert_with(|| {
                            loop {
                                let cn = ctor_name(next_ctor);
                                next_ctor += 1;
                                if used_ctor_names.insert(cn.clone()) {
                                    return cn;
                                }
                            }
                        });
                }
            }
            Decl::Bring(_) => {}
        }
    }
    // Source ctor name -> Some(renamed) when the name is declared by
    // exactly one type; None when the name is ambiguous (declared by
    // more than one type).  Ambiguous ctor names are left untouched so
    // that whatever resolution cgen already performs for the original
    // program is preserved; alpha cannot disambiguate them without
    // type information.
    let mut ctor_ren = HashMap::<String, Option<String>>::new();
    for ((_ty, ctor), new) in &ctor_map {
        match ctor_ren.get(ctor) {
            None => {
                ctor_ren.insert(ctor.clone(), Some(new.clone()));
            }
            Some(Some(_)) => {
                ctor_ren.insert(ctor.clone(), None);
            }
            Some(None) => {}
        }
    }

    {
        let mut amb: Vec<&String> = ctor_ren
            .iter()
            .filter(|(_, v)| v.is_none())
            .map(|(k, _)| k)
            .collect();
        amb.sort();
        eprintln!(
            "  [alpha-debug] ctor_ren: {} source ctors total; {} AMBIGUOUS (kept original): {:?}",
            ctor_ren.len(),
            amb.len(),
            amb,
        );
        let mut uniq: Vec<(&String, &String)> = ctor_ren
            .iter()
            .filter_map(|(k, v)| v.as_ref().map(|n| (k, n)))
            .collect();
        uniq.sort();
        eprintln!(
            "  [alpha-debug] ctor_ren: {} unique source ctors will be renamed:",
            uniq.len()
        );
        for (k, v) in &uniq {
            eprintln!("  [alpha-debug]   `{}` -> `{}`", k, v);
        }
    }

    // -----------------------------------------------------------------
    // Name lookup: innermost scope wins, then top-level, then ctors.
    // -----------------------------------------------------------------
    fn lookup(
        env: &[(String, String)],
        top: &HashMap<String, String>,
        ctor_ren: &HashMap<String, Option<String>>,
        n: &str,
    ) -> String {
        for (old, new) in env.iter().rev() {
            if old == n {
                return new.clone();
            }
        }
        if let Some(nn) = top.get(n) {
            return nn.clone();
        }
        if let Some(Some(nn)) = ctor_ren.get(n) {
            return nn.clone();
        }
        n.to_string()
    }

    fn ren_ty(t: Ty, type_map: &HashMap<String, String>) -> Ty {
        match t {
            Ty::Named(n) => Ty::Named(type_map.get(&n).cloned().unwrap_or(n)),
            Ty::App(n, args) => Ty::App(
                type_map.get(&n).cloned().unwrap_or(n),
                args.into_iter().map(|x| ren_ty(x, type_map)).collect(),
            ),
            Ty::Array(a) => Ty::Array(Box::new(ren_ty(*a, type_map))),
            Ty::Tuple(ts) => Ty::Tuple(ts.into_iter().map(|x| ren_ty(x, type_map)).collect()),
            Ty::Fun(a, b) => Ty::Fun(
                Box::new(ren_ty(*a, type_map)),
                Box::new(ren_ty(*b, type_map)),
            ),
            other => other,
        }
    }

    fn ren_pat(p: Pat, mapped: &[String], ctor_ren: &HashMap<String, Option<String>>) -> Pat {
        // The `mapped` slice is produced by `pat_binders` in the SAME
        // preorder traversal that `ren_pat` will consume it in, so we
        // never need a scope map here.
        let mut it = mapped.iter();
        fn go(
            p: Pat,
            it: &mut std::slice::Iter<'_, String>,
            ctor_ren: &HashMap<String, Option<String>>,
        ) -> Pat {
            match p {
                Pat::Wild => Pat::Wild,
                Pat::Bind(_) => Pat::Bind(it.next().cloned().unwrap()),
                Pat::Int(v) => Pat::Int(v),
                Pat::Str(b) => Pat::Str(b),
                Pat::Ctor { name, args } => Pat::Ctor {
                    name: match ctor_ren.get(&name) {
                        Some(Some(nn)) => nn.clone(),
                        _ => name,
                    },
                    args: args.into_iter().map(|a| go(a, it, ctor_ren)).collect(),
                },
                Pat::Tuple(xs) => Pat::Tuple(xs.into_iter().map(|x| go(x, it, ctor_ren)).collect()),
            }
        }
        go(p, &mut it, ctor_ren)
    }

    fn go(
        e: Expr,
        env: &mut Vec<(String, String)>,
        top: &HashMap<String, String>,
        ctor_ren: &HashMap<String, Option<String>>,
        values: &mut ValueNames,
    ) -> Expr {
        let sp = e.span;
        match e.kind {
            ExprKind::Var(n) => Expr {
                span: sp,
                kind: ExprKind::Var(lookup(env, top, ctor_ren, &n)),
            },
            ExprKind::Let { name, value, body } => {
                let value = go(*value, env, top, ctor_ren, values);
                let new_name = values.fresh();
                env.push((name, new_name.clone()));
                let body = go(*body, env, top, ctor_ren, values);
                env.pop();
                Expr {
                    span: sp,
                    kind: ExprKind::Let {
                        name: new_name,
                        value: Box::new(value),
                        body: Box::new(body),
                    },
                }
            }
            ExprKind::Fn { params, body } => {
                let old = env.len();
                let mut new_params = Vec::with_capacity(params.len());
                for p in params {
                    let nn = values.fresh();
                    env.push((p, nn.clone()));
                    new_params.push(nn);
                }
                let body = go(*body, env, top, ctor_ren, values);
                env.truncate(old);
                Expr {
                    span: sp,
                    kind: ExprKind::Fn {
                        params: new_params,
                        body: Box::new(body),
                    },
                }
            }
            ExprKind::App { fun, args } => Expr {
                span: sp,
                kind: ExprKind::App {
                    fun: Box::new(go(*fun, env, top, ctor_ren, values)),
                    args: args
                        .into_iter()
                        .map(|a| go(a, env, top, ctor_ren, values))
                        .collect(),
                },
            },
            ExprKind::Case { scrut, arms } => {
                let scrut = go(*scrut, env, top, ctor_ren, values);
                let mut out = Vec::with_capacity(arms.len());
                for arm in arms {
                    let old = env.len();
                    let mut binders = Vec::new();
                    pat_binders(&arm.pat, &mut binders);
                    let mut mapped = Vec::with_capacity(binders.len());
                    for b in binders {
                        let nn = values.fresh();
                        env.push((b, nn.clone()));
                        mapped.push(nn);
                    }
                    let pat = ren_pat(arm.pat, &mapped, ctor_ren);
                    let body = go(arm.body, env, top, ctor_ren, values);
                    env.truncate(old);
                    out.push(Arm {
                        pat,
                        body,
                        span: arm.span,
                    });
                }
                Expr {
                    span: sp,
                    kind: ExprKind::Case {
                        scrut: Box::new(scrut),
                        arms: out,
                    },
                }
            }
            ExprKind::Block { stmts, tail } => Expr {
                span: sp,
                kind: ExprKind::Block {
                    stmts: stmts
                        .into_iter()
                        .map(|s| go(s, env, top, ctor_ren, values))
                        .collect(),
                    tail: Box::new(go(*tail, env, top, ctor_ren, values)),
                },
            },
            ExprKind::Bin { op, l, r } => Expr {
                span: sp,
                kind: ExprKind::Bin {
                    op,
                    l: Box::new(go(*l, env, top, ctor_ren, values)),
                    r: Box::new(go(*r, env, top, ctor_ren, values)),
                },
            },
            ExprKind::Un { op, e } => Expr {
                span: sp,
                kind: ExprKind::Un {
                    op,
                    e: Box::new(go(*e, env, top, ctor_ren, values)),
                },
            },
            // cgen.rs lowers Quote transparently, so identifiers inside
            // a quote must be renamed consistently with the enclosing
            // scope.  Previously this arm returned the quote unchanged.
            ExprKind::Quote(x) => Expr {
                span: sp,
                kind: ExprKind::Quote(Box::new(go(*x, env, top, ctor_ren, values))),
            },
            k => Expr { span: sp, kind: k },
        }
    }

    // -----------------------------------------------------------------
    // Pass 2: rewrite every declaration in place.
    // -----------------------------------------------------------------
    let mut decls = Vec::with_capacity(p.decls.len());
    for d in p.decls {
        match d {
            Decl::Let { name, value, span } => {
                let new_name = top.get(&name).cloned().unwrap_or(name);
                decls.push(Decl::Let {
                    name: new_name,
                    value: Box::new(go(*value, &mut Vec::new(), &top, &ctor_ren, &mut values)),
                    span,
                });
            }
            Decl::External(mut e) => {
                // Keep external name as is, or map it if it was in top
                if let Some(nn) = top.get(&e.name) {
                    e.name = nn.clone();
                }
                e.mak_ty = ren_ty(e.mak_ty, &type_map);
                decls.push(Decl::External(e));
            }
            Decl::Type {
                name,
                params,
                ctors,
                span,
            } => {
                let new_name = type_map.get(&name).cloned().unwrap_or(name.clone());
                eprintln!("  [alpha-debug] Type `{}` -> `{}`", name, new_name);
                let new_ctors: Vec<Ctor> = ctors
                    .into_iter()
                    .map(|c| {
                        let old_cname = c.name.clone();
                        // IMPORTANT: use `ctor_ren`, NOT `ctor_map`.  See the
                        // long comment above alpha() for why.
                        let cn = match ctor_ren.get(&c.name) {
                            Some(Some(nn)) => nn.clone(),
                            _ => c.name,
                        };
                        if cn == old_cname {
                            eprintln!(
                                "  [alpha-debug]   ctor `{}.{}` KEPT (ambiguous or absent from ctor_ren)",
                                name, old_cname,
                            );
                        } else {
                            eprintln!(
                                "  [alpha-debug]   ctor `{}.{}` -> `{}`",
                                name, old_cname, cn,
                            );
                        }
                        let cargs = c.args.into_iter().map(|t| ren_ty(t, &type_map)).collect();
                        Ctor {
                            name: cn,
                            args: cargs,
                            span: c.span,
                        }
                    })
                    .collect();
                decls.push(Decl::Type {
                    name: new_name,
                    params,
                    ctors: new_ctors,
                    span,
                });
            }
            Decl::Bring(paths) => decls.push(Decl::Bring(paths)),
        }
    }

    let body = go(*p.body, &mut Vec::new(), &top, &ctor_ren, &mut values);
    Program {
        decls,
        body: Box::new(body),
    }
}

fn collect_pure(p: &Program) -> HashSet<String> {
    let mut pure = HashSet::new();

    for d in &p.decls {
        match d {
            Decl::External(e) => {
                if e.pure {
                    pure.insert(e.name.clone());
                }
            }
            Decl::Type { ctors, .. } => {
                for c in ctors {
                    pure.insert(c.name.clone());
                }
            }
            Decl::Let { .. } => {}
            Decl::Bring(_) => {}
        }
    }

    loop {
        let mut changed = false;
        for d in &p.decls {
            if let Decl::Let { name, value, .. } = d {
                if matches!(&value.kind, ExprKind::Fn { .. })
                    && !pure.contains(name)
                    && matches!(&value.kind, ExprKind::Fn { body, .. } if pure_body(body, &pure))
                {
                    pure.insert(name.clone());
                    changed = true;
                }
            }
        }
        if !changed {
            break;
        }
    }
    pure
}

fn pure_body(e: &Expr, pure: &HashSet<String>) -> bool {
    match &e.kind {
        ExprKind::Int(_) | ExprKind::Float(_) | ExprKind::Str(_) | ExprKind::Var(_) => true,
        ExprKind::Fn { .. } | ExprKind::Quote(_) => true,
        ExprKind::Let { value, body, .. } => pure_body(value, pure) && pure_body(body, pure),
        ExprKind::Bin { l, r, .. } => pure_body(l, pure) && pure_body(r, pure),
        ExprKind::Un { e, .. } => pure_body(e, pure),
        ExprKind::App { fun, args } => {
            let (base, flat_args) = flatten_application_for_opt(fun, args);
            if matches!(&base.kind, ExprKind::Var(n) if n == "$mak.index") {
                // Keep memory loads out of algebraic rewrites that require
                // referentially transparent operands.
                let _ = flat_args;
                return false;
            }
            let f = match &fun.kind {
                ExprKind::Var(n) => pure.contains(n),
                ExprKind::Fn { .. } => true,
                _ => pure_body(fun, pure),
            };
            f && args.iter().all(|a| pure_body(a, pure))
        }
        ExprKind::Case { scrut, arms } => {
            pure_body(scrut, pure) && arms.iter().all(|a| pure_body(&a.body, pure))
        }
        ExprKind::Block { stmts, tail } => {
            stmts.iter().all(|s| pure_body(s, pure)) && pure_body(tail, pure)
        }
    }
}

fn is_pure(e: &Expr, pure: &HashSet<String>) -> bool {
    pure_body(e, pure)
}

fn map_prog(p: Program, f: &mut dyn FnMut(Expr) -> Expr) -> Program {
    let decls = p
        .decls
        .into_iter()
        .map(|d| match d {
            Decl::Let { name, value, span } => Decl::Let {
                name,
                value: Box::new(f(*value)),
                span,
            },
            other => other,
        })
        .collect();
    Program {
        decls,
        body: Box::new(f(*p.body)),
    }
}

fn map_child(e: Expr, f: &mut dyn FnMut(Expr) -> Expr) -> Expr {
    let sp = e.span;
    match e.kind {
        ExprKind::Let { name, value, body } => Expr {
            span: sp,
            kind: ExprKind::Let {
                name,
                value: Box::new(f(*value)),
                body: Box::new(f(*body)),
            },
        },
        ExprKind::Fn { params, body } => Expr {
            span: sp,
            kind: ExprKind::Fn {
                params,
                body: Box::new(f(*body)),
            },
        },
        ExprKind::App { fun, args } => Expr {
            span: sp,
            kind: ExprKind::App {
                fun: Box::new(f(*fun)),
                args: args.into_iter().map(|a| f(a)).collect(),
            },
        },
        ExprKind::Case { scrut, arms } => Expr {
            span: sp,
            kind: ExprKind::Case {
                scrut: Box::new(f(*scrut)),
                arms: arms
                    .into_iter()
                    .map(|a| Arm {
                        pat: a.pat,
                        body: f(a.body),
                        span: a.span,
                    })
                    .collect(),
            },
        },
        ExprKind::Block { stmts, tail } => Expr {
            span: sp,
            kind: ExprKind::Block {
                stmts: stmts.into_iter().map(|s| f(s)).collect(),
                tail: Box::new(f(*tail)),
            },
        },
        ExprKind::Bin { op, l, r } => Expr {
            span: sp,
            kind: ExprKind::Bin {
                op,
                l: Box::new(f(*l)),
                r: Box::new(f(*r)),
            },
        },
        ExprKind::Un { op, e } => Expr {
            span: sp,
            kind: ExprKind::Un {
                op,
                e: Box::new(f(*e)),
            },
        },
        ExprKind::Quote(x) => Expr {
            span: sp,
            kind: ExprKind::Quote(Box::new(f(*x))),
        },
        k => Expr { span: sp, kind: k },
    }
}

fn strip_closure_surface(p: Program, _pure: &HashSet<String>, changed: &mut bool) -> Program {
    // Collect the set of top-level names that are safe to eta-reduce to:
    // those bound by `let f = fn (...) {...}`.  cgen lowers a reference to
    // such a name to `LirExpr::FnRef(id)`, which is a representable value.
    //
    // Eta-reducing to an *external* function (`let x = fn (c) { putchar c }`
    // -> `let x = putchar`) produces a bare `Var("putchar")`, and cgen has no
    // way to represent an external function as a value; it would panic with
    // "external function used as value".  The same is true for n-arity
    // constructors used as values.  So restrict eta reduction to the safe
    // subset: names introduced by `let f = fn (...) {...}` at the top level.
    let mut eta_safe: HashSet<String> = HashSet::new();
    for d in &p.decls {
        if let Decl::Let { name, value, .. } = d {
            if matches!(&value.kind, ExprKind::Fn { .. }) {
                eta_safe.insert(name.clone());
            }
        }
    }

    fn go(e: Expr, eta_safe: &HashSet<String>, changed: &mut bool) -> Expr {
        let e = map_child(e, &mut |x| go(x, eta_safe, changed));
        let sp = e.span;

        if let ExprKind::Fn { params, body } = &e.kind {
            if distinct(&params) {
                if let ExprKind::App { fun, args } = &body.kind {
                    if args.len() == params.len() {
                        if let ExprKind::Var(target) = &fun.kind {
                            if eta_safe.contains(target)
                                && !params.iter().any(|p| p == target)
                                && args.iter().enumerate().all(
                                    |(i, a)| matches!(&a.kind, ExprKind::Var(v) if v == &params[i]),
                                )
                            {
                                *changed = true;
                                return Expr {
                                    span: sp,
                                    kind: ExprKind::Var(target.clone()),
                                };
                            }
                        }
                    }
                }
            }
        }

        e
    }

    map_prog(p, &mut |e| go(e, &eta_safe, changed))
}

fn flatten_cases(p: Program, pure: &HashSet<String>, changed: &mut bool) -> Program {
    fn substitute_into_arms(inner: &[Arm], outer: &[Arm], sp: Span) -> Option<Vec<Arm>> {
        let mut out = Vec::with_capacity(inner.len() * outer.len());
        for ia in inner {
            for oa in outer {
                let binds = match_bindings(&ia.pat, &oa.pat)?;
                let body = apply_subs(oa.body.clone(), &binds);
                out.push(Arm {
                    pat: ia.pat.clone(),
                    body,
                    span: sp,
                });
            }
        }
        Some(out)
    }

    fn match_bindings(inner: &Pat, outer: &Pat) -> Option<Vec<(String, Expr)>> {
        match (inner, outer) {
            (_, Pat::Wild) => Some(Vec::new()),
            (i, Pat::Bind(n)) => {
                let s = NULL_SPAN;
                Some(vec![(n.clone(), pat_to_expr(i, s))])
            }
            (Pat::Int(a), Pat::Int(b)) if a == b => Some(Vec::new()),
            (Pat::Str(a), Pat::Str(b)) if a == b => Some(Vec::new()),
            (Pat::Tuple(a), Pat::Tuple(b)) if a.len() == b.len() => {
                let mut acc = Vec::new();
                for (x, y) in a.iter().zip(b) {
                    acc.extend(match_bindings(x, y)?);
                }
                Some(acc)
            }
            (Pat::Ctor { name: an, args: aa }, Pat::Ctor { name: bn, args: ba })
                if an == bn && aa.len() == ba.len() =>
            {
                let mut acc = Vec::new();
                for (x, y) in aa.iter().zip(ba) {
                    acc.extend(match_bindings(x, y)?);
                }
                Some(acc)
            }
            _ => None,
        }
    }

    fn pat_to_expr(p: &Pat, sp: Span) -> Expr {
        match p {
            Pat::Wild => Expr {
                span: sp,
                kind: ExprKind::Int(0),
            },
            Pat::Bind(n) => Expr {
                span: sp,
                kind: ExprKind::Var(n.clone()),
            },
            Pat::Int(v) => Expr {
                span: sp,
                kind: ExprKind::Int(*v),
            },
            Pat::Str(v) => Expr {
                span: sp,
                kind: ExprKind::Str(v.clone()),
            },
            Pat::Ctor { name, args } => Expr {
                span: sp,
                kind: ExprKind::App {
                    fun: Box::new(Expr {
                        span: sp,
                        kind: ExprKind::Var(name.clone()),
                    }),
                    args: args.iter().map(|a| pat_to_expr(a, sp)).collect(),
                },
            },
            Pat::Tuple(xs) => Expr {
                span: sp,
                kind: ExprKind::App {
                    fun: Box::new(Expr {
                        span: sp,
                        kind: ExprKind::Var("_tuple".into()),
                    }),
                    args: xs.iter().map(|a| pat_to_expr(a, sp)).collect(),
                },
            },
        }
    }

    fn go(e: Expr, pure: &HashSet<String>, changed: &mut bool) -> Expr {
        let e = map_child(e, &mut |x| go(x, pure, changed));
        if let ExprKind::Case { scrut, arms } = &e.kind {
            if let ExprKind::Case {
                scrut: inner_scrut,
                arms: inner_arms,
            } = &scrut.kind
            {
                if inner_arms.len() <= 6 && arms.len() <= 6 {
                    if let Some(merged) = substitute_into_arms(inner_arms, arms, e.span) {
                        *changed = true;
                        return Expr {
                            span: e.span,
                            kind: ExprKind::Case {
                                scrut: inner_scrut.clone(),
                                arms: merged,
                            },
                        };
                    }
                }
            }
        }
        e
    }
    let _ = pure;
    map_prog(p, &mut |e| go(e, pure, changed))
}

fn beta(p: Program, pure: &HashSet<String>, changed: &mut bool) -> Program {
    fn go(e: Expr, pure: &HashSet<String>, changed: &mut bool) -> Expr {
        let e = map_child(e, &mut |x| go(x, pure, changed));
        if let ExprKind::App { fun, args } = e.kind {
            if let ExprKind::Fn { params, body } = &fun.kind {
                if params.len() == args.len() && distinct(params) {
                    let mut b = (**body).clone();
                    let mut ok = true;
                    for (param, arg) in params.iter().zip(args.iter()) {
                        let uses = count_var(&b, param);
                        if uses == 0 {
                            if !is_pure(arg, pure) {
                                ok = false;
                                break;
                            }
                        } else if uses == 1 || is_pure(arg, pure) {
                            b = subst(b, param, arg);
                        } else {
                            ok = false;
                            break;
                        }
                        if count_var(&b, param) != 0 {
                            ok = false;
                            break;
                        }
                    }
                    if ok {
                        *changed = true;
                        return b;
                    }
                }
            }
            return Expr {
                span: e.span,
                kind: ExprKind::App { fun, args },
            };
        }
        e
    }
    map_prog(p, &mut |e| go(e, pure, changed))
}

fn top_fns(p: &Program) -> HashMap<String, (Vec<String>, Expr)> {
    let mut out = HashMap::new();
    for d in &p.decls {
        if let Decl::Let { name, value, .. } = d {
            if let ExprKind::Fn { params, body } = &value.kind {
                out.insert(name.clone(), (params.clone(), (**body).clone()));
            }
        }
    }
    out
}

fn calls(e: &Expr, names: &HashSet<String>, out: &mut HashSet<String>) {
    match &e.kind {
        ExprKind::App { fun, args } => {
            if let ExprKind::Var(n) = &fun.kind {
                if names.contains(n) {
                    out.insert(n.clone());
                }
            } else {
                calls(fun, names, out);
            }
            for a in args {
                calls(a, names, out);
            }
        }
        ExprKind::Let { value, body, .. } => {
            calls(value, names, out);
            calls(body, names, out);
        }
        ExprKind::Fn { body, .. } => calls(body, names, out),
        ExprKind::Case { scrut, arms } => {
            calls(scrut, names, out);
            for a in arms {
                calls(&a.body, names, out);
            }
        }
        ExprKind::Block { stmts, tail } => {
            for s in stmts {
                calls(s, names, out);
            }
            calls(tail, names, out);
        }
        ExprKind::Bin { l, r, .. } => {
            calls(l, names, out);
            calls(r, names, out);
        }
        ExprKind::Un { e, .. } | ExprKind::Quote(e) => calls(e, names, out),
        ExprKind::Int(_) | ExprKind::Float(_) | ExprKind::Str(_) | ExprKind::Var(_) => {}
    }
}

fn recursive_fns(p: &Program) -> HashSet<String> {
    let fns = top_fns(p);
    let names: HashSet<String> = fns.keys().cloned().collect();
    let mut graph = HashMap::<String, HashSet<String>>::new();
    for (name, (_, body)) in &fns {
        let mut edges = HashSet::new();
        calls(body, &names, &mut edges);
        graph.insert(name.clone(), edges);
    }

    fn reaches(
        cur: &str,
        target: &str,
        graph: &HashMap<String, HashSet<String>>,
        seen: &mut HashSet<String>,
    ) -> bool {
        if !seen.insert(cur.to_string()) {
            return false;
        }
        if let Some(next) = graph.get(cur) {
            for n in next {
                if n == target || reaches(n, target, graph, seen) {
                    return true;
                }
            }
        }
        false
    }

    let mut recursive = HashSet::new();
    for name in names {
        let mut seen = HashSet::new();
        if reaches(&name, &name, &graph, &mut seen) {
            recursive.insert(name);
        }
    }
    recursive
}

fn inline(p: Program, pure: &HashSet<String>, changed: &mut bool) -> Program {
    let funcs = top_fns(&p);
    if funcs.is_empty() {
        return p;
    }
    let recursive = recursive_fns(&p);
    let mut counter = 0usize;

    fn inline_one(
        params: &[String],
        body: &Expr,
        args: &[Expr],
        pure: &HashSet<String>,
        counter: &mut usize,
    ) -> Option<Expr> {
        if params.len() != args.len() || !distinct(params) {
            return None;
        }
        let mut b = body.clone();
        let mut lets = Vec::<(String, Expr)>::new();
        for (param, arg) in params.iter().zip(args) {
            let uses = count_var(&b, param);
            if uses == 0 {
                if is_pure(arg, pure) {
                    continue;
                }
                let fresh = format!("__mak_inline_{}", *counter);
                *counter += 1;
                b = subst(
                    b,
                    param,
                    &Expr {
                        span: arg.span,
                        kind: ExprKind::Var(fresh.clone()),
                    },
                );
                lets.push((fresh, arg.clone()));
            } else if is_pure(arg, pure) {
                b = subst(b, param, arg);
            } else {
                let fresh = format!("__mak_inline_{}", *counter);
                *counter += 1;
                b = subst(
                    b,
                    param,
                    &Expr {
                        span: arg.span,
                        kind: ExprKind::Var(fresh.clone()),
                    },
                );
                lets.push((fresh, arg.clone()));
            }
            if count_var(&b, param) != 0 {
                return None;
            }
        }
        for (name, value) in lets.into_iter().rev() {
            b = Expr {
                span: b.span,
                kind: ExprKind::Let {
                    name,
                    value: Box::new(value),
                    body: Box::new(b),
                },
            };
        }
        Some(b)
    }

    fn go(
        e: Expr,
        funcs: &HashMap<String, (Vec<String>, Expr)>,
        recursive: &HashSet<String>,
        pure: &HashSet<String>,
        changed: &mut bool,
        counter: &mut usize,
    ) -> Expr {
        let e = map_child(e, &mut |x| go(x, funcs, recursive, pure, changed, counter));
        if let ExprKind::App { fun, args } = &e.kind {
            if let ExprKind::Var(name) = &fun.kind {
                if let Some((params, body)) = funcs.get(name) {
                    let sz = size(body);
                    if !recursive.contains(name) && sz <= 768 && args.len() == params.len() {
                        if let Some(out) = inline_one(params, body, args, pure, counter) {
                            *changed = true;
                            return out;
                        }
                    }
                }
            }
        }
        e
    }

    let decls = p
        .decls
        .into_iter()
        .map(|d| match d {
            Decl::Let { name, value, span } => Decl::Let {
                name,
                value: Box::new(go(*value, &funcs, &recursive, pure, changed, &mut counter)),
                span,
            },
            other => other,
        })
        .collect();
    let body = go(*p.body, &funcs, &recursive, pure, changed, &mut counter);
    Program {
        decls,
        body: Box::new(body),
    }
}

fn known_binds(pat: &Pat, value: &Expr, out: &mut Vec<(String, Expr)>) -> bool {
    match (pat, &value.kind) {
        (Pat::Wild, _) => true,
        (Pat::Bind(name), _) => {
            out.push((name.clone(), value.clone()));
            true
        }
        (Pat::Int(a), ExprKind::Int(b)) => a == b,
        (Pat::Str(a), ExprKind::Str(b)) => a == b,
        (Pat::Ctor { name: pn, args: pa }, ExprKind::App { fun, args: va }) => {
            let ExprKind::Var(vn) = &fun.kind else {
                return false;
            };
            if pn != vn || pa.len() != va.len() {
                return false;
            }
            for (p, v) in pa.iter().zip(va) {
                if !known_binds(p, v, out) {
                    return false;
                }
            }
            true
        }
        _ => false,
    }
}

fn known_cases(p: Program, pure: &HashSet<String>, changed: &mut bool) -> Program {
    let ctor_names: HashSet<String> = p
        .decls
        .iter()
        .filter_map(|d| match d {
            Decl::Type { ctors, .. } => Some(ctors.iter().map(|c| c.name.clone())),
            _ => None,
        })
        .flatten()
        .collect();

    fn known_value(e: &Expr, ctor_names: &HashSet<String>) -> bool {
        match &e.kind {
            ExprKind::Int(_) | ExprKind::Float(_) | ExprKind::Str(_) | ExprKind::Fn { .. } => true,
            ExprKind::App { fun, args } => {
                matches!(&fun.kind, ExprKind::Var(n) if ctor_names.contains(n))
                    && args.iter().all(|a| known_value(a, ctor_names))
            }
            ExprKind::Block { .. }
            | ExprKind::Let { .. }
            | ExprKind::Case { .. }
            | ExprKind::Var(_)
            | ExprKind::Bin { .. }
            | ExprKind::Un { .. }
            | ExprKind::Quote(_) => false,
        }
    }

    fn go(
        e: Expr,
        pure: &HashSet<String>,
        ctor_names: &HashSet<String>,
        changed: &mut bool,
    ) -> Expr {
        let e = map_child(e, &mut |x| go(x, pure, ctor_names, changed));
        let ExprKind::Case { scrut, arms } = &e.kind else {
            return e;
        };

        if !known_value(scrut, ctor_names) {
            return e;
        }

        for arm in arms {
            let mut binds = Vec::new();
            if known_binds(&arm.pat, scrut, &mut binds) {
                let body = apply_subs(arm.body.clone(), &binds);
                *changed = true;
                return body;
            }
        }
        e
    }
    map_prog(p, &mut |e| go(e, pure, &ctor_names, changed))
}

fn tree_opt(p: Program, pure: &HashSet<String>, changed: &mut bool) -> Program {
    fn go(e: Expr, pure: &HashSet<String>, changed: &mut bool) -> Expr {
        let e = map_child(e, &mut |x| go(x, pure, changed));
        simp_expr(e, pure, changed)
    }
    map_prog(p, &mut |e| go(e, pure, changed))
}

fn simp_expr(e: Expr, pure: &HashSet<String>, changed: &mut bool) -> Expr {
    let sp = e.span;
    match e.kind {
        ExprKind::Bin { op, l, r } => simp_bin(
            Expr {
                span: sp,
                kind: ExprKind::Bin { op, l, r },
            },
            pure,
            changed,
        ),
        ExprKind::Un { op, e } => simp_un(
            Expr {
                span: sp,
                kind: ExprKind::Un { op, e },
            },
            pure,
            changed,
        ),
        ExprKind::Let { name, value, body } => simp_let(
            Expr {
                span: sp,
                kind: ExprKind::Let { name, value, body },
            },
            pure,
            changed,
        ),
        ExprKind::App { fun, args } => simp_app(
            Expr {
                span: sp,
                kind: ExprKind::App { fun, args },
            },
            pure,
            changed,
        ),
        ExprKind::Case { scrut, arms } => simp_case(
            Expr {
                span: sp,
                kind: ExprKind::Case { scrut, arms },
            },
            pure,
            changed,
        ),
        ExprKind::Block { stmts, tail } => simp_block(
            Expr {
                span: sp,
                kind: ExprKind::Block { stmts, tail },
            },
            pure,
            changed,
        ),
        k => Expr { span: sp, kind: k },
    }
}

fn simp_un(e: Expr, pure: &HashSet<String>, changed: &mut bool) -> Expr {
    let sp = e.span;
    let ExprKind::Un { op, e: inner } = e.kind else {
        return e;
    };
    let inner = *inner;

    if let ExprKind::Int(n) = &inner.kind {
        *changed = true;
        return int(eval_un(op, *n).unwrap_or(*n));
    }

    match op {
        UnOp::BitNot => {
            if let ExprKind::Un {
                op: UnOp::BitNot,
                e: x,
            } = &inner.kind
            {
                *changed = true;
                return (**x).clone();
            }
            if let ExprKind::Bin {
                op: BinOp::Xor,
                l,
                r,
            } = &inner.kind
            {
                *changed = true;
                return bin(
                    BinOp::Xor,
                    Expr {
                        span: sp,
                        kind: ExprKind::Un {
                            op: UnOp::BitNot,
                            e: l.clone(),
                        },
                    },
                    (**r).clone(),
                    sp,
                );
            }
            if let ExprKind::Bin { op: bop, l, r } = &inner.kind {
                if matches!(bop, BinOp::And | BinOp::Or) {
                    *changed = true;
                    let out = if *bop == BinOp::And {
                        BinOp::Or
                    } else {
                        BinOp::And
                    };
                    return bin(
                        out,
                        Expr {
                            span: sp,
                            kind: ExprKind::Un {
                                op: UnOp::BitNot,
                                e: l.clone(),
                            },
                        },
                        Expr {
                            span: sp,
                            kind: ExprKind::Un {
                                op: UnOp::BitNot,
                                e: r.clone(),
                            },
                        },
                        sp,
                    );
                }
            }
        }
        UnOp::Not => {
            if let ExprKind::Un {
                op: UnOp::Not,
                e: x,
            } = &inner.kind
            {
                *changed = true;
                return bin(BinOp::Ne, (**x).clone(), int(0), sp);
            }
            if let ExprKind::Un {
                op: UnOp::BitNot,
                e: x,
            } = &inner.kind
            {
                *changed = true;
                return bin(BinOp::Eq, (**x).clone(), int(u64::MAX), sp);
            }
            if let ExprKind::Bin { op: bop, l, r } = &inner.kind {
                let out = match bop {
                    BinOp::Eq => Some(BinOp::Ne),
                    BinOp::Ne => Some(BinOp::Eq),
                    _ => None,
                };
                if let Some(out) = out {
                    *changed = true;
                    return bin(out, (**l).clone(), (**r).clone(), sp);
                }
            }
            if let ExprKind::Bin { op: bop, l, r } = &inner.kind {
                if matches!(bop, BinOp::LAnd | BinOp::LOr) && is_pure(l, pure) && is_pure(r, pure) {
                    *changed = true;
                    let out = if *bop == BinOp::LAnd {
                        BinOp::LOr
                    } else {
                        BinOp::LAnd
                    };
                    return bin(
                        out,
                        Expr {
                            span: sp,
                            kind: ExprKind::Un {
                                op: UnOp::Not,
                                e: l.clone(),
                            },
                        },
                        Expr {
                            span: sp,
                            kind: ExprKind::Un {
                                op: UnOp::Not,
                                e: r.clone(),
                            },
                        },
                        sp,
                    );
                }
            }
        }
        UnOp::Neg => {
            if let ExprKind::Un {
                op: UnOp::Neg,
                e: x,
            } = &inner.kind
            {
                *changed = true;
                return (**x).clone();
            }
            if let ExprKind::Bin {
                op: BinOp::Sub,
                l,
                r,
            } = &inner.kind
            {
                if as_int(r).is_some() {
                    *changed = true;
                    return bin(BinOp::Sub, (**r).clone(), (**l).clone(), sp);
                }
            }
            if let ExprKind::Bin {
                op: BinOp::Mul,
                l,
                r,
            } = &inner.kind
            {
                if as_int(r).is_some() {
                    *changed = true;
                    return bin(
                        BinOp::Mul,
                        Expr {
                            span: sp,
                            kind: ExprKind::Un {
                                op: UnOp::Neg,
                                e: l.clone(),
                            },
                        },
                        (**r).clone(),
                        sp,
                    );
                }
            }
        }
    }

    Expr {
        span: sp,
        kind: ExprKind::Un {
            op,
            e: Box::new(inner),
        },
    }
}

fn simp_bin(e: Expr, pure: &HashSet<String>, changed: &mut bool) -> Expr {
    let sp = e.span;
    let ExprKind::Bin { op, l, r } = e.kind else {
        return e;
    };
    let l = *l;
    let r = *r;
    let li = as_int(&l);
    let ri = as_int(&r);
    use BinOp::*;

    if let (Some(a), Some(b)) = (li, ri) {
        if let Some(v) = eval_bin(op, a, b) {
            *changed = true;
            return int(v);
        }
    }

    match op {
        Add => {
            if ri == Some(0) {
                *changed = true;
                return l;
            }
            if li == Some(0) {
                *changed = true;
                return r;
            }
            if let ExprKind::Bin {
                op: Add,
                l: x,
                r: a,
            } = &l.kind
            {
                if let (Some(a), Some(b)) = (as_int(a), ri) {
                    *changed = true;
                    return bin(Add, (**x).clone(), int(a.wrapping_add(b)), sp);
                }
            }
            if let ExprKind::Bin {
                op: Sub,
                l: x,
                r: a,
            } = &l.kind
            {
                if let (Some(a), Some(b)) = (as_int(a), ri) {
                    *changed = true;
                    return bin(Add, (**x).clone(), int(b.wrapping_sub(a)), sp);
                }
            }
            if let ExprKind::Bin {
                op: aop,
                l: y,
                r: c,
            } = &r.kind
            {
                if matches!(aop, Add | Sub) {
                    if let Some(c) = as_int(c) {
                        *changed = true;
                        return bin(
                            if *aop == Add { Add } else { Sub },
                            bin(Add, l, (**y).clone(), sp),
                            int(c),
                            sp,
                        );
                    }
                }
            }
            if let ExprKind::Bin {
                op: Add,
                l: x,
                r: c,
            } = &l.kind
            {
                if as_int(c).is_some() && is_pure(&r, pure) {
                    *changed = true;
                    return bin(Add, bin(Add, (**x).clone(), r, sp), (**c).clone(), sp);
                }
            }
            if let ExprKind::Bin {
                op: Sub,
                l: x,
                r: c,
            } = &l.kind
            {
                if as_int(c).is_some() && is_pure(&r, pure) {
                    *changed = true;
                    return bin(Sub, bin(Add, (**x).clone(), r, sp), (**c).clone(), sp);
                }
            }
            if let ExprKind::Bin {
                op: Sub,
                l: x,
                r: a,
            } = &l.kind
            {
                if as_int(a).is_some() && same(a, &r) {
                    *changed = true;
                    return (**x).clone();
                }
            }
            if li.is_some() && ri.is_none() && is_pure(&l, pure) && is_pure(&r, pure) {
                *changed = true;
                return bin(Add, r, l, sp);
            }
            if let (
                ExprKind::Bin {
                    op: Add,
                    l: x,
                    r: a,
                },
                ExprKind::Bin {
                    op: Add,
                    l: y,
                    r: b,
                },
            ) = (&l.kind, &r.kind)
            {
                if is_pure(x, pure)
                    && is_pure(y, pure)
                    && as_int(a).is_some()
                    && as_int(b).is_some()
                {
                    *changed = true;
                    return bin(
                        Add,
                        bin(Add, (**x).clone(), (**y).clone(), sp),
                        int(as_int(a).unwrap().wrapping_add(as_int(b).unwrap())),
                        sp,
                    );
                }
            }
            if same(&l, &r) && is_pure(&l, pure) && is_int_expr(&l) {
                *changed = true;
                return bin(Shl, l, int(1), sp);
            }
        }
        Sub => {
            if ri == Some(0) {
                *changed = true;
                return l;
            }
            if let ExprKind::Bin {
                op: Add,
                l: x,
                r: a,
            } = &l.kind
            {
                if let (Some(a), Some(b)) = (as_int(a), ri) {
                    *changed = true;
                    return bin(Add, (**x).clone(), int(a.wrapping_sub(b)), sp);
                }
            }
            if let ExprKind::Bin {
                op: Sub,
                l: x,
                r: a,
            } = &l.kind
            {
                if let (Some(a), Some(b)) = (as_int(a), ri) {
                    *changed = true;
                    return bin(Sub, (**x).clone(), int(a.wrapping_add(b)), sp);
                }
            }
            if let ExprKind::Bin {
                op: Add,
                l: y,
                r: c,
            } = &r.kind
            {
                if as_int(c).is_some() {
                    *changed = true;
                    return bin(Sub, bin(Sub, l, (**y).clone(), sp), (**c).clone(), sp);
                }
            }
            if let ExprKind::Bin {
                op: Sub,
                l: y,
                r: c,
            } = &r.kind
            {
                if as_int(c).is_some() {
                    *changed = true;
                    return bin(Add, bin(Sub, l, (**y).clone(), sp), (**c).clone(), sp);
                }
            }
            if let ExprKind::Bin {
                op: Add,
                l: x,
                r: a,
            } = &l.kind
            {
                if let ExprKind::Bin {
                    op: Sub,
                    l: y,
                    r: b,
                } = &x.kind
                {
                    if same(a, b) && is_pure(a, pure) {
                        *changed = true;
                        return (**y).clone();
                    }
                }
            }
            if let ExprKind::Bin {
                op: Add,
                l: x,
                r: a,
            } = &l.kind
            {
                if same(a, &r) && is_pure(a, pure) {
                    *changed = true;
                    return (**x).clone();
                }
            }
            if is_pure(&l, pure) {
                if let ExprKind::Bin {
                    op: Sub,
                    l: x,
                    r: y,
                } = &r.kind
                {
                    if same(&l, x) && is_pure(y, pure) {
                        *changed = true;
                        return (**y).clone();
                    }
                }
            }
            if let ExprKind::Bin {
                op: Add,
                l: x,
                r: y,
            } = &l.kind
            {
                if same(y, &r) && is_pure(y, pure) {
                    *changed = true;
                    return (**x).clone();
                }
            }
            if let ExprKind::Bin {
                op: Add,
                l: x,
                r: y,
            } = &l.kind
            {
                if same(x, &r) && is_pure(x, pure) && is_pure(y, pure) {
                    *changed = true;
                    return (**y).clone();
                }
            }
            if let ExprKind::Bin {
                op: Add,
                l: x,
                r: y,
            } = &r.kind
            {
                if same(&l, x) && is_pure(x, pure) && is_pure(y, pure) && is_int_expr(x) {
                    *changed = true;
                    return Expr {
                        span: sp,
                        kind: ExprKind::Un {
                            op: UnOp::Neg,
                            e: y.clone(),
                        },
                    };
                }
            }
            if let ExprKind::Bin {
                op: Sub,
                l: x,
                r: y,
            } = &r.kind
            {
                if same(&l, x) && is_pure(&l, pure) && is_pure(y, pure) {
                    *changed = true;
                    return (**y).clone();
                }
            }
            if let ExprKind::Bin {
                op: Add,
                l: y,
                r: z,
            } = &r.kind
            {
                if as_int(z) == Some(0) {
                    *changed = true;
                    return bin(Sub, l, (**y).clone(), sp);
                }
            }
            if let ExprKind::Bin {
                op: Sub,
                l: y,
                r: z,
            } = &r.kind
            {
                if as_int(z) == Some(0) {
                    *changed = true;
                    return bin(Sub, l, (**y).clone(), sp);
                }
            }
            if let ExprKind::Bin {
                op: Add,
                l: x,
                r: z,
            } = &l.kind
            {
                if as_int(z) == Some(0) {
                    *changed = true;
                    return bin(Sub, (**x).clone(), r, sp);
                }
            }
            if let ExprKind::Bin {
                op: Sub,
                l: x,
                r: z,
            } = &l.kind
            {
                if as_int(z) == Some(0) {
                    *changed = true;
                    return bin(Sub, (**x).clone(), r, sp);
                }
            }
            if let ExprKind::Bin {
                op: Sub,
                l: x,
                r: z,
            } = &r.kind
            {
                if as_int(z) == Some(0) && same(&l, x) && is_pure(&l, pure) && is_int_expr(&l) {
                    *changed = true;
                    return int(0);
                }
            }
        }
        Mul => {
            if let Some(c) = ri {
                if c.is_power_of_two() && c != 0 {
                    let sh = c.trailing_zeros() as u64;
                    *changed = true;
                    return bin(Shl, l, int(sh), sp);
                }
            }
            if let Some(c) = li {
                if c.is_power_of_two() && c != 0 {
                    let sh = c.trailing_zeros() as u64;
                    *changed = true;
                    return bin(Shl, r, int(sh), sp);
                }
            }
            if ri == Some(1) {
                *changed = true;
                return l;
            }
            if li == Some(1) {
                *changed = true;
                return r;
            }
            if ri == Some(0) && is_pure(&l, pure) {
                *changed = true;
                return int(0);
            }
            if li == Some(0) && is_pure(&r, pure) {
                *changed = true;
                return int(0);
            }
            if let ExprKind::Bin {
                op: Mul,
                l: x,
                r: a,
            } = &l.kind
            {
                if let (Some(a), Some(b)) = (as_int(a), ri) {
                    *changed = true;
                    return bin(Mul, (**x).clone(), int(a.wrapping_mul(b)), sp);
                }
            }
            if let ExprKind::Bin {
                op: Mul,
                l: a,
                r: x,
            } = &r.kind
            {
                if let (Some(a), Some(b)) = (li, as_int(a)) {
                    *changed = true;
                    return bin(Mul, int(a.wrapping_mul(b)), (**x).clone(), sp);
                }
            }
            if let ExprKind::Bin {
                op: Mul,
                l: y,
                r: c,
            } = &r.kind
            {
                if as_int(c).is_some() {
                    *changed = true;
                    return bin(Mul, bin(Mul, l, (**y).clone(), sp), (**c).clone(), sp);
                }
            }
            if let ExprKind::Bin {
                op: Mul,
                l: x,
                r: c,
            } = &l.kind
            {
                if as_int(c).is_some() {
                    *changed = true;
                    return bin(Mul, bin(Mul, (**x).clone(), r, sp), (**c).clone(), sp);
                }
            }
            if let ExprKind::Bin {
                op: Add,
                l: x,
                r: a,
            } = &l.kind
            {
                if let (Some(a), Some(c)) = (as_int(a), ri) {
                    *changed = true;
                    return bin(
                        Add,
                        bin(Mul, (**x).clone(), int(c), sp),
                        int(a.wrapping_mul(c)),
                        sp,
                    );
                }
            }
            if let ExprKind::Bin {
                op: Sub,
                l: x,
                r: a,
            } = &l.kind
            {
                if let (Some(a), Some(c)) = (as_int(a), ri) {
                    *changed = true;
                    return bin(
                        Sub,
                        bin(Mul, (**x).clone(), int(c), sp),
                        int(a.wrapping_mul(c)),
                        sp,
                    );
                }
            }
            if let ExprKind::Bin {
                op: Add,
                l: x,
                r: a,
            } = &r.kind
            {
                if let (Some(c), Some(a)) = (li, as_int(a)) {
                    *changed = true;
                    return bin(
                        Add,
                        bin(Mul, int(c), (**x).clone(), sp),
                        int(c.wrapping_mul(a)),
                        sp,
                    );
                }
            }
            if let ExprKind::Bin {
                op: Sub,
                l: x,
                r: a,
            } = &r.kind
            {
                if let (Some(c), Some(a)) = (li, as_int(a)) {
                    *changed = true;
                    return bin(
                        Sub,
                        bin(Mul, int(c), (**x).clone(), sp),
                        int(c.wrapping_mul(a)),
                        sp,
                    );
                }
            }
            if li.is_some() && ri.is_none() && is_pure(&l, pure) && is_pure(&r, pure) {
                *changed = true;
                return bin(Mul, r, l, sp);
            }
        }
        Div => {
            if let Some(c) = ri {
                if c.is_power_of_two() && c > 1 {
                    let sh = c.trailing_zeros() as u64;
                    *changed = true;
                    return bin(Shr, l, int(sh), sp);
                }
            }
            if ri == Some(1) {
                *changed = true;
                return l;
            }
            if li == Some(0) && ri.is_some() && ri != Some(0) && is_pure(&r, pure) {
                *changed = true;
                return int(0);
            }
        }
        Rem => {
            if let Some(c) = ri {
                if c.is_power_of_two() && c > 1 && is_pure(&l, pure) {
                    *changed = true;
                    return bin(And, l, int(c - 1), sp);
                }
            }
            if ri == Some(1) && is_pure(&l, pure) {
                *changed = true;
                return int(0);
            }
            if li == Some(0) && ri.is_some() && ri != Some(0) && is_pure(&r, pure) {
                *changed = true;
                return int(0);
            }
        }
        And => {
            if ri == Some(0) && is_pure(&l, pure) {
                *changed = true;
                return int(0);
            }
            if li == Some(0) && is_pure(&r, pure) {
                *changed = true;
                return int(0);
            }
            if ri == Some(u64::MAX) {
                *changed = true;
                return l;
            }
            if li == Some(u64::MAX) {
                *changed = true;
                return r;
            }
            if same(&l, &r) && is_pure(&l, pure) {
                *changed = true;
                return l;
            }
            if let ExprKind::Bin {
                op: And,
                l: x,
                r: a,
            } = &l.kind
            {
                if let (Some(a), Some(b)) = (as_int(a), ri) {
                    *changed = true;
                    return bin(And, (**x).clone(), int(a & b), sp);
                }
            }
            if let ExprKind::Bin { op: Or, l: x, r: y } = &r.kind {
                if same(&l, x) && is_pure(&l, pure) && is_pure(y, pure) {
                    *changed = true;
                    return l;
                }
            }
            if let ExprKind::Bin {
                op: And,
                l: x,
                r: _y,
            } = &r.kind
            {
                if same(&l, x) && is_pure(&l, pure) {
                    *changed = true;
                    return r;
                }
            }
            if let ExprKind::Bin { op: Or, l: x, r: y } = &l.kind {
                if same(x, &r) && is_pure(x, pure) && is_pure(y, pure) {
                    *changed = true;
                    return r;
                }
            }
            if let (
                ExprKind::Bin {
                    op: And,
                    l: x1,
                    r: a,
                },
                ExprKind::Bin {
                    op: And,
                    l: x2,
                    r: b,
                },
            ) = (&l.kind, &r.kind)
            {
                if same(x1, x2) && is_pure(x1, pure) {
                    if let (Some(a), Some(b)) = (as_int(a), as_int(b)) {
                        *changed = true;
                        return bin(And, (**x1).clone(), int(a & b), sp);
                    }
                }
            }
            if let (
                ExprKind::Bin {
                    op: Or,
                    l: x1,
                    r: a,
                },
                ExprKind::Bin {
                    op: Or,
                    l: x2,
                    r: b,
                },
            ) = (&l.kind, &r.kind)
            {
                if same(x1, x2) && is_pure(x1, pure) {
                    if let (Some(a), Some(b)) = (as_int(a), as_int(b)) {
                        *changed = true;
                        return bin(Or, (**x1).clone(), int(a & b), sp);
                    }
                }
            }
            if let ExprKind::Bin {
                op: And,
                l: _y,
                r: x,
            } = &r.kind
            {
                if same(&l, x) && is_pure(&l, pure) {
                    *changed = true;
                    return r;
                }
            }
            if let ExprKind::Bin {
                op: Xor,
                l: x,
                r: y,
            } = &r.kind
            {
                if same(&l, x) && is_pure(&l, pure) && is_pure(y, pure) {
                    *changed = true;
                    return bin(
                        And,
                        l,
                        Expr {
                            span: sp,
                            kind: ExprKind::Un {
                                op: UnOp::BitNot,
                                e: y.clone(),
                            },
                        },
                        sp,
                    );
                }
            }
            if let ExprKind::Bin {
                op: Xor,
                l: x,
                r: y,
            } = &l.kind
            {
                if same(x, &r) && is_pure(x, pure) && is_pure(y, pure) {
                    *changed = true;
                    return bin(
                        And,
                        r,
                        Expr {
                            span: sp,
                            kind: ExprKind::Un {
                                op: UnOp::BitNot,
                                e: y.clone(),
                            },
                        },
                        sp,
                    );
                }
            }
        }
        Or => {
            if ri == Some(0) {
                *changed = true;
                return l;
            }
            if li == Some(0) {
                *changed = true;
                return r;
            }
            if ri == Some(u64::MAX) && is_pure(&l, pure) {
                *changed = true;
                return int(u64::MAX);
            }
            if li == Some(u64::MAX) && is_pure(&r, pure) {
                *changed = true;
                return int(u64::MAX);
            }
            if same(&l, &r) && is_pure(&l, pure) {
                *changed = true;
                return l;
            }
            if let ExprKind::Bin { op: Or, l: x, r: a } = &l.kind {
                if let (Some(a), Some(b)) = (as_int(a), ri) {
                    *changed = true;
                    return bin(Or, (**x).clone(), int(a | b), sp);
                }
            }
            if let ExprKind::Bin {
                op: And,
                l: x,
                r: y,
            } = &r.kind
            {
                if same(&l, x) && is_pure(&l, pure) && is_pure(y, pure) {
                    *changed = true;
                    return l;
                }
            }
            if let ExprKind::Bin {
                op: Or,
                l: x,
                r: _y,
            } = &r.kind
            {
                if same(&l, x) && is_pure(&l, pure) {
                    *changed = true;
                    return r;
                }
            }
            if let (
                ExprKind::Bin {
                    op: And,
                    l: x1,
                    r: a,
                },
                ExprKind::Bin {
                    op: And,
                    l: x2,
                    r: b,
                },
            ) = (&l.kind, &r.kind)
            {
                if same(x1, x2) && is_pure(x1, pure) {
                    if let (Some(a), Some(b)) = (as_int(a), as_int(b)) {
                        *changed = true;
                        return bin(And, (**x1).clone(), int(a | b), sp);
                    }
                }
            }
            if let (
                ExprKind::Bin {
                    op: And,
                    l: x1,
                    r: a,
                },
                ExprKind::Bin {
                    op: Or,
                    l: x2,
                    r: _b,
                },
            ) = (&l.kind, &r.kind)
            {
                if same(x1, x2) && is_pure(x1, pure) && is_pure(a, pure) {
                    *changed = true;
                    return r.clone();
                }
            }
            if let (
                ExprKind::Bin {
                    op: Or,
                    l: x1,
                    r: a,
                },
                ExprKind::Bin {
                    op: Or,
                    l: x2,
                    r: b,
                },
            ) = (&l.kind, &r.kind)
            {
                if same(x1, x2) && is_pure(x1, pure) {
                    if let (Some(a), Some(b)) = (as_int(a), as_int(b)) {
                        *changed = true;
                        return bin(Or, (**x1).clone(), int(a | b), sp);
                    }
                }
            }
            if let ExprKind::Bin {
                op: Xor,
                l: x,
                r: y,
            } = &r.kind
            {
                if same(&l, x) && is_pure(&l, pure) && is_pure(y, pure) {
                    *changed = true;
                    return bin(Or, l, (**y).clone(), sp);
                }
            }
            if let ExprKind::Bin {
                op: Xor,
                l: x,
                r: y,
            } = &l.kind
            {
                if same(x, &r) && is_pure(x, pure) && is_pure(y, pure) {
                    *changed = true;
                    return bin(Or, r, (**y).clone(), sp);
                }
            }
        }
        Xor => {
            if ri == Some(0) {
                *changed = true;
                return l;
            }
            if li == Some(0) {
                *changed = true;
                return r;
            }
            if ri == Some(u64::MAX) {
                *changed = true;
                return Expr {
                    span: sp,
                    kind: ExprKind::Un {
                        op: UnOp::BitNot,
                        e: Box::new(l),
                    },
                };
            }
            if li == Some(u64::MAX) {
                *changed = true;
                return Expr {
                    span: sp,
                    kind: ExprKind::Un {
                        op: UnOp::BitNot,
                        e: Box::new(r),
                    },
                };
            }
            if same(&l, &r) && is_pure(&l, pure) {
                *changed = true;
                return int(0);
            }
            if let ExprKind::Bin {
                op: Xor,
                l: x,
                r: a,
            } = &l.kind
            {
                if let (Some(a), Some(b)) = (as_int(a), ri) {
                    *changed = true;
                    return bin(Xor, (**x).clone(), int(a ^ b), sp);
                }
            }
            if let ExprKind::Bin {
                op: Xor,
                l: x,
                r: y,
            } = &r.kind
            {
                if same(&l, x) && is_pure(&l, pure) && is_pure(y, pure) {
                    *changed = true;
                    return (**y).clone();
                }
            }
            if let ExprKind::Bin {
                op: Xor,
                l: x,
                r: y,
            } = &l.kind
            {
                if same(y, &r) && is_pure(&r, pure) {
                    *changed = true;
                    return (**x).clone();
                }
            }
            if let (
                ExprKind::Bin {
                    op: Xor,
                    l: x1,
                    r: a,
                },
                ExprKind::Bin {
                    op: Xor,
                    l: x2,
                    r: b,
                },
            ) = (&l.kind, &r.kind)
            {
                if same(x1, x2) && is_pure(x1, pure) {
                    if let (Some(a), Some(b)) = (as_int(a), as_int(b)) {
                        *changed = true;
                        return int(a ^ b);
                    }
                }
            }
            if let ExprKind::Bin { op: Or, l: x, r: y } = &r.kind {
                if same(&l, x) && is_pure(&l, pure) && is_pure(y, pure) {
                    *changed = true;
                    return bin(
                        And,
                        Expr {
                            span: sp,
                            kind: ExprKind::Un {
                                op: UnOp::BitNot,
                                e: x.clone(),
                            },
                        },
                        (**y).clone(),
                        sp,
                    );
                }
            }
            if let ExprKind::Bin {
                op: And,
                l: x,
                r: y,
            } = &r.kind
            {
                if same(&l, x) && is_pure(&l, pure) && is_pure(y, pure) {
                    *changed = true;
                    return bin(
                        And,
                        l,
                        Expr {
                            span: sp,
                            kind: ExprKind::Un {
                                op: UnOp::BitNot,
                                e: y.clone(),
                            },
                        },
                        sp,
                    );
                }
            }
            if let ExprKind::Bin {
                op: And,
                l: x,
                r: y,
            } = &l.kind
            {
                if same(x, &r) && is_pure(x, pure) && is_pure(y, pure) {
                    *changed = true;
                    return bin(
                        And,
                        r,
                        Expr {
                            span: sp,
                            kind: ExprKind::Un {
                                op: UnOp::BitNot,
                                e: y.clone(),
                            },
                        },
                        sp,
                    );
                }
            }
        }
        Shl | Shr => {
            if ri == Some(0) {
                *changed = true;
                return l;
            }
            if let Some(b) = ri {
                let masked = b & 63;
                if masked != b {
                    *changed = true;
                    return bin(op, l, int(masked), sp);
                }
            }
            if let ExprKind::Bin {
                op: iop,
                l: x,
                r: a,
            } = &l.kind
            {
                if *iop == op {
                    if let (Some(a), Some(b)) = (as_int(a), ri) {
                        *changed = true;
                        return bin(op, (**x).clone(), int(a.wrapping_add(b) & 63), sp);
                    }
                }
            }
            if li == Some(0) && is_pure(&r, pure) {
                *changed = true;
                return int(0);
            }
        }
        Eq | Ne => {
            if li.is_some() && ri.is_none() && is_pure(&l, pure) && is_pure(&r, pure) {
                let out = op;
                *changed = true;
                return bin(out, r, l, sp);
            }
            if same(&l, &r) && is_pure(&l, pure) && is_int_expr(&l) {
                *changed = true;
                return int(if op == Eq { 1 } else { 0 });
            }
            if is_bool(&l) && ri == Some(1) {
                *changed = true;
                return l;
            }
            if is_bool(&l) && op == Ne && ri == Some(0) {
                *changed = true;
                return l;
            }
            if is_bool(&l) && op == Eq && ri == Some(0) {
                *changed = true;
                return Expr {
                    span: sp,
                    kind: ExprKind::Un {
                        op: UnOp::Not,
                        e: Box::new(l),
                    },
                };
            }
            if is_bool(&l) && op == Ne && ri == Some(1) {
                *changed = true;
                return Expr {
                    span: sp,
                    kind: ExprKind::Un {
                        op: UnOp::Not,
                        e: Box::new(l),
                    },
                };
            }
        }
        Lt | Gt | Le | Ge => {
            if li.is_some() && ri.is_none() && is_pure(&l, pure) && is_pure(&r, pure) {
                let rev = match op {
                    Lt => Gt,
                    Gt => Lt,
                    Le => Ge,
                    Ge => Le,
                    _ => op,
                };
                *changed = true;
                return bin(rev, r, l, sp);
            }
        }
        LAnd => {
            if ri == Some(1) {
                *changed = true;
                return bin(Ne, l, int(0), sp);
            }
            if li == Some(1) {
                *changed = true;
                return bin(Ne, r, int(0), sp);
            }
            if ri == Some(0) && is_pure(&l, pure) {
                *changed = true;
                return int(0);
            }
            if li == Some(0) && is_pure(&r, pure) {
                *changed = true;
                return int(0);
            }
            if same(&l, &r) && is_pure(&l, pure) {
                *changed = true;
                return bin(Ne, l, int(0), sp);
            }
        }
        LOr => {
            if ri == Some(0) {
                *changed = true;
                return bin(Ne, l, int(0), sp);
            }
            if li == Some(0) {
                *changed = true;
                return bin(Ne, r, int(0), sp);
            }
            if ri == Some(1) && is_pure(&l, pure) {
                *changed = true;
                return int(1);
            }
            if li == Some(1) && is_pure(&r, pure) {
                *changed = true;
                return int(1);
            }
            if same(&l, &r) && is_pure(&l, pure) {
                *changed = true;
                return bin(Ne, l, int(0), sp);
            }
        }
    }

    Expr {
        span: sp,
        kind: ExprKind::Bin {
            op,
            l: Box::new(l),
            r: Box::new(r),
        },
    }
}

fn simp_let(e: Expr, pure: &HashSet<String>, changed: &mut bool) -> Expr {
    let sp = e.span;
    let ExprKind::Let { name, value, body } = e.kind else {
        return e;
    };
    let value = *value;
    let body = *body;
    let uses = count_var(&body, &name);

    if matches!(&body.kind, ExprKind::Var(n) if n == &name) {
        *changed = true;
        return value;
    }
    if uses == 0 && is_pure(&value, pure) {
        *changed = true;
        return body;
    }
    if uses == 1 {
        let b = subst(body.clone(), &name, &value);
        if count_var(&b, &name) == 0 {
            *changed = true;
            return b;
        }
    }
    if uses > 1 && is_pure(&value, pure) {
        let b = subst(body.clone(), &name, &value);
        if count_var(&b, &name) == 0 {
            *changed = true;
            return b;
        }
    }
    if let ExprKind::Let {
        name: inner,
        value: iv,
        body: ib,
    } = &value.kind
    {
        if count_var(iv, &name) == 0 {
            *changed = true;
            let rebound = Expr {
                span: sp,
                kind: ExprKind::Let {
                    name: name.clone(),
                    value: ib.clone(),
                    body: Box::new(body),
                },
            };
            return Expr {
                span: sp,
                kind: ExprKind::Let {
                    name: inner.clone(),
                    value: iv.clone(),
                    body: Box::new(rebound),
                },
            };
        }
    }
    if let ExprKind::Let {
        name: inner,
        value: iv,
        body: ib,
    } = &body.kind
    {
        if count_var(iv, &name) == 0 && is_pure(&value, pure) {
            let nb = subst((**ib).clone(), &name, &value);
            if count_var(&nb, &name) == 0 {
                *changed = true;
                return Expr {
                    span: sp,
                    kind: ExprKind::Let {
                        name: inner.clone(),
                        value: iv.clone(),
                        body: Box::new(nb),
                    },
                };
            }
        }
    }
    if matches!(
        &value.kind,
        ExprKind::Var(_) | ExprKind::Int(_) | ExprKind::Float(_)
    ) && uses > 0
    {
        let b = subst(body.clone(), &name, &value);
        if count_var(&b, &name) == 0 {
            *changed = true;
            return b;
        }
    }
    Expr {
        span: sp,
        kind: ExprKind::Let {
            name,
            value: Box::new(value),
            body: Box::new(body),
        },
    }
}

fn simp_app(e: Expr, pure: &HashSet<String>, changed: &mut bool) -> Expr {
    let sp = e.span;
    let ExprKind::App { fun, args } = e.kind else {
        return e;
    };
    let fun = *fun;

    if let ExprKind::Fn { params, body } = &fun.kind {
        if params.len() == args.len() && distinct(params) {
            let mut b = (**body).clone();
            let mut ok = true;
            for (param, arg) in params.iter().zip(args.iter()) {
                let n = count_var(&b, param);
                if n == 0 {
                    if !is_pure(arg, pure) {
                        ok = false;
                        break;
                    }
                } else if n == 1 || is_pure(arg, pure) {
                    b = subst(b, param, arg);
                } else {
                    ok = false;
                    break;
                }
                if count_var(&b, param) != 0 {
                    ok = false;
                    break;
                }
            }
            if ok {
                *changed = true;
                return b;
            }
        }
    }

    if let ExprKind::Fn { params, body } = &fun.kind {
        if params.len() == 1
            && args.len() == 1
            && matches!(&body.kind, ExprKind::Var(n) if n == &params[0])
        {
            *changed = true;
            return args.into_iter().next().unwrap();
        }
    }

    if let ExprKind::Fn { params, body } = &fun.kind {
        if params.len() == args.len()
            && params.iter().enumerate().all(
                |(i, p)| matches!(args.get(i).map(|x| &x.kind), Some(ExprKind::Var(v)) if v == p),
            )
        {
            if let ExprKind::App { fun: target, .. } = &body.kind {
                if params.iter().all(|p| count_var(target, p) == 0) && is_pure(target, pure) {
                    *changed = true;
                    return (**target).clone();
                }
            }
        }
    }

    Expr {
        span: sp,
        kind: ExprKind::App {
            fun: Box::new(fun),
            args,
        },
    }
}

fn simp_case(e: Expr, pure: &HashSet<String>, changed: &mut bool) -> Expr {
    let sp = e.span;
    let ExprKind::Case { scrut, arms } = e.kind else {
        return e;
    };
    let scrut = *scrut;
    let mut arms = arms;

    if let Some(first) = arms.first() {
        if matches!(first.pat, Pat::Wild) {
            if is_pure(&scrut, pure) {
                *changed = true;
                return first.body.clone();
            }
            *changed = true;
            return Expr {
                span: sp,
                kind: ExprKind::Block {
                    stmts: vec![scrut],
                    tail: Box::new(first.body.clone()),
                },
            };
        }
        if let Pat::Bind(name) = &first.pat {
            let uses = count_var(&first.body, name);
            if uses == 1 || is_pure(&scrut, pure) {
                let b = subst(first.body.clone(), name, &scrut);
                if count_var(&b, name) == 0 {
                    *changed = true;
                    return b;
                }
            }
            if uses == 0 && !is_pure(&scrut, pure) {
                *changed = true;
                return Expr {
                    span: sp,
                    kind: ExprKind::Block {
                        stmts: vec![scrut],
                        tail: Box::new(first.body.clone()),
                    },
                };
            }
        }
    }

    let mut seen = Vec::<Pat>::new();
    let mut kept = Vec::<Arm>::with_capacity(arms.len());
    for arm in arms {
        if seen.iter().any(|p| same_pat(p, &arm.pat)) {
            *changed = true;
            continue;
        }
        let stop = matches!(arm.pat, Pat::Wild | Pat::Bind(_));
        seen.push(arm.pat.clone());
        kept.push(arm);
        if stop {
            *changed = true;
            break;
        }
    }
    arms = kept;

    if let Some(v) = as_int(&scrut) {
        for arm in &arms {
            match &arm.pat {
                Pat::Int(pv) if *pv == v => {
                    *changed = true;
                    return arm.body.clone();
                }
                Pat::Bind(name) => {
                    let uses = count_var(&arm.body, name);
                    if uses <= 1 || is_pure(&scrut, pure) {
                        let b = subst(arm.body.clone(), name, &scrut);
                        if count_var(&b, name) == 0 {
                            *changed = true;
                            return b;
                        }
                    }
                }
                Pat::Wild => {
                    *changed = true;
                    return arm.body.clone();
                }
                _ => {}
            }
        }
    }

    if let ExprKind::Str(bytes) = &scrut.kind {
        for arm in &arms {
            match &arm.pat {
                Pat::Str(pb) if pb == bytes => {
                    *changed = true;
                    return arm.body.clone();
                }
                Pat::Bind(name) => {
                    let uses = count_var(&arm.body, name);
                    if uses <= 1 || is_pure(&scrut, pure) {
                        let b = subst(arm.body.clone(), name, &scrut);
                        if count_var(&b, name) == 0 {
                            *changed = true;
                            return b;
                        }
                    }
                }
                Pat::Wild => {
                    *changed = true;
                    return arm.body.clone();
                }
                _ => {}
            }
        }
    }

    if arms.len() == 1 && is_pure(&scrut, pure) {
        if let Pat::Bind(name) = &arms[0].pat {
            let b = subst(arms[0].body.clone(), name, &scrut);
            if count_var(&b, name) == 0 {
                *changed = true;
                return b;
            }
        }
        if matches!(arms[0].pat, Pat::Wild) {
            *changed = true;
            return arms[0].body.clone();
        }
    }

    if arms.len() > 1 {
        let first = &arms[0].body;
        if arms.iter().all(|a| same(first, &a.body)) {
            if arms
                .iter()
                .all(|a| matches!(a.pat, Pat::Wild | Pat::Bind(_)))
            {
                *changed = true;
                return first.clone();
            }
        }
    }

    Expr {
        span: sp,
        kind: ExprKind::Case {
            scrut: Box::new(scrut),
            arms,
        },
    }
}

fn simp_block(e: Expr, pure: &HashSet<String>, changed: &mut bool) -> Expr {
    let sp = e.span;
    let ExprKind::Block { stmts, tail } = e.kind else {
        return e;
    };
    let mut out = Vec::with_capacity(stmts.len());

    for s in stmts {
        match s.kind {
            ExprKind::Block {
                stmts: inner,
                tail: it,
            } => {
                *changed = true;
                out.extend(inner);
                out.push(*it);
            }
            _ => out.push(s),
        }
    }

    let tail_span = tail.span;
    let tail = match tail.kind {
        ExprKind::Block {
            stmts: inner,
            tail: inner_tail,
        } => {
            *changed = true;
            out.extend(inner);
            *inner_tail
        }
        kind => Expr {
            span: tail_span,
            kind,
        },
    };

    let before = out.len();
    out.retain(|s| !is_pure(s, pure));
    if before != out.len() {
        *changed = true;
    }

    if out.is_empty() {
        *changed = true;
        return tail;
    }

    let mut dedup = Vec::with_capacity(out.len());
    for s in out {
        if is_pure(&s, pure) && dedup.iter().any(|x| same(x, &s)) {
            *changed = true;
        } else {
            dedup.push(s);
        }
    }

    if dedup.len() == 1 && same(&dedup[0], &tail) && is_pure(&dedup[0], pure) {
        *changed = true;
        return tail;
    }

    Expr {
        span: sp,
        kind: ExprKind::Block {
            stmts: dedup,
            tail: Box::new(tail),
        },
    }
}

fn adv_opt(mut p: Program, pure: &HashSet<String>, changed: &mut bool) -> Program {
    let mut used = identifiers(&p);
    let mut serial = 0usize;
    p = map_prog(p, &mut |e| {
        adv_expr(e, pure, changed, &mut used, &mut serial)
    });
    adv_case(p, pure, changed, &mut used, &mut serial)
}

fn identifiers(p: &Program) -> HashSet<String> {
    let mut out = HashSet::new();
    fn pat(p: &Pat, out: &mut HashSet<String>) {
        match p {
            Pat::Bind(n) => {
                out.insert(n.clone());
            }
            Pat::Ctor { name, args } => {
                out.insert(name.clone());
                for a in args {
                    pat(a, out);
                }
            }
            Pat::Tuple(xs) => {
                for x in xs {
                    pat(x, out);
                }
            }
            Pat::Wild | Pat::Int(_) | Pat::Str(_) => {}
        }
    }
    fn expr(e: &Expr, out: &mut HashSet<String>) {
        match &e.kind {
            ExprKind::Var(n) => {
                out.insert(n.clone());
            }
            ExprKind::Let { name, value, body } => {
                out.insert(name.clone());
                expr(value, out);
                expr(body, out);
            }
            ExprKind::Fn { params, body } => {
                for n in params {
                    out.insert(n.clone());
                }
                expr(body, out);
            }
            ExprKind::App { fun, args } => {
                expr(fun, out);
                for a in args {
                    expr(a, out);
                }
            }
            ExprKind::Case { scrut, arms } => {
                expr(scrut, out);
                for a in arms {
                    pat(&a.pat, out);
                    expr(&a.body, out);
                }
            }
            ExprKind::Block { stmts, tail } => {
                for x in stmts {
                    expr(x, out);
                }
                expr(tail, out);
            }
            ExprKind::Bin { l, r, .. } => {
                expr(l, out);
                expr(r, out);
            }
            ExprKind::Un { e, .. } | ExprKind::Quote(e) => expr(e, out),
            ExprKind::Int(_) | ExprKind::Float(_) | ExprKind::Str(_) => {}
        }
    }
    fn ty(t: &Ty, out: &mut HashSet<String>) {
        match t {
            Ty::Named(n) => {
                out.insert(n.clone());
            }
            Ty::App(n, xs) => {
                out.insert(n.clone());
                for x in xs {
                    ty(x, out);
                }
            }
            Ty::Array(x) => ty(x, out),
            Ty::Tuple(xs) => {
                for x in xs {
                    ty(x, out);
                }
            }
            Ty::Fun(a, b) => {
                ty(a, out);
                ty(b, out);
            }
            Ty::Prim(_) | Ty::Bottom => {}
        }
    }
    for d in &p.decls {
        match d {
            Decl::Let { name, value, .. } => {
                out.insert(name.clone());
                expr(value, &mut out);
            }
            Decl::External(e) => {
                out.insert(e.name.clone());
            }
            Decl::Type { name, ctors, .. } => {
                out.insert(name.clone());
                for c in ctors {
                    out.insert(c.name.clone());
                    for t in &c.args {
                        ty(t, &mut out);
                    }
                }
            }
            Decl::Bring(_) => {}
        }
    }
    expr(&p.body, &mut out);
    out
}

fn fresh_opt_name(used: &mut HashSet<String>, serial: &mut usize, stem: &str) -> String {
    loop {
        let n = format!("__mak_opt_{}_{}", stem, *serial);
        *serial += 1;
        if used.insert(n.clone()) {
            return n;
        }
    }
}

fn assoc(op: BinOp) -> bool {
    matches!(
        op,
        BinOp::Add | BinOp::Mul | BinOp::And | BinOp::Or | BinOp::Xor
    )
}

fn flatten_assoc(op: BinOp, e: &Expr, out: &mut Vec<Expr>) {
    if let ExprKind::Bin { op: inner, l, r } = &e.kind {
        if *inner == op {
            flatten_assoc(op, l, out);
            flatten_assoc(op, r, out);
            return;
        }
    }
    out.push(e.clone());
}

fn rebuild_assoc(op: BinOp, xs: &[Expr], sp: Span) -> Expr {
    let mut it = xs.iter();
    let mut acc = it.next().cloned().unwrap_or_else(|| int(0));
    for x in it {
        acc = bin(op, acc, x.clone(), sp);
    }
    acc
}

fn canon_assoc(e: Expr, pure: &HashSet<String>, changed: &mut bool) -> Expr {
    let sp = e.span;
    let ExprKind::Bin { op, .. } = &e.kind else {
        return e;
    };
    if !assoc(*op) {
        return e;
    }
    let mut xs = Vec::new();
    flatten_assoc(*op, &e, &mut xs);
    if xs.len() < 3 || !xs.iter().all(|x| is_pure(x, pure)) {
        return e;
    }
    let mut constants = Vec::new();
    let mut nonconst = Vec::new();
    for x in xs {
        if let Some(v) = as_int(&x) {
            constants.push(v);
        } else {
            nonconst.push(x);
        }
    }
    if constants.len() < 2 {
        return e;
    }
    let folded = match op {
        BinOp::Add => constants.into_iter().fold(0u64, |a, b| a.wrapping_add(b)),
        BinOp::Mul => constants.into_iter().fold(1u64, |a, b| a.wrapping_mul(b)),
        BinOp::And => constants.into_iter().fold(u64::MAX, |a, b| a & b),
        BinOp::Or => constants.into_iter().fold(0u64, |a, b| a | b),
        BinOp::Xor => constants.into_iter().fold(0u64, |a, b| a ^ b),
        _ => return e,
    };
    let neutral = match op {
        BinOp::Add | BinOp::Or | BinOp::Xor => folded == 0,
        BinOp::Mul => folded == 1,
        BinOp::And => folded == u64::MAX,
        _ => false,
    };
    if neutral && nonconst.is_empty() {
        *changed = true;
        return int(folded);
    }
    if neutral && !nonconst.is_empty() {
        *changed = true;
        return rebuild_assoc(*op, &nonconst, sp);
    }
    nonconst.push(int(folded));
    *changed = true;
    rebuild_assoc(*op, &nonconst, sp)
}

fn invert(op: BinOp) -> Option<BinOp> {
    Some(match op {
        BinOp::Eq => BinOp::Ne,
        BinOp::Ne => BinOp::Eq,
        BinOp::Lt => BinOp::Ge,
        BinOp::Le => BinOp::Gt,
        BinOp::Gt => BinOp::Le,
        BinOp::Ge => BinOp::Lt,
        _ => return None,
    })
}

fn adv_un(e: Expr, pure: &HashSet<String>, changed: &mut bool) -> Expr {
    let sp = e.span;
    let ExprKind::Un {
        op: UnOp::Not,
        e: inner,
    } = &e.kind
    else {
        return e;
    };
    if let ExprKind::Bin { op, l, r } = &inner.kind {
        if matches!(op, BinOp::LAnd | BinOp::LOr) {
            *changed = true;
            let bop = if *op == BinOp::LAnd {
                BinOp::LOr
            } else {
                BinOp::LAnd
            };
            return bin(
                bop,
                Expr {
                    span: sp,
                    kind: ExprKind::Un {
                        op: UnOp::Not,
                        e: l.clone(),
                    },
                },
                Expr {
                    span: sp,
                    kind: ExprKind::Un {
                        op: UnOp::Not,
                        e: r.clone(),
                    },
                },
                sp,
            );
        }
        if let Some(inv) = invert(*op) {
            *changed = true;
            return bin(inv, (**l).clone(), (**r).clone(), sp);
        }
    }
    let _ = pure;
    e
}

fn pat_shape_eq(a: &Pat, b: &Pat) -> bool {
    match (a, b) {
        (Pat::Wild, Pat::Wild) => true,
        (Pat::Int(x), Pat::Int(y)) => x == y,
        (Pat::Str(x), Pat::Str(y)) => x == y,
        (Pat::Bind(_), Pat::Bind(_)) => true,
        (Pat::Ctor { name: an, args: aa }, Pat::Ctor { name: bn, args: ba }) => {
            an == bn && aa.len() == ba.len() && aa.iter().zip(ba).all(|(x, y)| pat_shape_eq(x, y))
        }
        (Pat::Tuple(ax), Pat::Tuple(bx)) => {
            ax.len() == bx.len() && ax.iter().zip(bx).all(|(x, y)| pat_shape_eq(x, y))
        }
        _ => false,
    }
}

fn arm_uses(arm: &Arm) -> bool {
    let mut names = Vec::new();
    pat_binders(&arm.pat, &mut names);
    names.iter().any(|n| count_var(&arm.body, n) != 0)
}

fn preserve_scrut(
    scrut: Expr,
    body: Expr,
    pure: &HashSet<String>,
    used: &mut HashSet<String>,
    serial: &mut usize,
) -> Expr {
    if is_pure(&scrut, pure) {
        body
    } else {
        let name = fresh_opt_name(used, serial, "scrut");
        Expr {
            span: body.span,
            kind: ExprKind::Let {
                name,
                value: Box::new(scrut),
                body: Box::new(body),
            },
        }
    }
}

fn prune_arms(
    scrut: Expr,
    mut arms: Vec<Arm>,
    pure: &HashSet<String>,
    changed: &mut bool,
    used: &mut HashSet<String>,
    serial: &mut usize,
) -> Expr {
    let mut seen = Vec::<Pat>::new();
    let mut kept = Vec::with_capacity(arms.len());
    for arm in arms.drain(..) {
        if seen.iter().any(|p| pat_shape_eq(p, &arm.pat)) {
            *changed = true;
            continue;
        }
        let total = matches!(arm.pat, Pat::Wild | Pat::Bind(_));
        seen.push(arm.pat.clone());
        kept.push(arm);
        if total {
            if kept.len() != seen.len() {
                *changed = true;
            }
            break;
        }
    }
    if kept.len() != seen.len() {
        *changed = true;
    }
    if kept.is_empty() {
        return Expr {
            span: scrut.span,
            kind: ExprKind::Case {
                scrut: Box::new(scrut),
                arms: Vec::new(),
            },
        };
    }
    if kept.len() == 1 && matches!(kept[0].pat, Pat::Wild | Pat::Bind(_)) {
        let arm = kept.remove(0);
        *changed = true;

        match arm.pat {
            Pat::Wild => {
                return preserve_scrut(scrut, arm.body, pure, used, serial);
            }
            Pat::Bind(n) => {
                if is_pure(&scrut, pure) {
                    return subst(arm.body, &n, &scrut);
                }

                let bind_name = fresh_opt_name(used, serial, "scrut");
                let bound = Expr {
                    span: arm.body.span,
                    kind: ExprKind::Var(bind_name.clone()),
                };
                let body = subst(arm.body, &n, &bound);
                return Expr {
                    span: body.span,
                    kind: ExprKind::Let {
                        name: bind_name,
                        value: Box::new(scrut),
                        body: Box::new(body),
                    },
                };
            }
            _ => unreachable!(),
        }
    }
    if kept.len() >= 2 {
        let same_body = kept.windows(2).all(|w| same(&w[0].body, &w[1].body));
        if same_body && exhaustive(&kept) && kept.iter().all(|a| !arm_uses(a)) {
            *changed = true;
            return preserve_scrut(scrut, kept[0].body.clone(), pure, used, serial);
        }
    }
    if kept.len() != seen.len() {
        *changed = true;
    }
    Expr {
        span: scrut.span,
        kind: ExprKind::Case {
            scrut: Box::new(scrut),
            arms: kept,
        },
    }
}

fn adv_case(
    p: Program,
    pure: &HashSet<String>,
    changed: &mut bool,
    used: &mut HashSet<String>,
    serial: &mut usize,
) -> Program {
    fn go(
        e: Expr,
        pure: &HashSet<String>,
        changed: &mut bool,
        used: &mut HashSet<String>,
        serial: &mut usize,
    ) -> Expr {
        let e = map_child(e, &mut |x| go(x, pure, changed, used, serial));
        if let ExprKind::Case { scrut, arms } = e.kind {
            return prune_arms(*scrut, arms, pure, changed, used, serial);
        }
        e
    }
    map_prog(p, &mut |e| go(e, pure, changed, used, serial))
}

fn binder_set(p: &Pat, out: &mut HashSet<String>) {
    match p {
        Pat::Bind(n) => {
            out.insert(n.clone());
        }
        Pat::Ctor { args, .. } => {
            for a in args {
                binder_set(a, out);
            }
        }
        Pat::Tuple(xs) => {
            for x in xs {
                binder_set(x, out);
            }
        }
        Pat::Wild | Pat::Int(_) | Pat::Str(_) => {}
    }
}

fn uses_any(e: &Expr, names: &HashSet<String>) -> bool {
    let mut found = false;
    walk(e, &mut |x| {
        if let ExprKind::Var(n) = &x.kind {
            if names.contains(n) {
                found = true;
            }
        }
    });
    found
}

fn arm_caps(arm: &Arm, e: &Expr) -> bool {
    let mut binders = HashSet::new();
    binder_set(&arm.pat, &mut binders);
    uses_any(e, &binders)
}

fn exhaustive(arms: &[Arm]) -> bool {
    arms.iter()
        .any(|a| matches!(a.pat, Pat::Wild | Pat::Bind(_)))
}

fn distribute(e: Expr, pure: &HashSet<String>, changed: &mut bool) -> Expr {
    let sp = e.span;
    match e.kind {
        ExprKind::App { fun, args } => {
            for i in 0..args.len() {
                if let ExprKind::Case { scrut, arms } = &args[i].kind {
                    if !is_pure(&fun, pure) || args[..i].iter().any(|a| !is_pure(a, pure)) {
                        continue;
                    }
                    if arms.iter().any(|arm| {
                        arm_caps(arm, &fun)
                            || args
                                .iter()
                                .enumerate()
                                .any(|(j, a)| j != i && arm_caps(arm, a))
                    }) {
                        continue;
                    }
                    let mut out = Vec::with_capacity(arms.len());
                    for arm in arms {
                        let mut na = args.clone();
                        na[i] = arm.body.clone();
                        out.push(Arm {
                            pat: arm.pat.clone(),
                            span: arm.span,
                            body: Expr {
                                span: sp,
                                kind: ExprKind::App {
                                    fun: fun.clone(),
                                    args: na,
                                },
                            },
                        });
                    }
                    *changed = true;
                    return Expr {
                        span: sp,
                        kind: ExprKind::Case {
                            scrut: scrut.clone(),
                            arms: out,
                        },
                    };
                }
            }
            Expr {
                span: sp,
                kind: ExprKind::App { fun, args },
            }
        }
        ExprKind::Un { op, e: inner } => {
            if let ExprKind::Case { scrut, arms } = &inner.kind {
                *changed = true;
                let out = arms
                    .iter()
                    .map(|a| Arm {
                        pat: a.pat.clone(),
                        span: a.span,
                        body: Expr {
                            span: sp,
                            kind: ExprKind::Un {
                                op,
                                e: Box::new(a.body.clone()),
                            },
                        },
                    })
                    .collect();
                Expr {
                    span: sp,
                    kind: ExprKind::Case {
                        scrut: scrut.clone(),
                        arms: out,
                    },
                }
            } else {
                Expr {
                    span: sp,
                    kind: ExprKind::Un { op, e: inner },
                }
            }
        }
        ExprKind::Bin { op, l, r } => {
            if let ExprKind::Case { scrut, arms } = &l.kind {
                if arms.iter().any(|arm| arm_caps(arm, &r)) {
                    return Expr {
                        span: sp,
                        kind: ExprKind::Bin { op, l, r },
                    };
                }
                let out = arms
                    .iter()
                    .map(|a| Arm {
                        pat: a.pat.clone(),
                        span: a.span,
                        body: Expr {
                            span: sp,
                            kind: ExprKind::Bin {
                                op,
                                l: Box::new(a.body.clone()),
                                r: r.clone(),
                            },
                        },
                    })
                    .collect();
                *changed = true;
                return Expr {
                    span: sp,
                    kind: ExprKind::Case {
                        scrut: scrut.clone(),
                        arms: out,
                    },
                };
            }
            if let ExprKind::Case { scrut, arms } = &r.kind {
                if is_pure(&l, pure) && !arms.iter().any(|arm| arm_caps(arm, &l)) {
                    let out = arms
                        .iter()
                        .map(|a| Arm {
                            pat: a.pat.clone(),
                            span: a.span,
                            body: Expr {
                                span: sp,
                                kind: ExprKind::Bin {
                                    op,
                                    l: l.clone(),
                                    r: Box::new(a.body.clone()),
                                },
                            },
                        })
                        .collect();
                    *changed = true;
                    return Expr {
                        span: sp,
                        kind: ExprKind::Case {
                            scrut: scrut.clone(),
                            arms: out,
                        },
                    };
                }
            }
            Expr {
                span: sp,
                kind: ExprKind::Bin { op, l, r },
            }
        }
        other => Expr {
            span: sp,
            kind: other,
        },
    }
}

fn factor(e: Expr, pure: &HashSet<String>, changed: &mut bool) -> Expr {
    let sp = e.span;
    let ExprKind::Bin { op, l, r } = &e.kind else {
        return e;
    };
    if !matches!(op, BinOp::Add | BinOp::Sub) {
        return e;
    }
    fn mul_const(x: &Expr) -> Option<(Expr, u64)> {
        match &x.kind {
            ExprKind::Bin {
                op: BinOp::Mul,
                l,
                r,
            } => {
                if let Some(c) = as_int(r) {
                    Some(((**l).clone(), c))
                } else if let Some(c) = as_int(l) {
                    Some(((**r).clone(), c))
                } else {
                    None
                }
            }
            _ => None,
        }
    }
    let Some((a, ca)) = mul_const(l) else {
        return e;
    };
    let Some((b, cb)) = mul_const(r) else {
        return e;
    };
    if ca != cb || !is_pure(&a, pure) || !is_pure(&b, pure) || !is_int_expr(&a) || !is_int_expr(&b)
    {
        return e;
    }
    *changed = true;
    bin(BinOp::Mul, bin(*op, a, b, sp), int(ca), sp)
}

fn boolean(e: Expr, pure: &HashSet<String>, changed: &mut bool) -> Expr {
    let sp = e.span;
    let ExprKind::Bin { op, l, r } = &e.kind else {
        return e;
    };
    match op {
        BinOp::LAnd | BinOp::LOr => {
            if let (
                ExprKind::Bin {
                    op: lop,
                    l: al,
                    r: ar,
                },
                ExprKind::Bin {
                    op: rop,
                    l: bl,
                    r: br,
                },
            ) = (&l.kind, &r.kind)
            {
                if *op == BinOp::LOr
                    && *lop == BinOp::LAnd
                    && *rop == BinOp::LAnd
                    && same(al, bl)
                    && is_pure(al, pure)
                {
                    *changed = true;
                    return bin(
                        BinOp::LAnd,
                        (**al).clone(),
                        bin(BinOp::LOr, (**ar).clone(), (**br).clone(), sp),
                        sp,
                    );
                }
                if *op == BinOp::LAnd
                    && *lop == BinOp::LOr
                    && *rop == BinOp::LOr
                    && same(al, bl)
                    && is_pure(al, pure)
                {
                    *changed = true;
                    return bin(
                        BinOp::LOr,
                        (**al).clone(),
                        bin(BinOp::LAnd, (**ar).clone(), (**br).clone(), sp),
                        sp,
                    );
                }
            }
            if same(l, r) && is_pure(l, pure) {
                *changed = true;
                return bin(BinOp::Ne, (**l).clone(), int(0), sp);
            }
        }
        BinOp::Eq | BinOp::Ne | BinOp::Lt | BinOp::Le | BinOp::Gt | BinOp::Ge => {
            if same(l, r) && is_pure(l, pure) && is_int_expr(l) {
                *changed = true;
                return int(match op {
                    BinOp::Eq | BinOp::Le | BinOp::Ge => 1,
                    _ => 0,
                });
            }
        }
        _ => {}
    }
    e
}

fn eta(e: Expr, pure: &HashSet<String>, changed: &mut bool) -> Expr {
    let ExprKind::Fn { params, body } = &e.kind else {
        return e;
    };
    if params.is_empty() || !distinct(params) {
        return e;
    }
    let ExprKind::App { fun, args } = &body.kind else {
        return e;
    };
    if args.len() != params.len() || !is_pure(fun, pure) {
        return e;
    }
    for (p, a) in params.iter().zip(args) {
        if !matches!(&a.kind, ExprKind::Var(n) if n == p) || count_var(fun, p) != 0 {
            return e;
        }
    }
    *changed = true;
    (**fun).clone()
}

fn cse(
    e: Expr,
    pure: &HashSet<String>,
    changed: &mut bool,
    used: &mut HashSet<String>,
    serial: &mut usize,
) -> Expr {
    let sp = e.span;
    let ExprKind::Bin { op, l, r } = &e.kind else {
        return e;
    };
    if matches!(op, BinOp::LAnd | BinOp::LOr) || !same(l, r) || !is_pure(l, pure) {
        return e;
    }
    if matches!(
        &l.kind,
        ExprKind::Var(_) | ExprKind::Int(_) | ExprKind::Float(_) | ExprKind::Str(_)
    ) {
        return e;
    }
    let name = fresh_opt_name(used, serial, "cse");
    let var1 = Expr {
        span: sp,
        kind: ExprKind::Var(name.clone()),
    };
    let var2 = Expr {
        span: sp,
        kind: ExprKind::Var(name.clone()),
    };
    let body = bin(*op, var1, var2, sp);
    *changed = true;
    Expr {
        span: sp,
        kind: ExprKind::Let {
            name,
            value: Box::new((**l).clone()),
            body: Box::new(body),
        },
    }
}

fn adv_expr(
    e: Expr,
    pure: &HashSet<String>,
    changed: &mut bool,
    used: &mut HashSet<String>,
    serial: &mut usize,
) -> Expr {
    let e = map_child(e, &mut |x| adv_expr(x, pure, changed, used, serial));
    let e = distribute(e, pure, changed);
    let e = factor(e, pure, changed);
    let e = boolean(e, pure, changed);
    let e = adv_un(e, pure, changed);
    let e = eta(e, pure, changed);
    let e = canon_assoc(e, pure, changed);
    cse(e, pure, changed, used, serial)
}

fn effect_const(e: Expr, n: u64, pure: &HashSet<String>) -> Expr {
    if is_pure(&e, pure) {
        int(n)
    } else {
        Expr {
            span: e.span,
            kind: ExprKind::Block {
                stmts: vec![e],
                tail: Box::new(int(n)),
            },
        }
    }
}

fn mega_arith(e: Expr, pure: &HashSet<String>, changed: &mut bool) -> Expr {
    let sp = e.span;
    let ExprKind::Bin { op, l, r } = &e.kind else {
        return e;
    };
    let l = (**l).clone();
    let r = (**r).clone();
    let li = as_int(&l);
    let ri = as_int(&r);
    use BinOp::*;
    match op {
        Add => {
            if let ExprKind::Un {
                op: UnOp::Neg,
                e: x,
            } = &r.kind
            {
                *changed = true;
                return bin(Sub, l, (**x).clone(), sp);
            }
            if let ExprKind::Un {
                op: UnOp::Neg,
                e: x,
            } = &l.kind
            {
                if is_pure(x, pure) && is_pure(&r, pure) {
                    *changed = true;
                    return bin(Sub, r, (**x).clone(), sp);
                }
            }
            if let ExprKind::Bin {
                op: Sub,
                l: x,
                r: y,
            } = &l.kind
            {
                if same(y, &r) && is_pure(y, pure) {
                    *changed = true;
                    return (**x).clone();
                }
            }
            if let ExprKind::Bin {
                op: Sub,
                l: x,
                r: y,
            } = &r.kind
            {
                if same(&l, x) && is_pure(&l, pure) && is_pure(y, pure) {
                    *changed = true;
                    return (**y).clone();
                }
            }
            if let ExprKind::Bin {
                op: Add,
                l: x,
                r: y,
            } = &l.kind
            {
                if let Some(a) = as_int(y) {
                    if let Some(b) = ri {
                        *changed = true;
                        return bin(Add, (**x).clone(), int(a.wrapping_add(b)), sp);
                    }
                }
            }
            if let ExprKind::Bin {
                op: Add,
                l: x,
                r: y,
            } = &r.kind
            {
                if let Some(a) = as_int(y) {
                    if let Some(b) = li {
                        *changed = true;
                        return bin(Add, (**x).clone(), int(a.wrapping_add(b)), sp);
                    }
                }
            }
        }
        Sub => {
            if li == Some(0) {
                *changed = true;
                return Expr {
                    span: sp,
                    kind: ExprKind::Un {
                        op: UnOp::Neg,
                        e: Box::new(r),
                    },
                };
            }
            if let ExprKind::Un {
                op: UnOp::Neg,
                e: x,
            } = &r.kind
            {
                *changed = true;
                return bin(Add, l, (**x).clone(), sp);
            }
            if let ExprKind::Bin {
                op: Add,
                l: x,
                r: y,
            } = &l.kind
            {
                if same(y, &r) && is_pure(y, pure) {
                    *changed = true;
                    return (**x).clone();
                }
            }
            if let ExprKind::Bin {
                op: Add,
                l: x,
                r: y,
            } = &r.kind
            {
                if same(&l, x) && is_pure(&l, pure) && is_pure(y, pure) {
                    *changed = true;
                    return Expr {
                        span: sp,
                        kind: ExprKind::Un {
                            op: UnOp::Neg,
                            e: y.clone(),
                        },
                    };
                }
            }
            if let ExprKind::Bin {
                op: Sub,
                l: x,
                r: y,
            } = &l.kind
            {
                if same(y, &r) && is_pure(y, pure) {
                    *changed = true;
                    return (**x).clone();
                }
            }
        }
        Mul => {
            if ri == Some(0) {
                *changed = true;
                return effect_const(l, 0, pure);
            }
            if li == Some(0) {
                *changed = true;
                return effect_const(r, 0, pure);
            }
            if let ExprKind::Bin {
                op: Div,
                l: x,
                r: y,
            } = &l.kind
            {
                if same(x, &r) && as_int(y).is_some() && as_int(y) != Some(0) && is_pure(x, pure) {
                    *changed = true;
                    return bin(Div, (**x).clone(), (**y).clone(), sp);
                }
            }
            if let ExprKind::Bin {
                op: Mul,
                l: x,
                r: y,
            } = &l.kind
            {
                if let (Some(a), Some(b)) = (as_int(y), ri) {
                    *changed = true;
                    return bin(Mul, (**x).clone(), int(a.wrapping_mul(b)), sp);
                }
            }
            if let ExprKind::Bin {
                op: Mul,
                l: x,
                r: y,
            } = &r.kind
            {
                if let (Some(a), Some(b)) = (as_int(y), li) {
                    *changed = true;
                    return bin(Mul, (**x).clone(), int(a.wrapping_mul(b)), sp);
                }
            }
        }
        Div => {
            if ri == Some(1) {
                *changed = true;
                return l;
            }
            if let ExprKind::Bin {
                op: Div,
                l: x,
                r: c1,
            } = &l.kind
            {
                if let (Some(a), Some(b)) = (as_int(c1), ri) {
                    if a != 0 && b != 0 && a.checked_mul(b).is_some() {
                        *changed = true;
                        return bin(Div, (**x).clone(), int(a.wrapping_mul(b)), sp);
                    }
                }
            }
        }
        Rem => {
            if ri == Some(1) {
                *changed = true;
                return effect_const(l, 0, pure);
            }
            if let ExprKind::Bin {
                op: Rem,
                l: x,
                r: c1,
            } = &l.kind
            {
                if let (Some(a), Some(b)) = (as_int(c1), ri) {
                    if a == b && b != 0 {
                        *changed = true;
                        return bin(Rem, (**x).clone(), int(b), sp);
                    }
                }
            }
        }
        _ => {}
    }
    e
}

fn mega_bits(e: Expr, pure: &HashSet<String>, changed: &mut bool) -> Expr {
    let sp = e.span;
    let ExprKind::Bin { op, l, r } = &e.kind else {
        return e;
    };
    let l = (**l).clone();
    let r = (**r).clone();
    let li = as_int(&l);
    let ri = as_int(&r);
    use BinOp::*;
    match op {
        And => {
            if ri == Some(0) {
                *changed = true;
                return effect_const(l, 0, pure);
            }
            if li == Some(0) {
                *changed = true;
                return effect_const(r, 0, pure);
            }
            if ri == Some(u64::MAX) {
                *changed = true;
                return l;
            }
            if li == Some(u64::MAX) {
                *changed = true;
                return r;
            }
            if same(&l, &r) && is_pure(&l, pure) {
                *changed = true;
                return l;
            }
            if let ExprKind::Bin { op: Or, l: x, r: y } = &l.kind {
                if same(x, &r) && is_pure(x, pure) && is_pure(y, pure) {
                    *changed = true;
                    return r;
                }
                if same(y, &r) && is_pure(y, pure) && is_pure(x, pure) {
                    *changed = true;
                    return r;
                }
            }
            if let ExprKind::Bin { op: Or, l: x, r: m } = &r.kind {
                if same(x, &l) && is_pure(x, pure) && is_pure(m, pure) {
                    *changed = true;
                    return l;
                }
                if same(m, &l) && is_pure(m, pure) && is_pure(x, pure) {
                    *changed = true;
                    return l;
                }
            }
            if let ExprKind::Bin {
                op: Xor,
                l: x,
                r: y,
            } = &r.kind
            {
                if same(&l, x) && is_pure(&l, pure) && is_pure(y, pure) {
                    *changed = true;
                    return bin(
                        And,
                        l,
                        Expr {
                            span: sp,
                            kind: ExprKind::Un {
                                op: UnOp::BitNot,
                                e: y.clone(),
                            },
                        },
                        sp,
                    );
                }
            }
            if let ExprKind::Bin {
                op: And,
                l: x,
                r: y,
            } = &l.kind
            {
                if same(y, &r) && is_pure(y, pure) {
                    *changed = true;
                    return bin(And, (**x).clone(), (**y).clone(), sp);
                }
            }
            if let (Some(a), Some(b)) = (li, ri) {
                *changed = true;
                return int(a & b);
            }
        }
        Or => {
            if ri == Some(0) {
                *changed = true;
                return l;
            }
            if li == Some(0) {
                *changed = true;
                return r;
            }
            if ri == Some(u64::MAX) {
                *changed = true;
                return effect_const(l, u64::MAX, pure);
            }
            if li == Some(u64::MAX) {
                *changed = true;
                return effect_const(r, u64::MAX, pure);
            }
            if same(&l, &r) && is_pure(&l, pure) {
                *changed = true;
                return l;
            }
            if let ExprKind::Bin {
                op: And,
                l: x,
                r: m,
            } = &r.kind
            {
                if same(x, &l) && is_pure(x, pure) && is_pure(m, pure) {
                    *changed = true;
                    return l;
                }
            }
            if let ExprKind::Bin {
                op: Xor,
                l: x,
                r: m,
            } = &r.kind
            {
                if same(x, &l) && is_pure(x, pure) {
                    *changed = true;
                    return bin(Or, l, (**m).clone(), sp);
                }
            }
            if let (Some(a), Some(b)) = (li, ri) {
                *changed = true;
                return int(a | b);
            }
        }
        Xor => {
            if ri == Some(0) {
                *changed = true;
                return l;
            }
            if li == Some(0) {
                *changed = true;
                return r;
            }
            if ri == Some(u64::MAX) {
                *changed = true;
                return Expr {
                    span: sp,
                    kind: ExprKind::Un {
                        op: UnOp::BitNot,
                        e: Box::new(l),
                    },
                };
            }
            if li == Some(u64::MAX) {
                *changed = true;
                return Expr {
                    span: sp,
                    kind: ExprKind::Un {
                        op: UnOp::BitNot,
                        e: Box::new(r),
                    },
                };
            }
            if same(&l, &r) && is_pure(&l, pure) {
                *changed = true;
                return int(0);
            }
            if let ExprKind::Bin {
                op: Xor,
                l: x,
                r: y,
            } = &r.kind
            {
                if same(&l, x) && is_pure(&l, pure) && is_pure(y, pure) {
                    *changed = true;
                    return (**y).clone();
                }
            }
            if let ExprKind::Bin {
                op: Xor,
                l: x,
                r: y,
            } = &l.kind
            {
                if same(y, &r) && is_pure(&r, pure) && is_pure(x, pure) {
                    *changed = true;
                    return (**x).clone();
                }
            }
            if let ExprKind::Bin { op: Or, l: x, r: m } = &l.kind {
                if same(x, &r) && is_pure(x, pure) {
                    *changed = true;
                    return bin(
                        And,
                        Expr {
                            span: sp,
                            kind: ExprKind::Un {
                                op: UnOp::BitNot,
                                e: x.clone(),
                            },
                        },
                        (**m).clone(),
                        sp,
                    );
                }
            }
            if let ExprKind::Bin {
                op: And,
                l: x,
                r: m,
            } = &r.kind
            {
                if same(&l, x) && is_pure(x, pure) {
                    *changed = true;
                    return bin(
                        And,
                        l,
                        Expr {
                            span: sp,
                            kind: ExprKind::Un {
                                op: UnOp::BitNot,
                                e: m.clone(),
                            },
                        },
                        sp,
                    );
                }
            }
            if let (Some(a), Some(b)) = (li, ri) {
                *changed = true;
                return int(a ^ b);
            }
        }
        Shl | Shr => {
            if ri == Some(0) {
                *changed = true;
                return l;
            }
            if let ExprKind::Bin {
                op: iop,
                l: x,
                r: a,
            } = &l.kind
            {
                if *iop == *op {
                    if let (Some(a), Some(b)) = (as_int(a), ri) {
                        *changed = true;
                        return bin(*op, (**x).clone(), int(a.wrapping_add(b) & 63), sp);
                    }
                }
            }
            if let (Some(a), Some(b)) = (li, ri) {
                if b & 63 < 64 {
                    *changed = true;
                    return int(eval_bin(*op, a, b).unwrap_or(a));
                }
            }
        }
        _ => {}
    }
    e
}

fn mega_logic(e: Expr, pure: &HashSet<String>, changed: &mut bool) -> Expr {
    let sp = e.span;
    let ExprKind::Bin { op, l, r } = &e.kind else {
        return e;
    };
    let l = (**l).clone();
    let r = (**r).clone();
    let li = as_int(&l);
    let ri = as_int(&r);
    use BinOp::*;
    match op {
        LAnd => {
            if li == Some(0) {
                *changed = true;
                return int(0);
            }
            if ri == Some(0) {
                *changed = true;
                return effect_const(l, 0, pure);
            }
            if li == Some(1) {
                *changed = true;
                return bin(Ne, r, int(0), sp);
            }
            if ri == Some(1) {
                *changed = true;
                return bin(Ne, l, int(0), sp);
            }
            if same(&l, &r) && is_pure(&l, pure) {
                *changed = true;
                return bin(Ne, l, int(0), sp);
            }
        }
        LOr => {
            if li == Some(1) {
                *changed = true;
                return int(1);
            }
            if ri == Some(1) {
                *changed = true;
                return effect_const(l, 1, pure);
            }
            if li == Some(0) {
                *changed = true;
                return bin(Ne, r, int(0), sp);
            }
            if ri == Some(0) {
                *changed = true;
                return bin(Ne, l, int(0), sp);
            }
            if same(&l, &r) && is_pure(&l, pure) {
                *changed = true;
                return bin(Ne, l, int(0), sp);
            }
        }
        _ => {}
    }
    e
}

fn mega_cmp(e: Expr, pure: &HashSet<String>, changed: &mut bool) -> Expr {
    let sp = e.span;
    let ExprKind::Bin { op, l, r } = &e.kind else {
        return e;
    };
    let l = (**l).clone();
    let r = (**r).clone();
    let li = as_int(&l);
    let ri = as_int(&r);
    use BinOp::*;
    match op {
        Eq | Ne | Lt | Gt | Le | Ge => {
            if let (Some(a), Some(b)) = (li, ri) {
                *changed = true;
                return int(eval_bin(*op, a, b).unwrap_or(0));
            }
            if same(&l, &r) && is_pure(&l, pure) {
                *changed = true;
                return int(match op {
                    Eq | Le | Ge => 1,
                    _ => 0,
                });
            }
            if is_bool(&l) {
                if ri == Some(0) {
                    *changed = true;
                    return if matches!(op, Eq) {
                        Expr {
                            span: sp,
                            kind: ExprKind::Un {
                                op: UnOp::Not,
                                e: Box::new(l),
                            },
                        }
                    } else if matches!(op, Ne) {
                        l
                    } else {
                        e
                    };
                }
                if ri == Some(1) {
                    *changed = true;
                    return if matches!(op, Eq) {
                        l
                    } else if matches!(op, Ne) {
                        Expr {
                            span: sp,
                            kind: ExprKind::Un {
                                op: UnOp::Not,
                                e: Box::new(l),
                            },
                        }
                    } else {
                        e
                    };
                }
            }
            if is_bool(&l) {
                if matches!(op, Lt) && ri == Some(1) {
                    *changed = true;
                    return Expr {
                        span: sp,
                        kind: ExprKind::Un {
                            op: UnOp::Not,
                            e: Box::new(l),
                        },
                    };
                }
                if matches!(op, Ge) && ri == Some(1) {
                    *changed = true;
                    return l;
                }
            }
            if is_bool(&r) && li == Some(1) && matches!(op, Lt) {
                *changed = true;
                return Expr {
                    span: sp,
                    kind: ExprKind::Un {
                        op: UnOp::Not,
                        e: Box::new(r),
                    },
                };
            }
        }
        _ => {}
    }
    e
}

fn mega_un(e: Expr, pure: &HashSet<String>, changed: &mut bool) -> Expr {
    let sp = e.span;
    let ExprKind::Un { op, e: inner } = &e.kind else {
        return e;
    };
    let inner = (**inner).clone();
    match op {
        UnOp::Neg => {
            if let ExprKind::Un {
                op: UnOp::Neg,
                e: x,
            } = &inner.kind
            {
                *changed = true;
                return (**x).clone();
            }
            if let ExprKind::Bin {
                op: BinOp::Sub,
                l,
                r,
            } = &inner.kind
            {
                *changed = true;
                return bin(BinOp::Sub, (**r).clone(), (**l).clone(), sp);
            }
        }
        UnOp::Not => {
            if let ExprKind::Un {
                op: UnOp::Not,
                e: x,
            } = &inner.kind
            {
                *changed = true;
                return bin(BinOp::Ne, (**x).clone(), int(0), sp);
            }
            if let ExprKind::Bin { op, l, r } = &inner.kind {
                if let Some(i) = invert(*op) {
                    *changed = true;
                    return bin(i, (**l).clone(), (**r).clone(), sp);
                }
                if matches!(op, BinOp::LAnd | BinOp::LOr) {
                    *changed = true;
                    return bin(
                        if *op == BinOp::LAnd {
                            BinOp::LOr
                        } else {
                            BinOp::LAnd
                        },
                        Expr {
                            span: sp,
                            kind: ExprKind::Un {
                                op: UnOp::Not,
                                e: l.clone(),
                            },
                        },
                        Expr {
                            span: sp,
                            kind: ExprKind::Un {
                                op: UnOp::Not,
                                e: r.clone(),
                            },
                        },
                        sp,
                    );
                }
            }
        }
        UnOp::BitNot => {
            if let ExprKind::Un {
                op: UnOp::BitNot,
                e: x,
            } = &inner.kind
            {
                *changed = true;
                return (**x).clone();
            }
            if let ExprKind::Bin { op, l, r } = &inner.kind {
                if *op == BinOp::And || *op == BinOp::Or {
                    *changed = true;
                    let bop = if *op == BinOp::And {
                        BinOp::Or
                    } else {
                        BinOp::And
                    };
                    return bin(
                        bop,
                        Expr {
                            span: sp,
                            kind: ExprKind::Un {
                                op: UnOp::BitNot,
                                e: l.clone(),
                            },
                        },
                        Expr {
                            span: sp,
                            kind: ExprKind::Un {
                                op: UnOp::BitNot,
                                e: r.clone(),
                            },
                        },
                        sp,
                    );
                }
            }
        }
    }
    let _ = pure;
    e
}

fn mega_let(e: Expr, pure: &HashSet<String>, changed: &mut bool) -> Expr {
    let sp = e.span;
    let ExprKind::Let { name, value, body } = &e.kind else {
        return e;
    };
    let value = (**value).clone();
    let body = (**body).clone();
    let uses = count_var(&body, name);
    if uses == 0 && is_pure(&value, pure) {
        *changed = true;
        return body;
    }
    if matches!(&body.kind, ExprKind::Var(n) if n == name) {
        *changed = true;
        return value;
    }
    if uses == 1 {
        let b = subst(body.clone(), name, &value);
        if count_var(&b, name) == 0 {
            *changed = true;
            return b;
        }
    }
    if let ExprKind::Let {
        name: inner,
        value: iv,
        body: ib,
    } = &value.kind
    {
        if count_var(iv, name) == 0 {
            *changed = true;
            let rebound = Expr {
                span: sp,
                kind: ExprKind::Let {
                    name: name.clone(),
                    value: ib.clone(),
                    body: Box::new(body),
                },
            };
            return Expr {
                span: sp,
                kind: ExprKind::Let {
                    name: inner.clone(),
                    value: iv.clone(),
                    body: Box::new(rebound),
                },
            };
        }
    }
    e
}

fn expr_names(e: &Expr, out: &mut HashSet<String>) {
    match &e.kind {
        ExprKind::Var(n) => {
            out.insert(n.clone());
        }
        ExprKind::Let { name, value, body } => {
            out.insert(name.clone());
            expr_names(value, out);
            expr_names(body, out);
        }
        ExprKind::Fn { params, body } => {
            for n in params {
                out.insert(n.clone());
            }
            expr_names(body, out);
        }
        ExprKind::App { fun, args } => {
            expr_names(fun, out);
            for a in args {
                expr_names(a, out);
            }
        }
        ExprKind::Case { scrut, arms } => {
            expr_names(scrut, out);
            for arm in arms {
                let mut binders = Vec::new();
                pat_binders(&arm.pat, &mut binders);
                for n in binders {
                    out.insert(n);
                }
                expr_names(&arm.body, out);
            }
        }
        ExprKind::Block { stmts, tail } => {
            for s in stmts {
                expr_names(s, out);
            }
            expr_names(tail, out);
        }
        ExprKind::Bin { l, r, .. } => {
            expr_names(l, out);
            expr_names(r, out);
        }
        ExprKind::Un { e, .. } | ExprKind::Quote(e) => expr_names(e, out),
        ExprKind::Int(_) | ExprKind::Float(_) | ExprKind::Str(_) => {}
    }
}

fn mega_app(e: Expr, pure: &HashSet<String>, changed: &mut bool) -> Expr {
    let sp = e.span;
    let ExprKind::App { fun, args } = &e.kind else {
        return e;
    };
    if args.len() == 2
        && same(&args[0], &args[1])
        && is_pure(&args[0], pure)
        && !matches!(
            &args[0].kind,
            ExprKind::Var(_) | ExprKind::Int(_) | ExprKind::Float(_) | ExprKind::Str(_)
        )
    {
        let mut names = HashSet::new();
        expr_names(&e, &mut names);
        let mut serial = 0usize;
        let n = fresh_opt_name(&mut names, &mut serial, "arg");
        let v = Expr {
            span: sp,
            kind: ExprKind::Var(n.clone()),
        };
        *changed = true;
        return Expr {
            span: sp,
            kind: ExprKind::Let {
                name: n,
                value: Box::new(args[0].clone()),
                body: Box::new(Expr {
                    span: sp,
                    kind: ExprKind::App {
                        fun: fun.clone(),
                        args: vec![v.clone(), v],
                    },
                }),
            },
        };
    }
    if let ExprKind::Fn { params, body } = &fun.kind {
        if params.len() == 1
            && args.len() == 1
            && matches!(&body.kind, ExprKind::Var(n) if n == &params[0])
        {
            *changed = true;
            return args[0].clone();
        }
    }
    e
}

fn mega_case(e: Expr, pure: &HashSet<String>, changed: &mut bool) -> Expr {
    let sp = e.span;
    let ExprKind::Case { scrut, arms } = &e.kind else {
        return e;
    };
    if arms.len() > 1 {
        if arms.iter().all(|a| same(&a.body, &arms[0].body))
            && arms.iter().all(|a| {
                let mut b = Vec::new();
                pat_binders(&a.pat, &mut b);
                b.is_empty()
            })
        {
            *changed = true;
            let mut used = HashSet::new();
            expr_names(&e, &mut used);
            let mut serial = 0usize;
            return preserve_scrut(
                (**scrut).clone(),
                arms[0].body.clone(),
                pure,
                &mut used,
                &mut serial,
            );
        }
    }
    if let ExprKind::Case {
        scrut: inner_scrut,
        arms: inner_arms,
    } = &scrut.kind
    {
        if arms.len() <= 8
            && inner_arms.len() <= 8
            && !arms.iter().any(|a| {
                let mut b = Vec::new();
                pat_binders(&a.pat, &mut b);
                !b.is_empty()
            })
        {
            let outer = arms.clone();
            let nested = inner_arms
                .iter()
                .map(|a| Arm {
                    pat: a.pat.clone(),
                    body: Expr {
                        span: sp,
                        kind: ExprKind::Case {
                            scrut: Box::new(a.body.clone()),
                            arms: outer.clone(),
                        },
                    },
                    span: a.span,
                })
                .collect();
            *changed = true;
            return Expr {
                span: sp,
                kind: ExprKind::Case {
                    scrut: inner_scrut.clone(),
                    arms: nested,
                },
            };
        }
    }
    e
}

fn mega_block(e: Expr, pure: &HashSet<String>, changed: &mut bool) -> Expr {
    let sp = e.span;
    let ExprKind::Block { stmts, tail } = &e.kind else {
        return e;
    };
    let mut out = Vec::new();
    for stmt in stmts {
        if let ExprKind::Block {
            stmts: inner,
            tail: inner_tail,
        } = &stmt.kind
        {
            out.extend(inner.clone());
            out.push((**inner_tail).clone());
            *changed = true;
        } else {
            out.push(stmt.clone());
        }
    }
    let mut dedup = Vec::with_capacity(out.len());
    for x in out {
        if is_pure(&x, pure) && dedup.iter().any(|y| same(y, &x)) {
            *changed = true;
        } else {
            dedup.push(x);
        }
    }
    if dedup.is_empty() {
        *changed = true;
        return (**tail).clone();
    }
    if dedup.len() == 1 && same(&dedup[0], tail) && is_pure(&dedup[0], pure) {
        *changed = true;
        return (**tail).clone();
    }
    Expr {
        span: sp,
        kind: ExprKind::Block {
            stmts: dedup,
            tail: tail.clone(),
        },
    }
}

fn mega_cse(e: Expr, pure: &HashSet<String>, changed: &mut bool) -> Expr {
    let sp = e.span;
    match &e.kind {
        ExprKind::Bin { op, l, r }
            if same(l, r)
                && is_pure(l, pure)
                && !matches!(
                    &l.kind,
                    ExprKind::Var(_) | ExprKind::Int(_) | ExprKind::Float(_) | ExprKind::Str(_)
                )
                && !matches!(op, BinOp::LAnd | BinOp::LOr) =>
        {
            *changed = true;
            let mut used = HashSet::new();
            expr_names(&e, &mut used);
            let mut serial = 0usize;
            let name = fresh_opt_name(&mut used, &mut serial, "v");
            let v = Expr {
                span: sp,
                kind: ExprKind::Var(name.clone()),
            };
            Expr {
                span: sp,
                kind: ExprKind::Let {
                    name,
                    value: l.clone(),
                    body: Box::new(bin(*op, v.clone(), v, sp)),
                },
            }
        }
        _ => e,
    }
}

fn mega_expr(e: Expr, pure: &HashSet<String>, changed: &mut bool) -> Expr {
    let e = map_child(e, &mut |x| mega_expr(x, pure, changed));
    let mut e = simp_expr(e, pure, changed);
    for _ in 0..LOCAL_LAYERS {
        let mut local = false;
        let n = mega_arith(e, pure, &mut local);
        let n = mega_bits(n, pure, &mut local);
        let n = mega_logic(n, pure, &mut local);
        let n = mega_cmp(n, pure, &mut local);
        let n = mega_un(n, pure, &mut local);
        let n = mega_let(n, pure, &mut local);
        let n = mega_app(n, pure, &mut local);
        let n = mega_case(n, pure, &mut local);
        let n = mega_block(n, pure, &mut local);
        let n = mega_cse(n, pure, &mut local);
        e = n;
        if !local {
            break;
        }
        *changed = true;
    }
    e
}

fn mega_opt(p: Program, pure: &HashSet<String>, changed: &mut bool) -> Program {
    map_prog(p, &mut |e| mega_expr(e, pure, changed))
}

fn sink(p: Program, pure: &HashSet<String>, changed: &mut bool) -> Program {
    fn wrap(e: Expr, x: &str, v: &Expr) -> Expr {
        let sp = e.span;
        Expr {
            span: sp,
            kind: ExprKind::Let {
                name: x.to_string(),
                value: Box::new(v.clone()),
                body: Box::new(e),
            },
        }
    }

    fn sink_into(e: Expr, x: &str, v: &Expr, pure: &HashSet<String>) -> (Expr, bool) {
        if !is_pure(v, pure) {
            return (e, false);
        }
        if count_var(&e, x) == 0 {
            return (e, false);
        }

        let sp = e.span;
        match e.kind {
            ExprKind::Int(_)
            | ExprKind::Float(_)
            | ExprKind::Str(_)
            | ExprKind::Var(_)
            | ExprKind::Quote(_) => (e, false),

            ExprKind::Let { name, value, body } => {
                let vu = count_var(&value, x);
                let bu = count_var(&body, x);
                if vu > 0 && bu > 0 {
                    (
                        Expr {
                            span: sp,
                            kind: ExprKind::Let { name, value, body },
                        },
                        false,
                    )
                } else if vu > 0 {
                    let (nv, sunk) = sink_into(*value, x, v, pure);
                    let nv = if sunk { nv } else { wrap(nv, x, v) };
                    (
                        Expr {
                            span: sp,
                            kind: ExprKind::Let {
                                name,
                                value: Box::new(nv),
                                body,
                            },
                        },
                        true,
                    )
                } else {
                    let (nb, sunk) = sink_into(*body, x, v, pure);
                    let nb = if sunk { nb } else { wrap(nb, x, v) };
                    (
                        Expr {
                            span: sp,
                            kind: ExprKind::Let {
                                name,
                                value,
                                body: Box::new(nb),
                            },
                        },
                        true,
                    )
                }
            }

            ExprKind::Fn { params, body } => {
                let (nb, sunk) = sink_into(*body, x, v, pure);
                let nb = if sunk { nb } else { wrap(nb, x, v) };
                (
                    Expr {
                        span: sp,
                        kind: ExprKind::Fn {
                            params,
                            body: Box::new(nb),
                        },
                    },
                    true,
                )
            }

            ExprKind::App { fun, args } => {
                let fu = count_var(&fun, x);
                let arg_u: Vec<usize> = args.iter().map(|a| count_var(a, x)).collect();
                let nonzero: Vec<usize> = arg_u
                    .iter()
                    .copied()
                    .enumerate()
                    .filter(|(_, c)| *c > 0)
                    .map(|(i, _)| i)
                    .collect();

                if fu > 0 && !nonzero.is_empty() {
                    (
                        Expr {
                            span: sp,
                            kind: ExprKind::App { fun, args },
                        },
                        false,
                    )
                } else if fu > 0 {
                    let (nf, sunk) = sink_into(*fun, x, v, pure);
                    let nf = if sunk { nf } else { wrap(nf, x, v) };
                    (
                        Expr {
                            span: sp,
                            kind: ExprKind::App {
                                fun: Box::new(nf),
                                args,
                            },
                        },
                        true,
                    )
                } else if nonzero.len() == 1 {
                    let i = nonzero[0];
                    let mut new_args = args;
                    let old = std::mem::replace(
                        &mut new_args[i],
                        Expr {
                            span: sp,
                            kind: ExprKind::Int(0),
                        },
                    );
                    let (na, sunk) = sink_into(old, x, v, pure);
                    let na = if sunk { na } else { wrap(na, x, v) };
                    new_args[i] = na;
                    (
                        Expr {
                            span: sp,
                            kind: ExprKind::App {
                                fun,
                                args: new_args,
                            },
                        },
                        true,
                    )
                } else {
                    (
                        Expr {
                            span: sp,
                            kind: ExprKind::App { fun, args },
                        },
                        false,
                    )
                }
            }

            ExprKind::Case { scrut, arms } => {
                let su = count_var(&scrut, x);
                let arm_u: Vec<usize> = arms.iter().map(|a| count_var(&a.body, x)).collect();
                let nonzero: Vec<usize> = arm_u
                    .iter()
                    .copied()
                    .enumerate()
                    .filter(|(_, c)| *c > 0)
                    .map(|(i, _)| i)
                    .collect();

                if su > 0 && !nonzero.is_empty() {
                    (
                        Expr {
                            span: sp,
                            kind: ExprKind::Case { scrut, arms },
                        },
                        false,
                    )
                } else if su > 0 {
                    let (ns, sunk) = sink_into(*scrut, x, v, pure);
                    let ns = if sunk { ns } else { wrap(ns, x, v) };
                    (
                        Expr {
                            span: sp,
                            kind: ExprKind::Case {
                                scrut: Box::new(ns),
                                arms,
                            },
                        },
                        true,
                    )
                } else if nonzero.len() == 1 {
                    let i = nonzero[0];
                    let mut new_arms = arms;
                    let old = std::mem::replace(
                        &mut new_arms[i].body,
                        Expr {
                            span: sp,
                            kind: ExprKind::Int(0),
                        },
                    );
                    let (nb, sunk) = sink_into(old, x, v, pure);
                    let nb = if sunk { nb } else { wrap(nb, x, v) };
                    new_arms[i].body = nb;
                    (
                        Expr {
                            span: sp,
                            kind: ExprKind::Case {
                                scrut,
                                arms: new_arms,
                            },
                        },
                        true,
                    )
                } else {
                    (
                        Expr {
                            span: sp,
                            kind: ExprKind::Case { scrut, arms },
                        },
                        false,
                    )
                }
            }

            ExprKind::Block { stmts, tail } => {
                let tu = count_var(&tail, x);
                let stmt_u: Vec<usize> = stmts.iter().map(|s| count_var(s, x)).collect();
                let nonzero: Vec<usize> = stmt_u
                    .iter()
                    .copied()
                    .enumerate()
                    .filter(|(_, c)| *c > 0)
                    .map(|(i, _)| i)
                    .collect();

                if tu > 0 && !nonzero.is_empty() {
                    (
                        Expr {
                            span: sp,
                            kind: ExprKind::Block { stmts, tail },
                        },
                        false,
                    )
                } else if tu > 0 {
                    let (nt, sunk) = sink_into(*tail, x, v, pure);
                    let nt = if sunk { nt } else { wrap(nt, x, v) };
                    (
                        Expr {
                            span: sp,
                            kind: ExprKind::Block {
                                stmts,
                                tail: Box::new(nt),
                            },
                        },
                        true,
                    )
                } else if nonzero.len() == 1 {
                    let i = nonzero[0];
                    let mut new_stmts = stmts;
                    let old = std::mem::replace(
                        &mut new_stmts[i],
                        Expr {
                            span: sp,
                            kind: ExprKind::Int(0),
                        },
                    );
                    let (ns, sunk) = sink_into(old, x, v, pure);
                    let ns = if sunk { ns } else { wrap(ns, x, v) };
                    new_stmts[i] = ns;
                    (
                        Expr {
                            span: sp,
                            kind: ExprKind::Block {
                                stmts: new_stmts,
                                tail,
                            },
                        },
                        true,
                    )
                } else {
                    (
                        Expr {
                            span: sp,
                            kind: ExprKind::Block { stmts, tail },
                        },
                        false,
                    )
                }
            }

            ExprKind::Bin { op, l, r } => {
                let lu = count_var(&l, x);
                let ru = count_var(&r, x);
                if lu > 0 && ru > 0 {
                    (
                        Expr {
                            span: sp,
                            kind: ExprKind::Bin { op, l, r },
                        },
                        false,
                    )
                } else if lu > 0 {
                    let (nl, sunk) = sink_into(*l, x, v, pure);
                    let nl = if sunk { nl } else { wrap(nl, x, v) };
                    (
                        Expr {
                            span: sp,
                            kind: ExprKind::Bin {
                                op,
                                l: Box::new(nl),
                                r,
                            },
                        },
                        true,
                    )
                } else {
                    let (nr, sunk) = sink_into(*r, x, v, pure);
                    let nr = if sunk { nr } else { wrap(nr, x, v) };
                    (
                        Expr {
                            span: sp,
                            kind: ExprKind::Bin {
                                op,
                                l,
                                r: Box::new(nr),
                            },
                        },
                        true,
                    )
                }
            }

            ExprKind::Un { op, e: inner } => {
                let (ni, sunk) = sink_into(*inner, x, v, pure);
                let ni = if sunk { ni } else { wrap(ni, x, v) };
                (
                    Expr {
                        span: sp,
                        kind: ExprKind::Un {
                            op,
                            e: Box::new(ni),
                        },
                    },
                    true,
                )
            }
        }
    }

    fn go(e: Expr, pure: &HashSet<String>, changed: &mut bool) -> Expr {
        let e = map_child(e, &mut |c| go(c, pure, changed));

        match e.kind {
            ExprKind::Let { name, value, body } => {
                let uses = count_var(&body, &name);

                if uses == 0 {
                    if is_pure(&value, pure) {
                        *changed = true;
                        return *body;
                    }
                    return Expr {
                        span: e.span,
                        kind: ExprKind::Let { name, value, body },
                    };
                }

                let v_ref: &Expr = &value;
                let (new_body, sunk) = sink_into(*body, &name, v_ref, pure);
                if sunk {
                    *changed = true;
                    return new_body;
                }
                Expr {
                    span: e.span,
                    kind: ExprKind::Let {
                        name,
                        value,
                        body: Box::new(new_body),
                    },
                }
            }
            k => Expr {
                span: e.span,
                kind: k,
            },
        }
    }

    map_prog(p, &mut |e| go(e, pure, changed))
}

fn drop_params(p: Program, pure: &HashSet<String>, changed: &mut bool) -> Program {
    let mut drop_map: HashMap<String, Vec<usize>> = HashMap::new();
    for d in &p.decls {
        if let Decl::Let { name, value, .. } = d {
            if let ExprKind::Fn { params, body } = &value.kind {
                let unused: Vec<usize> = params
                    .iter()
                    .enumerate()
                    .filter(|(_, p)| count_var(body, p) == 0)
                    .map(|(i, _)| i)
                    .collect();

                if !unused.is_empty() && unused.len() < params.len() {
                    drop_map.insert(name.clone(), unused);
                }
            }
        }
    }
    if drop_map.is_empty() {
        return p;
    }

    let mut disqualify: HashSet<String> = HashSet::new();
    let body_prog = p.body.clone();
    let all_exprs: Vec<Expr> = p
        .decls
        .iter()
        .filter_map(|d| match d {
            Decl::Let { value, .. } => Some((**value).clone()),
            _ => None,
        })
        .chain(std::iter::once(*body_prog))
        .collect();

    for (fname, indices) in &drop_map {
        if !all_calls(&all_exprs, fname, params_len(&p, fname)) {
            disqualify.insert(fname.clone());
            continue;
        }
        for e in &all_exprs {
            if !pure_args(e, fname, indices, pure) {
                disqualify.insert(fname.clone());
                break;
            }
        }
    }
    for f in &disqualify {
        drop_map.remove(f);
    }
    if drop_map.is_empty() {
        return p;
    }

    fn rw_fn(e: Expr, drop_map: &HashMap<String, Vec<usize>>) -> Expr {
        let e = map_child(e, &mut |x| rw_fn(x, drop_map));
        if let ExprKind::App { fun, args } = e.kind {
            if let ExprKind::Var(ref n) = fun.kind {
                if let Some(indices) = drop_map.get(n) {
                    let new_args: Vec<Expr> = args
                        .into_iter()
                        .enumerate()
                        .filter(|(i, _)| !indices.contains(i))
                        .map(|(_, a)| a)
                        .collect();
                    return Expr {
                        span: e.span,
                        kind: ExprKind::App {
                            fun,
                            args: new_args,
                        },
                    };
                }
            }
            return Expr {
                span: e.span,
                kind: ExprKind::App { fun, args },
            };
        }
        e
    }

    let decls: Vec<Decl> = p
        .decls
        .into_iter()
        .map(|d| match d {
            Decl::Let { name, value, span } => {
                let v = if let Some(indices) = drop_map.get(&name) {
                    if let ExprKind::Fn { params, body } = value.kind {
                        let new_params: Vec<String> = params
                            .into_iter()
                            .enumerate()
                            .filter(|(i, _)| !indices.contains(i))
                            .map(|(_, p)| p)
                            .collect();
                        Box::new(Expr {
                            span,
                            kind: ExprKind::Fn {
                                params: new_params,
                                body,
                            },
                        })
                    } else {
                        value
                    }
                } else {
                    value
                };
                Decl::Let {
                    name,
                    value: Box::new(rw_fn(*v, &drop_map)),
                    span,
                }
            }
            other => other,
        })
        .collect();

    let body = rw_fn(*p.body, &drop_map);
    *changed = true;
    Program {
        decls,
        body: Box::new(body),
    }
}

fn params_len(p: &Program, fname: &str) -> Option<usize> {
    p.decls.iter().find_map(|d| match d {
        Decl::Let { name, value, .. } if name == fname => match &value.kind {
            ExprKind::Fn { params, .. } => Some(params.len()),
            _ => None,
        },
        _ => None,
    })
}

fn all_calls(exprs: &[Expr], fname: &str, arity: Option<usize>) -> bool {
    fn check(e: &Expr, fname: &str, arity: Option<usize>, ok: &mut bool) {
        if !*ok {
            return;
        }
        match &e.kind {
            ExprKind::Var(n) if n == fname => {
                *ok = false;
            }
            ExprKind::App { fun, args } => {
                let direct = matches!(&fun.kind, ExprKind::Var(n) if n == fname);
                if direct {
                    if arity != Some(args.len()) {
                        *ok = false;
                        return;
                    }
                } else {
                    check(fun, fname, arity, ok);
                }
                for a in args {
                    check(a, fname, arity, ok);
                }
            }
            ExprKind::Let { value, body, .. } => {
                check(value, fname, arity, ok);
                check(body, fname, arity, ok);
            }
            ExprKind::Fn { body, .. } => check(body, fname, arity, ok),
            ExprKind::Case { scrut, arms } => {
                check(scrut, fname, arity, ok);
                for a in arms {
                    check(&a.body, fname, arity, ok);
                }
            }
            ExprKind::Block { stmts, tail } => {
                for s in stmts {
                    check(s, fname, arity, ok);
                }
                check(tail, fname, arity, ok);
            }
            ExprKind::Bin { l, r, .. } => {
                check(l, fname, arity, ok);
                check(r, fname, arity, ok);
            }
            ExprKind::Un { e, .. } => check(e, fname, arity, ok),
            ExprKind::Quote(_) => {}
            ExprKind::Var(_) => {}
            ExprKind::Int(_) | ExprKind::Float(_) | ExprKind::Str(_) => {}
        }
    }
    let Some(_) = arity else {
        return false;
    };
    let mut ok = true;
    for e in exprs {
        check(e, fname, arity, &mut ok);
        if !ok {
            return false;
        }
    }
    true
}

fn pure_args(e: &Expr, fname: &str, indices: &[usize], pure: &HashSet<String>) -> bool {
    fn check(e: &Expr, fname: &str, indices: &[usize], pure: &HashSet<String>, ok: &mut bool) {
        if !*ok {
            return;
        }
        if let ExprKind::App { fun, args } = &e.kind {
            if let ExprKind::Var(n) = &fun.kind {
                if n == fname {
                    for i in indices.iter().copied() {
                        if let Some(a) = args.get(i) {
                            if !is_pure(a, pure) {
                                *ok = false;
                                return;
                            }
                        }
                    }
                }
            }
        }
        match &e.kind {
            ExprKind::App { fun, args } => {
                check(fun, fname, indices, pure, ok);
                for a in args {
                    check(a, fname, indices, pure, ok);
                }
            }
            ExprKind::Let { value, body, .. } => {
                check(value, fname, indices, pure, ok);
                check(body, fname, indices, pure, ok);
            }
            ExprKind::Fn { body, .. } => check(body, fname, indices, pure, ok),
            ExprKind::Case { scrut, arms } => {
                check(scrut, fname, indices, pure, ok);
                for a in arms {
                    check(&a.body, fname, indices, pure, ok);
                }
            }
            ExprKind::Block { stmts, tail } => {
                for s in stmts {
                    check(s, fname, indices, pure, ok);
                }
                check(tail, fname, indices, pure, ok);
            }
            ExprKind::Bin { l, r, .. } => {
                check(l, fname, indices, pure, ok);
                check(r, fname, indices, pure, ok);
            }
            ExprKind::Un { e, .. } => check(e, fname, indices, pure, ok),
            _ => {}
        }
    }
    let mut ok = true;
    check(e, fname, indices, pure, &mut ok);
    ok
}

#[derive(Clone, Copy)]
enum AbsVal {
    Const(u64),
    Unknown,
}

struct ConstEnv {
    vars: Vec<(String, AbsVal)>,
}

impl ConstEnv {
    fn new() -> Self {
        Self { vars: Vec::new() }
    }
    fn push(&mut self, name: String, value: AbsVal) {
        self.vars.push((name, value));
    }
    fn get(&self, name: &str) -> AbsVal {
        self.vars
            .iter()
            .rev()
            .find(|(n, _)| n == name)
            .map(|(_, v)| *v)
            .unwrap_or(AbsVal::Unknown)
    }
}

fn eval_abs(e: &Expr, env: &ConstEnv) -> AbsVal {
    match &e.kind {
        ExprKind::Int(n) => AbsVal::Const(*n),
        ExprKind::Var(n) => env.get(n),
        ExprKind::Bin { op, l, r } => match (eval_abs(l, env), eval_abs(r, env)) {
            (AbsVal::Const(a), AbsVal::Const(b)) => eval_bin(*op, a, b)
                .map(AbsVal::Const)
                .unwrap_or(AbsVal::Unknown),
            _ => AbsVal::Unknown,
        },
        ExprKind::Un { op, e } => match eval_abs(e, env) {
            AbsVal::Const(a) => eval_un(*op, a)
                .map(AbsVal::Const)
                .unwrap_or(AbsVal::Unknown),
            _ => AbsVal::Unknown,
        },
        _ => AbsVal::Unknown,
    }
}

fn const_prop(e: Expr, env: &mut ConstEnv, pure: &HashSet<String>, changed: &mut bool) -> Expr {
    let sp = e.span;
    match e.kind {
        ExprKind::Var(n) => match env.get(&n) {
            AbsVal::Const(v) => {
                *changed = true;
                int(v)
            }
            AbsVal::Unknown => Expr {
                span: sp,
                kind: ExprKind::Var(n),
            },
        },
        ExprKind::Let { name, value, body } => {
            let value = const_prop(*value, env, pure, changed);
            let av = eval_abs(&value, env);
            env.push(name.clone(), av);
            let body = const_prop(*body, env, pure, changed);
            env.vars.pop();
            Expr {
                span: sp,
                kind: ExprKind::Let {
                    name,
                    value: Box::new(value),
                    body: Box::new(body),
                },
            }
        }
        ExprKind::Fn { params, body } => {
            let old = env.vars.len();
            for p in &params {
                env.push(p.clone(), AbsVal::Unknown);
            }
            let body = const_prop(*body, env, pure, changed);
            env.vars.truncate(old);
            Expr {
                span: sp,
                kind: ExprKind::Fn {
                    params,
                    body: Box::new(body),
                },
            }
        }
        ExprKind::App { fun, args } => Expr {
            span: sp,
            kind: ExprKind::App {
                fun: Box::new(const_prop(*fun, env, pure, changed)),
                args: args
                    .into_iter()
                    .map(|a| const_prop(a, env, pure, changed))
                    .collect(),
            },
        },
        ExprKind::Case { scrut, arms } => {
            let scrut = const_prop(*scrut, env, pure, changed);
            let arms = arms
                .into_iter()
                .map(|a| Arm {
                    pat: a.pat,
                    body: const_prop(a.body, env, pure, changed),
                    span: a.span,
                })
                .collect();
            Expr {
                span: sp,
                kind: ExprKind::Case {
                    scrut: Box::new(scrut),
                    arms,
                },
            }
        }
        ExprKind::Block { stmts, tail } => Expr {
            span: sp,
            kind: ExprKind::Block {
                stmts: stmts
                    .into_iter()
                    .map(|s| const_prop(s, env, pure, changed))
                    .collect(),
                tail: Box::new(const_prop(*tail, env, pure, changed)),
            },
        },
        ExprKind::Bin { op, l, r } => simp_bin(
            Expr {
                span: sp,
                kind: ExprKind::Bin {
                    op,
                    l: Box::new(const_prop(*l, env, pure, changed)),
                    r: Box::new(const_prop(*r, env, pure, changed)),
                },
            },
            pure,
            changed,
        ),
        ExprKind::Un { op, e } => simp_un(
            Expr {
                span: sp,
                kind: ExprKind::Un {
                    op,
                    e: Box::new(const_prop(*e, env, pure, changed)),
                },
            },
            pure,
            changed,
        ),
        ExprKind::Quote(x) => Expr {
            span: sp,
            kind: ExprKind::Quote(x),
        },
        k => Expr { span: sp, kind: k },
    }
}

fn const_opt(p: Program, pure: &HashSet<String>, changed: &mut bool) -> Program {
    let mut env = ConstEnv::new();
    let mut decls = Vec::with_capacity(p.decls.len());
    for d in p.decls {
        match d {
            Decl::Let { name, value, span } => {
                let value = const_prop(*value, &mut env, pure, changed);
                let av = eval_abs(&value, &env);
                env.push(name.clone(), av);
                decls.push(Decl::Let {
                    name,
                    value: Box::new(value),
                    span,
                });
            }
            other => decls.push(other),
        }
    }
    let mut out = Program {
        decls,
        body: p.body,
    };

    let mut counts = HashMap::<String, usize>::new();
    for d in &out.decls {
        if let Decl::Let { value, .. } = d {
            use_counts(value, &mut counts);
        }
    }
    use_counts(&out.body, &mut counts);

    out.decls = out
        .decls
        .into_iter()
        .filter(|d| match d {
            Decl::Let { name, value, .. } => {
                let unused = *counts.get(name).unwrap_or(&0) == 0;
                let recursive = count_var(value, name) != 0;
                if unused && !recursive && is_pure(value, pure) {
                    *changed = true;
                    false
                } else {
                    true
                }
            }
            _ => true,
        })
        .collect();

    loop {
        let mut counts = HashMap::<String, usize>::new();
        for d in &out.decls {
            if let Decl::Let { value, .. } = d {
                use_counts(value, &mut counts);
            }
        }
        use_counts(&out.body, &mut counts);

        let candidate = out.decls.iter().find_map(|d| {
            if let Decl::Let { name, value, .. } = d {
                if *counts.get(name).unwrap_or(&0) == 1
                    && count_var(value, name) == 0
                    && is_pure(value, pure)
                {
                    Some((name.clone(), (**value).clone()))
                } else {
                    None
                }
            } else {
                None
            }
        });

        let Some((name, value)) = candidate else {
            break;
        };
        let subs = vec![(name.clone(), value)];
        let mut used_after = 0usize;
        let mut next_decls = Vec::with_capacity(out.decls.len());

        for d in out.decls.into_iter() {
            match d {
                Decl::Let {
                    name: dn,
                    value,
                    span,
                } if dn == name => {
                    *changed = true;
                    let _ = (value, span);
                }
                Decl::Let {
                    name: dn,
                    value,
                    span,
                } => {
                    let value = apply_subs(*value, &subs);
                    used_after += count_var(&value, &name);
                    next_decls.push(Decl::Let {
                        name: dn,
                        value: Box::new(value),
                        span,
                    });
                }
                other => next_decls.push(other),
            }
        }

        let body = apply_subs(*out.body, &subs);
        used_after += count_var(&body, &name);
        out.decls = next_decls;
        out.body = Box::new(body);
        if used_after != 0 {
            break;
        }
    }
    out
}

fn global_const_values(p: &Program, pure: &HashSet<String>) -> HashMap<String, u64> {
    let mut known = HashMap::<String, u64>::new();
    let limit = p.decls.len().saturating_mul(2).saturating_add(2);
    for _ in 0..limit {
        let mut changed = false;
        let mut env = ConstEnv::new();
        for (name, value) in &known {
            env.push(name.clone(), AbsVal::Const(*value));
        }
        for d in &p.decls {
            if let Decl::Let { name, value, .. } = d {
                let av = eval_abs(value, &env);
                if let AbsVal::Const(v) = av {
                    if known.insert(name.clone(), v) != Some(v) {
                        changed = true;
                    }
                    env.push(name.clone(), AbsVal::Const(v));
                }
            }
        }
        if !changed {
            break;
        }
    }
    let _ = pure;
    known
}

fn global_const_opt(mut p: Program, pure: &HashSet<String>, changed: &mut bool) -> Program {
    let known = global_const_values(&p, pure);
    if known.is_empty() {
        return p;
    }
    let mut env = ConstEnv::new();
    for (name, value) in &known {
        env.push(name.clone(), AbsVal::Const(*value));
    }
    let mut decls = Vec::with_capacity(p.decls.len());
    for d in p.decls {
        match d {
            Decl::Let { name, value, span } => {
                let value = const_prop(*value, &mut env, pure, changed);
                let av = eval_abs(&value, &env);
                let keep = if let AbsVal::Const(v) = av {
                    env.push(name.clone(), AbsVal::Const(v));
                    true
                } else {
                    env.push(name.clone(), AbsVal::Unknown);
                    true
                };
                if keep {
                    decls.push(Decl::Let {
                        name,
                        value: Box::new(value),
                        span,
                    });
                }
            }
            other => decls.push(other),
        }
    }
    p.decls = decls;
    p.body = Box::new(const_prop(*p.body, &mut env, pure, changed));
    p
}

fn dce_changed(p: Program, pure: &HashSet<String>, changed: &mut bool) -> Program {
    let before = p.decls.len();
    let out = dce(p, pure);
    if out.decls.len() != before {
        *changed = true;
    }
    out
}

fn dce(mut p: Program, pure: &HashSet<String>) -> Program {
    let mut reachable: HashSet<String> = HashSet::new();

    walk(&p.body, &mut |e| {
        if let ExprKind::Var(v) = &e.kind {
            reachable.insert(v.clone());
        }
    });

    for d in &p.decls {
        if let Decl::Let { name, value, .. } = d {
            if !is_pure(value, pure) {
                reachable.insert(name.clone());
            }
        }
    }

    loop {
        let mut added = false;
        for d in &p.decls {
            if let Decl::Let { name, value, .. } = d {
                if reachable.contains(name) {
                    walk(value, &mut |e| {
                        if let ExprKind::Var(v) = &e.kind {
                            if reachable.insert(v.clone()) {
                                added = true;
                            }
                        }
                    });
                }
            }
        }
        if !added {
            break;
        }
    }

    p.decls.retain(|d| match d {
        Decl::Let { name, .. } => reachable.contains(name),
        Decl::External(e) => reachable.contains(&e.name),
        Decl::Bring(_) => true,
        Decl::Type { .. } => true,
    });
    p
}

fn bin(op: BinOp, l: Expr, r: Expr, span: Span) -> Expr {
    Expr {
        span,
        kind: ExprKind::Bin {
            op,
            l: Box::new(l),
            r: Box::new(r),
        },
    }
}

fn int(n: u64) -> Expr {
    Expr {
        span: NULL_SPAN,
        kind: ExprKind::Int(n),
    }
}

fn as_int(e: &Expr) -> Option<u64> {
    match &e.kind {
        ExprKind::Int(n) => Some(*n),
        _ => None,
    }
}

fn is_bool(e: &Expr) -> bool {
    matches!(
        &e.kind,
        ExprKind::Un { op: UnOp::Not, .. }
            | ExprKind::Bin {
                op: BinOp::Eq
                    | BinOp::Ne
                    | BinOp::Lt
                    | BinOp::Gt
                    | BinOp::Le
                    | BinOp::Ge
                    | BinOp::LAnd
                    | BinOp::LOr,
                ..
            }
    )
}

fn is_int_expr(e: &Expr) -> bool {
    match &e.kind {
        ExprKind::Int(_) => true,
        ExprKind::Un {
            op: UnOp::Not | UnOp::BitNot,
            ..
        } => true,
        ExprKind::Bin {
            op:
                BinOp::And
                | BinOp::Or
                | BinOp::Xor
                | BinOp::Shl
                | BinOp::Shr
                | BinOp::Rem
                | BinOp::LAnd
                | BinOp::LOr,
            ..
        } => true,
        _ => false,
    }
}

fn eval_bin(op: BinOp, a: u64, b: u64) -> Option<u64> {
    use BinOp::*;
    Some(match op {
        Add => a.wrapping_add(b),
        Sub => a.wrapping_sub(b),
        Mul => a.wrapping_mul(b),
        Div => {
            if b == 0 {
                return None;
            } else {
                a / b
            }
        }
        Rem => {
            if b == 0 {
                return None;
            } else {
                a % b
            }
        }
        And => a & b,
        Or => a | b,
        Xor => a ^ b,
        Shl => a << (b & 63),
        Shr => a >> (b & 63),
        Eq => (a == b) as u64,
        Ne => (a != b) as u64,
        Lt => (a < b) as u64,
        Gt => (a > b) as u64,
        Le => (a <= b) as u64,
        Ge => (a >= b) as u64,
        LAnd => ((a != 0) && (b != 0)) as u64,
        LOr => ((a != 0) || (b != 0)) as u64,
    })
}

fn eval_un(op: UnOp, n: u64) -> Option<u64> {
    Some(match op {
        UnOp::Neg => n.wrapping_neg(),
        UnOp::Not => (n == 0) as u64,
        UnOp::BitNot => !n,
    })
}

fn size(e: &Expr) -> usize {
    match &e.kind {
        ExprKind::Int(n) => {
            if *n < 10 {
                1
            } else if *n < 100 {
                2
            } else if *n < 1000 {
                3
            } else if *n < 1_000_000_000 {
                4
            } else {
                5
            }
        }
        ExprKind::Float(_) => 4,
        ExprKind::Str(b) => 2 + b.len(),
        ExprKind::Var(_) => 1,
        ExprKind::Let { value, body, .. } => 6 + size(value) + size(body),
        ExprKind::Fn { params, body } => 4 + params.len() + size(body),
        ExprKind::App { fun, args } => 3 + size(fun) + args.iter().map(size).sum::<usize>(),
        ExprKind::Case { scrut, arms } => {
            3 + size(scrut) + arms.iter().map(|a| 4 + size(&a.body)).sum::<usize>()
        }
        ExprKind::Block { stmts, tail } => 2 + stmts.iter().map(size).sum::<usize>() + size(tail),
        ExprKind::Bin { l, r, .. } => 4 + size(l) + size(r),
        ExprKind::Un { e, .. } => 3 + size(e),
        ExprKind::Quote(e) => 2 + size(e),
    }
}

fn distinct(xs: &[String]) -> bool {
    let mut s = HashSet::new();
    xs.iter().all(|x| s.insert(x))
}

fn same(a: &Expr, b: &Expr) -> bool {
    match (&a.kind, &b.kind) {
        (ExprKind::Int(x), ExprKind::Int(y)) => x == y,
        (ExprKind::Float(x), ExprKind::Float(y)) => x.to_bits() == y.to_bits(),
        (ExprKind::Str(x), ExprKind::Str(y)) => x == y,
        (ExprKind::Var(x), ExprKind::Var(y)) => x == y,
        (
            ExprKind::Let {
                name: an,
                value: av,
                body: ab,
            },
            ExprKind::Let {
                name: bn,
                value: bv,
                body: bb,
            },
        ) => an == bn && same(av, bv) && same(ab, bb),
        (
            ExprKind::Fn {
                params: ap,
                body: ab,
            },
            ExprKind::Fn {
                params: bp,
                body: bb,
            },
        ) => ap == bp && same(ab, bb),
        (ExprKind::App { fun: af, args: aa }, ExprKind::App { fun: bf, args: ba }) => {
            same(af, bf) && aa.len() == ba.len() && aa.iter().zip(ba).all(|(x, y)| same(x, y))
        }
        (
            ExprKind::Case {
                scrut: as_,
                arms: aa,
            },
            ExprKind::Case {
                scrut: bs,
                arms: ba,
            },
        ) => {
            same(as_, bs)
                && aa.len() == ba.len()
                && aa
                    .iter()
                    .zip(ba)
                    .all(|(x, y)| same_pat(&x.pat, &y.pat) && same(&x.body, &y.body))
        }
        (
            ExprKind::Block {
                stmts: as_,
                tail: at,
            },
            ExprKind::Block {
                stmts: bs,
                tail: bt,
            },
        ) => as_.len() == bs.len() && as_.iter().zip(bs).all(|(x, y)| same(x, y)) && same(at, bt),
        (
            ExprKind::Bin {
                op: ao,
                l: al,
                r: ar,
            },
            ExprKind::Bin {
                op: bo,
                l: bl,
                r: br,
            },
        ) => ao == bo && same(al, bl) && same(ar, br),
        (ExprKind::Un { op: ao, e: ae }, ExprKind::Un { op: bo, e: be }) => {
            ao == bo && same(ae, be)
        }
        (ExprKind::Quote(a), ExprKind::Quote(b)) => same(a, b),
        _ => false,
    }
}

fn same_pat(a: &Pat, b: &Pat) -> bool {
    match (a, b) {
        (Pat::Wild, Pat::Wild) => true,
        (Pat::Bind(a), Pat::Bind(b)) => a == b,
        (Pat::Int(a), Pat::Int(b)) => a == b,
        (Pat::Str(a), Pat::Str(b)) => a == b,
        (Pat::Ctor { name: an, args: aa }, Pat::Ctor { name: bn, args: ba }) => {
            an == bn && aa.len() == ba.len() && aa.iter().zip(ba).all(|(x, y)| same_pat(x, y))
        }
        (Pat::Tuple(aa), Pat::Tuple(ba)) => {
            aa.len() == ba.len() && aa.iter().zip(ba).all(|(x, y)| same_pat(x, y))
        }
        _ => false,
    }
}

fn count_var(e: &Expr, name: &str) -> usize {
    let mut n = 0;
    walk(e, &mut |x| {
        if let ExprKind::Var(v) = &x.kind {
            if v == name {
                n += 1;
            }
        }
    });
    n
}

fn use_counts(e: &Expr, out: &mut HashMap<String, usize>) {
    walk(e, &mut |x| {
        if let ExprKind::Var(v) = &x.kind {
            *out.entry(v.clone()).or_default() += 1;
        }
    });
}

fn walk(e: &Expr, f: &mut dyn FnMut(&Expr)) {
    f(e);
    match &e.kind {
        ExprKind::Int(_)
        | ExprKind::Float(_)
        | ExprKind::Str(_)
        | ExprKind::Var(_)
        | ExprKind::Quote(_) => {}
        ExprKind::Let { value, body, .. } => {
            walk(value, f);
            walk(body, f);
        }
        ExprKind::Fn { body, .. } => walk(body, f),
        ExprKind::App { fun, args } => {
            walk(fun, f);
            for a in args {
                walk(a, f);
            }
        }
        ExprKind::Case { scrut, arms } => {
            walk(scrut, f);
            for a in arms {
                walk(&a.body, f);
            }
        }
        ExprKind::Block { stmts, tail } => {
            for s in stmts {
                walk(s, f);
            }
            walk(tail, f);
        }
        ExprKind::Bin { l, r, .. } => {
            walk(l, f);
            walk(r, f);
        }
        ExprKind::Un { e, .. } => walk(e, f),
    }
}

fn subst(e: Expr, name: &str, to: &Expr) -> Expr {
    fn go(e: Expr, name: &str, to: &Expr, shadowed: bool) -> Expr {
        let span = e.span;
        match e.kind {
            ExprKind::Var(v) => {
                if !shadowed && v == name {
                    to.clone()
                } else {
                    Expr {
                        span,
                        kind: ExprKind::Var(v),
                    }
                }
            }

            ExprKind::Let {
                name: binder,
                value,
                body,
            } => {
                let value = Box::new(go(*value, name, to, shadowed));
                let body_shadowed = shadowed || binder == name;
                let body = Box::new(go(*body, name, to, body_shadowed));
                Expr {
                    span,
                    kind: ExprKind::Let {
                        name: binder,
                        value,
                        body,
                    },
                }
            }

            ExprKind::Fn { params, body } => {
                let body_shadowed = shadowed || params.iter().any(|p| p == name);
                let body = if body_shadowed {
                    body
                } else {
                    Box::new(go(*body, name, to, false))
                };
                Expr {
                    span,
                    kind: ExprKind::Fn { params, body },
                }
            }

            ExprKind::App { fun, args } => Expr {
                span,
                kind: ExprKind::App {
                    fun: Box::new(go(*fun, name, to, shadowed)),
                    args: args
                        .into_iter()
                        .map(|a| go(a, name, to, shadowed))
                        .collect(),
                },
            },

            ExprKind::Case { scrut, arms } => {
                let scrut = Box::new(go(*scrut, name, to, shadowed));
                let arms = arms
                    .into_iter()
                    .map(|arm| {
                        let arm_shadowed = shadowed || {
                            let mut binders = Vec::new();
                            pat_binders(&arm.pat, &mut binders);
                            binders.iter().any(|b| b == name)
                        };
                        let body = if arm_shadowed {
                            arm.body
                        } else {
                            go(arm.body, name, to, false)
                        };
                        Arm {
                            pat: arm.pat,
                            body,
                            span: arm.span,
                        }
                    })
                    .collect();
                Expr {
                    span,
                    kind: ExprKind::Case { scrut, arms },
                }
            }

            ExprKind::Block { stmts, tail } => Expr {
                span,
                kind: ExprKind::Block {
                    stmts: stmts
                        .into_iter()
                        .map(|s| go(s, name, to, shadowed))
                        .collect(),
                    tail: Box::new(go(*tail, name, to, shadowed)),
                },
            },

            ExprKind::Bin { op, l, r } => Expr {
                span,
                kind: ExprKind::Bin {
                    op,
                    l: Box::new(go(*l, name, to, shadowed)),
                    r: Box::new(go(*r, name, to, shadowed)),
                },
            },

            ExprKind::Un { op, e } => Expr {
                span,
                kind: ExprKind::Un {
                    op,
                    e: Box::new(go(*e, name, to, shadowed)),
                },
            },

            ExprKind::Quote(x) => Expr {
                span,
                kind: ExprKind::Quote(x),
            },

            other => Expr { span, kind: other },
        }
    }

    go(e, name, to, false)
}

fn apply_subs(mut e: Expr, subs: &[(String, Expr)]) -> Expr {
    for _ in 0..=subs.len() {
        let mut progress = false;
        for (n, v) in subs {
            let before = count_var(&e, n);
            if before == 0 {
                continue;
            }
            let next = subst(e, n, v);
            let after = count_var(&next, n);
            progress |= after < before;
            e = next;
        }
        if !progress {
            break;
        }
    }
    e
}

fn pat_binders(p: &Pat, out: &mut Vec<String>) {
    match p {
        Pat::Bind(n) => out.push(n.clone()),
        Pat::Ctor { args, .. } => {
            for a in args {
                pat_binders(a, out);
            }
        }
        Pat::Tuple(xs) => {
            for x in xs {
                pat_binders(x, out);
            }
        }
        _ => {}
    }
}

#[cfg(test)]
mod pointer_optimization_regressions {
    use super::*;
    use crate::parser::Parser;

    fn parse_expr(source: &[u8]) -> Expr {
        *Parser::parse(source)
            .expect("regression expression should parse")
            .body
    }

    #[test]
    fn pointer_offset_neutral_elements_are_removed() {
        for source in [&b"ptr + 0"[..], &b"ptr - 0"[..], &b"0 + ptr"[..]] {
            let mut changed = false;
            let result = simp_bin(parse_expr(source), &HashSet::new(), &mut changed);
            assert!(changed, "expected pointer offset simplification for {source:?}");
            assert!(matches!(&result.kind, ExprKind::Var(name) if name == "ptr"));
        }
    }

    #[test]
    fn index_reads_are_not_misclassified_as_pure() {
        let expr = parse_expr(b"ptr[0]");
        let pure = HashSet::new();
        assert!(!pure_body(&expr, &pure));
        assert!(!wpo_pure_expr(&expr, &pure, &HashSet::new()));
    }

    #[test]
    fn scope_validator_accepts_index_intrinsic_before_and_after_alpha() {
        let program = Parser::parse(b"let values = 7; values[0]")
            .expect("program with indexing should parse");
        assert!(scoped(&program), "index intrinsic must be accepted before alpha");

        let renamed = alpha(program);
        assert!(
            scoped(&renamed),
            "alpha-renaming must preserve and validate the index intrinsic"
        );
    }

    #[test]
    fn scope_validator_still_rejects_unknown_variables() {
        let program = Parser::parse(b"missing_value[0]")
            .expect("program should parse even though its base name is undefined");
        assert!(
            !scoped(&program),
            "recognizing the index intrinsic must not permit an undefined base name"
        );
    }
}
