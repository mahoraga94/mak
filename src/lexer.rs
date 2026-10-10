use std::fmt;

#[derive(Clone, Copy, Debug, PartialEq, Eq)]
#[repr(u8)]
pub enum Kw {
    Let,
    Fn,
    Case,
    Type,
    Extern,
    Of,
    Pass,
    Bring,
}

#[derive(Clone, Copy, Debug, PartialEq, Eq)]
#[repr(u8)]
pub enum P {
    LBrace,
    RBrace,
    LParen,
    RParen,
    LBrack,
    RBrack,
    Semi,
    Comma,
    Colon,
    Dot,
    Backslash,
    Arrow,
    FatArrow,
    Eq,
    Plus,
    Minus,
    Star,
    Slash,
    Percent,
    Amp,
    Pipe,
    Caret,
    Shl,
    Shr,
    EqEq,
    Ne,
    Lt,
    Gt,
    Le,
    Ge,
    AndAnd,
    OrOr,
    Bang,
    Tilde,
}

#[derive(Clone, Copy, Debug, PartialEq, Eq)]
pub enum LexErr {
    UnexpectedByte(u8),
    UnterminatedStr,
    UnterminatedComment,
}

#[derive(Clone, Copy, Debug, PartialEq)]
pub enum Kind {
    Kw(Kw),
    Ident,
    Int(u64),
    Float(f64),
    Str,
    StrEsc,
    P(P),
    Eof,
    Err(LexErr),
}

#[derive(Clone, Copy, Debug, PartialEq, Eq)]
pub struct Span {
    pub lo: u32,
    pub hi: u32,
}

#[derive(Clone, Copy, Debug)]
pub struct Tok {
    pub kind: Kind,
    pub span: Span,
}

impl fmt::Display for Tok {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        write!(f, "{:?}@{}..{}", self.kind, self.span.lo, self.span.hi)
    }
}

const fn ident_start_tbl() -> [bool; 256] {
    let mut t = [false; 256];
    let mut i = 0;
    while i < 256 {
        let b = i as u8;
        t[i] = (b >= b'a' && b <= b'z') || (b >= b'A' && b <= b'Z') || b == b'_';
        i += 1;
    }
    t
}

const fn ident_tbl() -> [bool; 256] {
    let mut t = [false; 256];
    let mut i = 0;
    while i < 256 {
        let b = i as u8;
        t[i] = (b >= b'a' && b <= b'z')
            || (b >= b'A' && b <= b'Z')
            || (b >= b'0' && b <= b'9')
            || b == b'_';
        i += 1;
    }
    t
}

const fn digit_tbl() -> [bool; 256] {
    let mut t = [false; 256];
    let mut i = 0;
    while i < 256 {
        let b = i as u8;
        t[i] = b >= b'0' && b <= b'9';
        i += 1;
    }
    t
}

const fn hex_tbl() -> [u8; 256] {
    let mut t = [0xffu8; 256];
    let mut i = 0;
    while i < 256 {
        let b = i as u8;
        if b >= b'0' && b <= b'9' {
            t[i] = b - b'0';
        } else if b >= b'a' && b <= b'f' {
            t[i] = b - b'a' + 10;
        } else if b >= b'A' && b <= b'F' {
            t[i] = b - b'A' + 10;
        }
        i += 1;
    }
    t
}

static IDENT_START_T: [bool; 256] = ident_start_tbl();
static IDENT_T: [bool; 256] = ident_tbl();
static DIGIT_T: [bool; 256] = digit_tbl();
static HEX_T: [u8; 256] = hex_tbl();

pub struct Lexer<'a> {
    src: &'a [u8],
    end: usize,
    pos: usize,
    done: bool,
}

impl<'a> Lexer<'a> {
    #[inline]
    pub fn new(src: &'a [u8]) -> Self {
        let end = if src.len() >= 2 && src[src.len() - 2] == 0 {
            src.len() - 2
        } else {
            src.len() - 1
        };
        Self {
            src,
            end,
            pos: 0,
            done: false,
        }
    }

    #[inline(always)]
    fn peek(&self) -> u8 {
        unsafe { *self.src.get_unchecked(self.pos) }
    }
    #[inline(always)]
    fn peek2(&self) -> u8 {
        unsafe { *self.src.get_unchecked(self.pos + 1) }
    }
    #[inline(always)]
    fn at(&self, i: usize) -> u8 {
        unsafe { *self.src.get_unchecked(i) }
    }

    pub fn tokenize_all(src: &'a [u8], out: &mut Vec<Tok>) {
        out.clear();
        out.reserve(src.len() / 4 + 16);
        let mut lx = Lexer::new(src);
        loop {
            match lx.next() {
                None => break,
                Some(t) => {
                    let terminal = matches!(t.kind, Kind::Eof | Kind::Err(_));
                    out.push(t);
                    if terminal {
                        break;
                    }
                }
            }
        }
    }

    #[cold]
    fn err(&mut self, e: LexErr) -> Tok {
        let lo = self.pos as u32;
        self.pos = self.end;
        Tok {
            kind: Kind::Err(e),
            span: Span {
                lo,
                hi: self.end as u32,
            },
        }
    }
}

impl<'a> Iterator for Lexer<'a> {
    type Item = Tok;

    #[inline]
    fn next(&mut self) -> Option<Tok> {
        if self.done {
            return None;
        }
        loop {
            let b = self.peek();
            match b {
                0 => {
                    self.done = true;
                    let p = self.pos as u32;
                    return Some(Tok {
                        kind: Kind::Eof,
                        span: Span { lo: p, hi: p },
                    });
                }
                b' ' | b'\t' | b'\r' | b'\n' => {
                    self.pos += 1;
                }
                b'/' if self.peek2() == b'/' => {
                    self.pos += 2;
                    while self.peek() != b'\n' && self.peek() != 0 {
                        self.pos += 1;
                    }
                }
                b'/' if self.peek2() == b'*' => {
                    self.pos += 2;
                    let mut depth: u32 = 1;
                    loop {
                        let c = self.peek();
                        if c == 0 {
                            return Some(self.err(LexErr::UnterminatedComment));
                        }
                        if c == b'*' && self.peek2() == b'/' {
                            self.pos += 2;
                            depth -= 1;
                            if depth == 0 {
                                break;
                            }
                        } else if c == b'/' && self.peek2() == b'*' {
                            self.pos += 2;
                            depth += 1;
                        } else {
                            self.pos += 1;
                        }
                    }
                }
                _ => break,
            }
        }

        let lo = self.pos as u32;
        let b = self.peek();

        if IDENT_START_T[b as usize] {
            self.pos += 1;
            while IDENT_T[self.peek() as usize] {
                self.pos += 1;
            }
            let s = unsafe { self.src.get_unchecked(lo as usize..self.pos) };
            let kind = match s {
                b"let" => Kind::Kw(Kw::Let),
                b"fn" => Kind::Kw(Kw::Fn),
                b"case" => Kind::Kw(Kw::Case),
                b"type" => Kind::Kw(Kw::Type),
                b"extern" => Kind::Kw(Kw::Extern),
                b"of" => Kind::Kw(Kw::Of),
                b"pass" => Kind::Kw(Kw::Pass),
                b"bring" => Kind::Kw(Kw::Bring),
                _ => Kind::Ident,
            };
            return Some(Tok {
                kind,
                span: Span {
                    lo,
                    hi: self.pos as u32,
                },
            });
        }

        if DIGIT_T[b as usize] {
            return Some(self.lex_number(lo));
        }
        if b == b'"' || b == b'\'' {
            return Some(self.lex_string(b));
        }

        let b1 = self.peek2();
        let p: P = match b {
            b'{' => {
                self.pos += 1;
                P::LBrace
            }
            b'}' => {
                self.pos += 1;
                P::RBrace
            }
            b'(' => {
                self.pos += 1;
                P::LParen
            }
            b')' => {
                self.pos += 1;
                P::RParen
            }
            b'[' => {
                self.pos += 1;
                P::LBrack
            }
            b']' => {
                self.pos += 1;
                P::RBrack
            }
            b';' => {
                self.pos += 1;
                P::Semi
            }
            b',' => {
                self.pos += 1;
                P::Comma
            }
            b':' => {
                self.pos += 1;
                P::Colon
            }
            b'.' => {
                self.pos += 1;
                P::Dot
            }
            b'\\' => {
                self.pos += 1;
                P::Backslash
            }
            b'+' => {
                self.pos += 1;
                P::Plus
            }
            b'*' => {
                self.pos += 1;
                P::Star
            }
            b'%' => {
                self.pos += 1;
                P::Percent
            }
            b'^' => {
                self.pos += 1;
                P::Caret
            }
            b'~' => {
                self.pos += 1;
                P::Tilde
            }
            b'!' => {
                self.pos += 1;
                P::Bang
            }
            b'-' if b1 == b'>' => {
                self.pos += 2;
                P::Arrow
            }
            b'-' => {
                self.pos += 1;
                P::Minus
            }
            b'=' if b1 == b'=' => {
                self.pos += 2;
                P::EqEq
            }
            b'=' if b1 == b'>' => {
                self.pos += 2;
                P::FatArrow
            }
            b'=' => {
                self.pos += 1;
                P::Eq
            }
            b'/' if b1 == b'=' => {
                self.pos += 2;
                P::Ne
            }
            b'/' => {
                self.pos += 1;
                P::Slash
            }
            b'<' if b1 == b'<' => {
                self.pos += 2;
                P::Shl
            }
            b'<' if b1 == b'=' => {
                self.pos += 2;
                P::Le
            }
            b'<' => {
                self.pos += 1;
                P::Lt
            }
            b'>' if b1 == b'>' => {
                self.pos += 2;
                P::Shr
            }
            b'>' if b1 == b'=' => {
                self.pos += 2;
                P::Ge
            }
            b'>' => {
                self.pos += 1;
                P::Gt
            }
            b'&' if b1 == b'&' => {
                self.pos += 2;
                P::AndAnd
            }
            b'&' => {
                self.pos += 1;
                P::Amp
            }
            b'|' if b1 == b'|' => {
                self.pos += 2;
                P::OrOr
            }
            b'|' => {
                self.pos += 1;
                P::Pipe
            }
            _ => return Some(self.err(LexErr::UnexpectedByte(b))),
        };
        Some(Tok {
            kind: Kind::P(p),
            span: Span {
                lo,
                hi: self.pos as u32,
            },
        })
    }
}

impl<'a> Lexer<'a> {
    fn lex_number(&mut self, lo: u32) -> Tok {
        let b0 = self.at(self.pos);
        if b0 == b'0' {
            let b1 = self.at(self.pos + 1);
            if b1 == b'x' || b1 == b'X' {
                self.pos += 2;
                let mut v: u64 = 0;
                loop {
                    let h = HEX_T[self.peek() as usize];
                    if h == 0xff {
                        break;
                    }
                    v = (v << 4) | h as u64;
                    self.pos += 1;
                }
                return Tok {
                    kind: Kind::Int(v),
                    span: Span {
                        lo,
                        hi: self.pos as u32,
                    },
                };
            }
            if b1 == b'b' || b1 == b'B' {
                self.pos += 2;
                let mut v: u64 = 0;
                loop {
                    let c = self.peek();
                    if c != b'0' && c != b'1' {
                        break;
                    }
                    v = (v << 1) | (c - b'0') as u64;
                    self.pos += 1;
                }
                return Tok {
                    kind: Kind::Int(v),
                    span: Span {
                        lo,
                        hi: self.pos as u32,
                    },
                };
            }
        }

        let mut v: u64 = 0;
        let mut is_float = false;
        loop {
            let c = self.peek();
            if c == b'_' {
                self.pos += 1;
                continue;
            }
            if !DIGIT_T[c as usize] {
                break;
            }
            v = v.wrapping_mul(10).wrapping_add((c - b'0') as u64);
            self.pos += 1;
        }

        if self.peek() == b'.' && DIGIT_T[self.peek2() as usize] {
            is_float = true;
            self.pos += 1;
            while DIGIT_T[self.peek() as usize] {
                self.pos += 1;
            }
        }

        let e = self.peek();
        if e == b'e' || e == b'E' {
            is_float = true;
            self.pos += 1;
            let s = self.peek();
            if s == b'+' || s == b'-' {
                self.pos += 1;
            }
            while DIGIT_T[self.peek() as usize] {
                self.pos += 1;
            }
        }

        let span = Span {
            lo,
            hi: self.pos as u32,
        };
        if is_float {
            let s = unsafe { std::str::from_utf8_unchecked(&self.src[lo as usize..self.pos]) };
            Tok {
                kind: Kind::Float(s.parse().unwrap_or(0.0)),
                span,
            }
        } else {
            Tok {
                kind: Kind::Int(v),
                span,
            }
        }
    }

    fn lex_string(&mut self, quote: u8) -> Tok {
        self.pos += 1;
        let body = self.pos as u32;
        let mut esc = false;
        loop {
            let c = self.peek();
            if c == 0 {
                return self.err(LexErr::UnterminatedStr);
            }
            if c == quote {
                break;
            }
            if c == b'\\' {
                esc = true;
                self.pos += 1;
                if self.peek() == 0 {
                    return self.err(LexErr::UnterminatedStr);
                }
                self.pos += 1;
            } else {
                self.pos += 1;
            }
        }
        let end = self.pos as u32;
        self.pos += 1;
        let kind = if esc { Kind::StrEsc } else { Kind::Str };
        Tok {
            kind,
            span: Span { lo: body, hi: end },
        }
    }
}
