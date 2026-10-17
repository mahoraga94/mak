use crate::ast::*;
use std::cell::RefCell;
use std::collections::{BTreeMap, BTreeSet, HashMap, HashSet};
use std::fmt::Write;
use std::fs;
use std::io;
use std::path::{Path, PathBuf};

#[derive(Clone, Debug, PartialEq, Eq, Hash)]
enum Rep {
    Unknown,
    Unit,
    I8,
    I16,
    I32,
    I64,
    U8,
    U16,
    U32,
    U64,
    F32,
    F64,
    Str,
    Ptr,
    Adt(String),
    Function(usize),


    ExternFunction {
        ctype: String,
        args: Vec<Rep>,
        variadic: bool,
        ret: Box<Rep>,
    },
}

impl Rep {
    #[inline]
    fn is_integer(&self) -> bool {
        matches!(
            self,
            Self::I8
                | Self::I16
                | Self::I32
                | Self::I64
                | Self::U8
                | Self::U16
                | Self::U32
                | Self::U64
        )
    }
    #[inline]
    fn is_float(&self) -> bool {
        matches!(self, Self::F32 | Self::F64)
    }
    fn ctype(&self, functions: &[FunctionInfo]) -> String {
        match self {
            Self::Unknown => "int64_t".into(),
            Self::Unit => "void".into(),
            Self::I8 => "int8_t".into(),
            Self::I16 => "int16_t".into(),
            Self::I32 => "int32_t".into(),
            Self::I64 => "int64_t".into(),
            Self::U8 => "uint8_t".into(),
            Self::U16 => "uint16_t".into(),
            Self::U32 => "uint32_t".into(),
            Self::U64 => "uint64_t".into(),
            Self::F32 => "float".into(),
            Self::F64 => "double".into(),
            Self::Str => "const char *".into(),
            Self::Ptr => "const void *".into(),
            Self::Adt(n) => format!("struct mak_{} *", mangle_ident(n)),
            Self::Function(id) => function_pointer_type(*id, functions),
            Self::ExternFunction { ctype, .. } => ctype.clone(),
        }
    }
}

#[inline]
fn rep_is_adt(r: &Rep) -> bool {
    matches!(r, Rep::Adt(_))
}

#[inline]
fn adt_struct_type(r: &Rep) -> String {
    match r {
        Rep::Adt(n) => format!("struct mak_{}", mangle_ident(n)),
        _ => "void".into(),
    }
}

#[derive(Clone, Debug)]
struct CtorInfo {
    id: usize,
    name: String,
    type_name: String,
    tag: String,
    args: Vec<Ty>,
    fields: Vec<Rep>,
    arity: usize,
}

#[derive(Clone, Debug)]
struct ExternalInfo {
    id: usize,
    decl: Extern,
    c_name: String,
    noreturn: bool,
}

#[derive(Clone, Debug)]
struct FunctionInfo {
    id: usize,
    c_name: String,
    source_name: Option<String>,
    expr: Expr,
    params: Vec<String>,
    captures: Vec<String>,
    param_reps: Vec<Rep>,
    capture_reps: Vec<Rep>,
    ret_rep: Rep,
    pure: bool,
}

#[derive(Clone, Debug)]
enum CallTarget {
    Function(usize),
    External(usize),
    Indirect(Box<LirExpr>),
}

#[derive(Clone, Copy, Debug, PartialEq, Eq)]
enum BinOpLir {
    Add,
    Sub,
    Mul,
    Div,
    Rem,
    And,
    Or,
    Xor,
    Shl,
    Shr,
    Eq,
    Ne,
    Lt,
    Gt,
    Le,
    Ge,
    LAnd,
    LOr,
}

#[derive(Clone, Copy, Debug, PartialEq, Eq)]
enum UnOpLir {
    Neg,
    Not,
    BitNot,
}

#[derive(Clone, Debug)]
enum LirPat {
    Wild,
    Bind { code: String, rep: Rep },
    Int(u64),
    Str(Vec<u8>),
    Ctor { ctor: usize, args: Vec<LirPat> },
    Tuple(Vec<LirPat>),
}

#[derive(Clone, Debug)]
enum LirExpr {
    Unit,
    Int(i128, Rep),
    Float(f64, Rep),
    Str(Vec<u8>),
    Var(String, Rep),
    FnRef(usize),
    Call {
        target: CallTarget,
        args: Vec<LirExpr>,
        rep: Rep,
        noreturn: bool,
        pure: bool,
    },
    Construct {
        ctor: usize,
        args: Vec<LirExpr>,
        rep: Rep,
    },
    Field {
        base: Box<LirExpr>,
        ctor: usize,
        index: usize,
        rep: Rep,
    },
    Let {
        code: String,
        rep: Rep,
        value: Box<LirExpr>,
        body: Box<LirExpr>,
    },
    Block {
        stmts: Vec<LirExpr>,
        tail: Box<LirExpr>,
    },
    Case {
        scrut: Box<LirExpr>,
        arms: Vec<LirArm>,
        rep: Rep,
    },
    Bin {
        op: BinOpLir,
        left: Box<LirExpr>,
        right: Box<LirExpr>,
        rep: Rep,
    },
    Un {
        op: UnOpLir,
        expr: Box<LirExpr>,
        rep: Rep,
    },
    RawAst(Box<Expr>, Rep, HashMap<String, Binding>),
    RuntimeCall {
        name: String,
        args: Vec<LirExpr>,
        rep: Rep,
    },
    RuntimeConstruct {
        name: String,
        args: Vec<LirExpr>,
        rep: Rep,
    },
    DirectC {
        code: String,
        rep: Rep,
    },
    Fallback,
}

#[derive(Clone, Debug)]
struct LirArm {
    pat: LirPat,
    body: LirExpr,
}

#[derive(Clone, Debug)]
struct LirFunction {
    info: FunctionInfo,
    body: LirExpr,
}

#[derive(Clone, Debug)]
struct PointerInfo {

    elem_rep: Rep,

    elem_ctype: String,

    nested: Option<Box<PointerInfo>>,
}

#[derive(Clone, Debug)]
enum Binding {
    Value {
        code: String,
        rep: Rep,


        ptr_info: Option<PointerInfo>,
    },
    Function { id: usize },
}

#[derive(Clone, Debug)]
struct RecoveryNote {
    function: Option<usize>,
    span_lo: u32,
    span_hi: u32,
    stage: String,
    message: String,
}


#[derive(Clone, Copy, Debug, PartialEq, Eq, Hash, PartialOrd, Ord)]
enum RtFeature {

    Arena,
    Recovery,
    FloatBits,
    FoldEq,
    CanonEq,
    KindName,
    KindCompatible,
    Hash,


    FnDescType,
    CtorDescType,
    FnTable,
    CtorTable,
    FnDispatcher,
    CtorDispatcher,
}

#[derive(Clone, Debug, Default)]
struct RtPlan {
    features: BTreeSet<RtFeature>,
    runtime_fn_ids: BTreeSet<usize>,
    runtime_ctor_ids: BTreeSet<usize>,
}

impl RtPlan {
    fn on(&self, f: RtFeature) -> bool {
        self.features.contains(&f)
    }
    fn is_empty(&self) -> bool {
        self.features.is_empty()
    }
    fn any_dispatcher(&self) -> bool {
        self.on(RtFeature::FnDispatcher) || self.on(RtFeature::CtorDispatcher)
    }

    fn enable(&mut self, f: RtFeature) {
        self.features.insert(f);
    }

    fn detect(a: &Analyzer, fs: &[LirFunction], emitted_ids: &BTreeSet<usize>) -> Self {
        let mut p = RtPlan::default();
        p.enable(RtFeature::Recovery);

        let needs_arena = !a.constructors.is_empty();
        if needs_arena {
            p.enable(RtFeature::Arena);
        }

        fn walk(e: &LirExpr, p: &mut RtPlan, a: &Analyzer) {
            match e {
                LirExpr::RuntimeCall { args, .. } => {
                    let arity = args.len();
                    for id in a.runtime_function_candidates(arity) {
                        p.runtime_fn_ids.insert(id);
                    }
                    for arg in args {
                        walk(arg, p, a);
                    }
                }
                LirExpr::RuntimeConstruct { args, .. } => {
                    let arity = args.len();
                    for id in a.runtime_ctor_candidates(arity) {
                        p.runtime_ctor_ids.insert(id);
                    }
                    for arg in args {
                        walk(arg, p, a);
                    }
                }
                LirExpr::Call { target, args, .. } => {
                    if let CallTarget::Indirect(x) = target {
                        walk(x, p, a);
                    }
                    for arg in args {
                        walk(arg, p, a);
                    }
                }
                LirExpr::Construct { args, .. } => {
                    for arg in args {
                        walk(arg, p, a);
                    }
                }
                LirExpr::Field { base, .. } => walk(base, p, a),
                LirExpr::Let { value, body, .. } => {
                    walk(value, p, a);
                    walk(body, p, a);
                }
                LirExpr::Block { stmts, tail } => {
                    for s in stmts {
                        walk(s, p, a);
                    }
                    walk(tail, p, a);
                }
                LirExpr::Case { scrut, arms, .. } => {
                    walk(scrut, p, a);
                    for arm in arms {
                        walk(&arm.body, p, a);
                    }
                }
                LirExpr::Bin { left, right, .. } => {
                    walk(left, p, a);
                    walk(right, p, a);
                }
                LirExpr::Un { expr, .. } => walk(expr, p, a),
                LirExpr::RawAst(_, _, _) => {
                    p.enable(RtFeature::FnDispatcher);
                    p.enable(RtFeature::CtorDispatcher);
                }
                LirExpr::DirectC { .. }
                | LirExpr::Unit
                | LirExpr::Int(..)
                | LirExpr::Float(..)
                | LirExpr::Str(..)
                | LirExpr::Var(..)
                | LirExpr::FnRef(..)
                | LirExpr::Fallback => {}
            }
        }

        for f in fs {
            walk(&f.body, &mut p, a);
        }
        p.runtime_fn_ids.retain(|id| emitted_ids.contains(id));
        p.runtime_ctor_ids.retain(|id| id < &a.constructors.len());

        let has_fn = !p.runtime_fn_ids.is_empty();
        let has_ctor = !p.runtime_ctor_ids.is_empty();

        if has_fn || has_ctor || p.on(RtFeature::FnDispatcher) || p.on(RtFeature::CtorDispatcher) {
            if has_fn {
                p.enable(RtFeature::FnDispatcher);
            }
            if has_ctor {
                p.enable(RtFeature::CtorDispatcher);
            }
            if p.on(RtFeature::FnDispatcher) || p.on(RtFeature::CtorDispatcher) {
                p.enable(RtFeature::FloatBits);
                p.enable(RtFeature::FoldEq);
                p.enable(RtFeature::CanonEq);
                p.enable(RtFeature::KindName);
                p.enable(RtFeature::KindCompatible);
                p.enable(RtFeature::Hash);
            }
            if has_fn {
                p.enable(RtFeature::FnDescType);
                p.enable(RtFeature::FnTable);
            }
            if has_ctor {
                p.enable(RtFeature::CtorDescType);
                p.enable(RtFeature::CtorTable);
            }
        }

        if p.on(RtFeature::Arena)
            || p.on(RtFeature::FnDispatcher)
            || p.on(RtFeature::CtorDispatcher)
        {
            p.enable(RtFeature::Recovery);
        }

        p
    }
}


#[derive(Clone, Copy, Debug, PartialEq, Eq)]
enum MemorySiteKind {
    Construct,
    CallResult,
    DynamicCallResult,
    CaseResult,
    RuntimeCall,
    RuntimeConstruct,
    DirectAst,
}

#[derive(Clone, Copy, Debug, PartialEq, Eq)]
enum MemoryStrategy {
    Arena,
    Heap,
}

#[derive(Clone, Debug, Default)]
struct MemoryPlan {
    rounds: usize,
    converged: bool,
    ast_nodes: usize,
    lir_nodes: usize,
    allocation_sites: usize,
    stack_sites: usize,
    arena_sites: usize,
    heap_sites: usize,
    estimated_stack_bytes: usize,
    estimated_arena_bytes: usize,
    estimated_heap_bytes: usize,
    stack_local_lets: HashMap<(Option<usize>, String), Rep>,
    force_heap_unknown: bool,
}

impl MemoryPlan {
    const MAX_ROUNDS: usize = 4096;
    const STACK_OBJECT_LIMIT: usize = 16 * 1024;
    const ARENA_OBJECT_LIMIT: usize = 256 * 1024;

    fn analyze(a: &Analyzer, p: &Program, fs: &[LirFunction]) -> Self {
        let mut plan = Self::default();
        plan.ast_nodes = memory_ast_nodes(p);


        let mut dynamic = HashMap::<usize, bool>::new();
        for f in fs {
            dynamic.insert(f.info.id, memory_lir_has_dynamic(&f.body));
        }

        let mut converged = false;
        for round in 1..=Self::MAX_ROUNDS {
            let mut changed = false;
            for f in fs {
                let mut next = *dynamic.get(&f.info.id).unwrap_or(&false);
                let mut callees = Vec::new();
                memory_collect_function_edges(&f.body, &mut callees);
                for callee in callees {
                    if let Some(x) = dynamic.get(&callee).copied() {
                        next |= x;
                    }
                }
                if next && !dynamic.get(&f.info.id).copied().unwrap_or(false) {
                    dynamic.insert(f.info.id, true);
                    changed = true;
                }
            }
            plan.rounds = round;
            if !changed {
                converged = true;
                break;
            }
        }
        plan.converged = converged;
        plan.force_heap_unknown = !converged;

        for f in fs {
            memory_collect_stack_local_lets(
                &f.body,
                Some(f.info.id),
                a,
                &mut plan.stack_local_lets,
            );
        }

        for f in fs {
            plan.lir_nodes += memory_lir_nodes(&f.body);
            let mut sites = Vec::<(MemorySiteKind, Rep, Option<usize>)>::new();
            memory_collect_alloc_sites(&f.body, Some(f.info.id), &mut sites);
            for (kind, rep, fid) in sites {
                plan.account_site(a, kind, &rep, fid);
            }
        }


        if !fs.iter().any(|f| matches!(f.info.source_name.as_deref(), Some("main"))) {
            let mut lowerer = Lowerer::new(a);
            let mut env = HashMap::new();
            let entry = find_structural_entry(p, &a.function_by_source);
            if entry.is_none() {
                let body = lowerer.lower_expr(&p.body, &mut env, None);
                memory_collect_stack_local_lets(&body, None, a, &mut plan.stack_local_lets);
                plan.lir_nodes += memory_lir_nodes(&body);
                let mut sites = Vec::<(MemorySiteKind, Rep, Option<usize>)>::new();
                memory_collect_alloc_sites(&body, None, &mut sites);
                for (kind, rep, fid) in sites {
                    plan.account_site(a, kind, &rep, fid);
                }
            }
        }

        if !plan.force_heap_unknown {
            plan.stack_sites = plan.stack_local_lets.len();
            for rep in plan.stack_local_lets.values() {
                if let Rep::Adt(name) = rep {
                    let (bytes, _) = adt_object_layout(a, name);
                    plan.estimated_stack_bytes = plan.estimated_stack_bytes.saturating_add(bytes);
                }
            }
        }

        plan
    }

    fn account_site(&mut self, a: &Analyzer, kind: MemorySiteKind, rep: &Rep, fid: Option<usize>) {
        if !rep_is_adt(rep) {
            return;
        }
        let (bytes, _align) = adt_object_layout(a, match rep {
            Rep::Adt(n) => n,
            _ => return,
        });
        let strategy = self.strategy(kind, fid, bytes);
        self.allocation_sites += 1;
        match strategy {
            MemoryStrategy::Arena => {
                self.arena_sites += 1;
                self.estimated_arena_bytes = self.estimated_arena_bytes.saturating_add(bytes);
            }
            MemoryStrategy::Heap => {
                self.heap_sites += 1;
                self.estimated_heap_bytes = self.estimated_heap_bytes.saturating_add(bytes);
            }
        }
    }

    fn strategy(&self, kind: MemorySiteKind, fid: Option<usize>, bytes: usize) -> MemoryStrategy {


        if matches!(
            kind,
            MemorySiteKind::RuntimeCall
                | MemorySiteKind::RuntimeConstruct
                | MemorySiteKind::DynamicCallResult
                | MemorySiteKind::DirectAst
        ) {
            return MemoryStrategy::Heap;
        }

        if self.force_heap_unknown {
            return MemoryStrategy::Heap;
        }

        if bytes > Self::ARENA_OBJECT_LIMIT {
            return MemoryStrategy::Heap;
        }


        if matches!(
            kind,
            MemorySiteKind::Construct
                | MemorySiteKind::CallResult
                | MemorySiteKind::CaseResult
        ) {
            return MemoryStrategy::Arena;
        }

        let _ = fid;
        MemoryStrategy::Arena
    }


}

fn memory_ast_nodes(p: &Program) -> usize {
    fn pat(p: &Pat) -> usize {
        match p {
            Pat::Ctor { args, .. } => 1 + args.iter().map(pat).sum::<usize>(),
            Pat::Tuple(xs) => 1 + xs.iter().map(pat).sum::<usize>(),
            _ => 1,
        }
    }
    fn expr(e: &Expr) -> usize {
        match &e.kind {
            ExprKind::App { fun, args } => 1 + expr(fun) + args.iter().map(expr).sum::<usize>(),
            ExprKind::Let { value, body, .. } => 1 + expr(value) + expr(body),
            ExprKind::Fn { body, .. } => 1 + expr(body),
            ExprKind::Case { scrut, arms } => {
                1 + expr(scrut) + arms.iter().map(|a| pat(&a.pat) + expr(&a.body)).sum::<usize>()
            }
            ExprKind::Block { stmts, tail } => 1 + stmts.iter().map(expr).sum::<usize>() + expr(tail),
            ExprKind::Bin { l, r, .. } => 1 + expr(l) + expr(r),
            ExprKind::Un { e, .. } | ExprKind::Quote(e) => 1 + expr(e),
            ExprKind::Int(_) | ExprKind::Float(_) | ExprKind::Str(_) | ExprKind::Var(_) => 1,
        }
    }
    let mut n = expr(&p.body);
    for d in &p.decls {
        n += 1;
        if let Decl::Let { value, .. } = d {
            n += expr(value);
        }
    }
    n
}

fn memory_lir_nodes(e: &LirExpr) -> usize {
    match e {
        LirExpr::Unit | LirExpr::Int(..) | LirExpr::Float(..) | LirExpr::Str(_) | LirExpr::Var(..) | LirExpr::FnRef(_) | LirExpr::Fallback => 1,
        LirExpr::Call { target, args, .. } => {
            1 + match target { CallTarget::Indirect(x) => memory_lir_nodes(x), _ => 0 }
                + args.iter().map(memory_lir_nodes).sum::<usize>()
        }
        LirExpr::Construct { args, .. } => 1 + args.iter().map(memory_lir_nodes).sum::<usize>(),
        LirExpr::Field { base, .. } => 1 + memory_lir_nodes(base),
        LirExpr::Let { value, body, .. } => 1 + memory_lir_nodes(value) + memory_lir_nodes(body),
        LirExpr::Block { stmts, tail } => 1 + stmts.iter().map(memory_lir_nodes).sum::<usize>() + memory_lir_nodes(tail),
        LirExpr::Case { scrut, arms, .. } => 1 + memory_lir_nodes(scrut) + arms.iter().map(|a| memory_lir_nodes(&a.body)).sum::<usize>(),
        LirExpr::Bin { left, right, .. } => 1 + memory_lir_nodes(left) + memory_lir_nodes(right),
        LirExpr::Un { expr, .. } => 1 + memory_lir_nodes(expr),
        LirExpr::RawAst(_, _, _) => 1,
        LirExpr::RuntimeCall { args, .. } | LirExpr::RuntimeConstruct { args, .. } => 1 + args.iter().map(memory_lir_nodes).sum::<usize>(),
        LirExpr::DirectC { .. } => 1,
    }
}

fn memory_lir_has_dynamic(e: &LirExpr) -> bool {
    match e {
        LirExpr::RuntimeCall { .. } | LirExpr::RuntimeConstruct { .. } | LirExpr::RawAst(..) => true,
        LirExpr::Call { target, args, .. } => {
            matches!(target, CallTarget::Indirect(_))
                || args.iter().any(memory_lir_has_dynamic)
                || matches!(target, CallTarget::Indirect(x) if memory_lir_has_dynamic(x))
        }
        LirExpr::Construct { args, .. } => args.iter().any(memory_lir_has_dynamic),
        LirExpr::Field { base, .. } => memory_lir_has_dynamic(base),
        LirExpr::Let { value, body, .. } => memory_lir_has_dynamic(value) || memory_lir_has_dynamic(body),
        LirExpr::Block { stmts, tail } => stmts.iter().any(memory_lir_has_dynamic) || memory_lir_has_dynamic(tail),
        LirExpr::Case { scrut, arms, .. } => memory_lir_has_dynamic(scrut) || arms.iter().any(|x| memory_lir_has_dynamic(&x.body)),
        LirExpr::Bin { left, right, .. } => memory_lir_has_dynamic(left) || memory_lir_has_dynamic(right),
        LirExpr::Un { expr, .. } => memory_lir_has_dynamic(expr),
        LirExpr::Unit | LirExpr::Int(..) | LirExpr::Float(..) | LirExpr::Str(_) | LirExpr::Var(..) | LirExpr::FnRef(_) | LirExpr::DirectC { .. } | LirExpr::Fallback => false,
    }
}

fn memory_collect_function_edges(e: &LirExpr, out: &mut Vec<usize>) {
    match e {
        LirExpr::Call { target, args, .. } => {
            if let CallTarget::Function(id) = target {
                out.push(*id);
            }
            if let CallTarget::Indirect(x) = target {
                memory_collect_function_edges(x, out);
            }
            for x in args { memory_collect_function_edges(x, out); }
        }
        LirExpr::Construct { args, .. } | LirExpr::RuntimeCall { args, .. } | LirExpr::RuntimeConstruct { args, .. } => {
            for x in args { memory_collect_function_edges(x, out); }
        }
        LirExpr::Field { base, .. } => memory_collect_function_edges(base, out),
        LirExpr::Let { value, body, .. } => { memory_collect_function_edges(value, out); memory_collect_function_edges(body, out); }
        LirExpr::Block { stmts, tail } => { for x in stmts { memory_collect_function_edges(x, out); } memory_collect_function_edges(tail, out); }
        LirExpr::Case { scrut, arms, .. } => { memory_collect_function_edges(scrut, out); for x in arms { memory_collect_function_edges(&x.body, out); } }
        LirExpr::Bin { left, right, .. } => { memory_collect_function_edges(left, out); memory_collect_function_edges(right, out); }
        LirExpr::Un { expr, .. } => memory_collect_function_edges(expr, out),
        LirExpr::RawAst(..) | LirExpr::Unit | LirExpr::Int(..) | LirExpr::Float(..) | LirExpr::Str(_) | LirExpr::Var(..) | LirExpr::FnRef(_) | LirExpr::DirectC { .. } | LirExpr::Fallback => {}
    }
}

fn memory_collect_alloc_sites(e: &LirExpr, fid: Option<usize>, out: &mut Vec<(MemorySiteKind, Rep, Option<usize>)>) {
    match e {
        LirExpr::Call { target, args, rep, .. } => {
            if rep_is_adt(rep) {
                let kind = match target {
                    CallTarget::Function(_) => MemorySiteKind::CallResult,
                    CallTarget::External(_) | CallTarget::Indirect(_) => MemorySiteKind::DynamicCallResult,
                };
                out.push((kind, rep.clone(), fid));
            }
            if let CallTarget::Indirect(x) = target { memory_collect_alloc_sites(x, fid, out); }
            for x in args { memory_collect_alloc_sites(x, fid, out); }
        }
        LirExpr::Construct { args, rep, .. } => {
            if rep_is_adt(rep) { out.push((MemorySiteKind::Construct, rep.clone(), fid)); }
            for x in args { memory_collect_alloc_sites(x, fid, out); }
        }
        LirExpr::Case { scrut, arms, rep } => {
            if rep_is_adt(rep) { out.push((MemorySiteKind::CaseResult, rep.clone(), fid)); }
            memory_collect_alloc_sites(scrut, fid, out);
            for x in arms { memory_collect_alloc_sites(&x.body, fid, out); }
        }
        LirExpr::RuntimeCall { args, rep, .. } => {
            if rep_is_adt(rep) { out.push((MemorySiteKind::RuntimeCall, rep.clone(), fid)); }
            for x in args { memory_collect_alloc_sites(x, fid, out); }
        }
        LirExpr::RuntimeConstruct { args, rep, .. } => {
            if rep_is_adt(rep) { out.push((MemorySiteKind::RuntimeConstruct, rep.clone(), fid)); }
            for x in args { memory_collect_alloc_sites(x, fid, out); }
        }
        LirExpr::Field { base, .. } => memory_collect_alloc_sites(base, fid, out),
        LirExpr::Let { value, body, .. } => { memory_collect_alloc_sites(value, fid, out); memory_collect_alloc_sites(body, fid, out); }
        LirExpr::Block { stmts, tail } => { for x in stmts { memory_collect_alloc_sites(x, fid, out); } memory_collect_alloc_sites(tail, fid, out); }
        LirExpr::Bin { left, right, .. } => { memory_collect_alloc_sites(left, fid, out); memory_collect_alloc_sites(right, fid, out); }
        LirExpr::Un { expr, .. } => memory_collect_alloc_sites(expr, fid, out),
        LirExpr::RawAst(_, rep, _) => { if rep_is_adt(rep) { out.push((MemorySiteKind::DirectAst, rep.clone(), fid)); } }
        LirExpr::Unit | LirExpr::Int(..) | LirExpr::Float(..) | LirExpr::Str(_) | LirExpr::Var(..) | LirExpr::FnRef(_) | LirExpr::DirectC { .. } | LirExpr::Fallback => {}
    }
}

fn memory_local_let_uses_are_non_escaping(e: &LirExpr, code: &str) -> bool {
    fn walk(e: &LirExpr, code: &str, read_only: bool) -> bool {
        match e {
            LirExpr::Var(name, _) => name != code || read_only,
            LirExpr::Field { base, .. } => walk(base, code, true),
            LirExpr::Case { scrut, arms, .. } => {
                walk(scrut, code, true)
                    && arms.iter().all(|arm| {
                        !matches!(&arm.pat, LirPat::Bind { .. })
                            && walk(&arm.body, code, false)
                    })
            }
            LirExpr::Call { target, args, .. } => {
                let target_ok = match target {
                    CallTarget::Indirect(x) => walk(x, code, false),
                    CallTarget::Function(_) | CallTarget::External(_) => true,
                };
                target_ok && args.iter().all(|x| walk(x, code, false))
            }
            LirExpr::Construct { args, .. }
            | LirExpr::RuntimeCall { args, .. }
            | LirExpr::RuntimeConstruct { args, .. } => {
                args.iter().all(|x| walk(x, code, false))
            }
            LirExpr::Let { value, body, .. } => {
                walk(value, code, false) && walk(body, code, false)
            }
            LirExpr::Block { stmts, tail } => {
                stmts.iter().all(|x| walk(x, code, false)) && walk(tail, code, false)
            }
            LirExpr::Bin { left, right, .. } => {
                walk(left, code, false) && walk(right, code, false)
            }
            LirExpr::Un { expr, .. } => walk(expr, code, false),
            LirExpr::RawAst(..) | LirExpr::DirectC { .. } | LirExpr::Fallback => false,
            LirExpr::Unit
            | LirExpr::Int(..)
            | LirExpr::Float(..)
            | LirExpr::Str(_)
            | LirExpr::FnRef(_) => true,
        }
    }
    walk(e, code, false)
}

fn memory_collect_stack_local_lets(
    e: &LirExpr,
    fid: Option<usize>,
    a: &Analyzer,
    out: &mut HashMap<(Option<usize>, String), Rep>,
) {
    match e {
        LirExpr::Let { code, rep, value, body } => {
            if let Rep::Adt(_) = rep {
                if memory_local_let_uses_are_non_escaping(body, code) {
                    let bytes = match rep {
                        Rep::Adt(name) => adt_object_layout(a, name).0,
                        _ => usize::MAX,
                    };
                    if bytes <= MemoryPlan::STACK_OBJECT_LIMIT {
                        out.insert((fid, code.clone()), rep.clone());
                    }
                }
            }
            memory_collect_stack_local_lets(value, fid, a, out);
            memory_collect_stack_local_lets(body, fid, a, out);
        }
        LirExpr::Call { target, args, .. } => {
            if let CallTarget::Indirect(x) = target {
                memory_collect_stack_local_lets(x, fid, a, out);
            }
            for x in args { memory_collect_stack_local_lets(x, fid, a, out); }
        }
        LirExpr::Construct { args, .. }
        | LirExpr::RuntimeCall { args, .. }
        | LirExpr::RuntimeConstruct { args, .. } => {
            for x in args { memory_collect_stack_local_lets(x, fid, a, out); }
        }
        LirExpr::Field { base, .. } | LirExpr::Un { expr: base, .. } => {
            memory_collect_stack_local_lets(base, fid, a, out);
        }
        LirExpr::Block { stmts, tail } => {
            for x in stmts { memory_collect_stack_local_lets(x, fid, a, out); }
            memory_collect_stack_local_lets(tail, fid, a, out);
        }
        LirExpr::Case { scrut, arms, .. } => {
            memory_collect_stack_local_lets(scrut, fid, a, out);
            for arm in arms { memory_collect_stack_local_lets(&arm.body, fid, a, out); }
        }
        LirExpr::Bin { left, right, .. } => {
            memory_collect_stack_local_lets(left, fid, a, out);
            memory_collect_stack_local_lets(right, fid, a, out);
        }
        LirExpr::Unit | LirExpr::Int(..) | LirExpr::Float(..) | LirExpr::Str(_)
        | LirExpr::Var(..) | LirExpr::FnRef(_) | LirExpr::RawAst(..)
        | LirExpr::DirectC { .. } | LirExpr::Fallback => {}
    }
}

fn memory_align_up(v: usize, a: usize) -> usize {
    if a <= 1 { return v; }
    let m = a - 1;
    v.saturating_add(m) & !m
}

fn memory_rep_size_align(r: &Rep) -> (usize, usize) {
    match r {
        Rep::Unit => (0, 1),
        Rep::I8 | Rep::U8 => (1, 1),
        Rep::I16 | Rep::U16 => (2, 2),
        Rep::I32 | Rep::U32 | Rep::F32 => (4, 4),
        Rep::I64 | Rep::U64 | Rep::F64 | Rep::Str | Rep::Ptr | Rep::Function(_) | Rep::ExternFunction { .. } | Rep::Adt(_) | Rep::Unknown => (8, 8),
    }
}

fn adt_object_layout(a: &Analyzer, name: &str) -> (usize, usize) {
    let ctors: Vec<&CtorInfo> = a.constructors.iter().filter(|c| c.type_name == name).collect();
    if ctors.is_empty() { return (8, 8); }

    let mut payload_size = 0usize;
    let mut payload_align = 1usize;
    for c in ctors {
        let mut off = 0usize;
        let mut align = 1usize;
        for r in &c.fields {
            let (sz, al) = memory_rep_size_align(r);
            align = align.max(al);
            off = memory_align_up(off, al);
            off = off.saturating_add(sz);
        }
        off = memory_align_up(off, align);
        payload_size = payload_size.max(off);
        payload_align = payload_align.max(align);
    }

    let after_tag = memory_align_up(4, payload_align);
    let total = memory_align_up(after_tag.saturating_add(payload_size), payload_align.max(4));
    (total.max(4), payload_align.max(4))
}


struct Analyzer {
    constructors: Vec<CtorInfo>,
    ctor_by_name: HashMap<String, usize>,
    externals: Vec<ExternalInfo>,
    external_by_name: HashMap<String, usize>,
    functions: Vec<FunctionInfo>,
    function_by_source: HashMap<String, usize>,
    lambda_by_span: HashMap<(u32, u32), usize>,
    globals: HashSet<String>,
    global_reps: HashMap<String, Rep>,
    int_consts: HashMap<String, i128>,
    type_names: HashSet<String>,
    needs_string: bool,
    needs_stdlib: bool,
    recursive_functions: HashSet<usize>,
    reachable_functions: HashSet<usize>,
    recovered_function_aliases: HashMap<String, usize>,
    recovered_ctor_aliases: HashMap<String, usize>,
    pattern_ctor_names: HashSet<String>,
    recovery: RefCell<Vec<RecoveryNote>>,
    debug: bool,
}

impl Analyzer {
    #[inline]
    fn dbg(&self, stage: &str, msg: impl std::fmt::Display) {
        if self.debug {
            eprintln!("[CGEN:{:<7}] {}", stage, msg);
        }
    }

    fn note_recovery(
        &self,
        current: Option<usize>,
        e: &Expr,
        stage: &str,
        message: impl Into<String>,
    ) {
        self.recovery.borrow_mut().push(RecoveryNote {
            function: current,
            span_lo: e.span.lo,
            span_hi: e.span.hi,
            stage: stage.to_string(),
            message: message.into(),
        });
    }

    fn recovery_detail(&self, current: Option<usize>, e: &Expr) -> String {
        let records = self
            .recovery
            .borrow()
            .iter()
            .filter(|x| x.function == current && x.span_lo == e.span.lo && x.span_hi == e.span.hi)
            .map(|x| format!("{}: {}", x.stage, x.message))
            .collect::<Vec<_>>();
        let f = current
            .map(|x| format!("f{}", x))
            .unwrap_or_else(|| "main".into());
        let attempts = if records.is_empty() {
            "no recorded recovery attempt".into()
        } else {
            records.join(" | ")
        };
        format!(
            "CGen failure; function={}; span={}..{}; expr={}; attempts={}",
            f,
            e.span.lo,
            e.span.hi,
            expr_debug(e),
            attempts
        )
    }

    fn new() -> Self {
        Self {
            constructors: Vec::new(),
            ctor_by_name: HashMap::new(),
            externals: Vec::new(),
            external_by_name: HashMap::new(),
            functions: Vec::new(),
            function_by_source: HashMap::new(),
            lambda_by_span: HashMap::new(),
            globals: HashSet::new(),
            global_reps: HashMap::new(),
            int_consts: HashMap::new(),
            type_names: HashSet::new(),
            needs_string: false,
            needs_stdlib: false,
            recursive_functions: HashSet::new(),
            reachable_functions: HashSet::new(),
            recovered_function_aliases: HashMap::new(),
            recovered_ctor_aliases: HashMap::new(),
            pattern_ctor_names: HashSet::new(),
            recovery: RefCell::new(Vec::new()),
            debug: matches!(
                std::env::var("MAK_CGEN_DEBUG").ok().as_deref(),
                Some("1") | Some("true") | Some("on") | Some("yes")
            ),
        }
    }

    fn flatten_application<'a>(fun: &'a Expr, args: &'a [Expr]) -> (&'a Expr, Vec<&'a Expr>) {
        fn walk<'a>(e: &'a Expr, out: &mut Vec<&'a Expr>) -> &'a Expr {
            match &e.kind {
                ExprKind::App { fun, args } => {
                    let base = walk(fun, out);
                    out.extend(args.iter());
                    base
                }
                _ => e,
            }
        }
        let mut out = Vec::new();
        let base = walk(fun, &mut out);
        out.extend(args.iter());
        (base, out)
    }

    fn register(&mut self, p: &Program) {
        for d in &p.decls {
            match d {
                Decl::Type { name, ctors, .. } => {
                    self.type_names.insert(name.clone());
                    for c in ctors {
                        let id = self.constructors.len();
                        self.ctor_by_name.insert(c.name.clone(), id);
                        self.constructors.push(CtorInfo {
                            id,
                            name: c.name.clone(),
                            type_name: name.clone(),
                            tag: format!("mak_ir_tag_{}", id),
                            args: c.args.clone(),
                            fields: vec![Rep::Unknown; c.args.len()],
                            arity: c.args.len(),
                        });
                    }
                }
                Decl::Let { name, value, .. } => {
                    self.globals.insert(name.clone());
                    if let ExprKind::Int(v) = &value.kind {
                        self.int_consts.insert(name.clone(), (*v).into());
                    }
                }
                Decl::External(e) => {
                    self.globals.insert(e.name.clone());
                    let id = self.externals.len();
                    let noreturn = matches!(&e.mak_ty, Ty::Bottom);
                    self.external_by_name.insert(e.name.clone(), id);
                    self.externals.push(ExternalInfo {
                        id,
                        decl: e.clone(),
                        c_name: format!("mak_ir_ext_{}", id),
                        noreturn,
                    });
                }
                Decl::Bring(_) => {}
            }
        }
        for c in &mut self.constructors {
            for i in 0..c.fields.len() {
                c.fields[i] = rep_from_ty(&c.args[i], &self.type_names);
            }
        }
        self.scan_requirements(p);
        for d in &p.decls {
            if let Decl::Let { name, value, .. } = d {
                if matches!(&value.kind, ExprKind::Fn { .. }) {
                    let id = self.new_function(value, Some(name.clone()), &HashSet::new());
                    self.function_by_source.insert(name.clone(), id);
                }
            }
        }
        for i in 0..self.functions.len() {
            let f = self.functions[i].clone();
            let mut visible = self.globals.clone();
            for p in &f.params {
                visible.insert(p.clone());
            }
            if let ExprKind::Fn { body, .. } = &f.expr.kind {
                self.discover(body, &visible);
            }
        }
        self.discover(&p.body, &self.globals.clone());
        self.seed_types();
        self.seed_global_reps(p);
        self.collect_pattern_ctor_names(p);
        self.recover_symbol_aliases(p);
        self.dbg("ANALYZE", format!(
            "registered {} function(s), {} constructor(s), {} external(s), {} integer constant(s)",
            self.functions.len(), self.constructors.len(), self.externals.len(), self.int_consts.len()));
    }

    fn seed_global_reps(&mut self, p: &Program) {
        for d in &p.decls {
            if let Decl::Let { name, value, .. } = d {
                if matches!(&value.kind, ExprKind::Fn { .. }) {
                    if let Some(id) = self.function_by_source.get(name).copied() {
                        self.global_reps.insert(name.clone(), Rep::Function(id));
                    }
                } else {
                    self.global_reps
                        .insert(name.clone(), infer_ast_rep(value, self));
                }
            }
            if let Decl::External(e) = d {
                self.global_reps
                    .insert(e.name.clone(), foreign_value_rep(&e.foreign));
            }
        }
    }

    fn refresh_global_reps(&mut self, p: &Program) {
        for d in &p.decls {
            if let Decl::Let { name, value, .. } = d {
                let r = if let Some(id) = self.function_by_source.get(name).copied() {
                    Rep::Function(id)
                } else {
                    infer_ast_rep(value, self)
                };
                let old = self.global_reps.get(name).cloned().unwrap_or(Rep::Unknown);
                self.global_reps.insert(name.clone(), merge_rep(old, r));
            }
        }
    }

    fn scan_requirements(&mut self, p: &Program) {
        fn walk(e: &Expr, string_need: &mut bool) {
            match &e.kind {
                ExprKind::Str(_) => *string_need = true,
                ExprKind::App { fun, args } => {
                    walk(fun, string_need);
                    for a in args {
                        walk(a, string_need);
                    }
                }
                ExprKind::Case { scrut, arms } => {
                    walk(scrut, string_need);
                    for arm in arms {
                        if pat_has_string(&arm.pat) {
                            *string_need = true;
                        }
                        walk(&arm.body, string_need);
                    }
                }
                ExprKind::Let { value, body, .. } => {
                    walk(value, string_need);
                    walk(body, string_need);
                }
                ExprKind::Fn { body, .. } => walk(body, string_need),
                ExprKind::Block { stmts, tail } => {
                    for s in stmts {
                        walk(s, string_need);
                    }
                    walk(tail, string_need);
                }
                ExprKind::Bin { l, r, .. } => {
                    walk(l, string_need);
                    walk(r, string_need);
                }
                ExprKind::Un { e, .. } | ExprKind::Quote(e) => walk(e, string_need),
                ExprKind::Int(_) | ExprKind::Float(_) | ExprKind::Var(_) => {}
            }
        }
        let mut strings = false;
        walk(&p.body, &mut strings);
        for d in &p.decls {
            match d {
                Decl::Let { value, .. } => walk(value, &mut strings),
                Decl::Type { ctors, .. } => {
                    if ctors.iter().any(|c| !c.args.is_empty()) {
                        self.needs_stdlib = true;
                    }
                }
                Decl::External(_) | Decl::Bring(_) => {}
            }
        }
        self.needs_string = strings;
    }

    fn seed_types(&mut self) {
        for c in &mut self.constructors {
            for (i, t) in c.args.iter().enumerate() {
                let r = rep_from_ty(t, &self.type_names);
                if !matches!(r, Rep::Unknown) {
                    c.fields[i] = r;
                }
            }
        }
    }

    fn discover(&mut self, e: &Expr, visible: &HashSet<String>) {
        match &e.kind {
            ExprKind::Fn { params, body } => {
                let id = if let Some(id) = self.lambda_by_span.get(&(e.span.lo, e.span.hi)).copied()
                {
                    id
                } else {
                    let mut outer = visible.clone();
                    for p in params {
                        outer.remove(p);
                    }
                    self.new_function(e, None, &outer)
                };
                let mut nested = visible.clone();
                for p in params {
                    nested.insert(p.clone());
                }
                let _ = id;
                self.discover(body, &nested);
            }
            ExprKind::Let { name, value, body } => {
                self.discover(value, visible);
                let mut next = visible.clone();
                next.insert(name.clone());
                self.discover(body, &next);
            }
            ExprKind::App { fun, args } => {
                self.discover(fun, visible);
                for a in args {
                    self.discover(a, visible);
                }
            }
            ExprKind::Case { scrut, arms } => {
                self.discover(scrut, visible);
                for arm in arms {
                    let mut next = visible.clone();
                    for n in pat_names(&arm.pat) {
                        next.insert(n);
                    }
                    self.discover(&arm.body, &next);
                }
            }
            ExprKind::Block { stmts, tail } => {
                for s in stmts {
                    self.discover(s, visible);
                }
                self.discover(tail, visible);
            }
            ExprKind::Bin { l, r, .. } => {
                self.discover(l, visible);
                self.discover(r, visible);
            }
            ExprKind::Un { e, .. } | ExprKind::Quote(e) => self.discover(e, visible),
            ExprKind::Int(_) | ExprKind::Float(_) | ExprKind::Str(_) | ExprKind::Var(_) => {}
        }
    }

    fn new_function(
        &mut self,
        e: &Expr,
        source_name: Option<String>,
        outer: &HashSet<String>,
    ) -> usize {
        let id = self.functions.len();
        let (params, body) = match &e.kind {
            ExprKind::Fn { params, body } => (params.clone(), body.clone()),
            _ => return id,
        };
        let captures = if source_name.is_some() {
            Vec::new()
        } else {
            collect_captures(&body, outer, &params, &self.globals, &self.type_names)
        };
        self.lambda_by_span.insert((e.span.lo, e.span.hi), id);
        self.functions.push(FunctionInfo {
            id,
            c_name: format!("mak_ir_fn_{}", id),
            source_name,
            expr: Expr {
                span: e.span,
                kind: ExprKind::Fn {
                    params: params.clone(),
                    body: body.clone(),
                },
            },
            params: params.clone(),
            captures: captures.clone(),
            param_reps: vec![Rep::Unknown; params.len()],
            capture_reps: vec![Rep::Unknown; captures.len()],
            ret_rep: Rep::Unknown,
            pure: false,
        });
        id
    }

    fn resolve_function_name(&self, name: &str) -> Option<usize> {
        self.function_by_source
            .get(name)
            .copied()
            .or_else(|| self.recovered_function_aliases.get(name).copied())
    }

    fn resolve_ctor_name(&self, name: &str) -> Option<usize> {
        let exact = self
            .constructors
            .iter()
            .filter(|c| c.name == name)
            .map(|c| c.id)
            .collect::<Vec<_>>();
        match exact.len() {
            1 => exact.first().copied(),
            n if n > 1 => None,
            _ => self.recovered_ctor_aliases.get(name).copied(),
        }
    }

    fn last_resort_function(
        &self,
        name: &str,
        arity: usize,
        current: Option<usize>,
    ) -> Option<usize> {
        for (n, id) in &self.function_by_source {
            if n.eq_ignore_ascii_case(name)
                && self.functions.get(*id).map(|f| f.params.len()) == Some(arity)
            {
                return Some(*id);
            }
        }
        for (n, id) in &self.recovered_function_aliases {
            if n.eq_ignore_ascii_case(name)
                && self.functions.get(*id).map(|f| f.params.len()) == Some(arity)
            {
                return Some(*id);
            }
        }
        let mut found = None;
        for f in &self.functions {
            if Some(f.id) == current {
                continue;
            }
            if f.params.len() != arity {
                continue;
            }
            if found.is_some() {
                return None;
            }
            found = Some(f.id);
        }
        found
    }

    fn best_function_recovery(
        &self,
        name: &str,
        arity: usize,
        args: &[Rep],
        _current: Option<usize>,
    ) -> Option<usize> {
        let candidates = self
            .functions
            .iter()
            .filter(|f| f.params.len() == arity && f.captures.is_empty())
            .collect::<Vec<_>>();
        if candidates.is_empty() {
            return None;
        }
        let exact_name = candidates
            .iter()
            .filter(|f| f.source_name.as_deref() == Some(name))
            .map(|f| f.id)
            .collect::<Vec<_>>();
        if exact_name.len() == 1 {
            return exact_name.first().copied();
        }
        let folded = candidates
            .iter()
            .filter(|f| {
                f.source_name
                    .as_deref()
                    .map(|n| n.eq_ignore_ascii_case(name))
                    .unwrap_or(false)
            })
            .map(|f| f.id)
            .collect::<Vec<_>>();
        if folded.len() == 1 {
            return folded.first().copied();
        }
        let normalized = canonical_ident(name);
        if !normalized.is_empty() {
            let normalized_name = candidates
                .iter()
                .filter(|f| {
                    f.source_name
                        .as_deref()
                        .map(|n| canonical_ident(n) == normalized)
                        .unwrap_or(false)
                })
                .map(|f| f.id)
                .collect::<Vec<_>>();
            if normalized_name.len() == 1 {
                return normalized_name.first().copied();
            }
        }
        let exact_shape = candidates
            .iter()
            .filter(|f| {
                f.param_reps.len() == arity
                    && args
                        .iter()
                        .zip(&f.param_reps)
                        .all(|(a, p)| matches!(a, Rep::Unknown) || a == p)
            })
            .map(|f| f.id)
            .collect::<Vec<_>>();
        if exact_shape.len() == 1 {
            return exact_shape.first().copied();
        }
        let compatible = candidates
            .iter()
            .filter(|f| {
                f.param_reps.len() == arity
                    && args
                        .iter()
                        .zip(&f.param_reps)
                        .all(|(a, p)| rep_compatible(a, p))
            })
            .map(|f| f.id)
            .collect::<Vec<_>>();
        if compatible.len() == 1 && args.iter().all(|r| !matches!(r, Rep::Unknown)) {
            return compatible.first().copied();
        }
        if candidates.len() == 1 {
            return candidates.first().map(|f| f.id);
        }
        None
    }

    fn runtime_function_candidates(&self, arity: usize) -> Vec<usize> {
        self.functions
            .iter()
            .filter(|f| f.params.len() == arity && f.captures.is_empty())
            .map(|f| f.id)
            .collect()
    }

    fn runtime_ctor_candidates(&self, arity: usize) -> Vec<usize> {
        self.constructors
            .iter()
            .filter(|c| c.arity == arity)
            .map(|c| c.id)
            .collect()
    }

    fn runtime_ctor_rep(&self, name: &str, arity: usize) -> Rep {
        let candidates = self
            .constructors
            .iter()
            .filter(|c| c.arity == arity)
            .collect::<Vec<_>>();
        if candidates.is_empty() {
            return Rep::Ptr;
        }
        let exact = candidates
            .iter()
            .filter(|c| c.name == name)
            .copied()
            .collect::<Vec<_>>();
        if exact.len() == 1 {
            return Rep::Adt(exact[0].type_name.clone());
        }
        let folded = candidates
            .iter()
            .filter(|c| c.name.eq_ignore_ascii_case(name))
            .copied()
            .collect::<Vec<_>>();
        if folded.len() == 1 {
            return Rep::Adt(folded[0].type_name.clone());
        }
        let ty = candidates[0].type_name.clone();
        if candidates.iter().all(|c| c.type_name == ty) {
            Rep::Adt(ty)
        } else {
            Rep::Ptr
        }
    }

    fn runtime_function_rep(&self, name: &str, arity: usize, args: &[Rep]) -> Rep {
        let mut candidates = self
            .functions
            .iter()
            .filter(|f| f.params.len() == arity && f.captures.is_empty())
            .collect::<Vec<_>>();
        if candidates.is_empty() {
            return Rep::Unknown;
        }
        let exact = candidates
            .iter()
            .filter(|f| f.source_name.as_deref() == Some(name))
            .copied()
            .collect::<Vec<_>>();
        if exact.len() == 1 {
            return exact[0].ret_rep.clone();
        }
        candidates.retain(|f| {
            f.param_reps
                .iter()
                .zip(args.iter())
                .all(|(p, a)| rep_compatible(a, p))
        });
        if candidates.is_empty() {
            return Rep::Unknown;
        }
        let first = candidates[0].ret_rep.clone();
        if candidates.iter().all(|f| f.ret_rep == first) {
            first
        } else {
            Rep::Unknown
        }
    }

    fn best_ctor_recovery(&self, name: &str, arity: usize, args: &[Rep]) -> Option<usize> {
        let candidates = self
            .constructors
            .iter()
            .filter(|c| c.arity == arity)
            .collect::<Vec<_>>();
        if candidates.is_empty() {
            return None;
        }
        let exact_name = candidates
            .iter()
            .filter(|c| c.name == name)
            .map(|c| c.id)
            .collect::<Vec<_>>();
        if exact_name.len() == 1 {
            return exact_name.first().copied();
        }
        let folded = candidates
            .iter()
            .filter(|c| c.name.eq_ignore_ascii_case(name))
            .map(|c| c.id)
            .collect::<Vec<_>>();
        if folded.len() == 1 {
            return folded.first().copied();
        }
        let normalized = canonical_ident(name);
        if !normalized.is_empty() {
            let normalized_name = candidates
                .iter()
                .filter(|c| canonical_ident(&c.name) == normalized)
                .map(|c| c.id)
                .collect::<Vec<_>>();
            if normalized_name.len() == 1 {
                return normalized_name.first().copied();
            }
        }
        let exact_shape = candidates
            .iter()
            .filter(|c| {
                c.fields.len() == arity
                    && args
                        .iter()
                        .zip(&c.fields)
                        .all(|(a, p)| matches!(a, Rep::Unknown) || a == p)
            })
            .map(|c| c.id)
            .collect::<Vec<_>>();
        if exact_shape.len() == 1 {
            return exact_shape.first().copied();
        }
        let compatible = candidates
            .iter()
            .filter(|c| {
                c.fields.len() == arity
                    && args
                        .iter()
                        .zip(&c.fields)
                        .all(|(a, p)| rep_compatible(a, p))
            })
            .map(|c| c.id)
            .collect::<Vec<_>>();
        if compatible.len() == 1 && args.iter().all(|r| !matches!(r, Rep::Unknown)) {
            return compatible.first().copied();
        }
        if candidates.len() == 1 {
            return candidates.first().map(|c| c.id);
        }
        None
    }

    fn collect_pattern_ctor_names(&mut self, p: &Program) {
        fn pat(p: &Pat, out: &mut HashSet<String>) {
            match p {
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
                Pat::Wild | Pat::Bind(_) | Pat::Int(_) | Pat::Str(_) => {}
            }
        }
        fn expr(e: &Expr, out: &mut HashSet<String>) {
            match &e.kind {
                ExprKind::Case { scrut, arms } => {
                    expr(scrut, out);
                    for arm in arms {
                        pat(&arm.pat, out);
                        expr(&arm.body, out);
                    }
                }
                ExprKind::Let { value, body, .. } => {
                    expr(value, out);
                    expr(body, out);
                }
                ExprKind::Fn { body, .. } => expr(body, out),
                ExprKind::App { fun, args } => {
                    expr(fun, out);
                    for a in args {
                        expr(a, out);
                    }
                }
                ExprKind::Block { stmts, tail } => {
                    for s in stmts {
                        expr(s, out);
                    }
                    expr(tail, out);
                }
                ExprKind::Bin { l, r, .. } => {
                    expr(l, out);
                    expr(r, out);
                }
                ExprKind::Un { e, .. } | ExprKind::Quote(e) => expr(e, out),
                ExprKind::Int(_) | ExprKind::Float(_) | ExprKind::Str(_) | ExprKind::Var(_) => {}
            }
        }
        for d in &p.decls {
            match d {
                Decl::Let { value, .. } => expr(value, &mut self.pattern_ctor_names),
                Decl::External(_) | Decl::Type { .. } | Decl::Bring(_) => {}
            }
        }
        expr(&p.body, &mut self.pattern_ctor_names);
    }

    fn unique_ctor_from_pattern(&self, name: &str, arity: usize) -> Option<usize> {
        if !self.pattern_ctor_names.contains(name) {
            return None;
        }
        if let Some(cid) = self.resolve_ctor_name(name) {
            if self.constructors[cid].arity == arity {
                return Some(cid);
            }
            return None;
        }
        let mut found = None;
        for c in &self.constructors {
            if c.arity == arity {
                if found.is_some() {
                    return None;
                }
                found = Some(c.id);
            }
        }
        found
    }

    fn shape_self_call(&self, name: &str, arity: usize, current: usize) -> Option<usize> {
        let f = self.functions.get(current)?;
        if f.params.len() != arity {
            return None;
        }
        if self.resolve_function_name(name).is_some()
            || self.external_by_name.contains_key(name)
            || self.resolve_ctor_name(name).is_some()
            || self.pattern_ctor_names.contains(name)
        {
            return None;
        }
        Some(current)
    }

    fn unique_ctor_from_arity(&self, arity: usize) -> Option<usize> {
        let mut found = None;
        for c in &self.constructors {
            if c.arity != arity {
                continue;
            }
            if found.is_some() {
                return None;
            }
            found = Some(c.id);
        }
        found
    }

    #[inline]
    fn global_int_const(&self, name: &str) -> Option<i128> {
        self.int_consts.get(name).copied()
    }

    fn constructor_for_pattern(&self, name: &str, arity: usize, scrut: &Rep) -> Option<usize> {
        let expected_type = match scrut {
            Rep::Adt(n) => Some(n.as_str()),
            _ => None,
        };
        let mut exact = self
            .constructors
            .iter()
            .filter(|c| c.arity == arity && c.name == name)
            .collect::<Vec<_>>();
        if let Some(t) = expected_type {
            exact.retain(|c| c.type_name == t);
        }
        if exact.len() == 1 {
            return exact.first().map(|c| c.id);
        }
        if exact.len() > 1 {
            return None;
        }
        let folded_name = name.to_ascii_lowercase();
        let mut folded = self
            .constructors
            .iter()
            .filter(|c| c.arity == arity && c.name.to_ascii_lowercase() == folded_name)
            .collect::<Vec<_>>();
        if let Some(t) = expected_type {
            folded.retain(|c| c.type_name == t);
        }
        if folded.len() == 1 {
            return folded.first().map(|c| c.id);
        }
        if folded.len() > 1 {
            return None;
        }
        let normalized = canonical_ident(name);
        if !normalized.is_empty() {
            let mut canonical = self
                .constructors
                .iter()
                .filter(|c| c.arity == arity && canonical_ident(&c.name) == normalized)
                .collect::<Vec<_>>();
            if let Some(t) = expected_type {
                canonical.retain(|c| c.type_name == t);
            }
            if canonical.len() == 1 {
                return canonical.first().map(|c| c.id);
            }
            if canonical.len() > 1 {
                return None;
            }
        }
        if let Some(t) = expected_type {
            let by_type = self
                .constructors
                .iter()
                .filter(|c| c.type_name == t && c.arity == arity)
                .collect::<Vec<_>>();
            if by_type.len() == 1 {
                return by_type.first().map(|c| c.id);
            }
            if by_type.len() > 1 {
                return None;
            }
        }
        self.recovered_ctor_aliases
            .get(name)
            .copied()
            .filter(|cid| *cid < self.constructors.len() && self.constructors[*cid].arity == arity)
    }

    fn recover_symbol_aliases(&mut self, p: &Program) {
        let mut pattern_ctor_names = HashSet::<String>::new();
        fn collect_pattern_names(p: &Pat, out: &mut HashSet<String>) {
            match p {
                Pat::Ctor { name, args } => {
                    out.insert(name.clone());
                    for a in args {
                        collect_pattern_names(a, out);
                    }
                }
                Pat::Tuple(xs) => {
                    for x in xs {
                        collect_pattern_names(x, out);
                    }
                }
                Pat::Wild | Pat::Bind(_) | Pat::Int(_) | Pat::Str(_) => {}
            }
        }
        fn collect_expr_pattern_names(e: &Expr, out: &mut HashSet<String>) {
            match &e.kind {
                ExprKind::Case { scrut, arms } => {
                    collect_expr_pattern_names(scrut, out);
                    for arm in arms {
                        collect_pattern_names(&arm.pat, out);
                        collect_expr_pattern_names(&arm.body, out);
                    }
                }
                ExprKind::Let { value, body, .. } => {
                    collect_expr_pattern_names(value, out);
                    collect_expr_pattern_names(body, out);
                }
                ExprKind::Fn { body, .. } => collect_expr_pattern_names(body, out),
                ExprKind::App { fun, args } => {
                    collect_expr_pattern_names(fun, out);
                    for a in args {
                        collect_expr_pattern_names(a, out);
                    }
                }
                ExprKind::Block { stmts, tail } => {
                    for x in stmts {
                        collect_expr_pattern_names(x, out);
                    }
                    collect_expr_pattern_names(tail, out);
                }
                ExprKind::Bin { l, r, .. } => {
                    collect_expr_pattern_names(l, out);
                    collect_expr_pattern_names(r, out);
                }
                ExprKind::Un { e, .. } | ExprKind::Quote(e) => collect_expr_pattern_names(e, out),
                ExprKind::Int(_) | ExprKind::Float(_) | ExprKind::Str(_) | ExprKind::Var(_) => {}
            }
        }
        for f in &self.functions {
            if let ExprKind::Fn { body, .. } = &f.expr.kind {
                collect_expr_pattern_names(body, &mut pattern_ctor_names);
            }
        }
        collect_expr_pattern_names(&p.body, &mut pattern_ctor_names);
        let mut ctor_by_arity: BTreeMap<usize, Vec<usize>> = BTreeMap::new();
        for c in &self.constructors {
            ctor_by_arity.entry(c.arity).or_default().push(c.id);
        }
        fn walk_expr(
            a: &mut Analyzer,
            e: &Expr,
            current: Option<usize>,
            ctor_by_arity: &BTreeMap<usize, Vec<usize>>,
            pattern_ctor_names: &HashSet<String>,
        ) {
            match &e.kind {
                ExprKind::App { fun, args } => {
                    let (base, all_args) = Analyzer::flatten_application(fun, args);
                    if let ExprKind::Var(name) = &base.kind {
                        let arity = all_args.len();
                        let known = a.resolve_function_name(name).is_some()
                            || a.external_by_name.contains_key(name)
                            || a.resolve_ctor_name(name).is_some();
                        if !known && pattern_ctor_names.contains(name) {
                            if let Some(cands) = ctor_by_arity.get(&arity) {
                                if cands.len() == 1 {
                                    a.recovered_ctor_aliases.insert(name.clone(), cands[0]);
                                }
                            }
                        }
                    }
                    walk_expr(a, fun, current, ctor_by_arity, pattern_ctor_names);
                    for x in args {
                        walk_expr(a, x, current, ctor_by_arity, pattern_ctor_names);
                    }
                }
                ExprKind::Let { value, body, .. } => {
                    walk_expr(a, value, current, ctor_by_arity, pattern_ctor_names);
                    walk_expr(a, body, current, ctor_by_arity, pattern_ctor_names);
                }
                ExprKind::Fn { body, .. } => {
                    walk_expr(a, body, current, ctor_by_arity, pattern_ctor_names)
                }
                ExprKind::Case { scrut, arms } => {
                    walk_expr(a, scrut, current, ctor_by_arity, pattern_ctor_names);
                    for arm in arms {
                        if let Pat::Ctor { name, args } = &arm.pat {
                            if a.resolve_ctor_name(name).is_none() {
                                let pat_arity = args.len();
                                if let Some(cands) = ctor_by_arity.get(&pat_arity) {
                                    if cands.len() == 1 {
                                        a.recovered_ctor_aliases.insert(name.clone(), cands[0]);
                                    }
                                }
                            }
                        }
                        walk_expr(a, &arm.body, current, ctor_by_arity, pattern_ctor_names);
                    }
                }
                ExprKind::Block { stmts, tail } => {
                    for x in stmts {
                        walk_expr(a, x, current, ctor_by_arity, pattern_ctor_names);
                    }
                    walk_expr(a, tail, current, ctor_by_arity, pattern_ctor_names);
                }
                ExprKind::Bin { l, r, .. } => {
                    walk_expr(a, l, current, ctor_by_arity, pattern_ctor_names);
                    walk_expr(a, r, current, ctor_by_arity, pattern_ctor_names);
                }
                ExprKind::Un { e, .. } | ExprKind::Quote(e) => {
                    walk_expr(a, e, current, ctor_by_arity, pattern_ctor_names)
                }
                ExprKind::Int(_) | ExprKind::Float(_) | ExprKind::Str(_) | ExprKind::Var(_) => {}
            }
        }
        for id in 0..self.functions.len() {
            let body = match &self.functions[id].expr.kind {
                ExprKind::Fn { body, .. } => body.clone(),
                _ => continue,
            };
            walk_expr(self, &body, Some(id), &ctor_by_arity, &pattern_ctor_names);
        }
        walk_expr(self, &p.body, None, &ctor_by_arity, &pattern_ctor_names);
    }

    fn seed_parameter_shapes(&mut self) {
        fn walk(a: &Analyzer, e: &Expr, params: &HashMap<String, usize>, out: &mut [Rep]) {
            match &e.kind {
                ExprKind::Case { scrut, arms } => {
                    if let ExprKind::Var(name) = &scrut.kind {
                        if let Some(&idx) = params.get(name) {
                            let mut shape = Rep::Unknown;
                            for arm in arms {
                                if let Some(r) = a.pattern_shape(&arm.pat) {
                                    shape = merge_rep(shape, r);
                                }
                            }
                            if !matches!(shape, Rep::Unknown) {
                                out[idx] = shape;
                            }
                        }
                    }
                    walk(a, scrut, params, out);
                    for arm in arms {
                        walk(a, &arm.body, params, out);
                    }
                }
                ExprKind::Let { value, body, .. } => {
                    walk(a, value, params, out);
                    walk(a, body, params, out);
                }
                ExprKind::Fn { body, .. } => walk(a, body, params, out),
                ExprKind::App { fun, args } => {
                    walk(a, fun, params, out);
                    for arg in args {
                        walk(a, arg, params, out);
                    }
                }
                ExprKind::Block { stmts, tail } => {
                    for stmt in stmts {
                        walk(a, stmt, params, out);
                    }
                    walk(a, tail, params, out);
                }
                ExprKind::Bin { l, r, .. } => {
                    walk(a, l, params, out);
                    walk(a, r, params, out);
                }
                ExprKind::Un { e, .. } | ExprKind::Quote(e) => walk(a, e, params, out),
                ExprKind::Int(_) | ExprKind::Float(_) | ExprKind::Str(_) | ExprKind::Var(_) => {}
            }
        }
        let mut updates = Vec::with_capacity(self.functions.len());
        for f in &self.functions {
            let body = match &f.expr.kind {
                ExprKind::Fn { body, .. } => body.clone(),
                _ => continue,
            };
            let params = f
                .params
                .iter()
                .enumerate()
                .map(|(i, n)| (n.clone(), i))
                .collect::<HashMap<_, _>>();
            let mut reps = f.param_reps.clone();
            walk(self, &body, &params, &mut reps);
            updates.push((f.id, reps));
        }
        for (id, reps) in updates {
            if let Some(f) = self.functions.get_mut(id) {
                f.param_reps = reps;
            }
        }
    }

    fn solve(&mut self) {
        self.seed_parameter_shapes();
        for _round in 0..24 {
            self.seed_parameter_shapes();
            let mut changed = false;
            let old_constructor_fields = self
                .constructors
                .iter()
                .map(|c| c.fields.clone())
                .collect::<Vec<_>>();
            for id in 0..self.functions.len() {
                let old = self.functions[id].clone();
                let body = match &old.expr.kind {
                    ExprKind::Fn { body, .. } => body.clone(),
                    _ => Box::new(Expr {
                        span: old.expr.span,
                        kind: ExprKind::Int(0),
                    }),
                };
                let mut env = HashMap::new();
                for (n, r) in old.captures.iter().zip(old.capture_reps.iter()) {
                    env.insert(n.clone(), r.clone());
                }
                for (n, r) in old.params.iter().zip(old.param_reps.iter()) {
                    env.insert(n.clone(), r.clone());
                }
                let inferred = self.infer_expr(&body, &mut env, id);
                let mut next = self.functions[id].clone();
                next.ret_rep = refine_rep(next.ret_rep.clone(), inferred);
                for (i, n) in next.params.iter().enumerate() {
                    if let Some(r) = env.get(n) {
                        next.param_reps[i] = refine_rep(next.param_reps[i].clone(), r.clone());
                    }
                }
                for (i, n) in next.captures.iter().enumerate() {
                    if let Some(r) = env.get(n) {
                        next.capture_reps[i] = refine_rep(next.capture_reps[i].clone(), r.clone());
                    }
                }
                next.pure = self.ast_pure(&body, id);
                if next.ret_rep != old.ret_rep
                    || next.param_reps != old.param_reps
                    || next.capture_reps != old.capture_reps
                    || next.pure != old.pure
                {
                    changed = true;
                }
                self.functions[id] = next;
            }
            if self
                .constructors
                .iter()
                .zip(old_constructor_fields.iter())
                .any(|(c, old)| c.fields != *old)
            {
                changed = true;
            }
            if !changed {
                break;
            }
        }
        self.seed_parameter_shapes();


        self.force_indexed_parameter_shapes();
        self.dbg("THINK", "Representation inference converged with syntax-derived parameter shapes and bounded propagation.");
        for id in 0..self.functions.len() {
            let body = match &self.functions[id].expr.kind {
                ExprKind::Fn { body, .. } => body,
                _ => continue,
            };
            if let Some(shape) = self.constructor_result_shape(body) {
                if !matches!(shape, Rep::Unknown) {
                    let old = self.functions[id].ret_rep.clone();
                    let new_rep = match (old.clone(), shape.clone()) {
                        (Rep::F64, s @ Rep::Adt(_)) | (Rep::F32, s @ Rep::Adt(_)) => s,
                        (o, s) => observe_rep(o, s),
                    };
                    self.functions[id].ret_rep = new_rep.clone();
                    self.dbg(
                        "INVARIANT",
                        format!(
                            "f{} return forced by concrete constructor result: {} -> {}",
                            id,
                            rep_debug(&old),
                            rep_debug(&new_rep)
                        ),
                    );
                }
            }
        }
        let mut fallback_ret_shapes: Vec<Option<Rep>> = Vec::with_capacity(self.functions.len());
        for f in &self.functions {
            let shape = if matches!(f.ret_rep, Rep::Unknown) {
                match &f.expr.kind {
                    ExprKind::Fn { body, .. } => self.constructor_result_shape(body),
                    _ => None,
                }
            } else {
                None
            };
            fallback_ret_shapes.push(shape);
        }
        let debug = self.debug;
        for (fid, f) in self.functions.iter_mut().enumerate() {
            for r in &mut f.param_reps {
                if matches!(r, Rep::Unknown) {
                    *r = Rep::I64;
                }
            }
            for r in &mut f.capture_reps {
                if matches!(r, Rep::Unknown) {
                    *r = Rep::I64;
                }
            }
            if matches!(f.ret_rep, Rep::Unknown) {
                if let Some(shape) = fallback_ret_shapes.get(fid).and_then(|x| x.clone()) {
                    if debug {
                        eprintln!(
                            "[MAK:CGEN:{:<12}] f{} return was Unknown; syntax-shape chose {}",
                            "INFER",
                            fid,
                            rep_debug(&shape)
                        );
                    }
                    f.ret_rep = shape;
                }
                if matches!(f.ret_rep, Rep::Unknown) {
                    let fallback = if f.params.is_empty() {
                        Rep::Unit
                    } else {
                        Rep::I64
                    };
                    if debug {
                        eprintln!(
                            "[MAK:CGEN:{:<12}] f{} return still Unknown; fallback chose {} because params={}",
                            "INFER",
                            fid,
                            rep_debug(&fallback),
                            f.params.len()
                        );
                    }
                    f.ret_rep = fallback;
                }
            }
        }
        for c in &mut self.constructors {
            for i in 0..c.fields.len() {
                c.fields[i] = rep_from_ty(&c.args[i], &self.type_names);
            }
        }
        for f in &self.functions {
            debug_assert!(f.param_reps.iter().all(|r| !matches!(r, Rep::Unknown)));
            debug_assert!(f.capture_reps.iter().all(|r| !matches!(r, Rep::Unknown)));
            debug_assert!(!matches!(f.ret_rep, Rep::Unknown));
            self.dbg(
                "FN",
                format!(
                    "f{} {}({}) -> {}",
                    f.id,
                    f.source_name.as_deref().unwrap_or("<lambda>"),
                    f.param_reps
                        .iter()
                        .enumerate()
                        .map(|(i, r)| format!(
                            "{}:{}",
                            f.params.get(i).map(String::as_str).unwrap_or("?"),
                            rep_debug(r)
                        ))
                        .collect::<Vec<_>>()
                        .join(","),
                    rep_debug(&f.ret_rep)
                ),
            );
        }
        self.refresh_global_reps_from_functions();
        self.compute_call_graph_facts();
    }

    fn refresh_global_reps_from_functions(&mut self) {
        let names: Vec<(String, usize)> = self
            .function_by_source
            .iter()
            .map(|(n, id)| (n.clone(), *id))
            .collect();
        for (name, id) in names {
            self.global_reps.insert(name, Rep::Function(id));
        }
    }

    fn compute_call_graph_facts(&mut self) {
        let n = self.functions.len();
        let mut graph = vec![HashSet::<usize>::new(); n];
        fn walk(e: &Expr, graph: &mut HashSet<usize>, names: &HashMap<String, usize>) {
            match &e.kind {
                ExprKind::App { fun, args } => {
                    let (base, all_args) = Analyzer::flatten_application(fun, args);
                    if let ExprKind::Var(name) = &base.kind {
                        if let Some(id) = names.get(name) {
                            graph.insert(*id);
                        }
                    }
                    walk(fun, graph, names);
                    for a in all_args {
                        walk(a, graph, names);
                    }
                }
                ExprKind::Let { value, body, .. } => {
                    walk(value, graph, names);
                    walk(body, graph, names);
                }
                ExprKind::Fn { body, .. } => walk(body, graph, names),
                ExprKind::Case { scrut, arms } => {
                    walk(scrut, graph, names);
                    for arm in arms {
                        walk(&arm.body, graph, names);
                    }
                }
                ExprKind::Block { stmts, tail } => {
                    for s in stmts {
                        walk(s, graph, names);
                    }
                    walk(tail, graph, names);
                }
                ExprKind::Bin { l, r, .. } => {
                    walk(l, graph, names);
                    walk(r, graph, names);
                }
                ExprKind::Un { e, .. } | ExprKind::Quote(e) => walk(e, graph, names),
                ExprKind::Int(_) | ExprKind::Float(_) | ExprKind::Str(_) | ExprKind::Var(_) => {}
            }
        }
        for id in 0..n {
            if let ExprKind::Fn { body, .. } = &self.functions[id].expr.kind {
                walk(body, &mut graph[id], &self.function_by_source);
            }
        }
        for id in 0..n {
            let mut stack = graph[id].iter().copied().collect::<Vec<_>>();
            let mut seen = HashSet::new();
            while let Some(x) = stack.pop() {
                if !seen.insert(x) {
                    continue;
                }
                if graph[x].contains(&id) {
                    self.recursive_functions.insert(id);
                    break;
                }
                stack.extend(graph[x].iter().copied());
            }
            if graph[id].contains(&id) {
                self.recursive_functions.insert(id);
            }
        }
        self.reachable_functions.extend(0..n);
    }

    fn force_indexed_parameter_shapes(&mut self) {
        fn walk(e: &Expr, param_index: &HashMap<String, usize>, indexed: &mut HashSet<usize>) {
            match &e.kind {
                ExprKind::App { fun, args } => {
                    let (base, flat_args) = Analyzer::flatten_application(fun, args);
                    if matches!(&base.kind, ExprKind::Var(name) if name == "$mak.index")
                        && flat_args.len() == 2
                    {
                        if let ExprKind::Var(name) = &flat_args[0].kind {
                            if let Some(index) = param_index.get(name) {
                                indexed.insert(*index);
                            }
                        }
                    }
                    walk(fun, param_index, indexed);
                    for arg in args {
                        walk(arg, param_index, indexed);
                    }
                }
                ExprKind::Let { value, body, .. } => {
                    walk(value, param_index, indexed);
                    walk(body, param_index, indexed);
                }
                ExprKind::Fn { body, .. } => walk(body, param_index, indexed),
                ExprKind::Case { scrut, arms } => {
                    walk(scrut, param_index, indexed);
                    for arm in arms {
                        walk(&arm.body, param_index, indexed);
                    }
                }
                ExprKind::Block { stmts, tail } => {
                    for stmt in stmts {
                        walk(stmt, param_index, indexed);
                    }
                    walk(tail, param_index, indexed);
                }
                ExprKind::Bin { l, r, .. } => {
                    walk(l, param_index, indexed);
                    walk(r, param_index, indexed);
                }
                ExprKind::Un { e, .. } | ExprKind::Quote(e) => walk(e, param_index, indexed),
                ExprKind::Int(_) | ExprKind::Float(_) | ExprKind::Str(_) | ExprKind::Var(_) => {}
            }
        }

        let mut forced = Vec::<(usize, usize)>::new();
        for (id, function) in self.functions.iter().enumerate() {
            let body = match &function.expr.kind {
                ExprKind::Fn { body, .. } => body,
                _ => continue,
            };
            let param_index = function
                .params
                .iter()
                .enumerate()
                .map(|(i, name)| (name.clone(), i))
                .collect::<HashMap<_, _>>();
            let mut indexed = HashSet::new();
            walk(body, &param_index, &mut indexed);
            for index in indexed {
                forced.push((id, index));
            }
        }

        for (fid, index) in forced {
            if let Some(rep) = self
                .functions
                .get_mut(fid)
                .and_then(|f| f.param_reps.get_mut(index))
            {


                if !matches!(rep, Rep::Ptr | Rep::Str) {
                    *rep = Rep::Ptr;
                }
            }
        }
    }

    fn constructor_result_shape(&self, e: &Expr) -> Option<Rep> {
        fn go(a: &Analyzer, e: &Expr) -> Option<Rep> {
            match &e.kind {
                ExprKind::App { fun, args } => {
                    let (base, all_args) = Analyzer::flatten_application(fun, args);
                    if let ExprKind::Var(n) = &base.kind {
                        if a.global_int_const(n).is_some() {
                            return None;
                        }
                        if let Some(cid) = a.resolve_ctor_name(n) {
                            if all_args.len() == a.constructors[cid].arity {
                                return Some(Rep::Adt(a.constructors[cid].type_name.clone()));
                            }
                        }
                        if let Some(cid) = a.unique_ctor_from_arity(all_args.len()) {
                            if a.pattern_ctor_names.contains(n) {
                                return Some(Rep::Adt(a.constructors[cid].type_name.clone()));
                            }
                        }
                    }
                    None
                }
                ExprKind::Case { arms, .. } => {
                    let mut found = None;
                    for arm in arms {
                        if let Some(r) = go(a, &arm.body) {
                            found = Some(match found {
                                None => r,
                                Some(old) => merge_rep(old, r),
                            });
                        }
                    }
                    found
                }
                ExprKind::Block { tail, .. } => go(a, tail),
                ExprKind::Let { body, .. } => go(a, body),
                _ => None,
            }
        }
        go(self, e)
    }

    fn pattern_shape(&self, pat: &Pat) -> Option<Rep> {
        match pat {
            Pat::Ctor { name, args } => {
                if args.is_empty() {
                    if self.global_int_const(name).is_some() {
                        return Some(Rep::I64);
                    }
                }
                self.resolve_ctor_name(name).and_then(|cid| {
                    if args.len() == self.constructors[cid].arity {
                        Some(Rep::Adt(self.constructors[cid].type_name.clone()))
                    } else {
                        None
                    }
                })
            }
            Pat::Int(_) => Some(Rep::I64),
            Pat::Str(_) => Some(Rep::Str),
            Pat::Tuple(_) => Some(Rep::Ptr),
            Pat::Wild | Pat::Bind(_) => None,
        }
    }

    fn constrain_expr_rep(
        &mut self,
        e: &Expr,
        expected: &Rep,
        env: &mut HashMap<String, Rep>,
        current: usize,
    ) {
        if matches!(expected, Rep::Unknown) {
            return;
        }
        match &e.kind {
            ExprKind::Var(name) => {
                if let Some(old) = env.get(name).cloned() {
                    env.insert(name.clone(), refine_rep(old, expected.clone()));
                }
            }
            ExprKind::App { fun, args } => {
                let (base, all_args) = Self::flatten_application(fun, args);
                let target = if let ExprKind::Var(name) = &base.kind {
                    if let Some(fid) = self.resolve_function_name(name) {
                        (all_args.len() == self.functions[fid].params.len()).then_some(fid)
                    } else {
                        self.shape_self_call(name, all_args.len(), current)
                    }
                } else {
                    match self.infer_expr(fun, env, current) {
                        Rep::Function(fid) if fid < self.functions.len() => {
                            (all_args.len() == self.functions[fid].params.len()).then_some(fid)
                        }
                        _ => None,
                    }
                };
                if let Some(fid) = target {
                    let params = self.functions[fid].param_reps.clone();
                    self.functions[fid].ret_rep =
                        refine_rep(self.functions[fid].ret_rep.clone(), expected.clone());
                    for (i, arg) in all_args.iter().enumerate() {
                        if let Some(param_rep) = params.get(i) {
                            let observed = self.infer_expr(arg, env, current);
                            let resolved = if matches!(param_rep, Rep::Unknown) {
                                observed
                            } else {
                                param_rep.clone()
                            };
                            self.constrain_expr_rep(arg, &resolved, env, current);
                            if let ExprKind::Var(name) = &arg.kind {
                                if let Some(old) = env.get(name).cloned() {
                                    env.insert(name.clone(), refine_rep(old, resolved));
                                }
                            }
                        }
                    }
                }
            }
            ExprKind::Fn { .. } => {}
            ExprKind::Let { body, .. } => {
                self.constrain_expr_rep(body, expected, env, current);
            }
            ExprKind::Case { arms, .. } => {
                for arm in arms {
                    let mut local = env.clone();
                    self.constrain_expr_rep(&arm.body, expected, &mut local, current);
                }
            }
            ExprKind::Block { tail, .. } => {
                self.constrain_expr_rep(tail, expected, env, current);
            }
            ExprKind::Bin { l, r, .. } => {
                if expected.is_integer() || expected.is_float() {
                    self.constrain_expr_rep(l, expected, env, current);
                    self.constrain_expr_rep(r, expected, env, current);
                }
            }
            ExprKind::Un { e, op } => {
                if !matches!(op, UnOp::Not) {
                    self.constrain_expr_rep(e, expected, env, current);
                }
            }
            ExprKind::Quote(e) => self.constrain_expr_rep(e, expected, env, current),
            ExprKind::Int(_) | ExprKind::Float(_) | ExprKind::Str(_) => {}
        }
    }

    fn constrain_scrutinee(
        &mut self,
        scrut: &Expr,
        expected: &Rep,
        env: &mut HashMap<String, Rep>,
        current: usize,
    ) {
        self.constrain_expr_rep(scrut, expected, env, current);
    }

    fn infer_expr(&mut self, e: &Expr, env: &mut HashMap<String, Rep>, current: usize) -> Rep {
        match &e.kind {
            ExprKind::Int(v) => integer_literal_rep(*v),
            ExprKind::Float(_) => Rep::F64,
            ExprKind::Str(_) => Rep::Str,
            ExprKind::Var(n) => {
                if let Some(r) = env.get(n) {
                    return r.clone();
                }
                if let Some(id) = self.resolve_function_name(n) {
                    return Rep::Function(id);
                }
                if let Some(id) = self.external_by_name.get(n) {
                    return foreign_value_rep(&self.externals[*id].decl.foreign);
                }
                if let Some(r) = self.global_reps.get(n) {
                    return r.clone();
                }
                if self.global_int_const(n).is_some() {
                    return Rep::I64;
                }
                if let Some(cid) = self.resolve_ctor_name(n) {
                    let c = &self.constructors[cid];
                    if c.arity == 0 {
                        return Rep::Adt(c.type_name.clone());
                    }
                    return Rep::Unknown;
                }
                Rep::Unknown
            }
            ExprKind::Let { name, value, body } => {
                let r = self.infer_expr(value, env, current);
                let binding = if let ExprKind::Fn { .. } = &value.kind {
                    if let Some(id) = self
                        .lambda_by_span
                        .get(&(value.span.lo, value.span.hi))
                        .copied()
                    {
                        Rep::Function(id)
                    } else {
                        Rep::Unknown
                    }
                } else {
                    r
                };
                let old = env.insert(name.clone(), binding);
                let result = self.infer_expr(body, env, current);
                if let Some(old) = old {
                    env.insert(name.clone(), old);
                } else {
                    env.remove(name);
                }
                result
            }
            ExprKind::Fn { .. } => self
                .lambda_by_span
                .get(&(e.span.lo, e.span.hi))
                .copied()
                .map(Rep::Function)
                .unwrap_or(Rep::Unknown),
            ExprKind::App { fun, args } => self.infer_call(fun, args, env, current),
            ExprKind::Case { scrut, arms } => {
                let mut sr = self.infer_expr(scrut, env, current);
                for arm in arms {
                    if let Some(pr) = self.pattern_shape(&arm.pat) {
                        sr = refine_rep(sr, pr);
                    }
                }
                self.constrain_scrutinee(scrut, &sr, env, current);
                let mut out = Rep::Unknown;
                for arm in arms {
                    let mut local = env.clone();
                    self.constrain_pattern(&arm.pat, &sr, &mut local);
                    out = merge_rep(out, self.infer_expr(&arm.body, &mut local, current));
                }
                self.dbg(
                    "CASE",
                    format!(
                        "f{} scrut={} arms={} => {}",
                        current,
                        rep_debug(&sr),
                        arms.len(),
                        rep_debug(&out)
                    ),
                );
                out
            }
            ExprKind::Block { stmts, tail } => {
                for s in stmts {
                    self.infer_expr(s, env, current);
                }
                self.infer_expr(tail, env, current)
            }
            ExprKind::Bin { op, l, r } => {
                let a = self.infer_expr(l, env, current);
                let b = self.infer_expr(r, env, current);


                if matches!(op, BinOp::Rem) {
                    self.constrain_expr_rep(l, &Rep::I64, env, current);
                    self.constrain_expr_rep(r, &Rep::I64, env, current);
                    return Rep::I64;
                }
                if matches!(
                    op,
                    BinOp::Eq | BinOp::Ne | BinOp::Lt | BinOp::Gt | BinOp::Le | BinOp::Ge
                ) {
                    let common = merge_rep(a, b);
                    if !matches!(common, Rep::Unknown) {
                        self.constrain_expr_rep(l, &common, env, current);
                        self.constrain_expr_rep(r, &common, env, current);
                    }
                    Rep::I32
                } else if matches!(op, BinOp::LAnd | BinOp::LOr) {
                    let truth = if a.is_integer() || b.is_integer() {
                        merge_rep(a, b)
                    } else {
                        Rep::I32
                    };
                    let truth = if matches!(truth, Rep::Unknown) {
                        Rep::I32
                    } else {
                        truth
                    };
                    self.constrain_expr_rep(l, &truth, env, current);
                    self.constrain_expr_rep(r, &truth, env, current);
                    Rep::I32
                } else if matches!(op, BinOp::Add)
                    && ((is_data_pointer_rep(&a) && (b.is_integer() || matches!(b, Rep::Unknown)))
                        || ((a.is_integer() || matches!(a, Rep::Unknown)) && is_data_pointer_rep(&b)))
                {
                    if is_data_pointer_rep(&a) { a } else { b }
                } else if matches!(op, BinOp::Sub)
                    && is_data_pointer_rep(&a)
                    && (b.is_integer() || matches!(b, Rep::Unknown))
                {
                    a
                } else if matches!(op, BinOp::Sub) && is_data_pointer_rep(&a) && is_data_pointer_rep(&b) {
                    Rep::I64
                } else {
                    let both_numeric =
                        (a.is_integer() || a.is_float()) && (b.is_integer() || b.is_float());
                    if !both_numeric {
                        Rep::Unknown
                    } else {
                        let common = if a.is_float() || b.is_float() {
                            Rep::F64
                        } else {
                            merge_rep(a, b)
                        };
                        self.constrain_expr_rep(l, &common, env, current);
                        self.constrain_expr_rep(r, &common, env, current);
                        common
                    }
                }
            }
            ExprKind::Un { op, e } => {
                let r = self.infer_expr(e, env, current);
                match op {
                    UnOp::Not => {
                        self.constrain_expr_rep(e, &Rep::I64, env, current);
                        Rep::I32
                    }
                    UnOp::Neg | UnOp::BitNot => {
                        if matches!(r, Rep::Unknown) {
                            self.constrain_expr_rep(e, &Rep::I64, env, current);
                            Rep::I64
                        } else {
                            r
                        }
                    }
                }
            }
            ExprKind::Quote(x) => self.infer_expr(x, env, current),
        }
    }

    fn infer_call(
        &mut self,
        fun: &Expr,
        args: &[Expr],
        env: &mut HashMap<String, Rep>,
        current: usize,
    ) -> Rep {
        let (base_fun, flat_args) = Self::flatten_application(fun, args);
        if matches!(&base_fun.kind, ExprKind::Var(n) if n == "$mak.index") {
            if flat_args.len() != 2 {
                return Rep::Unknown;
            }


            let mut base_rep = self.infer_expr(flat_args[0], env, current);
            if matches!(base_rep, Rep::Unknown | Rep::Adt(_)) {


                self.constrain_expr_rep(flat_args[0], &Rep::Ptr, env, current);
                if let ExprKind::Var(name) = &flat_args[0].kind {
                    env.insert(name.clone(), Rep::Ptr);
                    let param_index = self
                        .functions
                        .get(current)
                        .and_then(|function| function.params.iter().position(|p| p == name));
                    if let Some(index) = param_index {
                        self.functions[current].param_reps[index] = Rep::Ptr;
                    }
                }
                base_rep = self.infer_expr(flat_args[0], env, current);
            }

            let mut index_rep = self.infer_expr(flat_args[1], env, current);
            if matches!(index_rep, Rep::Unknown) {
                self.constrain_expr_rep(flat_args[1], &Rep::I64, env, current);
                index_rep = self.infer_expr(flat_args[1], env, current);
            }

            if !is_data_pointer_rep(&base_rep) {
                return Rep::Unknown;
            }
            if !index_rep.is_integer() {
                return Rep::Unknown;
            }

            return pointer_info_of_expr(flat_args[0], self, None)
                .map(|info| info.elem_rep)
                .unwrap_or(Rep::U8);
        }
        let mut ars = Vec::with_capacity(flat_args.len());
        for a in &flat_args {
            ars.push(self.infer_expr(a, env, current));
        }

        fn refine_argument(
            this: &mut Analyzer,
            arg_expr: &Expr,
            observed: &Rep,
            expected: &Rep,
            env: &mut HashMap<String, Rep>,
            current: usize,
        ) {
            let resolved = if matches!(expected, Rep::Unknown) {
                observed.clone()
            } else {
                expected.clone()
            };
            this.constrain_expr_rep(arg_expr, &resolved, env, current);
            if let ExprKind::Var(name) = &arg_expr.kind {
                if let Some(old) = env.get(name).cloned() {
                    env.insert(name.clone(), refine_rep(old, resolved));
                }
            }
        }

        if let ExprKind::Var(n) = &base_fun.kind {
            let arity = flat_args.len();
            if let Some(cid) = self.resolve_ctor_name(n) {
                if arity == self.constructors[cid].arity {
                    for i in 0..arity {
                        let expected =
                            rep_from_ty(&self.constructors[cid].args[i], &self.type_names);
                        refine_argument(self, flat_args[i], &ars[i], &expected, env, current);
                    }
                    return Rep::Adt(self.constructors[cid].type_name.clone());
                }
            }
            let fid = if let Some(Rep::Function(fid)) = env.get(n) {
                if *fid < self.functions.len() && arity == self.functions[*fid].params.len() {
                    Some(*fid)
                } else {
                    None
                }
            } else if let Some(Rep::ExternFunction {
                args: fargs,
                variadic,
                ret,
                ..
            }) = env.get(n).cloned()
            {
                if arity >= fargs.len() && (variadic || arity == fargs.len()) {
                    for (i, expected) in fargs.iter().enumerate() {
                        refine_argument(self, &flat_args[i], &ars[i], expected, env, current);
                    }
                    return *ret;
                }
                None
            } else if let Some(fid) = self.resolve_function_name(n) {
                if arity == self.functions[fid].params.len() {
                    Some(fid)
                } else {
                    None
                }
            } else if self
                .functions
                .get(current)
                .and_then(|f| {
                    if f.source_name.as_deref() == Some(n.as_str()) && arity == f.params.len() {
                        Some(current)
                    } else {
                        None
                    }
                })
                .is_some()
            {
                Some(current)
            } else {
                self.best_function_recovery(n, arity, &ars, Some(current))
            };
            if let Some(fid) = fid {
                for i in 0..arity {
                    let expected = self.functions[fid].param_reps[i].clone();
                    refine_argument(self, flat_args[i], &ars[i], &expected, env, current);
                    self.functions[fid].param_reps[i] = observe_rep(expected, ars[i].clone());
                }
                return self.functions[fid].ret_rep.clone();
            }
            if let Some(eid) = self.external_by_name.get(n).copied() {
                let ext = self.externals[eid].clone();
                if let ForeignTy::Fun { args: fargs, ret } = &ext.decl.foreign {
                    for (i, expected) in fargs.iter().enumerate() {
                        if let ForeignArg::Ty(t) = expected {
                            let expected_rep = rep_from_foreign(t);
                            if let Some(arg_expr) = flat_args.get(i) {
                                let observed = ars.get(i).cloned().unwrap_or(Rep::Unknown);
                                refine_argument(
                                    self,
                                    arg_expr,
                                    &observed,
                                    &expected_rep,
                                    env,
                                    current,
                                );
                            }
                        }
                    }
                    return if ext.noreturn {
                        Rep::Unit
                    } else {
                        foreign_ret_rep(ret)
                    };
                }
                return foreign_value_rep(&ext.decl.foreign);
            }
            if let Some(cid) = self.best_ctor_recovery(n, arity, &ars) {
                self.recovered_ctor_aliases.entry(n.clone()).or_insert(cid);
                for i in 0..arity {
                    let expected = rep_from_ty(&self.constructors[cid].args[i], &self.type_names);
                    refine_argument(self, flat_args[i], &ars[i], &expected, env, current);
                }
                return Rep::Adt(self.constructors[cid].type_name.clone());
            }
        }
        match self.infer_expr(fun, env, current) {
            Rep::Function(fid)
                if fid < self.functions.len()
                    && flat_args.len() == self.functions[fid].params.len() =>
            {
                for i in 0..flat_args.len() {
                    let expected = self.functions[fid].param_reps[i].clone();
                    refine_argument(self, flat_args[i], &ars[i], &expected, env, current);
                    self.functions[fid].param_reps[i] = observe_rep(expected, ars[i].clone());
                }
                return self.functions[fid].ret_rep.clone();
            }
            Rep::ExternFunction {
                args: fargs,
                variadic,
                ret,
                ..
            } if flat_args.len() >= fargs.len()
                && (variadic || flat_args.len() == fargs.len()) =>
            {
                for (i, expected) in fargs.iter().enumerate() {
                    refine_argument(self, &flat_args[i], &ars[i], expected, env, current);
                }
                return *ret;
            }
            _ => {}
        }
        Rep::Unknown
    }

    fn constrain_pattern(&mut self, pat: &Pat, scrut: &Rep, env: &mut HashMap<String, Rep>) {
        match pat {
            Pat::Bind(n) => {
                let old = env.get(n).cloned().unwrap_or(Rep::Unknown);
                env.insert(n.clone(), refine_rep(old, scrut.clone()));
            }
            Pat::Ctor { name, args } => {
                if args.is_empty() && self.global_int_const(name).is_some() {
                    return;
                }
                if let Some(cid) = self.resolve_ctor_name(name) {
                    if args.len() != self.constructors[cid].arity {
                        return;
                    }
                    let declared_len = self.constructors[cid].args.len();
                    for (i, p) in args.iter().enumerate() {
                        let declared = if i < declared_len {
                            rep_from_ty(&self.constructors[cid].args[i], &self.type_names)
                        } else {
                            Rep::Unknown
                        };
                        let mut r = declared.clone();
                        if let Pat::Ctor { name: nested, .. } = p {
                            if let Some(ncid) = self.resolve_ctor_name(nested) {
                                let nr = Rep::Adt(self.constructors[ncid].type_name.clone());
                                r = if matches!(r, Rep::Unknown) {
                                    nr
                                } else {
                                    refine_rep(r, nr)
                                };
                            }
                        }
                        self.constrain_pattern(p, &r, env);
                    }
                }
            }
            Pat::Tuple(xs) => {
                for p in xs {
                    self.constrain_pattern(p, &Rep::Ptr, env);
                }
            }
            Pat::Wild | Pat::Int(_) | Pat::Str(_) => {}
        }
    }

    fn ast_pure(&self, e: &Expr, current: usize) -> bool {
        match &e.kind {
            ExprKind::Int(_)
            | ExprKind::Float(_)
            | ExprKind::Str(_)
            | ExprKind::Var(_)
            | ExprKind::Quote(_) => true,
            ExprKind::Let { value, body, .. } => {
                self.ast_pure(value, current) && self.ast_pure(body, current)
            }
            ExprKind::Fn { .. } => true,
            ExprKind::App { fun, args } => {
                let (base, flat_args) = Analyzer::flatten_application(fun, args);
                if matches!(&base.kind, ExprKind::Var(n) if n == "$mak.index") {


                    let _ = flat_args;
                    return false;
                }
                let fp = match &fun.kind {
                    ExprKind::Var(n) => {
                        if let Some(id) = self.external_by_name.get(n) {
                            self.externals[*id].decl.pure && !self.externals[*id].noreturn
                        } else if let Some(id) = self.resolve_function_name(n) {
                            id != current && self.functions[id].pure
                        } else {
                            false
                        }
                    }
                    _ => false,
                };
                fp && args.iter().all(|a| self.ast_pure(a, current))
            }
            ExprKind::Case { scrut, arms } => {
                self.ast_pure(scrut, current)
                    && arms.iter().all(|a| self.ast_pure(&a.body, current))
            }
            ExprKind::Block { stmts, tail } => {
                stmts.iter().all(|s| self.ast_pure(s, current)) && self.ast_pure(tail, current)
            }
            ExprKind::Bin { l, r, .. } => self.ast_pure(l, current) && self.ast_pure(r, current),
            ExprKind::Un { e, .. } => self.ast_pure(e, current),
        }
    }
}


struct Lowerer<'a> {
    a: &'a Analyzer,
    local_serial: usize,
}

impl<'a> Lowerer<'a> {
    fn new(a: &'a Analyzer) -> Self {
        Self { a, local_serial: 0 }
    }

    fn fresh_local(&mut self, hint: &str) -> String {
        let n = self.local_serial;
        self.local_serial += 1;
        format!("mak_local_{}_{}", mangle_ident(hint), n)
    }

    fn lower_all(&mut self) -> Vec<LirFunction> {
        let mut out = Vec::with_capacity(self.a.functions.len());
        for id in 0..self.a.functions.len() {
            out.push(self.lower_function(id));
        }
        out
    }

    fn lower_function(&mut self, id: usize) -> LirFunction {
        let info = self.a.functions[id].clone();
        let body = match &info.expr.kind {
            ExprKind::Fn { body, .. } => body.clone(),
            _ => {
                self.a.dbg(
                    "RUNTIME",
                    format!("f{} has no function body; using runtime recovery body", id),
                );
                Box::new(Expr {
                    span: info.expr.span,
                    kind: ExprKind::Int(0),
                })
            }
        };
        let mut env = HashMap::new();
        for (i, n) in info.captures.iter().enumerate() {
            env.insert(
                n.clone(),
                Binding::Value {
                    code: format!("c{}", i),
                    rep: info.capture_reps[i].clone(),
                    ptr_info: None,
                },
            );
        }
        for (i, n) in info.params.iter().enumerate() {
            env.insert(
                n.clone(),
                Binding::Value {
                    code: format!("a{}", i),
                    rep: info.param_reps[i].clone(),
                    ptr_info: None,
                },
            );
        }
        self.a.dbg(
            "LOWER",
            format!(
                "enter f{} {} expected-return={} params=[{}]",
                id,
                info.source_name.as_deref().unwrap_or("<lambda>"),
                rep_debug(&info.ret_rep),
                info.param_reps
                    .iter()
                    .enumerate()
                    .map(|(i, r)| format!(
                        "{}={}:{}",
                        i,
                        info.params.get(i).map(String::as_str).unwrap_or("?"),
                        rep_debug(r)
                    ))
                    .collect::<Vec<_>>()
                    .join(", ")
            ),
        );
        let ir = self.lower_expr(&body, &mut env, Some(id));
        self.a.dbg(
            "LOWER",
            format!(
                "exit f{} => LIR rep={} kind={}",
                id,
                rep_debug(&lir_rep(&ir)),
                lir_kind_debug(&ir)
            ),
        );
        let lowered_rep = lir_rep(&ir);
        if !matches!(lowered_rep, Rep::Unknown)
            && !matches!(info.ret_rep, Rep::Unknown)
            && lowered_rep != info.ret_rep
        {
            self.a.dbg(
                "RUNTIME",
                format!(
                    "f{} return representation mismatch: analyzer={} lowered={}; preserving runtime validation",
                    id,
                    rep_debug(&info.ret_rep),
                    rep_debug(&lowered_rep)
                ),
            );
        }
        if lir_contains_fallback(&ir) {
            self.a.dbg(
                "RUNTIME",
                format!(
                    "f{} lowered to unresolved fallback; preserving runtime recovery",
                    id
                ),
            );
        }
        LirFunction { info, body: ir }
    }

    fn direct_index_expr(
        &mut self,
        base: &Expr,
        index: &Expr,
        env: &HashMap<String, Binding>,
        current: Option<usize>,
    ) -> Result<(String, Rep), String> {
        let (base_code, base_rep) = self.direct_ast_expr(base, env, current)?;
        if !is_data_pointer_rep(&base_rep) && !matches!(base_rep, Rep::Unknown) {
            return Err(format!(
                "cannot index a non-pointer value of representation {}",
                rep_debug(&base_rep)
            ));
        }
        let (index_code, index_rep) = self.direct_ast_expr(index, env, current)?;
        if !index_rep.is_integer() && !matches!(index_rep, Rep::Unknown) {
            return Err(format!(
                "an index must be an integer, got {}",
                rep_debug(&index_rep)
            ));
        }


        let (elem_ctype, elem_rep) = match pointer_info_of_expr(base, self.a, Some(env)) {
            Some(info) => (info.elem_ctype, info.elem_rep),
            None => ("uint8_t".to_string(), Rep::U8),
        };
        let code = format!(
            "(*(({} *)(uintptr_t)({}) + (intptr_t)({})))",
            elem_ctype, base_code, index_code
        );
        Ok((code, elem_rep))
    }

    fn direct_ast_expr(
        &mut self,
        e: &Expr,
        env: &HashMap<String, Binding>,
        current: Option<usize>,
    ) -> Result<(String, Rep), String> {
        match &e.kind {
            ExprKind::Int(v) => Ok((c_i128(*v as i128), integer_literal_rep(*v))),
            ExprKind::Float(v) => Ok((c_float(*v), Rep::F64)),
            ExprKind::Str(s) => Ok((c_string(s), Rep::Str)),
            ExprKind::Var(n) => {
                if let Some(b) = env.get(n) {
                    return Ok(match b {
                        Binding::Value { code, rep, .. } => (code.clone(), rep.clone()),
                        Binding::Function { id } => {
                            (self.a.functions[*id].c_name.clone(), Rep::Function(*id))
                        }
                    });
                }
                if let Some(id) = self.a.resolve_function_name(n) {
                    return Ok((self.a.functions[id].c_name.clone(), Rep::Function(id)));
                }
                if let Some(v) = self.a.global_int_const(n) {
                    return Ok((c_i128(v), Rep::I64));
                }

                if let Some(eid) = self.a.external_by_name.get(n).copied() {
                    let ext = &self.a.externals[eid];
                    match &ext.decl.foreign {
                        ForeignTy::Atom(_) => {
                            return Ok((ext.c_name.clone(), foreign_value_rep(&ext.decl.foreign)));
                        }
                        ForeignTy::Fun { .. } => {


                            return Ok((
                                ext.c_name.clone(),
                                foreign_value_rep(&ext.decl.foreign),
                            ));
                        }
                    }
                }
                if let Some(r) = self.a.global_reps.get(n) {
                    return Ok((user_value_name(n), r.clone()));
                }
                if let Some(cid) = self.a.resolve_ctor_name(n) {
                    let c = &self.a.constructors[cid];
                    if c.arity == 0 {
                        return Ok((
                            format!("&mak_ir_ctor_obj_{}", cid),
                            Rep::Adt(c.type_name.clone()),
                        ));
                    }
                    return Err(format!(
                        "constructor '{}' requires {} argument(s)",
                        n, c.arity
                    ));
                }
                Err(format!("unresolved variable '{}'", n))
            }
            ExprKind::Let { name, value, body } => {
                let (vc, vr) = self.direct_ast_expr(value, env, current)?;
                let mut next = env.clone();
                let code = self.fresh_local(name);
                next.insert(
                    name.clone(),
                    Binding::Value {
                        code: code.clone(),
                        rep: vr.clone(),
                        ptr_info: pointer_info_of_expr(value, self.a, Some(env)),
                    },
                );
                let (bc, br) = self.direct_ast_expr(body, &next, current)?;
                let declaration = rep_decl(&vr, &code, &self.a.functions);
                let mut out = String::from("(__extension__({ ");
                if matches!(vr, Rep::Unit) {
                    if !vc.is_empty() {
                        out.push_str(&vc);
                        out.push_str("; ");
                    }
                } else {
                    out.push_str(&declaration);
                    out.push_str(" = ");
                    out.push_str(&cast_code(&vc, &vr, &vr, self.a));
                    out.push_str("; ");
                }
                if bc.is_empty() {
                    out.push_str("0; ");
                } else {
                    out.push_str(&bc);
                    out.push_str("; ");
                }
                out.push_str("}))");
                Ok((out, br))
            }
            ExprKind::Fn { .. } => {
                if let Some(id) = self.a.lambda_by_span.get(&(e.span.lo, e.span.hi)).copied() {
                    if self.a.functions[id].captures.is_empty() {
                        return Ok((self.a.functions[id].c_name.clone(), Rep::Function(id)));
                    }
                    return Err(format!(
                        "lambda f{} captures {} value(s); direct C function-pointer recovery is not ABI-safe",
                        id,
                        self.a.functions[id].captures.len()
                    ));
                }
                Err("lambda span is not registered".into())
            }
            ExprKind::App { fun, args } => {
                let (base, all_args) = Analyzer::flatten_application(fun, args);
                if matches!(&base.kind, ExprKind::Var(n) if n == "$mak.index") {
                    if all_args.len() != 2 {
                        return Err(format!("index intrinsic expects 2 arguments, got {}", all_args.len()));
                    }
                    return self.direct_index_expr(all_args[0], all_args[1], env, current);
                }
                let mut ac = Vec::with_capacity(all_args.len());
                let mut ars = Vec::with_capacity(all_args.len());
                for arg in &all_args {
                    let (c, r) = self.direct_ast_expr(arg, env, current)?;
                    ac.push(c);
                    ars.push(r);
                }
                if let ExprKind::Var(name) = &base.kind {
                    if let Some(fid) = self.a.resolve_function_name(name).or_else(|| {
                        self.a
                            .best_function_recovery(name, all_args.len(), &ars, current)
                    }) {
                        if fid < self.a.functions.len()
                            && self.a.functions[fid].captures.is_empty()
                            && self.a.functions[fid].params.len() == all_args.len()
                        {
                            let info = &self.a.functions[fid];
                            let mut call_args = Vec::with_capacity(ac.len());
                            for i in 0..ac.len() {
                                let expected = &info.param_reps[i];
                                if !rep_compatible(&ars[i], expected) {
                                    return Err(format!(
                                        "direct AST call f{} argument {} has representation {} but parameter requires {}",
                                        fid,
                                        i,
                                        rep_debug(&ars[i]),
                                        rep_debug(expected)
                                    ));
                                }
                                call_args.push(cast_code(&ac[i], &ars[i], expected, self.a));
                            }
                            let call = format!("{}({})", info.c_name, call_args.join(", "));
                            if rep_is_adt(&info.ret_rep) {
                                let n = self.fresh_local("direct_call");
                                let ty = adt_struct_type(&info.ret_rep);
                                let fallback = default_value_expr(&info.ret_rep, self.a);
                                let mut out = String::from("(__extension__({ ");
                                out.push_str(&ty);
                                out.push_str(" * ");
                                out.push_str(&n);
                                out.push_str(" = (");
                                out.push_str(&ty);
                                out.push_str(" *)mak_ir_arena_alloc(sizeof(");
                                out.push_str(&ty);
                                out.push_str("), _Alignof(");
                                out.push_str(&ty);
                                out.push_str(")); if (!");
                                out.push_str(&n);
                                out.push_str(") { mak_ir_recover(\"direct AST ADT call arena exhaustion\"); ");
                                out.push_str(&n);
                                out.push_str(" = ");
                                out.push_str(&fallback);
                                out.push_str("; } else { ");
                                let mut call_with_out = String::from(&call);
                                if call_with_out.ends_with(')') {
                                    call_with_out.pop();
                                    if !info.params.is_empty() {
                                        call_with_out.push_str(", ");
                                    }
                                    call_with_out.push_str(&n);
                                    call_with_out.push(')');
                                } else {
                                    return Err(format!(
                                        "internal direct-C call formatting failure for f{}",
                                        fid
                                    ));
                                }
                                out.push_str(&call_with_out);
                                out.push_str("; } ");
                                out.push_str(&n);
                                out.push_str("; }))");
                                return Ok((out, info.ret_rep.clone()));
                            }
                            return Ok((call, info.ret_rep.clone()));
                        }
                    }
                    if let Some(eid) = self.a.external_by_name.get(name).copied() {
                        let ext = &self.a.externals[eid];
                        if let ForeignTy::Fun { args: fargs, ret } = &ext.decl.foreign {
                            if fargs
                                .iter()
                                .filter(|x| matches!(x, ForeignArg::Ty(_)))
                                .count()
                                > all_args.len()
                                && !fargs.iter().any(|x| matches!(x, ForeignArg::VarArgs))
                            {
                                return Err(format!(
                                    "external '{}' needs more arguments than supplied",
                                    name
                                ));
                            }
                            let rr = foreign_ret_rep(ret);
                            if rep_is_adt(&rr) {
                                return Err(format!(
                                    "external '{}' returns ADT '{}', whose foreign ABI cannot be reconstructed safely",
                                    name,
                                    rep_debug(&rr)
                                ));
                            }
                            let mut call_args = Vec::with_capacity(ac.len());
                            for (i, c) in ac.iter().enumerate() {
                                let ar = ars.get(i).cloned().unwrap_or(Rep::Unknown);
                                let expected = match fargs.get(i) {
                                    Some(ForeignArg::Ty(t)) => rep_from_foreign(t),
                                    _ => ar.clone(),
                                };
                                call_args.push(cast_code(c, &ar, &expected, self.a));
                            }
                            return Ok((
                                format!("{}({})", ext.c_name, call_args.join(", ")),
                                if ext.noreturn { Rep::Unit } else { rr },
                            ));
                        }
                    }
                    if let Some(cid) = self
                        .a
                        .resolve_ctor_name(name)
                        .or_else(|| self.a.best_ctor_recovery(name, all_args.len(), &ars))
                    {
                        let c = &self.a.constructors[cid];
                        if c.arity != all_args.len() {
                            return Err(format!(
                                "constructor '{}' arity mismatch: expected {}, got {}",
                                name,
                                c.arity,
                                all_args.len()
                            ));
                        }
                        return self.direct_construct(cid, &ac, &ars, current);
                    }
                    return Err(format!(
                        "call '{}' /{} could not be resolved by exact name, case-insensitive name, canonical name, type shape, compatible shape, or unique arity",
                        name,
                        all_args.len()
                    ));
                }
                let (fc, fr) = self.direct_ast_expr(fun, env, current)?;
                match fr {
                    Rep::Function(fid) => {
                        if self
                            .a
                            .functions
                            .get(fid)
                            .map(|f| f.captures.is_empty())
                            .unwrap_or(false)
                        {
                            let call = format!("{}({})", fc, ac.join(", "));
                            Ok((call, self.a.functions[fid].ret_rep.clone()))
                        } else {
                            Err(format!(
                                "callee f{} has captures and cannot be called through the raw C fallback ABI",
                                fid
                            ))
                        }
                    }
                    Rep::ExternFunction { ret, .. } => {
                        let call = format!("{}({})", fc, ac.join(", "));
                        Ok((call, *ret))
                    }
                    _ => Err(format!(
                        "callee is not a direct function value; representation={}",
                        rep_debug(&fr)
                    )),
                }
            }
            ExprKind::Case { scrut, arms } => self.direct_case(scrut, arms, env, current),
            ExprKind::Block { stmts, tail } => {
                let next = env.clone();
                let mut out = String::from("(__extension__({ ");
                for stmt in stmts {
                    let (c, r) = self.direct_ast_expr(stmt, &next, current)?;
                    if !c.is_empty() {
                        out.push_str(&c);
                        out.push_str("; ");
                    }
                    let _ = r;
                }
                let (c, r) = self.direct_ast_expr(tail, &next, current)?;
                if c.is_empty() {
                    out.push_str("0; ");
                } else {
                    out.push_str(&c);
                    out.push_str("; ");
                }
                out.push_str("}))");
                Ok((out, r))
            }
            ExprKind::Bin { op, l, r } => {
                let (lc, lr) = self.direct_ast_expr(l, env, current)?;
                let (rc, rr) = self.direct_ast_expr(r, env, current)?;
                let lir_op = bin_lir(*op);

                let lptr = is_data_pointer_rep(&lr);
                let rptr = is_data_pointer_rep(&rr);
                let loff = lr.is_integer() || matches!(lr, Rep::Unknown);
                let roff = rr.is_integer() || matches!(rr, Rep::Unknown);
                if matches!(op, BinOp::Add) && ((lptr && roff) || (loff && rptr)) {
                    let (ptr_code, ptr_rep, off_code, ptr_expr) = if lptr {
                        (lc.as_str(), lr.clone(), rc.as_str(), l.as_ref())
                    } else {
                        (rc.as_str(), rr.clone(), lc.as_str(), r.as_ref())
                    };
                    let elem_ctype = pointer_element_ctype(ptr_expr, self.a, Some(env), &ptr_rep);
                    let out_type = ptr_rep.ctype(&self.a.functions);
                    let code = format!(
                        "(({})((({} *)(uintptr_t)({})) + (intptr_t)({})))",
                        out_type, elem_ctype, ptr_code, off_code
                    );
                    return Ok((code, ptr_rep));
                }
                if matches!(op, BinOp::Sub) && lptr && roff {
                    let elem_ctype = pointer_element_ctype(l, self.a, Some(env), &lr);
                    let out_type = lr.ctype(&self.a.functions);
                    let code = format!(
                        "(({})((({} *)(uintptr_t)({})) - (intptr_t)({})))",
                        out_type, elem_ctype, lc, rc
                    );
                    return Ok((code, lr));
                }
                if matches!(op, BinOp::Sub) && lptr && rptr {
                    let elem_ctype = pointer_element_ctype(l, self.a, Some(env), &lr);
                    if pointer_info_of_expr(l, self.a, Some(env)).is_some()
                        || matches!(lr, Rep::Adt(_))
                    {
                        let code = format!(
                            "((int64_t)((({} *)(uintptr_t)({})) - (({} *)(uintptr_t)({}))))",
                            elem_ctype, lc, elem_ctype, rc
                        );
                        return Ok((code, Rep::I64));
                    }
                    let code = format!(
                        "((int64_t)((intptr_t)(uintptr_t)({}) - (intptr_t)(uintptr_t)({})))",
                        lc, rc
                    );
                    return Ok((code, Rep::I64));
                }

                let arithmetic = matches!(
                    lir_op,
                    BinOpLir::Add
                        | BinOpLir::Sub
                        | BinOpLir::Mul
                        | BinOpLir::Div
                        | BinOpLir::Rem
                        | BinOpLir::And
                        | BinOpLir::Or
                        | BinOpLir::Xor
                        | BinOpLir::Shl
                        | BinOpLir::Shr
                );
                let relational = matches!(
                    lir_op,
                    BinOpLir::Lt | BinOpLir::Gt | BinOpLir::Le | BinOpLir::Ge
                );
                let both_numeric =
                    (lr.is_integer() || lr.is_float()) && (rr.is_integer() || rr.is_float());
                let rep = if matches!(lir_op, BinOpLir::LAnd | BinOpLir::LOr) {
                    Rep::I32
                } else if matches!(lir_op, BinOpLir::Rem) {


                    if !both_numeric {
                        return Err(format!(
                            "non-numeric binary expression: {} {} {}",
                            rep_debug(&lr),
                            op_debug(*op),
                            rep_debug(&rr)
                        ));
                    }
                    Rep::I64
                } else if arithmetic {
                    if !both_numeric {
                        return Err(format!(
                            "non-numeric binary expression: {} {} {}",
                            rep_debug(&lr),
                            op_debug(*op),
                            rep_debug(&rr)
                        ));
                    }
                    if lr.is_float() || rr.is_float() {
                        Rep::F64
                    } else {
                        merge_rep(lr.clone(), rr.clone())
                    }
                } else if relational {
                    if !both_numeric {
                        return Err(format!(
                            "non-numeric relational expression: {} {} {}",
                            rep_debug(&lr),
                            op_debug(*op),
                            rep_debug(&rr)
                        ));
                    }
                    Rep::I32
                } else if matches!(lir_op, BinOpLir::Eq | BinOpLir::Ne) {
                    let eq_ok = match (&lr, &rr) {
                        (a, b)
                            if (a.is_integer() || a.is_float())
                                && (b.is_integer() || b.is_float()) =>
                        {
                            true
                        }
                        (Rep::Str, Rep::Str) => true,
                        (Rep::Adt(a), Rep::Adt(b)) if a == b => true,
                        (Rep::Ptr, Rep::Ptr) => true,
                        (
                            Rep::ExternFunction { ctype: a, .. },
                            Rep::ExternFunction { ctype: b, .. },
                        ) if a == b => true,
                        (Rep::Function(a), Rep::Function(b)) if a == b => true,
                        _ => false,
                    };
                    if !eq_ok {
                        return Err(format!(
                            "incompatible equality operands: {} {} {}",
                            rep_debug(&lr),
                            op_debug(*op),
                            rep_debug(&rr)
                        ));
                    }
                    Rep::I32
                } else {
                    return Err("unsupported binary expression".into());
                };
                let lhs = cast_code(&lc, &lr, &rep, self.a);
                let rhs = cast_code(&rc, &rr, &rep, self.a);
                if matches!(&rep, Rep::I64) {
                    let helper = match lir_op {
                        BinOpLir::Add => Some("mak_ir_i64_add_wrap"),
                        BinOpLir::Sub => Some("mak_ir_i64_sub_wrap"),
                        BinOpLir::Mul => Some("mak_ir_i64_mul_wrap"),
                        BinOpLir::Div => Some("mak_ir_i64_div_wrap"),
                        BinOpLir::Rem => Some("mak_ir_i64_rem_wrap"),
                        BinOpLir::Shl => Some("mak_ir_i64_shl_wrap"),
                        BinOpLir::Shr => Some("mak_ir_i64_shr"),
                        _ => None,
                    };
                    if let Some(helper) = helper {
                        return Ok((format!("{}({}, {})", helper, lhs, rhs), rep));
                    }
                }
                if matches!(lir_op, BinOpLir::Eq | BinOpLir::Ne)
                    && matches!(lr, Rep::Str)
                    && matches!(rr, Rep::Str)
                {
                    let cmp = format!("strcmp({}, {})", lc, rc);
                    return Ok((
                        if matches!(lir_op, BinOpLir::Eq) {
                            format!("({} == 0)", cmp)
                        } else {
                            format!("({} != 0)", cmp)
                        },
                        Rep::I32,
                    ));
                }
                let opstr = match lir_op {
                    BinOpLir::Add => "+",
                    BinOpLir::Sub => "-",
                    BinOpLir::Mul => "*",
                    BinOpLir::Div => "/",
                    BinOpLir::Rem => "%",
                    BinOpLir::And => "&",
                    BinOpLir::Or => "|",
                    BinOpLir::Xor => "^",
                    BinOpLir::Shl => "<<",
                    BinOpLir::Shr => ">>",
                    BinOpLir::Eq => "==",
                    BinOpLir::Ne => "!=",
                    BinOpLir::Lt => "<",
                    BinOpLir::Gt => ">",
                    BinOpLir::Le => "<=",
                    BinOpLir::Ge => ">=",
                    BinOpLir::LAnd => "&&",
                    BinOpLir::LOr => "||",
                };
                Ok((format!("({} {} {})", lhs, opstr, rhs), rep))
            }
            ExprKind::Un { op, e } => {
                let (c, r) = self.direct_ast_expr(e, env, current)?;
                let (code, rep) = match op {
                    UnOp::Neg if matches!(&r, Rep::I64) => {
                        (format!("mak_ir_i64_neg_wrap((int64_t)({}))", c), r)
                    }
                    UnOp::Neg => (format!("(-{})", c), r),
                    UnOp::Not => (format!("(!({}))", c), Rep::I32),
                    UnOp::BitNot => (format!("(~({}))", c), r),
                };
                Ok((code, rep))
            }
            ExprKind::Quote(x) => self.direct_ast_expr(x, env, current),
        }
    }

    fn direct_construct(
        &mut self,
        cid: usize,
        args: &[String],
        arg_reps: &[Rep],
        _current: Option<usize>,
    ) -> Result<(String, Rep), String> {
        let c = self
            .a
            .constructors
            .get(cid)
            .ok_or_else(|| format!("constructor id {} is invalid", cid))?;
        if c.arity != args.len() {
            return Err(format!(
                "constructor #{} expects {}, got {} argument(s)",
                cid,
                c.arity,
                args.len()
            ));
        }
        let rep = Rep::Adt(c.type_name.clone());
        if c.arity == 0 {
            return Ok((format!("&mak_ir_ctor_obj_{}", cid), rep));
        }
        let ty = format!("struct mak_{}", mangle_ident(&c.type_name));
        let p = self.fresh_local("direct_ctor");
        let fallback = default_value_expr(&rep, self.a);
        let mut out = String::from("(__extension__({ ");
        out.push_str(&ty);
        out.push_str(" * ");
        out.push_str(&p);
        out.push_str(" = (");
        out.push_str(&ty);
        out.push_str(" *)mak_ir_arena_alloc(sizeof(");
        out.push_str(&ty);
        out.push_str("), _Alignof(");
        out.push_str(&ty);
        out.push_str(")); if (!");
        out.push_str(&p);
        out.push_str(") { mak_ir_recover(\"direct AST constructor arena exhaustion\"); ");
        out.push_str(&p);
        out.push_str(" = ");
        out.push_str(&fallback);
        out.push_str("; } else { ");
        out.push_str(&p);
        out.push_str("->tag = ");
        out.push_str(&c.tag);
        out.push_str("; ");
        for i in 0..c.arity {
            let fr = c
                .fields
                .get(i)
                .cloned()
                .or_else(|| c.args.get(i).map(|t| rep_from_ty(t, &self.a.type_names)))
                .unwrap_or(Rep::Unknown);
            if !rep_compatible(&arg_reps[i], &fr) {
                return Err(format!(
                    "direct AST constructor #{} argument {} has representation {} but field requires {}",
                    cid,
                    i,
                    rep_debug(&arg_reps[i]),
                    rep_debug(&fr)
                ));
            }
            out.push('(');
            out.push_str(&p);
            out.push_str(")->");
            out.push_str(&constructor_field_prefix(self.a, cid));
            out.push_str("f");
            out.push_str(&i.to_string());
            out.push_str(" = ");
            out.push_str(&cast_code(&args[i], &arg_reps[i], &fr, self.a));
            out.push_str("; ");
        }
        out.push_str("} ");
        out.push_str(&p);
        out.push_str("; }))");
        Ok((out, rep))
    }

    fn direct_pattern(
        &mut self,
        pat: &Pat,
        value: &str,
        value_rep: &Rep,
        env: &HashMap<String, Binding>,
    ) -> Result<(String, HashMap<String, Binding>), String> {
        let mut next = env.clone();
        match pat {
            Pat::Wild => Ok(("1".into(), next)),
            Pat::Bind(n) => {
                next.insert(
                    n.clone(),
                    Binding::Value {
                        code: value.to_string(),
                        rep: value_rep.clone(),
                        ptr_info: None,
                    },
                );
                Ok(("1".into(), next))
            }
            Pat::Int(v) => {
                if !value_rep.is_integer() && !matches!(value_rep, Rep::Unknown) {
                    return Ok(("0".into(), next));
                }
                Ok((
                    format!(
                        "({} == {})",
                        cast_code(value, value_rep, value_rep, self.a),
                        c_uint_case(*v)
                    ),
                    next,
                ))
            }
            Pat::Str(s) => {
                if !matches!(value_rep, Rep::Str | Rep::Ptr | Rep::Unknown) {
                    return Ok(("0".into(), next));
                }
                Ok((
                    format!(
                        "({} != NULL && strcmp((const char *)({}), {}) == 0)",
                        value,
                        value,
                        c_string(s)
                    ),
                    next,
                ))
            }
            Pat::Ctor { name, args } => {
                if args.is_empty() {
                    if let Some(v) = self.a.global_int_const(name) {
                        return Ok((
                            format!(
                                "({} == {})",
                                cast_code(value, value_rep, &Rep::I64, self.a),
                                c_i128(v)
                            ),
                            next,
                        ));
                    }
                }
                let ars = args
                    .iter()
                    .map(|p| lower_pattern_rep(self.a, p).unwrap_or(Rep::Unknown))
                    .collect::<Vec<_>>();
                let cid = self
                    .a
                    .constructor_for_pattern(name, args.len(), value_rep)
                    .or_else(|| self.a.best_ctor_recovery(name, args.len(), &ars))
                    .ok_or_else(|| {
                        format!(
                            "constructor pattern '{}' /{} could not be resolved",
                            name,
                            args.len()
                        )
                    })?;
                let c = &self.a.constructors[cid];
                let expected = Rep::Adt(c.type_name.clone());
                if !rep_compatible(value_rep, &expected) {
                    return Ok(("0".into(), next));
                }
                let base = format!("((struct mak_{} *)({}))", mangle_ident(&c.type_name), value);
                let mut conds = vec![format!(
                    "({} != NULL && ({})->tag == {})",
                    base, base, c.tag
                )];
                for (i, p) in args.iter().enumerate() {
                    let fr = c
                        .fields
                        .get(i)
                        .cloned()
                        .or_else(|| c.args.get(i).map(|t| rep_from_ty(t, &self.a.type_names)))
                        .unwrap_or(Rep::Unknown);
                    let field = format!(
                        "({})->{}f{}",
                        base,
                        constructor_field_prefix(self.a, cid),
                        i
                    );
                    let (cond, bound) = self.direct_pattern(p, &field, &fr, &next)?;
                    next = bound;
                    conds.push(cond);
                }
                Ok((
                    conds
                        .into_iter()
                        .filter(|x| x != "1")
                        .collect::<Vec<_>>()
                        .join(" && ")
                        .if_empty_default(),
                    next,
                ))
            }
            Pat::Tuple(_) => Err("tuple pattern has no defined C layout in the current ABI".into()),
        }
    }

    fn direct_case(
        &mut self,
        scrut: &Expr,
        arms: &[Arm],
        env: &HashMap<String, Binding>,
        current: Option<usize>,
    ) -> Result<(String, Rep), String> {
        match &scrut.kind {
            ExprKind::Int(v) => {
                for arm in arms {
                    match &arm.pat {
                        Pat::Int(n) if *n == *v => {
                            return self.direct_ast_expr(&arm.body, env, current);
                        }
                        Pat::Ctor { name, args }
                            if args.is_empty()
                                && self.a.global_int_const(name) == Some(*v as i128) =>
                        {
                            return self.direct_ast_expr(&arm.body, env, current);
                        }
                        Pat::Bind(name) => {
                            let mut next = env.clone();
                            next.insert(
                                name.clone(),
                                Binding::Value {
                                    code: c_i128(*v as i128),
                                    rep: integer_literal_rep(*v),
                                    ptr_info: None,
                                },
                            );
                            return self.direct_ast_expr(&arm.body, &next, current);
                        }
                        Pat::Wild => return self.direct_ast_expr(&arm.body, env, current),
                        _ => {}
                    }
                }
                return Err(format!(
                    "constant integer case {} has no directly recoverable arm",
                    v
                ));
            }
            ExprKind::Str(s) => {
                for arm in arms {
                    match &arm.pat {
                        Pat::Str(p) if p == s => {
                            return self.direct_ast_expr(&arm.body, env, current);
                        }
                        Pat::Bind(name) => {
                            let mut next = env.clone();
                            next.insert(
                                name.clone(),
                                Binding::Value {
                                    code: c_string(s),
                                    rep: Rep::Str,
                                    ptr_info: None,
                                },
                            );
                            return self.direct_ast_expr(&arm.body, &next, current);
                        }
                        Pat::Wild => return self.direct_ast_expr(&arm.body, env, current),
                        _ => {}
                    }
                }
                return Err("constant string case has no directly recoverable arm".into());
            }
            _ => {}
        }
        let (sc, sr0) = self.direct_ast_expr(scrut, env, current)?;
        let mut sr = sr0.clone();
        if matches!(sr, Rep::Unknown) {
            for arm in arms {
                if let Some(r) = lower_pattern_rep(self.a, &arm.pat) {
                    sr = merge_rep(sr, r);
                }
            }
        }
        if matches!(sr, Rep::Unknown) {
            return Err(
                "case scrutinee representation remained Unknown after AST pattern evidence".into(),
            );
        }
        let mut result_rep = Rep::Unknown;
        for arm in arms {
            let r = infer_ast_rep(&arm.body, self.a);
            result_rep = merge_rep(result_rep, r);
        }
        if matches!(result_rep, Rep::Unknown) {
            result_rep = if current
                .and_then(|id| self.a.functions.get(id))
                .map(|f| !matches!(f.ret_rep, Rep::Unknown))
                .unwrap_or(false)
            {
                self.a.functions[current.unwrap()].ret_rep.clone()
            } else {
                return Err("case result representation remained Unknown".into());
            };
        }
        let mut out = String::from("(__extension__({ ");
        let scrut_name = self.fresh_local("direct_scrut");
        out.push_str(&rep_decl(&sr, &scrut_name, &self.a.functions));
        out.push_str(" = ");
        out.push_str(&cast_code(&sc, &sr0, &sr, self.a));
        out.push_str("; ");
        let result_name = self.fresh_local("direct_case");
        if rep_is_adt(&result_rep) {
            let ty = adt_struct_type(&result_rep);
            out.push_str(&ty);
            out.push_str(" * ");
            out.push_str(&result_name);
            out.push_str(" = ");
            out.push_str(&default_value_expr(&result_rep, self.a));
            out.push_str("; ");
        } else {
            let result_ty = if matches!(result_rep, Rep::Unit) {
                Rep::I32
            } else {
                result_rep.clone()
            };
            out.push_str(&rep_decl(&result_ty, &result_name, &self.a.functions));
            out.push_str(" = ");
            out.push_str(&zero_for(&result_ty));
            out.push_str("; ");
        }
        let mut emitted_any = false;
        let mut terminated = false;
        for arm in arms {
            if terminated {
                break;
            }
            let (cond, arm_env) = self.direct_pattern(&arm.pat, &scrut_name, &sr, env)?;
            if cond == "0" {
                continue;
            }
            let (bc, br) = self.direct_ast_expr(&arm.body, &arm_env, current)?;
            if !emitted_any {
                if cond == "1" {
                    out.push_str("{ ");
                } else {
                    out.push_str("if ");
                    out.push_str(&cond);
                    out.push_str(" { ");
                }
            } else if cond == "1" {
                out.push_str("else { ");
            } else {
                out.push_str("else if ");
                out.push_str(&cond);
                out.push_str(" { ");
            }
            if !bc.is_empty() {
                if rep_is_adt(&result_rep) {
                    out.push_str(&result_name);
                    out.push_str(" = ");
                    out.push_str(&cast_code(&bc, &br, &result_rep, self.a));
                    out.push_str("; ");
                } else if !matches!(result_rep, Rep::Unit) {
                    out.push_str(&result_name);
                    out.push_str(" = ");
                    out.push_str(&cast_code(&bc, &br, &result_rep, self.a));
                    out.push_str("; ");
                } else {
                    out.push_str(&bc);
                    out.push_str("; ");
                }
            }
            out.push_str("} ");
            emitted_any = true;
            if matches!(arm.pat, Pat::Wild | Pat::Bind(_)) {
                terminated = true;
            }
        }
        if !emitted_any {
            return Err("all case branches were rejected by AST-C pattern recovery".into());
        }
        if !arms
            .iter()
            .any(|a| matches!(a.pat, Pat::Wild | Pat::Bind(_)))
        {
            out.push_str("else { mak_ir_recover(\"AST-C case had no matching arm\"); } ");
        }
        out.push_str(&result_name);
        out.push_str("; }))");
        Ok((out, result_rep))
    }

    fn recover_direct_ast(
        &mut self,
        e: &Expr,
        env: &HashMap<String, Binding>,
        current: Option<usize>,
        origin: &str,
    ) -> Option<LirExpr> {
        self.a.note_recovery(current, e, "LIR", origin.to_string());
        match self.direct_ast_expr(e, env, current) {
            Ok((code, rep)) => {
                self.a.dbg(
                    "RECOVER",
                    format!(
                        "{} -> direct AST-C recovery succeeded as {}",
                        expr_debug(e),
                        rep_debug(&rep)
                    ),
                );
                Some(LirExpr::DirectC { code, rep })
            }
            Err(reason) => {
                self.a.note_recovery(current, e, "AST-C", reason.clone());
                None
            }
        }
    }

    fn lower_expr(
        &mut self,
        e: &Expr,
        env: &mut HashMap<String, Binding>,
        current: Option<usize>,
    ) -> LirExpr {
        match &e.kind {
            ExprKind::Int(v) => LirExpr::Int(*v as i128, integer_literal_rep(*v)),
            ExprKind::Float(v) => LirExpr::Float(*v, Rep::F64),
            ExprKind::Str(s) => LirExpr::Str(s.clone()),
            ExprKind::Var(n) => {
                if let Some(b) = env.get(n) {
                    return match b {
                        Binding::Value { code, rep, .. } => LirExpr::Var(code.clone(), rep.clone()),
                        Binding::Function { id } => LirExpr::FnRef(*id),
                    };
                }
                if let Some(id) = self.a.resolve_function_name(n) {
                    return LirExpr::FnRef(id);
                }
                if let Some(v) = self.a.global_int_const(n) {
                    return LirExpr::Int(v, Rep::I64);
                }


                if let Some(eid) = self.a.external_by_name.get(n).copied() {
                    let ext = &self.a.externals[eid];
                    match ext.decl.foreign {
                        ForeignTy::Atom(_) => {
                            return LirExpr::Var(
                                ext.c_name.clone(),
                                foreign_value_rep(&ext.decl.foreign),
                            );
                        }
                        ForeignTy::Fun { .. } => {


                            return LirExpr::Var(
                                ext.c_name.clone(),
                                foreign_value_rep(&ext.decl.foreign),
                            );
                        }
                    }
                }
                if let Some(r) = self.a.global_reps.get(n) {
                    return LirExpr::Var(user_value_name(n), r.clone());
                }


                if let Some(cid) = self.a.resolve_ctor_name(n) {
                    let c = &self.a.constructors[cid];
                    if c.arity == 0 {
                        return LirExpr::Construct {
                            ctor: cid,
                            args: Vec::new(),
                            rep: Rep::Adt(c.type_name.clone()),
                        };
                    }
                    let rep = current
                        .and_then(|id| self.a.functions.get(id))
                        .map(|f| f.ret_rep.clone())
                        .filter(|r| !matches!(r, Rep::Unknown))
                        .unwrap_or(Rep::I64);
                    self.a.dbg(
                        "RUNTIME",
                        format!(
                            "{} constructor '{}' /{} used as a value; using runtime recovery",
                            current_label(current),
                            n,
                            c.arity
                        ),
                    );
                    return runtime_recovery_lir(
                        rep,
                        &format!("constructor '{}' used as value", n),
                        &self.a.functions,
                    );
                }


                let zero_arity: Vec<usize> = self
                    .a
                    .constructors
                    .iter()
                    .filter(|c| c.name == *n && c.arity == 0)
                    .map(|c| c.id)
                    .collect();

                if zero_arity.len() == 1 {
                    let cid = zero_arity[0];
                    let c = &self.a.constructors[cid];
                    return LirExpr::Construct {
                        ctor: cid,
                        args: Vec::new(),
                        rep: Rep::Adt(c.type_name.clone()),
                    };
                }

                let hint_type: Option<String> = current
                    .and_then(|id| self.a.functions.get(id))
                    .and_then(|f| match &f.ret_rep {
                        Rep::Adt(t) => Some(t.clone()),
                        _ => None,
                    });

                if let Some(want) = hint_type.as_deref() {
                    let filtered: Vec<usize> = zero_arity
                        .iter()
                        .filter(|cid| self.a.constructors[**cid].type_name == want)
                        .copied()
                        .collect();
                    if filtered.len() == 1 {
                        let cid = filtered[0];
                        let c = &self.a.constructors[cid];
                        return LirExpr::Construct {
                            ctor: cid,
                            args: Vec::new(),
                            rep: Rep::Adt(c.type_name.clone()),
                        };
                    }
                }


                if let Some(recovered) = self.recover_direct_ast(
                    e,
                    env,
                    current,
                    &format!("unresolved variable '{}'", n),
                ) {
                    return recovered;
                }


                if !zero_arity.is_empty() && !self.a.runtime_ctor_candidates(0).is_empty() {
                    let query = match hint_type.as_deref() {
                        Some(t) => format!("{}:{}", n, t),
                        None => n.clone(),
                    };
                    let rep = match hint_type.as_deref() {
                        Some(t) => Rep::Adt(t.to_string()),
                        None => Rep::Ptr,
                    };
                    self.a.dbg(
                        "RUNTIME",
                        format!(
                            "{} ambiguous zero-arity ctor '{}' -> runtime ctor resolver (query={})",
                            current_label(current),
                            n,
                            query
                        ),
                    );
                    return LirExpr::RuntimeConstruct {
                        name: query,
                        args: Vec::new(),
                        rep,
                    };
                }


                if !self.a.runtime_function_candidates(0).is_empty() {
                    let rep = self.a.runtime_function_rep(n, 0, &[]);
                    self.a.dbg(
                        "RUNTIME",
                        format!(
                            "{} unresolved zero-arity symbol '{}' -> runtime fn resolver",
                            current_label(current),
                            n
                        ),
                    );
                    return LirExpr::RuntimeCall {
                        name: n.clone(),
                        args: Vec::new(),
                        rep,
                    };
                }

                let rep = current
                    .and_then(|id| self.a.functions.get(id))
                    .map(|f| f.ret_rep.clone())
                    .filter(|r| !matches!(r, Rep::Unknown))
                    .unwrap_or(Rep::I64);
                self.a.dbg(
                    "RUNTIME",
                    format!(
                        "{} unresolved variable '{}' -> runtime recovery",
                        current_label(current),
                        n
                    ),
                );
                runtime_recovery_lir(
                    rep,
                    &format!("unresolved variable '{}'", n),
                    &self.a.functions,
                )
            }
            ExprKind::Let { name, value, body } => {
                if let ExprKind::Fn { .. } = &value.kind {
                    if let Some(id) = self
                        .a
                        .lambda_by_span
                        .get(&(value.span.lo, value.span.hi))
                        .copied()
                    {
                        env.insert(name.clone(), Binding::Function { id });
                        let ir = self.lower_expr(body, env, current);
                        env.remove(name);
                        return ir;
                    }
                }
                let v = self.lower_expr(value, env, current);
                let rep = lir_rep(&v);
                let code = self.fresh_local(name);


                let ptr_info = pointer_info_of_expr(value, self.a, Some(env));
                let old = env.insert(
                    name.clone(),
                    Binding::Value {
                        code: code.clone(),
                        rep: rep.clone(),
                        ptr_info,
                    },
                );
                let b = self.lower_expr(body, env, current);
                if let Some(old) = old {
                    env.insert(name.clone(), old);
                } else {
                    env.remove(name);
                }
                LirExpr::Let {
                    code,
                    rep,
                    value: Box::new(v),
                    body: Box::new(b),
                }
            }
            ExprKind::Fn { .. } => {
                if let Some(id) = self.a.lambda_by_span.get(&(e.span.lo, e.span.hi)).copied() {
                    LirExpr::FnRef(id)
                } else if let Some(recovered) =
                    self.recover_direct_ast(e, env, current, "lambda span was not registered")
                {
                    recovered
                } else {
                    let rep = current
                        .and_then(|id| self.a.functions.get(id))
                        .map(|f| f.ret_rep.clone())
                        .filter(|r| !matches!(r, Rep::Unknown))
                        .unwrap_or(Rep::I64);
                    self.a.dbg(
                        "RUNTIME",
                        format!(
                            "{} unregistered lambda -> runtime recovery",
                            current_label(current)
                        ),
                    );
                    runtime_recovery_lir(
                        rep,
                        "unregistered lambda",
                        &self.a.functions,
                    )
                }
            }
            ExprKind::App { fun, args } => {
                let lowered = self.lower_app(fun, args, env, current);
                if matches!(lowered, LirExpr::Fallback) {
                    if let Some(recovered) = self.recover_direct_ast(
                        e,
                        env,
                        current,
                        "LIR application lowering exhausted",
                    ) {
                        return recovered;
                    }
                    let (base, _) = Analyzer::flatten_application(fun, args);
                    let name = match &base.kind {
                        ExprKind::Var(n) => n.clone(),
                        _ => "<complex>".into(),
                    };
                    let rep = current
                        .and_then(|id| self.a.functions.get(id))
                        .map(|f| f.ret_rep.clone())
                        .filter(|r| !matches!(r, Rep::Unknown))
                        .unwrap_or(Rep::I64);
                    self.a.dbg(
                        "RUNTIME",
                        format!(
                            "{} application '{}' could not be statically lowered; using runtime recovery",
                            current_label(current),
                            name
                        ),
                    );
                    return LirExpr::RuntimeCall {
                        name: "__mak_unresolved_application".into(),
                        args: Vec::new(),
                        rep,
                    };
                }
                lowered
            }
            ExprKind::Case { scrut, arms } => {
                let mut s = self.lower_expr(scrut, env, current);
                let mut sr = lir_rep(&s);
                if matches!(sr, Rep::Unknown) {
                    for arm in arms {
                        if let Some(pr) = self.a.pattern_shape(&arm.pat) {
                            if !matches!(pr, Rep::Unknown) {
                                sr = pr;
                                break;
                            }
                        }
                    }
                    if !matches!(sr, Rep::Unknown) {
                        s = retag_lir_result_rep(s, sr.clone());
                    }
                }
                self.a.dbg(
                    "LOWER",
                    format!("CASE scrut-rep={} arms={}", rep_debug(&sr), arms.len()),
                );
                let mut out = Vec::with_capacity(arms.len());
                let mut ar = Rep::Unknown;
                for arm in arms {
                    let mut next = env.clone();
                    let pat = self.lower_pattern(&arm.pat, &sr, &mut next);
                    let body = self.lower_expr(&arm.body, &mut next, current);
                    ar = merge_rep(ar, lir_rep(&body));
                    out.push(LirArm { pat, body });
                }
                if lir_contains_fallback(&s) || out.iter().any(|x| lir_contains_fallback(&x.body)) {
                    self.a.dbg(
                        "RUNTIME",
                        format!(
                            "{} case lowering retained unresolved branch; emitter will recover at runtime",
                            current_label(current)
                        ),
                    );
                }
                if matches!(ar, Rep::Unknown) {
                    ar = current
                        .and_then(|id| self.a.functions.get(id))
                        .map(|f| f.ret_rep.clone())
                        .filter(|r| !matches!(r, Rep::Unknown))
                        .unwrap_or(Rep::Unknown);
                }
                LirExpr::Case {
                    scrut: Box::new(s),
                    arms: out,
                    rep: ar,
                }
            }
            ExprKind::Block { stmts, tail } => {
                let mut next = env.clone();
                let mut ss = Vec::with_capacity(stmts.len());
                for s in stmts {
                    ss.push(self.lower_expr(s, &mut next, current));
                }
                let t = self.lower_expr(tail, &mut next, current);
                LirExpr::Block {
                    stmts: ss,
                    tail: Box::new(t),
                }
            }
            ExprKind::Bin { op, l, r } => {
                let left = self.lower_expr(l, env, current);
                if matches!(op, BinOp::LAnd | BinOp::LOr) {
                    let right = self.lower_expr(r, env, current);
                    return LirExpr::Bin {
                        op: bin_lir(*op),
                        left: Box::new(left),
                        right: Box::new(right),
                        rep: Rep::I32,
                    };
                }
                let right = self.lower_expr(r, env, current);
                let lr = lir_rep(&left);
                let rr = lir_rep(&right);


                let left_is_ptr = is_data_pointer_rep(&lr);
                let right_is_ptr = is_data_pointer_rep(&rr);
                let left_is_offset = lr.is_integer() || matches!(lr, Rep::Unknown);
                let right_is_offset = rr.is_integer() || matches!(rr, Rep::Unknown);
                let pointer_arithmetic =
                    (matches!(op, BinOp::Add)
                        && ((left_is_ptr && right_is_offset)
                            || (left_is_offset && right_is_ptr)))
                    || (matches!(op, BinOp::Sub)
                        && left_is_ptr
                        && (right_is_offset || right_is_ptr));
                if pointer_arithmetic {
                    if let Ok((code, rep)) = self.direct_ast_expr(e, env, current) {
                        return LirExpr::DirectC { code, rep };
                    }
                }

                let comparison = matches!(
                    op,
                    BinOp::Eq | BinOp::Ne | BinOp::Lt | BinOp::Gt | BinOp::Le | BinOp::Ge
                );
                let relational = matches!(op, BinOp::Lt | BinOp::Gt | BinOp::Le | BinOp::Ge);
                let arithmetic = matches!(
                    op,
                    BinOp::Add
                        | BinOp::Sub
                        | BinOp::Mul
                        | BinOp::Div
                        | BinOp::Rem
                        | BinOp::And
                        | BinOp::Or
                        | BinOp::Xor
                        | BinOp::Shl
                        | BinOp::Shr
                );
                let both_numeric =
                    (lr.is_integer() || lr.is_float()) && (rr.is_integer() || rr.is_float());
                let base = if matches!(op, BinOp::Rem) && both_numeric {


                    Rep::I64
                } else if arithmetic && both_numeric {
                    if lr.is_float() || rr.is_float() {
                        Rep::F64
                    } else {
                        merge_rep(lr.clone(), rr.clone())
                    }
                } else if arithmetic {
                    Rep::Unknown
                } else if lr.is_float() || rr.is_float() {
                    Rep::F64
                } else {
                    merge_rep(lr.clone(), rr.clone())
                };
                if arithmetic && (!both_numeric || matches!(base, Rep::Unknown)) {
                    if let Some(recovered) = self.recover_direct_ast(
                        e,
                        env,
                        current,
                        "binary lowering produced incompatible/non-numeric operands",
                    ) {
                        return recovered;
                    }
                    let rep = current
                        .and_then(|id| self.a.functions.get(id))
                        .map(|f| f.ret_rep.clone())
                        .filter(|r| !matches!(r, Rep::Unknown))
                        .unwrap_or(Rep::I64);
                    return runtime_recovery_lir(
                        rep,
                        "non-numeric binary operation",
                        &self.a.functions,
                    );
                }
                if relational && !both_numeric {
                    return runtime_recovery_lir(
                        Rep::I32,
                        "non-numeric relational operation",
                        &self.a.functions,
                    );
                }
                let rep = if comparison { Rep::I32 } else { base };
                LirExpr::Bin {
                    op: bin_lir(*op),
                    left: Box::new(left),
                    right: Box::new(right),
                    rep,
                }
            }
            ExprKind::Un { op, e } => {
                let x = self.lower_expr(e, env, current);
                let xr = lir_rep(&x);
                let numeric = xr.is_integer() || xr.is_float();
                let rep = match op {
                    UnOp::Not => Rep::I32,
                    UnOp::Neg | UnOp::BitNot => {
                        if !numeric {
                            let rep = current
                                .and_then(|id| self.a.functions.get(id))
                                .map(|f| f.ret_rep.clone())
                                .filter(|r| !matches!(r, Rep::Unknown))
                                .unwrap_or(Rep::I64);
                            return runtime_recovery_lir(
                                rep,
                                "non-numeric unary operation",
                                &self.a.functions,
                            );
                        }
                        xr.clone()
                    }
                };
                LirExpr::Un {
                    op: un_lir(*op),
                    expr: Box::new(x),
                    rep,
                }
            }
            ExprKind::Quote(x) => self.lower_expr(x, env, current),
        }
    }
    fn lower_app(
        &mut self,
        fun: &Expr,
        args: &[Expr],
        env: &mut HashMap<String, Binding>,
        current: Option<usize>,
    ) -> LirExpr {
        let (base_fun, flat_args) = Analyzer::flatten_application(fun, args);


        if matches!(&base_fun.kind, ExprKind::Var(n) if n == "$mak.index") {
            if flat_args.len() != 2 {
                self.a.note_recovery(current, fun, "LIR", format!("index intrinsic expects 2 arguments, got {}", flat_args.len()));
                return runtime_recovery_lir(Rep::Unknown, "malformed index intrinsic", &self.a.functions);
            }
            let index_env = (*env).clone();
            return match self.direct_index_expr(flat_args[0], flat_args[1], &index_env, current) {
                Ok((code, rep)) => LirExpr::DirectC { code, rep },
                Err(reason) => {
                    self.a.note_recovery(current, fun, "index", reason);
                    runtime_recovery_lir(Rep::Unknown, "invalid index expression", &self.a.functions)
                }
            };
        }


        if let ExprKind::Fn { params, body } = &base_fun.kind {
            if params.len() == flat_args.len() {
                let mut new_body: Expr = (**body).clone();
                for (p, a) in params.iter().zip(flat_args.iter()) {
                    new_body = subst_ast(new_body, p, a);
                }
                return self.lower_expr(&new_body, env, current);
            }
        }


        if let ExprKind::Var(n) = &base_fun.kind {
            let arity = flat_args.len();
            let mut lowered_args = Vec::with_capacity(arity);
            for a in &flat_args {
                lowered_args.push(self.lower_expr(a, env, current));
            }

            if let Some(Binding::Function { id }) = env.get(n).cloned() {
                let info = &self.a.functions[id];
                if arity == info.params.len() && info.captures.is_empty() {
                    return LirExpr::Call {
                        target: CallTarget::Function(id),
                        args: lowered_args,
                        rep: info.ret_rep.clone(),
                        noreturn: false,
                        pure: info.pure,
                    };
                }
            }
            if let Some(Binding::Value { code, rep, .. }) = env.get(n).cloned() {
                match &rep {
                    Rep::Function(id)
                        if *id < self.a.functions.len()
                            && self.a.functions[*id].captures.is_empty()
                            && arity == self.a.functions[*id].params.len() =>
                    {
                        let info = &self.a.functions[*id];
                        return LirExpr::Call {
                            target: CallTarget::Function(*id),
                            args: lowered_args,
                            rep: info.ret_rep.clone(),
                            noreturn: false,
                            pure: info.pure,
                        };
                    }
                    Rep::ExternFunction {
                        args: fargs,
                        variadic,
                        ret,
                        ..
                    } if arity >= fargs.len() && (*variadic || arity == fargs.len()) => {
                        let callee = LirExpr::Var(code, rep.clone());
                        return LirExpr::Call {
                            target: CallTarget::Indirect(Box::new(callee)),
                            args: lowered_args,
                            rep: (**ret).clone(),
                            noreturn: false,
                            pure: false,
                        };
                    }
                    _ => {}
                }
            }
            if let Some(cid) = self.a.resolve_ctor_name(n) {
                if arity == self.a.constructors[cid].arity {
                    return LirExpr::Construct {
                        ctor: cid,
                        args: lowered_args,
                        rep: Rep::Adt(self.a.constructors[cid].type_name.clone()),
                    };
                }
            }
            let self_id = current.filter(|id| *id < self.a.functions.len());
            if let Some(sf) = self_id.and_then(|id| self.a.functions.get(id)) {
                if (sf.source_name.as_deref() == Some(n.as_str())
                    || self.a.resolve_function_name(n) == self_id)
                    && arity == sf.params.len()
                {
                    return LirExpr::Call {
                        target: CallTarget::Function(self_id.unwrap()),
                        args: lowered_args,
                        rep: sf.ret_rep.clone(),
                        noreturn: false,
                        pure: sf.pure,
                    };
                }
            }
            if let Some(cid) = self.a.unique_ctor_from_pattern(n, arity) {
                return LirExpr::Construct {
                    ctor: cid,
                    args: lowered_args,
                    rep: Rep::Adt(self.a.constructors[cid].type_name.clone()),
                };
            }
            if let Some(eid) = self.a.external_by_name.get(n).copied() {
                let ext = &self.a.externals[eid];
                let rep = match &ext.decl.foreign {
                    ForeignTy::Fun { ret, .. } => foreign_ret_rep(ret),
                    ForeignTy::Atom(_) => foreign_value_rep(&ext.decl.foreign),
                };
                return LirExpr::Call {
                    target: CallTarget::External(eid),
                    args: lowered_args,
                    rep,
                    noreturn: ext.noreturn,
                    pure: ext.decl.pure && !ext.noreturn,
                };
            }
            if let Some(fid) = self.a.resolve_function_name(n) {
                let info = &self.a.functions[fid];
                if arity == info.params.len() {
                    return LirExpr::Call {
                        target: CallTarget::Function(fid),
                        args: lowered_args,
                        rep: info.ret_rep.clone(),
                        noreturn: false,
                        pure: info.pure,
                    };
                }
            }
            if let Some(cid) = self.a.unique_ctor_from_pattern(n, arity) {
                return LirExpr::Construct {
                    ctor: cid,
                    args: lowered_args,
                    rep: Rep::Adt(self.a.constructors[cid].type_name.clone()),
                };
            }
            let arg_reps = lowered_args.iter().map(lir_rep).collect::<Vec<_>>();
            if let Some(cid) = self.a.best_ctor_recovery(n, arity, &arg_reps) {
                return LirExpr::Construct {
                    ctor: cid,
                    args: lowered_args,
                    rep: Rep::Adt(self.a.constructors[cid].type_name.clone()),
                };
            }
            if let Some(fid) = self.a.best_function_recovery(n, arity, &arg_reps, current) {
                let info = &self.a.functions[fid];
                return LirExpr::Call {
                    target: CallTarget::Function(fid),
                    args: lowered_args,
                    rep: info.ret_rep.clone(),
                    noreturn: false,
                    pure: info.pure,
                };
            }


            let named_candidates: Vec<usize> = self
                .a
                .constructors
                .iter()
                .filter(|c| c.name == *n && c.arity == arity)
                .map(|c| c.id)
                .collect();
            if !named_candidates.is_empty() {
                let hint_type: Option<String> = current
                    .and_then(|id| self.a.functions.get(id))
                    .and_then(|f| match &f.ret_rep {
                        Rep::Adt(t) => Some(t.clone()),
                        _ => None,
                    });
                if let Some(want) = hint_type.as_deref() {
                    let filtered: Vec<usize> = named_candidates
                        .iter()
                        .filter(|cid| self.a.constructors[**cid].type_name == want)
                        .copied()
                        .collect();
                    if filtered.len() == 1 {
                        let cid = filtered[0];
                        return LirExpr::Construct {
                            ctor: cid,
                            args: lowered_args,
                            rep: Rep::Adt(self.a.constructors[cid].type_name.clone()),
                        };
                    }
                }
                if named_candidates.len() == 1 {
                    let cid = named_candidates[0];
                    return LirExpr::Construct {
                        ctor: cid,
                        args: lowered_args,
                        rep: Rep::Adt(self.a.constructors[cid].type_name.clone()),
                    };
                }
                if !self.a.runtime_ctor_candidates(arity).is_empty() {
                    let query = match hint_type.as_deref() {
                        Some(t) => format!("{}:{}", n, t),
                        None => n.clone(),
                    };
                    let rep = match hint_type.as_deref() {
                        Some(t) => Rep::Adt(t.to_string()),
                        None => Rep::Ptr,
                    };
                    self.a.dbg(
                        "RUNTIME",
                        format!(
                            "{} ambiguous ctor '{}' /{} -> runtime ctor resolver (query={})",
                            current_label(current),
                            n,
                            arity,
                            query
                        ),
                    );
                    return LirExpr::RuntimeConstruct {
                        name: query,
                        args: lowered_args,
                        rep,
                    };
                }
            }


            if !self.a.runtime_function_candidates(arity).is_empty() {
                let rep = self.a.runtime_function_rep(n, arity, &arg_reps);
                self.a.dbg(
                    "RUNTIME",
                    format!(
                        "{} unresolved function '{}' /{} -> runtime fn resolver return={}",
                        current_label(current),
                        n,
                        arity,
                        rep_debug(&rep)
                    ),
                );
                return LirExpr::RuntimeCall {
                    name: n.clone(),
                    args: lowered_args,
                    rep,
                };
            }
            if !self.a.runtime_ctor_candidates(arity).is_empty() {
                self.a.dbg(
                    "RUNTIME",
                    format!(
                        "{} unresolved constructor '{}' /{} -> runtime ctor resolver",
                        current_label(current),
                        n,
                        arity
                    ),
                );
                return LirExpr::RuntimeConstruct {
                    name: n.clone(),
                    args: lowered_args,
                    rep: self.a.runtime_ctor_rep(n, arity),
                };
            }


            let fallback_rep = current
                .and_then(|id| self.a.functions.get(id))
                .map(|f| f.ret_rep.clone())
                .filter(|r| !matches!(r, Rep::Unknown))
                .unwrap_or(Rep::I64);
            self.a.dbg(
                "RUNTIME",
                format!(
                    "{} unresolved call '{}' /{} -> last-resort runtime call (rep={})",
                    current_label(current),
                    n,
                    arity,
                    rep_debug(&fallback_rep)
                ),
            );
            return LirExpr::RuntimeCall {
                name: n.clone(),
                args: lowered_args,
                rep: fallback_rep,
            };
        }


        let mut lowered_args = Vec::with_capacity(flat_args.len());
        for a in &flat_args {
            lowered_args.push(self.lower_expr(a, env, current));
        }
        let callee = self.lower_expr(&base_fun, env, current);

        if let LirExpr::FnRef(fid) = &callee {
            if *fid < self.a.functions.len()
                && self.a.functions[*fid].captures.is_empty()
                && lowered_args.len() == self.a.functions[*fid].params.len()
            {
                let info = &self.a.functions[*fid];
                return LirExpr::Call {
                    target: CallTarget::Function(*fid),
                    args: lowered_args,
                    rep: info.ret_rep.clone(),
                    noreturn: false,
                    pure: info.pure,
                };
            }
        }

        let callee_rep = lir_rep(&callee);
        let callee_is_callable = matches!(
            callee_rep,
            Rep::Function(_) | Rep::ExternFunction { .. } | Rep::Ptr
        ) || matches!(&callee, LirExpr::FnRef(_));

        if callee_is_callable {
            let ret_rep = match &callee_rep {
                Rep::ExternFunction { ret, .. } => (**ret).clone(),
                _ => match &callee {
                    LirExpr::FnRef(id) if *id < self.a.functions.len() => {
                        self.a.functions[*id].ret_rep.clone()
                    }
                    _ => current
                        .and_then(|id| self.a.functions.get(id))
                        .map(|f| f.ret_rep.clone())
                        .filter(|r| !matches!(r, Rep::Unknown))
                        .unwrap_or(Rep::I64),
                },
            };
            self.a.dbg(
                "RECOVER",
                format!(
                    "{} <complex> application -> indirect call via {} (callee-rep={}, ret={})",
                    current_label(current),
                    lir_kind_debug(&callee),
                    rep_debug(&callee_rep),
                    rep_debug(&ret_rep)
                ),
            );
            return LirExpr::Call {
                target: CallTarget::Indirect(Box::new(callee)),
                args: lowered_args,
                rep: ret_rep,
                noreturn: false,
                pure: false,
            };
        }


        let fallback_rep = current
            .and_then(|id| self.a.functions.get(id))
            .map(|f| f.ret_rep.clone())
            .filter(|r| !matches!(r, Rep::Unknown))
            .unwrap_or(Rep::I64);
        self.a.dbg(
            "RUNTIME",
            format!(
                "{} <complex> application of non-callable {} (rep={}) -> runtime recovery (rep={})",
                current_label(current),
                lir_kind_debug(&callee),
                rep_debug(&callee_rep),
                rep_debug(&fallback_rep)
            ),
        );
        LirExpr::RuntimeCall {
            name: String::new(),
            args: lowered_args,
            rep: fallback_rep,
        }
    }
    fn lower_pattern(
        &mut self,
        pat: &Pat,
        scrut: &Rep,
        env: &mut HashMap<String, Binding>,
    ) -> LirPat {
        match pat {
            Pat::Wild => LirPat::Wild,
            Pat::Bind(n) => {
                let code = self.fresh_local(n);
                let rep = scrut.clone();
                env.insert(
                    n.clone(),
                    Binding::Value {
                        code: code.clone(),
                        rep: rep.clone(),
                        ptr_info: None,
                    },
                );
                LirPat::Bind { code, rep }
            }
            Pat::Int(v) => LirPat::Int(*v),
            Pat::Str(s) => LirPat::Str(s.clone()),
            Pat::Ctor { name, args } => {
                if args.is_empty() {
                    if let Some(v) = self.a.global_int_const(name) {
                        return LirPat::Int(v as u64);
                    }
                }
                let cid = self.a.constructor_for_pattern(name, args.len(), scrut);
                if let Some(cid) = cid {
                    let mut out = Vec::with_capacity(args.len());
                    for (i, p) in args.iter().enumerate() {
                        let mut r = self.a.constructors[cid]
                            .fields
                            .get(i)
                            .cloned()
                            .or_else(|| {
                                self.a.constructors[cid]
                                    .args
                                    .get(i)
                                    .map(|t| rep_from_ty(t, &self.a.type_names))
                            })
                            .unwrap_or(Rep::Unknown);
                        if matches!(r, Rep::Unknown) {
                            if let Some(pr) = lower_pattern_rep(self.a, p) {
                                r = pr;
                            }
                        }
                        out.push(self.lower_pattern(p, &r, env));
                    }
                    return LirPat::Ctor {
                        ctor: cid,
                        args: out,
                    };
                }
                self.a.dbg(
                    "RECOVER",
                    format!(
                        "unresolved constructor pattern '{}' /{} for scrutinee {}",
                        name,
                        args.len(),
                        rep_debug(scrut)
                    ),
                );
                let mut out = Vec::with_capacity(args.len());
                for p in args {
                    let r = lower_pattern_rep(self.a, p).unwrap_or(Rep::Unknown);
                    out.push(self.lower_pattern(p, &r, env));
                }
                LirPat::Ctor {
                    ctor: usize::MAX,
                    args: out,
                }
            }
            Pat::Tuple(xs) => {
                let mut out = Vec::with_capacity(xs.len());
                for x in xs {
                    out.push(self.lower_pattern(x, &Rep::Ptr, env));
                }
                LirPat::Tuple(out)
            }
        }
    }
}

fn lower_pattern_rep(a: &Analyzer, pat: &Pat) -> Option<Rep> {
    match pat {
        Pat::Ctor { name, args } => a.resolve_ctor_name(name).and_then(|cid| {
            if args.len() == a.constructors[cid].arity {
                Some(Rep::Adt(a.constructors[cid].type_name.clone()))
            } else {
                None
            }
        }),
        Pat::Tuple(_) => Some(Rep::Ptr),
        Pat::Str(_) => Some(Rep::Str),
        Pat::Int(_) => Some(Rep::I64),
        Pat::Wild | Pat::Bind(_) => None,
    }
}


struct Optimizer<'a> {
    a: &'a Analyzer,
}

impl<'a> Optimizer<'a> {
    fn optimize_program(&self, mut fs: Vec<LirFunction>) -> Vec<LirFunction> {
        for f in &mut fs {
            let (body, _) = self.optimize_expr(f.body.clone());
            f.body = body;
        }
        let snapshot = fs
            .iter()
            .map(|f| (f.info.id, f.body.clone()))
            .collect::<HashMap<usize, LirExpr>>();
        for f in &mut fs {
            let (body, _) = self.inline_expr(f.body.clone(), &snapshot, f.info.id);
            f.body = body;
        }
        for f in &mut fs {
            let (body, _) = self.optimize_expr(f.body.clone());
            f.body = body;
        }
        fs
    }

    fn inline_expr(
        &self,
        e: LirExpr,
        bodies: &HashMap<usize, LirExpr>,
        current: usize,
    ) -> (LirExpr, bool) {
        match e {
            LirExpr::Call {
                target,
                args,
                rep,
                noreturn,
                pure,
            } => {
                let mut changed = false;
                let target = match target {
                    CallTarget::Indirect(callee) => {
                        let (x, c) = self.inline_expr(*callee, bodies, current);
                        changed |= c;
                        CallTarget::Indirect(Box::new(x))
                    }
                    x => x,
                };
                let mut args_out = Vec::with_capacity(args.len());
                for arg in args {
                    let (x, c) = self.inline_expr(arg, bodies, current);
                    changed |= c;
                    args_out.push(x);
                }
                if let CallTarget::Function(fid) = target {
                    if let Some(body) = bodies.get(&fid) {
                        if fid != current
                            && !self.a.recursive_functions.contains(&fid)
                            && fid < self.a.functions.len()
                            && self.a.functions[fid].captures.is_empty()
                            && pure
                            && !noreturn
                            && !rep_is_adt(&rep)
                            && args_out.len() == self.a.functions[fid].params.len()
                            && lir_size(body) <= 64
                            && lir_is_pure(body)
                            && args_out.iter().all(lir_is_pure)
                        {
                            let mut replacements = HashMap::with_capacity(args_out.len());
                            let mut safe = true;
                            for (i, arg) in args_out.iter().enumerate() {
                                let key = format!("a{}", i);
                                let uses = lir_count_var(body, &key);
                                if uses > 1 && !lir_is_simple_value(arg) {
                                    safe = false;
                                    break;
                                }
                                replacements.insert(key, arg.clone());
                            }
                            if safe {
                                return (lir_substitute_many(body.clone(), &replacements), true);
                            }
                        }
                    }
                    (
                        LirExpr::Call {
                            target: CallTarget::Function(fid),
                            args: args_out,
                            rep,
                            noreturn,
                            pure,
                        },
                        changed,
                    )
                } else {
                    (
                        LirExpr::Call {
                            target,
                            args: args_out,
                            rep,
                            noreturn,
                            pure,
                        },
                        changed,
                    )
                }
            }
            LirExpr::Construct { ctor, args, rep } => {
                let mut out = Vec::with_capacity(args.len());
                let mut changed = false;
                for arg in args {
                    let (x, c) = self.inline_expr(arg, bodies, current);
                    changed |= c;
                    out.push(x);
                }
                (
                    LirExpr::Construct {
                        ctor,
                        args: out,
                        rep,
                    },
                    changed,
                )
            }
            LirExpr::Field {
                base,
                ctor,
                index,
                rep,
            } => {
                let (base, changed) = self.inline_expr(*base, bodies, current);
                (
                    LirExpr::Field {
                        base: Box::new(base),
                        ctor,
                        index,
                        rep,
                    },
                    changed,
                )
            }
            LirExpr::Let {
                code,
                rep,
                value,
                body,
            } => {
                let (value, c1) = self.inline_expr(*value, bodies, current);
                let (body, c2) = self.inline_expr(*body, bodies, current);
                (
                    LirExpr::Let {
                        code,
                        rep,
                        value: Box::new(value),
                        body: Box::new(body),
                    },
                    c1 || c2,
                )
            }
            LirExpr::Block { stmts, tail } => {
                let mut out = Vec::with_capacity(stmts.len());
                let mut changed = false;
                for stmt in stmts {
                    let (x, c) = self.inline_expr(stmt, bodies, current);
                    changed |= c;
                    out.push(x);
                }
                let (tail, c) = self.inline_expr(*tail, bodies, current);
                changed |= c;
                (
                    LirExpr::Block {
                        stmts: out,
                        tail: Box::new(tail),
                    },
                    changed,
                )
            }
            LirExpr::Case { scrut, arms, rep } => {
                let (scrut, mut changed) = self.inline_expr(*scrut, bodies, current);
                let mut out = Vec::with_capacity(arms.len());
                for arm in arms {
                    let (body, c) = self.inline_expr(arm.body, bodies, current);
                    changed |= c;
                    out.push(LirArm { pat: arm.pat, body });
                }
                (
                    LirExpr::Case {
                        scrut: Box::new(scrut),
                        arms: out,
                        rep,
                    },
                    changed,
                )
            }
            LirExpr::Bin {
                op,
                left,
                right,
                rep,
            } => {
                let (left, c1) = self.inline_expr(*left, bodies, current);
                let (right, c2) = self.inline_expr(*right, bodies, current);
                (
                    LirExpr::Bin {
                        op,
                        left: Box::new(left),
                        right: Box::new(right),
                        rep,
                    },
                    c1 || c2,
                )
            }
            LirExpr::RuntimeCall { name, args, rep } => {
                let mut out = Vec::with_capacity(args.len());
                let mut any = false;
                for x in args {
                    let (y, c) = self.optimize_expr(x);
                    any |= c;
                    out.push(y);
                }
                (
                    LirExpr::RuntimeCall {
                        name,
                        args: out,
                        rep,
                    },
                    any,
                )
            }
            LirExpr::RuntimeConstruct { name, args, rep } => {
                let mut out = Vec::with_capacity(args.len());
                let mut any = false;
                for x in args {
                    let (y, c) = self.optimize_expr(x);
                    any |= c;
                    out.push(y);
                }
                (
                    LirExpr::RuntimeConstruct {
                        name,
                        args: out,
                        rep,
                    },
                    any,
                )
            }
            LirExpr::Un { op, expr, rep } => {
                let (expr, changed) = self.inline_expr(*expr, bodies, current);
                (
                    LirExpr::Un {
                        op,
                        expr: Box::new(expr),
                        rep,
                    },
                    changed,
                )
            }
            x => (x, false),
        }
    }

    fn optimize_expr(&self, e: LirExpr) -> (LirExpr, bool) {
        let (base, mut changed) = match e {
            LirExpr::Call {
                target,
                args,
                rep,
                noreturn,
                pure,
            } => {
                let (target, target_changed) = match target {
                    CallTarget::Indirect(callee) => {
                        let (c, ch) = self.optimize_expr(*callee);
                        (CallTarget::Indirect(Box::new(c)), ch)
                    }
                    x => (x, false),
                };
                let mut out = Vec::with_capacity(args.len());
                let mut c = target_changed;
                for a in args {
                    let (x, ch) = self.optimize_expr(a);
                    out.push(x);
                    c |= ch;
                }
                (
                    LirExpr::Call {
                        target,
                        args: out,
                        rep,
                        noreturn,
                        pure,
                    },
                    c,
                )
            }
            LirExpr::Construct { ctor, args, rep } => {
                let mut out = Vec::with_capacity(args.len());
                let mut c = false;
                for a in args {
                    let (x, ch) = self.optimize_expr(a);
                    out.push(x);
                    c |= ch;
                }
                (
                    LirExpr::Construct {
                        ctor,
                        args: out,
                        rep,
                    },
                    c,
                )
            }
            LirExpr::Field {
                base,
                ctor,
                index,
                rep,
            } => {
                let (b, c) = self.optimize_expr(*base);
                (
                    LirExpr::Field {
                        base: Box::new(b),
                        ctor,
                        index,
                        rep,
                    },
                    c,
                )
            }
            LirExpr::Let {
                code,
                rep,
                value,
                body,
            } => {
                let (v, c1) = self.optimize_expr(*value);
                let (b, c2) = self.optimize_expr(*body);
                let uses = lir_count_var(&b, &code);
                if uses == 0 && lir_is_pure(&v) {
                    (b, true)
                } else if uses == 1 && lir_is_inlineable(&v) && !matches!(b, LirExpr::Case { .. }) {
                    (lir_substitute(b, &code, &v), true)
                } else {
                    (
                        LirExpr::Let {
                            code,
                            rep,
                            value: Box::new(v),
                            body: Box::new(b),
                        },
                        c1 || c2,
                    )
                }
            }
            LirExpr::Block { stmts, tail } => {
                let mut out = Vec::with_capacity(stmts.len());
                let mut c = false;
                for s in stmts {
                    let (x, ch) = self.optimize_expr(s);
                    c |= ch;
                    if matches!(x, LirExpr::Unit) && lir_is_pure(&x) {
                        c = true;
                    } else {
                        out.push(x);
                    }
                }
                let (t, ch) = self.optimize_expr(*tail);
                c |= ch;
                (
                    LirExpr::Block {
                        stmts: out,
                        tail: Box::new(t),
                    },
                    c,
                )
            }
            LirExpr::Case { scrut, arms, rep } => {
                let (s, c1) = self.optimize_expr(*scrut);
                let mut out = Vec::with_capacity(arms.len());
                let mut c = c1;
                for arm in arms {
                    let (b, ch) = self.optimize_expr(arm.body);
                    c |= ch;
                    out.push(LirArm {
                        pat: arm.pat,
                        body: b,
                    });
                }
                (
                    LirExpr::Case {
                        scrut: Box::new(s),
                        arms: out,
                        rep,
                    },
                    c,
                )
            }
            LirExpr::Bin {
                op,
                left,
                right,
                rep,
            } => {
                let (l, c1) = self.optimize_expr(*left);
                let (r, c2) = self.optimize_expr(*right);
                (
                    LirExpr::Bin {
                        op,
                        left: Box::new(l),
                        right: Box::new(r),
                        rep,
                    },
                    c1 || c2,
                )
            }
            LirExpr::Un { op, expr, rep } => {
                let (x, c) = self.optimize_expr(*expr);
                (
                    LirExpr::Un {
                        op,
                        expr: Box::new(x),
                        rep,
                    },
                    c,
                )
            }
            x => (x, false),
        };
        let (simplified, local) = self.simplify(base);
        changed |= local;
        (simplified, changed)
    }

    fn simplify(&self, e: LirExpr) -> (LirExpr, bool) {
        match e {
            LirExpr::Let {
                code,
                rep,
                value,
                body,
            } => {
                let value = *value;
                let body = *body;
                let uses = lir_count_var(&body, &code);
                if uses == 0 && lir_is_pure(&value) {
                    return (body, true);
                }
                if uses > 0 && lir_is_constant(&value) {
                    return (lir_substitute(body, &code, &value), true);
                }
                if uses == 1 && lir_is_simple_value(&value) {
                    return (lir_substitute(body, &code, &value), true);
                }
                if uses == 1 && lir_is_pure(&value) && lir_size(&value) <= 12 {
                    return (lir_substitute(body, &code, &value), true);
                }
                (
                    LirExpr::Let {
                        code,
                        rep,
                        value: Box::new(value),
                        body: Box::new(body),
                    },
                    false,
                )
            }
            LirExpr::Block { stmts, tail } => {
                let mut out = Vec::with_capacity(stmts.len());
                let mut changed = false;
                for stmt in stmts {
                    match stmt {
                        LirExpr::Block {
                            stmts: inner,
                            tail: inner_tail,
                        } => {
                            out.extend(inner);
                            out.push(*inner_tail);
                            changed = true;
                        }
                        x if lir_is_pure(&x) => {
                            changed = true;
                        }
                        x => out.push(x),
                    }
                }
                while matches!(out.last(), Some(x) if lir_is_pure(x)) {
                    out.pop();
                    changed = true;
                }
                let tail = *tail;
                if out.is_empty() {
                    return (tail, true);
                }
                (
                    LirExpr::Block {
                        stmts: out,
                        tail: Box::new(tail),
                    },
                    changed,
                )
            }
            LirExpr::Bin {
                op,
                left,
                right,
                rep,
            } => {
                let l = *left;
                let r = *right;
                if let Some(v) = fold_bin(op, &l, &r, rep.clone()) {
                    return (v, true);
                }
                if lir_is_pure(&l) && lir_same_integer_value(&l, &r) && rep.is_integer() {
                    return match op {
                        BinOpLir::Eq | BinOpLir::Le | BinOpLir::Ge => {
                            (LirExpr::Int(1, Rep::I32), true)
                        }
                        BinOpLir::Ne | BinOpLir::Lt | BinOpLir::Gt => {
                            (LirExpr::Int(0, Rep::I32), true)
                        }
                        BinOpLir::Sub | BinOpLir::Xor => (LirExpr::Int(0, rep.clone()), true),
                        BinOpLir::And | BinOpLir::Or => (l, true),
                        _ => (
                            LirExpr::Bin {
                                op,
                                left: Box::new(l),
                                right: Box::new(r),
                                rep,
                            },
                            false,
                        ),
                    };
                }
                if rep.is_integer() {
                    if lir_is_const_zero(&r) && lir_is_pure(&l) {
                        match op {
                            BinOpLir::Add
                            | BinOpLir::Sub
                            | BinOpLir::Or
                            | BinOpLir::Xor
                            | BinOpLir::Shl
                            | BinOpLir::Shr => return (l, true),
                            BinOpLir::Mul | BinOpLir::And => {
                                return (LirExpr::Int(0, rep.clone()), true);
                            }
                            _ => {}
                        }
                    }
                    if lir_is_const_one(&r) && lir_is_pure(&l) {
                        match op {
                            BinOpLir::Mul | BinOpLir::Div => return (l, true),
                            BinOpLir::Rem => return (LirExpr::Int(0, rep.clone()), true),
                            _ => {}
                        }
                    }
                    if lir_is_const_zero(&l) && lir_is_pure(&r) {
                        match op {
                            BinOpLir::Add | BinOpLir::Or | BinOpLir::Xor => return (r, true),
                            BinOpLir::Mul | BinOpLir::And => {
                                return (LirExpr::Int(0, rep.clone()), true);
                            }
                            _ => {}
                        }
                    }
                    if lir_is_const_one(&l) && lir_is_pure(&r) && matches!(op, BinOpLir::Mul) {
                        return (r, true);
                    }
                }
                match op {
                    BinOpLir::LAnd => {
                        if lir_is_const_zero(&l) {
                            return (LirExpr::Int(0, Rep::I32), true);
                        }
                        if lir_is_const_zero(&r) && lir_is_pure(&l) {
                            return (LirExpr::Int(0, Rep::I32), true);
                        }
                        if lir_is_const_one(&l) && lir_is_pure(&r) {
                            return (
                                LirExpr::Un {
                                    op: UnOpLir::Not,
                                    expr: Box::new(LirExpr::Un {
                                        op: UnOpLir::Not,
                                        expr: Box::new(r),
                                        rep: Rep::I32,
                                    }),
                                    rep: Rep::I32,
                                },
                                true,
                            );
                        }
                        if lir_is_const_one(&r) && lir_is_pure(&l) {
                            return (
                                LirExpr::Un {
                                    op: UnOpLir::Not,
                                    expr: Box::new(LirExpr::Un {
                                        op: UnOpLir::Not,
                                        expr: Box::new(l),
                                        rep: Rep::I32,
                                    }),
                                    rep: Rep::I32,
                                },
                                true,
                            );
                        }
                    }
                    BinOpLir::LOr => {
                        if lir_is_const_one(&l) {
                            return (LirExpr::Int(1, Rep::I32), true);
                        }
                        if lir_is_const_one(&r) && lir_is_pure(&l) {
                            return (LirExpr::Int(1, Rep::I32), true);
                        }
                        if lir_is_const_zero(&l) && lir_is_pure(&r) {
                            return (
                                LirExpr::Un {
                                    op: UnOpLir::Not,
                                    expr: Box::new(LirExpr::Un {
                                        op: UnOpLir::Not,
                                        expr: Box::new(r),
                                        rep: Rep::I32,
                                    }),
                                    rep: Rep::I32,
                                },
                                true,
                            );
                        }
                        if lir_is_const_zero(&r) && lir_is_pure(&l) {
                            return (
                                LirExpr::Un {
                                    op: UnOpLir::Not,
                                    expr: Box::new(LirExpr::Un {
                                        op: UnOpLir::Not,
                                        expr: Box::new(l),
                                        rep: Rep::I32,
                                    }),
                                    rep: Rep::I32,
                                },
                                true,
                            );
                        }
                    }
                    _ => {}
                }
                (
                    LirExpr::Bin {
                        op,
                        left: Box::new(l),
                        right: Box::new(r),
                        rep,
                    },
                    false,
                )
            }
            LirExpr::Un { op, expr, rep } => {
                let x = *expr;
                if let Some(v) = fold_un(op, &x, rep.clone()) {
                    return (v, true);
                }
                if matches!(op, UnOpLir::BitNot) {
                    if let LirExpr::Un {
                        op: UnOpLir::BitNot,
                        expr: inner,
                        ..
                    } = x
                    {
                        return (*inner, true);
                    }
                }
                if matches!(op, UnOpLir::Not) {
                    if let LirExpr::Un {
                        op: UnOpLir::Not,
                        expr: inner,
                        ..
                    } = x
                    {
                        return (*inner, true);
                    }
                }
                (
                    LirExpr::Un {
                        op,
                        expr: Box::new(x),
                        rep,
                    },
                    false,
                )
            }
            LirExpr::Case { scrut, arms, rep } => {
                if let Some(v) = select_constant_case(&scrut, &arms) {
                    return (v, true);
                }
                if let Some(v) = select_construct_case(self.a, &scrut, &arms) {
                    return (v, true);
                }
                let sr = lir_rep(&scrut);
                let mut out: Vec<LirArm> = Vec::with_capacity(arms.len());
                let mut case_changed = false;
                for arm in &arms {
                    if matches!(
                        lir_pattern_provably_false(self.a, &sr, &arm.pat),
                        Some(true)
                    ) {
                        case_changed = true;
                        continue;
                    }
                    if out.iter().any(|old| lir_pat_shape_eq(&old.pat, &arm.pat)) {
                        continue;
                    }
                    let is_default = matches!(arm.pat, LirPat::Wild | LirPat::Bind { .. });
                    out.push(arm.clone());
                    if is_default {
                        break;
                    }
                }
                if out.is_empty() {
                    return (LirExpr::Case { scrut, arms, rep }, false);
                }
                if out.len() == 1 && lir_is_pure(&scrut) {
                    if let Some(arm) = out.first() {
                        match &arm.pat {
                            LirPat::Wild => return (arm.body.clone(), true),
                            LirPat::Bind { code, .. } => {
                                return (
                                    lir_substitute(arm.body.clone(), code.as_str(), &scrut),
                                    true,
                                );
                            }
                            _ => {}
                        }
                    }
                }
                case_changed |= out.len() != arms.len();
                (
                    LirExpr::Case {
                        scrut,
                        arms: out,
                        rep,
                    },
                    case_changed,
                )
            }
            x => (x, false),
        }
    }
}


struct CEmitter<'a> {
    a: &'a Analyzer,
    plan: RtPlan,
    memory: MemoryPlan,
    out: String,
    temp: usize,
    current_ret: Rep,
    current_function: Option<usize>,
    current_out: Option<String>,
}

impl<'a> CEmitter<'a> {
    fn new(a: &'a Analyzer) -> Self {
        Self {
            a,
            plan: RtPlan::default(),
            memory: MemoryPlan::default(),
            out: String::new(),
            temp: 0,
            current_ret: Rep::Unit,
            current_function: None,
            current_out: None,
        }
    }

    fn emit_program(&mut self, p: &Program, fs: &[LirFunction]) -> String {
        self.out.clear();
        let entry = find_structural_entry(p, &self.a.function_by_source);
        let emitted_function_ids = fs
            .iter()
            .map(|f| f.info.id)
            .filter(|id| Some(*id) != entry)
            .collect::<BTreeSet<_>>();
        self.plan = RtPlan::detect(self.a, fs, &emitted_function_ids);
        self.memory = MemoryPlan::analyze(self.a, p, fs);
        self.a.dbg(
            "MEM",
            format!(
                "whole-AST nodes={} LIR nodes={} allocation-sites={} stack={} arena={} heap={} rounds={} converged={} est(stack={}B arena={}B heap={}B)",
                self.memory.ast_nodes,
                self.memory.lir_nodes,
                self.memory.allocation_sites,
                self.memory.stack_sites,
                self.memory.arena_sites,
                self.memory.heap_sites,
                self.memory.rounds,
                self.memory.converged,
                self.memory.estimated_stack_bytes,
                self.memory.estimated_arena_bytes,
                self.memory.estimated_heap_bytes
            ),
        );

        let mut headers = BTreeSet::new();
        headers.insert("#include <stdint.h>".to_string());
        if self.plan.on(RtFeature::Arena) || !self.a.constructors.is_empty() {
            headers.insert("#include <stddef.h>".to_string());
        }
        if self.plan.on(RtFeature::Arena) {
            headers.insert("#include <stdatomic.h>".to_string());
        }
        headers.insert("#include <stdio.h>".to_string());
        headers.insert("#include <stdlib.h>".to_string());


        headers.insert("#include <string.h>".to_string());
        if self.a.needs_string
            || !self.a.constructors.is_empty()
            || self.plan.on(RtFeature::FoldEq)
            || self.plan.on(RtFeature::CanonEq)
        {
            headers.insert("#include <string.h>".to_string());
        }
        if self.a.needs_stdlib {
            headers.insert("#include <stdlib.h>".to_string());
        }
        for e in &self.a.externals {
            if let Some(h) = &e.decl.from {
                if h.starts_with('<') || h.starts_with('"') {
                    headers.insert(format!("#include {}", h));
                } else {
                    headers.insert(format!("#include <{}>", h));
                }
            }
        }
        for h in headers {
            let _ = writeln!(self.out, "{}", h);
        }
        self.out.push('\n');

        self.emit_integer_wrap_helpers();
        self.emit_types();
        if self.plan.on(RtFeature::Arena) {
            self.emit_arena_helper();
        }
        self.emit_externals();
        self.emit_prototypes(fs);
        if self.plan.on(RtFeature::Recovery) {
            self.emit_recovery_helper();
        }
        if !self.a.constructors.is_empty() {
            self.emit_adt_runtime_helper();
        }
        if self.plan.any_dispatcher() {
            self.emit_runtime_resolver();
        }
        self.emit_globals(p);

        for f in fs {
            if Some(f.info.id) == entry {
                continue;
            }
            self.emit_function(f);
        }
        self.out.push('\n');
        self.emit_main(p, fs, entry);
        std::mem::take(&mut self.out)
    }

    fn emit_integer_wrap_helpers(&mut self) {


        let _ = writeln!(self.out, "int64_t mak_ir_i64_from_bits(uint64_t bits) {{ int64_t out; memcpy(&out, &bits, sizeof out); return out; }}");
        let _ = writeln!(self.out, "int64_t mak_ir_i64_neg_wrap(int64_t a) {{ return mak_ir_i64_from_bits(UINT64_C(0) - (uint64_t)a); }}");
        let _ = writeln!(self.out, "int64_t mak_ir_i64_add_wrap(int64_t a, int64_t b) {{ return mak_ir_i64_from_bits((uint64_t)a + (uint64_t)b); }}");
        let _ = writeln!(self.out, "int64_t mak_ir_i64_sub_wrap(int64_t a, int64_t b) {{ return mak_ir_i64_from_bits((uint64_t)a - (uint64_t)b); }}");
        let _ = writeln!(self.out, "int64_t mak_ir_i64_mul_wrap(int64_t a, int64_t b) {{ return mak_ir_i64_from_bits((uint64_t)a * (uint64_t)b); }}");
        let _ = writeln!(self.out, "int64_t mak_ir_i64_div_wrap(int64_t a, int64_t b) {{ if (a == INT64_MIN && b == -1) return INT64_MIN; return a / b; }}");
        let _ = writeln!(self.out, "int64_t mak_ir_i64_rem_wrap(int64_t a, int64_t b) {{ if (a == INT64_MIN && b == -1) return 0; return a % b; }}");
        let _ = writeln!(self.out, "int64_t mak_ir_i64_shl_wrap(int64_t a, int64_t b) {{ return mak_ir_i64_from_bits((uint64_t)a << ((uint64_t)b & UINT64_C(63))); }}");
        let _ = writeln!(self.out, "int64_t mak_ir_i64_shr(int64_t a, int64_t b) {{ uint64_t n = (uint64_t)b & UINT64_C(63); uint64_t bits; memcpy(&bits, &a, sizeof bits); if (n == 0) return a; return mak_ir_i64_from_bits((bits >> n) | (a < 0 ? (UINT64_MAX << (64 - n)) : UINT64_C(0))); }}");
        self.out.push('\n');
    }

    fn emit_types(&mut self) {
        if !self.a.constructors.is_empty() {
            let _ = writeln!(self.out, "enum {{");
            for c in &self.a.constructors {
                let _ = writeln!(self.out, "    {} = {}u,", c.tag, c.id + 1);
            }
            let _ = writeln!(self.out, "}};\n");
        }
        let mut groups: BTreeMap<String, Vec<CtorInfo>> = BTreeMap::new();
        for c in &self.a.constructors {
            groups
                .entry(c.type_name.clone())
                .or_default()
                .push(c.clone());
        }
        for (name, mut ctors) in groups {
            ctors.sort_by_key(|c| c.id);
            let t = format!("mak_{}", mangle_ident(&name));
            let _ = writeln!(self.out, "struct {} {{", t);
            let _ = writeln!(self.out, "    uint32_t tag;");
            if ctors.iter().any(|c| c.arity > 0) {
                if ctors.len() == 1 {
                    let c = &ctors[0];
                    let _ = writeln!(self.out, "    struct {{");
                    for (i, r) in c.fields.iter().enumerate() {
                        let _ =
                            writeln!(self.out, "        {} f{};", r.ctype(&self.a.functions), i);
                    }
                    let _ = writeln!(self.out, "    }} v;");
                } else {
                    let _ = writeln!(self.out, "    union {{");


                    // A union belongs to exactly one ADT. Including constructors
                    // from unrelated types silently inflates layouts and can make
                    // checked copies/allocations unnecessarily expensive.
                    for c in &ctors {
                        if c.arity == 0 {
                            continue;
                        }
                        let _ = writeln!(self.out, "        struct {{");
                        for (i, r) in c.fields.iter().enumerate() {
                            let _ = writeln!(
                                self.out,
                                "            {} f{};",
                                r.ctype(&self.a.functions),
                                i
                            );
                        }
                        let _ = writeln!(self.out, "        }} v{};", c.id);
                    }
                    let _ = writeln!(self.out, "    }} v;");
                }
            }
            let _ = writeln!(self.out, "}};\n");
            for c in &ctors {
                if c.arity == 0 {
                    let _ = writeln!(
                        self.out,
                        "static struct {} mak_ir_ctor_obj_{} = {{ {} }};",
                        t, c.id, c.tag
                    );
                }
            }
            if let Some(first) = ctors.first() {
                let _ = writeln!(
                    self.out,
                    "static struct {} mak_ir_default_obj_{} = {{ {} }};",
                    t,
                    mangle_ident(&name),
                    first.tag
                );
            }
        }
    }

    fn emit_adt_runtime_helper(&mut self) {


        let _ = writeln!(
            self.out,
            "static inline uint32_t mak_ir_adt_type_of_tag(uint32_t tag) {{"
        );
        let _ = writeln!(self.out, "    switch (tag) {{");
        for c in &self.a.constructors {
            let type_id = adt_type_id(&c.type_name);
            let _ = writeln!(
                self.out,
                "    case {}: return UINT32_C({});",
                c.tag, type_id
            );
        }
        let _ = writeln!(self.out, "    default: return 0u;");
        let _ = writeln!(self.out, "    }}");
        let _ = writeln!(self.out, "}}");

        let _ = writeln!(
            self.out,
            "static inline const void *mak_ir_adt_cast_checked(const void *src, uint32_t dst_type, const void *fallback) {{"
        );
        let _ = writeln!(
            self.out,
            "    if (!src) {{ mak_ir_recover(\"null ADT cast\"); return fallback; }}"
        );
        let _ = writeln!(
            self.out,
            "    uint32_t src_type = mak_ir_adt_type_of_tag(*(const uint32_t *)src);"
        );
        let _ = writeln!(
            self.out,
            "    if (src_type == 0u || dst_type == 0u) {{ mak_ir_recover(\"unknown ADT runtime type\"); return fallback; }}"
        );
        let _ = writeln!(
            self.out,
            "    if (src_type == dst_type) return src;"
        );
        let _ = writeln!(
            self.out,
            "    mak_ir_recover(\"ADT runtime cast mismatch\");"
        );
        let _ = writeln!(self.out, "    return fallback;");
        let _ = writeln!(self.out, "}}");

        let _ = writeln!(
            self.out,
            "static inline const void *mak_ir_adt_recover_ptr(const void *fallback, const char *reason) {{"
        );
        let _ = writeln!(self.out, "    mak_ir_recover(reason);");
        let _ = writeln!(self.out, "    return fallback;");
        let _ = writeln!(self.out, "}}");

        let _ = writeln!(
            self.out,
            "static inline int mak_ir_adt_copy_checked(void *dst, size_t dst_size, uint32_t dst_type, const void *src) {{"
        );
        let _ = writeln!(
            self.out,
            "    if (!dst || !src) {{ mak_ir_recover(\"null ADT value\"); return 0; }}"
        );
        let _ = writeln!(
            self.out,
            "    uint32_t src_tag = *(const uint32_t *)src;"
        );
        let _ = writeln!(
            self.out,
            "    uint32_t src_type = mak_ir_adt_type_of_tag(src_tag);"
        );
        let _ = writeln!(
            self.out,
            "    if (src_type == 0u || dst_type == 0u) {{ mak_ir_recover(\"unknown ADT runtime type\"); return 0; }}"
        );
        let _ = writeln!(
            self.out,
            "    if (src_type == dst_type) {{ memcpy(dst, src, dst_size); return 1; }}"
        );

        let _ = writeln!(self.out, "    switch (dst_type) {{");

        let mut dest_types: Vec<String> = self
            .a
            .constructors
            .iter()
            .map(|c| c.type_name.clone())
            .collect();
        dest_types.sort();
        dest_types.dedup();

        for dst_name in dest_types {
            let dst_ctors: Vec<&CtorInfo> = self
                .a
                .constructors
                .iter()
                .filter(|c| c.type_name == dst_name)
                .collect();
            let dst_id = adt_type_id(&dst_name);

            let _ = writeln!(
                self.out,
                "    case UINT32_C({}): {{",
                dst_id
            );
            let _ = writeln!(self.out, "        switch (src_tag) {{");

            for src_ctor in &self.a.constructors {
                let mut matches: Vec<&CtorInfo> = Vec::new();

                for dst_ctor in &dst_ctors {
                    let compatible = if src_ctor.arity != dst_ctor.arity {
                        false
                    } else if src_ctor.arity == 0 {
                        let src_suffix = src_ctor
                            .name
                            .trim_start_matches(|ch: char| ch.is_ascii_uppercase());
                        let dst_suffix = dst_ctor
                            .name
                            .trim_start_matches(|ch: char| ch.is_ascii_uppercase());
                        !src_suffix.is_empty() && src_suffix == dst_suffix
                    } else {
                        src_ctor.fields.len() == dst_ctor.fields.len()
                            && src_ctor
                                .fields
                                .iter()
                                .zip(dst_ctor.fields.iter())
                                .all(|(a, b)| a == b)
                    };

                    if compatible {
                        matches.push(*dst_ctor);
                    }
                }

                if matches.len() != 1 {
                    continue;
                }

                let dst_ctor = matches[0];
                let _ = writeln!(
                    self.out,
                    "        case {}: {{",
                    src_ctor.tag
                );
                let _ = writeln!(
                    self.out,
                    "            struct mak_{} *s = (struct mak_{} *)(uintptr_t)src;",
                    mangle_ident(&src_ctor.type_name),
                    mangle_ident(&src_ctor.type_name)
                );
                let _ = writeln!(
                    self.out,
                    "            struct mak_{} *d = (struct mak_{} *)dst;",
                    mangle_ident(&dst_ctor.type_name),
                    mangle_ident(&dst_ctor.type_name)
                );
                let _ = writeln!(
                    self.out,
                    "            d->tag = {};",
                    dst_ctor.tag
                );

                for i in 0..src_ctor.arity {
                    let sp = constructor_field_prefix(self.a, src_ctor.id);
                    let dp = constructor_field_prefix(self.a, dst_ctor.id);
                    let _ = writeln!(
                        self.out,
                        "            d->{}f{} = s->{}f{};",
                        dp,
                        i,
                        sp,
                        i
                    );
                }

                let _ = writeln!(self.out, "            return 1;");
                let _ = writeln!(self.out, "        }}");
            }

            let _ = writeln!(
                self.out,
                "        default: break;"
            );
            let _ = writeln!(self.out, "        }}");
            let _ = writeln!(self.out, "        break;");
            let _ = writeln!(self.out, "    }}");
        }

        let _ = writeln!(self.out, "    default: break;");
        let _ = writeln!(self.out, "    }}");
        let _ = writeln!(
            self.out,
            "    mak_ir_recover(\"ADT runtime representation mismatch\");"
        );
        let _ = writeln!(self.out, "    return 0;");
        let _ = writeln!(self.out, "}}");
        self.out.push('\n');
    }

    fn emit_arena_helper(&mut self) {


        let _ = writeln!(self.out, "#ifndef MAK_ARENA_PAGE_BYTES");
        let _ = writeln!(
            self.out,
            "#define MAK_ARENA_PAGE_BYTES (64ULL * 1024ULL)"
        );
        let _ = writeln!(self.out, "#endif");
        let _ = writeln!(self.out, "#ifndef MAK_ARENA_BYTES");
        let _ = writeln!(
            self.out,
            "#define MAK_ARENA_BYTES (256ULL * 1024ULL * 1024ULL)"
        );
        let _ = writeln!(self.out, "#endif");
        let _ = writeln!(self.out, "#ifndef MAK_ARENA_MAX_BYTES");
        let _ = writeln!(
            self.out,
            "#define MAK_ARENA_MAX_BYTES MAK_ARENA_BYTES"
        );
        let _ = writeln!(self.out, "#endif");
        let _ = writeln!(self.out, "#ifndef MAK_ARENA_HARDEN");
        let _ = writeln!(self.out, "#define MAK_ARENA_HARDEN 1");
        let _ = writeln!(self.out, "#endif");
        let _ = writeln!(self.out, "#ifndef MAK_ARENA_ABORT_ON_CORRUPTION");
        let _ = writeln!(self.out, "#define MAK_ARENA_ABORT_ON_CORRUPTION 1");
        let _ = writeln!(self.out, "#endif");
        let _ = writeln!(self.out, "#ifndef MAK_ARENA_QUARANTINE");
        let _ = writeln!(self.out, "#define MAK_ARENA_QUARANTINE 64u");
        let _ = writeln!(self.out, "#endif");
        let _ = writeln!(self.out, "#ifndef MAK_ARENA_MAX_ALIGN");
        let _ = writeln!(self.out, "#define MAK_ARENA_MAX_ALIGN (1ULL << 30)");
        let _ = writeln!(self.out, "#endif");
        let _ = writeln!(self.out, "#define MAK_IR_HEAP_MAGIC UINT64_C(0x4d414b4845415031)");
        let _ = writeln!(self.out, "#define MAK_IR_HEAP_LINK_MAGIC UINT64_C(0x4d414b4c494e4848)");
        let _ = writeln!(self.out, "#define MAK_IR_HEAP_TAIL UINT64_C(0x4d414b5441494c48)");
        let _ = writeln!(self.out, "struct mak_ir_heap_hdr {{ uint64_t magic; uint64_t cookie; size_t total; size_t payload_off; size_t size; size_t alignment; }};");
        let _ = writeln!(self.out, "struct mak_ir_heap_link {{ uint64_t magic; struct mak_ir_heap_hdr *hdr; }};");
        let _ = writeln!(self.out, "static inline uint64_t mak_ir_heap_cookie(const struct mak_ir_heap_hdr *h) {{ uintptr_t x=(uintptr_t)h; return MAK_IR_HEAP_MAGIC ^ (uint64_t)x ^ UINT64_C(0x9e3779b97f4a7c15); }}");
        let _ = writeln!(self.out, "static inline void mak_ir_heap_fault(const char *reason) {{ if(MAK_ARENA_ABORT_ON_CORRUPTION) {{ fprintf(stderr, \"[MAK:MEM] heap fault: %s\\n\", reason); abort(); }} }}");
        let _ = writeln!(self.out, "static inline void *mak_ir_heap_alloc(size_t size, size_t align) {{");
        let _ = writeln!(self.out, "    if (size == 0) size = 1; if (align == 0) align = _Alignof(max_align_t); if ((align & (align-1)) != 0 || align > MAK_ARENA_MAX_ALIGN) return NULL;");
        let _ = writeln!(self.out, "    size_t base=sizeof(struct mak_ir_heap_hdr)+sizeof(struct mak_ir_heap_link); if(base>SIZE_MAX-(align-1)) return NULL; size_t pre=base+(align-1); if(size>SIZE_MAX-pre) return NULL; size_t total=pre+size; if(sizeof(uint64_t)>SIZE_MAX-total) return NULL; total+=sizeof(uint64_t);");
        let _ = writeln!(self.out, "    unsigned char *raw=(unsigned char *)malloc(total); if(!raw) return NULL; uintptr_t q=(uintptr_t)raw+sizeof(struct mak_ir_heap_hdr)+sizeof(struct mak_ir_heap_link); uintptr_t mask=(uintptr_t)align-1; if(q>UINTPTR_MAX-mask){{free(raw);return NULL;}} uintptr_t payload=(q+mask)&~mask; size_t off=(size_t)(payload-(uintptr_t)raw); if(off>total-size-sizeof(uint64_t)-sizeof(struct mak_ir_heap_link)){{free(raw);return NULL;}} struct mak_ir_heap_hdr *h=(struct mak_ir_heap_hdr *)(void *)raw; struct mak_ir_heap_link *ln=(struct mak_ir_heap_link *)(void *)(payload-sizeof(struct mak_ir_heap_link)); memset(h,0,sizeof(*h)); h->magic=MAK_IR_HEAP_MAGIC; h->total=total; h->payload_off=off; h->size=size; h->alignment=align; h->cookie=mak_ir_heap_cookie(h); ln->magic=MAK_IR_HEAP_LINK_MAGIC; ln->hdr=h; uint64_t *tail=(uint64_t *)(void *)(payload+size); *tail=MAK_IR_HEAP_TAIL ^ (uint64_t)(uintptr_t)h ^ (uint64_t)size; if(MAK_ARENA_HARDEN) memset((void *)payload,0,size); return (void *)payload;");
        let _ = writeln!(self.out, "}}");
        let _ = writeln!(self.out, "static inline void mak_ir_heap_free(void *ptr) {{ if(!ptr) return; unsigned char *payload=(unsigned char *)ptr; if((uintptr_t)payload<sizeof(struct mak_ir_heap_link)){{mak_ir_heap_fault(\"invalid heap pointer\");return;}} struct mak_ir_heap_link *ln=(struct mak_ir_heap_link *)(void *)(payload-sizeof(struct mak_ir_heap_link)); if(ln->magic!=MAK_IR_HEAP_LINK_MAGIC || !ln->hdr){{mak_ir_heap_fault(\"invalid heap allocation link\");return;}} struct mak_ir_heap_hdr *h=ln->hdr; if(h->magic!=MAK_IR_HEAP_MAGIC || h->cookie!=mak_ir_heap_cookie(h) || h->payload_off<sizeof(struct mak_ir_heap_hdr)+sizeof(struct mak_ir_heap_link) || h->payload_off>h->total-h->size-sizeof(uint64_t) || h->alignment==0 || (h->alignment&(h->alignment-1))!=0){{mak_ir_heap_fault(\"invalid heap header\");return;}} unsigned char *raw=payload-h->payload_off; if(raw!=((unsigned char *)h)){{mak_ir_heap_fault(\"heap base mismatch\");return;}} uint64_t *tail=(uint64_t *)(void *)(payload+h->size); if(*tail!=(MAK_IR_HEAP_TAIL ^ (uint64_t)(uintptr_t)h ^ (uint64_t)h->size)){{mak_ir_heap_fault(\"heap allocation canary corrupted\");return;}} if(MAK_ARENA_HARDEN) memset(payload,0xdd,h->size); h->magic=0; h->cookie=0; ln->magic=0; free(raw); }}");
        let _ = writeln!(self.out, "#if MAK_ARENA_PAGE_BYTES < 4096");
        let _ = writeln!(
            self.out,
            "#error \"MAK_ARENA_PAGE_BYTES must be at least 4096\""
        );
        let _ = writeln!(self.out, "#endif");
        let _ = writeln!(self.out, "#if MAK_ARENA_MAX_BYTES < MAK_ARENA_PAGE_BYTES");
        let _ = writeln!(
            self.out,
            "#error \"MAK_ARENA_MAX_BYTES must be >= MAK_ARENA_PAGE_BYTES\""
        );
        let _ = writeln!(self.out, "#endif");
        let _ = writeln!(self.out, "#if MAK_ARENA_QUARANTINE < 1");
        let _ = writeln!(
            self.out,
            "#error \"MAK_ARENA_QUARANTINE must be >= 1\""
        );
        let _ = writeln!(self.out, "#endif");

        let _ = writeln!(
            self.out,
            "#define MAK_IR_MEM_PAGE_MAGIC UINT64_C(0x4d414b5041474531)"
        );
        let _ = writeln!(
            self.out,
            "#define MAK_IR_MEM_HEAD_MAGIC UINT64_C(0x4d414b4845414431)"
        );
        let _ = writeln!(
            self.out,
            "#define MAK_IR_MEM_LINK_MAGIC UINT64_C(0x4d414b4c494e4b31)"
        );
        let _ = writeln!(
            self.out,
            "#define MAK_IR_MEM_TAIL_MAGIC UINT64_C(0x4d414b5441494c31)"
        );
        let _ = writeln!(self.out, "#define MAK_IR_MEM_LIVE 1u");
        let _ = writeln!(self.out, "#define MAK_IR_MEM_FREE 2u");
        let _ = writeln!(self.out, "#define MAK_IR_MEM_QUARANTINED 3u");

        let _ = writeln!(self.out, "struct mak_ir_mem_page;");
        let _ = writeln!(self.out, "struct mak_ir_mem_hdr;");
        let _ = writeln!(
            self.out,
            "struct mak_ir_mem_link {{ uint64_t magic; struct mak_ir_mem_hdr *hdr; }};"
        );
        let _ = writeln!(
            self.out,
            "struct mak_ir_mem_hdr {{"
        );
        let _ = writeln!(self.out, "    uint64_t magic;");
        let _ = writeln!(self.out, "    uint64_t cookie;");
        let _ = writeln!(self.out, "    struct mak_ir_mem_page *page;");
        let _ = writeln!(self.out, "    size_t total;");
        let _ = writeln!(self.out, "    size_t payload_offset;");
        let _ = writeln!(self.out, "    size_t size;");
        let _ = writeln!(self.out, "    size_t capacity;");
        let _ = writeln!(self.out, "    size_t alignment;");
        let _ = writeln!(self.out, "    uint32_t state;");
        let _ = writeln!(self.out, "    uint32_t class_id;");
        let _ = writeln!(self.out, "    struct mak_ir_mem_hdr *prev;");
        let _ = writeln!(self.out, "    struct mak_ir_mem_hdr *next;");
        let _ = writeln!(self.out, "}};");

        let _ = writeln!(
            self.out,
            "struct mak_ir_mem_page {{"
        );
        let _ = writeln!(self.out, "    uint64_t magic;");
        let _ = writeln!(self.out, "    uint64_t cookie;");
        let _ = writeln!(self.out, "    size_t capacity;");
        let _ = writeln!(self.out, "    size_t used;");
        let _ = writeln!(self.out, "    size_t blocks;");
        let _ = writeln!(self.out, "    size_t live;");
        let _ = writeln!(self.out, "    size_t free_blocks;");
        let _ = writeln!(self.out, "    int dedicated;");
        let _ = writeln!(self.out, "    struct mak_ir_mem_page *prev;");
        let _ = writeln!(self.out, "    struct mak_ir_mem_page *next;");
        let _ = writeln!(self.out, "    max_align_t alignment;");
        let _ = writeln!(self.out, "    unsigned char data[];");
        let _ = writeln!(self.out, "}};");

        let _ = writeln!(
            self.out,
            "static struct mak_ir_mem_page *mak_ir_mem_pages = NULL;"
        );
        let _ = writeln!(
            self.out,
            "static struct mak_ir_mem_page *mak_ir_mem_current = NULL;"
        );
        let _ = writeln!(
            self.out,
            "static struct mak_ir_mem_hdr *mak_ir_mem_free_bins[64] = {{0}};"
        );
        let _ = writeln!(
            self.out,
            "static struct mak_ir_mem_hdr *mak_ir_mem_quarantine[MAK_ARENA_QUARANTINE] = {{0}};"
        );
        let _ = writeln!(self.out, "static size_t mak_ir_mem_q_head = 0;");
        let _ = writeln!(self.out, "static size_t mak_ir_mem_q_count = 0;");
        let _ = writeln!(self.out, "static size_t mak_ir_mem_committed = 0;");
        let _ = writeln!(self.out, "static size_t mak_ir_mem_live_bytes = 0;");
        let _ = writeln!(self.out, "static size_t mak_ir_mem_peak_bytes = 0;");
        let _ = writeln!(self.out, "static uint64_t mak_ir_mem_faults = 0;");
        let _ = writeln!(
            self.out,
            "static atomic_flag mak_ir_mem_lock = ATOMIC_FLAG_INIT;"
        );
        let _ = writeln!(self.out, "static int mak_ir_mem_atexit_registered = 0;");

        let _ = writeln!(
            self.out,
            "static inline void mak_ir_mem_lock_enter(void) {{"
        );
        let _ = writeln!(
            self.out,
            "    while (atomic_flag_test_and_set_explicit(&mak_ir_mem_lock, memory_order_acquire)) {{ }}"
        );
        let _ = writeln!(self.out, "}}");

        let _ = writeln!(
            self.out,
            "static inline void mak_ir_mem_lock_leave(void) {{"
        );
        let _ = writeln!(
            self.out,
            "    atomic_flag_clear_explicit(&mak_ir_mem_lock, memory_order_release);"
        );
        let _ = writeln!(self.out, "}}");

        let _ = writeln!(
            self.out,
            "static inline uint64_t mak_ir_mem_mix(uint64_t x) {{"
        );
        let _ = writeln!(self.out, "    x ^= x >> 30; x *= UINT64_C(0xbf58476d1ce4e5b9);");
        let _ = writeln!(self.out, "    x ^= x >> 27; x *= UINT64_C(0x94d049bb133111eb);");
        let _ = writeln!(self.out, "    x ^= x >> 31; return x;");
        let _ = writeln!(self.out, "}}");

        let _ = writeln!(
            self.out,
            "static inline uint64_t mak_ir_mem_cookie_for(const void *p) {{"
        );
        let _ = writeln!(
            self.out,
            "    return mak_ir_mem_mix(UINT64_C(0x6d616b2d73656375) ^ (uint64_t)(uintptr_t)p ^ (UINT64_C(0x9e3779b97f4a7c15) * (uint64_t)sizeof(void *)));"
        );
        let _ = writeln!(self.out, "}}");

        let _ = writeln!(
            self.out,
            "static inline size_t mak_ir_mem_class(size_t n) {{"
        );
        let _ = writeln!(self.out, "    size_t k = 0; size_t x = n ? n : 1;");
        let _ = writeln!(
            self.out,
            "    while (x > 1 && k < 63) {{ x = (x + 1) >> 1; ++k; }}"
        );
        let _ = writeln!(self.out, "    return k;");
        let _ = writeln!(self.out, "}}");

        let _ = writeln!(
            self.out,
            "static inline int mak_ir_mem_add_overflow(size_t a, size_t b, size_t *out) {{"
        );
        let _ = writeln!(self.out, "    if (b > SIZE_MAX - a) return 1;");
        let _ = writeln!(self.out, "    *out = a + b; return 0;");
        let _ = writeln!(self.out, "}}");

        let _ = writeln!(
            self.out,
            "static inline uintptr_t mak_ir_mem_align_up_uintptr(uintptr_t x, size_t align, int *ok) {{"
        );
        let _ = writeln!(
            self.out,
            "    uintptr_t mask = (uintptr_t)(align - 1);"
        );
        let _ = writeln!(
            self.out,
            "    if (x > UINTPTR_MAX - mask) {{ *ok = 0; return 0; }}"
        );
        let _ = writeln!(self.out, "    *ok = 1; return (x + mask) & ~mask;");
        let _ = writeln!(self.out, "}}");

        let _ = writeln!(
            self.out,
            "static inline void mak_ir_mem_wipe(void *ptr, size_t n, unsigned char byte) {{"
        );
        let _ = writeln!(
            self.out,
            "    volatile unsigned char *p = (volatile unsigned char *)ptr;"
        );
        let _ = writeln!(
            self.out,
            "    while (n--) *p++ = byte;"
        );
        let _ = writeln!(self.out, "}}");

        let _ = writeln!(
            self.out,
            "static inline void mak_ir_mem_fault(const char *reason) {{"
        );
        let _ = writeln!(self.out, "    ++mak_ir_mem_faults;");
        let _ = writeln!(self.out, "#if MAK_ARENA_ABORT_ON_CORRUPTION");
        let _ = writeln!(
            self.out,
            "    fprintf(stderr, \"[MAK] arena integrity failure: %s\\n\", reason);"
        );
        let _ = writeln!(self.out, "    abort();");
        let _ = writeln!(self.out, "#else");
        let _ = writeln!(self.out, "    (void)reason;");
        let _ = writeln!(self.out, "#endif");
        let _ = writeln!(self.out, "}}");

        let _ = writeln!(
            self.out,
            "static inline uint64_t mak_ir_mem_tail(const struct mak_ir_mem_hdr *h) {{"
        );
        let _ = writeln!(
            self.out,
            "    return mak_ir_mem_mix(h->cookie ^ (uint64_t)h->size ^ UINT64_C(0xa55aa55af00df00d));"
        );
        let _ = writeln!(self.out, "}}");

        let _ = writeln!(
            self.out,
            "static inline unsigned char *mak_ir_mem_payload(const struct mak_ir_mem_hdr *h) {{"
        );
        let _ = writeln!(
            self.out,
            "    return h && h->page ? h->page->data + h->payload_offset : NULL;"
        );
        let _ = writeln!(self.out, "}}");

        let _ = writeln!(
            self.out,
            "static inline uint64_t *mak_ir_mem_tail_ptr(const struct mak_ir_mem_hdr *h) {{"
        );
        let _ = writeln!(
            self.out,
            "    return (uint64_t *)(void *)(mak_ir_mem_payload(h) + h->size);"
        );
        let _ = writeln!(self.out, "}}");

        let _ = writeln!(
            self.out,
            "static inline int mak_ir_mem_page_contains(const struct mak_ir_mem_page *p, const void *ptr) {{"
        );
        let _ = writeln!(
            self.out,
            "    if (!p || !ptr) return 0;"
        );
        let _ = writeln!(
            self.out,
            "    if (p->used > p->capacity) return 0;"
        );
        let _ = writeln!(
            self.out,
            "    uintptr_t lo = (uintptr_t)(const void *)p->data;"
        );
        let _ = writeln!(
            self.out,
            "    uintptr_t hi = lo + p->used;"
        );
        let _ = writeln!(
            self.out,
            "    uintptr_t x = (uintptr_t)ptr;"
        );
        let _ = writeln!(
            self.out,
            "    return hi >= lo && x >= lo && x < hi;"
        );
        let _ = writeln!(self.out, "}}");

        let _ = writeln!(
            self.out,
            "static inline void mak_ir_mem_unlink_free(struct mak_ir_mem_hdr *h) {{"
        );
        let _ = writeln!(self.out, "    if (!h) return;");
        let _ = writeln!(
            self.out,
            "    size_t c = h->class_id < 64u ? (size_t)h->class_id : 63u;"
        );
        let _ = writeln!(
            self.out,
            "    if (h->prev) h->prev->next = h->next; else mak_ir_mem_free_bins[c] = h->next;"
        );
        let _ = writeln!(
            self.out,
            "    if (h->next) h->next->prev = h->prev;"
        );
        let _ = writeln!(self.out, "    h->prev = NULL; h->next = NULL;");
        let _ = writeln!(self.out, "}}");

        let _ = writeln!(
            self.out,
            "static inline void mak_ir_mem_link_free(struct mak_ir_mem_hdr *h) {{"
        );
        let _ = writeln!(self.out, "    size_t c = h->class_id < 64u ? (size_t)h->class_id : 63u;");
        let _ = writeln!(self.out, "    h->prev = NULL; h->next = mak_ir_mem_free_bins[c];");
        let _ = writeln!(
            self.out,
            "    if (h->next) h->next->prev = h;"
        );
        let _ = writeln!(self.out, "    mak_ir_mem_free_bins[c] = h;");
        let _ = writeln!(self.out, "}}");

        let _ = writeln!(
            self.out,
            "static inline int mak_ir_mem_header_valid(const struct mak_ir_mem_hdr *h) {{"
        );
        let _ = writeln!(
            self.out,
            "    if (!h || h->magic != MAK_IR_MEM_HEAD_MAGIC || !h->page) return 0;"
        );
        let _ = writeln!(
            self.out,
            "    if (h->cookie != mak_ir_mem_cookie_for(h)) return 0;"
        );
        let _ = writeln!(
            self.out,
            "    if (h->class_id >= 64u || h->capacity == 0 || h->payload_offset < sizeof(struct mak_ir_mem_hdr) + sizeof(struct mak_ir_mem_link)) return 0;"
        );
        let _ = writeln!(
            self.out,
            "    if (h->payload_offset > h->page->capacity || h->size > h->page->capacity - h->payload_offset) return 0;"
        );
        let _ = writeln!(
            self.out,
            "    if (h->alignment == 0 || (h->alignment & (h->alignment - 1)) != 0) return 0;"
        );
        let _ = writeln!(
            self.out,
            "    if (h->size > h->capacity) return 0;"
        );
        let _ = writeln!(self.out, "    return 1;");
        let _ = writeln!(self.out, "}}");

        let _ = writeln!(
            self.out,
            "static inline int mak_ir_mem_canary_valid(const struct mak_ir_mem_hdr *h) {{"
        );
        let _ = writeln!(
            self.out,
            "    if (!mak_ir_mem_header_valid(h)) return 0;"
        );
        let _ = writeln!(
            self.out,
            "    return *mak_ir_mem_tail_ptr(h) == mak_ir_mem_tail(h);"
        );
        let _ = writeln!(self.out, "}}");

        let _ = writeln!(
            self.out,
            "static void mak_ir_mem_reclaim_empty_locked(void) {{"
        );
        let _ = writeln!(
            self.out,
            "    struct mak_ir_mem_page *p = mak_ir_mem_pages;"
        );
        let _ = writeln!(
            self.out,
            "    while (p) {{"
        );
        let _ = writeln!(
            self.out,
            "        struct mak_ir_mem_page *next = p->next;"
        );
        let _ = writeln!(
            self.out,
            "        int clean = 1;"
        );
        let _ = writeln!(
            self.out,
            "        if (p->magic != MAK_IR_MEM_PAGE_MAGIC || p->used > p->capacity) {{ p = next; continue; }}"
        );
        let _ = writeln!(
            self.out,
            "        if (p->live == 0 && p->free_blocks == p->blocks) {{"
        );
        let _ = writeln!(
            self.out,
            "            size_t off = 0;"
        );
        let _ = writeln!(
            self.out,
            "            while (off < p->used) {{"
        );
        let _ = writeln!(
            self.out,
            "                if (p->used - off < sizeof(struct mak_ir_mem_hdr)) {{ clean = 0; break; }}"
        );
        let _ = writeln!(
            self.out,
            "                struct mak_ir_mem_hdr *h = (struct mak_ir_mem_hdr *)(void *)(p->data + off);"
        );
        let _ = writeln!(
            self.out,
            "                if (!mak_ir_mem_header_valid(h) || h->state != MAK_IR_MEM_FREE) {{ clean = 0; break; }}"
        );
        let _ = writeln!(
            self.out,
            "                mak_ir_mem_unlink_free(h);"
        );
        let _ = writeln!(
            self.out,
            "                if (h->total == 0 || h->total > p->used - off) {{ clean = 0; break; }}"
        );
        let _ = writeln!(
            self.out,
            "                off += h->total;"
        );
        let _ = writeln!(
            self.out,
            "            }}"
        );
        let _ = writeln!(
            self.out,
            "            if (!clean || off != p->used) {{ p = next; continue; }}"
        );
        let _ = writeln!(
            self.out,
            "            if (p->dedicated || p != mak_ir_mem_current) {{"
        );
        let _ = writeln!(
            self.out,
            "                if (p->prev) p->prev->next = p->next; else mak_ir_mem_pages = p->next;"
        );
        let _ = writeln!(
            self.out,
            "                if (p->next) p->next->prev = p->prev;"
        );
        let _ = writeln!(
            self.out,
            "                if (mak_ir_mem_current == p) mak_ir_mem_current = p->prev ? p->prev : p->next;"
        );
        let _ = writeln!(
            self.out,
            "                if (mak_ir_mem_committed >= sizeof(*p) + p->capacity) mak_ir_mem_committed -= sizeof(*p) + p->capacity; else mak_ir_mem_committed = 0;"
        );
        let _ = writeln!(
            self.out,
            "                free(p);"
        );
        let _ = writeln!(
            self.out,
            "            }} else {{"
        );
        let _ = writeln!(
            self.out,
            "                p->used = 0; p->blocks = 0; p->live = 0; p->free_blocks = 0;"
        );
        let _ = writeln!(
            self.out,
            "            }}"
        );
        let _ = writeln!(
            self.out,
            "        }}"
        );
        let _ = writeln!(
            self.out,
            "        p = next;"
        );
        let _ = writeln!(
            self.out,
            "    }}"
        );
        let _ = writeln!(self.out, "}}");

        let _ = writeln!(
            self.out,
            "static inline struct mak_ir_mem_page *mak_ir_mem_page_new_locked(size_t cap, int dedicated) {{"
        );
        let _ = writeln!(
            self.out,
            "    size_t bytes;"
        );
        let _ = writeln!(
            self.out,
            "    if (cap < MAK_ARENA_PAGE_BYTES) cap = MAK_ARENA_PAGE_BYTES;"
        );
        let _ = writeln!(
            self.out,
            "    if (mak_ir_mem_add_overflow(sizeof(struct mak_ir_mem_page), cap, &bytes)) return NULL;"
        );
        let _ = writeln!(
            self.out,
            "    if (bytes > MAK_ARENA_MAX_BYTES - (mak_ir_mem_committed < MAK_ARENA_MAX_BYTES ? mak_ir_mem_committed : MAK_ARENA_MAX_BYTES)) return NULL;"
        );
        let _ = writeln!(
            self.out,
            "    struct mak_ir_mem_page *p = (struct mak_ir_mem_page *)malloc(bytes);"
        );
        let _ = writeln!(
            self.out,
            "    if (!p) return NULL;"
        );
        let _ = writeln!(
            self.out,
            "    memset(p, 0, sizeof(*p));"
        );
        let _ = writeln!(
            self.out,
            "    p->magic = MAK_IR_MEM_PAGE_MAGIC;"
        );
        let _ = writeln!(
            self.out,
            "    p->cookie = mak_ir_mem_cookie_for(p);"
        );
        let _ = writeln!(
            self.out,
            "    p->capacity = cap; p->dedicated = dedicated;"
        );
        let _ = writeln!(
            self.out,
            "    p->prev = NULL; p->next = mak_ir_mem_pages;"
        );
        let _ = writeln!(
            self.out,
            "    if (p->next) p->next->prev = p;"
        );
        let _ = writeln!(
            self.out,
            "    mak_ir_mem_pages = p; mak_ir_mem_current = p;"
        );
        let _ = writeln!(
            self.out,
            "    mak_ir_mem_committed += bytes;"
        );
        let _ = writeln!(
            self.out,
            "    if (mak_ir_mem_committed > mak_ir_mem_peak_bytes) mak_ir_mem_peak_bytes = mak_ir_mem_committed;"
        );
        let _ = writeln!(
            self.out,
            "    return p;"
        );
        let _ = writeln!(self.out, "}}");

        let _ = writeln!(
            self.out,
            "static inline struct mak_ir_mem_hdr *mak_ir_mem_find_free_locked(size_t size, size_t align) {{"
        );
        let _ = writeln!(
            self.out,
            "    size_t c0 = mak_ir_mem_class(size);"
        );
        let _ = writeln!(
            self.out,
            "    for (size_t c = c0; c < 64; ++c) {{"
        );
        let _ = writeln!(
            self.out,
            "        for (struct mak_ir_mem_hdr *h = mak_ir_mem_free_bins[c]; h; h = h->next) {{"
        );
        let _ = writeln!(
            self.out,
            "            if (h->state != MAK_IR_MEM_FREE || !mak_ir_mem_header_valid(h)) continue;"
        );
        let _ = writeln!(
            self.out,
            "            if (!mak_ir_mem_canary_valid(h)) {{ mak_ir_mem_fault(\"free-block canary corrupted\"); continue; }}"
        );
        let _ = writeln!(
            self.out,
            "            if (h->capacity < size || h->alignment < align) continue;"
        );
        let _ = writeln!(
            self.out,
            "            return h;"
        );
        let _ = writeln!(
            self.out,
            "        }}"
        );
        let _ = writeln!(
            self.out,
            "    }}"
        );
        let _ = writeln!(
            self.out,
            "    return NULL;"
        );
        let _ = writeln!(self.out, "}}");

        let _ = writeln!(
            self.out,
            "static inline void mak_ir_mem_make_live_locked(struct mak_ir_mem_hdr *h, size_t size, size_t align) {{"
        );
        let _ = writeln!(
            self.out,
            "    struct mak_ir_mem_page *p = h->page;"
        );
        let _ = writeln!(
            self.out,
            "    if (h->state == MAK_IR_MEM_FREE && p->free_blocks) --p->free_blocks;"
        );
        let _ = writeln!(
            self.out,
            "    h->state = MAK_IR_MEM_LIVE;"
        );
        let _ = writeln!(
            self.out,
            "    h->size = size;"
        );
        let _ = writeln!(
            self.out,
            "    h->alignment = h->alignment > align ? h->alignment : align;"
        );
        let _ = writeln!(
            self.out,
            "    h->class_id = (uint32_t)mak_ir_mem_class(h->capacity);"
        );
        let _ = writeln!(
            self.out,
            "    *mak_ir_mem_tail_ptr(h) = mak_ir_mem_tail(h);"
        );
        let _ = writeln!(
            self.out,
            "    if (MAK_ARENA_HARDEN) memset(mak_ir_mem_payload(h), 0, h->capacity);"
        );
        let _ = writeln!(
            self.out,
            "    ++p->live;"
        );
        let _ = writeln!(
            self.out,
            "    mak_ir_mem_live_bytes += size;"
        );
        let _ = writeln!(self.out, "}}");

        let _ = writeln!(
            self.out,
            "static inline void mak_ir_mem_quarantine_push_locked(struct mak_ir_mem_hdr *h) {{"
        );
        let _ = writeln!(
            self.out,
            "    if (mak_ir_mem_q_count == MAK_ARENA_QUARANTINE) {{"
        );
        let _ = writeln!(
            self.out,
            "        struct mak_ir_mem_hdr *old = mak_ir_mem_quarantine[mak_ir_mem_q_head];"
        );
        let _ = writeln!(
            self.out,
            "        mak_ir_mem_q_head = (mak_ir_mem_q_head + 1) % MAK_ARENA_QUARANTINE;"
        );
        let _ = writeln!(
            self.out,
            "        --mak_ir_mem_q_count;"
        );
        let _ = writeln!(
            self.out,
            "        if (old && old->state == MAK_IR_MEM_QUARANTINED) {{ old->state = MAK_IR_MEM_FREE; ++old->page->free_blocks; mak_ir_mem_link_free(old); }}"
        );
        let _ = writeln!(
            self.out,
            "    }}"
        );
        let _ = writeln!(
            self.out,
            "    size_t slot = (mak_ir_mem_q_head + mak_ir_mem_q_count) % MAK_ARENA_QUARANTINE;"
        );
        let _ = writeln!(
            self.out,
            "    mak_ir_mem_quarantine[slot] = h;"
        );
        let _ = writeln!(
            self.out,
            "    ++mak_ir_mem_q_count;"
        );
        let _ = writeln!(self.out, "}}");

        let _ = writeln!(
            self.out,
            "static inline void mak_ir_mem_flush_quarantine_locked(size_t count) {{"
        );
        let _ = writeln!(
            self.out,
            "    while (count && mak_ir_mem_q_count) {{"
        );
        let _ = writeln!(
            self.out,
            "        struct mak_ir_mem_hdr *h = mak_ir_mem_quarantine[mak_ir_mem_q_head];"
        );
        let _ = writeln!(
            self.out,
            "        mak_ir_mem_quarantine[mak_ir_mem_q_head] = NULL;"
        );
        let _ = writeln!(
            self.out,
            "        mak_ir_mem_q_head = (mak_ir_mem_q_head + 1) % MAK_ARENA_QUARANTINE;"
        );
        let _ = writeln!(
            self.out,
            "        --mak_ir_mem_q_count; --count;"
        );
        let _ = writeln!(
            self.out,
            "        if (h && h->state == MAK_IR_MEM_QUARANTINED) {{ h->state = MAK_IR_MEM_FREE; ++h->page->free_blocks; mak_ir_mem_link_free(h); }}"
        );
        let _ = writeln!(
            self.out,
            "    }}"
        );
        let _ = writeln!(self.out, "}}");

        let _ = writeln!(
            self.out,
            "static void mak_ir_arena_shutdown(void);"
        );

        let _ = writeln!(
            self.out,
            "static inline void mak_ir_arena_register_exit_locked(void) {{"
        );
        let _ = writeln!(
            self.out,
            "    if (!mak_ir_mem_atexit_registered) {{"
        );
        let _ = writeln!(
            self.out,
            "        if (atexit(mak_ir_arena_shutdown) == 0) mak_ir_mem_atexit_registered = 1;"
        );
        let _ = writeln!(
            self.out,
            "    }}"
        );
        let _ = writeln!(self.out, "}}");

        let _ = writeln!(
            self.out,
            "static inline void *mak_ir_arena_alloc(size_t size, size_t align) {{"
        );
        let _ = writeln!(
            self.out,
            "    if (size == 0) size = 1;"
        );
        let _ = writeln!(
            self.out,
            "    if (align == 0) align = _Alignof(max_align_t);"
        );
        let _ = writeln!(
            self.out,
            "    if ((align & (align - 1)) != 0 || align > MAK_ARENA_MAX_ALIGN) return NULL;"
        );
        let _ = writeln!(
            self.out,
            "    mak_ir_mem_lock_enter();"
        );
        let _ = writeln!(
            self.out,
            "    mak_ir_arena_register_exit_locked();"
        );
        let _ = writeln!(
            self.out,
            "    struct mak_ir_mem_hdr *reuse = mak_ir_mem_find_free_locked(size, align);"
        );
        let _ = writeln!(
            self.out,
            "    if (reuse) {{"
        );
        let _ = writeln!(
            self.out,
            "        mak_ir_mem_unlink_free(reuse);"
        );
        let _ = writeln!(
            self.out,
            "        mak_ir_mem_make_live_locked(reuse, size, align);"
        );
        let _ = writeln!(
            self.out,
            "        void *ret = (void *)mak_ir_mem_payload(reuse);"
        );
        let _ = writeln!(
            self.out,
            "        mak_ir_mem_lock_leave();"
        );
        let _ = writeln!(
            self.out,
            "        return ret;"
        );
        let _ = writeln!(
            self.out,
            "    }}"
        );

        let _ = writeln!(
            self.out,
            "    size_t overhead = sizeof(struct mak_ir_mem_hdr) + sizeof(struct mak_ir_mem_link);"
        );
        let _ = writeln!(
            self.out,
            "    size_t needed = overhead;"
        );
        let _ = writeln!(
            self.out,
            "    if (align - 1 > SIZE_MAX - needed) {{ mak_ir_mem_lock_leave(); return NULL; }}"
        );
        let _ = writeln!(
            self.out,
            "    needed += align - 1;"
        );
        let _ = writeln!(
            self.out,
            "    if (size > SIZE_MAX - needed) {{ mak_ir_mem_lock_leave(); return NULL; }}"
        );
        let _ = writeln!(
            self.out,
            "    needed += size;"
        );
        let _ = writeln!(
            self.out,
            "    if (sizeof(uint64_t) > SIZE_MAX - needed) {{ mak_ir_mem_lock_leave(); return NULL; }}"
        );
        let _ = writeln!(
            self.out,
            "    needed += sizeof(uint64_t);"
        );
        let _ = writeln!(
            self.out,
            "    size_t page_cap = MAK_ARENA_PAGE_BYTES;"
        );
        let _ = writeln!(
            self.out,
            "    int dedicated = (needed > MAK_ARENA_PAGE_BYTES / 2u);"
        );
        let _ = writeln!(
            self.out,
            "    if (dedicated) {{"
        );
        let _ = writeln!(
            self.out,
            "        page_cap = needed;"
        );
        let _ = writeln!(
            self.out,
            "        if (page_cap < MAK_ARENA_PAGE_BYTES) page_cap = MAK_ARENA_PAGE_BYTES;"
        );
        let _ = writeln!(
            self.out,
            "    }}"
        );

        let _ = writeln!(
            self.out,
            "    struct mak_ir_mem_page *p = mak_ir_mem_current;"
        );
        let _ = writeln!(
            self.out,
            "    if (!dedicated && p && p->magic == MAK_IR_MEM_PAGE_MAGIC) {{"
        );
        let _ = writeln!(
            self.out,
            "        size_t candidate = p->used;"
        );
        let _ = writeln!(
            self.out,
            "        uintptr_t raw = (uintptr_t)(void *)p->data + candidate + overhead;"
        );
        let _ = writeln!(
            self.out,
            "        int ok = 1;"
        );
        let _ = writeln!(
            self.out,
            "        uintptr_t payload = mak_ir_mem_align_up_uintptr(raw, align, &ok);"
        );
        let _ = writeln!(
            self.out,
            "        if (!ok || payload < (uintptr_t)(void *)p->data) {{ p = NULL; }}"
        );
        let _ = writeln!(
            self.out,
            "        else {{"
        );
        let _ = writeln!(
            self.out,
            "            uintptr_t page_base = (uintptr_t)(void *)p->data;"
        );
        let _ = writeln!(
            self.out,
            "            size_t payload_off = (size_t)(payload - page_base);"
        );
        let _ = writeln!(
            self.out,
            "            size_t end = payload_off;"
        );
        let _ = writeln!(
            self.out,
            "            if (size > SIZE_MAX - end) p = NULL;"
        );
        let _ = writeln!(
            self.out,
            "            else {{ end += size; if (sizeof(uint64_t) > SIZE_MAX - end) p = NULL; else end += sizeof(uint64_t); }}"
        );
        let _ = writeln!(
            self.out,
            "            size_t next_used = end;"
        );
        let _ = writeln!(
            self.out,
            "            size_t maxa = _Alignof(max_align_t);"
        );
        let _ = writeln!(
            self.out,
            "            if (maxa > 1 && next_used <= SIZE_MAX - (maxa - 1)) next_used = (next_used + (maxa - 1)) & ~(maxa - 1); else if (maxa > 1) p = NULL;"
        );
        let _ = writeln!(
            self.out,
            "            if (p && next_used <= p->capacity) {{"
        );
        let _ = writeln!(
            self.out,
            "                size_t off = p->used;"
        );
        let _ = writeln!(
            self.out,
            "                struct mak_ir_mem_hdr *h = (struct mak_ir_mem_hdr *)(void *)(p->data + off);"
        );
        let _ = writeln!(
            self.out,
            "                memset(h, 0, sizeof(*h));"
        );
        let _ = writeln!(
            self.out,
            "                h->magic = MAK_IR_MEM_HEAD_MAGIC; h->cookie = mak_ir_mem_cookie_for(h); h->page = p;"
        );
        let _ = writeln!(
            self.out,
            "                h->total = next_used - off; h->payload_offset = payload_off; h->size = size; h->capacity = size; h->alignment = align; h->state = MAK_IR_MEM_LIVE; h->class_id = (uint32_t)mak_ir_mem_class(h->capacity);"
        );
        let _ = writeln!(
            self.out,
            "                struct mak_ir_mem_link *ln = (struct mak_ir_mem_link *)(void *)(p->data + payload_off - sizeof(struct mak_ir_mem_link));"
        );
        let _ = writeln!(
            self.out,
            "                ln->magic = MAK_IR_MEM_LINK_MAGIC; ln->hdr = h;"
        );
        let _ = writeln!(
            self.out,
            "                *mak_ir_mem_tail_ptr(h) = mak_ir_mem_tail(h);"
        );
        let _ = writeln!(
            self.out,
            "                if (MAK_ARENA_HARDEN) memset(mak_ir_mem_payload(h), 0, h->capacity);"
        );
        let _ = writeln!(
            self.out,
            "                p->used = next_used; ++p->blocks; ++p->live;"
        );
        let _ = writeln!(
            self.out,
            "                mak_ir_mem_live_bytes += size;"
        );
        let _ = writeln!(
            self.out,
            "                void *ret = (void *)mak_ir_mem_payload(h);"
        );
        let _ = writeln!(
            self.out,
            "                mak_ir_mem_lock_leave();"
        );
        let _ = writeln!(
            self.out,
            "                return ret;"
        );
        let _ = writeln!(
            self.out,
            "            }}"
        );
        let _ = writeln!(
            self.out,
            "        }}"
        );
        let _ = writeln!(
            self.out,
            "    }}"
        );

        let _ = writeln!(
            self.out,
            "    if (mak_ir_mem_committed > MAK_ARENA_MAX_BYTES || page_cap > MAK_ARENA_MAX_BYTES - mak_ir_mem_committed) {{"
        );
        let _ = writeln!(
            self.out,
            "        mak_ir_mem_flush_quarantine_locked(MAK_ARENA_QUARANTINE);"
        );
        let _ = writeln!(
            self.out,
            "        mak_ir_mem_reclaim_empty_locked();"
        );
        let _ = writeln!(
            self.out,
            "    }}"
        );
        let _ = writeln!(
            self.out,
            "    if (mak_ir_mem_committed > MAK_ARENA_MAX_BYTES || page_cap > MAK_ARENA_MAX_BYTES - mak_ir_mem_committed) {{"
        );
        let _ = writeln!(
            self.out,
            "        mak_ir_mem_lock_leave(); return NULL;"
        );
        let _ = writeln!(
            self.out,
            "    }}"
        );

        let _ = writeln!(
            self.out,
            "    p = mak_ir_mem_page_new_locked(page_cap, dedicated);"
        );
        let _ = writeln!(
            self.out,
            "    if (!p) {{ mak_ir_mem_lock_leave(); return NULL; }}"
        );
        let _ = writeln!(
            self.out,
            "    uintptr_t raw = (uintptr_t)(void *)p->data + overhead;"
        );
        let _ = writeln!(
            self.out,
            "    int ok = 1;"
        );
        let _ = writeln!(
            self.out,
            "    uintptr_t payload = mak_ir_mem_align_up_uintptr(raw, align, &ok);"
        );
        let _ = writeln!(
            self.out,
            "    if (!ok || payload < (uintptr_t)(void *)p->data) {{ mak_ir_mem_lock_leave(); return NULL; }}"
        );
        let _ = writeln!(
            self.out,
            "    size_t payload_off = (size_t)(payload - (uintptr_t)(void *)p->data);"
        );
        let _ = writeln!(
            self.out,
            "    size_t end = payload_off;"
        );
        let _ = writeln!(
            self.out,
            "    if (size > SIZE_MAX - end) {{ mak_ir_mem_lock_leave(); return NULL; }}"
        );
        let _ = writeln!(
            self.out,
            "    end += size; if (sizeof(uint64_t) > SIZE_MAX - end) {{ mak_ir_mem_lock_leave(); return NULL; }} end += sizeof(uint64_t);"
        );
        let _ = writeln!(
            self.out,
            "    size_t maxa = _Alignof(max_align_t);"
        );
        let _ = writeln!(
            self.out,
            "    size_t next_used = (maxa > 1) ? ((end + (maxa - 1)) & ~(maxa - 1)) : end;"
        );
        let _ = writeln!(
            self.out,
            "    if (next_used > p->capacity) {{ mak_ir_mem_lock_leave(); return NULL; }}"
        );
        let _ = writeln!(
            self.out,
            "    struct mak_ir_mem_hdr *h = (struct mak_ir_mem_hdr *)(void *)p->data;"
        );
        let _ = writeln!(
            self.out,
            "    memset(h, 0, sizeof(*h));"
        );
        let _ = writeln!(
            self.out,
            "    h->magic = MAK_IR_MEM_HEAD_MAGIC; h->cookie = mak_ir_mem_cookie_for(h); h->page = p;"
        );
        let _ = writeln!(
            self.out,
            "    h->total = next_used; h->payload_offset = payload_off; h->size = size; h->capacity = size; h->alignment = align; h->state = MAK_IR_MEM_LIVE; h->class_id = (uint32_t)mak_ir_mem_class(h->capacity);"
        );
        let _ = writeln!(
            self.out,
            "    struct mak_ir_mem_link *ln = (struct mak_ir_mem_link *)(void *)(p->data + payload_off - sizeof(struct mak_ir_mem_link));"
        );
        let _ = writeln!(
            self.out,
            "    ln->magic = MAK_IR_MEM_LINK_MAGIC; ln->hdr = h;"
        );
        let _ = writeln!(
            self.out,
            "    *mak_ir_mem_tail_ptr(h) = mak_ir_mem_tail(h);"
        );
        let _ = writeln!(
            self.out,
            "    if (MAK_ARENA_HARDEN) memset(mak_ir_mem_payload(h), 0, h->capacity);"
        );
        let _ = writeln!(
            self.out,
            "    p->used = next_used; ++p->blocks; ++p->live;"
        );
        let _ = writeln!(
            self.out,
            "    mak_ir_mem_live_bytes += size;"
        );
        let _ = writeln!(
            self.out,
            "    void *ret = (void *)mak_ir_mem_payload(h);"
        );
        let _ = writeln!(
            self.out,
            "    mak_ir_mem_lock_leave(); return ret;"
        );
        let _ = writeln!(self.out, "}}");

        let _ = writeln!(
            self.out,
            "static inline void mak_ir_arena_free(void *ptr) {{"
        );
        let _ = writeln!(
            self.out,
            "    if (!ptr) return;"
        );
        let _ = writeln!(
            self.out,
            "    mak_ir_mem_lock_enter();"
        );
        let _ = writeln!(
            self.out,
            "    struct mak_ir_mem_page *p = NULL;"
        );
        let _ = writeln!(
            self.out,
            "    for (struct mak_ir_mem_page *it = mak_ir_mem_pages; it; it = it->next) {{"
        );
        let _ = writeln!(
            self.out,
            "        if (it->magic == MAK_IR_MEM_PAGE_MAGIC && mak_ir_mem_page_contains(it, ptr)) {{ p = it; break; }}"
        );
        let _ = writeln!(
            self.out,
            "    }}"
        );
        let _ = writeln!(
            self.out,
            "    if (!p) {{ mak_ir_mem_lock_leave(); mak_ir_mem_fault(\"free of non-arena pointer\"); return; }}"
        );
        let _ = writeln!(
            self.out,
            "    uintptr_t x = (uintptr_t)ptr;"
        );
        let _ = writeln!(
            self.out,
            "    uintptr_t base = (uintptr_t)(void *)p->data;"
        );
        let _ = writeln!(
            self.out,
            "    if (x < base + sizeof(struct mak_ir_mem_link)) {{ mak_ir_mem_lock_leave(); mak_ir_mem_fault(\"invalid arena pointer\"); return; }}"
        );
        let _ = writeln!(
            self.out,
            "    struct mak_ir_mem_link *ln = (struct mak_ir_mem_link *)(void *)((unsigned char *)ptr - sizeof(struct mak_ir_mem_link));"
        );
        let _ = writeln!(
            self.out,
            "    if (ln->magic != MAK_IR_MEM_LINK_MAGIC || !ln->hdr) {{ mak_ir_mem_lock_leave(); mak_ir_mem_fault(\"invalid allocation link\"); return; }}"
        );
        let _ = writeln!(
            self.out,
            "    struct mak_ir_mem_hdr *h = ln->hdr;"
        );
        let _ = writeln!(
            self.out,
            "    if (h->page != p || !mak_ir_mem_header_valid(h) || mak_ir_mem_payload(h) != (unsigned char *)ptr) {{ mak_ir_mem_lock_leave(); mak_ir_mem_fault(\"allocation header mismatch\"); return; }}"
        );
        let _ = writeln!(
            self.out,
            "    if (h->state != MAK_IR_MEM_LIVE) {{ mak_ir_mem_lock_leave(); mak_ir_mem_fault(\"double free or stale free\"); return; }}"
        );
        let _ = writeln!(
            self.out,
            "    if (!mak_ir_mem_canary_valid(h)) {{ mak_ir_mem_lock_leave(); mak_ir_mem_fault(\"allocation canary corrupted\"); return; }}"
        );
        let _ = writeln!(
            self.out,
            "    if (p->live == 0 || mak_ir_mem_live_bytes < h->size) {{ mak_ir_mem_lock_leave(); mak_ir_mem_fault(\"arena accounting corruption\"); return; }}"
        );
        let _ = writeln!(
            self.out,
            "    --p->live;"
        );
        let _ = writeln!(
            self.out,
            "    mak_ir_mem_live_bytes -= h->size;"
        );
        let _ = writeln!(
            self.out,
            "    h->state = MAK_IR_MEM_QUARANTINED;"
        );
        let _ = writeln!(
            self.out,
            "    if (MAK_ARENA_HARDEN) mak_ir_mem_wipe(mak_ir_mem_payload(h), h->capacity, 0xdd);"
        );
        let _ = writeln!(
            self.out,
            "    mak_ir_mem_quarantine_push_locked(h);"
        );
        let _ = writeln!(
            self.out,
            "    mak_ir_mem_reclaim_empty_locked();"
        );
        let _ = writeln!(
            self.out,
            "    mak_ir_mem_lock_leave();"
        );
        let _ = writeln!(self.out, "}}");

        let _ = writeln!(
            self.out,
            "static inline void mak_ir_arena_trim(void) {{"
        );
        let _ = writeln!(
            self.out,
            "    mak_ir_mem_lock_enter();"
        );
        let _ = writeln!(
            self.out,
            "    mak_ir_mem_flush_quarantine_locked(MAK_ARENA_QUARANTINE);"
        );
        let _ = writeln!(
            self.out,
            "    mak_ir_mem_reclaim_empty_locked();"
        );
        let _ = writeln!(
            self.out,
            "    mak_ir_mem_lock_leave();"
        );
        let _ = writeln!(self.out, "}}");

        let _ = writeln!(
            self.out,
            "static inline int mak_ir_arena_owns(const void *ptr) {{"
        );
        let _ = writeln!(
            self.out,
            "    if (!ptr) return 0;"
        );
        let _ = writeln!(
            self.out,
            "    mak_ir_mem_lock_enter();"
        );
        let _ = writeln!(
            self.out,
            "    int result = 0;"
        );
        let _ = writeln!(
            self.out,
            "    for (struct mak_ir_mem_page *p = mak_ir_mem_pages; p; p = p->next) {{"
        );
        let _ = writeln!(
            self.out,
            "        if (p->magic == MAK_IR_MEM_PAGE_MAGIC && mak_ir_mem_page_contains(p, ptr)) {{"
        );
        let _ = writeln!(
            self.out,
            "            uintptr_t x = (uintptr_t)ptr;"
        );
        let _ = writeln!(
            self.out,
            "            if (x >= (uintptr_t)(void *)p->data + sizeof(struct mak_ir_mem_link)) {{"
        );
        let _ = writeln!(
            self.out,
            "                struct mak_ir_mem_link *ln = (struct mak_ir_mem_link *)(void *)((unsigned char *)ptr - sizeof(struct mak_ir_mem_link));"
        );
        let _ = writeln!(
            self.out,
            "                result = ln->magic == MAK_IR_MEM_LINK_MAGIC && ln->hdr && ln->hdr->page == p && ln->hdr->state == MAK_IR_MEM_LIVE && mak_ir_mem_header_valid(ln->hdr) && mak_ir_mem_canary_valid(ln->hdr);"
        );
        let _ = writeln!(
            self.out,
            "            }}"
        );
        let _ = writeln!(
            self.out,
            "            break;"
        );
        let _ = writeln!(
            self.out,
            "        }}"
        );
        let _ = writeln!(
            self.out,
            "    }}"
        );
        let _ = writeln!(
            self.out,
            "    mak_ir_mem_lock_leave(); return result;"
        );
        let _ = writeln!(self.out, "}}");

        let _ = writeln!(
            self.out,
            "static inline size_t mak_ir_arena_live_bytes(void) {{"
        );
        let _ = writeln!(
            self.out,
            "    mak_ir_mem_lock_enter(); size_t n = mak_ir_mem_live_bytes; mak_ir_mem_lock_leave(); return n;"
        );
        let _ = writeln!(self.out, "}}");

        let _ = writeln!(
            self.out,
            "static inline size_t mak_ir_arena_committed_bytes(void) {{"
        );
        let _ = writeln!(
            self.out,
            "    mak_ir_mem_lock_enter(); size_t n = mak_ir_mem_committed; mak_ir_mem_lock_leave(); return n;"
        );
        let _ = writeln!(self.out, "}}");

        let _ = writeln!(
            self.out,
            "static void mak_ir_arena_shutdown(void) {{"
        );
        let _ = writeln!(
            self.out,
            "    mak_ir_mem_lock_enter();"
        );
        let _ = writeln!(
            self.out,
            "    for (size_t i = 0; i < MAK_ARENA_QUARANTINE; ++i) mak_ir_mem_quarantine[i] = NULL;"
        );
        let _ = writeln!(
            self.out,
            "    mak_ir_mem_q_head = 0; mak_ir_mem_q_count = 0;"
        );
        let _ = writeln!(
            self.out,
            "    for (struct mak_ir_mem_page *p = mak_ir_mem_pages; p;) {{"
        );
        let _ = writeln!(
            self.out,
            "        struct mak_ir_mem_page *next = p->next; free(p); p = next;"
        );
        let _ = writeln!(
            self.out,
            "    }}"
        );
        let _ = writeln!(
            self.out,
            "    mak_ir_mem_pages = NULL; mak_ir_mem_current = NULL; mak_ir_mem_committed = 0; mak_ir_mem_live_bytes = 0;"
        );
        let _ = writeln!(
            self.out,
            "    mak_ir_mem_peak_bytes = 0;"
        );
        let _ = writeln!(
            self.out,
            "    memset(mak_ir_mem_free_bins, 0, sizeof mak_ir_mem_free_bins);"
        );
        let _ = writeln!(
            self.out,
            "    mak_ir_mem_lock_leave();"
        );
        let _ = writeln!(self.out, "}}\n");
    }

    fn emit_recovery_helper(&mut self) {
        let _ = writeln!(self.out, "#ifndef MAK_RUNTIME_STRICT");
        let _ = writeln!(self.out, "#define MAK_RUNTIME_STRICT 0");
        let _ = writeln!(self.out, "#endif");
        let _ = writeln!(self.out, "static uint64_t mak_rt_recovery_count = 0;");
        let _ = writeln!(
            self.out,
            "static inline void mak_ir_recover(const char *reason) {{"
        );
        let _ = writeln!(self.out, "    (void)reason; ++mak_rt_recovery_count;");
        let _ = writeln!(self.out, "#if MAK_RUNTIME_STRICT");
        let _ = writeln!(
            self.out,
            "    fprintf(stderr, \"[MAK] runtime failure: %s\\n\", reason); abort();"
        );
        let _ = writeln!(self.out, "#endif");
        let _ = writeln!(self.out, "}}");
        let _ = writeln!(
            self.out,
            "static inline uint64_t mak_ir_fail_u64(const char *reason) {{ mak_ir_recover(reason); return 0; }}\n"
        );
    }

    fn emit_runtime_resolver(&mut self) {
        if self.plan.on(RtFeature::FloatBits) {
            let _ = writeln!(
                self.out,
                "static inline uint64_t mak_rt_f32_bits(float x) {{ uint32_t u; memcpy(&u, &x, sizeof u); return (uint64_t)u; }}"
            );
            let _ = writeln!(
                self.out,
                "static inline uint64_t mak_rt_f64_bits(double x) {{ uint64_t u; memcpy(&u, &x, sizeof u); return u; }}"
            );
            let _ = writeln!(
                self.out,
                "static inline float mak_rt_f32_value(uint64_t u) {{ float x; uint32_t v=(uint32_t)u; memcpy(&x, &v, sizeof x); return x; }}"
            );
            let _ = writeln!(
                self.out,
                "static inline double mak_rt_f64_value(uint64_t u) {{ double x; memcpy(&x, &u, sizeof x); return x; }}"
            );
        }
        if self.plan.on(RtFeature::FoldEq) {
            let _ = writeln!(
                self.out,
                "static int mak_rt_fold_eq(const char *a, const char *b) {{"
            );
            let _ = writeln!(
                self.out,
                "    while (*a || *b) {{ unsigned char ca=(unsigned char)*a++, cb=(unsigned char)*b++; if (ca>='A'&&ca<='Z') ca=(unsigned char)(ca+('a'-'A')); if (cb>='A'&&cb<='Z') cb=(unsigned char)(cb+('a'-'A')); if (ca!=cb) return 0; }}"
            );
            let _ = writeln!(self.out, "    return 1;");
            let _ = writeln!(self.out, "}}");
        }
        if self.plan.on(RtFeature::CanonEq) {
            let _ = writeln!(
                self.out,
                "static int mak_rt_canon_eq(const char *a, const char *b) {{ for (;;) {{ while (*a && !((*a>='0'&&*a<='9')||(*a>='A'&&*a<='Z')||(*a>='a'&&*a<='z')||*a=='_')) ++a; while (*b && !((*b>='0'&&*b<='9')||(*b>='A'&&*b<='Z')||(*b>='a'&&*b<='z')||*b=='_')) ++b; if (!*a || !*b) return *a==*b; unsigned char ca=(unsigned char)*a++, cb=(unsigned char)*b++; if (ca>='A'&&ca<='Z') ca=(unsigned char)(ca+('a'-'A')); if (cb>='A'&&cb<='Z') cb=(unsigned char)(cb+('a'-'A')); if (ca!=cb) return 0; }} }}"
            );
        }
        if self.plan.on(RtFeature::KindName) {
            let _ = writeln!(
                self.out,
                "static const char *mak_rt_kind_name(uint8_t k) {{ switch(k) {{ case 1:return \"integer\"; case 2:return \"float\"; case 3:return \"string\"; case 4:return \"pointer/ADT\"; case 5:return \"function\"; default:return \"unknown\"; }} }}"
            );
        }
        if self.plan.on(RtFeature::KindCompatible) {
            let _ = writeln!(
                self.out,
                "static int mak_rt_kind_compatible(uint8_t actual, uint8_t expected) {{ return actual==0 || expected==0 || actual==expected; }}"
            );
        }
        if self.plan.on(RtFeature::Hash) {
            let _ = writeln!(
                self.out,
                "static inline uint64_t mak_rt_hash(const char *s) {{ uint64_t h=1469598103934665603ULL; while(*s){{ h^=(unsigned char)*s++; h*=1099511628211ULL; }} return h; }}"
            );
        }

        if self.plan.on(RtFeature::FnDescType) {
            let _ = writeln!(
                self.out,
                "typedef uint64_t (*mak_rt_fn)(const uint64_t *, const uint8_t *, size_t);"
            );
        }
        if self.plan.on(RtFeature::CtorDescType) {
            let _ = writeln!(
                self.out,
                "typedef uintptr_t (*mak_rt_ctor)(const uint64_t *, const uint8_t *, size_t);"
            );
        }

        for id in self.plan.runtime_fn_ids.iter().copied().collect::<Vec<_>>() {
            self.emit_runtime_function_wrapper(id);
        }
        for id in self
            .plan
            .runtime_ctor_ids
            .iter()
            .copied()
            .collect::<Vec<_>>()
        {
            self.emit_runtime_ctor_wrapper(id);
        }

        if self.plan.on(RtFeature::FnTable) {
            let mut rows = Vec::new();
            for id in &self.plan.runtime_fn_ids {
                let f = &self.a.functions[*id];
                let canon = canonical_ident(f.source_name.as_deref().unwrap_or(""));
                let kinds = {
                    let x = f
                        .param_reps
                        .iter()
                        .map(runtime_kind_for_rep)
                        .map(|x| x.to_string())
                        .collect::<Vec<_>>()
                        .join(",");
                    if x.is_empty() { "0".to_string() } else { x }
                };
                let kinds_name = format!("mak_rt_fk_{}", id);
                let _ = writeln!(
                    self.out,
                    "static const uint8_t {}[]={{ {} }};",
                    kinds_name, kinds
                );
                rows.push(format!(
                    "{{\"{}\",\"{}\",{}, {}, {}, {}}}",
                    c_escape(f.source_name.as_deref().unwrap_or("")),
                    c_escape(&canon),
                    f.params.len(),
                    runtime_kind_for_rep(&f.ret_rep),
                    kinds_name,
                    format!("mak_rt_wrap_fn_{}", id)
                ));
            }
            let _ = writeln!(
                self.out,
                "struct mak_rt_fn_desc {{ const char *name; const char *canon; size_t arity; uint8_t ret_kind; const uint8_t *kinds; mak_rt_fn call; }};"
            );
            if rows.is_empty() {


                let _ = writeln!(
                    self.out,
                    "static const struct mak_rt_fn_desc mak_rt_functions[] = {{ {{\"\",\"\",0,0,NULL,(void *)0}} }};"
                );
                let _ = writeln!(self.out, "static const size_t mak_rt_function_count = 0;");
            } else {
                let _ = writeln!(
                    self.out,
                    "static const struct mak_rt_fn_desc mak_rt_functions[]={{ {} }};",
                    rows.join(",")
                );
                let _ = writeln!(
                    self.out,
                    "static const size_t mak_rt_function_count=sizeof(mak_rt_functions)/sizeof(mak_rt_functions[0]);"
                );
            }
        }

        if self.plan.on(RtFeature::FnDispatcher) {
            let _ = writeln!(
                self.out,
                "static uint64_t mak_rt_call_function(const char *query, const uint64_t *args, const uint8_t *kinds, size_t n, uint8_t expected_ret) {{"
            );
            let _ = writeln!(
                self.out,
                "    static _Thread_local const struct mak_rt_fn_desc *cache[128]; static _Thread_local uint64_t keys[128]; static _Thread_local uint32_t sig[128];"
            );
            let _ = writeln!(
                self.out,
                "    uint64_t h=mak_rt_hash(query)^((uint64_t)n*0x9e3779b97f4a7c15ULL)^((uint64_t)expected_ret<<56); for(size_t j=0;j<n;j++) h=(h*1099511628211ULL)^kinds[j]; size_t slot=(size_t)h&127u;"
            );
            let _ = writeln!(
                self.out,
                "    const struct mak_rt_fn_desc *hit=cache[slot]; if(hit && keys[slot]==h && sig[slot]==(uint32_t)(n*257u+expected_ret) && hit->arity==n && (expected_ret==0 || hit->ret_kind==expected_ret)) {{ int ok=1; if(!(strcmp(query,hit->name)==0 || mak_rt_fold_eq(query,hit->name) || mak_rt_canon_eq(query,hit->name))) ok=0; for(size_t j=0;j<n && ok;j++) if(!mak_rt_kind_compatible(kinds[j],hit->kinds[j])) ok=0; if(ok) return hit->call(args,kinds,n); }}"
            );
            let _ = writeln!(
                self.out,
                "    const struct mak_rt_fn_desc *best=NULL; int best_score=-1, ties=0; for(size_t i=0;i<mak_rt_function_count;i++) {{ const struct mak_rt_fn_desc *d=&mak_rt_functions[i]; if(d->arity!=n) continue; int score=0; if(!strcmp(query,d->name)) score+=1000; else if(mak_rt_fold_eq(query,d->name)) score+=800; else if(mak_rt_canon_eq(query,d->name)) score+=650; for(size_t j=0;j<n;j++) if(mak_rt_kind_compatible(kinds[j],d->kinds[j])) score+=(kinds[j]==d->kinds[j]?20:3); if(expected_ret==0 || expected_ret==d->ret_kind) score+=25; else score-=100; if(score>best_score) {{best=d;best_score=score;ties=0;}} else if(score==best_score) ++ties; }}"
            );
            let _ = writeln!(
                self.out,
                "    if(!best) {{ mak_ir_recover(\"unresolved runtime function\"); return 0; }}"
            );
            let _ = writeln!(
                self.out,
                "    if(ties>1) {{ mak_ir_recover(\"runtime function tie\"); }}"
            );
            let _ = writeln!(
                self.out,
                "    cache[slot]=best; keys[slot]=h; sig[slot]=(uint32_t)(n*257u+expected_ret); return best->call(args,kinds,n);"
            );
            let _ = writeln!(self.out, "}}");
        }

        if self.plan.on(RtFeature::CtorTable) {
            let mut rows = Vec::new();
            for id in &self.plan.runtime_ctor_ids {
                let c = &self.a.constructors[*id];
                let kinds = {
                    let x = c
                        .fields
                        .iter()
                        .map(runtime_kind_for_rep)
                        .map(|x| x.to_string())
                        .collect::<Vec<_>>()
                        .join(",");
                    if x.is_empty() { "0".to_string() } else { x }
                };
                let kn = format!("mak_rt_ck_{}", id);
                let _ = writeln!(self.out, "static const uint8_t {}[]={{ {} }};", kn, kinds);
                rows.push(format!(
                    "{{\"{}\",\"{}\",\"{}\",{}, {}, {}}}",
                    c_escape(&c.name),
                    c_escape(&canonical_ident(&c.name)),
                    c_escape(&c.type_name),
                    c.arity,
                    kn,
                    format!("mak_rt_wrap_ctor_{}", id)
                ));
            }
            let _ = writeln!(
                self.out,
                "struct mak_rt_ctor_desc {{ const char *name; const char *canon; const char *type; size_t arity; const uint8_t *kinds; mak_rt_ctor make; }};"
            );
            if rows.is_empty() {
                let _ = writeln!(
                    self.out,
                    "static const struct mak_rt_ctor_desc mak_rt_ctors[] = {{ {{\"\",\"\",\"\",0,NULL,(void *)0}} }};"
                );
                let _ = writeln!(self.out, "static const size_t mak_rt_ctor_count = 0;");
            } else {
                let _ = writeln!(
                    self.out,
                    "static const struct mak_rt_ctor_desc mak_rt_ctors[]={{ {} }};",
                    rows.join(",")
                );
                let _ = writeln!(
                    self.out,
                    "static const size_t mak_rt_ctor_count=sizeof(mak_rt_ctors)/sizeof(mak_rt_ctors[0]);"
                );
            }
        }

        if self.plan.on(RtFeature::CtorDispatcher) {
            let _ = writeln!(
                self.out,
                "static uintptr_t mak_rt_call_ctor(const char *query, const uint64_t *args, const uint8_t *kinds, size_t n) {{"
            );
            let _ = writeln!(
                self.out,
                "    char name_buf[256]; const char *type_hint=NULL;"
            );
            let _ = writeln!(self.out, "    const char *colon=strchr(query,':');");
            let _ = writeln!(
                self.out,
                "    if(colon) {{ size_t len=(size_t)(colon-query); if(len>=sizeof name_buf) len=sizeof name_buf-1; memcpy(name_buf,query,len); name_buf[len]=0; type_hint=colon+1; query=name_buf; }}"
            );
            let _ = writeln!(
                self.out,
                "    static _Thread_local const struct mak_rt_ctor_desc *cache[128]; static _Thread_local uint64_t keys[128]; static _Thread_local uint32_t sig[128]; uint64_t h=mak_rt_hash(query)^((uint64_t)n*0x9e3779b97f4a7c15ULL); for(size_t j=0;j<n;j++) h=(h*1099511628211ULL)^kinds[j]; size_t slot=(size_t)h&127u; const struct mak_rt_ctor_desc *hit=cache[slot];"
            );
            let _ = writeln!(
                self.out,
                "    if(hit && keys[slot]==h && sig[slot]==(uint32_t)n && hit->arity==n && (strcmp(query,hit->name)==0 || mak_rt_fold_eq(query,hit->name) || mak_rt_canon_eq(query,hit->name))) {{ int ok=1; for(size_t j=0;j<n && ok;j++) if(!mak_rt_kind_compatible(kinds[j],hit->kinds[j])) ok=0; if(ok && (!type_hint || strcmp(type_hint,hit->type)==0)) return hit->make(args,kinds,n); }}"
            );
            let _ = writeln!(
                self.out,
                "    const struct mak_rt_ctor_desc *best=NULL; int best_score=-1,ties=0; for(size_t i=0;i<mak_rt_ctor_count;i++) {{ const struct mak_rt_ctor_desc *d=&mak_rt_ctors[i]; if(d->arity!=n) continue; if(type_hint && strcmp(type_hint,d->type)!=0) continue; int score=0; if(!strcmp(query,d->name))score+=1000; else if(mak_rt_fold_eq(query,d->name))score+=800; else if(mak_rt_canon_eq(query,d->name))score+=650; for(size_t j=0;j<n;j++) if(mak_rt_kind_compatible(kinds[j],d->kinds[j])) score+=(kinds[j]==d->kinds[j]?20:3); if(score>best_score) {{best=d;best_score=score;ties=0;}} else if(score==best_score) ++ties; }}"
            );
            let _ = writeln!(
                self.out,
                "    if(!best) {{ mak_ir_recover(\"unresolved runtime constructor\"); return (uintptr_t)0; }}"
            );
            let _ = writeln!(
                self.out,
                "    if(ties>1) {{ mak_ir_recover(\"ambiguous runtime constructor\"); return (uintptr_t)0; }}"
            );
            let _ = writeln!(
                self.out,
                "    cache[slot]=best; keys[slot]=h; sig[slot]=(uint32_t)n; return best->make(args,kinds,n); }}"
            );
        }
    }

    fn emit_runtime_function_wrapper(&mut self, id: usize) {
        let f = &self.a.functions[id];
        let _ = writeln!(
            self.out,
            "static uint64_t mak_rt_wrap_fn_{}(const uint64_t *v,const uint8_t *k,size_t n) {{",
            id
        );
        let _ = writeln!(self.out, "    (void)k; (void)n;");
        let mut xs = Vec::new();
        for (i, r) in f.param_reps.iter().enumerate() {
            let nme = format!("mak_rt_a_{}", i);
            let code = runtime_arg_from_rep(r, &format!("v[{}]", i), self.a);
            let decl = rep_decl(r, &nme, &self.a.functions);
            let _ = writeln!(self.out, "    {} = {};", decl, code);
            xs.push(nme);
        }
        if rep_is_adt(&f.ret_rep) {
            let ty = adt_struct_type(&f.ret_rep);
            let _ = writeln!(
                self.out,
                "    {} *out=( {} *)mak_ir_heap_alloc(sizeof({}),_Alignof({}));",
                ty, ty, ty, ty
            );
            let _ = writeln!(
                self.out,
                "    if(!out) {{ mak_ir_recover(\"runtime function result allocation failed\"); return (uint64_t)0; }}"
            );
            xs.push("out".into());
            let _ = writeln!(self.out, "    mak_ir_fn_{}({});", id, xs.join(","));
            let _ = writeln!(self.out, "    return (uint64_t)(uintptr_t)out;");
        } else if matches!(f.ret_rep, Rep::Unit) {
            let _ = writeln!(self.out, "    mak_ir_fn_{}({});", id, xs.join(","));
            let _ = writeln!(self.out, "    return 0;");
        } else {
            let _ = writeln!(
                self.out,
                "    {} rv=mak_ir_fn_{}({});",
                f.ret_rep.ctype(&self.a.functions),
                id,
                xs.join(",")
            );
            match f.ret_rep {
                Rep::F32 | Rep::F64 => {
                    let _ = writeln!(
                        self.out,
                        "    return mak_rt_{}_bits(rv);",
                        if matches!(f.ret_rep, Rep::F32) {
                            "f32"
                        } else {
                            "f64"
                        }
                    );
                }
                Rep::Str | Rep::Ptr => {
                    let _ = writeln!(self.out, "    return (uint64_t)(uintptr_t)rv;");
                }
                _ => {
                    let _ = writeln!(self.out, "    return (uint64_t)rv;");
                }
            }
        }
        let _ = writeln!(self.out, "}}");
    }

    fn emit_runtime_ctor_wrapper(&mut self, id: usize) {
        let c = &self.a.constructors[id];
        let _ = writeln!(
            self.out,
            "static uintptr_t mak_rt_wrap_ctor_{}(const uint64_t *v,const uint8_t *k,size_t n) {{",
            id
        );
        let _ = writeln!(self.out, "    (void)k; (void)n;");
        if c.arity == 0 {
            let _ = writeln!(self.out, "    return (uintptr_t)&mak_ir_ctor_obj_{};", id);
        } else {
            let ty = format!("struct mak_{}", mangle_ident(&c.type_name));
            let _ = writeln!(
                self.out,
                "    {} *out=({} *)mak_ir_heap_alloc(sizeof({}),_Alignof({}));",
                ty, ty, ty, ty
            );
            let _ = writeln!(
                self.out,
                "    if(!out) {{ mak_ir_recover(\"runtime constructor allocation failed\"); return (uintptr_t)0; }}"
            );
            let _ = writeln!(self.out, "    out->tag={};", c.tag);
            for (i, r) in c.fields.iter().enumerate() {
                let _ = writeln!(
                    self.out,
                    "    out->{}f{}={};",
                    constructor_field_prefix(self.a, id),
                    i,
                    runtime_arg_from_rep(r, &format!("v[{}]", i), self.a)
                );
            }
            let _ = writeln!(self.out, "    return (uintptr_t)out;");
        }
        let _ = writeln!(self.out, "}}");
    }

    fn emit_externals(&mut self) {
        for e in &self.a.externals {
            match &e.decl.foreign {
                ForeignTy::Atom(a) => {


                    let c_type = foreign_index_ctype(&ForeignTy::Atom(a.clone()));
                    let _ = writeln!(
                        self.out,
                        "extern {} {} __asm__(\"{}\");",
                        c_type,
                        e.c_name,
                        c_escape(&e.decl.symbol)
                    );
                }
                ForeignTy::Fun { args, ret } => {
                    let _ = write!(self.out, "extern {} {}(", foreign_ret_type(ret), e.c_name);
                    for (i, a) in args.iter().enumerate() {
                        if i > 0 {
                            self.out.push_str(", ");
                        }
                        match a {
                            ForeignArg::Ty(t) => self.out.push_str(&foreign_arg_type(t)),
                            ForeignArg::VarArgs => self.out.push_str("..."),
                        }
                    }
                    if args.is_empty() {
                        self.out.push_str("void");
                    }
                    if e.noreturn {
                        let _ = writeln!(
                            self.out,
                            ") __attribute__((noreturn)) __asm__(\"{}\");",
                            c_escape(&e.decl.symbol)
                        );
                    } else {
                        let _ = writeln!(self.out, ") __asm__(\"{}\");", c_escape(&e.decl.symbol));
                    }
                }
            }
        }
        if !self.a.externals.is_empty() {
            self.out.push('\n');
        }
    }

    fn emit_prototypes(&mut self, fs: &[LirFunction]) {
        for f in fs {
            let _ = writeln!(self.out, "static {};", self.function_decl(f));
        }
        if !fs.is_empty() {
            self.out.push('\n');
        }
    }

    fn emit_globals(&mut self, p: &Program) {
        for d in &p.decls {
            if let Decl::Let { name, value, .. } = d {

                let r = if let Some(id) = self.a.resolve_function_name(name) {
                    Rep::Function(id)
                } else {
                    infer_ast_rep(value, self.a)
                };

                if let Rep::Adt(_) = r {
                    let storage = format!("mak_global_storage_{}", mangle_ident(name));
                    let _ = writeln!(self.out, "static {} {};", adt_struct_type(&r), storage);
                    let _ = writeln!(
                        self.out,
                        "static {} *{} = &{};",
                        adt_struct_type(&r),
                        user_value_name(name),
                        storage
                    );
                } else if let Rep::Function(id) = r {
                    let _ = writeln!(
                        self.out,
                        "static {};",
                        function_pointer_decl(id, &user_value_name(name), &self.a.functions)
                    );
                } else if matches!(&r, Rep::ExternFunction { .. }) {
                    let decl = extern_function_pointer_decl(&r, &user_value_name(name))
                        .unwrap_or_else(|| format!("void (*{})(void)", user_value_name(name)));
                    let _ = writeln!(self.out, "static {};", decl);
                } else {
                    let _ = writeln!(
                        self.out,
                        "static {} {};",
                        r.ctype(&self.a.functions),
                        user_value_name(name)
                    );
                }
            }
        }
        if p.decls.iter().any(|d| matches!(d, Decl::Let { .. })) {
            self.out.push('\n');
        }
    }
    fn emit_function(&mut self, f: &LirFunction) {
        let body = f.body.clone();
        if lir_contains_fallback(&body) {
            self.a.dbg(
                "RUNTIME",
                format!(
                    "fallback reached CEmitter in f{}; runtime recovery will handle it",
                    f.info.id
                ),
            );
        }
        self.current_ret = f.info.ret_rep.clone();
        self.current_function = Some(f.info.id);
        self.current_out = if rep_is_adt(&f.info.ret_rep) {
            Some("out".into())
        } else {
            None
        };
        let _ = writeln!(self.out, "static {} {{", self.function_decl(f));
        if lir_has_self_tail_call(&body, f.info.id) {
            let _ = writeln!(self.out, "    for (;;) {{");
            self.emit_tail(&body, 2);
            let _ = writeln!(self.out, "    }}");
        } else {
            self.emit_tail(&body, 1);
        }
        let _ = writeln!(self.out, "}}\n");
        self.current_function = None;
        self.current_out = None;
    }

    fn emit_main(&mut self, p: &Program, fs: &[LirFunction], entry: Option<usize>) {
        self.current_ret = Rep::I32;
        self.current_function = None;
        self.out.push_str("int main(void) {\n");
        self.emit_globals_init(p);
        if let Some(id) = entry.and_then(|i| fs.iter().find(|f| f.info.id == i)) {
            self.current_function = Some(id.info.id);
            self.emit_as_main(&id.body, 1);
            self.current_function = None;
        } else {
            let mut lowerer = Lowerer::new(self.a);
            let mut env = HashMap::new();
            let body = lowerer.lower_expr(&p.body, &mut env, None);


            let optimizer = Optimizer { a: self.a };
            let (body, _) = optimizer.optimize_expr(body);
            self.emit_expr_stmt(&body, 1);
        }
        if self.plan.on(RtFeature::Arena) {
            self.out.push_str("    mak_ir_arena_shutdown();\n");
        }
        self.out.push_str("    return 0;\n}\n");
    }

    fn emit_globals_init(&mut self, p: &Program) {
        let mut lowerer = Lowerer::new(self.a);
        let mut env = HashMap::new();
        for d in &p.decls {
            if let Decl::Let { name, value, .. } = d {


                let ir = lowerer.lower_expr(value, &mut env, None);
                let r = if let Some(id) = self.a.resolve_function_name(name) {
                    Rep::Function(id)
                } else {
                    infer_ast_rep(value, self.a)
                };
                if let Rep::Adt(_) = &r {
                    let storage = format!("mak_global_storage_{}", mangle_ident(name));
                    self.emit_default_into(&format!("&{}", storage), &r, 1);
                    self.emit_into(&ir, &format!("&{}", storage), &r, 1);
                } else if matches!(r, Rep::Unit) {
                    self.emit_expr_stmt(&ir, 1);
                } else {
                    let code = self.emit_value(&ir, 1);
                    if !code.is_empty() {
                        let _ = writeln!(
                            self.out,
                            "    {} = {};",
                            user_value_name(name),
                            cast_code(&code, &lir_rep(&ir), &r, self.a)
                        );
                    }
                }
            }
        }
    }

    fn emit_as_main(&mut self, e: &LirExpr, depth: usize) {
        match e {
            LirExpr::Block { stmts, tail } => {
                for s in stmts {
                    self.emit_expr_stmt(s, depth);
                }
                self.emit_expr_stmt(tail, depth);
            }
            _ => self.emit_expr_stmt(e, depth),
        }
    }

    fn emit_tail(&mut self, e: &LirExpr, depth: usize) {
        if let LirExpr::DirectC { code, rep } = e {


            if rep_is_adt(&self.current_ret) {
                let out = self.current_out.clone().unwrap_or_else(|| "out".into());
                let current_ret = self.current_ret.clone();
                self.emit_into(e, &out, &current_ret, depth);
                let _ = writeln!(self.out, "{}return;", indent(depth));
            } else if rep_is_adt(rep) {
                let _ = writeln!(self.out, "{}*out = *({});", indent(depth), code);
                let _ = writeln!(self.out, "{}return;", indent(depth));
            } else if matches!(rep, Rep::Unit) {
                if !code.is_empty() {
                    let _ = writeln!(self.out, "{}{};", indent(depth), code);
                }
                let _ = writeln!(self.out, "{}return;", indent(depth));
            } else {
                let _ = writeln!(
                    self.out,
                    "{}return {};",
                    indent(depth),
                    cast_code(code, rep, &self.current_ret, self.a)
                );
            }
            return;
        }
        if let Some(out) = self.current_out.clone() {
            if let LirExpr::Call {
                target: CallTarget::Function(fid),
                args,
                noreturn,
                ..
            } = e
            {
                if self.current_function == Some(*fid)
                    && !*noreturn
                    && *fid < self.a.functions.len()
                    && self.a.functions[*fid].captures.is_empty()
                    && args.len() == self.a.functions[*fid].params.len()
                {
                    let mut temps = Vec::with_capacity(args.len());
                    for arg in args {
                        let rep = lir_rep(arg);
                        let tmp = self.fresh_temp("tailarg");
                        let c = self.materialize_arg(arg, depth);
                        let decl = rep_decl(&rep, &tmp, &self.a.functions);
                        let _ = writeln!(
                            self.out,
                            "{}{} = {};",
                            indent(depth),
                            decl,
                            cast_code(&c, &lir_rep(arg), &rep, self.a)
                        );
                        temps.push(tmp);
                    }
                    for (i, tmp) in temps.iter().enumerate() {
                        let rep = self.a.functions[*fid].param_reps[i].clone();
                        let _ = writeln!(
                            self.out,
                            "{}a{} = {};",
                            indent(depth),
                            i,
                            cast_code(tmp, &rep, &rep, self.a)
                        );
                    }
                    let _ = writeln!(self.out, "{}continue;", indent(depth));
                    return;
                }
            }
            let current_ret = self.current_ret.clone();
            self.emit_into(e, &out, &current_ret, depth);
            let _ = writeln!(self.out, "{}return;", indent(depth));
            return;
        }
        match e {
            LirExpr::Unit => {
                let _ = writeln!(self.out, "{}return;", indent(depth));
            }
            LirExpr::Call {
                target: CallTarget::Function(fid),
                args,
                noreturn,
                ..
            } if self.current_function == Some(*fid)
                && !*noreturn
                && *fid < self.a.functions.len()
                && self.a.functions[*fid].captures.is_empty()
                && args.len() == self.a.functions[*fid].params.len() =>
            {
                let mut temps = Vec::with_capacity(args.len());
                for arg in args {
                    let rep = lir_rep(arg);
                    let tmp = self.fresh_temp("tailarg");
                    let c = self.emit_value(arg, depth);
                    let decl = rep_decl(&rep, &tmp, &self.a.functions);
                    let _ = writeln!(
                        self.out,
                        "{}{} = {};",
                        indent(depth),
                        decl,
                        cast_code(&c, &lir_rep(arg), &rep, self.a)
                    );
                    temps.push(tmp);
                }
                for (i, tmp) in temps.iter().enumerate() {
                    let rep = self.a.functions[*fid].param_reps[i].clone();
                    let _ = writeln!(
                        self.out,
                        "{}a{} = {};",
                        indent(depth),
                        i,
                        cast_code(tmp, &lir_rep(&args[i]), &rep, self.a)
                    );
                }
                let _ = writeln!(self.out, "{}continue;", indent(depth));
            }
            LirExpr::Call { noreturn, .. } => {
                let code = self.emit_value(e, depth);
                if *noreturn {
                    if !code.is_empty() {
                        let _ = writeln!(self.out, "{}{};", indent(depth), code);
                    }
                    self.emit_default_return(depth, "unexpected return from noreturn call");
                } else if matches!(self.current_ret, Rep::Unit) {
                    if !code.is_empty() {
                        let _ = writeln!(self.out, "{}(void){};", indent(depth), code);
                    }
                    let _ = writeln!(self.out, "{}return;", indent(depth));
                } else if !code.is_empty() {
                    let _ = writeln!(
                        self.out,
                        "{}return {};",
                        indent(depth),
                        cast_code(&code, &lir_rep(e), &self.current_ret, self.a)
                    );
                }
            }
            LirExpr::Case { scrut, arms, .. } => self.emit_tail_case(scrut, arms, depth),
            LirExpr::Let {
                code,
                rep,
                value,
                body,
            } => {
                self.emit_local_let(code, rep, value, depth);
                self.emit_tail(body, depth);
            }
            LirExpr::Block { stmts, tail } => {
                for s in stmts {
                    self.emit_expr_stmt(s, depth);
                }
                self.emit_tail(tail, depth);
            }
            _ => {
                let code = self.emit_value(e, depth);
                if matches!(self.current_ret, Rep::Unit) {
                    if !code.is_empty() {
                        let _ = writeln!(self.out, "{}(void){};", indent(depth), code);
                    }
                    let _ = writeln!(self.out, "{}return;", indent(depth));
                } else if !code.is_empty() {
                    let _ = writeln!(
                        self.out,
                        "{}return {};",
                        indent(depth),
                        cast_code(&code, &lir_rep(e), &self.current_ret, self.a)
                    );
                }
            }
        }
    }

    fn emit_local_let(&mut self, code: &str, rep: &Rep, value: &LirExpr, depth: usize) {
        if matches!(rep, Rep::Unit) {
            self.emit_expr_stmt(value, depth);
            return;
        }
        if let Rep::Adt(_) = rep {


            let storage = self.fresh_temp("letobj");
            let ty = adt_struct_type(rep);
            let stack_safe = self
                .memory
                .stack_local_lets
                .contains_key(&(self.current_function, code.to_string()))
                && !self.memory.force_heap_unknown
                && match rep {
                    Rep::Adt(name) => adt_object_layout(self.a, name).0 <= MemoryPlan::STACK_OBJECT_LIMIT,
                    _ => false,
                };
            if stack_safe {
                let _ = writeln!(self.out, "{}{} {};", indent(depth), ty, storage);
                let storage_ref = format!("&{}", storage);
                self.emit_default_into(&storage_ref, rep, depth);
                self.emit_into(value, &storage_ref, rep, depth);
                let _ = writeln!(
                    self.out,
                    "{}{} * {} = &{};",
                    indent(depth), ty, code, storage
                );
            } else {
                let strategy = self.emit_adt_temp_alloc(
                    &storage,
                    rep,
                    depth,
                    MemorySiteKind::Construct,
                    "ADT local allocation failed",
                );
                self.emit_default_into(&storage, rep, depth + 1);
                self.emit_into(value, &storage, rep, depth + 1);
                self.emit_adt_temp_close(strategy, depth);
                let _ = writeln!(
                    self.out,
                    "{}{} * {} = {};",
                    indent(depth), ty, code, storage
                );
            }
            return;
        }
        let c = self.emit_value(value, depth);
        let _ = writeln!(
            self.out,
            "{}{} {} = {};",
            indent(depth),
            rep.ctype(&self.a.functions),
            code,
            cast_code(&c, &lir_rep(value), rep, self.a)
        );
    }

    fn emit_tail_case(&mut self, scrut: &LirExpr, arms: &[LirArm], depth: usize) {
        let (scrut_code, scrut_rep) = self.materialize_scrutinee(scrut, depth);
        if can_switch(arms) && scrut_rep.is_integer() {
            let _ = writeln!(self.out, "{}switch ({}) {{", indent(depth), scrut_code);
            let mut has_default = false;
            for arm in arms {
                match &arm.pat {
                    LirPat::Int(v) => {
                        let _ = writeln!(
                            self.out,
                            "{}case {}: {{",
                            indent(depth + 1),
                            c_uint_case(*v)
                        );
                        self.emit_tail(&arm.body, depth + 2);
                        let _ = writeln!(self.out, "{}}}", indent(depth + 1));
                    }
                    LirPat::Wild | LirPat::Bind { .. } => {
                        has_default = true;
                        let _ = writeln!(self.out, "{}default: {{", indent(depth + 1));
                        self.emit_pattern_binds(&arm.pat, &scrut_code, &scrut_rep, depth + 2);
                        self.emit_tail(&arm.body, depth + 2);
                        let _ = writeln!(self.out, "{}}}", indent(depth + 1));
                    }
                    _ => {}
                }
            }
            if !has_default {
                let _ = writeln!(self.out, "{}default: {{", indent(depth + 1));
                self.emit_default_return(depth + 2, "integer case had no matching arm");
                let _ = writeln!(self.out, "{}}}", indent(depth + 1));
            }
            let _ = writeln!(self.out, "{}}}", indent(depth));
            return;
        }
        for (i, arm) in arms.iter().enumerate() {
            let cond = self.pattern_cond(&arm.pat, &scrut_code, &scrut_rep);
            if i == 0 {
                let _ = writeln!(self.out, "{}if ({}) {{", indent(depth), cond);
            } else {
                let _ = writeln!(self.out, "{}else if ({}) {{", indent(depth), cond);
            }
            self.emit_pattern_binds(&arm.pat, &scrut_code, &scrut_rep, depth + 1);
            self.emit_tail(&arm.body, depth + 1);
            let _ = writeln!(self.out, "{}}}", indent(depth));
            if matches!(arm.pat, LirPat::Wild | LirPat::Bind { .. }) {
                return;
            }
        }
        if !has_exhaustive_wild_lir(arms) {
            let _ = writeln!(self.out, "{}else {{", indent(depth));
            self.emit_default_return(depth + 1, "case had no matching arm");
            let _ = writeln!(self.out, "{}}}", indent(depth));
        }
    }

    fn emit_expr_stmt(&mut self, e: &LirExpr, depth: usize) {
        match e {
            LirExpr::Unit => {}
            LirExpr::Block { stmts, tail } => {
                for s in stmts {
                    self.emit_expr_stmt(s, depth);
                }
                self.emit_expr_stmt(tail, depth);
            }
            LirExpr::Let {
                code,
                rep,
                value,
                body,
            } => {
                self.emit_local_let(code, rep, value, depth);
                self.emit_expr_stmt(body, depth);
            }
            LirExpr::Case { .. } => {
                let r = lir_rep(e);
                if rep_is_adt(&r) {
                    let storage = self.fresh_temp("stackcase");
                    let ty = adt_struct_type(&r);
                    let _ = writeln!(self.out, "{}{} {} = {{ 0 }};", indent(depth), ty, storage);
                    self.emit_default_into(&format!("&{}", storage), &r, depth);
                    self.emit_case_into(e, &format!("&{}", storage), &r, depth);
                    return;
                }
                if matches!(r, Rep::Unit) {
                    if let LirExpr::Case { scrut, arms, .. } = e {
                        self.emit_tail_case(scrut, arms, depth);
                    }
                } else {
                    let tmp = self.fresh_temp("discard");
                    let decl = rep_decl(&r, &tmp, &self.a.functions);
                    let _ = writeln!(
                        self.out,
                        "{}{} = {};",
                        indent(depth),
                        decl,
                        zero_for(&r)
                    );
                    self.emit_case_into(e, &tmp, &r, depth);
                }
            }
            LirExpr::Call { noreturn, .. } => {
                let c = self.emit_value(e, depth);
                if !c.is_empty() {
                    let _ = writeln!(self.out, "{}{};", indent(depth), c);
                }
                let _ = noreturn;
            }
            _ => {
                let c = self.emit_value(e, depth);
                if !c.is_empty() {
                    let _ = writeln!(self.out, "{}(void){};", indent(depth), c);
                }
            }
        }
    }

    fn emit_case_into(&mut self, e: &LirExpr, dest: &str, dest_rep: &Rep, depth: usize) {
        let LirExpr::Case { scrut, arms, .. } = e else {
            return;
        };
        let (s, sr) = self.materialize_scrutinee(scrut, depth);
        let result_rep = dest_rep.clone();

        if rep_is_adt(&result_rep) {
            if can_switch(arms) && sr.is_integer() {
                let _ = writeln!(self.out, "{}switch ({}) {{", indent(depth), s);
                let mut has_default = false;
                for arm in arms {
                    match &arm.pat {
                        LirPat::Int(v) => {
                            let _ = writeln!(
                                self.out,
                                "{}case {}: {{",
                                indent(depth + 1),
                                c_uint_case(*v)
                            );
                            let tail = self.is_sret_self_tail_expr(&arm.body);
                            self.emit_into(&arm.body, dest, &result_rep, depth + 2);
                            if !tail {
                                let _ = writeln!(self.out, "{}break;", indent(depth + 2));
                            }
                            let _ = writeln!(self.out, "{}}}", indent(depth + 1));
                        }
                        LirPat::Wild | LirPat::Bind { .. } => {
                            has_default = true;
                            let _ = writeln!(self.out, "{}default: {{", indent(depth + 1));
                            self.emit_pattern_binds(&arm.pat, &s, &sr, depth + 2);
                            let tail = self.is_sret_self_tail_expr(&arm.body);
                            self.emit_into(&arm.body, dest, &result_rep, depth + 2);
                            if !tail {
                                let _ = writeln!(self.out, "{}break;", indent(depth + 2));
                            }
                            let _ = writeln!(self.out, "{}}}", indent(depth + 1));
                        }
                        _ => {}
                    }
                }
                if !has_default {
                    let _ = writeln!(self.out, "{}default: {{", indent(depth + 1));
                    self.emit_default_into(dest, &result_rep, depth + 2);
                    let _ = writeln!(self.out, "{}}}", indent(depth + 1));
                }
                let _ = writeln!(self.out, "{}}}", indent(depth));
                return;
            }
            for (i, arm) in arms.iter().enumerate() {
                let cond = self.pattern_cond(&arm.pat, &s, &sr);
                if i == 0 {
                    let _ = writeln!(self.out, "{}if ({}) {{", indent(depth), cond);
                } else {
                    let _ = writeln!(self.out, "{}else if ({}) {{", indent(depth), cond);
                }
                self.emit_pattern_binds(&arm.pat, &s, &sr, depth + 1);
                self.emit_into(&arm.body, dest, &result_rep, depth + 1);
                let _ = writeln!(self.out, "{}}}", indent(depth));
                if matches!(arm.pat, LirPat::Wild | LirPat::Bind { .. }) {
                    return;
                }
            }
            if !has_exhaustive_wild_lir(arms) {
                let _ = writeln!(self.out, "{}else {{", indent(depth));
                self.emit_default_into(dest, &result_rep, depth + 1);
                let _ = writeln!(self.out, "{}}}", indent(depth));
            }
            return;
        }

        if can_switch(arms) && sr.is_integer() {
            let _ = writeln!(self.out, "{}switch ({}) {{", indent(depth), s);
            let mut has_default = false;
            for arm in arms {
                match &arm.pat {
                    LirPat::Int(v) => {
                        let _ = writeln!(
                            self.out,
                            "{}case {}: {{",
                            indent(depth + 1),
                            c_uint_case(*v)
                        );
                        let c = self.emit_value(&arm.body, depth + 2);
                        if !c.is_empty() {
                            let _ = writeln!(
                                self.out,
                                "{}{} = {};",
                                indent(depth + 2),
                                dest,
                                cast_code(&c, &lir_rep(&arm.body), &result_rep, self.a)
                            );
                        }
                        let _ = writeln!(self.out, "{}break;", indent(depth + 2));
                        let _ = writeln!(self.out, "{}}}", indent(depth + 1));
                    }
                    LirPat::Wild | LirPat::Bind { .. } => {
                        has_default = true;
                        let _ = writeln!(self.out, "{}default: {{", indent(depth + 1));
                        self.emit_pattern_binds(&arm.pat, &s, &sr, depth + 2);
                        let c = self.emit_value(&arm.body, depth + 2);
                        if !c.is_empty() {
                            let _ = writeln!(
                                self.out,
                                "{}{} = {};",
                                indent(depth + 2),
                                dest,
                                cast_code(&c, &lir_rep(&arm.body), &result_rep, self.a)
                            );
                        }
                        let _ = writeln!(self.out, "{}break;", indent(depth + 2));
                        let _ = writeln!(self.out, "{}}}", indent(depth + 1));
                    }
                    _ => {}
                }
            }
            if !has_default {
                let _ = writeln!(self.out, "{}default: {{", indent(depth + 1));
                self.emit_default_value(dest, &result_rep, depth + 2);
                let _ = writeln!(self.out, "{}}}", indent(depth + 1));
            }
            let _ = writeln!(self.out, "{}}}", indent(depth));
            return;
        }

        for (i, arm) in arms.iter().enumerate() {
            let cond = self.pattern_cond(&arm.pat, &s, &sr);
            if i == 0 {
                let _ = writeln!(self.out, "{}if ({}) {{", indent(depth), cond);
            } else {
                let _ = writeln!(self.out, "{}else if ({}) {{", indent(depth), cond);
            }
            self.emit_pattern_binds(&arm.pat, &s, &sr, depth + 1);
            let c = self.emit_value(&arm.body, depth + 1);
            if !c.is_empty() {
                let _ = writeln!(
                    self.out,
                    "{}{} = {};",
                    indent(depth + 1),
                    dest,
                    cast_code(&c, &lir_rep(&arm.body), &result_rep, self.a)
                );
            }
            let _ = writeln!(self.out, "{}}}", indent(depth));
            if matches!(arm.pat, LirPat::Wild | LirPat::Bind { .. }) {
                return;
            }
        }
        if !has_exhaustive_wild_lir(arms) {
            let _ = writeln!(self.out, "{}else {{", indent(depth));
            self.emit_default_value(dest, &result_rep, depth + 1);
            let _ = writeln!(self.out, "{}}}", indent(depth));
        }
    }

    fn materialize_scrutinee(&mut self, e: &LirExpr, depth: usize) -> (String, Rep) {
        let rep = lir_rep(e);
        if lir_is_simple_value(e) {
            (self.inline_code(e), rep)
        } else {
            let tmp = self.fresh_temp("scrut");
            let c = self.emit_value(e, depth);
            let decl = rep_decl(&rep, &tmp, &self.a.functions);
            let _ = writeln!(
                self.out,
                "{}{} = {};",
                indent(depth),
                decl,
                cast_code(&c, &lir_rep(e), &rep, self.a)
            );
            (tmp, rep)
        }
    }

    fn emit_pattern_binds(&mut self, pat: &LirPat, value: &str, value_rep: &Rep, depth: usize) {
        match pat {
            LirPat::Bind { code, rep } => {
                let rhs = cast_code(value, value_rep, rep, self.a);
                let decl = rep_decl(rep, code, &self.a.functions);
                let _ = writeln!(
                    self.out,
                    "{}{} = {};",
                    indent(depth),
                    decl,
                    rhs
                );
            }
            LirPat::Ctor { ctor, args } => {
                if *ctor == usize::MAX || *ctor >= self.a.constructors.len() {
                    for p in args {
                        self.emit_default_pattern_bind(p, depth);
                    }
                    return;
                }
                let c = &self.a.constructors[*ctor];
                if args.len() > c.arity {
                    for p in args {
                        self.emit_default_pattern_bind(p, depth);
                    }
                    return;
                }
                let expected = Rep::Adt(c.type_name.clone());
                let typed_value = cast_code(value, value_rep, &expected, self.a);
                for (i, p) in args.iter().enumerate() {
                    let Some(r) = c
                        .fields
                        .get(i)
                        .cloned()
                        .or_else(|| c.args.get(i).map(|t| rep_from_ty(t, &self.a.type_names)))
                    else {
                        self.emit_default_pattern_bind(p, depth);
                        continue;
                    };
                    let fr = format!(
                        "({})->{}f{}",
                        typed_value,
                        constructor_field_prefix(self.a, *ctor),
                        i
                    );
                    self.emit_pattern_binds(p, &fr, &r, depth);
                }
            }
            LirPat::Tuple(xs) => {
                for p in xs {
                    self.emit_default_pattern_bind(p, depth);
                }
            }
            LirPat::Wild | LirPat::Int(_) | LirPat::Str(_) => {}
        }
    }

    fn emit_default_pattern_bind(&mut self, pat: &LirPat, depth: usize) {
        match pat {
            LirPat::Bind { code, rep } => {
                let decl = rep_decl(rep, code, &self.a.functions);
                let _ = writeln!(
                    self.out,
                    "{}{} = {};",
                    indent(depth),
                    decl,
                    default_value_expr(rep, self.a)
                );
            }
            LirPat::Ctor { args, .. } | LirPat::Tuple(args) => {
                for p in args {
                    self.emit_default_pattern_bind(p, depth);
                }
            }
            LirPat::Wild | LirPat::Int(_) | LirPat::Str(_) => {}
        }
    }

    fn pattern_cond(&mut self, pat: &LirPat, value: &str, value_rep: &Rep) -> String {
        match pat {
            LirPat::Wild | LirPat::Bind { .. } => "1".into(),
            LirPat::Int(v) => {
                if !value_rep.is_integer() {
                    return "0".into();
                }
                format!("({} == {})", value, c_uint_case(*v))
            }
            LirPat::Str(s) => {
                if !matches!(value_rep, Rep::Str | Rep::Ptr) {
                    return "0".into();
                }
                format!(
                    "(strcmp({}, {}) == 0)",
                    cast_code(value, value_rep, &Rep::Str, self.a),
                    c_string(s)
                )
            }
            LirPat::Ctor { ctor, args } => {
                if *ctor == usize::MAX || *ctor >= self.a.constructors.len() {
                    return "0".into();
                }
                let c = &self.a.constructors[*ctor];
                if args.len() > c.arity {
                    return "0".into();
                }
                if !matches!(value_rep, Rep::Adt(_) | Rep::Ptr | Rep::Unknown) {
                    return "0".into();
                }
                let expected = Rep::Adt(c.type_name.clone());
                let value = cast_code(value, value_rep, &expected, self.a);
                let mut xs = vec![format!(
                    "({} != NULL && ({})->tag == {})",
                    value, value, c.tag
                )];
                for (i, p) in args.iter().enumerate() {
                    let Some(field_rep) = c
                        .fields
                        .get(i)
                        .cloned()
                        .or_else(|| c.args.get(i).map(|t| rep_from_ty(t, &self.a.type_names)))
                    else {
                        return "0".into();
                    };
                    let raw_field = format!(
                        "{}->{}f{}",
                        value,
                        constructor_field_prefix(self.a, *ctor),
                        i
                    );
                    match p {
                        LirPat::Int(v) => xs.push(format!(
                            "({} == {})",
                            cast_code(&raw_field, &field_rep, &field_rep, self.a),
                            c_uint_case(*v)
                        )),
                        LirPat::Str(s) => xs.push(format!(
                            "(strcmp({}, {}) == 0)",
                            cast_code(&raw_field, &field_rep, &Rep::Str, self.a),
                            c_string(s)
                        )),
                        LirPat::Ctor { .. } => xs.push(self.nested_ctor_cond(p, &raw_field)),
                        _ => {}
                    }
                }
                xs.join(" && ")
            }
            LirPat::Tuple(_) => "0".into(),
        }
    }

    fn nested_ctor_cond(&self, pat: &LirPat, field: &str) -> String {
        let LirPat::Ctor { ctor, args } = pat else {
            return "1".into();
        };
        if *ctor == usize::MAX || *ctor >= self.a.constructors.len() {
            return "0".into();
        }
        let c = &self.a.constructors[*ctor];
        if args.len() > c.arity {
            return "0".into();
        }
        let t = mangle_ident(&c.type_name);
        let p = format!("((struct mak_{} *)({}))", t, field);
        let mut xs = vec![format!("({} != NULL && ({})->tag == {})", p, p, c.tag)];
        for (i, a) in args.iter().enumerate() {
            let Some(field_rep) = c
                .fields
                .get(i)
                .cloned()
                .or_else(|| c.args.get(i).map(|t| rep_from_ty(t, &self.a.type_names)))
            else {
                return "0".into();
            };
            let field2 = format!("({})->{}f{}", p, constructor_field_prefix(self.a, *ctor), i);
            match a {
                LirPat::Int(v) => xs.push(format!(
                    "({} == {})",
                    cast_code(&field2, &field_rep, &field_rep, self.a),
                    c_uint_case(*v)
                )),
                LirPat::Str(s) => xs.push(format!(
                    "(strcmp({}, {}) == 0)",
                    cast_code(&field2, &field_rep, &Rep::Str, self.a),
                    c_string(s)
                )),
                LirPat::Ctor { .. } => xs.push(self.nested_ctor_cond(a, &field2)),
                _ => {}
            }
        }
        xs.join(" && ")
    }

    fn emit_runtime_call(
        &mut self,
        name: &str,
        args: &[LirExpr],
        rep: &Rep,
        depth: usize,
    ) -> String {
        let mut vals = Vec::with_capacity(args.len());
        let mut kinds = Vec::with_capacity(args.len());
        for a in args {
            let ar = lir_rep(a);
            let c = self.materialize_arg(a, depth);
            vals.push(runtime_value_bits(&c, &ar));
            kinds.push(runtime_kind_for_rep(&ar).to_string());
        }
        let call = format!(
            "mak_rt_call_function({}, (const uint64_t[]){{{}}}, (const uint8_t[]){{{}}}, {}, {})",
            c_string(name.as_bytes()),
            vals.join(","),
            kinds.join(","),
            args.len(),
            runtime_kind_for_rep(rep)
        );
        match rep {
            Rep::F32 => format!("mak_rt_f32_value({})", call),
            Rep::F64 => format!("mak_rt_f64_value({})", call),
            Rep::Str => format!("(const char *)(uintptr_t)({})", call),
            Rep::Ptr => format!("(const void *)(uintptr_t)({})", call),
            Rep::Adt(n) => format!("(struct mak_{} *)(uintptr_t)({})", mangle_ident(n), call),
            Rep::Function(id) => format!(
                "({})((uintptr_t)({}))",
                function_pointer_type(*id, &self.a.functions),
                call
            ),
            Rep::ExternFunction { ctype, .. } => {
                format!("({})((uintptr_t)({}))", ctype, call)
            },
            Rep::I8 | Rep::I16 | Rep::I32 | Rep::I64 | Rep::U8 | Rep::U16 | Rep::U32 | Rep::U64 => {
                format!("({})({})", rep.ctype(&self.a.functions), call)
            }
            Rep::Unit => format!("({})", call),
            Rep::Unknown => call,
        }
    }

    fn emit_runtime_construct(
        &mut self,
        name: &str,
        args: &[LirExpr],
        rep: &Rep,
        depth: usize,
    ) -> String {
        let mut vals = Vec::with_capacity(args.len());
        let mut kinds = Vec::with_capacity(args.len());
        for a in args {
            let ar = lir_rep(a);
            let c = self.materialize_arg(a, depth);
            vals.push(runtime_value_bits(&c, &ar));
            kinds.push(runtime_kind_for_rep(&ar).to_string());
        }


        let query = match rep {
            Rep::Adt(n) if !name.contains(':') => format!("{}:{}", name, n),
            _ => name.to_string(),
        };
        let call = format!(
            "mak_rt_call_ctor({}, (const uint64_t[]){{{}}}, (const uint8_t[]){{{}}}, {})",
            c_string(query.as_bytes()),
            vals.join(","),
            kinds.join(","),
            args.len()
        );
        match rep {
            Rep::Adt(n) => format!("(struct mak_{} *)(uintptr_t)({})", mangle_ident(n), call),
            _ => format!("(const void *)(uintptr_t)({})", call),
        }
    }
    fn emit_adt_temp_alloc(
        &mut self,
        storage: &str,
        rep: &Rep,
        depth: usize,
        kind: MemorySiteKind,
        reason: &str,
    ) -> MemoryStrategy {
        let strategy = {
            let (bytes, _) = match rep {
                Rep::Adt(n) => adt_object_layout(self.a, n),
                _ => (0, 1),
            };
            self.memory.strategy(kind, self.current_function, bytes)
        };
        let ty = adt_struct_type(rep);
        match strategy {
            MemoryStrategy::Heap => {
                let _ = writeln!(
                    self.out,
                    "{}{} *{} = ({} *)mak_ir_heap_alloc(sizeof({}), _Alignof({}));",
                    indent(depth), ty, storage, ty, ty, ty
                );
            }
            MemoryStrategy::Arena => {
                let _ = writeln!(
                    self.out,
                    "{}{} *{} = ({} *)mak_ir_arena_alloc(sizeof({}), _Alignof({}));",
                    indent(depth), ty, storage, ty, ty, ty
                );
            }
        }
        let fallback = default_value_expr(rep, self.a);
        let _ = writeln!(self.out, "{}if (!{}) {{", indent(depth), storage);
        let _ = writeln!(
            self.out,
            "{}mak_ir_recover(\"{}\");",
            indent(depth + 1), reason
        );
        let _ = writeln!(
            self.out,
            "{}{} = {};",
            indent(depth + 1), storage, fallback
        );
        let _ = writeln!(self.out, "{} }} else {{", indent(depth));
        strategy
    }

    fn emit_adt_temp_close(&mut self, _strategy: MemoryStrategy, depth: usize) {
        let _ = writeln!(self.out, "{}}}", indent(depth));
    }

    fn emit_value(&mut self, e: &LirExpr, depth: usize) -> String {
        match e {
            LirExpr::Unit => String::new(),
            LirExpr::Int(v, r) => cast_code(&c_i128(*v), &Rep::I64, r, self.a),
            LirExpr::Float(v, r) => cast_code(&c_float(*v), &Rep::F64, r, self.a),
            LirExpr::Str(s) => c_string(s),
            LirExpr::Var(code, _) => code.clone(),
            LirExpr::FnRef(id) => self.a.functions[*id].c_name.clone(),
            LirExpr::RuntimeCall { name, args, rep } => {
                self.emit_runtime_call(name, args, rep, depth)
            }
            LirExpr::RuntimeConstruct { name, args, rep } => {
                self.emit_runtime_construct(name, args, rep, depth)
            }
            LirExpr::Call {
                target,
                args,
                rep,
                noreturn,
                ..
            } => {
                if rep_is_adt(rep) {
                    let storage = self.fresh_temp("arena_call");
                    let call_kind = match target {
                        CallTarget::Function(_) => MemorySiteKind::CallResult,
                        CallTarget::External(_) | CallTarget::Indirect(_) => MemorySiteKind::DynamicCallResult,
                    };
                    let strategy = self.emit_adt_temp_alloc(
                        &storage,
                        rep,
                        depth,
                        call_kind,
                        "ADT result allocation failed",
                    );
                    let mut xs = Vec::with_capacity(args.len() + 1);
                    for (i, a) in args.iter().enumerate() {
                        let expected = self.expected_call_arg_rep(target, i);
                        xs.push(self.materialize_arg_as(a, expected.as_ref(), depth + 1));
                    }
                    xs.push(self.sret_destination_for_target(target, &storage));
                    let call = match target {
                        CallTarget::Function(id) => {
                            format!("{}({})", self.a.functions[*id].c_name, xs.join(", "))
                        }
                        CallTarget::Indirect(callee) => format!(
                            "({})({})",
                            self.materialize_if_needed(callee, depth + 1),
                            xs.join(", ")
                        ),
                        CallTarget::External(id) => {
                            format!("{}({})", self.a.externals[*id].c_name, xs.join(", "))
                        }
                    };
                    let _ = writeln!(self.out, "{}{};", indent(depth + 1), call);
                    self.emit_adt_temp_close(strategy, depth);
                    return storage;
                }
                let fname = match target {
                    CallTarget::Function(id) => self.a.functions[*id].c_name.clone(),
                    CallTarget::External(id) => self.a.externals[*id].c_name.clone(),
                    CallTarget::Indirect(callee) => {
                        let c = self.materialize_if_needed(callee, depth);
                        let mut xs = Vec::with_capacity(args.len());
                        for a in args {
                            xs.push(self.materialize_arg(a, depth));
                        }
                        let call = format!("({})({})", c, xs.join(", "));
                        if *noreturn || matches!(rep, Rep::Unit) {
                            let _ = writeln!(self.out, "{}{};", indent(depth), call);
                            return String::new();
                        }
                        return call;
                    }
                };
                let mut xs = Vec::with_capacity(args.len());
                for (i, a) in args.iter().enumerate() {
                    let expected = self.expected_call_arg_rep(target, i);
                    xs.push(self.materialize_arg_as(a, expected.as_ref(), depth));
                }
                let call = format!("{}({})", fname, xs.join(", "));
                if *noreturn || matches!(rep, Rep::Unit) {
                    let _ = writeln!(self.out, "{}{};", indent(depth), call);
                    String::new()
                } else {
                    call
                }
            }
            LirExpr::Construct { ctor, .. } => {
                let c = &self.a.constructors[*ctor];
                let construct_rep = Rep::Adt(c.type_name.clone());
                if c.arity == 0 {
                    return format!("&mak_ir_ctor_obj_{}", ctor);
                }
                let storage = self.fresh_temp("arena_obj");
                let strategy = self.emit_adt_temp_alloc(
                    &storage,
                    &construct_rep,
                    depth,
                    MemorySiteKind::Construct,
                    "ADT object allocation failed",
                );
                self.emit_into(e, &storage, &construct_rep, depth + 1);
                self.emit_adt_temp_close(strategy, depth);
                storage
            }
            LirExpr::Field {
                base, ctor, index, ..
            } => {
                let b = self.materialize_if_needed(base, depth);
                let c = &self.a.constructors[*ctor];
                let expected = Rep::Adt(c.type_name.clone());
                let b = cast_code(&b, &lir_rep(base), &expected, self.a);
                format!(
                    "{}->{}f{}",
                    b,
                    constructor_field_prefix(self.a, *ctor),
                    index
                )
            }
            LirExpr::Let {
                code,
                rep,
                value,
                body,
            } => {
                self.emit_local_let(code, rep, value, depth);
                self.emit_value(body, depth)
            }
            LirExpr::Block { stmts, tail } => {
                for s in stmts {
                    self.emit_expr_stmt(s, depth);
                }
                self.emit_value(tail, depth)
            }
            LirExpr::Case { .. } => {
                let r = lir_rep(e);
                if matches!(r, Rep::Unit) {
                    if let LirExpr::Case { scrut, arms, .. } = e {
                        self.emit_tail_case(scrut, arms, depth);
                    }
                    String::new()
                } else if let Rep::Adt(_) = &r {
                    let storage = self.fresh_temp("arena_case");
                    let strategy = self.emit_adt_temp_alloc(
                        &storage,
                        &r,
                        depth,
                        MemorySiteKind::CaseResult,
                        "ADT case-result allocation failed",
                    );
                    self.emit_case_into(e, &storage, &r, depth + 1);
                    self.emit_adt_temp_close(strategy, depth);
                    storage
                } else {
                    let tmp = self.fresh_temp("case");
                    let _ = writeln!(
                        self.out,
                        "{}{} {} = {};",
                        indent(depth),
                        r.ctype(&self.a.functions),
                        tmp,
                        zero_for(&r)
                    );
                    self.emit_case_into(e, &tmp, &r, depth);
                    tmp
                }
            }
            LirExpr::Bin {
                op,
                left,
                right,
                rep,
            } => self.emit_bin(*op, left, right, rep, depth),
            LirExpr::Un { op, expr, rep } => {
                let x = self.materialize_if_needed(expr, depth);
                match op {
                    UnOpLir::Neg if matches!(&rep, Rep::I64) => {
                        format!("mak_ir_i64_neg_wrap((int64_t)({}))", x)
                    }
                    UnOpLir::Neg => format!("(-{})", x),
                    UnOpLir::Not => format!("(!({}))", x),
                    UnOpLir::BitNot => format!("(~({}))", x),
                }
            }
            LirExpr::RawAst(..) => {
                self.a.dbg(
                    "RUNTIME",
                    "RawAst reached CEmitter; emitting runtime recovery/default instead of panicking",
                );
                match &self.current_ret {
                    Rep::Unit => "mak_ir_fail_u64(\"RawAst reached CEmitter\")".into(),
                    Rep::Adt(n) => format!(
                        "(struct mak_{} *)(uintptr_t)mak_ir_fail_u64(\"RawAst reached CEmitter\")",
                        mangle_ident(n)
                    ),
                    r => format!(
                        "(({})mak_ir_fail_u64(\"RawAst reached CEmitter\"))",
                        r.ctype(&*self.a.functions)
                    ),
                }
            }
            LirExpr::DirectC { code, .. } => code.clone(),
            LirExpr::Fallback => {
                self.a.dbg(
                    "RUNTIME",
                    "Fallback reached CEmitter; emitting runtime recovery/default instead of panicking",
                );
                match &self.current_ret {
                    Rep::Unit => "mak_ir_fail_u64(\"LIR fallback\")".into(),
                    Rep::Adt(n) => format!(
                        "(struct mak_{} *)(uintptr_t)mak_ir_fail_u64(\"LIR fallback\")",
                        mangle_ident(n)
                    ),
                    r => format!(
                        "(({})mak_ir_fail_u64(\"LIR fallback\"))",
                        r.ctype(&self.a.functions)
                    ),
                }
            }
        }
    }

    fn is_sret_self_tail_expr(&self, e: &LirExpr) -> bool {
        match e {
            LirExpr::Call {
                target: CallTarget::Function(fid),
                rep,
                noreturn,
                args,
                ..
            } => {
                self.current_out.is_some()
                    && self.current_function == Some(*fid)
                    && rep_is_adt(rep)
                    && !*noreturn
                    && *fid < self.a.functions.len()
                    && self.a.functions[*fid].captures.is_empty()
                    && args.len() == self.a.functions[*fid].params.len()
            }
            LirExpr::Let { body, .. } => self.is_sret_self_tail_expr(body),
            LirExpr::Block { tail, .. } => self.is_sret_self_tail_expr(tail),
            _ => false,
        }
    }

    fn emit_sret_self_tail_call(
        &mut self,
        fid: usize,
        args: &[LirExpr],
        dest: &str,
        depth: usize,
    ) -> bool {
        if self.current_out.as_deref() != Some(dest)
            || self.current_function != Some(fid)
            || fid >= self.a.functions.len()
        {
            return false;
        }
        let (param_reps, arity, captures_empty) = {
            let f = &self.a.functions[fid];
            (f.param_reps.clone(), f.params.len(), f.captures.is_empty())
        };
        if !captures_empty || args.len() != arity || param_reps.len() != arity {
            return false;
        }
        let mut temps = Vec::with_capacity(args.len());
        for (i, arg) in args.iter().enumerate() {
            let rep = &param_reps[i];
            let tmp = self.fresh_temp("tailarg");
            let c = self.materialize_arg_as(arg, Some(rep), depth);
            let decl = rep_decl(rep, &tmp, &self.a.functions);
            let _ = writeln!(
                self.out,
                "{}{} = {};",
                indent(depth),
                decl,
                c
            );
            temps.push(tmp);
        }
        for (i, tmp) in temps.iter().enumerate() {
            let rep = &param_reps[i];
            let _ = writeln!(
                self.out,
                "{}a{} = {};",
                indent(depth),
                i,
                cast_code(tmp, rep, rep, self.a)
            );
        }
        let _ = writeln!(self.out, "{}continue;", indent(depth));
        true
    }

    fn emit_default_into(&mut self, dest: &str, rep: &Rep, depth: usize) {
        match rep {
            Rep::Unit => {}
            Rep::Adt(_) => {
                let fallback = default_value_expr(rep, self.a);
                let _ = writeln!(self.out, "{}*({}) = *({});", indent(depth), dest, fallback);
            }
            _ => {
                let _ = writeln!(
                    self.out,
                    "{}*({}) = {};",
                    indent(depth),
                    dest,
                    zero_for(rep)
                );
            }
        }
    }

    fn emit_default_value(&mut self, dest: &str, rep: &Rep, depth: usize) {
        match rep {
            Rep::Unit => {}
            Rep::Adt(_) => {
                let fallback = default_value_expr(rep, self.a);
                let _ = writeln!(self.out, "{}{} = *({});", indent(depth), dest, fallback);
            }
            _ => {
                let _ = writeln!(self.out, "{}{} = {};", indent(depth), dest, zero_for(rep));
            }
        }
    }

    fn emit_runtime_type_recovery_into(
        &mut self,
        dest: &str,
        rep: &Rep,
        depth: usize,
        reason: &str,
    ) {
        let _ = writeln!(
            self.out,
            "{}mak_ir_recover(\"{}\");",
            indent(depth),
            c_escape(reason)
        );
        self.emit_default_into(dest, rep, depth);
    }

    fn emit_default_return(&mut self, depth: usize, reason: &str) {
        let ret = self.current_ret.clone();
        let _ = writeln!(
            self.out,
            "{}mak_ir_recover(\"{}\");",
            indent(depth),
            c_escape(reason)
        );
        match ret {
            Rep::Unit => {
                let _ = writeln!(self.out, "{}return;", indent(depth));
            }
            Rep::Adt(_) => {
                let fallback = default_value_expr(&ret, self.a);
                let _ = writeln!(self.out, "{}*out = *({});", indent(depth), fallback);
                let _ = writeln!(self.out, "{}return;", indent(depth));
            }
            _ => {
                let _ = writeln!(self.out, "{}return {};", indent(depth), zero_for(&ret));
            }
        }
    }

    fn emit_into(&mut self, e: &LirExpr, dest: &str, dest_rep: &Rep, depth: usize) {
        match e {
            LirExpr::Construct { ctor, args, .. } => {
                let c = &self.a.constructors[*ctor];
                let construct_rep = Rep::Adt(c.type_name.clone());
                if construct_rep != *dest_rep {
                    let value = self.emit_value(e, depth);
                    match (construct_rep, dest_rep) {
                        (Rep::Adt(_), Rep::Adt(dst_name)) if !value.is_empty() => {
                            let dst_type = adt_type_id(dst_name);
                            let _ = writeln!(
                                self.out,
                                "{}if (!mak_ir_adt_copy_checked((void *)({}), sizeof(struct mak_{}), UINT32_C({}), (const void *)(uintptr_t)({}))) {{",
                                indent(depth),
                                dest,
                                mangle_ident(dst_name),
                                dst_type,
                                value
                            );
                            self.emit_default_into(dest, dest_rep, depth + 1);
                            let _ = writeln!(self.out, "{}}}", indent(depth));
                        }
                        _ => {
                            self.emit_runtime_type_recovery_into(dest, dest_rep, depth, "ADT destination/source representation mismatch");
                        }
                    }
                    return;
                }
                if c.arity == 0 {
                    let _ = writeln!(
                        self.out,
                        "{}*({}) = mak_ir_ctor_obj_{};",
                        indent(depth),
                        dest,
                        ctor
                    );
                    return;
                }
                let _ = writeln!(self.out, "{}({})->tag = {};", indent(depth), dest, c.tag);
                for (i, a) in args.iter().enumerate() {
                    let fr = c
                        .fields
                        .get(i)
                        .cloned()
                        .or_else(|| c.args.get(i).map(|t| rep_from_ty(t, &self.a.type_names)))
                        .unwrap_or(Rep::Unknown);
                    let ac = self.materialize_arg_as(a, Some(&fr), depth);
                    let _ = writeln!(
                        self.out,
                        "{}({})->{}f{} = {};",
                        indent(depth),
                        dest,
                        constructor_field_prefix(self.a, *ctor),
                        i,
                        cast_code(&ac, &fr, &fr, self.a)
                    );
                }
            }
            LirExpr::Call {
                target,
                args,
                rep,
                noreturn,
                ..
            } if rep_is_adt(rep) => {


                if *rep != *dest_rep {
                    let c = self.emit_value(e, depth);
                    match (rep, dest_rep) {
                        (Rep::Adt(_), Rep::Adt(dst_name)) if !c.is_empty() => {
                            let dst_type = adt_type_id(dst_name);
                            let _ = writeln!(
                                self.out,
                                "{}if (!mak_ir_adt_copy_checked((void *)({}), sizeof(struct mak_{}), UINT32_C({}), (const void *)(uintptr_t)({}))) {{",
                                indent(depth),
                                dest,
                                mangle_ident(dst_name),
                                dst_type,
                                c
                            );
                            self.emit_default_into(dest, dest_rep, depth + 1);
                            let _ = writeln!(self.out, "{}}}", indent(depth));
                        }
                        _ => {
                            self.emit_runtime_type_recovery_into(dest, dest_rep, depth, "ADT call result representation mismatch");
                        }
                    }
                    return;
                }
                if let CallTarget::Function(fid) = target {
                    if !*noreturn && self.emit_sret_self_tail_call(*fid, args, dest, depth) {
                        return;
                    }
                }
                let mut xs = Vec::with_capacity(args.len() + 1);
                for (i, a) in args.iter().enumerate() {
                    let expected = self.expected_call_arg_rep(target, i);
                    xs.push(self.materialize_arg_as(a, expected.as_ref(), depth));
                }
                xs.push(self.sret_destination_for_target(target, dest));
                let call = match target {
                    CallTarget::Function(id) => {
                        format!("{}({})", self.a.functions[*id].c_name, xs.join(", "))
                    }
                    CallTarget::External(id) => {
                        format!("{}({})", self.a.externals[*id].c_name, xs.join(", "))
                    }
                    CallTarget::Indirect(callee) => format!(
                        "({})({})",
                        self.materialize_if_needed(callee, depth),
                        xs.join(", ")
                    ),
                };
                let _ = writeln!(self.out, "{}{};", indent(depth), call);
                if *noreturn {
                    let _ = writeln!(self.out, "{}0;", indent(depth));
                }
            }
            LirExpr::Var(_, rep) if rep_is_adt(rep) => {
                let src = self.emit_value(e, depth);
                let _ = writeln!(self.out, "{}*({}) = *({});", indent(depth), dest, src);
            }
            LirExpr::Field {
                base,
                rep,
                ctor,
                index,
            } if rep_is_adt(rep) => {
                let src = self.emit_value(base, depth);
                let c = &self.a.constructors[*ctor];
                let expected = Rep::Adt(c.type_name.clone());
                let src = cast_code(&src, &lir_rep(base), &expected, self.a);
                let field = format!(
                    "({})->{}f{}",
                    src,
                    constructor_field_prefix(self.a, *ctor),
                    index
                );
                let _ = writeln!(self.out, "{}*({}) = *({});", indent(depth), dest, field);
            }
            LirExpr::Let {
                code,
                rep,
                value,
                body,
            } => {
                self.emit_local_let(code, rep, value, depth);
                self.emit_into(body, dest, dest_rep, depth);
            }
            LirExpr::Block { stmts, tail } => {
                for s in stmts {
                    self.emit_expr_stmt(s, depth);
                }
                self.emit_into(tail, dest, dest_rep, depth);
            }
            LirExpr::Case { .. } => {
                self.emit_case_into(e, dest, dest_rep, depth);
            }
            LirExpr::RuntimeCall { name, args, rep } => {
                let effective =
                    if matches!(rep, Rep::Unknown) && self.current_out.as_deref() == Some(dest) {
                        self.current_ret.clone()
                    } else {
                        rep.clone()
                    };
                let c = self.emit_runtime_call(name, args, &effective, depth);
                if let Rep::Adt(n) = effective {
                    let rt_tmp = self.fresh_temp("rt_heap_obj");
                    let _ = writeln!(
                        self.out,
                        "{}struct mak_{} *{} = (struct mak_{} *)(uintptr_t)({});",
                        indent(depth),
                        mangle_ident(&n),
                        rt_tmp,
                        mangle_ident(&n),
                        c
                    );
                    if let Rep::Adt(dst_name) = dest_rep {
                        let dst_type = adt_type_id(dst_name);
                        let _ = writeln!(
                            self.out,
                            "{}if (!mak_ir_adt_copy_checked((void *)({}), sizeof(struct mak_{}), UINT32_C({}), (const void *)({})) ) {{",
                            indent(depth),
                            dest,
                            mangle_ident(dst_name),
                            dst_type,
                            rt_tmp
                        );
                        self.emit_default_into(dest, dest_rep, depth + 1);
                        let _ = writeln!(self.out, "{}}}", indent(depth));
                    } else {
                        self.emit_runtime_type_recovery_into(dest, dest_rep, depth, "runtime ADT result used in non-ADT destination");
                    }
                    let _ = writeln!(self.out, "{}mak_ir_heap_free((void *){});", indent(depth), rt_tmp);
                } else if matches!(effective, Rep::Unit) {
                    if !c.is_empty() {
                        let _ = writeln!(self.out, "{}{};", indent(depth), c);
                    }
                    if rep_is_adt(dest_rep) {
                        self.emit_runtime_type_recovery_into(
                            dest,
                            dest_rep,
                            depth,
                            "unit runtime call used as ADT result",
                        );
                    }
                } else if rep_is_adt(dest_rep) {
                    if !c.is_empty() {
                        let _ = writeln!(self.out, "{}(void)({});", indent(depth), c);
                    }
                    self.emit_runtime_type_recovery_into(
                        dest,
                        dest_rep,
                        depth,
                        "non-ADT runtime call used as ADT result",
                    );
                } else {
                    let _ = writeln!(self.out, "{}*({}) = {};", indent(depth), dest, c);
                }
            }
            LirExpr::RuntimeConstruct { name, args, .. } => {
                let target_rep = if let Rep::Adt(n) = lir_rep(e) {
                    Rep::Adt(n)
                } else if self.current_out.as_deref() == Some(dest) {
                    self.current_ret.clone()
                } else {
                    Rep::Ptr
                };
                let c = self.emit_runtime_construct(name, args, &target_rep, depth);
                let target = if self.current_out.as_deref() == Some(dest) {
                    self.current_ret.clone()
                } else {
                    lir_rep(e)
                };
                if let Rep::Adt(n) = target {
                    let rt_tmp = self.fresh_temp("rt_heap_obj");
                    let _ = writeln!(
                        self.out,
                        "{}struct mak_{} *{} = (struct mak_{} *)(uintptr_t)({});",
                        indent(depth),
                        mangle_ident(&n),
                        rt_tmp,
                        mangle_ident(&n),
                        c
                    );
                    if let Rep::Adt(dst_name) = dest_rep {
                        let dst_type = adt_type_id(dst_name);
                        let _ = writeln!(
                            self.out,
                            "{}if (!mak_ir_adt_copy_checked((void *)({}), sizeof(struct mak_{}), UINT32_C({}), (const void *)({})) ) {{",
                            indent(depth),
                            dest,
                            mangle_ident(dst_name),
                            dst_type,
                            rt_tmp
                        );
                        self.emit_default_into(dest, dest_rep, depth + 1);
                        let _ = writeln!(self.out, "{}}}", indent(depth));
                    } else {
                        self.emit_runtime_type_recovery_into(dest, dest_rep, depth, "runtime constructor result used in non-ADT destination");
                    }
                    // Nullary constructor wrappers return addresses of static
                    // singleton objects; only payload constructors allocate here.
                    if !args.is_empty() {
                        let _ = writeln!(self.out, "{}mak_ir_heap_free((void *){});", indent(depth), rt_tmp);
                    }
                } else if rep_is_adt(dest_rep) {
                    if !c.is_empty() {
                        let _ = writeln!(self.out, "{}(void)({});", indent(depth), c);
                    }
                    self.emit_runtime_type_recovery_into(
                        dest,
                        dest_rep,
                        depth,
                        "non-ADT runtime constructor used as ADT result",
                    );
                } else {
                    let _ = writeln!(self.out, "{}*({}) = {};", indent(depth), dest, c);
                }
            }
            LirExpr::DirectC { code, rep } => {
                match (rep, dest_rep) {
                    (Rep::Adt(_), Rep::Adt(dst_name)) => {
                        let dst_type = adt_type_id(dst_name);
                        let _ = writeln!(
                            self.out,
                            "{}if (!mak_ir_adt_copy_checked((void *)({}), sizeof(struct mak_{}), UINT32_C({}), (const void *)(uintptr_t)({}))) {{",
                            indent(depth),
                            dest,
                            mangle_ident(dst_name),
                            dst_type,
                            code
                        );
                        self.emit_default_into(dest, dest_rep, depth + 1);
                        let _ = writeln!(self.out, "{}}}", indent(depth));
                    }
                    (Rep::Unit, Rep::Adt(_)) => {
                        if !code.is_empty() {
                            let _ = writeln!(self.out, "{}{};", indent(depth), code);
                        }
                        self.emit_runtime_type_recovery_into(
                            dest,
                            dest_rep,
                            depth,
                            "unit expression used as ADT result",
                        );
                    }
                    (Rep::Unit, _) => {
                        if !code.is_empty() {
                            let _ = writeln!(self.out, "{}{};", indent(depth), code);
                        }
                    }
                    (_, Rep::Adt(_)) => {
                        if !code.is_empty() {
                            let _ = writeln!(self.out, "{}(void)({});", indent(depth), code);
                        }
                        self.emit_runtime_type_recovery_into(
                            dest,
                            dest_rep,
                            depth,
                            "non-ADT expression used as ADT result",
                        );
                    }
                    _ => {
                        let _ = writeln!(
                            self.out,
                            "{}*({}) = {};",
                            indent(depth),
                            dest,
                            cast_code(code, rep, dest_rep, self.a)
                        );
                    }
                }
            }
            LirExpr::RawAst(..) => {
                self.emit_runtime_type_recovery_into(
                    dest,
                    dest_rep,
                    depth,
                    "RawAst reached CEmitter",
                );
            }
            LirExpr::Fallback => {
                self.emit_runtime_type_recovery_into(
                    dest,
                    dest_rep,
                    depth,
                    "LIR fallback reached CEmitter",
                );
            }
            _ => {
                let r = lir_rep(e);
                let c = self.emit_value(e, depth);
                if rep_is_adt(dest_rep) && !rep_is_adt(&r) {


                    if !c.is_empty() {
                        let _ = writeln!(self.out, "{}(void)({});", indent(depth), c);
                    }
                    self.emit_runtime_type_recovery_into(
                        dest,
                        dest_rep,
                        depth,
                        "non-ADT expression used as ADT result",
                    );
                } else if !c.is_empty() {
                    let _ = writeln!(
                        self.out,
                        "{}*({}) = {};",
                        indent(depth),
                        dest,
                        cast_code(&c, &r, dest_rep, self.a)
                    );
                }
            }
        }
    }

    fn emit_bin(
        &mut self,
        op: BinOpLir,
        left: &LirExpr,
        right: &LirExpr,
        rep: &Rep,
        depth: usize,
    ) -> String {
        if matches!(op, BinOpLir::LAnd | BinOpLir::LOr) {
            if lir_is_simple_value(left) && lir_is_simple_value(right) {
                let l = self.inline_code(left);
                let r = self.inline_code(right);
                return match op {
                    BinOpLir::LAnd => format!("(({}) && ({}))", l, r),
                    BinOpLir::LOr => format!("(({}) || ({}))", l, r),
                    _ => "0".into(),
                };
            }
            let l = self.materialize_if_needed(left, depth);
            let tmp = self.fresh_temp("sc");
            let _ = writeln!(self.out, "{}int32_t {} = 0;", indent(depth), tmp);
            match op {
                BinOpLir::LAnd => {
                    let _ = writeln!(self.out, "{}if ({}) {{", indent(depth), l);
                    let r = self.materialize_if_needed(right, depth + 1);
                    let _ = writeln!(self.out, "{}{} = !!({});", indent(depth + 1), tmp, r);
                    let _ = writeln!(self.out, "{}}}", indent(depth));
                }
                BinOpLir::LOr => {
                    let _ = writeln!(self.out, "{}if (!({})) {{", indent(depth), l);
                    let r = self.materialize_if_needed(right, depth + 1);
                    let _ = writeln!(self.out, "{}{} = !!({});", indent(depth + 1), tmp, r);
                    let _ = writeln!(self.out, "{}}} else {{", indent(depth));
                    let _ = writeln!(self.out, "{}{} = 1;", indent(depth + 1), tmp);
                    let _ = writeln!(self.out, "{}}}", indent(depth));
                }
                _ => {}
            }
            return tmp;
        }
        let lr = lir_rep(left);
        let rr = lir_rep(right);
        if matches!(op, BinOpLir::Eq | BinOpLir::Ne) && !rep_equality_compatible(&lr, &rr) {
            return "mak_ir_fail_u64(\"incompatible equality\")".into();
        }
        let l = self.materialize_if_needed(left, depth);
        let r = self.materialize_if_needed(right, depth);
        let ls = if rep.is_float() {
            format!("(double)({})", cast_code(&l, &lr, rep, self.a))
        } else {
            cast_code(&l, &lr, rep, self.a)
        };
        let rs = if rep.is_float() {
            format!("(double)({})", cast_code(&r, &rr, rep, self.a))
        } else {
            cast_code(&r, &rr, rep, self.a)
        };
        if matches!(op, BinOpLir::Eq | BinOpLir::Ne)
            && matches!(lr, Rep::Str)
            && matches!(rr, Rep::Str)
        {
            let cmp = format!("strcmp({}, {})", l, r);
            return if matches!(op, BinOpLir::Eq) {
                format!("({} == 0)", cmp)
            } else {
                format!("({} != 0)", cmp)
            };
        }
        if matches!(rep, Rep::I64) {
            let helper = match op {
                BinOpLir::Add => Some("mak_ir_i64_add_wrap"),
                BinOpLir::Sub => Some("mak_ir_i64_sub_wrap"),
                BinOpLir::Mul => Some("mak_ir_i64_mul_wrap"),
                BinOpLir::Div => Some("mak_ir_i64_div_wrap"),
                BinOpLir::Rem => Some("mak_ir_i64_rem_wrap"),
                BinOpLir::Shl => Some("mak_ir_i64_shl_wrap"),
                BinOpLir::Shr => Some("mak_ir_i64_shr"),
                _ => None,
            };
            if let Some(helper) = helper {
                return format!("{}({}, {})", helper, ls, rs);
            }
        }
        let opstr = match op {
            BinOpLir::Add => "+",
            BinOpLir::Sub => "-",
            BinOpLir::Mul => "*",
            BinOpLir::Div => "/",
            BinOpLir::Rem => "%",
            BinOpLir::And => "&",
            BinOpLir::Or => "|",
            BinOpLir::Xor => "^",
            BinOpLir::Shl => "<<",
            BinOpLir::Shr => ">>",
            BinOpLir::Eq => "==",
            BinOpLir::Ne => "!=",
            BinOpLir::Lt => "<",
            BinOpLir::Gt => ">",
            BinOpLir::Le => "<=",
            BinOpLir::Ge => ">=",
            BinOpLir::LAnd | BinOpLir::LOr => return "0".into(),
        };
        if matches!(
            op,
            BinOpLir::Eq | BinOpLir::Ne | BinOpLir::Lt | BinOpLir::Gt | BinOpLir::Le | BinOpLir::Ge
        ) {
            format!("{} {} {}", ls, opstr, rs)
        } else {
            format!("({} {} {})", ls, opstr, rs)
        }
    }

    fn sret_destination_for_target(&self, target: &CallTarget, dest: &str) -> String {
        match target {
            CallTarget::Function(fid) => {
                if let Some(Rep::Adt(name)) = self.a.functions.get(*fid).map(|f| &f.ret_rep) {
                    return format!("(struct mak_{} *)({})", mangle_ident(name), dest);
                }
            }
            CallTarget::External(eid) => {
                if let Some(ExternalInfo {
                    decl:
                        Extern {
                            mak_ty: Ty::Named(name),
                            ..
                        },
                    ..
                }) = self.a.externals.get(*eid)
                {
                    return format!("(struct mak_{} *)({})", mangle_ident(name), dest);
                }
            }
            CallTarget::Indirect(_) => {}
        }
        dest.into()
    }

    fn expected_call_arg_rep(&self, target: &CallTarget, index: usize) -> Option<Rep> {
        match target {
            CallTarget::Function(fid) => self
                .a
                .functions
                .get(*fid)
                .and_then(|f| f.param_reps.get(index).cloned()),
            CallTarget::External(eid) => {
                match self.a.externals.get(*eid).map(|x| &x.decl.foreign) {
                    Some(ForeignTy::Fun { args, .. }) => match args.get(index) {
                        Some(ForeignArg::Ty(t)) => Some(rep_from_foreign(t)),
                        Some(ForeignArg::VarArgs) | None => None,
                    },
                    Some(ForeignTy::Atom(_)) | None => None,
                }
            }
            CallTarget::Indirect(_) => None,
        }
    }

    fn materialize_arg_as(&mut self, e: &LirExpr, expected: Option<&Rep>, depth: usize) -> String {
        let actual = lir_rep(e);
        if let Some(target) = expected {
            if lir_is_simple_value(e) {
                return cast_code(&self.inline_code(e), &actual, target, self.a);
            }
            let c = self.emit_value(e, depth);
            if actual == *target {
                return c;
            }
            let tmp = self.fresh_temp("arg");
            let decl = rep_decl(target, &tmp, &self.a.functions);
            let _ = writeln!(
                self.out,
                "{}{} = {};",
                indent(depth),
                decl,
                cast_code(&c, &actual, target, self.a)
            );
            return tmp;
        }
        self.materialize_arg(e, depth)
    }

    fn materialize_arg(&mut self, e: &LirExpr, depth: usize) -> String {
        if lir_is_simple_value(e) {
            return self.inline_code(e);
        }
        let rep = lir_rep(e);
        let tmp = self.fresh_temp("arg");
        let c = self.emit_value(e, depth);
        let decl = rep_decl(&rep, &tmp, &self.a.functions);
        let _ = writeln!(
            self.out,
            "{}{} = {};",
            indent(depth),
            decl,
            cast_code(&c, &lir_rep(e), &rep, self.a)
        );
        tmp
    }

    fn materialize_if_needed(&mut self, e: &LirExpr, depth: usize) -> String {
        if lir_is_simple_value(e) {
            return self.inline_code(e);
        }
        let rep = lir_rep(e);
        let tmp = self.fresh_temp("eval");
        let c = self.emit_value(e, depth);
        let decl = rep_decl(&rep, &tmp, &self.a.functions);
        let _ = writeln!(
            self.out,
            "{}{} = {};",
            indent(depth),
            decl,
            cast_code(&c, &lir_rep(e), &rep, self.a)
        );
        tmp
    }

    fn inline_code(&self, e: &LirExpr) -> String {
        match e {
            LirExpr::Var(c, _) => c.clone(),
            LirExpr::Int(v, _) => c_i128(*v),
            LirExpr::Float(v, _) => c_float(*v),
            LirExpr::Str(s) => c_string(s),
            LirExpr::FnRef(id) => self.a.functions[*id].c_name.clone(),
            _ => default_value_expr(&lir_rep(e), self.a),
        }
    }

    fn fresh_temp(&mut self, prefix: &str) -> String {
        let n = self.temp;
        self.temp += 1;
        format!("{}_{}", prefix, n)
    }

    fn function_decl(&self, f: &LirFunction) -> String {
        let aggregate = rep_is_adt(&f.info.ret_rep);
        let ret_ty = if aggregate {
            "void".into()
        } else {
            f.info.ret_rep.ctype(&self.a.functions)
        };
        let mut s = String::new();
        let _ = write!(s, "inline {} {}(", ret_ty, f.info.c_name);
        let mut first = true;
        for (i, r) in f.info.capture_reps.iter().enumerate() {
            if !first {
                s.push_str(", ");
            }
            first = false;
            let decl = rep_decl(r, &format!("c{}", i), &self.a.functions);
            let _ = write!(s, "{}", decl);
        }
        for (i, r) in f.info.param_reps.iter().enumerate() {
            if !first {
                s.push_str(", ");
            }
            first = false;
            let decl = rep_decl(r, &format!("a{}", i), &self.a.functions);
            let _ = write!(s, "{}", decl);
        }
        if aggregate {
            if !first {
                s.push_str(", ");
            }
            let adt_name = match &f.info.ret_rep {
                Rep::Adt(n) => mangle_ident(n),
                _ => "_a".into(),
            };
            let _ = write!(s, "struct mak_{} *out", adt_name);
            first = false;
        }
        if first {
            s.push_str("void");
        }
        s.push(')');
        s
    }
}

fn lir_contains_fallback(e: &LirExpr) -> bool {
    match e {
        LirExpr::Fallback | LirExpr::RawAst(..) => true,
        LirExpr::DirectC { .. } => false,
        LirExpr::Call { target, args, .. } => {
            matches!(target, CallTarget::Indirect(x) if lir_contains_fallback(x))
                || args.iter().any(lir_contains_fallback)
        }
        LirExpr::Construct { args, .. } => args.iter().any(lir_contains_fallback),
        LirExpr::Field { base, .. } => lir_contains_fallback(base),
        LirExpr::Let { value, body, .. } => {
            lir_contains_fallback(value) || lir_contains_fallback(body)
        }
        LirExpr::Block { stmts, tail } => {
            stmts.iter().any(lir_contains_fallback) || lir_contains_fallback(tail)
        }
        LirExpr::Case { scrut, arms, .. } => {
            lir_contains_fallback(scrut) || arms.iter().any(|a| lir_contains_fallback(&a.body))
        }
        LirExpr::Bin { left, right, .. } => {
            lir_contains_fallback(left) || lir_contains_fallback(right)
        }
        LirExpr::Un { expr, .. } => lir_contains_fallback(expr),
        LirExpr::RuntimeCall { args, .. } | LirExpr::RuntimeConstruct { args, .. } => {
            args.iter().any(lir_contains_fallback)
        }
        _ => false,
    }
}
fn retag_lir_result_rep(mut e: LirExpr, rep: Rep) -> LirExpr {
    match &mut e {
        LirExpr::Int(_, r) | LirExpr::Float(_, r) | LirExpr::Var(_, r) => *r = rep,
        LirExpr::Call { rep: r, .. }
        | LirExpr::Construct { rep: r, .. }
        | LirExpr::Field { rep: r, .. }
        | LirExpr::Case { rep: r, .. }
        | LirExpr::Bin { rep: r, .. }
        | LirExpr::Un { rep: r, .. } => *r = rep,
        LirExpr::Let { body, .. } => {
            let next = retag_lir_result_rep((**body).clone(), rep);
            *body = Box::new(next);
        }
        LirExpr::Block { tail, .. } => {
            let next = retag_lir_result_rep((**tail).clone(), rep);
            *tail = Box::new(next);
        }
        LirExpr::RawAst(_, r, _) => *r = rep,
        LirExpr::RuntimeCall { rep: r, .. } | LirExpr::RuntimeConstruct { rep: r, .. } => *r = rep,
        LirExpr::DirectC { rep: r, .. } => *r = rep,
        LirExpr::Unit | LirExpr::Str(_) | LirExpr::FnRef(_) | LirExpr::Fallback => {}
    }
    e
}
fn lir_rep(e: &LirExpr) -> Rep {
    match e {
        LirExpr::Unit => Rep::Unit,
        LirExpr::Int(_, r) | LirExpr::Float(_, r) | LirExpr::Var(_, r) => r.clone(),
        LirExpr::Str(_) => Rep::Str,
        LirExpr::FnRef(id) => Rep::Function(*id),
        LirExpr::Call { rep, .. }
        | LirExpr::Construct { rep, .. }
        | LirExpr::Field { rep, .. }
        | LirExpr::Case { rep, .. }
        | LirExpr::Bin { rep, .. }
        | LirExpr::Un { rep, .. } => rep.clone(),
        LirExpr::Let { body, .. } => lir_rep(body),
        LirExpr::Block { tail, .. } => lir_rep(tail),
        LirExpr::RawAst(_, r, _) => r.clone(),
        LirExpr::RuntimeCall { rep, .. } | LirExpr::RuntimeConstruct { rep, .. } => rep.clone(),
        LirExpr::DirectC { rep, .. } => rep.clone(),
        LirExpr::Fallback => Rep::Unknown,
    }
}

fn lir_is_pure(e: &LirExpr) -> bool {
    match e {
        LirExpr::Unit
        | LirExpr::Int(_, _)
        | LirExpr::Float(_, _)
        | LirExpr::Str(_)
        | LirExpr::Var(_, _)
        | LirExpr::FnRef(_) => true,
        LirExpr::Call { pure, args, .. } => *pure && args.iter().all(lir_is_pure),
        LirExpr::Construct { .. } => false,
        LirExpr::Field { base, .. } => lir_is_pure(base),
        LirExpr::Let { value, body, .. } => lir_is_pure(value) && lir_is_pure(body),
        LirExpr::Block { stmts, tail } => stmts.iter().all(lir_is_pure) && lir_is_pure(tail),
        LirExpr::Case { scrut, arms, .. } => {
            lir_is_pure(scrut) && arms.iter().all(|a| lir_is_pure(&a.body))
        }
        LirExpr::Bin { left, right, .. } => lir_is_pure(left) && lir_is_pure(right),
        LirExpr::Un { expr, .. } => lir_is_pure(expr),
        LirExpr::RuntimeCall { args, .. } | LirExpr::RuntimeConstruct { args, .. } => {
            args.iter().all(lir_is_pure)
        }
        LirExpr::RawAst(..) => false,
        LirExpr::DirectC { .. } => false,
        LirExpr::Fallback => false,
    }
}

fn lir_is_inlineable(e: &LirExpr) -> bool {
    matches!(
        e,
        LirExpr::Unit
            | LirExpr::Int(_, _)
            | LirExpr::Float(_, _)
            | LirExpr::Str(_)
            | LirExpr::Var(_, _)
            | LirExpr::FnRef(_)
    )
}

fn lir_count_var(e: &LirExpr, code: &str) -> usize {
    match e {
        LirExpr::Var(n, _) => usize::from(n == code),
        LirExpr::Let {
            code: n,
            value,
            body,
            ..
        } => {
            lir_count_var(value, code)
                + if n == code {
                    0
                } else {
                    lir_count_var(body, code)
                }
        }
        LirExpr::Call { args, .. } | LirExpr::Construct { args, .. } => {
            args.iter().map(|x| lir_count_var(x, code)).sum()
        }
        LirExpr::Field { base, .. } => lir_count_var(base, code),
        LirExpr::Block { stmts, tail } => {
            stmts.iter().map(|x| lir_count_var(x, code)).sum::<usize>() + lir_count_var(tail, code)
        }
        LirExpr::Case { scrut, arms, .. } => {
            lir_count_var(scrut, code)
                + arms
                    .iter()
                    .map(|a| {
                        if lir_pat_binds(&a.pat, code) {
                            0
                        } else {
                            lir_count_var(&a.body, code)
                        }
                    })
                    .sum::<usize>()
        }
        LirExpr::Bin { left, right, .. } => lir_count_var(left, code) + lir_count_var(right, code),
        LirExpr::Un { expr, .. } => lir_count_var(expr, code),
        LirExpr::RuntimeCall { args, .. } | LirExpr::RuntimeConstruct { args, .. } => {
            args.iter().map(|x| lir_count_var(x, code)).sum()
        }
        LirExpr::RawAst(..) => 0,
        _ => 0,
    }
}

fn lir_pat_binds(p: &LirPat, code: &str) -> bool {
    match p {
        LirPat::Bind { code: n, .. } => n == code,
        LirPat::Ctor { args, .. } => args.iter().any(|x| lir_pat_binds(x, code)),
        LirPat::Tuple(xs) => xs.iter().any(|x| lir_pat_binds(x, code)),
        _ => false,
    }
}

fn lir_substitute(mut e: LirExpr, code: &str, value: &LirExpr) -> LirExpr {
    match &mut e {
        LirExpr::Var(n, _) if n == code => return value.clone(),
        LirExpr::Let {
            code: n,
            value: v,
            body,
            ..
        } => {
            *v = Box::new(lir_substitute((**v).clone(), code, value));
            if n != code {
                *body = Box::new(lir_substitute((**body).clone(), code, value));
            }
        }
        LirExpr::Call { args, .. } | LirExpr::Construct { args, .. } => {
            for a in args {
                *a = lir_substitute(a.clone(), code, value);
            }
        }
        LirExpr::Field { base, .. } => {
            *base = Box::new(lir_substitute((**base).clone(), code, value));
        }
        LirExpr::Block { stmts, tail } => {
            for s in stmts {
                *s = lir_substitute(s.clone(), code, value);
            }
            *tail = Box::new(lir_substitute((**tail).clone(), code, value));
        }
        LirExpr::Case { scrut, arms, .. } => {
            *scrut = Box::new(lir_substitute((**scrut).clone(), code, value));
            for arm in arms {
                if !lir_pat_binds(&arm.pat, code) {
                    arm.body = lir_substitute(arm.body.clone(), code, value);
                }
            }
        }
        LirExpr::Bin { left, right, .. } => {
            *left = Box::new(lir_substitute((**left).clone(), code, value));
            *right = Box::new(lir_substitute((**right).clone(), code, value));
        }
        LirExpr::Un { expr, .. } => {
            *expr = Box::new(lir_substitute((**expr).clone(), code, value));
        }
        LirExpr::RuntimeCall { args, .. } | LirExpr::RuntimeConstruct { args, .. } => {
            for a in args {
                *a = lir_substitute(a.clone(), code, value);
            }
        }
        _ => {}
    }
    e
}

fn lir_substitute_many(mut e: LirExpr, values: &HashMap<String, LirExpr>) -> LirExpr {
    match &mut e {
        LirExpr::Var(n, _) => {
            if let Some(value) = values.get(n) {
                return value.clone();
            }
        }
        LirExpr::Let {
            code, value, body, ..
        } => {
            *value = Box::new(lir_substitute_many((**value).clone(), values));
            if !values.contains_key(code) {
                *body = Box::new(lir_substitute_many((**body).clone(), values));
            }
        }
        LirExpr::Call { target, args, .. } => {
            if let CallTarget::Indirect(callee) = target {
                *callee = Box::new(lir_substitute_many((**callee).clone(), values));
            }
            for arg in args {
                *arg = lir_substitute_many(arg.clone(), values);
            }
        }
        LirExpr::Construct { args, .. } => {
            for arg in args {
                *arg = lir_substitute_many(arg.clone(), values);
            }
        }
        LirExpr::RuntimeCall { args, .. } | LirExpr::RuntimeConstruct { args, .. } => {
            for arg in args {
                *arg = lir_substitute_many(arg.clone(), values);
            }
        }
        LirExpr::Field { base, .. } => {
            *base = Box::new(lir_substitute_many((**base).clone(), values));
        }
        LirExpr::Block { stmts, tail } => {
            for stmt in stmts {
                *stmt = lir_substitute_many(stmt.clone(), values);
            }
            *tail = Box::new(lir_substitute_many((**tail).clone(), values));
        }
        LirExpr::Case { scrut, arms, .. } => {
            *scrut = Box::new(lir_substitute_many((**scrut).clone(), values));
            for arm in arms {
                let shadowed = values.keys().any(|k| lir_pat_binds(&arm.pat, k));
                if !shadowed {
                    arm.body = lir_substitute_many(arm.body.clone(), values);
                }
            }
        }
        LirExpr::Bin { left, right, .. } => {
            *left = Box::new(lir_substitute_many((**left).clone(), values));
            *right = Box::new(lir_substitute_many((**right).clone(), values));
        }
        LirExpr::Un { expr, .. } => {
            *expr = Box::new(lir_substitute_many((**expr).clone(), values));
        }
        _ => {}
    }
    e
}

fn lir_size(e: &LirExpr) -> usize {
    match e {
        LirExpr::Unit
        | LirExpr::Int(_, _)
        | LirExpr::Float(_, _)
        | LirExpr::Str(_)
        | LirExpr::Var(_, _)
        | LirExpr::FnRef(_)
        | LirExpr::Fallback
        | LirExpr::RawAst(..)
        | LirExpr::DirectC { .. } => 1,
        LirExpr::Call { target, args, .. } => {
            1 + match target {
                CallTarget::Indirect(callee) => lir_size(callee),
                _ => 0,
            } + args.iter().map(lir_size).sum::<usize>()
        }
        LirExpr::Construct { args, .. } => 1 + args.iter().map(lir_size).sum::<usize>(),
        LirExpr::Field { base, .. } => 1 + lir_size(base),
        LirExpr::Let { value, body, .. } => 1 + lir_size(value) + lir_size(body),
        LirExpr::Block { stmts, tail } => {
            1 + stmts.iter().map(lir_size).sum::<usize>() + lir_size(tail)
        }
        LirExpr::Case { scrut, arms, .. } => {
            1 + lir_size(scrut) + arms.iter().map(|a| lir_size(&a.body)).sum::<usize>()
        }
        LirExpr::Bin { left, right, .. } => 1 + lir_size(left) + lir_size(right),
        LirExpr::Un { expr, .. } => 1 + lir_size(expr),
        LirExpr::RuntimeCall { args, .. } | LirExpr::RuntimeConstruct { args, .. } => {
            1 + args.iter().map(lir_size).sum::<usize>()
        }
    }
}

fn fold_bin(op: BinOpLir, l: &LirExpr, r: &LirExpr, rep: Rep) -> Option<LirExpr> {
    match (l, r) {
        (LirExpr::Int(a, _), LirExpr::Int(b, _)) if matches!(&rep, Rep::I64) => {
            let a = (*a as u64) as i64;
            let b = (*b as u64) as i64;
            let v = match op {
                BinOpLir::Add => a.wrapping_add(b) as i128,
                BinOpLir::Sub => a.wrapping_sub(b) as i128,
                BinOpLir::Mul => a.wrapping_mul(b) as i128,
                BinOpLir::Div => {
                    if b == 0 { return None; }
                    if a == i64::MIN && b == -1 { i64::MIN as i128 } else { (a / b) as i128 }
                }
                BinOpLir::Rem => {
                    if b == 0 { return None; }
                    if a == i64::MIN && b == -1 { 0 } else { (a % b) as i128 }
                }
                BinOpLir::And => ((a as u64) & (b as u64)) as i64 as i128,
                BinOpLir::Or => ((a as u64) | (b as u64)) as i64 as i128,
                BinOpLir::Xor => ((a as u64) ^ (b as u64)) as i64 as i128,
                BinOpLir::Shl => a.wrapping_shl((b as u32) & 63) as i128,
                BinOpLir::Shr => a.wrapping_shr((b as u32) & 63) as i128,
                BinOpLir::Eq => (a == b) as i128,
                BinOpLir::Ne => (a != b) as i128,
                BinOpLir::Lt => (a < b) as i128,
                BinOpLir::Gt => (a > b) as i128,
                BinOpLir::Le => (a <= b) as i128,
                BinOpLir::Ge => (a >= b) as i128,
                BinOpLir::LAnd => ((a != 0) && (b != 0)) as i128,
                BinOpLir::LOr => ((a != 0) || (b != 0)) as i128,
            };
            Some(LirExpr::Int(v, rep))
        }
        (LirExpr::Int(a, _), LirExpr::Int(b, _)) => {
            let v = match op {
                BinOpLir::Add => a.checked_add(*b)?,
                BinOpLir::Sub => a.checked_sub(*b)?,
                BinOpLir::Mul => a.checked_mul(*b)?,
                BinOpLir::Div => {
                    if *b == 0 { return None; }
                    a.checked_div(*b)?
                }
                BinOpLir::Rem => {
                    if *b == 0 { return None; }
                    a.checked_rem(*b)?
                }
                BinOpLir::And => a & b,
                BinOpLir::Or => a | b,
                BinOpLir::Xor => a ^ b,
                BinOpLir::Shl => {
                    if *b < 0 || *b > 127 { return None; }
                    (*a).checked_shl(*b as u32)?
                }
                BinOpLir::Shr => {
                    if *b < 0 || *b > 127 { return None; }
                    (*a).checked_shr(*b as u32)?
                }
                BinOpLir::Eq => (*a == *b) as i128,
                BinOpLir::Ne => (*a != *b) as i128,
                BinOpLir::Lt => (*a < *b) as i128,
                BinOpLir::Gt => (*a > *b) as i128,
                BinOpLir::Le => (*a <= *b) as i128,
                BinOpLir::Ge => (*a >= *b) as i128,
                BinOpLir::LAnd => ((*a != 0) && (*b != 0)) as i128,
                BinOpLir::LOr => ((*a != 0) || (*b != 0)) as i128,
            };
            Some(LirExpr::Int(v, rep))
        }
        (LirExpr::Float(a, _), LirExpr::Float(b, _)) => match op {
            BinOpLir::Add => Some(LirExpr::Float(a + b, rep)),
            BinOpLir::Sub => Some(LirExpr::Float(a - b, rep)),
            BinOpLir::Mul => Some(LirExpr::Float(a * b, rep)),
            BinOpLir::Div => Some(LirExpr::Float(a / b, rep)),
            BinOpLir::Eq => Some(LirExpr::Int((*a == *b) as i128, Rep::I32)),
            BinOpLir::Ne => Some(LirExpr::Int((*a != *b) as i128, Rep::I32)),
            BinOpLir::Lt => Some(LirExpr::Int((*a < *b) as i128, Rep::I32)),
            BinOpLir::Gt => Some(LirExpr::Int((*a > *b) as i128, Rep::I32)),
            BinOpLir::Le => Some(LirExpr::Int((*a <= *b) as i128, Rep::I32)),
            BinOpLir::Ge => Some(LirExpr::Int((*a >= *b) as i128, Rep::I32)),
            _ => None,
        },
        _ => None,
    }
}

fn fold_un(op: UnOpLir, x: &LirExpr, rep: Rep) -> Option<LirExpr> {
    match x {
        LirExpr::Int(v, _) => {
            let n = match op {
                UnOpLir::Neg if matches!(&rep, Rep::I64) => {
                    (0u64.wrapping_sub(*v as u64) as i64) as i128
                }
                UnOpLir::BitNot if matches!(&rep, Rep::I64) => ((!(*v as u64)) as i64) as i128,
                UnOpLir::Neg => v.checked_neg()?,
                UnOpLir::Not => (*v == 0) as i128,
                UnOpLir::BitNot => !*v,
            };
            Some(LirExpr::Int(n, rep))
        }
        LirExpr::Float(v, _) if matches!(op, UnOpLir::Neg) => Some(LirExpr::Float(-*v, rep)),
        _ => None,
    }
}

fn select_constant_case(scrut: &LirExpr, arms: &[LirArm]) -> Option<LirExpr> {
    if !lir_is_pure(scrut) {
        return None;
    }
    match scrut {
        LirExpr::Int(v, _) => {
            for arm in arms {
                match &arm.pat {
                    LirPat::Int(n) if *n as i128 == *v => return Some(arm.body.clone()),
                    LirPat::Wild => return Some(arm.body.clone()),
                    LirPat::Bind { code, .. } => {
                        return Some(lir_substitute(arm.body.clone(), code, scrut));
                    }
                    _ => {}
                }
            }
        }
        LirExpr::Str(s) => {
            for arm in arms {
                match &arm.pat {
                    LirPat::Str(p) if p == s => return Some(arm.body.clone()),
                    LirPat::Wild => return Some(arm.body.clone()),
                    LirPat::Bind { code, .. } => {
                        return Some(lir_substitute(arm.body.clone(), code, scrut));
                    }
                    _ => {}
                }
            }
        }
        _ => {}
    }
    None
}

fn match_lir_pat_value(
    pat: &LirPat,
    value: &LirExpr,
    bindings: &mut Vec<(String, LirExpr)>,
) -> bool {
    match (pat, value) {
        (LirPat::Wild, _) => true,
        (LirPat::Bind { code, .. }, _) => {
            bindings.push((code.clone(), value.clone()));
            true
        }
        (LirPat::Int(p), LirExpr::Int(v, _)) => *p as i128 == *v,
        (LirPat::Str(p), LirExpr::Str(v)) => p == v,
        (
            LirPat::Ctor {
                ctor: pctor,
                args: pargs,
            },
            LirExpr::Construct {
                ctor: vctor,
                args: vargs,
                ..
            },
        ) => {
            if pctor != vctor || pargs.len() != vargs.len() {
                return false;
            }
            for (p, v) in pargs.iter().zip(vargs) {
                if !match_lir_pat_value(p, v, bindings) {
                    return false;
                }
            }
            true
        }
        _ => false,
    }
}

fn apply_lir_bindings(body: LirExpr, bindings: &[(String, LirExpr)]) -> LirExpr {
    if bindings.is_empty() {
        return body;
    }
    let map = bindings
        .iter()
        .cloned()
        .collect::<HashMap<String, LirExpr>>();
    lir_substitute_many(body, &map)
}

fn select_construct_case(_a: &Analyzer, scrut: &LirExpr, arms: &[LirArm]) -> Option<LirExpr> {
    let LirExpr::Construct { args, .. } = scrut else {
        return None;
    };
    if !args.iter().all(lir_is_simple_value) {
        return None;
    }
    for arm in arms {
        let mut bindings = Vec::new();
        if match_lir_pat_value(&arm.pat, scrut, &mut bindings) {
            return Some(apply_lir_bindings(arm.body.clone(), &bindings));
        }
    }
    None
}

fn lir_is_constant(e: &LirExpr) -> bool {
    matches!(
        e,
        LirExpr::Unit
            | LirExpr::Int(_, _)
            | LirExpr::Float(_, _)
            | LirExpr::Str(_)
            | LirExpr::FnRef(_)
    )
}

fn lir_pattern_provably_false(a: &Analyzer, scrut: &Rep, pat: &LirPat) -> Option<bool> {
    match pat {
        LirPat::Wild | LirPat::Bind { .. } => Some(false),
        LirPat::Int(_) => Some(!scrut.is_integer() && !matches!(scrut, Rep::Unknown)),
        LirPat::Str(_) => Some(!matches!(scrut, Rep::Str | Rep::Ptr | Rep::Unknown)),
        LirPat::Ctor { ctor, args } => {
            let c = a.constructors.get(*ctor)?;
            if args.len() != c.arity {
                return Some(false);
            }
            match scrut {
                Rep::Adt(name) => Some(name != &c.type_name),
                Rep::Unknown | Rep::Ptr => None,
                _ => Some(false),
            }
        }
        LirPat::Tuple(_) => Some(!matches!(scrut, Rep::Ptr | Rep::Unknown)),
    }
}

fn lir_pat_shape_eq(a: &LirPat, b: &LirPat) -> bool {
    match (a, b) {
        (LirPat::Wild, LirPat::Wild)
        | (LirPat::Wild, LirPat::Bind { .. })
        | (LirPat::Bind { .. }, LirPat::Wild)
        | (LirPat::Bind { .. }, LirPat::Bind { .. }) => true,
        (LirPat::Int(x), LirPat::Int(y)) => x == y,
        (LirPat::Str(x), LirPat::Str(y)) => x == y,
        (LirPat::Ctor { ctor: ac, args: aa }, LirPat::Ctor { ctor: bc, args: ba }) => {
            ac == bc
                && aa.len() == ba.len()
                && aa.iter().zip(ba).all(|(x, y)| lir_pat_shape_eq(x, y))
        }
        (LirPat::Tuple(ax), LirPat::Tuple(bx)) => {
            ax.len() == bx.len() && ax.iter().zip(bx).all(|(x, y)| lir_pat_shape_eq(x, y))
        }
        _ => false,
    }
}

fn lir_has_self_tail_call(e: &LirExpr, id: usize) -> bool {
    match e {
        LirExpr::Call {
            target: CallTarget::Function(fid),
            ..
        } if *fid == id => true,
        LirExpr::Let { body, .. } => lir_has_self_tail_call(body, id),
        LirExpr::Block { tail, .. } => lir_has_self_tail_call(tail, id),
        LirExpr::Case { arms, .. } => arms.iter().any(|a| lir_has_self_tail_call(&a.body, id)),
        _ => false,
    }
}

fn lir_same_integer_value(a: &LirExpr, b: &LirExpr) -> bool {
    match (a, b) {
        (LirExpr::Int(x, ra), LirExpr::Int(y, rb)) => x == y && ra.is_integer() && rb.is_integer(),
        (LirExpr::Var(x, ra), LirExpr::Var(y, rb)) => x == y && ra.is_integer() && rb.is_integer(),
        _ => false,
    }
}

fn lir_is_const_zero(e: &LirExpr) -> bool {
    matches!(e, LirExpr::Int(v, _) if *v == 0) || matches!(e, LirExpr::Float(v, _) if *v == 0.0)
}

fn lir_is_const_one(e: &LirExpr) -> bool {
    matches!(e, LirExpr::Int(v, _) if *v == 1) || matches!(e, LirExpr::Float(v, _) if *v == 1.0)
}
fn runtime_recovery_lir(rep: Rep, reason: &str, functions: &[FunctionInfo]) -> LirExpr {
    let escaped = c_escape(reason);
    match &rep {
        Rep::Unit => LirExpr::DirectC {
            code: format!("mak_ir_fail_u64(\"{}\")", escaped),
            rep,
        },
        Rep::Adt(n) => LirExpr::DirectC {
            code: default_adt_expr(n),
            rep,
        },
        Rep::F32 | Rep::F64 | Rep::I8 | Rep::I16 | Rep::I32 | Rep::I64
        | Rep::U8 | Rep::U16 | Rep::U32 | Rep::U64
        | Rep::Str | Rep::Ptr | Rep::Function(_) | Rep::ExternFunction { .. } => {
            let ty = rep.ctype(functions);
            LirExpr::DirectC {
                code: format!("(({})mak_ir_fail_u64(\"{}\"))", ty, escaped),
                rep,
            }
        }
        Rep::Unknown => LirExpr::DirectC {
            code: format!("mak_ir_fail_u64(\"{}\")", escaped),
            rep,
        },
    }
}

fn current_label(current: Option<usize>) -> String {
    current
        .map(|id| id.to_string())
        .unwrap_or_else(|| "top".into())
}
fn rep_debug(r: &Rep) -> String {
    match r {
        Rep::Unknown => "Unknown".into(),
        Rep::Unit => "Unit".into(),
        Rep::I8 => "i8".into(),
        Rep::I16 => "i16".into(),
        Rep::I32 => "i32".into(),
        Rep::I64 => "i64".into(),
        Rep::U8 => "u8".into(),
        Rep::U16 => "u16".into(),
        Rep::U32 => "u32".into(),
        Rep::U64 => "u64".into(),
        Rep::F32 => "f32".into(),
        Rep::F64 => "f64".into(),
        Rep::Str => "str".into(),
        Rep::Ptr => "ptr".into(),
        Rep::Adt(n) => format!("adt<{}>", n),
        Rep::Function(id) => format!("fn#{}", id),
        Rep::ExternFunction { ctype, .. } => format!("extern-fn<{}>", ctype),
    }
}

fn lir_kind_debug(e: &LirExpr) -> &'static str {
    match e {
        LirExpr::Unit => "Unit",
        LirExpr::Int(_, _) => "Int",
        LirExpr::Float(_, _) => "Float",
        LirExpr::Str(_) => "Str",
        LirExpr::Var(_, _) => "Var",
        LirExpr::FnRef(_) => "FnRef",
        LirExpr::Call { .. } => "Call",
        LirExpr::Construct { .. } => "Construct",
        LirExpr::Field { .. } => "Field",
        LirExpr::Let { .. } => "Let",
        LirExpr::Block { .. } => "Block",
        LirExpr::Case { .. } => "Case",
        LirExpr::Bin { .. } => "Bin",
        LirExpr::Un { .. } => "Un",
        LirExpr::RawAst(..) => "RawAst",
        LirExpr::RuntimeCall { .. } => "RuntimeCall",
        LirExpr::RuntimeConstruct { .. } => "RuntimeConstruct",
        LirExpr::DirectC { .. } => "DirectC",
        LirExpr::Fallback => "Fallback",
    }
}

fn expr_debug(e: &Expr) -> String {
    match &e.kind {
        ExprKind::Var(n) => format!("Var({})", n),
        ExprKind::App { fun, args } => format!("App({}, {} arg(s))", expr_debug(fun), args.len()),
        ExprKind::Int(v) => format!("Int({})", v),
        ExprKind::Float(v) => format!("Float({})", v),
        ExprKind::Str(_) => "Str(...)".into(),
        ExprKind::Let { name, .. } => format!("Let({})", name),
        ExprKind::Fn { params, .. } => format!("Fn({} param(s))", params.len()),
        ExprKind::Case { arms, .. } => format!("Case({} arm(s))", arms.len()),
        ExprKind::Block { stmts, .. } => format!("Block({} stmt(s))", stmts.len()),
        ExprKind::Bin { .. } => "Bin(...)".into(),
        ExprKind::Un { .. } => "Un(...)".into(),
        ExprKind::Quote(_) => "Quote(...)".into(),
    }
}

trait StringDefaultExt {
    fn if_empty_default(self) -> String;
}
impl StringDefaultExt for String {
    fn if_empty_default(self) -> String {
        if self.is_empty() { "1".into() } else { self }
    }
}

fn canonical_ident(name: &str) -> String {
    let mut out = String::with_capacity(name.len());
    for b in name.bytes() {
        if b.is_ascii_alphanumeric() || b == b'_' {
            out.push(b.to_ascii_lowercase() as char);
        }
    }
    out
}

fn rep_compatible(actual: &Rep, expected: &Rep) -> bool {
    if matches!(actual, Rep::Unknown) || matches!(expected, Rep::Unknown) {
        return true;
    }
    if actual == expected {
        return true;
    }
    match (actual, expected) {
        (a, b) if a.is_integer() && b.is_integer() => true,
        (a, b) if a.is_float() && b.is_float() => true,
        (Rep::Str, Rep::Ptr) | (Rep::Ptr, Rep::Str) => true,
        (Rep::Adt(a), Rep::Ptr) | (Rep::Ptr, Rep::Adt(a)) => !a.is_empty(),
        (Rep::Function(a), Rep::Ptr) | (Rep::Ptr, Rep::Function(a)) => *a != usize::MAX,
        (Rep::ExternFunction { .. }, Rep::Ptr)
        | (Rep::Ptr, Rep::ExternFunction { .. }) => true,
        (
            Rep::ExternFunction { ctype: a, .. },
            Rep::ExternFunction { ctype: b, .. },
        ) => a == b,
        (Rep::Adt(a), Rep::Adt(b)) => a == b,
        _ => false,
    }
}

fn integer_literal_rep(v: u64) -> Rep {
    if v <= i64::MAX as u64 {
        Rep::I64
    } else {
        Rep::U64
    }
}
fn user_value_name(name: &str) -> String {
    format!("mak_user_{}", mangle_ident(name))
}

fn rep_from_ty(t: &Ty, types: &HashSet<String>) -> Rep {
    match t {
        Ty::Prim(p) => match p {
            Prim::U8 => Rep::U8,
            Prim::U16 => Rep::U16,
            Prim::U32 => Rep::U32,
            Prim::U64 => Rep::U64,
            Prim::I8 => Rep::I8,
            Prim::I16 => Rep::I16,
            Prim::I32 => Rep::I32,
            Prim::I64 => Rep::I64,
            Prim::F32 => Rep::F32,
            Prim::F64 => Rep::F64,
            Prim::Unit => Rep::Unit,
        },
        Ty::Named(n) => match n.as_str() {
            "ptr" => Rep::Ptr,
            "str" => Rep::Str,
            _ if types.contains(n) => Rep::Adt(n.clone()),
            _ => Rep::Unknown,
        },
        Ty::App(n, _) => Rep::Adt(n.clone()),
        Ty::Array(_) | Ty::Tuple(_) => Rep::Ptr,
        Ty::Fun(_, _) => Rep::Ptr,
        Ty::Bottom => Rep::Unit,
    }
}

fn foreign_value_rep(t: &ForeignTy) -> Rep {
    rep_from_foreign(t)
}

fn runtime_kind_for_rep(r: &Rep) -> u8 {
    match r {
        Rep::F32 | Rep::F64 => 2,
        Rep::Str => 3,
        Rep::Ptr | Rep::Adt(_) => 4,
        Rep::Function(_) | Rep::ExternFunction { .. } => 5,
        Rep::Unknown => 0,
        Rep::Unit => 0,
        _ => 1,
    }
}

fn runtime_arg_from_rep(r: &Rep, value: &str, a: &Analyzer) -> String {
    match r {
        Rep::F32 => format!("mak_rt_f32_value({})", value),
        Rep::F64 => format!("mak_rt_f64_value({})", value),
        Rep::Str => format!("(const char *)(uintptr_t)({})", value),
        Rep::Ptr => format!("(const void *)(uintptr_t)({})", value),
        Rep::Adt(n) => format!("(struct mak_{} *)(uintptr_t)({})", mangle_ident(n), value),
        Rep::Function(id) => format!(
            "({})((uintptr_t)({}))",
            function_pointer_type(*id, &a.functions),
            value
        ),
        Rep::ExternFunction { ctype, .. } => {
            format!("({})((uintptr_t)({}))", ctype, value)
        }
        Rep::Unknown => format!("(uint64_t)({})", value),
        Rep::Unit => "0".into(),
        _ => format!("({})({})", r.ctype(&a.functions), value),
    }
}

fn runtime_value_bits(code: &str, rep: &Rep) -> String {
    match rep {
        Rep::F32 => format!("mak_rt_f32_bits({})", code),
        Rep::F64 => format!("mak_rt_f64_bits({})", code),
        Rep::Str | Rep::Ptr | Rep::Adt(_) | Rep::Function(_) | Rep::ExternFunction { .. } => {
            format!("(uint64_t)(uintptr_t)({})", code)
        }
        Rep::Unknown | Rep::Unit => format!("(uint64_t)({})", code),
        _ => format!("(uint64_t)({})", code),
    }
}

fn foreign_ret_rep(t: &ForeignTy) -> Rep {
    foreign_value_rep(t)
}

fn rep_from_foreign(t: &ForeignTy) -> Rep {
    match t {
        ForeignTy::Atom(a) => match a {
            ForeignAtom::I8 => Rep::I8,
            ForeignAtom::I16 => Rep::I16,
            ForeignAtom::I32 => Rep::I32,
            ForeignAtom::I64 => Rep::I64,
            ForeignAtom::U8 => Rep::U8,
            ForeignAtom::U16 => Rep::U16,
            ForeignAtom::U32 => Rep::U32,
            ForeignAtom::U64 => Rep::U64,
            ForeignAtom::F32 => Rep::F32,
            ForeignAtom::F64 => Rep::F64,
            ForeignAtom::Ptr | ForeignAtom::PtrTo(_) => Rep::Ptr,
            ForeignAtom::Void => Rep::Unit,
            ForeignAtom::Named(_) => Rep::Ptr,
        },
        ForeignTy::Fun { args, ret } => {
            let mut fixed_args = Vec::with_capacity(args.len());
            let mut variadic = false;
            for arg in args {
                match arg {
                    ForeignArg::Ty(t) => fixed_args.push(rep_from_foreign(t)),
                    ForeignArg::VarArgs => variadic = true,
                }
            }
            Rep::ExternFunction {
                ctype: foreign_function_pointer_type(&ForeignTy::Fun {
                    args: args.clone(),
                    ret: ret.clone(),
                }),
                args: fixed_args,
                variadic,
                ret: Box::new(foreign_ret_rep(ret)),
            }
        }
    }
}

fn foreign_atom_type(a: &ForeignAtom) -> String {
    match a {
        ForeignAtom::I8 => "int8_t".into(),
        ForeignAtom::I16 => "int16_t".into(),
        ForeignAtom::I32 => "int32_t".into(),
        ForeignAtom::I64 => "int64_t".into(),
        ForeignAtom::U8 => "uint8_t".into(),
        ForeignAtom::U16 => "uint16_t".into(),
        ForeignAtom::U32 => "uint32_t".into(),
        ForeignAtom::U64 => "uint64_t".into(),
        ForeignAtom::F32 => "float".into(),
        ForeignAtom::F64 => "double".into(),


        ForeignAtom::Ptr | ForeignAtom::PtrTo(_) => "const void *".into(),
        ForeignAtom::Void => "void".into(),
        ForeignAtom::Named(_) => "uintptr_t".into(),
    }
}

fn foreign_function_pointer_type(t: &ForeignTy) -> String {
    let ForeignTy::Fun { args, ret } = t else {
        return "void (*)(void)".into();
    };

    let mut c_args = Vec::with_capacity(args.len());
    for arg in args {
        match arg {
            ForeignArg::Ty(t) => c_args.push(foreign_arg_type(t)),
            ForeignArg::VarArgs => c_args.push("...".into()),
        }
    }
    if c_args.is_empty() {
        c_args.push("void".into());
    }

    format!("{} (*)({})", foreign_ret_type(ret), c_args.join(", "))
}

fn foreign_arg_type(t: &ForeignTy) -> String {
    match t {
        ForeignTy::Atom(a) => foreign_atom_type(a),
        ForeignTy::Fun { .. } => foreign_function_pointer_type(t),
    }
}

fn foreign_ret_type(t: &ForeignTy) -> String {
    match t {
        ForeignTy::Atom(a) => foreign_atom_type(a),
        ForeignTy::Fun { .. } => foreign_function_pointer_type(t),
    }
}

fn extern_function_pointer_decl(r: &Rep, name: &str) -> Option<String> {
    let Rep::ExternFunction { ctype, .. } = r else {
        return None;
    };


    let marker = "(*)";
    if let Some(pos) = ctype.find(marker) {
        let mut out = String::with_capacity(ctype.len() + name.len());
        out.push_str(&ctype[..pos]);
        out.push_str("(*");
        out.push_str(name);
        out.push(')');
        out.push_str(&ctype[pos + marker.len()..]);
        Some(out)
    } else {

        Some(format!("{} {}", ctype, name))
    }
}

fn function_pointer_type(id: usize, fs: &[FunctionInfo]) -> String {
    if id >= fs.len() {
        return "void (*)(void)".into();
    }
    let f = &fs[id];
    let aggregate = rep_is_adt(&f.ret_rep);
    let mut args = Vec::new();
    for r in &f.capture_reps {
        args.push(r.ctype(fs));
    }
    for r in &f.param_reps {
        args.push(r.ctype(fs));
    }
    if aggregate {
        if let Rep::Adt(n) = &f.ret_rep {
            args.push(format!("struct mak_{} *", mangle_ident(n)));
        }
    }
    if args.is_empty() {
        args.push("void".into());
    }
    let ret = if aggregate {
        "void".into()
    } else {
        f.ret_rep.ctype(fs)
    };
    format!("{} (*)({})", ret, args.join(", "))
}

fn function_pointer_decl_with_name(id: usize, var_name: &str, fs: &[FunctionInfo]) -> String {
    if id >= fs.len() {
        return format!("void (*{})(void)", var_name);
    }
    let f = &fs[id];
    let aggregate = rep_is_adt(&f.ret_rep);
    let mut args = Vec::new();
    for r in &f.capture_reps {
        args.push(r.ctype(fs));
    }
    for r in &f.param_reps {
        args.push(r.ctype(fs));
    }
    if aggregate {
        if let Rep::Adt(n) = &f.ret_rep {
            args.push(format!("struct mak_{} *", mangle_ident(n)));
        }
    }
    if args.is_empty() {
        args.push("void".into());
    }
    let ret = if aggregate {
        "void".into()
    } else {
        f.ret_rep.ctype(fs)
    };
    format!("{} (*{} )({})", ret, var_name, args.join(", "))
}


fn function_pointer_decl(id: usize, name: &str, fs: &[FunctionInfo]) -> String {
    if id >= fs.len() {
        return format!("void (*{})(void)", name);
    }
    let f = &fs[id];
    let aggregate = rep_is_adt(&f.ret_rep);
    let mut args = Vec::new();
    for r in &f.capture_reps {
        args.push(r.ctype(fs));
    }
    for r in &f.param_reps {
        args.push(r.ctype(fs));
    }
    if aggregate {
        if let Rep::Adt(n) = &f.ret_rep {
            args.push(format!("struct mak_{} *", mangle_ident(n)));
        }
    }
    if args.is_empty() {
        args.push("void".into());
    }
    let ret = if aggregate {
        "void".into()
    } else {
        f.ret_rep.ctype(fs)
    };
    format!("{} (*{})({})", ret, name, args.join(", "))
}

fn rep_decl(r: &Rep, name: &str, functions: &[FunctionInfo]) -> String {
    match r {
        Rep::Function(id) => function_pointer_decl(*id, name, functions),
        Rep::ExternFunction { .. } => extern_function_pointer_decl(r, name)
            .unwrap_or_else(|| format!("void (*{})(void)", name)),
        _ => format!("{} {}", r.ctype(functions), name),
    }
}

fn foreign_index_ctype(t: &ForeignTy) -> String {


    match t {
        ForeignTy::Atom(ForeignAtom::PtrTo(inner)) => {
            format!("{} *", foreign_index_ctype(inner))
        }
        _ => foreign_arg_type(t),
    }
}

fn pointer_info_from_foreign(t: &ForeignTy) -> Option<PointerInfo> {
    let ForeignTy::Atom(ForeignAtom::PtrTo(inner)) = t else {
        return None;
    };
    Some(PointerInfo {
        elem_rep: rep_from_foreign(inner),
        elem_ctype: foreign_index_ctype(inner),
        nested: pointer_info_from_foreign(inner).map(Box::new),
    })
}


fn pointer_info_of_expr(
    e: &Expr,
    a: &Analyzer,
    env: Option<&HashMap<String, Binding>>,
) -> Option<PointerInfo> {
    match &e.kind {
        ExprKind::Str(_) => Some(PointerInfo {
            elem_rep: Rep::U8,
            elem_ctype: "const char".into(),
            nested: None,
        }),
        ExprKind::Var(n) => {
            if let Some(binding) = env.and_then(|env| env.get(n)) {
                return match binding {
                    Binding::Value { ptr_info, .. } => ptr_info.clone(),
                    Binding::Function { .. } => None,
                };
            }
            let eid = a.external_by_name.get(n).copied()?;
            pointer_info_from_foreign(&a.externals[eid].decl.foreign)
        }
        ExprKind::App { fun, args } => {
            let (base, all_args) = Analyzer::flatten_application(fun, args);
            if let ExprKind::Var(n) = &base.kind {
                if n == "$mak.index" && all_args.len() == 2 {
                    return pointer_info_of_expr(all_args[0], a, env)
                        .and_then(|info| info.nested.map(|nested| *nested));
                }
                if let Some(eid) = a.external_by_name.get(n).copied() {
                    if let ForeignTy::Fun { ret, .. } = &a.externals[eid].decl.foreign {
                        return pointer_info_from_foreign(ret);
                    }
                }
            }
            None
        }
        ExprKind::Bin { op, l, r } if matches!(op, BinOp::Add | BinOp::Sub) => {
            pointer_info_of_expr(l, a, env).or_else(|| pointer_info_of_expr(r, a, env))
        }
        ExprKind::Let { name, value, body } => {
            let mut local_env = env.cloned().unwrap_or_default();
            let value_info = pointer_info_of_expr(value, a, Some(&local_env));
            let value_rep = infer_ast_rep(value, a);
            local_env.insert(
                name.clone(),
                Binding::Value {
                    code: String::new(),
                    rep: value_rep,
                    ptr_info: value_info,
                },
            );
            pointer_info_of_expr(body, a, Some(&local_env))
        }
        ExprKind::Quote(inner) => pointer_info_of_expr(inner, a, env),
        _ => None,
    }
}

fn pointer_element_ctype(
    expr: &Expr,
    a: &Analyzer,
    env: Option<&HashMap<String, Binding>>,
    rep: &Rep,
) -> String {
    if let Some(info) = pointer_info_of_expr(expr, a, env) {
        return info.elem_ctype;
    }
    match rep {
        Rep::Adt(name) => format!("struct mak_{}", mangle_ident(name)),
        Rep::Str => "const char".into(),


        _ => "uint8_t".into(),
    }
}

fn is_data_pointer_rep(rep: &Rep) -> bool {
    matches!(rep, Rep::Ptr | Rep::Str | Rep::Adt(_))
}

fn infer_ast_rep(e: &Expr, a: &Analyzer) -> Rep {
    match &e.kind {
        ExprKind::Int(v) => integer_literal_rep(*v),
        ExprKind::Float(_) => Rep::F64,
        ExprKind::Str(_) => Rep::Str,
        ExprKind::Var(n) => {
            if let Some(id) = a.resolve_function_name(n) {
                Rep::Function(id)
            } else if let Some(cid) = a.resolve_ctor_name(n) {
                let c = &a.constructors[cid];
                if c.arity == 0 {
                    Rep::Adt(c.type_name.clone())
                } else {
                    Rep::Unknown
                }
            } else if let Some(id) = a.external_by_name.get(n) {
                foreign_value_rep(&a.externals[*id].decl.foreign)
            } else if let Some(r) = a.global_reps.get(n) {
                r.clone()
            } else {
                Rep::Unknown
            }
        }
        ExprKind::Fn { .. } => a
            .lambda_by_span
            .get(&(e.span.lo, e.span.hi))
            .copied()
            .map(Rep::Function)
            .unwrap_or(Rep::Unknown),
        ExprKind::Let { body, .. } => infer_ast_rep(body, a),
        ExprKind::App { fun, args } => {
            let (base, all_args) = Analyzer::flatten_application(fun, args);
            if matches!(&base.kind, ExprKind::Var(n) if n == "$mak.index") {
                return if all_args.len() == 2 {
                    pointer_info_of_expr(all_args[0], a, None)
                        .map(|info| info.elem_rep)
                        .unwrap_or(Rep::Unknown)
                } else {
                    Rep::Unknown
                };
            }
            if let ExprKind::Var(n) = &base.kind {
                if let Some(id) = a.resolve_function_name(n) {
                    if all_args.len() == a.functions[id].params.len() {
                        return a.functions[id].ret_rep.clone();
                    }
                }
                if let Some(id) = a.external_by_name.get(n) {
                    return match &a.externals[*id].decl.foreign {
                        ForeignTy::Fun { ret, .. } => foreign_ret_rep(ret),
                        ForeignTy::Atom(_) => foreign_value_rep(&a.externals[*id].decl.foreign),
                    };
                }
                if let Some(id) = a.resolve_ctor_name(n) {
                    if all_args.len() == a.constructors[id].arity {
                        return Rep::Adt(a.constructors[id].type_name.clone());
                    }
                }
            }
            Rep::Unknown
        }
        ExprKind::Case { arms, .. } => {
            let mut r = Rep::Unknown;
            for arm in arms {
                r = merge_rep(r, infer_ast_rep(&arm.body, a));
            }
            r
        }
        ExprKind::Block { tail, .. } => infer_ast_rep(tail, a),
        ExprKind::Bin { op, l, r } => {
            let lr = infer_ast_rep(l, a);
            let rr = infer_ast_rep(r, a);
            if matches!(op, BinOp::Add)
                && ((is_data_pointer_rep(&lr) && rr.is_integer())
                    || (lr.is_integer() && is_data_pointer_rep(&rr)))
            {
                if is_data_pointer_rep(&lr) { lr } else { rr }
            } else if matches!(op, BinOp::Sub) && is_data_pointer_rep(&lr) && rr.is_integer() {
                lr
            } else if matches!(op, BinOp::Sub) && is_data_pointer_rep(&lr) && is_data_pointer_rep(&rr) {
                Rep::I64
            } else if matches!(
                op,
                BinOp::Eq
                    | BinOp::Ne
                    | BinOp::Lt
                    | BinOp::Gt
                    | BinOp::Le
                    | BinOp::Ge
                    | BinOp::LAnd
                    | BinOp::LOr
            ) {
                Rep::I32
            } else if lr.is_float() || rr.is_float() {
                Rep::F64
            } else {
                merge_rep(lr, rr)
            }
        }
        ExprKind::Un { op, e } => {
            let r = infer_ast_rep(e, a);
            match op {
                UnOp::Not => Rep::I32,
                UnOp::Neg | UnOp::BitNot => r,
            }
        }
        ExprKind::Quote(_) => Rep::Ptr,
    }
}

fn refine_rep(current: Rep, expected: Rep) -> Rep {
    use Rep::*;
    match (&current, &expected) {
        (Unknown, _) => expected,
        (_, Unknown) => current,
        (x, y) if x == y => current,
        (Unit, x) => x.clone(),
        (x, Unit) => x.clone(),
        (Adt(_), Ptr) | (Ptr, Adt(_)) => Unknown,
        (Function(_), Ptr) | (Ptr, Function(_)) => Unknown,
        (x @ ExternFunction { .. }, Ptr) | (Ptr, x @ ExternFunction { .. }) => x.clone(),
        (Str, Ptr) | (Ptr, Str) => Str,
        (F64, F32) | (F32, F64) => F64,
        (x, y) if x.is_float() && y.is_integer() => x.clone(),
        (x, y) if y.is_float() && x.is_integer() => y.clone(),
        (x, y) if x.is_integer() && y.is_integer() => merge_rep(x.clone(), y.clone()),
        _ => Unknown,
    }
}

fn observe_rep(current: Rep, observed: Rep) -> Rep {
    use Rep::*;
    match (&current, &observed) {
        (Unknown, _) => observed,
        (_, Unknown) => current,
        (x, y) if x == y => current,
        (x, y) if x.is_integer() && y.is_integer() => merge_rep(x.clone(), y.clone()),
        (x, y) if x.is_float() && y.is_float() => merge_rep(x.clone(), y.clone()),
        (F64, y) if y.is_integer() => F64,
        (y, F64) if y.is_integer() => F64,
        (F32, y) if y.is_integer() => F32,
        (y, F32) if y.is_integer() => F32,
        (Adt(x), Ptr) | (Ptr, Adt(x)) => Adt(x.clone()),
        (Function(x), Ptr) | (Ptr, Function(x)) => Function(*x),
        (x @ ExternFunction { .. }, Ptr) | (Ptr, x @ ExternFunction { .. }) => x.clone(),
        (Str, Ptr) | (Ptr, Str) => Str,
        _ => Unknown,
    }
}

fn op_debug(op: BinOp) -> &'static str {
    match op {
        BinOp::Add => "+",
        BinOp::Sub => "-",
        BinOp::Mul => "*",
        BinOp::Div => "/",
        BinOp::Rem => "%",
        BinOp::And => "&",
        BinOp::Or => "|",
        BinOp::Xor => "^",
        BinOp::Shl => "<<",
        BinOp::Shr => ">>",
        BinOp::Eq => "==",
        BinOp::Ne => "!=",
        BinOp::Lt => "<",
        BinOp::Gt => ">",
        BinOp::Le => "<=",
        BinOp::Ge => ">=",
        BinOp::LAnd => "&&",
        BinOp::LOr => "||",
    }
}

fn rep_equality_compatible(a: &Rep, b: &Rep) -> bool {
    match (a, b) {
        (Rep::Unknown, _) | (_, Rep::Unknown) => false,
        (x, y) if (x.is_integer() || x.is_float()) && (y.is_integer() || y.is_float()) => true,
        (Rep::Str, Rep::Str) => true,
        (Rep::Adt(x), Rep::Adt(y)) => x == y,
        (Rep::Ptr, Rep::Ptr) => true,
        (
            Rep::ExternFunction { ctype: x, .. },
            Rep::ExternFunction { ctype: y, .. },
        ) => x == y,
        (Rep::ExternFunction { .. }, Rep::Ptr)
        | (Rep::Ptr, Rep::ExternFunction { .. }) => true,
        (Rep::Function(x), Rep::Function(y)) => x == y,
        _ => false,
    }
}

fn merge_rep(a: Rep, b: Rep) -> Rep {
    use Rep::*;
    match (a, b) {
        (Unknown, x) | (x, Unknown) => x,
        (Unit, x) | (x, Unit) => x,
        (x, y) if x == y => x,
        (Adt(x), Ptr) | (Ptr, Adt(x)) => Adt(x),
        (Function(x), Ptr) | (Ptr, Function(x)) => Function(x),
        (x @ ExternFunction { .. }, Ptr) | (Ptr, x @ ExternFunction { .. }) => x,
        (Str, Ptr) | (Ptr, Str) => Str,
        (F64, F32) | (F32, F64) => F64,
        (F64, I8)
        | (I8, F64)
        | (F64, I16)
        | (I16, F64)
        | (F64, I32)
        | (I32, F64)
        | (F64, I64)
        | (I64, F64)
        | (F64, U8)
        | (U8, F64)
        | (F64, U16)
        | (U16, F64)
        | (F64, U32)
        | (U32, F64)
        | (F64, U64)
        | (U64, F64) => F64,
        (F32, I8)
        | (I8, F32)
        | (F32, I16)
        | (I16, F32)
        | (F32, I32)
        | (I32, F32)
        | (F32, I64)
        | (I64, F32)
        | (F32, U8)
        | (U8, F32)
        | (F32, U16)
        | (U16, F32)
        | (F32, U32)
        | (U32, F32)
        | (F32, U64)
        | (U64, F32) => F32,
        (I64, I8)
        | (I8, I64)
        | (I64, I16)
        | (I16, I64)
        | (I64, I32)
        | (I32, I64)
        | (I64, U8)
        | (U8, I64)
        | (I64, U16)
        | (U16, I64)
        | (I64, U32)
        | (U32, I64)
        | (I64, U64)
        | (U64, I64) => I64,
        (U64, U8) | (U8, U64) | (U64, U16) | (U16, U64) | (U64, U32) | (U32, U64) => U64,
        (I32, U32) | (U32, I32) => I64,
        (I32, I16) | (I16, I32) | (I32, I8) | (I8, I32) => I32,
        (U32, U16) | (U16, U32) | (U32, U8) | (U8, U32) => U32,
        (I16, U16) | (U16, I16) | (I16, I8) | (I8, I16) => I16,
        (U16, U8) | (U8, U16) => U16,
        (Ptr, Ptr) => Ptr,
        (Str, Str) => Str,
        (Adt(x), Adt(y)) if x == y => Adt(x),
        (Function(x), Function(y)) if x == y => Function(x),
        _ => Unknown,
    }
}

fn collect_captures(
    body: &Expr,
    outer: &HashSet<String>,
    params: &[String],
    globals: &HashSet<String>,
    types: &HashSet<String>,
) -> Vec<String> {
    let mut bound: HashSet<String> = params.iter().cloned().collect();
    let mut found = BTreeSet::new();
    fn walk(
        e: &Expr,
        outer: &HashSet<String>,
        bound: &mut HashSet<String>,
        globals: &HashSet<String>,
        types: &HashSet<String>,
        found: &mut BTreeSet<String>,
    ) {
        match &e.kind {
            ExprKind::Var(n) => {
                if outer.contains(n)
                    && !bound.contains(n)
                    && !globals.contains(n)
                    && !types.contains(n)
                {
                    found.insert(n.clone());
                }
            }
            ExprKind::Let { name, value, body } => {
                walk(value, outer, bound, globals, types, found);
                let old = bound.insert(name.clone());
                walk(body, outer, bound, globals, types, found);
                if !old {
                    bound.remove(name);
                }
            }
            ExprKind::Fn { params, body } => {
                let mut next = bound.clone();
                for p in params {
                    next.insert(p.clone());
                }
                walk(body, outer, &mut next, globals, types, found);
            }
            ExprKind::App { fun, args } => {
                walk(fun, outer, bound, globals, types, found);
                for a in args {
                    walk(a, outer, bound, globals, types, found);
                }
            }
            ExprKind::Case { scrut, arms } => {
                walk(scrut, outer, bound, globals, types, found);
                for arm in arms {
                    let mut next = bound.clone();
                    for n in pat_names(&arm.pat) {
                        next.insert(n);
                    }
                    walk(&arm.body, outer, &mut next, globals, types, found);
                }
            }
            ExprKind::Block { stmts, tail } => {
                for s in stmts {
                    walk(s, outer, bound, globals, types, found);
                }
                walk(tail, outer, bound, globals, types, found);
            }
            ExprKind::Bin { l, r, .. } => {
                walk(l, outer, bound, globals, types, found);
                walk(r, outer, bound, globals, types, found);
            }
            ExprKind::Un { e, .. } | ExprKind::Quote(e) => {
                walk(e, outer, bound, globals, types, found)
            }
            ExprKind::Int(_) | ExprKind::Float(_) | ExprKind::Str(_) => {}
        }
    }
    walk(body, outer, &mut bound, globals, types, &mut found);
    found.into_iter().collect()
}

fn find_structural_entry(p: &Program, functions: &HashMap<String, usize>) -> Option<usize> {
    match &p.body.kind {
        ExprKind::App { fun, args } if args.is_empty() => {
            if let ExprKind::Var(n) = &fun.kind {
                functions.get(n).copied()
            } else {
                None
            }
        }
        _ => None,
    }
}

fn constructor_field_prefix(a: &Analyzer, cid: usize) -> String {
    let c = &a.constructors[cid];
    let count = a
        .constructors
        .iter()
        .filter(|x| x.type_name == c.type_name)
        .count();
    if count == 1 {
        "v.".into()
    } else {
        format!("v.v{}.", cid)
    }
}

fn cast_code(code: &str, from: &Rep, to: &Rep, a: &Analyzer) -> String {
    if code.is_empty() {
        return zero_for(to);
    }
    if from == to {
        return code.into();
    }
    match (from, to) {
        (Rep::F32 | Rep::F64, Rep::I8) => format!("(int8_t)({})", code),
        (Rep::F32 | Rep::F64, Rep::I16) => format!("(int16_t)({})", code),
        (Rep::F32 | Rep::F64, Rep::I32) => format!("(int32_t)({})", code),
        (Rep::F32 | Rep::F64, Rep::I64) => format!("(int64_t)({})", code),
        (Rep::F32 | Rep::F64, Rep::U8) => format!("(uint8_t)({})", code),
        (Rep::F32 | Rep::F64, Rep::U16) => format!("(uint16_t)({})", code),
        (Rep::F32 | Rep::F64, Rep::U32) => format!("(uint32_t)({})", code),
        (Rep::F32 | Rep::F64, Rep::U64) => format!("(uint64_t)({})", code),
        (x, y) if x.is_integer() && y.is_integer() => {
            format!("({})({})", y.ctype(&a.functions), code)
        }
        (Rep::Str, Rep::Ptr) => format!("(const void *)({})", code),
        (Rep::Ptr, Rep::Str) => format!("(const char *)({})", code),
        (Rep::Adt(_), Rep::Ptr) => format!("(const void *)({})", code),
        (Rep::Ptr, Rep::Adt(n)) => format!(
            "(struct mak_{} *)mak_ir_adt_cast_checked((const void *)({}), UINT32_C({}), (const void *)&mak_ir_default_obj_{})",
            mangle_ident(n),
            code,
            adt_type_id(n),
            mangle_ident(n)
        ),
        (Rep::Function(_), Rep::Ptr) => format!("(const void *)((uintptr_t)({}))", code),
        (Rep::ExternFunction { .. }, Rep::Ptr) =>
            format!("(const void *)((uintptr_t)({}))", code),
        (Rep::Ptr, Rep::Function(id)) => format!(
            "({})((uintptr_t)({}))",
            function_pointer_type(*id, &a.functions),
            code
        ),
        (Rep::Ptr, Rep::ExternFunction { ctype, .. }) => {
            format!("({})((uintptr_t)({}))", ctype, code)
        },
        (Rep::Unknown, Rep::Ptr) => format!("(const void *)((uintptr_t)({}))", code),
        (Rep::Unknown, Rep::Str) => format!("(const char *)((uintptr_t)({}))", code),
        (Rep::Unknown, Rep::Adt(n)) => format!(
            "(struct mak_{} *)mak_ir_adt_cast_checked((const void *)(uintptr_t)({}), UINT32_C({}), (const void *)&mak_ir_default_obj_{})",
            mangle_ident(n),
            code,
            adt_type_id(n),
            mangle_ident(n)
        ),
        (Rep::Unknown, Rep::Function(id)) => format!(
            "({})((uintptr_t)({}))",
            function_pointer_type(*id, &a.functions),
            code
        ),
        (Rep::Unknown, Rep::ExternFunction { ctype, .. }) => {
            format!("({})((uintptr_t)({}))", ctype, code)
        },
        (x, Rep::Ptr) if x.is_integer() => format!("(const void *)((uintptr_t)({}))", code),
        (x, Rep::Str) if x.is_integer() => format!("(const char *)((uintptr_t)({}))", code),
        (x, Rep::Adt(n)) if x.is_integer() => format!(
            "(struct mak_{} *)mak_ir_adt_recover_ptr((const void *)&mak_ir_default_obj_{}, \"integer used as ADT\")",
            mangle_ident(n),
            mangle_ident(n)
        ),
        (x, Rep::Function(id)) if x.is_integer() => format!(
            "({})((uintptr_t)({}))",
            function_pointer_type(*id, &a.functions),
            code
        ),
        (x, Rep::ExternFunction { ctype, .. }) if x.is_integer() => {
            format!("({})((uintptr_t)({}))", ctype, code)
        },
        (Rep::Ptr, y) if y.is_integer() => {
            format!("({})((uintptr_t)({}))", y.ctype(&a.functions), code)
        }
        (Rep::Str, y) if y.is_integer() => {
            format!("({})((uintptr_t)({}))", y.ctype(&a.functions), code)
        }
        (Rep::Adt(_), y) if y.is_integer() => {
            format!("({})((uintptr_t)({}))", y.ctype(&a.functions), code)
        }
        (Rep::Function(_), y) if y.is_integer() => {
            format!("({})((uintptr_t)({}))", y.ctype(&a.functions), code)
        }
        (Rep::ExternFunction { .. }, y) if y.is_integer() => {
            format!("({})((uintptr_t)({}))", y.ctype(&a.functions), code)
        }
        (Rep::Adt(_), Rep::Adt(n)) => {
            format!(
                "(struct mak_{} *)mak_ir_adt_cast_checked((const void *)({}), UINT32_C({}), (const void *)&mak_ir_default_obj_{})",
                mangle_ident(n),
                code,
                adt_type_id(n),
                mangle_ident(n)
            )
        }
        (Rep::Str, Rep::Adt(n)) => format!(
            "(struct mak_{} *)mak_ir_adt_recover_ptr((const void *)&mak_ir_default_obj_{}, \"string used as ADT\")",
            mangle_ident(n),
            mangle_ident(n)
        ),
        (Rep::Adt(_), Rep::Str) => format!("(const char *)((void *)({}))", code),
        (Rep::Function(_), Rep::Adt(n)) => format!(
            "(struct mak_{} *)mak_ir_adt_recover_ptr((const void *)&mak_ir_default_obj_{}, \"function used as ADT\")",
            mangle_ident(n),
            mangle_ident(n)
        ),
        (Rep::ExternFunction { .. }, Rep::Adt(n)) => format!(
            "(struct mak_{} *)mak_ir_adt_recover_ptr((const void *)&mak_ir_default_obj_{}, \"foreign function used as ADT\")",
            mangle_ident(n),
            mangle_ident(n)
        ),
        (Rep::Adt(_), Rep::Function(id)) => format!(
            "({})((uintptr_t)({}))",
            function_pointer_type(*id, &a.functions),
            code
        ),
        (Rep::Adt(_), Rep::ExternFunction { ctype, .. }) => {
            format!("({})((uintptr_t)({}))", ctype, code)
        },
        (Rep::Adt(_), Rep::Unknown) => format!("(uintptr_t)({})", code),
        (Rep::Unknown, y) if y.is_integer() => format!("({})({})", y.ctype(&a.functions), code),


        (
            Rep::Adt(_)
            | Rep::Ptr
            | Rep::Str
            | Rep::Function(_)
            | Rep::ExternFunction { .. },
            Rep::F32,
        ) => {
            format!("((float)(uintptr_t)({}))", code)
        }
        (
            Rep::Adt(_)
            | Rep::Ptr
            | Rep::Str
            | Rep::Function(_)
            | Rep::ExternFunction { .. },
            Rep::F64,
        ) => {
            format!("((double)(uintptr_t)({}))", code)
        }
        _ => code.into(),
    }
}

fn c_uint_case(v: u64) -> String {
    if v <= i64::MAX as u64 {
        format!("INT64_C({})", v)
    } else {
        format!("((int64_t)UINT64_C({}))", v)
    }
}

fn c_i128(v: i128) -> String {
    if v == i64::MIN as i128 {
        return "INT64_MIN".into();
    }
    if v >= i64::MIN as i128 && v <= i64::MAX as i128 {
        format!("INT64_C({})", v)
    } else if v >= 0 {
        format!("((int64_t)UINT64_C({}))", v as u64)
    } else {
        format!("((int64_t)-UINT64_C({}))", (-v) as u64)
    }
}

fn c_float(v: f64) -> String {
    if v.is_nan() {
        return "__builtin_nan(\"\")".into();
    }
    if v.is_infinite() {
        return if v.is_sign_negative() {
            "-__builtin_inf()".into()
        } else {
            "__builtin_inf()".into()
        };
    }
    let mut s = format!("{:.17}", v);
    if !s.contains('.') && !s.contains('e') && !s.contains('E') {
        s.push_str(".0");
    }
    s
}

fn c_string(v: &[u8]) -> String {
    let mut s = String::new();
    s.push('"');
    for b in v {
        match *b {
            b'\\' => s.push_str("\\\\"),
            b'"' => s.push_str("\\\""),
            b'\n' => s.push_str("\\n"),
            b'\r' => s.push_str("\\r"),
            b'\t' => s.push_str("\\t"),
            0 => s.push_str("\\000"),
            0x20..=0x7e => s.push(*b as char),
            _ => {
                let _ = write!(s, "\\{:03o}", b);
            }
        }
    }
    s.push('"');
    s
}

fn c_escape(v: &str) -> String {
    let mut s = String::with_capacity(v.len());
    for b in v.bytes() {
        match b {
            b'\\' => s.push_str("\\\\"),
            b'"' => s.push_str("\\\""),
            b'\n' => s.push_str("\\n"),
            b'\r' => s.push_str("\\r"),
            b'\t' => s.push_str("\\t"),
            0x20..=0x7e => s.push(b as char),
            _ => {
                let _ = write!(s, "\\{:03o}", b);
            }
        }
    }
    s
}

fn adt_type_id(name: &str) -> u32 {
    let mut h = 2166136261u32;
    for b in name.as_bytes() {
        h ^= *b as u32;
        h = h.wrapping_mul(16777619);
    }
    h
}

fn default_adt_expr(name: &str) -> String {
    format!("&mak_ir_default_obj_{}", mangle_ident(name))
}

fn default_value_expr(r: &Rep, _a: &Analyzer) -> String {
    match r {
        Rep::F32 | Rep::F64 => "0.0".into(),
        Rep::Str => "\"\"".into(),
        Rep::Ptr => "(const void *)0".into(),
        Rep::Adt(n) => default_adt_expr(n),
        Rep::Function(id) => format!("({})0", function_pointer_type(*id, &_a.functions)),
        Rep::ExternFunction { ctype, .. } => format!("({})0", ctype),
        Rep::Unit => "0".into(),
        _ => "0".into(),
    }
}

fn zero_for(r: &Rep) -> String {
    match r {
        Rep::F32 | Rep::F64 => "0.0".into(),
        Rep::Str | Rep::Ptr | Rep::Adt(_) | Rep::Function(_) | Rep::ExternFunction { .. } => {
            "0".into()
        }
        _ => "0".into(),
    }
}

fn indent(n: usize) -> String {
    "    ".repeat(n)
}

fn mangle_ident(s: &str) -> String {
    let mut out = String::with_capacity(s.len() + 1);
    for (i, b) in s.bytes().enumerate() {
        if (b as char).is_ascii_alphanumeric() || b == b'_' {
            if i == 0 && (b as char).is_ascii_digit() {
                out.push('_');
            }
            out.push(b as char);
        } else {
            out.push('_');
        }
    }
    if out.is_empty() {
        out.push('_');
    }
    if is_c_keyword(&out) {
        out.insert(0, '_');
    }
    out
}

fn is_c_keyword(s: &str) -> bool {
    matches!(
        s,
        "auto"
            | "break"
            | "case"
            | "char"
            | "const"
            | "continue"
            | "default"
            | "do"
            | "double"
            | "else"
            | "enum"
            | "extern"
            | "float"
            | "for"
            | "goto"
            | "if"
            | "inline"
            | "int"
            | "long"
            | "register"
            | "restrict"
            | "return"
            | "short"
            | "signed"
            | "sizeof"
            | "static"
            | "struct"
            | "switch"
            | "typedef"
            | "union"
            | "unsigned"
            | "void"
            | "volatile"
            | "while"
            | "_Alignas"
            | "_Atomic"
            | "_Bool"
            | "_Complex"
            | "_Generic"
            | "_Imaginary"
            | "_Noreturn"
            | "_Static_assert"
            | "_Thread_local"
    )
}

fn pat_names(p: &Pat) -> Vec<String> {
    let mut out = Vec::new();
    fn go(p: &Pat, out: &mut Vec<String>) {
        match p {
            Pat::Bind(n) => out.push(n.clone()),
            Pat::Ctor { args, .. } => {
                for a in args {
                    go(a, out);
                }
            }
            Pat::Tuple(xs) => {
                for x in xs {
                    go(x, out);
                }
            }
            _ => {}
        }
    }
    go(p, &mut out);
    out
}

fn pat_has_string(p: &Pat) -> bool {
    match p {
        Pat::Str(_) => true,
        Pat::Ctor { args, .. } => args.iter().any(pat_has_string),
        Pat::Tuple(xs) => xs.iter().any(pat_has_string),
        _ => false,
    }
}

fn can_switch(arms: &[LirArm]) -> bool {
    arms.len() >= 3
        && arms
            .iter()
            .all(|a| matches!(a.pat, LirPat::Int(_) | LirPat::Wild | LirPat::Bind { .. }))
}

fn has_exhaustive_wild_lir(arms: &[LirArm]) -> bool {
    arms.iter()
        .any(|a| matches!(a.pat, LirPat::Wild | LirPat::Bind { .. }))
}

fn lir_is_simple_value(e: &LirExpr) -> bool {
    matches!(
        e,
        LirExpr::Var(_, _)
            | LirExpr::Int(_, _)
            | LirExpr::Float(_, _)
            | LirExpr::Str(_)
            | LirExpr::FnRef(_)
    )
}

fn bin_lir(op: BinOp) -> BinOpLir {
    match op {
        BinOp::Add => BinOpLir::Add,
        BinOp::Sub => BinOpLir::Sub,
        BinOp::Mul => BinOpLir::Mul,
        BinOp::Div => BinOpLir::Div,
        BinOp::Rem => BinOpLir::Rem,
        BinOp::And => BinOpLir::And,
        BinOp::Or => BinOpLir::Or,
        BinOp::Xor => BinOpLir::Xor,
        BinOp::Shl => BinOpLir::Shl,
        BinOp::Shr => BinOpLir::Shr,
        BinOp::Eq => BinOpLir::Eq,
        BinOp::Ne => BinOpLir::Ne,
        BinOp::Lt => BinOpLir::Lt,
        BinOp::Gt => BinOpLir::Gt,
        BinOp::Le => BinOpLir::Le,
        BinOp::Ge => BinOpLir::Ge,
        BinOp::LAnd => BinOpLir::LAnd,
        BinOp::LOr => BinOpLir::LOr,
    }
}

fn un_lir(op: UnOp) -> UnOpLir {
    match op {
        UnOp::Neg => UnOpLir::Neg,
        UnOp::Not => UnOpLir::Not,
        UnOp::BitNot => UnOpLir::BitNot,
    }
}


pub fn generate(program: &Program) -> String {
    let mut analyzer = Analyzer::new();
    analyzer.register(program);
    analyzer.solve();
    let mut lowerer = Lowerer::new(&analyzer);
    let user_lir = lowerer.lower_all();
    for f in &user_lir {
        if lir_contains_fallback(&f.body) {
            analyzer.dbg(
                "RUNTIME",
                format!(
                    "f{} contains unresolved LIR; retaining it for runtime recovery",
                    f.info.id
                ),
            );
        }
    }
    let optimizer = Optimizer { a: &analyzer };
    let optimized = optimizer.optimize_program(user_lir);
    for f in &optimized {
        if lir_contains_fallback(&f.body) {
            analyzer.dbg(
                "RUNTIME",
                format!(
                    "f{} still contains unresolved LIR after optimization; C emitter will use runtime recovery",
                    f.info.id
                ),
            );
        }
    }
    let mut emitter = CEmitter::new(&analyzer);
    emitter.emit_program(program, &optimized)
}

pub fn emit_to_file(input_path: &str, program: &Program) -> io::Result<PathBuf> {
    let c = generate(program);
    let output = c_path(input_path);
    fs::write(&output, c.as_bytes())?;
    Ok(output)
}

pub fn generate_mlir(program: &Program) -> String {
    let mut analyzer = Analyzer::new();
    analyzer.register(program);
    analyzer.solve();
    let mut lowerer = Lowerer::new(&analyzer);
    let lir = lowerer.lower_all();
    let optimizer = Optimizer { a: &analyzer };
    let lir = optimizer.optimize_program(lir);

    let mut out = String::with_capacity(4096);
    writeln!(&mut out, "module @mak {{").unwrap();
    writeln!(&mut out, "  format = {}", mlir_quote("mLIR 0.1")).unwrap();
    writeln!(&mut out, "  optimized = true").unwrap();

    if !analyzer.constructors.is_empty() {
        out.push('\n');
        writeln!(&mut out, "  types {{").unwrap();
        let mut type_names = BTreeSet::new();
        for c in &analyzer.constructors {
            type_names.insert(c.type_name.clone());
        }
        for name in type_names {
            writeln!(&mut out, "    type @{} = adt", mangle_ident(&name)).unwrap();
            for c in analyzer.constructors.iter().filter(|x| x.type_name == name) {
                let args = c.fields.iter().map(mlir_rep).collect::<Vec<_>>().join(", ");
                writeln!(
                    &mut out,
                    "      ctor @{} tag={} ({})",
                    mangle_ident(&c.name),
                    c.id + 1,
                    args
                )
                .unwrap();
            }
        }
        writeln!(&mut out, "  }}").unwrap();
    }

    if !analyzer.externals.is_empty() {
        out.push('\n');
        writeln!(&mut out, "  externals {{").unwrap();
        for ext in &analyzer.externals {
            writeln!(
                &mut out,
                "    extern @{} symbol={} type={}{}",
                ext.c_name,
                mlir_quote(&ext.decl.symbol),
                mlir_foreign_ty(&ext.decl.foreign),
                if ext.decl.pure { " pure" } else { "" }
            )
            .unwrap();
        }
        writeln!(&mut out, "  }}").unwrap();
    }

    let has_globals = program.decls.iter().any(
        |d| matches!(d, Decl::Let { value, .. } if !matches!(&value.kind, ExprKind::Fn { .. })),
    );
    if has_globals {
        out.push('\n');
        writeln!(&mut out, "  globals {{").unwrap();
        let mut lowerer = Lowerer::new(&analyzer);
        let mut env = HashMap::new();
        for d in &program.decls {
            if let Decl::Let { name, value, .. } = d {
                if matches!(&value.kind, ExprKind::Fn { .. }) {
                    continue;
                }
                let ir = lowerer.lower_expr(value, &mut env, None);
                writeln!(
                    &mut out,
                    "    global @{} : {} = {}",
                    mangle_ident(name),
                    mlir_rep(&lir_rep(&ir)),
                    mlir_expr_inline(&ir)
                )
                .unwrap();
            }
        }
        writeln!(&mut out, "  }}").unwrap();
    }

    if !lir.is_empty() {
        out.push('\n');
        writeln!(&mut out, "  functions {{").unwrap();
        for f in &lir {
            let info = &f.info;
            let mut args = Vec::new();
            for (i, r) in info.capture_reps.iter().enumerate() {
                args.push(format!("%c{i}: {}", mlir_rep(r)));
            }
            for (i, r) in info.param_reps.iter().enumerate() {
                args.push(format!("%a{i}: {}", mlir_rep(r)));
            }
            let source_name = info.source_name.as_deref().unwrap_or("<lambda>");
            writeln!(
                &mut out,
                "    fn @{} [{}] : ({}) -> {}{} {{",
                info.c_name,
                mlir_quote(source_name),
                args.join(", "),
                mlir_rep(&info.ret_rep),
                if info.pure { " pure" } else { "" }
            )
            .unwrap();
            render_mlir_expr(&mut out, &f.body, 3);
            writeln!(&mut out, "    }}").unwrap();
        }
        writeln!(&mut out, "  }}").unwrap();
    }

    let entry = find_structural_entry(program, &analyzer.function_by_source);
    out.push('\n');
    writeln!(&mut out, "  entry {{").unwrap();
    if let Some(id) = entry {
        let ret = lir
            .iter()
            .find(|f| f.info.id == id)
            .map(|f| mlir_rep(&f.info.ret_rep))
            .unwrap_or_else(|| "unit".into());
        writeln!(&mut out, "    call @f{id}() -> {ret}").unwrap();
    } else {
        let mut lowerer = Lowerer::new(&analyzer);
        let mut env = HashMap::new();
        let body = lowerer.lower_expr(&program.body, &mut env, None);
        render_mlir_expr(&mut out, &body, 2);
    }
    writeln!(&mut out, "  }}").unwrap();
    writeln!(&mut out, "}}").unwrap();
    out
}

fn mlir_quote(s: &str) -> String {
    let mut out = String::with_capacity(s.len() + 2);
    out.push('"');
    for ch in s.chars() {
        match ch {
            '\\' => out.push_str("\\\\"),
            '"' => out.push_str("\\\""),
            '\n' => out.push_str("\\n"),
            '\r' => out.push_str("\\r"),
            '\t' => out.push_str("\\t"),
            c if c.is_control() => out.push_str(&format!("\\u{{{:x}}}", c as u32)),
            c => out.push(c),
        }
    }
    out.push('"');
    out
}

fn mlir_bytes(bytes: &[u8]) -> String {
    let mut s = String::with_capacity(bytes.len() * 2 + 2);
    s.push('"');
    for &b in bytes {
        match b {
            b'\\' => s.push_str("\\\\"),
            b'"' => s.push_str("\\\""),
            b'\n' => s.push_str("\\n"),
            b'\r' => s.push_str("\\r"),
            b'\t' => s.push_str("\\t"),
            0x20..=0x7e => s.push(b as char),
            _ => s.push_str(&format!("\\x{b:02x}")),
        }
    }
    s.push('"');
    s
}

fn mlir_rep(r: &Rep) -> String {
    match r {
        Rep::Unknown => "unknown".into(),
        Rep::Unit => "unit".into(),
        Rep::I8 => "i8".into(),
        Rep::I16 => "i16".into(),
        Rep::I32 => "i32".into(),
        Rep::I64 => "i64".into(),
        Rep::U8 => "u8".into(),
        Rep::U16 => "u16".into(),
        Rep::U32 => "u32".into(),
        Rep::U64 => "u64".into(),
        Rep::F32 => "f32".into(),
        Rep::F64 => "f64".into(),
        Rep::Str => "str".into(),
        Rep::Ptr => "ptr".into(),
        Rep::Adt(n) => format!("!adt.{}", mangle_ident(n)),
        Rep::Function(id) => format!("!fn.{}", id),
        Rep::ExternFunction { ctype, .. } => format!("!extern-fn<{}>", ctype),
    }
}

fn mlir_foreign_atom(a: &ForeignAtom) -> String {
    match a {
        ForeignAtom::I8 => "i8".into(),
        ForeignAtom::I16 => "i16".into(),
        ForeignAtom::I32 => "i32".into(),
        ForeignAtom::I64 => "i64".into(),
        ForeignAtom::U8 => "u8".into(),
        ForeignAtom::U16 => "u16".into(),
        ForeignAtom::U32 => "u32".into(),
        ForeignAtom::U64 => "u64".into(),
        ForeignAtom::F32 => "f32".into(),
        ForeignAtom::F64 => "f64".into(),
        ForeignAtom::Ptr => "ptr".into(),
        ForeignAtom::Void => "unit".into(),
        ForeignAtom::Named(n) => format!("named<{}>", mangle_ident(n)),
        ForeignAtom::PtrTo(t) => format!("ptr<{}>", mlir_foreign_ty(t)),
    }
}

fn mlir_foreign_ty(t: &ForeignTy) -> String {
    match t {
        ForeignTy::Atom(a) => mlir_foreign_atom(a),
        ForeignTy::Fun { args, ret } => {
            let args = args
                .iter()
                .map(|a| match a {
                    ForeignArg::Ty(t) => mlir_foreign_ty(t),
                    ForeignArg::VarArgs => "...".into(),
                })
                .collect::<Vec<_>>()
                .join(", ");
            format!("fn({args}) -> {}", mlir_foreign_ty(ret))
        }
    }
}

fn mlir_bin(op: BinOpLir) -> &'static str {
    match op {
        BinOpLir::Add => "add",
        BinOpLir::Sub => "sub",
        BinOpLir::Mul => "mul",
        BinOpLir::Div => "div",
        BinOpLir::Rem => "rem",
        BinOpLir::And => "and",
        BinOpLir::Or => "or",
        BinOpLir::Xor => "xor",
        BinOpLir::Shl => "shl",
        BinOpLir::Shr => "shr",
        BinOpLir::Eq => "eq",
        BinOpLir::Ne => "ne",
        BinOpLir::Lt => "lt",
        BinOpLir::Gt => "gt",
        BinOpLir::Le => "le",
        BinOpLir::Ge => "ge",
        BinOpLir::LAnd => "land",
        BinOpLir::LOr => "lor",
    }
}

fn mlir_un(op: UnOpLir) -> &'static str {
    match op {
        UnOpLir::Neg => "neg",
        UnOpLir::Not => "not",
        UnOpLir::BitNot => "bitnot",
    }
}

fn mlir_call_target(t: &CallTarget) -> String {
    match t {
        CallTarget::Function(id) => format!("@f{id}"),
        CallTarget::External(id) => format!("@z{id}"),
        CallTarget::Indirect(e) => format!("indirect {}", mlir_expr_inline(e)),
    }
}

fn mlir_pat(p: &LirPat) -> String {
    match p {
        LirPat::Wild => "_".into(),
        LirPat::Bind { code, rep } => format!("%{code}:{}", mlir_rep(rep)),
        LirPat::Int(v) => v.to_string(),
        LirPat::Str(s) => format!("str {}", mlir_bytes(s)),
        LirPat::Ctor { ctor, args } => {
            if args.is_empty() {
                format!("@k{ctor}")
            } else {
                format!(
                    "@k{ctor}({})",
                    args.iter().map(mlir_pat).collect::<Vec<_>>().join(", ")
                )
            }
        }
        LirPat::Tuple(xs) => format!(
            "({})",
            xs.iter().map(mlir_pat).collect::<Vec<_>>().join(", ")
        ),
    }
}

fn mlir_expr_inline(e: &LirExpr) -> String {
    match e {
        LirExpr::Unit => "unit".into(),
        LirExpr::Int(v, r) => format!("const.{} {}", mlir_rep(r), v),
        LirExpr::Float(v, r) => format!("const.{} {:.17}", mlir_rep(r), v),
        LirExpr::Str(s) => format!("const.str {}", mlir_bytes(s)),
        LirExpr::Var(n, r) => format!("%{n}:{}", mlir_rep(r)),
        LirExpr::FnRef(id) => format!("@f{id}"),
        LirExpr::Call {
            target,
            args,
            rep,
            pure,
            noreturn,
        } => {
            let args = args
                .iter()
                .map(mlir_expr_inline)
                .collect::<Vec<_>>()
                .join(", ");
            let mut attrs = Vec::new();
            if *pure {
                attrs.push("pure");
            }
            if *noreturn {
                attrs.push("noreturn");
            }
            let suffix = if attrs.is_empty() {
                String::new()
            } else {
                format!(" [{}]", attrs.join(", "))
            };
            format!(
                "call {}({}) : {}{}",
                mlir_call_target(target),
                args,
                mlir_rep(rep),
                suffix
            )
        }
        LirExpr::Construct { ctor, args, rep } => format!(
            "make @{}({}) : {}",
            mangle_ident(&format!("k{ctor}")),
            args.iter()
                .map(mlir_expr_inline)
                .collect::<Vec<_>>()
                .join(", "),
            mlir_rep(rep)
        ),
        LirExpr::Field {
            base,
            ctor,
            index,
            rep,
        } => format!(
            "field @k{ctor}[{index}] {} : {}",
            mlir_expr_inline(base),
            mlir_rep(rep)
        ),
        LirExpr::Let {
            code,
            rep,
            value,
            body,
        } => format!(
            "let %{code}:{} = {} in {}",
            mlir_rep(rep),
            mlir_expr_inline(value),
            mlir_expr_inline(body)
        ),
        LirExpr::Block { stmts, tail } => {
            let mut s = String::from("block { ");
            for x in stmts {
                s.push_str(&mlir_expr_inline(x));
                s.push_str("; ");
            }
            s.push_str("yield ");
            s.push_str(&mlir_expr_inline(tail));
            s.push_str(" }");
            s
        }
        LirExpr::Case { scrut, arms, rep } => {
            let mut s = format!("case {} : {} {{ ", mlir_expr_inline(scrut), mlir_rep(rep));
            for arm in arms {
                s.push_str(&mlir_pat(&arm.pat));
                s.push_str(" => ");
                s.push_str(&mlir_expr_inline(&arm.body));
                s.push_str("; ");
            }
            s.push('}');
            s
        }
        LirExpr::Bin {
            op,
            left,
            right,
            rep,
        } => format!(
            "bin.{} {} {} : {}",
            mlir_bin(*op),
            mlir_expr_inline(left),
            mlir_expr_inline(right),
            mlir_rep(rep)
        ),
        LirExpr::Un { op, expr, rep } => format!(
            "un.{} {} : {}",
            mlir_un(*op),
            mlir_expr_inline(expr),
            mlir_rep(rep)
        ),
        LirExpr::RawAst(e, _, _) => format!("raw-ast {}", expr_debug(e)),
        LirExpr::RuntimeCall { name, rep, .. } => {
            format!("runtime-call {} : {}", name, mlir_rep(rep))
        }
        LirExpr::RuntimeConstruct { name, rep, .. } => {
            format!("runtime-ctor {} : {}", name, mlir_rep(rep))
        }
        LirExpr::DirectC { rep, .. } => format!("direct-c : {}", mlir_rep(rep)),
        LirExpr::Fallback => "fallback".into(),
    }
}

fn render_mlir_expr(out: &mut String, e: &LirExpr, depth: usize) {
    let pad = "  ".repeat(depth);
    match e {
        LirExpr::Block { stmts, tail } => {
            writeln!(out, "{pad}block {{").unwrap();
            for stmt in stmts {
                render_mlir_expr(out, stmt, depth + 1);
            }
            writeln!(
                out,
                "{}yield {}",
                "  ".repeat(depth + 1),
                mlir_expr_inline(tail)
            )
            .unwrap();
            writeln!(out, "{pad}}}").unwrap();
        }
        LirExpr::Case { scrut, arms, rep } => {
            writeln!(
                out,
                "{pad}case {} : {} {{",
                mlir_expr_inline(scrut),
                mlir_rep(rep)
            )
            .unwrap();
            for arm in arms {
                writeln!(
                    out,
                    "{}{} => {}",
                    "  ".repeat(depth + 1),
                    mlir_pat(&arm.pat),
                    mlir_expr_inline(&arm.body)
                )
                .unwrap();
            }
            writeln!(out, "{pad}}}").unwrap();
        }
        LirExpr::Let {
            code,
            rep,
            value,
            body,
        } => {
            writeln!(
                out,
                "{pad}let %{code}:{} = {}",
                mlir_rep(rep),
                mlir_expr_inline(value)
            )
            .unwrap();
            render_mlir_expr(out, body, depth);
        }
        _ => writeln!(out, "{pad}yield {}", mlir_expr_inline(e)).unwrap(),
    }
}

fn c_path(input: &str) -> PathBuf {
    let p = Path::new(input);
    if p.extension().is_some() {
        p.with_extension("c")
    } else {
        let mut s = p.as_os_str().to_os_string();
        s.push(".c");
        PathBuf::from(s)
    }
}

fn subst_ast(e: Expr, name: &str, to: &Expr) -> Expr {
    let span = e.span;
    let kind = e.kind;
    let new_kind = match kind {
        ExprKind::Var(n) if n == name => return to.clone(),
        ExprKind::Let {
            name: n,
            value,
            body,
        } => {
            let new_value = subst_ast(*value, name, to);
            if n == name {
                ExprKind::Let {
                    name: n,
                    value: Box::new(new_value),
                    body,
                }
            } else {
                let new_body = subst_ast(*body, name, to);
                ExprKind::Let {
                    name: n,
                    value: Box::new(new_value),
                    body: Box::new(new_body),
                }
            }
        }
        ExprKind::Fn { params, body } => {
            if params.iter().any(|p| p == name) {
                ExprKind::Fn { params, body }
            } else {
                let new_body = subst_ast(*body, name, to);
                ExprKind::Fn {
                    params,
                    body: Box::new(new_body),
                }
            }
        }
        ExprKind::App { fun, args } => {
            let new_fun = subst_ast(*fun, name, to);
            let new_args = args.into_iter().map(|a| subst_ast(a, name, to)).collect();
            ExprKind::App {
                fun: Box::new(new_fun),
                args: new_args,
            }
        }
        ExprKind::Case { scrut, arms } => {
            let new_scrut = subst_ast(*scrut, name, to);
            let new_arms = arms
                .into_iter()
                .map(|arm| {
                    if pattern_binds_name(&arm.pat, name) {
                        arm
                    } else {
                        Arm {
                            pat: arm.pat,
                            body: subst_ast(arm.body, name, to),
                            span: arm.span,
                        }
                    }
                })
                .collect();
            ExprKind::Case {
                scrut: Box::new(new_scrut),
                arms: new_arms,
            }
        }
        ExprKind::Block { stmts, tail } => {
            let new_stmts = stmts.into_iter().map(|s| subst_ast(s, name, to)).collect();
            let new_tail = subst_ast(*tail, name, to);
            ExprKind::Block {
                stmts: new_stmts,
                tail: Box::new(new_tail),
            }
        }
        ExprKind::Bin { op, l, r } => {
            let new_l = subst_ast(*l, name, to);
            let new_r = subst_ast(*r, name, to);
            ExprKind::Bin {
                op,
                l: Box::new(new_l),
                r: Box::new(new_r),
            }
        }
        ExprKind::Un { op, e: inner } => {
            let new_inner = subst_ast(*inner, name, to);
            ExprKind::Un {
                op,
                e: Box::new(new_inner),
            }
        }
        k => k,
    };
    Expr {
        span,
        kind: new_kind,
    }
}

fn pattern_binds_name(p: &Pat, name: &str) -> bool {
    match p {
        Pat::Bind(n) => n == name,
        Pat::Ctor { args, .. } => args.iter().any(|a| pattern_binds_name(a, name)),
        Pat::Tuple(xs) => xs.iter().any(|x| pattern_binds_name(x, name)),
        _ => false,
    }
}

pub(crate) fn validate_program(program: &Program) -> bool {
    let mut a = Analyzer::new();
    a.register(program);
    a.solve();
    if a.functions
        .iter()
        .any(|f| matches!(f.ret_rep, Rep::Unknown))
    {
        return false;
    }
    if a.constructors
        .iter()
        .any(|c| c.fields.iter().any(|r| matches!(r, Rep::Unknown)))
    {
        return false;
    }
    let mut lowerer = Lowerer::new(&a);
    let lir = lowerer.lower_all();
    !lir.iter().any(|f| lir_contains_fallback(&f.body))
}

#[cfg(test)]
mod pointer_regressions {
    use super::*;
    use crate::parser::Parser;

    fn generated(source: &[u8]) -> String {
        let program = Parser::parse(source).expect("regression program should parse");
        generate(&program)
    }

    #[test]
    fn typed_multilevel_indexing_emits_pointee_scaled_loads() {
        let c = generated(
            b"let extern matrix : [i32] = \"matrix\" : i32**; matrix[0][1]",
        );
        assert!(c.contains("int32_t *"), "inner pointer type should be retained");
        assert!(c.contains("int32_t * *)(uintptr_t)"), "outer indexing should step over i32* elements");
        assert!(c.contains("int32_t *)(uintptr_t)"), "inner indexing should step over i32 elements");
        assert!(!c.contains("$mak.index"), "the parser intrinsic must not leak into C");
    }

    #[test]
    fn indexing_infers_pointer_representation_for_unknown_parameters() {


        let c = generated(b"let read_byte fmt i = fmt[i]; read_byte \"x\" 0");
        assert!(
            c.contains("uint8_t *)(uintptr_t)(a0)"),
            "an unknown indexed parameter should lower to an actual byte load"
        );
        assert!(
            !c.contains("mak_ir_fail_u64(\"invalid index expression\")"),
            "valid pointer indexing must not lower to runtime recovery"
        );
    }

    #[test]
    fn indexing_does_not_keep_a_stale_adt_representation() {
        let c = generated(
            b"type Bytes { Empty; Cons i32 Bytes; }; let chars s i acc = case i < 0 of { 1 -> acc; _ -> chars s (i - 1) (Cons (s[i]) acc) }; chars \"abc\" 2 Empty",
        );
        assert!(
            c.contains("uint8_t *)(uintptr_t)(a0)"),
            "indexing should compile to a byte load from the pointer parameter"
        );
        assert!(
            !c.contains("string used as ADT"),
            "a string passed to an indexed parameter must not fall back to a default ADT"
        );
    }

    #[test]
    fn recursive_string_indexing_overrides_stale_adt_parameter_shape() {
        let c = generated(
            b"type Bytes { Empty; Cons i32 Bytes; }; let chars s i acc = case i < 0 of { 1 -> acc; _ -> chars s (i - 1) (Cons (s[i]) acc) }; chars \"abc\" 2 Empty",
        );
        assert!(
            c.contains("const void * a0") || c.contains("const char * a0"),
            "recursive index bases must be emitted as pointers, not ADT pointers"
        );
        assert!(!c.contains("string used as ADT"));
    }

    #[test]
    fn adt_let_bindings_use_managed_storage() {
        let c = generated(
            b"type List { Nil; Cons i64 List; }; let build x = let child = Cons x Nil in Cons x child; build 7",
        );
        assert!(
            c.contains("mak_ir_arena_alloc(sizeof(struct mak__a)"),
            "ADT let bindings that can escape must use managed storage"
        );
        assert!(
            !c.contains("stackobj_"),
            "ADT let bindings should not point into a returning function's stack"
        );
    }

    #[test]
    fn typed_pointer_addition_uses_element_stride() {
        let c = generated(b"let extern values : [i32] = \"values\" : i32*; values + 2");
        assert!(c.contains("int32_t *)(uintptr_t)"));
        assert!(c.contains("intptr_t)(2)"));
    }

    #[test]
    fn typed_pointer_difference_is_in_elements_not_bytes() {
        let c = generated(
            b"let extern end : [i32] = \"end\" : i32*; let extern start : [i32] = \"start\" : i32*; end - start",
        );
        assert!(c.contains("((int64_t)(((int32_t *)(uintptr_t)("),
            "typed pointer subtraction should emit C pointer difference");
    }
}

#[cfg(test)]
mod memory_lifetime_regressions {
    use super::*;

    #[test]
    fn returned_adt_temporaries_remain_managed() {
        let plan = MemoryPlan::default();
        for kind in [
            MemorySiteKind::Construct,
            MemorySiteKind::CallResult,
            MemorySiteKind::CaseResult,
        ] {
            assert_eq!(
                plan.strategy(kind, Some(7), 64),
                MemoryStrategy::Arena,
                "expression results may escape their creating function"
            );
        }
    }

    #[test]
    fn local_adt_field_reads_are_non_escaping() {
        let field = LirExpr::Field {
            base: Box::new(LirExpr::Var("local".into(), Rep::Adt("Pair".into()))),
            ctor: 0,
            index: 0,
            rep: Rep::I64,
        };
        assert!(memory_local_let_uses_are_non_escaping(&field, "local"));
    }

    #[test]
    fn returning_a_local_adt_pointer_is_an_escape() {
        let returned = LirExpr::Var("local".into(), Rep::Adt("Pair".into()));
        assert!(!memory_local_let_uses_are_non_escaping(&returned, "local"));
    }

    #[test]
    fn embedding_a_local_adt_in_another_value_is_an_escape() {
        let parent = LirExpr::Construct {
            ctor: 1,
            args: vec![LirExpr::Var("local".into(), Rep::Adt("Pair".into()))],
            rep: Rep::Adt("Tree".into()),
        };
        assert!(!memory_local_let_uses_are_non_escaping(&parent, "local"));
    }
}

#[cfg(test)]
mod codegen_regressions {
    use super::*;
    use crate::parser::Parser;

    fn generated(source: &[u8]) -> String {
        let program = Parser::parse(source).expect("regression program should parse");
        generate(&program)
    }

    #[test]
    fn remainder_uses_integer_operands_when_shared_params_are_floats() {


        let c = generated(
            b"let rem = fn (x y) => { x + 0.5; x % y }; rem 17.5 5.0",
        );
        assert!(
            c.contains("mak_ir_i64_rem_wrap((int64_t)("),
            "remainder should use the signed i64 helper with integer-cast operands"
        );
        assert!(
            !c.contains("% (double)("),
            "C's `%` operator must never receive double operands"
        );
    }

    #[test]
    fn scalar_expression_recovers_to_adt_value_not_adt_pointer_assignment() {
        let analyzer = Analyzer::new();
        let mut emitter = CEmitter::new(&analyzer);
        emitter.emit_into(
            &LirExpr::Int(1, Rep::I64),
            "out",
            &Rep::Adt("sample".into()),
            1,
        );
        assert!(emitter.out.contains("mak_ir_recover(\"non-ADT expression used as ADT result\")"));
        assert!(emitter.out.contains("*(out) = *(&mak_ir_default_obj_sample);"));
        assert!(!emitter.out.contains("*(out) = (struct mak_sample *)"));
    }

    #[test]
    fn direct_c_scalar_recovers_to_adt_value_not_adt_pointer_assignment() {
        let analyzer = Analyzer::new();
        let mut emitter = CEmitter::new(&analyzer);
        emitter.emit_into(
            &LirExpr::DirectC {
                code: "some_integer()".into(),
                rep: Rep::I64,
            },
            "out",
            &Rep::Adt("sample".into()),
            1,
        );
        assert!(emitter.out.contains("(void)(some_integer());"));
        assert!(emitter.out.contains("*(out) = *(&mak_ir_default_obj_sample);"));
        assert!(!emitter.out.contains("*(out) = (struct mak_sample *)"));
    }

    #[test]
    fn signed_i64_overflow_uses_defined_wrapping_helpers() {
        let analyzer = Analyzer::new();
        let mut emitter = CEmitter::new(&analyzer);
        let code = emitter.emit_bin(
            BinOpLir::Add,
            &LirExpr::Var("a".into(), Rep::I64),
            &LirExpr::Var("b".into(), Rep::I64),
            &Rep::I64,
            1,
        );
        assert_eq!(code, "mak_ir_i64_add_wrap(a, b)");

        let c = generated(b"9223372036854775807 + 1");
        assert!(c.contains("mak_ir_i64_add_wrap"));
        assert!(!c.contains("(INT64_C(9223372036854775807) + INT64_C(1))"));
    }

    #[test]
    fn integer_remainder_emitter_casts_float_representations() {
        let analyzer = Analyzer::new();
        let mut emitter = CEmitter::new(&analyzer);
        let code = emitter.emit_bin(
            BinOpLir::Rem,
            &LirExpr::Var("a0".into(), Rep::F64),
            &LirExpr::Var("a1".into(), Rep::F64),
            &Rep::I64,
            1,
        );
        assert_eq!(code, "mak_ir_i64_rem_wrap((int64_t)(a0), (int64_t)(a1))");
    }
}
