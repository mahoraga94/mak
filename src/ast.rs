use crate::lexer::Span;

#[derive(Clone, Debug)]
pub struct Program {
    pub decls: Vec<Decl>,
    pub body: Box<Expr>,
}

#[derive(Clone, Debug)]
pub enum Decl {
    Type {
        name: String,
        params: Vec<String>,
        ctors: Vec<Ctor>,
        span: Span,
    },
    Let {
        name: String,
        value: Box<Expr>,
        span: Span,
    },
    External(Extern),
    Bring(Vec<String>),
}

#[derive(Clone, Debug)]
pub struct Ctor {
    pub name: String,
    pub args: Vec<Ty>,
    pub span: Span,
}

#[derive(Clone, Debug)]
pub struct Extern {
    pub name: String,
    pub mak_ty: Ty,
    pub symbol: String,
    pub is_global: bool,
    pub foreign: ForeignTy,
    pub pure: bool,
    pub from: Option<String>,
    pub abi: Option<String>,
    pub span: Span,
}

#[derive(Clone, Debug)]
pub enum Ty {
    Prim(Prim),
    Named(String),
    App(String, Vec<Ty>),
    Array(Box<Ty>),
    Tuple(Vec<Ty>),
    Fun(Box<Ty>, Box<Ty>),
    Bottom,
}

#[derive(Clone, Copy, Debug, PartialEq, Eq)]
pub enum Prim {
    U8,
    U16,
    U32,
    U64,
    I8,
    I16,
    I32,
    I64,
    F32,
    F64,
    Unit,
}

#[derive(Clone, Debug)]
pub enum ForeignTy {
    Atom(ForeignAtom),
    Fun {
        args: Vec<ForeignArg>,
        ret: Box<ForeignTy>,
    },
}

#[derive(Clone, Debug)]
pub enum ForeignArg {
    Ty(ForeignTy),
    VarArgs,
}

#[derive(Clone, Debug)]
pub enum ForeignAtom {
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
    Ptr,
    Void,
    Named(String),
    PtrTo(Box<ForeignTy>),
}

#[derive(Clone, Debug)]
pub struct Expr {
    pub kind: ExprKind,
    pub span: Span,
}

#[derive(Clone, Debug)]
pub enum ExprKind {
    Int(u64),
    Float(f64),
    Str(Vec<u8>),
    Var(String),
    Let {
        name: String,
        value: Box<Expr>,
        body: Box<Expr>,
    },
    Fn {
        params: Vec<String>,
        body: Box<Expr>,
    },
    App {
        fun: Box<Expr>,
        args: Vec<Expr>,
    },
    Case {
        scrut: Box<Expr>,
        arms: Vec<Arm>,
    },
    Block {
        stmts: Vec<Expr>,
        tail: Box<Expr>,
    },
    Bin {
        op: BinOp,
        l: Box<Expr>,
        r: Box<Expr>,
    },
    Un {
        op: UnOp,
        e: Box<Expr>,
    },
    Quote(Box<Expr>),
}

#[derive(Clone, Debug)]
pub struct Arm {
    pub pat: Pat,
    pub body: Expr,
    pub span: Span,
}

#[derive(Clone, Debug)]
pub enum Pat {
    Wild,
    Bind(String),
    Int(u64),
    Str(Vec<u8>),
    Ctor { name: String, args: Vec<Pat> },
    Tuple(Vec<Pat>),
}

#[derive(Clone, Copy, Debug, PartialEq, Eq)]
pub enum BinOp {
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
pub enum UnOp {
    Neg,
    Not,
    BitNot,
}
