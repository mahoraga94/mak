
use crate::ast::*;
use crate::lexer::{Kind, Kw, Lexer, P, Span, Tok};

pub struct Parser<'a> {
    src: &'a [u8],
    toks: Vec<Tok>,
    pos: usize,
    errors: Vec<ParseErr>,
}

#[derive(Debug, Clone)]
pub struct ParseErr {
    pub msg: String,
    pub span: Span,
    pub note: Option<String>,
}

pub type PResult<T> = Result<T, ParseErr>;

impl<'a> Parser<'a> {
    pub fn parse(src: &'a [u8]) -> Result<Program, Vec<ParseErr>> {
        let mut toks = Vec::new();
        Lexer::tokenize_all(src, &mut toks);

        if let Some(bad) = toks.iter().find(|t| matches!(t.kind, Kind::Err(_))) {
            let msg = match bad.kind {
                Kind::Err(crate::lexer::LexErr::UnexpectedByte(b)) => {
                    format!("unexpected byte `{}` (0x{:02x})", printable(b), b)
                }
                Kind::Err(crate::lexer::LexErr::UnterminatedStr) => {
                    "unterminated string literal".into()
                }
                Kind::Err(crate::lexer::LexErr::UnterminatedComment) => {
                    "unterminated block comment".into()
                }
                _ => "lexer error".into(),
            };
            return Err(vec![ParseErr {
                msg,
                span: bad.span,
                note: None,
            }]);
        }

        let mut p = Self {
            src,
            toks,
            pos: 0,
            errors: Vec::new(),
        };
        let mut decls = Vec::new();

        while p.is_decl_start() {
            match p.decl() {
                Ok(d) => decls.push(d),
                Err(e) => {
                    p.errors.push(e);
                    p.recover_decl();
                }
            }
        }

        let body = if matches!(p.cur().kind, Kind::Eof) {
            Box::new(Expr {
                span: Span { lo: 0, hi: 0 },
                kind: ExprKind::Int(0),
            })
        } else {
            match p.expr() {
                Ok(e) => Box::new(e),
                Err(e) => {
                    p.errors.push(e);
                    p.recover_decl();
                    Box::new(Expr {
                        span: Span { lo: 0, hi: 0 },
                        kind: ExprKind::Int(0),
                    })
                }
            }
        };

        if !matches!(p.cur().kind, Kind::Eof) {
            p.errors.push(ParseErr {
                msg: format!("expected end of input, found {}", describe(&p.cur())),
                span: p.cur().span,
                note: Some("remove the trailing tokens or wrap them in a `let` binding".into()),
            });
        }

        if p.errors.is_empty() {
            Ok(Program { decls, body })
        } else {
            Err(p.errors)
        }
    }

    fn recover_decl(&mut self) {
        let mut depth: i32 = 0;
        while !matches!(self.cur().kind, Kind::Eof) {
            match self.cur().kind {
                Kind::P(P::LBrace) | Kind::P(P::LParen) | Kind::P(P::LBrack) => depth += 1,
                Kind::P(P::RBrace) | Kind::P(P::RParen) | Kind::P(P::RBrack) => {
                    depth -= 1;
                    if depth < 0 {
                        self.bump();
                        return;
                    }
                }
                Kind::P(P::Semi) if depth == 0 => {
                    self.bump();
                    return;
                }
                Kind::Kw(Kw::Let) | Kind::Kw(Kw::Type) | Kind::Kw(Kw::Bring) if depth == 0 => {
                    return;
                }
                _ => {}
            }
            self.bump();
        }
    }

    fn recover_block(&mut self) {
        let mut depth: i32 = 0;
        while !matches!(self.cur().kind, Kind::Eof) {
            match self.cur().kind {
                Kind::P(P::LBrace) | Kind::P(P::LParen) | Kind::P(P::LBrack) => depth += 1,
                Kind::P(P::RBrace) if depth == 0 => return,
                Kind::P(P::RBrace) | Kind::P(P::RParen) | Kind::P(P::RBrack) => {
                    depth -= 1;
                    if depth < 0 {
                        return;
                    }
                }
                Kind::P(P::Semi) if depth == 0 => {
                    self.bump();
                    if matches!(self.cur().kind, Kind::P(P::RBrace)) {
                        return;
                    }
                    continue;
                }
                _ => {}
            }
            self.bump();
        }
    }

    #[inline]
    fn cur(&self) -> Tok {
        self.toks[self.pos]
    }

    #[inline]
    fn peek(&self, n: usize) -> Tok {
        self.toks
            .get(self.pos + n)
            .copied()
            .unwrap_or_else(|| *self.toks.last().unwrap())
    }

    #[inline]
    fn bump(&mut self) -> Tok {
        let t = self.toks[self.pos];
        if self.pos + 1 < self.toks.len() {
            self.pos += 1;
        }
        t
    }

    #[inline]
    fn eat_p(&mut self, p: P) -> Option<Span> {
        if let Kind::P(q) = self.cur().kind {
            if q == p {
                return Some(self.bump().span);
            }
        }
        None
    }

    #[inline]
    fn eat_kw(&mut self, k: Kw) -> Option<Span> {
        if let Kind::Kw(q) = self.cur().kind {
            if q == k {
                return Some(self.bump().span);
            }
        }
        None
    }

    #[cold]
    fn err_expect(&self, what: &str) -> ParseErr {
        ParseErr {
            msg: format!("expected {}, found {}", what, describe(&self.cur())),
            span: self.cur().span,
            note: None,
        }
    }

    #[cold]
    fn err_expect_note(&self, what: &str, note: &str) -> ParseErr {
        ParseErr {
            msg: format!("expected {}, found {}", what, describe(&self.cur())),
            span: self.cur().span,
            note: Some(note.into()),
        }
    }

    #[inline]
    fn expect_p(&mut self, p: P, what: &str) -> PResult<Span> {
        self.eat_p(p).ok_or_else(|| self.err_expect(what))
    }

    #[inline]
    fn expect_kw(&mut self, k: Kw, what: &str) -> PResult<Span> {
        self.eat_kw(k).ok_or_else(|| self.err_expect(what))
    }

    #[inline]
    fn eat_expr_arrow(&mut self) -> Option<Span> {
        if let Some(span) = self.eat_p(P::Arrow) {
            return Some(span);
        }
        self.eat_p(P::FatArrow)
    }

    #[inline]
    fn expect_expr_arrow(&mut self, what: &str) -> PResult<Span> {
        self.eat_expr_arrow().ok_or_else(|| self.err_expect(what))
    }

    fn ident(&mut self) -> PResult<String> {
        if let Kind::Ident = self.cur().kind {
            let sp = self.bump().span;
            Ok(self.slice(sp).to_owned())
        } else {
            Err(self.err_expect("an identifier"))
        }
    }

    #[inline]
    fn slice(&self, s: Span) -> &str {
        unsafe { std::str::from_utf8_unchecked(&self.src[s.lo as usize..s.hi as usize]) }
    }

    fn is_soft_kw(&self, pos: usize, name: &[u8]) -> bool {
        if let Some(t) = self.toks.get(pos) {
            if let Kind::Ident = t.kind {
                return &self.src[t.span.lo as usize..t.span.hi as usize] == name;
            }
        }
        false
    }

    fn is_decl_start(&self) -> bool {
        matches!(
            self.cur().kind,
            Kind::Kw(Kw::Let) | Kind::Kw(Kw::Type) | Kind::Kw(Kw::Bring)
        )
    }

    fn decl(&mut self) -> PResult<Decl> {
        match self.cur().kind {
            Kind::Kw(Kw::Type) => self.decl_type(),
            Kind::Kw(Kw::Let) => self.decl_let(),
            Kind::Kw(Kw::Bring) => self.decl_bring(),
            _ => Err(self.err_expect("a top-level `let`, `type`, or `bring` declaration")),
        }
    }

    fn decl_bring(&mut self) -> PResult<Decl> {
        // Consume `bring`. The span is not needed by Decl::Bring.
        self.bump();

        let mut paths = Vec::new();
        while matches!(self.cur().kind, Kind::Str | Kind::StrEsc) {
            let sp = self.bump().span;
            let raw = &self.src[sp.lo as usize..sp.hi as usize];
            paths.push(String::from_utf8_lossy(&decode_string(raw)).into_owned());
        }

        if paths.is_empty() {
            return Err(self.err_expect("at least one string literal after `bring`"));
        }

        self.expect_p(P::Semi, "`;` after `bring`")?;
        Ok(Decl::Bring(paths))
    }

    fn decl_let(&mut self) -> PResult<Decl> {
        let start = self.bump().span;

        if matches!(self.cur().kind, Kind::Kw(Kw::Extern))
            || self.is_soft_kw(self.pos, b"external")
        {
            self.bump();
            return Ok(Decl::External(self.decl_external_body(start)?));
        }

        let name = self.ident()?;
        let mut params = Vec::new();

        while !matches!(self.cur().kind, Kind::P(P::Eq)) {
            if matches!(self.cur().kind, Kind::Ident) {
                params.push(self.ident()?);
            } else if matches!(self.cur().kind, Kind::Eof) {
                return Err(ParseErr {
                    msg: "unexpected end of input inside `let`".into(),
                    span: self.cur().span,
                    note: Some(format!("`{name}` needs a value: `let {name} = ...;`")),
                });
            } else {
                return Err(self.err_expect_note(
                    "`=` after the binding name",
                    "a top-level `let` takes the form `let name = value;`",
                ));
            }
        }

        self.expect_p(P::Eq, "`=` in `let`")?;
        let mut value = self.expr()?;

        if !params.is_empty() {
            let sp = value.span;
            value = Expr {
                span: sp,
                kind: ExprKind::Fn {
                    params,
                    body: Box::new(value),
                },
            };
        }

        self.expect_p(P::Semi, "`;` after `let`")?;
        Ok(Decl::Let {
            name,
            value: Box::new(value),
            span: start,
        })
    }

    fn decl_type(&mut self) -> PResult<Decl> {
        let span = self.bump().span;
        let name = self.ident()?;
        let mut params = Vec::new();

        while let Kind::Ident = self.cur().kind {
            params.push(self.ident()?);
        }

        if self.eat_p(P::Semi).is_some() {
            return Ok(Decl::Type {
                name,
                params,
                ctors: vec![],
                span,
            });
        }

        self.expect_p(P::LBrace, "`{` to open type body")?;
        let mut ctors = Vec::new();

        while self.cur().kind != Kind::P(P::RBrace) {
            if matches!(self.cur().kind, Kind::Eof) {
                return Err(ParseErr {
                    msg: format!("unterminated body of type `{name}`"),
                    span: self.cur().span,
                    note: Some("add a closing `}`".into()),
                });
            }

            let cspan = self.cur().span;
            let cname = self.ident()?;
            let mut args = Vec::new();

            while !matches!(self.cur().kind, Kind::P(P::Semi) | Kind::P(P::RBrace)) {
                args.push(self.ty()?);
            }

            self.eat_p(P::Semi);
            ctors.push(Ctor {
                name: cname,
                args,
                span: cspan,
            });
        }

        self.expect_p(P::RBrace, "`}` to close type body")?;
        self.expect_p(P::Semi, "`;` after type declaration")?;

        Ok(Decl::Type {
            name,
            params,
            ctors,
            span,
        })
    }

    fn decl_external_body(&mut self, start: Span) -> PResult<Extern> {
        let name = self.ident()?;

        let mak_ty = if self.eat_p(P::Colon).is_some() {
            self.ty()?
        } else {
            Ty::Bottom
        };

        self.expect_p(P::Eq, "`=` in external declaration")?;
        let is_global = self.is_soft_kw(self.pos, b"global");

        if is_global {
            self.bump();
        }

        let sym_span = match self.cur().kind {
            Kind::Str | Kind::StrEsc => self.bump().span,
            _ => {
                return Err(self.err_expect_note(
                    "a string literal symbol name",
                    "external declarations use `= \"symbol\"` to name the linked symbol",
                ));
            }
        };

        let symbol = String::from_utf8_lossy(
            &self.src[sym_span.lo as usize..sym_span.hi as usize],
        )
        .into_owned();

        let foreign = if self.eat_p(P::Colon).is_some() {
            self.foreign_ty()?
        } else {
            ForeignTy::Atom(ForeignAtom::Void)
        };

        let mut pure = false;
        let mut from = None;
        let mut abi = None;

        if self.eat_p(P::LBrace).is_some() {
            while self.cur().kind != Kind::P(P::RBrace) {
                if matches!(self.cur().kind, Kind::Eof) {
                    return Err(ParseErr {
                        msg: "unterminated descriptor block".into(),
                        span: self.cur().span,
                        note: Some("add a closing `}`".into()),
                    });
                }

                let key = self.ident()?;
                self.expect_p(P::Eq, "`=` in descriptor entry")?;

                match key.as_str() {
                    "pure" => {
                        let v = self.ident()?;
                        pure = v == "true";
                    }
                    "from" | "abi" => {
                        let sp = match self.cur().kind {
                            Kind::Str | Kind::StrEsc => self.bump().span,
                            _ => {
                                return Err(self.err_expect(
                                    "a string literal in descriptor",
                                ));
                            }
                        };

                        let v = String::from_utf8_lossy(
                            &self.src[sp.lo as usize..sp.hi as usize],
                        )
                        .into_owned();

                        if key == "from" {
                            from = Some(v);
                        } else {
                            abi = Some(v);
                        }
                    }
                    _ => {
                        let _ = self.expr()?;
                    }
                }

                self.eat_p(P::Semi);
            }

            self.expect_p(P::RBrace, "`}` in descriptor")?;
        }

        self.expect_p(P::Semi, "`;` after external declaration")?;

        Ok(Extern {
            name,
            mak_ty,
            symbol,
            is_global,
            foreign,
            pure,
            from,
            abi,
            span: start,
        })
    }

    fn ty(&mut self) -> PResult<Ty> {
        let lhs = self.ty_atom()?;

        if self.eat_p(P::Arrow).is_some() {
            let rhs = self.ty()?;
            return Ok(Ty::Fun(Box::new(lhs), Box::new(rhs)));
        }

        Ok(lhs)
    }

    fn ty_atom(&mut self) -> PResult<Ty> {
        match self.cur().kind {
            Kind::Ident => {
                let s = self.ident()?;

                Ok(match s.as_str() {
                    "u8" => Ty::Prim(Prim::U8),
                    "u16" => Ty::Prim(Prim::U16),
                    "u32" => Ty::Prim(Prim::U32),
                    "u64" => Ty::Prim(Prim::U64),
                    "i8" => Ty::Prim(Prim::I8),
                    "i16" => Ty::Prim(Prim::I16),
                    "i32" => Ty::Prim(Prim::I32),
                    "i64" => Ty::Prim(Prim::I64),
                    "f32" => Ty::Prim(Prim::F32),
                    "f64" => Ty::Prim(Prim::F64),
                    _ => Ty::Named(s),
                })
            }
            Kind::P(P::LBrack) => {
                self.bump();
                let t = self.ty()?;
                self.expect_p(P::RBrack, "`]` to close array type")?;
                Ok(Ty::Array(Box::new(t)))
            }
            Kind::P(P::LParen) => {
                self.bump();

                if self.eat_p(P::RParen).is_some() {
                    return Ok(Ty::Prim(Prim::Unit));
                }

                let first = self.ty()?;

                if self.eat_p(P::Comma).is_some() {
                    let mut ts = vec![first];
                    ts.push(self.ty()?);

                    while self.eat_p(P::Comma).is_some() {
                        ts.push(self.ty()?);
                    }

                    self.expect_p(P::RParen, "`)` to close tuple type")?;
                    return Ok(Ty::Tuple(ts));
                }

                if starts_ty_atom(&self.cur()) {
                    let name = match first {
                        Ty::Named(n) => n,
                        _ => {
                            return Err(ParseErr {
                                msg: "the head of a parenthesized type application must be a type name"
                                    .into(),
                                span: self.cur().span,
                                note: Some("write `(List a)` rather than `([u8] a)`".into()),
                            });
                        }
                    };

                    let mut args = vec![self.ty_atom()?];

                    while starts_ty_atom(&self.cur()) {
                        args.push(self.ty_atom()?);
                    }

                    self.expect_p(P::RParen, "`)` to close type application")?;
                    return Ok(Ty::App(name, args));
                }

                self.expect_p(P::RParen, "`)` to close parenthesized type")?;
                Ok(first)
            }
            Kind::P(P::Bang) => {
                self.bump();
                Ok(Ty::Bottom)
            }
            _ => Err(self.err_expect_note(
                "a type",
                "types are primitives (`u8`, `i64`, ...), names, `[T]`, `(T U)`, or `T -> U`",
            )),
        }
    }

    fn foreign_ty(&mut self) -> PResult<ForeignTy> {
        if self.eat_p(P::LParen).is_some() {
            let mut args = Vec::new();

            while self.cur().kind != Kind::P(P::RParen) {
                if matches!(self.cur().kind, Kind::Eof) {
                    return Err(ParseErr {
                        msg: "unterminated foreign function type".into(),
                        span: self.cur().span,
                        note: Some("add a closing `)` and a `->` return type".into()),
                    });
                }

                if self.cur().kind == Kind::P(P::Dot)
                    && self.peek(1).kind == Kind::P(P::Dot)
                    && self.peek(2).kind == Kind::P(P::Dot)
                {
                    self.bump();
                    self.bump();
                    self.bump();
                    args.push(ForeignArg::VarArgs);
                } else {
                    args.push(ForeignArg::Ty(self.foreign_ty()?));
                }

                if self.eat_p(P::Comma).is_none() {
                    break;
                }
            }

            self.expect_p(P::RParen, "`)` to close foreign arg list")?;
            self.expect_p(P::Arrow, "`->` before foreign return type")?;
            let ret = self.foreign_ty()?;

            return Ok(ForeignTy::Fun {
                args,
                ret: Box::new(ret),
            });
        }

        let atom = match self.cur().kind {
            Kind::Ident => {
                let s = self.ident()?;

                match s.as_str() {
                    "i8" => ForeignAtom::I8,
                    "i16" => ForeignAtom::I16,
                    "i32" => ForeignAtom::I32,
                    "i64" => ForeignAtom::I64,
                    "u8" => ForeignAtom::U8,
                    "u16" => ForeignAtom::U16,
                    "u32" => ForeignAtom::U32,
                    "u64" => ForeignAtom::U64,
                    "f32" => ForeignAtom::F32,
                    "f64" => ForeignAtom::F64,
                    "ptr" => ForeignAtom::Ptr,
                    "void" => ForeignAtom::Void,
                    _ => ForeignAtom::Named(s),
                }
            }
            _ => return Err(self.err_expect("a foreign type atom")),
        };

        // Accept repeated pointer stars (`i32*`, `i32**`, `u8***`).
        // Preserve each pointer layer so codegen can determine the element
        // type at every level of chained indexing.
        let mut ty = ForeignTy::Atom(atom);

        while self.eat_p(P::Star).is_some() {
            ty = ForeignTy::Atom(ForeignAtom::PtrTo(Box::new(ty)));
        }

        Ok(ty)
    }

    pub fn expr(&mut self) -> PResult<Expr> {
        self.expr_bp(0)
    }

    /// Parse prefix operators before entering the binary/application layer.
    ///
    /// The operand is parsed with a binding power above `.` and every binary
    /// operator, while implicit function application remains available.
    fn prefix_expr(&mut self) -> PResult<Expr> {
        let t = self.cur();

        let op = match t.kind {
            Kind::P(P::Minus) => Some(UnOp::Neg),
            Kind::P(P::Bang) => Some(UnOp::Not),
            Kind::P(P::Tilde) => Some(UnOp::BitNot),
            _ => None,
        };

        if let Some(op) = op {
            self.bump();

            // Keep application inside the unary operand, but stop before
            // the dot/composition layer and all binary operators.
            let e = self.expr_bp(10)?;
            let span = Span {
                lo: t.span.lo,
                hi: e.span.hi.max(t.span.hi),
            };

            return Ok(Expr {
                kind: ExprKind::Un {
                    op,
                    e: Box::new(e),
                },
                span,
            });
        }

        self.atom()
    }

    fn expr_bp(&mut self, min_bp: u8) -> PResult<Expr> {
        let mut lhs = self.prefix_expr()?;

        loop {
            // Postfix indexing binds more tightly than implicit application
            // and every infix operator. Lower indexing to a reserved AST
            // intrinsic so a new public AST node is not required.
            if matches!(self.cur().kind, Kind::P(P::LBrack)) {
                lhs = self.index_suffix(lhs)?;
                continue;
            }

            // Preserve the existing empty-call syntax: f().
            if matches!(self.cur().kind, Kind::P(P::LParen))
                && matches!(self.peek(1).kind, Kind::P(P::RParen))
            {
                let open = self.bump().span;
                let close = self.bump().span;
                let span = Span {
                    lo: lhs.span.lo,
                    hi: close.hi.max(open.hi),
                };

                lhs = Expr {
                    span,
                    kind: ExprKind::App {
                        fun: Box::new(lhs),
                        args: Vec::new(),
                    },
                };

                continue;
            }

            if self.starts_atom() {
                // Parse indexing on the argument first, so `f xs[i]`
                // means `f (xs[i])`. Keep the intermediate in a local:
                // nesting self.atom() inside self.index_suffix() would
                // borrow self mutably twice in the same expression.
                let atom = self.atom()?;
                let arg = self.index_suffix(atom)?;

                let span = Span {
                    lo: lhs.span.lo,
                    hi: arg.span.hi,
                };

                lhs = Expr {
                    span,
                    kind: ExprKind::App {
                        fun: Box::new(lhs),
                        args: vec![arg],
                    },
                };

                continue;
            }

            let (op, lbp, rbp): (Option<BinOp>, u8, u8) = match self.cur().kind {
                Kind::P(P::Dot) => (None, 9, 9),
                Kind::P(P::Star) => (Some(BinOp::Mul), 7, 8),
                Kind::P(P::Slash) => (Some(BinOp::Div), 7, 8),
                Kind::P(P::Percent) => (Some(BinOp::Rem), 7, 8),
                Kind::P(P::Plus) => (Some(BinOp::Add), 6, 7),
                Kind::P(P::Minus) => (Some(BinOp::Sub), 6, 7),
                Kind::P(P::Shl) => (Some(BinOp::Shl), 5, 6),
                Kind::P(P::Shr) => (Some(BinOp::Shr), 5, 6),
                Kind::P(P::Amp) => (Some(BinOp::And), 4, 5),
                Kind::P(P::Caret) => (Some(BinOp::Xor), 4, 5),
                Kind::P(P::Pipe) => (Some(BinOp::Or), 4, 5),
                Kind::P(P::EqEq) => (Some(BinOp::Eq), 3, 4),
                Kind::P(P::Ne) => (Some(BinOp::Ne), 3, 4),
                Kind::P(P::Lt) => (Some(BinOp::Lt), 3, 4),
                Kind::P(P::Gt) => (Some(BinOp::Gt), 3, 4),
                Kind::P(P::Le) => (Some(BinOp::Le), 3, 4),
                Kind::P(P::Ge) => (Some(BinOp::Ge), 3, 4),
                Kind::P(P::AndAnd) => (Some(BinOp::LAnd), 2, 3),
                Kind::P(P::OrOr) => (Some(BinOp::LOr), 1, 2),
                _ => break,
            };

            if lbp < min_bp {
                break;
            }

            if op.is_none() {
                self.bump();
                let rhs = self.expr_bp(rbp)?;
                let span = Span {
                    lo: lhs.span.lo,
                    hi: rhs.span.hi,
                };
                let x = "$x".to_string();
                let xvar = || Expr {
                    span,
                    kind: ExprKind::Var(x.clone()),
                };

                lhs = Expr {
                    span,
                    kind: ExprKind::Fn {
                        params: vec![x.clone()],
                        body: Box::new(Expr {
                            span,
                            kind: ExprKind::App {
                                fun: Box::new(lhs),
                                args: vec![Expr {
                                    span,
                                    kind: ExprKind::App {
                                        fun: Box::new(rhs),
                                        args: vec![xvar()],
                                    },
                                }],
                            },
                        }),
                    },
                };

                continue;
            }

            self.bump();
            let rhs = self.expr_bp(rbp)?;
            let span = Span {
                lo: lhs.span.lo,
                hi: rhs.span.hi,
            };

            lhs = Expr {
                span,
                kind: ExprKind::Bin {
                    op: op.unwrap(),
                    l: Box::new(lhs),
                    r: Box::new(rhs),
                },
            };
        }

        Ok(lhs)
    }

    /// Parse postfix indexing suffixes from an already parsed expression.
    /// `base[i][j]` becomes `$mak.index($mak.index(base, i), j)`.
    fn index_suffix(&mut self, mut base: Expr) -> PResult<Expr> {
        while matches!(self.cur().kind, Kind::P(P::LBrack)) {
            let open = self.bump().span;

            if matches!(self.cur().kind, Kind::P(P::RBrack)) {
                return Err(ParseErr {
                    msg: "an index expression cannot be empty".into(),
                    span: self.cur().span,
                    note: Some(
                        "write `value[index]`; standalone `[]` array literals are not supported"
                            .into(),
                    ),
                });
            }

            let index = self.expr()?;
            let close = self.expect_p(P::RBrack, "`]` to close index expression")?;

            let span = Span {
                lo: base.span.lo.min(open.lo),
                hi: close.hi.max(index.span.hi),
            };

            base = Expr {
                span,
                kind: ExprKind::App {
                    fun: Box::new(Expr {
                        span: open,
                        kind: ExprKind::Var("$mak.index".to_string()),
                    }),
                    args: vec![base, index],
                },
            };
        }

        Ok(base)
    }

    fn starts_atom(&self) -> bool {
        matches!(
            self.cur().kind,
            Kind::Ident
                | Kind::Int(_)
                | Kind::Float(_)
                | Kind::Str
                | Kind::StrEsc
                | Kind::P(P::LParen)
                | Kind::P(P::LBrace)
                | Kind::P(P::Backslash)
                | Kind::Kw(Kw::Fn)
                | Kind::Kw(Kw::Let)
                | Kind::Kw(Kw::Case)
        )
    }

    fn atom(&mut self) -> PResult<Expr> {
        let t = self.cur();

        match t.kind {
            Kind::Int(v) => {
                self.bump();
                Ok(Expr {
                    kind: ExprKind::Int(v),
                    span: t.span,
                })
            }
            Kind::Float(v) => {
                self.bump();
                Ok(Expr {
                    kind: ExprKind::Float(v),
                    span: t.span,
                })
            }
            Kind::Str | Kind::StrEsc => {
                self.bump();
                let raw = &self.src[t.span.lo as usize..t.span.hi as usize];

                Ok(Expr {
                    kind: ExprKind::Str(decode_string(raw)),
                    span: t.span,
                })
            }
            Kind::Ident => {
                let name = self.ident()?;
                Ok(Expr {
                    kind: ExprKind::Var(name),
                    span: t.span,
                })
            }
            Kind::P(P::LParen) => {
                self.bump();
                let e = self.expr()?;
                self.expect_p(P::RParen, "`)` to close parenthesized expression")?;
                Ok(e)
            }
            Kind::P(P::LBrack) => Err(ParseErr {
                msg: "unexpected `[`; indexing must follow an expression".into(),
                span: t.span,
                note: Some(
                    "write `value[index]`; standalone `[]` array literals are not supported"
                        .into(),
                ),
            }),
            Kind::P(P::LBrace) => {
                self.bump();
                let kind = self.block_body()?;
                self.expect_p(P::RBrace, "`}` to close block")?;
                Ok(Expr { span: t.span, kind })
            }
            Kind::Kw(Kw::Let) => self.let_expr(),
            Kind::Kw(Kw::Fn) | Kind::P(P::Backslash) => self.fn_expr(),
            Kind::Kw(Kw::Case) => self.case_expr(),
            _ => Err(self.err_expect_note(
                "an expression",
                "expressions are literals, identifiers, `(expr)`, `{...}`, `fn`, `case`, or `let`",
            )),
        }
    }

    fn block_body(&mut self) -> PResult<ExprKind> {
        let mut stmts: Vec<Expr> = Vec::new();

        loop {
            if matches!(self.cur().kind, Kind::P(P::RBrace)) {
                if stmts.is_empty() {
                    return Ok(ExprKind::Int(0));
                }

                let tail = stmts.pop().unwrap();

                if stmts.is_empty() {
                    return Ok(tail.kind);
                }

                return Ok(ExprKind::Block {
                    stmts,
                    tail: Box::new(tail),
                });
            }

            if matches!(self.cur().kind, Kind::Eof) {
                return Err(ParseErr {
                    msg: "unterminated block: missing `}`".into(),
                    span: self.cur().span,
                    note: Some("every `{` needs a matching `}`".into()),
                });
            }

            if matches!(self.cur().kind, Kind::Kw(Kw::Let)) {
                let sp = self.cur().span;
                let let_kind = self.block_let()?;
                let let_expr = Expr {
                    span: sp,
                    kind: let_kind,
                };

                if stmts.is_empty() {
                    return Ok(let_expr.kind);
                }

                return Ok(ExprKind::Block {
                    stmts,
                    tail: Box::new(let_expr),
                });
            }

            let e = match self.expr() {
                Ok(e) => e,
                Err(err) => {
                    self.errors.push(err);
                    self.recover_block();

                    if matches!(self.cur().kind, Kind::P(P::RBrace)) {
                        if stmts.is_empty() {
                            return Ok(ExprKind::Int(0));
                        }

                        let tail = stmts.pop().unwrap();

                        if stmts.is_empty() {
                            return Ok(tail.kind);
                        }

                        return Ok(ExprKind::Block {
                            stmts,
                            tail: Box::new(tail),
                        });
                    }

                    continue;
                }
            };

            if self.eat_p(P::Semi).is_none() {
                if stmts.is_empty() {
                    return Ok(e.kind);
                }

                return Ok(ExprKind::Block {
                    stmts,
                    tail: Box::new(e),
                });
            }

            stmts.push(e);
        }
    }

    fn block_let(&mut self) -> PResult<ExprKind> {
        let sp = self.bump().span;

        if matches!(self.cur().kind, Kind::Kw(Kw::Extern))
            || self.is_soft_kw(self.pos, b"external")
        {
            return Err(ParseErr {
                msg: "`external` declarations may only appear at the top level".into(),
                span: self.cur().span,
                note: Some("move this declaration outside of any function body".into()),
            });
        }

        let name = self.ident()?;
        let mut params = Vec::new();

        while !matches!(self.cur().kind, Kind::P(P::Eq)) {
            match self.cur().kind {
                Kind::Ident => params.push(self.ident()?),
                _ => {
                    return Err(self.err_expect_note(
                        "`=` in `let` binding",
                        "a local binding takes the form `let name = value;`",
                    ));
                }
            }
        }

        self.expect_p(P::Eq, "`=` in `let` binding")?;
        let mut value = self.expr()?;

        if !params.is_empty() {
            let vs = value.span;
            value = Expr {
                span: vs,
                kind: ExprKind::Fn {
                    params,
                    body: Box::new(value),
                },
            };
        }

        self.expect_p(P::Semi, "`;` after `let` binding")?;
        let body_kind = self.block_body()?;
        let body = Expr {
            span: sp,
            kind: body_kind,
        };

        Ok(ExprKind::Let {
            name,
            value: Box::new(value),
            body: Box::new(body),
        })
    }

    fn let_expr(&mut self) -> PResult<Expr> {
        let sp = self.bump().span;
        let name = self.ident()?;
        let mut params = Vec::new();

        while !matches!(self.cur().kind, Kind::P(P::Eq)) {
            match self.cur().kind {
                Kind::Ident => params.push(self.ident()?),
                _ => {
                    return Err(self.err_expect_note(
                        "`=` in `let` binding",
                        "a `let` expression takes the form `let name = value; body`",
                    ));
                }
            }
        }

        self.expect_p(P::Eq, "`=` in `let` binding")?;
        let mut value = self.expr()?;

        if !params.is_empty() {
            let vs = value.span;
            value = Expr {
                span: vs,
                kind: ExprKind::Fn {
                    params,
                    body: Box::new(value),
                },
            };
        }

        self.expect_p(P::Semi, "`;` after `let` binding")?;
        let body = self.expr()?;
        let span = Span {
            lo: sp.lo,
            hi: body.span.hi,
        };

        Ok(Expr {
            span,
            kind: ExprKind::Let {
                name,
                value: Box::new(value),
                body: Box::new(body),
            },
        })
    }

    fn fn_expr(&mut self) -> PResult<Expr> {
        let sp = self.bump().span;
        let mut params = Vec::new();

        if self.eat_p(P::LParen).is_some() {
            while !matches!(self.cur().kind, Kind::P(P::RParen)) {
                if matches!(self.cur().kind, Kind::Eof) {
                    return Err(ParseErr {
                        msg: "unterminated parameter list: missing `)`".into(),
                        span: self.cur().span,
                        note: Some("parameter lists look like `fn (a b c) { ... }`".into()),
                    });
                }

                match self.cur().kind {
                    Kind::Ident => params.push(self.ident()?),
                    _ => {
                        return Err(self.err_expect_note(
                            "a parameter name",
                            "parameters are identifiers, optionally separated by commas",
                        ));
                    }
                }

                self.eat_p(P::Comma);
            }

            self.expect_p(P::RParen, "`)` to close parameter list")?;
        } else {
            while let Kind::Ident = self.cur().kind {
                params.push(self.ident()?);
            }
        }

        let body = if self.eat_expr_arrow().is_some() {
            self.expr()?
        } else {
            let open = match self.eat_p(P::LBrace) {
                Some(s) => s,
                None => {
                    return Err(self.err_expect_note(
                        "`{`, `->`, or `=>` after parameters",
                        "write `fn (a b) { body }`, `fn (a b) -> body`, or `fn (a b) => body`",
                    ));
                }
            };

            let kind = self.block_body()?;
            self.expect_p(P::RBrace, "`}` to close function body")?;
            Expr { span: open, kind }
        };

        let span = Span {
            lo: sp.lo,
            hi: body.span.hi,
        };

        Ok(Expr {
            span,
            kind: ExprKind::Fn {
                params,
                body: Box::new(body),
            },
        })
    }

    fn case_expr(&mut self) -> PResult<Expr> {
        let sp = self.bump().span;
        let scrut = self.expr()?;

        self.expect_kw(Kw::Of, "`of` after `case` scrutinee")?;
        self.expect_p(P::LBrace, "`{` to open case arms")?;

        let mut arms = Vec::new();

        while self.cur().kind != Kind::P(P::RBrace) {
            if matches!(self.cur().kind, Kind::Eof) {
                return Err(ParseErr {
                    msg: "unterminated `case`: missing `}`".into(),
                    span: self.cur().span,
                    note: Some("each arm is `pattern -> body;`".into()),
                });
            }

            let asp = self.cur().span;
            let pat = self.pat()?;
            self.expect_expr_arrow("`->` or `=>` in case arm")?;
            let body = self.expr()?;
            self.eat_p(P::Semi);

            arms.push(Arm {
                pat,
                body,
                span: asp,
            });
        }

        self.expect_p(P::RBrace, "`}` to close case")?;

        let span = Span {
            lo: sp.lo,
            hi: self.cur().span.hi,
        };

        Ok(Expr {
            span,
            kind: ExprKind::Case {
                scrut: Box::new(scrut),
                arms,
            },
        })
    }

    fn pat(&mut self) -> PResult<Pat> {
        match self.cur().kind {
            Kind::Ident => {
                let name = self.ident()?;

                if name == "_" {
                    return Ok(Pat::Wild);
                }

                if name.as_bytes()[0].is_ascii_uppercase() {
                    let mut args = Vec::new();

                    while self.starts_atom() {
                        args.push(self.pat()?);
                    }

                    Ok(Pat::Ctor { name, args })
                } else {
                    Ok(Pat::Bind(name))
                }
            }
            Kind::Int(v) => {
                self.bump();
                Ok(Pat::Int(v))
            }
            Kind::Str | Kind::StrEsc => {
                let sp = self.bump().span;
                let raw = &self.src[sp.lo as usize..sp.hi as usize];
                Ok(Pat::Str(decode_string(raw)))
            }
            Kind::P(P::LParen) => {
                self.bump();

                if self.eat_p(P::RParen).is_some() {
                    return Ok(Pat::Tuple(vec![]));
                }

                let mut ps = vec![self.pat()?];

                while self.eat_p(P::Comma).is_some() {
                    ps.push(self.pat()?);
                }

                self.expect_p(P::RParen, "`)` to close tuple pattern")?;

                Ok(if ps.len() == 1 {
                    ps.pop().unwrap()
                } else {
                    Pat::Tuple(ps)
                })
            }
            _ => Err(self.err_expect_note(
                "a pattern",
                "patterns are `_`, a binding, a literal, `Ctor p1 p2`, or `(p1, p2)`",
            )),
        }
    }
}

fn starts_ty_atom(t: &Tok) -> bool {
    matches!(
        t.kind,
        Kind::Ident | Kind::P(P::LParen) | Kind::P(P::LBrack)
    )
}

fn describe(t: &Tok) -> String {
    match &t.kind {
        Kind::Eof => "end of input".into(),
        Kind::Ident => "an identifier".into(),
        Kind::Int(_) => "an integer".into(),
        Kind::Float(_) => "a float".into(),
        Kind::Str | Kind::StrEsc => "a string".into(),
        Kind::Kw(k) => format!("keyword `{}`", kw_str(*k)),
        Kind::P(p) => format!("`{}`", p_str(*p)),
        Kind::Err(_) => "a lexer error".into(),
    }
}

fn kw_str(k: Kw) -> &'static str {
    match k {
        Kw::Let => "let",
        Kw::Fn => "fn",
        Kw::Case => "case",
        Kw::Type => "type",
        Kw::Extern => "extern",
        Kw::Of => "of",
        Kw::Pass => "pass",
        Kw::Bring => "bring",
    }
}

fn p_str(p: P) -> &'static str {
    match p {
        P::LBrace => "{",
        P::RBrace => "}",
        P::LParen => "(",
        P::RParen => ")",
        P::LBrack => "[",
        P::RBrack => "]",
        P::Semi => ";",
        P::Comma => ",",
        P::Colon => ":",
        P::Dot => ".",
        P::Backslash => "\\",
        P::Arrow => "->",
        P::FatArrow => "=>",
        P::Eq => "=",
        P::Plus => "+",
        P::Minus => "-",
        P::Star => "*",
        P::Slash => "/",
        P::Percent => "%",
        P::Amp => "&",
        P::Pipe => "|",
        P::Caret => "^",
        P::Tilde => "~",
        P::Shl => "<<",
        P::Shr => ">>",
        P::EqEq => "==",
        P::Ne => "/=",
        P::Lt => "<",
        P::Gt => ">",
        P::Le => "<=",
        P::Ge => ">=",
        P::AndAnd => "&&",
        P::OrOr => "||",
        P::Bang => "!",
    }
}

fn printable(b: u8) -> char {
    if b.is_ascii_graphic() {
        b as char
    } else {
        '?'
    }
}

fn decode_string(raw: &[u8]) -> Vec<u8> {
    let mut out = Vec::with_capacity(raw.len());
    let mut i = 0;

    while i < raw.len() {
        let b = raw[i];

        if b == b'\\' && i + 1 < raw.len() {
            i += 1;

            match raw[i] {
                b'n' => out.push(b'\n'),
                b't' => out.push(b'\t'),
                b'r' => out.push(b'\r'),
                b'\\' => out.push(b'\\'),
                b'"' => out.push(b'"'),
                b'\'' => out.push(b'\''),
                b'0' => out.push(0),
                b'x' if i + 2 < raw.len() => {
                    let h = hex(raw[i + 1]);
                    let l = hex(raw[i + 2]);

                    if let (Some(h), Some(l)) = (h, l) {
                        out.push((h << 4) | l);
                        i += 2;
                    } else {
                        out.push(b'x');
                    }
                }
                x => out.push(x),
            }
        } else {
            out.push(b);
        }

        i += 1;
    }

    out
}

#[inline]
fn hex(b: u8) -> Option<u8> {
    match b {
        b'0'..=b'9' => Some(b - b'0'),
        b'a'..=b'f' => Some(b - b'a' + 10),
        b'A'..=b'F' => Some(b - b'A' + 10),
        _ => None,
    }
}

#[cfg(test)]
mod indexing_regressions {
    use super::*;

    fn is_index(e: &Expr) -> bool {
        matches!(
            &e.kind,
            ExprKind::App { fun, args }
                if matches!(&fun.kind, ExprKind::Var(name) if name == "$mak.index")
                    && args.len() == 2
        )
    }

    #[test]
    fn parses_simple_and_chained_postfix_indexing() {
        let program = Parser::parse(b"matrix[row][column]").expect("valid index expression");

        let ExprKind::App { args, .. } = &program.body.kind else {
            panic!("outer index must be an intrinsic application");
        };

        assert_eq!(args.len(), 2);
        assert!(is_index(&args[0]), "the left side should be the inner index");
        assert!(matches!(&args[1].kind, ExprKind::Var(n) if n == "column"));
    }

    #[test]
    fn indexing_binds_tighter_than_implicit_application() {
        let program = Parser::parse(b"f values[index]").expect("valid application");

        let ExprKind::App { fun, args } = &program.body.kind else {
            panic!("expression should be a function application");
        };

        assert!(matches!(&fun.kind, ExprKind::Var(n) if n == "f"));
        assert_eq!(args.len(), 1);
        assert!(is_index(&args[0]));
    }

    #[test]
    fn rejects_empty_index_syntax_instead_of_treating_it_as_zero() {
        assert!(Parser::parse(b"values[]").is_err());
        assert!(Parser::parse(b"[]").is_err());
    }

    #[test]
    fn preserves_repeated_pointer_stars_in_external_types() {
        let program = Parser::parse(
            b"let extern matrix : [i32] = \"matrix\" : i32**; matrix[0][1]",
        )
        .expect("valid typed external pointer declaration");

        let Decl::External(external) = &program.decls[0] else {
            panic!("expected external declaration");
        };

        let ForeignTy::Atom(ForeignAtom::PtrTo(inner)) = &external.foreign else {
            panic!("outer pointer layer was lost");
        };

        assert!(matches!(
            inner.as_ref(),
            ForeignTy::Atom(ForeignAtom::PtrTo(_))
        ));
        assert!(is_index(&program.body));
    }
}
