# MAK Language Syntax Reference

**Status:** source-derived language reference  
**Source reviewed:** MAK lexer, parser, AST, and related source snapshot supplied on 10 October 2026  
**Purpose:** describe how MAK programs are written, how the accepted forms group together, and where the implementation has important exceptions.

This is a reference for people writing MAK, not a guide to the compiler's internal architecture. The rules below are derived from the supplied parser and AST definitions. Where a construct is accepted syntactically but its meaning is incomplete, surprising, ABI-dependent, or unsafe, the text says so instead of presenting it as a language guarantee.

## How to read this document

- **Syntax rule** means the supplied parser explicitly accepts the form.
- **Practical rule** means the form follows from how expressions, delimiters, or declarations are parsed.
- **Caveat** identifies a behavior that may be surprising or hazardous.
- **Not supported by this parser** means the source form is not represented by a corresponding parser rule, even if a related AST or backend feature exists.
- **Implementation detail** means the behavior is visible in the supplied source but should not automatically be treated as an intentional long-term language promise.

Unless stated otherwise, examples are source snippets, not claims that every example has been independently executed on every MAK build. The three `ringest` examples in §11 are analyzed separately because their small textual differences materially change their parse and generated C.

---

# 1. The shape of a MAK source file

A MAK source file consists of zero or more top-level declarations followed by at most one final expression:

```mak
let answer = 40 + 2;
let square x = x * x;

square answer
```

The declarations establish names. The final expression is the program body's value. In the driver shown in the supplied source, the program body is the expression ultimately handed to compilation; the language does not require a textual `main` function in every file.

## 1.1 Top-level order

At the beginning of a file, the parser repeatedly consumes declarations whose first token is `let`, `type`, or `bring`. Once the next token is not one of those declaration starters, it parses the remainder as the program expression.

Consequences:

1. A declaration must appear in declaration position, before the final expression.
2. `let`, `type`, and `bring` are reserved declaration starters at the beginning of a file; they are not ordinary top-level expression atoms.
3. A second declaration after the final expression is not accepted as a new declaration. It becomes trailing input and causes a parse error.
4. A declaration-only file is accepted by the supplied parser. If no expression remains at EOF, the parser synthesizes the integer expression `0` as the body.
5. An empty file also takes the parser's empty-body path and becomes a program whose body is integer `0`.

For a conventional file, put declarations first and the expression last:

```mak
let add x y = x + y;
let double x = add x x;

double 21
```

Do not put a semicolon after the final expression unless it is inside a context that explicitly accepts statement separators, such as a block. At the top level, `expression;` leaves a semicolon after the parsed body and therefore is not a complete source file under the supplied grammar.

**Leading-`let` ambiguity:** at the beginning of the file, a leading `let` is consumed by the top-level declaration loop. If the intended final body is itself a local `let` expression, wrap it in braces or parentheses so it appears as an expression atom rather than as a top-level declaration starter. For example, use `{ let x = 1; x + 1 }` as a root body when that distinction matters.

## 1.2 Top-level grammar at a glance

```ebnf
program          = { declaration }, [ expression ], EOF ;
declaration      = let_declaration
                 | external_declaration
                 | type_declaration
                 | bring_declaration ;
```

`external_declaration` is spelled with the `let` prefix in source; it is not a standalone declaration starter:

```mak
let external printf : i32 = "printf" : (ptr, ...) -> i32 {
    from = "<stdio.h>";
};
```

The `[ expression ]` notation above describes the parser's empty-body fallback. A source program intended to do useful work should supply a final expression explicitly.

---

# 2. Names, keywords, and punctuation

## 2.1 Identifier spelling

Identifiers use ASCII letters, digits, and underscore. The first character must be an ASCII letter or underscore; later characters may also be digits.

```text
name
_name
value2
MAX_SIZE
lowerCamel
snake_case
```

These are examples of valid identifier shapes. A name cannot begin with a digit. Non-ASCII letters are not accepted as identifier characters by the supplied tokenizer.

**Naming convention:** lowercase names are normally used for values, functions, and type parameters; uppercase names are normally used for data constructors. This convention is important for patterns: the parser distinguishes constructors from bindings using the first character's ASCII uppercase status.

## 2.2 Reserved words

The tokenizer recognizes these exact lowercase spellings as keywords:

| Keyword | Syntactic role |
|---|---|
| `let` | Top-level binding, local binding, or let-expression starter |
| `fn` | Lambda/function expression |
| `case` | Pattern-matching expression |
| `type` | Top-level type declaration |
| `extern` | External declaration modifier after `let` |
| `of` | Separator in a `case` expression |
| `bring` | Top-level import declaration |
| `pass` | Tokenized as a keyword, but has no ordinary expression or declaration rule in the supplied parser |

Keywords are case-sensitive. `let` is a keyword; `Let` is an identifier. The parser does not treat `Pass`, `CASE`, or `TYPE` as their lowercase keyword equivalents.

### Contextual words that are not globally reserved

Some meaningful words are ordinary identifiers except at a particular point in a declaration:

- `external` is recognized as a soft spelling for `extern` immediately after `let`.
- `global` is recognized as an optional modifier immediately after the `=` in an external declaration.
- `pure`, `from`, and `abi` are ordinary identifiers interpreted as descriptor keys inside an external descriptor block.
- `true` is an ordinary identifier; it is read as the value of the `pure` descriptor only when that descriptor expects an identifier and the exact text is `true`.
- `false` is also an ordinary identifier. In a `pure` descriptor, every identifier other than exact `true` results in `pure = false` in the parsed descriptor.

These are contextual uses, not global literal or keyword definitions. Outside their special positions, they are parsed as identifiers if the surrounding grammar permits an identifier.

## 2.3 Punctuation accepted by the syntax

The supplied tokenizer recognizes the following punctuation and operator spellings:

```text
{ }  ( )  [ ]  ;  ,  :  .  \
->   =>   =
+  -  *  /  %  &  |  ^  <<  >>
==  /=  <  >  <=  >=  &&  ||  !  ~
```

The three dots in `...` are not a single punctuation token. The foreign-function type parser recognizes three consecutive dot tokens as a variadic marker.

Important near-misses:

- `!=` is **not** the not-equal operator in this grammar; use `/=`.
- `===` is not a defined operator.
- `:=`, `::`, `++`, `--`, `??`, `?.`, and `**` are not defined operators here.
- `=>` is accepted wherever the parser calls for an expression arrow, including lambda-body and `case`-arm separators. Type arrows and foreign-function return arrows use `->` specifically.
- `=` is declaration/descriptor punctuation, not an equality operator. Equality is `==`.

## 2.4 Whitespace, line breaks, and comments

Spaces, tabs, carriage returns, and line feeds separate tokens; normal indentation is permitted. Newlines do not generally terminate an expression. The parser uses punctuation and expression grammar, not indentation, to decide where constructs end.

Both comment forms are accepted:

```mak
// A line comment runs through the end of this line.

/* A block comment can span
   multiple lines. */

/* Block comments may be nested:
   /* nested comment */
   the outer comment continues here.
*/
```

Line comments begin with `//`. Block comments begin with `/*` and end at the matching `*/`; nesting is supported. An unterminated block comment is a lexical error.

Comments are separators, not statement terminators. A comment does not replace a required semicolon.

## 2.5 Where separators are required

| Construct | Separator rule |
|---|---|
| Top-level `let` | Final `;` is required |
| Local `let` expression | `;` between the value and body is required |
| `let` inside a block | `;` after the bound value is required |
| `bring` declaration | Final `;` is required |
| `type Name;` | Final `;` is required |
| `type Name { ... };` | `;` after the closing `}` is required |
| Constructors inside a type body | Separate consecutive constructors with `;`; the final constructor may be immediately followed by `}` |
| External declaration | Final `;` is required, after its optional descriptor block |
| External descriptor entries | `;` is optional to the parser, but recommended and practically important for unknown keys |
| `case` arms | `;` is optional to the parser, but recommended between every pair of arms |
| Block expressions | `;` separates non-final expressions; a trailing `;` before `}` is permitted |
| Final top-level expression | Do not append `;` |

For type declarations, semicolons are not decoration: without a separator, a following identifier can be consumed as another field type of the preceding constructor. For example, `type T { First Second; };` is parsed like a constructor `First` with a field of type `Second`, not as two zero-field constructors. Write `First; Second;` when two constructors are intended.

---

# 3. Literals

The parser has three literal expression kinds: integers, floating-point numbers, and strings. There is no dedicated Boolean literal, tuple expression literal, or array expression literal in this parser.

## 3.1 Integer literals

Accepted integer spellings include decimal, hexadecimal, and binary forms:

```mak
0
7
42
1_000
0xff
0XFF
0b1010
0B1010
```

### Decimal integers

A decimal literal starts with a decimal digit. Underscores are consumed within the decimal integer digit run and are ignored when accumulating the integer value:

```mak
1_000
12__34
7_
```

The latter spellings are accepted by the current scanning loop, even though repeated or trailing separators are poor style. Prefer one underscore between digit groups, such as `1_000_000`.

### Hexadecimal integers

A leading `0x` or `0X` selects hexadecimal. The scanner consumes any run of hexadecimal digits following the prefix:

```mak
0x0
0x2a
0XDEAD
```

**Caveat:** the current scanner does not require at least one hexadecimal digit after the prefix. A bare `0x` is converted to integer zero by the implementation, with the prefix consumed as part of the token. Treat this as an implementation quirk, not a spelling to use in real code.

### Binary integers

A leading `0b` or `0B` selects binary:

```mak
0b0
0b1
0b101010
```

**Caveat:** as with hexadecimal, the current scanner does not require any binary digit after the prefix. It also stops consuming binary digits at the first character other than `0` or `1`; the remaining source is then tokenized separately. Avoid malformed prefixes such as `0b` and `0b2`.

### Integer range and sign

The literal token's value is represented as an unsigned 64-bit value (`u64`) in the AST. A leading minus sign is a separate unary expression, not part of the literal token:

```mak
-1
-(1 + 2)
```

The scanner uses wrapping accumulation for decimal digits and shift/OR accumulation for radix-prefixed values. Do not depend on out-of-range literals to report a neat overflow error. Use literals within the range your program and chosen target type are intended to support.

There is no octal prefix rule in the supplied parser. A leading zero by itself does not select octal.

## 3.2 Floating-point literals

Examples of intended decimal floating-point forms:

```mak
0.5
3.14159
10.0
1e3
1E3
1e-3
6.02e+23
```

A decimal point belongs to a floating-point literal only when a decimal digit follows the point. An exponent marker `e` or `E` can also mark a number as floating-point.

### Decimal point caveats

```text
1.25   floating-point literal
1.     integer `1`, followed by dot `.`
.5     dot `.`, followed by integer `5`
```

The last two forms do not mean `1.0` or `0.5` in the current grammar. Write `1.0` and `0.5`.

### Exponent caveats

The scanner accepts an exponent marker and an optional sign, but its scan loop does not require an exponent digit. If the captured text cannot be parsed as an `f64`, the supplied implementation substitutes `0.0` rather than reporting a lexical error. Therefore forms such as `1e`, `1e+`, or `1e-` must be considered malformed and must not be used, even though the current scanner may still produce a floating token for them.

Underscores are handled inconsistently between integer and floating-point scanning: the integer portion's scan loop permits underscores, but floating-point conversion parses the original token text. Do not put underscores in floating-point literals; write `1000.25`, not `1_000.25`.

No suffixes such as `f32`, `f64`, `u64`, or `i32` are part of numeric literal syntax.

## 3.3 String literals

Single-quoted and double-quoted string literals are accepted:

```mak
"hello"
'hello'
"a string with spaces"
'embedded "double quotes" are fine'
"embedded \'single quotes\' are fine"
```

The opening and closing quote must match. A single quote does **not** introduce a character literal; it introduces a string literal.

String contents are decoded into bytes. The supported escape spellings are:

| Source escape | Decoded byte/value |
|---|---|
| `\n` | Newline |
| `\t` | Horizontal tab |
| `\r` | Carriage return |
| `\\` | Backslash |
| `\"` | Double quote |
| `\'` | Single quote |
| `\0` | Zero byte |
| `\xHH` | One byte from two hexadecimal digits `H` |

Examples:

```mak
"first line\nsecond line"
"column1\tcolumn2"
"C:\\tools\\mak"
"quote: \"hello\""
"byte: \x41"
```

### Escape caveats

- An unknown escape such as `\q` is decoded by dropping the backslash and retaining `q` in the current implementation. Do not assume unknown escapes are preserved verbatim or rejected.
- `\xHH` is intended to contain exactly two hexadecimal digits. Malformed or incomplete hexadecimal escapes are not reliably diagnosed by the current decoder; avoid them.
- There is no `\uXXXX` or `\UXXXXXXXX` Unicode escape syntax in the supplied decoder.
- String data is byte-oriented. Unicode text can appear as UTF-8 source bytes, but the escape grammar does not add a Unicode-code-point escape mechanism.
- The tokenizer scans until the matching quote and does not explicitly reject a line break inside the literal. Multiline string contents may therefore be accepted; ordinary style should use `\n` or deliberate multiline text consistently.
- `\0` embeds a NUL byte. This may be significant when a string is passed to C APIs that treat strings as NUL-terminated; a byte string containing NUL is not equivalent to ordinary visible text for those APIs.

The decoder is used for expression strings, patterns, and `bring` paths. External symbol names and the `from`/`abi` descriptor strings are extracted differently in the supplied parser: escaped text in those positions should not be assumed to decode the same way as an ordinary expression string. Prefer plain symbol names and unescaped header/ABI strings.

## 3.4 Boolean-like values

`true` and `false` are not lexical Boolean literals in the supplied parser. They are identifiers. A program may define values with these names, but their status as Boolean values depends on the surrounding program and backend behavior.

For a simple integer truth convention, a program might use `1` and `0`, but this is a programming convention rather than a built-in Boolean-literal syntax.

## 3.5 Missing literal forms

The supplied parser does not provide source syntax for:

- Character literals distinct from strings.
- `null` or `nil` literals.
- Tuple values such as `(a, b)`.
- Non-empty array values such as `[1, 2, 3]`.
- Record/object literals.
- Numeric type suffixes.
- Interpolated strings.
- Raw string delimiters.

An isolated `[]` is a special exception described in §7.6: it is accepted as a placeholder expression producing integer zero, not as a usable array value.

---

# 4. Type syntax

Types occur in external declarations and constructor field declarations. The AST defines primitive, named, applied, array, tuple, function, and bottom-type forms.

## 4.1 Primitive types

The primitive spellings explicitly recognized by the type parser are:

```text
u8   u16   u32   u64
i8   i16   i32   i64
f32  f64
()
!
```

`()` is the unit type when it appears in type position. `!` is the bottom/never-returning type in type position.

### Important naming exception

The parser recognizes `u8` through `u64`, `i8` through `i64`, `f32`, and `f64` specially. It does **not** map the identifier `unit` to the unit primitive; `unit` parses as a named type. Write `()` for unit in a type declaration.

`ptr` is not one of the ordinary primitive `Ty::Prim` spellings. It is recognized as a foreign type atom in a C external signature. Do not assume `ptr` has the same status in all type contexts.

## 4.2 Named types

An identifier that is not one of the recognized primitive spellings becomes a named type:

```mak
MyType
List
Result
T
```

A bare name is a named type. The parser does not prove that the name is declared or that the type is usable in a particular context; such questions belong to later processing.

## 4.3 Array types

An array type wraps one type in square brackets:

```mak
[u8]
[i32]
[[u8]]
[(i32, i32)]
```

The grammar is type-level only. It does not imply that square-bracket array values can be constructed with a literal expression.

## 4.4 Tuple types

Tuple types use parentheses and commas:

```mak
(i32, i32)
(i32, f64, [u8])
()
```

The comma is what distinguishes a tuple type from a parenthesized single type. `()` is specifically unit. A one-item form such as `(i32)` is just the parenthesized type `i32`, not a one-element tuple.

A trailing comma is not a general one-tuple feature. The parser requires another type after each comma in a tuple type, so `(i32,)` is invalid. Prefer tuples with at least two entries.

## 4.5 Function types

The arrow `->` builds a function type:

```mak
 i32 -> i32
 (i32, i32) -> i32
 i32 -> i32 -> i32
```

Function arrows associate to the right. Thus:

```mak
i32 -> i32 -> i32
```

is read as:

```text
i32 -> (i32 -> i32)
```

For a multi-argument function signature, the tuple form on the left explicitly groups the arguments, as in `(i32, i32) -> i32`. The source type representation may be more general than every C ABI or backend path can support; use the foreign type on an external declaration to describe the linked C calling shape.

## 4.6 Type application

The parser's explicit type-application form is parenthesized:

```mak
(List a)
(Result e value)
(Map key value)
(List [u8])
```

Inside the parentheses, the head must be a named type. The argument types are parsed as type atoms rather than arbitrary unparenthesized function types.

Caveats:

- `List a` written without the type-application parentheses is not one `Ty::App` node in this grammar. In contexts that parse multiple constructor field types, it may instead be read as separate type tokens/fields.
- The parser's ability to create a `Ty::App` node does not prove that all generic types are fully supported by representation inference and C layout.
- Use nested parentheses to make complex type applications unambiguous.

## 4.7 Type grammar summary

```ebnf
type             = type_atom, [ "->", type ] ;
type_atom        = primitive_or_name
                 | "[", type, "]"
                 | "(", ")"
                 | "(", type, ")"
                 | "(", type, ",", type, { ",", type }, ")"
                 | "(", named_type, type_atom, { type_atom }, ")"
                 | "!" ;
```

This compact grammar is explanatory rather than a replacement for the detailed rules above. The applied-type form is a special parenthesized shape, and function types are right-associative.

---

# 5. Top-level declarations

## 5.1 Value declarations

The basic declaration is:

```mak
let name = expression;
```

Example:

```mak
let answer = 42;
let greeting = "hello";

answer
```

The semicolon is required after a top-level binding.

Top-level bindings can also use parameter sugar:

```mak
let add x y = x + y;
let square x = x * x;
```

The parameterized form is represented as a function value. Its shape corresponds to:

```mak
let add = fn x y -> x + y;
let square = fn x -> x * x;
```

The parameter list in a top-level `let` declaration is an unparenthesized sequence of identifiers between the declared name and `=`. Unlike the `fn` parameter form, commas and parentheses are not accepted there:

```mak
let add x y = x + y;       // accepted shape
let add (x, y) = x + y;    // not this declaration grammar
```

### Binding boundaries

- The parser reads the declaration name first.
- Every consecutive identifier before `=` is treated as a parameter.
- A non-identifier before `=` (other than `=` itself) causes an error.
- The entire right-hand expression is parsed up to the declaration's required semicolon.
- There is no type-annotation syntax on an ordinary `let` binding in this grammar.

For a local binding with a body, see §6.3. The final semicolon in `let name = value; body` separates the bound value from the expression that can use it; it does not terminate the whole program.

## 5.2 Type declarations

A type declaration has one of two forms.

### Declaration without a constructor body

```mak
type Name;
type Name parameter;
type Name parameter1 parameter2;
```

This records the type name and its parameter names with an empty constructor list in the AST. The parser accepts the form, but an empty type is not the same as a type with a conventional default value.

### Declaration with constructors

```mak
type Maybe a {
    None;
    Some a;
};
```

A more involved example:

```mak
type List a {
    Nil;
    Cons a (List a);
};
```

Rules:

1. The declaration begins with `type`, followed by an identifier.
2. Zero or more following identifiers are consumed as type parameters until the parser reaches a token that is not an identifier.
3. A semicolon at that point completes an empty type declaration; otherwise `{` must open the constructor body.
4. Each constructor begins with an identifier.
5. Constructor field types follow the constructor name. Each field type is parsed in sequence until `;` or `}`.
6. A constructor's semicolon is consumed if present; the parser also permits a constructor immediately followed by `}` without its own semicolon.
7. The closing `}` of a non-empty-body-form declaration must be followed by a semicolon.

The last semicolon is significant:

```mak
type Color {
    Red;
    Green;
    Blue;
};                       // declaration terminator required
```

### Constructor naming caveat

The type-declaration parser accepts any identifier as a constructor name. It does not enforce initial uppercase. Pattern parsing, however, treats an identifier beginning with an uppercase ASCII letter as a constructor pattern and a lowercase identifier as a binding pattern.

The parser also does not require constructor names to be globally unique across type declarations. Reusing one name for constructors of different types can make later resolution ambiguous or version-dependent. Prefer unique constructor names unless the compiler version documents and tests a reliable disambiguation rule.

Therefore, use uppercase constructor names consistently:

```mak
type Option a {
    None;
    Some a;
};
```

A lowercase constructor may parse in a declaration but will be ambiguous or unusable as the constructor pattern you intended.

### Constructor-field syntax

Constructor field types are written adjacent to the constructor name, without commas:

```mak
Pair i32 i32;
Wrapped (List i32);
Callback (i32 -> i32);
```

Use type parentheses where necessary to keep a complex field type together. Do not use commas as constructor field separators; commas have roles in tuple types and parameter/pattern lists, not here.

The parser records constructor type names and field-type syntax. That alone does not guarantee that polymorphic recursion, generic layouts, or every nested representation is supported correctly at runtime.

## 5.3 External declarations: calling C or another foreign symbol

External declarations declare a MAK-visible name and the external symbol/type information used by the backend.

General shape:

```mak
let external name : MAK_TYPE = "foreign_symbol" : FOREIGN_TYPE {
    pure = false;
    from = "<header.h>";
    abi = "C";
};
```

The keyword spelling `extern` is also accepted after `let`:

```mak
let extern name : MAK_TYPE = "foreign_symbol" : FOREIGN_TYPE;
```

The shorter required skeleton is:

```mak
let external name = "foreign_symbol";
```

In this shorter form, the parser defaults the MAK-side type to bottom (`!`) and the foreign type to `void`. Those defaults may not describe the actual function you are linking; give explicit types for real interop.

### External declaration fields

| Piece | Required? | Meaning in the syntax |
|---|---:|---|
| `let` | Yes | Introduces the top-level declaration |
| `external` or `extern` | Yes | Marks it as an external declaration |
| `name` | Yes | MAK-visible identifier |
| `: MAK_TYPE` | No | MAK-side type; defaults to `!` if absent |
| `=` | Yes | Introduces the foreign symbol selection |
| `global` | No | Contextual modifier recognized immediately after `=` |
| string literal | Yes | Foreign symbol name |
| `: FOREIGN_TYPE` | No | Foreign-call type; defaults to `void` if absent |
| `{ ... }` | No | Descriptor block |
| final `;` | Yes | Terminates the declaration |

Examples:

```mak
let external printf : i32 = "printf" : (ptr, ...) -> i32 {
    from = "<stdio.h>";
};

let external putchar : i32 -> ! = "putchar" : (i32) -> void {
    from = "<stdio.h>";
};

let external exit : i32 -> ! = "exit" : (i32) -> void {
    from = "<stdlib.h>";
};
```

The source-visible type and the foreign calling type are distinct descriptions. They must agree about argument shape, return shape, pointer/value interpretation, and calling convention if the call is to be safe.

### The contextual `global` modifier

The parser recognizes the identifier `global` specially only in the position immediately following the external declaration's `=`:

```mak
let external errno : i32 = global "errno" : i32 {
    from = "<errno.h>";
};
```

This is accepted as a syntactic global external declaration. Its actual suitability depends on how the backend handles global symbols; it is not equivalent to a function call signature. Use the form only when the target backend and symbol kind support it.

### Descriptor blocks

The descriptor block is optional and contains entries of the following known forms:

```mak
pure = true;
pure = false;
from = "<stdio.h>";
abi = "C";
```

`pure`, `from`, and `abi` are identifiers interpreted specially inside this block. Known entries require `=` followed by the expected value. A semicolon after an entry is optional in the parser, but using one is strongly recommended for readability and separation.

Rules and exceptions:

- `pure` reads one identifier. It sets the parsed flag to true only for the exact spelling `true`; another identifier such as `false` sets it to false. A quoted string or integer is not accepted as the `pure` value.
- `from` and `abi` require a string literal.
- Unknown descriptor keys are not rejected by the parser. The parser consumes an expression as their value and discards that value. Do not depend on unknown keys: they may be ignored rather than having any meaning.
- Since unknown-key values are parsed as expressions, omitting a semicolon before a following identifier can cause that identifier to be consumed as part of the expression. Use a semicolon after every descriptor entry.
- A descriptor block must close with `}` and the external declaration must then end with `;`.
- The supplied parser extracts the text for `from` and `abi` differently from ordinary expression strings. Keep these values plain and avoid escape sequences.

### Variadic foreign functions

A foreign signature can use `...` to indicate a variadic argument marker:

```mak
let external printf : i32 = "printf" : (ptr, ...) -> i32 {
    from = "<stdio.h>";
};
```

The fixed arguments and the variadic marker are separated by commas. The foreign parser recognizes three consecutive dot tokens as `...`; whitespace may appear between those dot tokens because they are tokenized separately, but write them contiguously to express intent.

The parser's grammar is more permissive than a robust C declaration validator: it does not itself guarantee that a variadic marker appears only in a position valid for the C ABI. Use the conventional shape `(fixed_arg_types..., ...) -> result_type`, with `...` last in the argument list.

### Foreign type atoms and pointer syntax

The foreign type atom spellings are:

```text
i8 i16 i32 i64
u8 u16 u32 u64
f32 f64
ptr void
```

Any other identifier is represented as a named foreign type. An atom may be followed by one `*` to express a pointer-to-atom type:

```mak
i32*
ptr*
MyStruct*
```

The supplied parser supports one star suffix on a foreign atom. Do not assume arbitrary pointer depth (`i32**`) or a pointer suffix on a whole foreign function type is supported.

Foreign function types use parentheses for the argument list and `->` for the result:

```mak
() -> i32
(i32) -> i32
(ptr, u64) -> i32
(ptr, ...) -> i32
(i32, f64) -> void
```

The parser accepts an empty argument list and also allows a trailing comma. For portable, readable declarations, avoid unusual arrangements of repeated or non-final variadic markers even if they get past parsing.

### Safety rules for FFI declarations

This syntax exposes powerful behavior, but does not automatically make the signature safe. Before calling an external function, check all of the following:

1. The symbol string exactly names the intended linked function or global.
2. The MAK-side type describes how source code will call the name.
3. The foreign type matches the actual C prototype and ABI.
4. Pointer arguments are actual valid pointers, not integer values converted to pointers by a cast.
5. Variadic arguments have the C types expected by the function's format string or API.
6. Any return-type mismatch is avoided, especially for `void`, pointer, floating-point, and aggregate returns.
7. `pure = true` is used only if the external function truly has no observable side effects and its result depends only on its arguments. A false purity claim can make optimization unsound.

For example, this declaration describes `printf` as accepting a pointer followed by variadic arguments:

```mak
let external printf : i32 = "printf" : (ptr, ...) -> i32 {
    from = "<stdio.h>";
};
```

The format string must be the first argument. `printf 16` is not a safe way to print the number sixteen: it puts an integer where the foreign signature expects a pointer. Use `printf "%d\n" 16` or an equivalently parenthesized expression.

## 5.4 Import declarations (`bring`)

The import declaration is:

```mak
bring "path/to/file.mak";
```

One `bring` declaration can name several paths before its one terminating semicolon:

```mak
bring "math.mak" "strings.mak" "platform/io.mak";
```

Rules:

- At least one string literal is required after `bring`.
- The declaration must end with `;`.
- Both single-quoted and double-quoted strings are accepted by the parser.
- The driver resolves relative paths against the directory of the file containing that import.
- The loader tracks visited paths to avoid loading the same canonical path more than once.
- Imported declarations are incorporated into the program; the imported file's final expression is not used as the root program body by the supplied loader.
- `bring` is a top-level declaration. It is not an expression and cannot be used as a block statement or lambda body.

Use clear relative paths and keep imports at the beginning of a file with the other declarations:

```mak
bring "prelude.mak";

let main_value = 7;
main_value
```

The exact module visibility and duplicate-name rules depend on later semantic/backend behavior; `bring` itself is not a namespace-qualified import syntax.

---

# 6. Expressions

Every final program body, function body, local `let` body, block tail, and `case` arm body is an expression. The main expression forms supported by the parser are:

- integer, floating-point, and string literals;
- identifiers;
- parenthesized expressions;
- the special empty-bracket placeholder `[]`;
- brace-delimited blocks;
- `let` expressions;
- `fn` and backslash lambda expressions;
- `case` expressions;
- function application;
- function composition with `.`;
- unary and binary operators.

## 6.1 Identifiers and parenthesized expressions

A bare identifier is a variable expression:

```mak
answer
square
printf
```

Parentheses group an expression:

```mak
(answer)
(square (x + 1))
((\x -> x * x) 4)
```

Parentheses are the reliable way to make a desired grouping explicit. A parenthesized expression evaluates syntactically to the inner expression; the AST does not gain a separate grouping node.

### No tuple expression syntax

Although tuple types and tuple patterns exist, a comma-separated tuple *value* is not parsed by the expression grammar:

```mak
(a, b)        // not a tuple expression
```

Inside ordinary expression parentheses, the parser reads one expression and then expects `)`. A comma therefore causes an error.

## 6.2 Function expressions (lambdas)

The parser accepts `fn` and backslash as lambda starters. Each form creates a function expression.

### Arrow-bodied lambda, unparenthesized parameters

```mak
fn x -> x + 1
fn x y -> x + y
\x -> x + 1
\x y -> x + y
```

Parameters are consecutive identifiers. In the unparenthesized form, stop writing parameters when the next token is not an identifier.

### Arrow-bodied lambda, parenthesized parameters

```mak
fn (x) -> x + 1
fn (x, y) -> x + y
fn (x y) -> x + y
\(x, y) -> x + y
```

Commas are optional between identifiers in a parenthesized lambda parameter list. A trailing comma is accepted by the current loop:

```mak
fn (x,) -> x
```

This permissiveness is a parser behavior; for style and consistency, omit the trailing comma unless there is an explicit project convention.

A comma-separated parameter list is most readable:

```mak
fn (left, right) -> left + right
```

### Braced lambda body

A lambda may use a brace-delimited body instead of an arrow:

```mak
fn (x) { x + 1 }
fn (x, y) { x + y }
fn x y { x + y }
\x { x + 1 }
```

The parser expects `{`, `->`, or `=>` after the parameter list. The braced body uses the same block-body rules described in §6.5.

### Fat arrow

`=>` is accepted as an alternative expression arrow:

```mak
fn x => x + 1
case value of { 0 => "zero"; _ => "other" }
```

The type syntax does not use `=>`; use `->` for function types and foreign function result types.

### Zero-parameter functions

The parser allows an empty parameter list or no parameters before an arrow/body:

```mak
fn -> 42
fn () -> 42
fn () { 42 }
\ -> 42
```

This is a syntactic allowance. Confirm how zero-parameter functions are represented and called in the compiler version you are using before relying on them in a public API.

### Parameters are names, not patterns

Lambda parameter positions accept identifiers only. They do not accept constructor patterns, literals, tuple destructuring, or type annotations:

```mak
fn (x, y) -> x + y     // valid parameter names
fn (Some x) -> x       // not a pattern parameter
fn (x: i32) -> x       // no parameter type-annotation grammar
```

Use `case` to destructure a value after binding it to a parameter.

## 6.3 `let` expressions

A local `let` expression has a binding, a required semicolon, and a body expression:

```mak
let name = value; body
```

Example:

```mak
let radius = 5;
radius * radius
```

The body is part of the expression and may refer to the new name. There is no `in` keyword.

### Function sugar in a local `let`

```mak
let square x = x * x;
square 9
```

The parser wraps the right-hand value in a lambda when one or more parameter identifiers appear between the local name and `=`. This is equivalent in shape to:

```mak
let square = fn x -> x * x;
square 9
```

### Local binding syntax and scope

- The local binding's `=` is mandatory.
- The semicolon after the value is mandatory.
- A body expression is mandatory.
- The binding name is one identifier.
- Zero or more unparenthesized identifier parameters may follow that name.
- A local external declaration is prohibited; `external` declarations may appear only at top level.

This is the idiomatic way to name an intermediate result:

```mak
let x = 10;
let y = x * 2;
y + 1
```

In this source shape, the second `let` is the body of the first `let`. Braces are not necessary for nested local `let` expressions.

### Binding scope

A local binding's value is parsed before the binding's body. The value is not written as a recursive binding by this syntax alone. Do not assume the new name is in scope inside its own right-hand value.

Use an explicit top-level function or another supported recursive pattern for recursion; the accepted syntax does not document a `let rec` form.

## 6.4 Function application

Application places one or more argument expressions after a function expression, generally without commas:

```mak
f x
add x y
square (n + 1)
printf "%d\n" 16
```

The intended simple grouping is left-associated:

```mak
f x y
```

means repeated application in the order:

```text
(f x) y
```

The backend also has machinery to flatten applications while resolving calls. That does not mean every application expression is guaranteed to behave like a C call with arbitrary arguments; currying, known function arity, external declarations, and optimization may affect lowering.

### Arguments are atoms unless grouped

An argument can be an identifier, literal, parenthesized expression, block, lambda, `let`, `case`, or other expression form that begins an atom. Since function application binds more tightly than ordinary infix operators, write parentheses around compound argument expressions:

```mak
f (x + y)
f (g x)
f (case x of { 0 -> 1; _ -> 2 })
```

For example:

```mak
f x + y
```

is grouped as:

```mak
(f x) + y
```

not as `f (x + y)`.

### Zero-argument postfix application

A special postfix form consisting of empty parentheses is accepted after an expression:

```mak
f()
f ()
f()()
```

The parser represents this as an application with zero arguments. In this implementation, `f()` and `f ()` have the same token sequence for the parser's special check, so whitespace does not turn it into a unit-valued argument.

A standalone `()` is not a unit *expression*. In expression position, parentheses must contain an expression; `()` by itself therefore does not parse as a value.

Do not confuse:

```mak
f()      // zero-argument application syntax
f (x)    // one argument, the value of x
```

### No comma-separated ordinary call syntax

Ordinary application does not use comma separators:

```mak
f x y           // ordinary application shape
f(x, y)         // not a general C-style argument-list syntax
```

Parentheses group a single expression, while the parser's special empty-parentheses case handles zero-argument application. Comma-separated parameters and foreign ABI argument lists have their own grammar.

## 6.5 Blocks

A block is delimited by braces:

```mak
{
    expression1;
    expression2;
    final_expression
}
```

A block is an expression. Its final expression supplies the block's value. Semicolons separate preceding expressions from what follows; a trailing semicolon is permitted.

Examples:

```mak
{ 7 }
{ 7; }
{ 1; 2; 3 }
{ let x = 5; x * 2 }
```

### Block edge cases in the supplied parser

The current parser has several specific behaviors worth knowing:

- An empty block `{}` is represented as integer `0`.
- A block with one expression returns that expression directly, with or without a final semicolon.
- A block with multiple expressions stores earlier semicolon-terminated expressions as statements and the last expression as its tail.
- A trailing semicolon is allowed before `}`.
- A local `let` encountered during block parsing is handled specially: the binding's body consumes the remainder of that block. Its scope includes the following portion of the block, not expressions that precede the binding.
- A local `let` has its own required semicolon after the bound value; this is different from the optional final expression separator before a closing brace.

Illustration:

```mak
{
    do_first_thing;
    let value = compute();
    use value
}
```

The local binding encloses the remaining block body so that `value` is available to `use value`. It does not retroactively enclose `do_first_thing`.

### Block values are not implicit unit values

The parser uses integer zero for an empty block rather than a dedicated unit-expression node. Do not assume `{}` is interchangeable with a value of type `()` in all contexts.

## 6.6 `case` expressions

A `case` expression matches one scrutinee against ordered arms:

```mak
case expression of {
    pattern -> result;
    pattern -> result;
    _ -> default_result
}
```

`of`, the opening brace, and the closing brace are mandatory. Every arm needs a pattern and an expression arrow (`->` or `=>`). The semicolon after an arm is optional in the parser; using it consistently is strongly recommended.

Example:

```mak
case number of {
    0 -> "zero";
    1 -> "one";
    _ -> "other"
}
```

### Arm ordering and separators

The parser retains arms in source order. Pattern matching normally uses that order, so place more specific patterns before catch-all patterns.

Although the parser permits an omitted semicolon between arm bodies, omission is not always unambiguous: an adjacent token that can start an atom can be absorbed into the preceding expression as another application argument. Use semicolons between all arms in normal code.

The arm loop itself does not require a first arm before `}`; consequently `case value of {}` is accepted by the parser as a case node with zero arms. This is a parse-level edge case, not a useful default or a promise that later compilation can evaluate it. Provide at least one arm in practical programs.

### No guards or `if` clauses

The supplied pattern grammar has no guard syntax such as `pattern when condition`, and it has no `if`/`then`/`else` expression form. Express conditional behavior with nested `case` expressions or a function/library convention that is available in your program.

## 6.7 Function composition (`.`)

The dot operator is accepted between expressions:

```mak
f . g
```

The parser desugars this syntax into a one-parameter lambda whose body applies the left expression to the result of applying the right expression to the parameter. Conceptually:

```mak
f . g
```

means:

```mak
\x -> f (g x)
```

The parser uses the internal name `$x` for that generated binder. It is an implementation-generated placeholder, not a name to write in source code.

### Composition is not function piping

Composition does not mean “evaluate `f` and then pass it to `g`.” The left expression is applied to the result of applying the right expression to a new input.

If the right side is itself an application, grouping matters:

```mak
f . (g x)
```

means composition of `f` with the value-expression `g x`, and desugars to a lambda which attempts to apply `g x` to the new argument. This is usually not the intent.

A composition expression is useful when both sides represent functions:

```mak
let increment x = x + 1;
let double x = x * 2;

increment . double
```

Because application binds tighter than dot, the parentheses in complicated compositions are important. When in doubt, define intermediate functions explicitly and test them.

### Dot associativity

The parser's binding-power rules make dot right-associative:

```mak
f . g . h
```

is grouped as:

```mak
f . (g . h)
```

Its nested lambda desugaring can be non-obvious. Parenthesize compositions to make intent obvious.

## 6.8 Prefix unary operators

The prefix operators are:

| Operator | AST operator | Conventional meaning |
|---|---|---|
| `-x` | `Neg` | Numeric negation |
| `!x` | `Not` | Logical negation |
| `~x` | `BitNot` | Bitwise complement |

The unary operand includes an ordinary function application, while stopping before dot and binary operators. Thus the parser comments and behavior indicate these groupings:

```mak
-f x       // -(f x)
!f x       // !(f x)
~f x       // ~(f x)
-(a + b)   // negation of the grouped sum
!-x        // logical-not applied to the negated x
```

Parentheses should be used whenever an expression could be read in more than one way. The exact type-validity and runtime behavior of each unary operator depends on the operand and backend representation; syntax alone does not make an invalid operator/type combination meaningful.

## 6.9 Binary operators

MAK's AST contains the following binary operator groups:

| Group | Spellings |
|---|---|
| Arithmetic | `+`, `-`, `*`, `/`, `%` |
| Bitwise | `&`, `|`, `^`, `<<`, `>>` |
| Equality | `==`, `/=` |
| Ordering | `<`, `>`, `<=`, `>=` |
| Logical | `&&`, `||` |

Precedence and associativity are specified in §8. `=` is not included because it is not an expression-level equality operator.

### Operator reminders

- `==` is equality.
- `/=` is not-equal.
- `!` is prefix logical negation; it is not an infix operator.
- `&`, `|`, and `^` are bitwise operators at the same precedence in this parser.
- `&&` and `||` are separate logical operators at lower precedence than comparisons.
- `<<` and `>>` are shift operators.
- `%` is the remainder operator in the AST.

The grammar defines the shape of these expressions, not an unrestricted set of operand types. Check the compiler's supported representations and write explicit conversions/FFI wrappers where required.

## 6.10 Quoting and related syntax that are not ordinary expressions

The AST has an internal `Quote` variant, but the supplied source parser has no normal expression-atom rule that creates a quote expression. A backtick is not a token in the listed punctuation grammar. A single quote begins a string literal; it is not quotation syntax.

Similarly, the presence of type, array, tuple, function, or pointer-like forms in AST/type enums does not mean each of them has an expression-literal syntax. See §9 for the unsupported surface forms.

---

# 7. Patterns

Patterns are used in the arms of a `case` expression. The AST supports wildcards, variable bindings, integer literals, string literals, constructor patterns, and tuple patterns.

## 7.1 Wildcard pattern

```mak
_
```

The underscore wildcard matches without introducing a named binding. It is the usual final/default arm:

```mak
case n of {
    0 -> "zero";
    _ -> "nonzero or other"
}
```

A lone `_` is special in pattern position. In other expression contexts it is an identifier-shaped name unless later rules reject or reinterpret it.

## 7.2 Variable-binding pattern

A lowercase-leading identifier is parsed as a variable binding:

```mak
case value of {
    x -> x
}
```

The arm binds the scrutinee value to `x`, then the body may use `x`.

The parser uses the first ASCII character to determine whether an identifier is a constructor pattern or a binding pattern. Uppercase-leading names do not act as ordinary pattern binders under this rule.

The wildcard spelling `_` is special in pattern position only. In expression position it is parsed as an ordinary identifier-shaped variable name (which may or may not resolve later).

## 7.3 Integer-literal pattern

```mak
case n of {
    0 -> "zero";
    1 -> "one";
    42 -> "answer";
    _ -> "other"
}
```

Integer patterns use the integer literal token, whose AST value is unsigned. Negative patterns are not a supported direct pattern form:

```mak
-1 -> "negative one"    // not accepted as an integer pattern
```

If a signed comparison is needed, bind the value and use an expression condition implemented through whatever comparison/case structure the program supports.

## 7.4 String-literal pattern

String patterns accept either quote style and the same expression-string escape decoder:

```mak
case text of {
    "yes" -> 1;
    'no' -> 0;
    _ -> 2
}
```

The pattern AST stores decoded bytes. String pattern matching is therefore a value-level operation, but its precise semantics depend on the backend's supported string representation.

## 7.5 Constructor pattern

A constructor pattern begins with an identifier whose first ASCII character is uppercase:

```mak
case value of {
    None -> 0;
    Some x -> x;
    _ -> -1
}
```

Multiple constructor fields are written as successive patterns, without commas:

```mak
case pair of {
    Pair x y -> x + y;
    _ -> 0
}
```

Use parentheses to delimit nested structures:

```mak
case xs of {
    Cons head (Cons second tail) -> head + second;
    _ -> 0
}
```

### Constructor-pattern arity

The parser consumes successive pattern-starting tokens as arguments to an uppercase constructor. It does not necessarily know the constructor's declared arity at the parsing step; later semantic/backend processing resolves the constructor and its fields.

Write the same number of patterns as the constructor declares fields. Incorrect arity may fail later or trigger behavior that is not a syntax guarantee.

### Greedy argument parsing

Because constructor arguments are adjacent and have no delimiter, this form:

```mak
Ctor a b
```

is read as a constructor pattern with two arguments. If a subpattern is structurally complex, group it:

```mak
Ctor (Other x) (Pair y z)
```

Commas are not constructor-pattern argument separators.

### Uppercase heuristic caveat

The parser accepts lowercase identifiers as constructor declarations, but pattern syntax classifies a lowercase-leading identifier as a binding. For example, a declaration named `some` cannot be relied on to parse as a constructor pattern `some x`; the pattern parser sees `some` as a binder. Use uppercase constructor names such as `Some`.

### Float-pattern exception

The expression grammar has float literals, but the pattern grammar does not. A float token is not accepted as a direct pattern. Patterns also have no comparison/range form such as `1..10`.

## 7.6 Tuple patterns

Tuple patterns use parentheses and comma separators:

```mak
case value of {
    (left, right) -> left + right;
    _ -> 0
}
```

Nested tuple patterns are permitted:

```mak
case value of {
    (x, (y, z)) -> x + y + z;
    _ -> 0
}
```

The parser supports the following precise distinctions:

| Pattern spelling | Current parser interpretation |
|---|---|
| `()` | Empty tuple pattern (`Tuple([])`) |
| `(x)` | Parenthesized pattern `x`, not a one-element tuple |
| `(x, y)` | Two-element tuple pattern |
| `(x, y, z)` | Three-element tuple pattern |
| `(x,)` | Invalid: a pattern is required after the comma |

Thus, the parser can represent an empty tuple pattern and tuples with two or more entries, but not a dedicated one-element tuple pattern.

## 7.7 Pattern forms not provided

There is no parser rule for these pattern forms:

```text
float literal pattern
negative integer pattern
range pattern (1..10)
OR pattern (A | B)
AND pattern (p & q)
guarded pattern (p when condition)
record/field pattern ({ field = x })
list syntax pattern ([x, y])
```

A constructor can contain nested constructor and tuple patterns, but that is not the same as general pattern combinators or field-name destructuring.

## 7.8 Pattern matching style

Prefer arms separated by semicolons, put specific cases first, and use `_` only when a genuine fallback is intended:

```mak
case maybe_value of {
    Some 0 -> "some zero";
    Some n -> "some other integer";
    None -> "nothing";
    _ -> "fallback for any remaining supported case"
}
```

The parser does not itself guarantee exhaustiveness or absence of unreachable arms. It also does not prove that all arm bodies have a compatible result representation. Treat those as separate correctness requirements.

---

# 8. Operator precedence and associativity

This table is based on the expression parser's binding-power rules. Rows higher in the table bind more tightly, subject to the explicit note that application is recognized before the infix-operator loop and is therefore available even while parsing the operand of a prefix operator.

| Binding strength | Form | Associativity / grouping |
|---:|---|---|
| Strongest postfix form | `f()` | Repeated postfix zero-argument application |
| Very high | `f x`, `f x y` | Left-associated application: `(f x) y` |
| High | Prefix `-`, `!`, `~` | Operand includes ordinary application; stops before dot and binary operators |
| 9 | `.` | Right-associative composition |
| 7 | `*`, `/`, `%` | Left-associative |
| 6 | `+`, `-` | Left-associative |
| 5 | `<<`, `>>` | Left-associative |
| 4 | `&`, `^`, `|` | Left-associative; all share this level |
| 3 | `==`, `/=`, `<`, `>`, `<=`, `>=` | Left-associative in the parser |
| 2 | `&&` | Left-associative |
| 1 | `||` | Left-associative |

Parentheses override these rules and are recommended for any nontrivial expression.

## 8.1 Application wins over binary operators

```mak
f x + y
```

parses as:

```mak
(f x) + y
```

To make the sum the argument, write:

```mak
f (x + y)
```

Likewise:

```mak
f x * y       // (f x) * y
f (x * y)     // f applied to the product x*y
```

## 8.2 Arithmetic grouping

The usual arithmetic grouping follows the table:

```mak
a + b * c      // a + (b * c)
a * b + c      // (a * b) + c
a - b - c      // (a - b) - c
a / b / c      // (a / b) / c
```

Do not assume integer division, floating division, overflow, or remainder behavior from the syntax alone; those depend on operand representation and generated operations.

## 8.3 Shifts and bitwise operators

```mak
a << b
x >> n
mask & value
flags | option
x ^ y
```

Shifts bind more tightly than bitwise AND/XOR/OR in this parser. The three bitwise operators share a precedence level:

```mak
a & b ^ c | d
```

is parsed left-associated across that common level. Prefer explicit parentheses because many readers expect different precedence conventions from other languages.

## 8.4 Comparisons

```mak
x == y
x /= y
x < y
x > y
x <= y
x >= y
```

The parser associates comparisons from left to right. A chained comparison such as:

```mak
a < b < c
```

is not a special mathematical range comparison. It is grouped as `(a < b) < c`, which is likely not what you intend and may not be valid for the inferred result types. Write two comparisons in a structure supported by the language instead.

## 8.5 Logical operators

```mak
a && b
x || fallback
```

`&&` binds more tightly than `||`:

```mak
a || b && c
```

is grouped as:

```mak
a || (b && c)
```

The parser creates distinct logical operator nodes. The exact result values and evaluation/short-circuit behavior should be checked against the backend for the version being used; do not infer all operational guarantees from the punctuation alone.

## 8.6 Unary operators versus application

The parser's prefix rule explicitly lets application remain inside a unary operand:

```mak
-f x        // -(f x)
!f x        // !(f x)
~f x        // ~(f x)
```

To negate a value and then pass it as an argument, group the unary expression:

```mak
f (-x)
f (!(x == 0))
```

A leading `-` immediately after an already parsed left expression is recognized as the binary subtraction operator. Source whitespace does not change token identity:

```mak
x - y
```

is subtraction; if the desired right-hand value is a negated expression, make the grouping explicit:

```mak
x - (-y)
```

## 8.7 Composition grouping

Dot binds less tightly than application and more tightly than binary arithmetic:

```mak
f . g x + y
```

must not be read intuitively as a sequence of function calls. Application is consumed first, composition is desugared, and addition is parsed outside the composition unless parentheses say otherwise. Prefer:

```mak
(f . g) x
f . (g x)
(f . g) (x + y)
```

according to the actual intent, remembering that `.` constructs a function. In particular, `f . (g x)` applies the value `g x` to the new composition parameter, which is often a mistake.

## 8.8 Binding powers, for readers who want the exact parser levels

The supplied parser assigns these left/right binding powers to infix operators:

| Infix | Left binding power | Right binding power |
|---|---:|---:|
| `.` | 9 | 9 |
| `*`, `/`, `%` | 7 | 8 |
| `+`, `-` | 6 | 7 |
| `<<`, `>>` | 5 | 6 |
| `&`, `^`, `|` | 4 | 5 |
| `==`, `/=`, `<`, `>`, `<=`, `>=` | 3 | 4 |
| `&&` | 2 | 3 |
| `||` | 1 | 2 |

Different left/right values give the ordinary binary operators left-associative behavior. Dot uses equal left and right binding powers, allowing a same-precedence dot expression on the right and thereby making composition right-associative.

Prefix expressions parse their operand at binding power 10. Since application is handled unconditionally in the expression loop, the operand can contain application but cannot absorb dot or a lower-precedence binary operator. This is the source of the `-f x` grouping described above.

---

# 9. Forms that are not source syntax (or are only partial placeholders)

This section is intentionally blunt. An AST node, a type form, a C backend helper, or a reserved token does not automatically mean there is a corresponding source expression.

## 9.1 `pass`

`pass` is tokenized as a keyword, but the supplied parser has no `pass` branch in its declaration or expression grammar. It is not a usable no-op statement in this snapshot.

## 9.2 Arrays

Array types are supported syntactically:

```mak
[u8]
[i64]
```

Non-empty array value literals are not:

```mak
[1, 2, 3]    // rejected by the current expression grammar
[1]          // rejected by the current expression grammar
```

The odd exception is:

```mak
[]
```

The parser accepts `[]` as an atom and represents it as integer zero. It is **not** a real empty-array value. Do not use it as though it had an array type or array semantics.

## 9.3 Tuples

Tuple type syntax and tuple pattern syntax exist, but the expression parser does not accept tuple literals:

```mak
(i32, i32)   // tuple type in type position
(x, y)       // tuple pattern in pattern position
(x, y)       // not a tuple value expression
```

The surrounding grammar determines which of these contexts is active.

## 9.4 Unit value versus unit type

`()` is recognized as the unit type in type position. It is not parsed as a standalone unit expression. After an expression has already been parsed, `()` can instead act as postfix zero-argument application:

```mak
()       // not a standalone expression value
f()      // a zero-argument application form
```

The two spellings are not interchangeable across contexts.

## 9.5 Booleans and control flow

There are no built-in `true` and `false` expression literals, and no direct parser forms for `if`, `else`, `while`, `for`, `break`, `continue`, `return`, `try`, `catch`, `throw`, or `switch`.

`true` and `false` parse as identifiers in an expression. `case` is the built-in conditional/matching expression form represented by the parser.

## 9.6 Assignment and mutation

There is no general assignment expression such as `x = y`. The `=` token is used in declarations and descriptor entries. The AST contains immutable-shaped `let` bindings; the parser does not define a conventional mutable-variable assignment statement.

## 9.7 Access, indexing, records, and namespaces

No source expression grammar is provided for:

```text
object.field
array[index]
Type { field = value }
module::name
```

The dot operator is function composition, not field access. Brackets are type syntax and the `[]` placeholder, not indexing syntax. Braces create blocks or declaration/matching bodies; they are not general record literals.

## 9.8 Quotation

The AST includes `ExprKind::Quote`, but there is no parser rule that constructs it from ordinary MAK source in the supplied parser. Backtick quotation should not be assumed to work. Single quote is a string delimiter.

## 9.9 Other absent operators and conveniences

No syntax is provided for exponentiation `**`, concatenation `++`, pipelines such as `|>`, null coalescing `??`, optional chaining `?.`, type ascription, generic angle brackets, or comparison ranges. Do not copy these forms from another language and expect MAK to recognize them.

---

# 10. Full compact grammar

This grammar consolidates the source forms described above. It is a readable grammar sketch of the current parser, not a replacement implementation; the notes after it describe the most important parser-specific exceptions.

## 10.1 Program and declarations

```ebnf
program              = { declaration }, [ expression ], EOF ;

declaration          = let_declaration
                     | external_declaration
                     | type_declaration
                     | bring_declaration ;

let_declaration      = "let", identifier, { identifier }, "=", expression, ";" ;
external_declaration = "let", ("extern" | "external"), external_body ;
bring_declaration    = "bring", string_literal, { string_literal }, ";" ;

type_declaration     = "type", identifier, { identifier }, ";"
                     | "type", identifier, { identifier }, "{",
                       { constructor_declaration }, "}", ";" ;

constructor_declaration
                     = identifier, { type }, [ ";" ] ;
```

`let_declaration` in this compact grammar represents ordinary top-level let declarations, not the separate external-declaration branch. The ordinary top-level declaration's identifiers after its name are function-sugar parameters. The final expression may be absent; the parser then supplies integer zero.

## 10.2 External declarations and foreign types

```ebnf
external_body       = identifier, [ ":", type ], "=", [ "global" ],
                      string_literal, [ ":", foreign_type ],
                      [ "{", { descriptor_entry }, "}" ], ";" ;

descriptor_entry    = "pure", "=", identifier, [ ";" ]
                    | ("from" | "abi"), "=", string_literal, [ ";" ]
                    | identifier, "=", expression, [ ";" ] ;

foreign_type        = "(", [ foreign_arg, { ",", foreign_arg }, [ "," ] ], ")",
                      "->", foreign_type
                    | foreign_atom, [ "*" ] ;

foreign_arg         = foreign_type | "..." ;
foreign_atom        = identifier ;
```

This is the conceptual shape. Actual parser caveats: `pure`, `from`, and `abi` are checked by the text of identifiers; unknown descriptor entries parse and discard an expression; `...` is recognized as three dot tokens; only one star suffix is parsed after a foreign atom; and the parser is not a complete C ABI validator.

## 10.3 Types

```ebnf
type                = type_atom, [ "->", type ] ;

type_atom           = primitive_or_named_type
                    | "[", type, "]"
                    | "(", ")"
                    | "(", type, ")"
                    | "(", type, ",", type, { ",", type }, ")"
                    | "(", named_type, type_atom, { type_atom }, ")"
                    | "!" ;

primitive_or_named_type
                    = identifier ;
```

The right side of a function arrow recursively parses a full type, making function arrows right-associative. Type application is a special parenthesized form whose head is a named type. The primitive spellings are interpreted as primitives only in the ordinary type parser; `ptr` is a foreign type atom and is not the same rule.

## 10.4 Expressions

```ebnf
expression          = infix_expression ;

infix_expression    = prefix_expression,
                      { application_or_infix_tail } ;

prefix_expression   = ("-" | "!" | "~"), expression_at_binding_power_10
                    | atom ;

atom                = integer_literal
                    | float_literal
                    | string_literal
                    | identifier
                    | "(", expression, ")"
                    | "[", "]"
                    | block
                    | let_expression
                    | lambda_expression
                    | case_expression ;

application_or_infix_tail
                    = "(", ")"                      (* zero-argument application *)
                    | atom                            (* ordinary application *)
                    | ".", expression_at_binding_power_9
                    | binary_operator, expression_at_operator_right_binding_power ;

binary_operator     = "*" | "/" | "%" | "+" | "-"
                    | "<<" | ">>" | "&" | "^" | "|"
                    | "==" | "/=" | "<" | ">" | "<=" | ">="
                    | "&&" | "||" ;

lambda_expression   = ("fn" | "\\"), parameter_list, lambda_body ;
parameter_list      = { identifier }
                    | "(", [ identifier, { [ "," ], identifier }, [ "," ] ], ")" ;
lambda_body         = "->", expression
                    | "=>", expression
                    | block ;

let_expression      = "let", identifier, { identifier }, "=", expression,
                      ";", expression ;

case_expression     = "case", expression, "of", "{", case_arm, { case_arm }, "}" ;
case_arm            = pattern, ("->" | "=>"), expression, [ ";" ] ;

block               = "{", block_body, "}" ;
block_body          = empty_block
                    | expression, [ ";" ]
                    | expression, ";", block_body
                    | local_let, block_body ;
empty_block         = ;
local_let           = "let", identifier, { identifier }, "=", expression, ";" ;
```

Several productions above are intentionally schematic: the real expression parser uses binding powers rather than a separate grammar production for each operator; block-local `let` is parsed recursively so it encloses the remaining block body; the parser has a special empty-bracket placeholder; and empty parentheses are only a postfix form after an existing expression, not an ordinary expression atom.

## 10.5 Patterns

```ebnf
pattern             = "_"
                    | identifier_as_binding
                    | integer_literal
                    | string_literal
                    | constructor_identifier, { constructor_pattern_argument }
                    | "(", ")"
                    | "(", pattern, ")"
                    | "(", pattern, ",", pattern, { ",", pattern }, ")" ;

constructor_pattern_argument
                    = pattern ;
```

The distinction between `identifier_as_binding` and `constructor_identifier` is made by initial ASCII case, not by a separate declaration namespace rule. A one-item parenthesized pattern is grouping, not a tuple. There is no float pattern, negative integer pattern, or pattern guard.

## 10.6 Grammar caveat checklist

Before treating the EBNF above as a specification for a long-lived language, retain these exceptions:

1. The parser consumes top-level declarations only while the next token is `let`, `type`, or `bring`.
2. External declarations are `let external ...` or `let extern ...` forms.
3. An absent top-level body becomes integer `0`.
4. Top-level and local let declarations require semicolons after their values.
5. A block's last expression is its value; an empty block currently becomes integer `0`.
6. `[]` is an integer-zero placeholder, not an array literal.
7. `()` is a unit type or a zero-argument-call suffix depending on context; it is not a value literal.
8. `pass` is a keyword token without an expression rule.
9. The dot operator expands to a lambda, not field access or piping.
10. Float literals are not patterns.
11. Constructor-pattern recognition depends on uppercase initial spelling.
12. Only `->` is a type arrow and foreign function result arrow; `=>` is only accepted for expression arrows.
13. The foreign type grammar accepts more arrangements than all real C ABIs permit.
14. Parsed type forms do not guarantee every corresponding runtime representation is complete.

---

# 11. The three `ringest` programs: why their syntax differs

These three examples look similar but express different programs. Their outputs were supplied alongside the MAK source dump. The comparison below explains their source-level grouping and the significant difference that survives optimization.

## 11.1 Example A: composition where a print call was intended

Source:

```mak
let external printf : i32 = "printf" : (ptr, ...) -> i32 {
    from = "<stdio.h>";
};

printf "%d\n" . ((\x -> x*x)4)
```

### What the source says

The expression to the left of dot is already an application:

```mak
printf "%d\n"
```

The expression to the right of dot is a parenthesized application:

```mak
(\x -> x*x) 4
```

The lambda calculates the square of its argument. The optimizer can reduce this particular application to the integer `16` under the supplied example's optimization path.

But dot does not mean “append another `printf` argument.” It means function composition. The conceptual desugaring is:

```mak
\z -> (printf "%d\n") (((\x -> x*x) 4) z)
```

After the arithmetic lambda application simplifies, that shape is conceptually close to:

```mak
\z -> (printf "%d\n") (16 z)
```

The problem is `16 z`: the source attempts to apply the integer `16` as if it were a function.

### Why the supplied C is large and still prints nothing

The supplied C artifact contains a generated lambda function and runtime function-dispatch support. It does not call that lambda from `main`; `main` merely casts its function name to `void` and returns. Therefore the shown artifact produces no output.

This is the key lesson: the presence of `printf` inside a lambda does not mean that the lambda runs. The final top-level expression is a function value, and the supplied emitted `main` does not invoke it.

### Correct source shape

Use composition only when composing functions. To print the computed result, pass a format string and the computed value as ordinary arguments:

```mak
printf "%d\n" ((\x -> x * x) 4)
```

## 11.2 Example B: one argument passed to `printf`

Source:

```mak
let external printf : i32 = "printf" : (ptr, ...) -> i32 {
    from = "<stdio.h>";
};

printf ((\x -> x*x)4)
```

The expression contains an ordinary application with only one argument. The lambda application reduces to `16`, leaving the equivalent call shape:

```mak
printf 16
```

That is syntactically a one-argument call, but the external declaration describes the first C argument as `ptr` (a pointer). It is therefore not a correct call to `printf` for printing an integer.

The supplied C output illustrates the mismatch by converting the integer into a pointer-shaped value and passing it as the sole argument. It does not provide a format string and does not provide a second variadic integer argument.

**Safety warning:** passing the value `16` as the format-string pointer is invalid for this intended use. The resulting C behavior is undefined; a crash is possible, but the language specification should not promise a particular failure mode.

Correct printing needs the format argument first:

```mak
printf "%d\n" 16
```

or with the computation left visible:

```mak
printf "%d\n" ((\x -> x * x) 4)
```

## 11.3 Example C: correctly shaped variadic call

Source:

```mak
let external printf : i32 = "printf" : (ptr, ...) -> i32 {
    from = "<stdio.h>";
};

printf "%d\n" ((\x -> x * x) 4)
```

This time the call has two arguments:

1. The string `"%d\n"`, in the required first (format-string pointer) position.
2. The computed integer value `16`, corresponding to `%d`.

After constant reduction, the supplied C output has the essential shape:

```c
mak_ir_ext_0((const void *)("%d\n"), INT64_C(16));
```

The supplied mLIR similarly shows a direct call with a string constant followed by an integer constant. Assuming the external declaration and target ABI are correct, this is the intended program and should print:

```text
16
```

## 11.4 Comparison table

| Source expression | Core source meaning | Main issue or expected intent |
|---|---|---|
| `printf "%d\n" . ((\x -> x*x)4)` | Compose the function represented by `printf "%d\n"` with the value-producing expression on the right | Produces a lambda involving the invalid application `16 z`; the supplied `main` does not invoke that lambda |
| `printf ((\x -> x*x)4)` | Call `printf` with one argument, the computed value `16` | The integer occupies the format-string/pointer position; unsafe and not a valid integer-printing call |
| `printf "%d\n" ((\x -> x*x)4)` | Call `printf` with format string and computed integer | Correctly shaped call for the intended output `16` |

Whitespace between tokens does not turn application into composition or vice versa. The change from the first example to the others is the operator itself (`.` versus application); the change from the second to the third is the addition of the format-string argument.

### The supplied mLIR shapes

The first variant's supplied mLIR has a generated function and a runtime-call fallback, while the entry yields a function reference:

```text
functions {
  fn @mak_ir_fn_0 ["<lambda>"] : (%a0: i64) -> i32 {
    yield call @z0(const.str "%d\n", runtime-call : i32) : i32
  }
}
entry {
  yield @f0
}
```

The second variant's supplied mLIR reduces to a single argument for the external call:

```text
entry {
  yield call @z0(const.i64 16) : i32
}
```

The third variant's supplied mLIR has the format string and integer value as two arguments:

```text
entry {
  yield call @z0(const.str "%d\n", const.i64 16) : i32
}
```

Here `@z0`, `@f0`, and the runtime-call marker are names/features of the supplied custom mLIR output, not source-level MAK identifiers. The examples demonstrate the distinction between a top-level function value, a one-argument external call, and a two-argument variadic call. They are descriptive snapshots supplied with the question, not freshly generated by a compiler run in this documentation step.

## 11.5 The shortest practical rule

For C-style variadic printing:

```mak
printf "%d\n" expression
```

The first argument is the format string. The expression after it is the value matched by the format specifier. Do not use dot composition as a substitute for supplying another argument.

---

# 12. Readable MAK style rules

These recommendations stay within the supplied syntax; they are style guidance rather than extra grammar requirements.

## 12.1 Prefer one declaration per line

```mak
let add x y = x + y;
let square x = x * x;
let answer = add 20 22;

square answer
```

Avoid packing declarations into a single line while learning the language. A missing semicolon can otherwise make the diagnostic harder to interpret.

## 12.2 Use semicolons consistently where they matter

Always terminate top-level declarations, local let values, and `bring` declarations with semicolons. Put a semicolon after every `case` arm even though the parser permits some omitted separators.

```mak
case result of {
    Some x -> x;
    None -> 0;
}
```

A final semicolon before a closing `}` is accepted in `case` and block bodies.

## 12.3 Parenthesize application arguments that are not simple atoms

```mak
f (x + y)
f (g x)
printf "%d\n" (compute x)
```

A large percentage of difficult MAK expressions become clear once application and composition are explicitly parenthesized.

## 12.4 Use uppercase data constructors

```mak
type Result value error {
    Ok value;
    Error error;
};
```

This makes constructor declarations agree with pattern parsing's uppercase convention. Use lowercase identifiers for ordinary bindings:

```mak
case result of {
    Ok value -> value;
    Error problem -> fallback problem;
}
```

## 12.5 Keep foreign declarations explicit

```mak
let external puts : ptr -> i32 = "puts" : (ptr) -> i32 {
    from = "<stdio.h>";
    pure = false;
};
```

Use the actual linked symbol name, actual header, and accurate argument/return types. Mark a function pure only when that assertion is true. Keep every descriptor on a separate line and terminate every descriptor with a semicolon.

## 12.6 Do not rely on parser recovery quirks

Avoid malformed numeric literals, unknown FFI descriptor keys, invalid `\x` escapes, lowercase constructor patterns, `[]` as a pretend array, or malformed variadic signatures. If a malformed form happens to be tokenized or parsed, that does not make it a supported language feature.

## 12.7 Test one language feature at a time

For new syntax, create a minimal standalone file and record:

- Whether the parser accepts it.
- The result you expect.
- The result actually produced by the exact compiler build.
- Whether the generated C matches the intended meaning.

For example, test a one-argument function application separately from dot composition, and test an external function's correct format argument separately from a variadic integer argument. These small programs are easier to debug than a large example that combines many features.

---

# 13. Common mistakes and their corrections

| Mistake | Why it is wrong or surprising | Safer form |
|---|---|---|
| `printf "%d\n" . value` | Dot composes functions; it does not append an argument | `printf "%d\n" value` |
| `printf 16` | `printf` interprets its first argument as a format-string pointer | `printf "%d\n" 16` |
| `f x + y` when you intend `f (x+y)` | Application binds more tightly than addition | `f (x + y)` |
| `f . (g x)` when both `f` and `g` are meant to be functions | The right expression becomes a value applied to the new composition parameter | `f . g`, or write the intended function explicitly |
| `!=` | Not-equal is spelled `/=` | `a /= b` |
| `()` as a value | Standalone empty parentheses are not an expression atom | Return/use a value the language currently supports; use `()` only as a type or after a callee for zero-arg application |
| `[1, 2, 3]` | Non-empty array literals are unsupported | Use a supported ADT or an explicitly designed FFI allocation API |
| `(x, y)` as a computed tuple | Tuple expressions are unsupported | Use an ADT with fields or another documented representation |
| `pass` as a no-op | It is tokenized but has no parser rule | Use a supported block/body expression |
| `type option { some a; };` followed by `some x` | Lowercase `some` is parsed as a binding pattern | Rename the constructor `Some` |
| `let x: i32 = 1;` | Ordinary `let` has no type-ascription syntax | Let inference determine the representation, or express type information in a supported context |
| `bring "file.mak"` without `;` | The import declaration requires a terminator | `bring "file.mak";` |
| `type Color { Red; Green; }` without trailing `;` | A type with a body requires `;` after `}` | `type Color { Red; Green; };` |
| A `case` arm body immediately followed by another arm without a separator | The next arm's first token may be absorbed into application | Terminate each arm with `;` |
| `pure = "true";` | `pure` expects an identifier, not a string | `pure = true;` |
| `from = stdio;` | `from` expects a string literal | `from = "<stdio.h>";` |
| `i32**` as a foreign type | Only one pointer star suffix is parsed | Declare a C wrapper with a simpler supported signature when multiple indirections are required |
| `1.` or `.5` as float syntax | Decimal points require digits on both sides in the forms recognized | `1.0` and `0.5` |
| `1e+` as a valid exponent | The current conversion fallback may turn malformed input into `0.0` | Supply exponent digits, e.g. `1e+3` |

---

# 14. Worked examples

These examples illustrate the syntax forms rather than define a standard library. Names such as `print`, `fallback`, or `compute` work only if the program provides them or imports them.

## 14.1 Arithmetic and precedence

```mak
let square x = x * x;
let result = 2 + 3 * 4;

square result
```

`3 * 4` groups before addition, so `result` is intended to be `14`; the final expression applies `square` to that value.

## 14.2 Lambda and application

```mak
let twice = fn x -> x * 2;

twice (10 + 11)
```

The explicit parentheses force the sum to be the one argument.

## 14.3 Local values

```mak
let base = 10;
let incremented = base + 1;
incremented * 2
```

Each local `let` expression has a value expression followed by `;` and then a body expression.

## 14.4 Blocks with a value at the end

```mak
{
    let first = 3;
    let second = 4;
    first * first + second * second
}
```

The last expression is the block's result. The local lets each end their value with a semicolon.

## 14.5 ADT declarations and case patterns

```mak
type Maybe a {
    None;
    Some a;
};

let get_or_zero value = case value of {
    Some n -> n;
    None -> 0;
};

get_or_zero (Some 42)
```

The example demonstrates the intended surface shape for an algebraic data type, construction, and pattern matching. Whether the exact generic ADT example is fully supported by a particular MAK backend revision should be verified in the actual compiler; the parser's type grammar alone does not guarantee complete generic representation support.

## 14.6 Nested constructor patterns

```mak
case input of {
    Cons first (Cons second rest) -> first + second;
    Cons only rest -> only;
    Nil -> 0;
}
```

Constructor patterns use adjacent subpatterns. Parentheses make nested arguments explicit and reduce ambiguity.

## 14.7 Composition of functions

```mak
let increment x = x + 1;
let double x = x * 2;

increment . double
```

The final expression is a composed function value. To calculate a result from input `21`, apply the composed function explicitly:

```mak
(increment . double) 21
```

This is different from using dot after a call that has already produced a number.

## 14.8 External declaration and a formatted print call

```mak
let external printf : i32 = "printf" : (ptr, ...) -> i32 {
    from = "<stdio.h>";
};

printf "%d\n" ((\x -> x * x) 4)
```

The call has a format string as its first argument. The arithmetic lambda is applied to `4`; the intended computed value is `16`, supplied as the second argument.

This example relies on the foreign declaration being accurate for the target C environment. Foreign declarations should always be checked against the actual system prototype and target ABI.

## 14.9 Imports

```mak
bring "numbers.mak" "formatting.mak";

format_number (double_value 21)
```

The imported files need to define the names used below the declarations. The root file's body remains the expression following its own declarations.

---

# 15. Accepted syntax versus semantic guarantees

MAK's parser can accept a source shape even when a later phase cannot safely interpret it. This is especially important for a language whose backend has an FFI boundary and inferred representations.

| Feature | What syntax acceptance establishes | What it does not establish |
|---|---|---|
| Ordinary `let` | A binding name, value expression, and terminator were parsed | That every later type/representation choice is sound |
| Type declaration | A named type and constructor forms were parsed | That every generic/nested type gets a correct C layout |
| Constructor pattern | A constructor-shaped pattern and its subpatterns were parsed | That the constructor name/arity resolves to a valid declared constructor |
| Function application | A callee expression and argument expressions were grouped | That the callee is actually callable or arity/type-compatible |
| Function composition | A composition-shaped expression was desugared to a lambda | That both expressions are functions or that applying them is meaningful |
| External declaration | MAK-side and optional foreign signature syntax was parsed | That the real foreign symbol has that signature or that calls are safe |
| Variadic type | An ellipsis marker was parsed | That the C ABI, default argument promotions, or format string are correct |
| `pure = true` | The descriptor flag was set | That the external function is genuinely pure; the programmer must ensure it |
| Array type | The type form `[T]` was parsed | That an array value literal or complete array runtime is available |
| `[]` | The parser emitted integer zero | That an array was created |
| `()` | Depending on context, a unit type or zero-argument application token sequence was parsed | That there is a unit literal value |

This distinction is central to writing safe MAK. Parseability is only the first step. A good program must also have sensible source semantics, representable values, accurate foreign signatures, and verified behavior in the emitted executable.

---

# 16. Source-derived reference checklist

Use this checklist when adding or reviewing a MAK syntax feature. It is deliberately focused on the visible source language.

## File structure

- [ ] Declarations precede the final body expression.
- [ ] Top-level declarations use `let`, `type`, or `bring` as their starter.
- [ ] A top-level ordinary `let` ends with `;`.
- [ ] A type declaration with a body ends with `};`.
- [ ] An external declaration ends with `;` after any descriptor block.
- [ ] A `bring` declaration contains at least one string and ends with `;`.
- [ ] There is no accidental trailing token or top-level semicolon after the final expression.

## Expressions

- [ ] Application arguments that contain operators are parenthesized.
- [ ] Dot is used only when composing functions, not to pass another argument.
- [ ] Each block has a clear final expression.
- [ ] Each local `let` has a value, required semicolon, and body.
- [ ] Every `case` arm uses `->` or `=>` and has a body expression.
- [ ] `case` arms are separated with semicolons.
- [ ] Prefix operators are parenthesized when their interaction with application is not obvious.
- [ ] `==` and `/=` are used for equality and inequality; `=` is not used as an expression operator.

## Patterns and types

- [ ] Constructor names start uppercase.
- [ ] Constructor patterns use the declared field count.
- [ ] Float and negative integer patterns have not been used.
- [ ] Tuple types/patterns are not mistakenly used as tuple value literals.
- [ ] Unit `()` appears only in type context or as an empty-call suffix after a callee.
- [ ] Generic type application uses the parser's parenthesized type-application syntax where required.

## Foreign declarations

- [ ] The linked symbol string is correct.
- [ ] The MAK-side type is appropriate for source calls.
- [ ] The foreign type matches the real prototype.
- [ ] The `from` header is correct.
- [ ] A variadic marker is last and follows the fixed arguments conventionally.
- [ ] Integer values are not supplied where a pointer is required.
- [ ] Every `printf` call begins with a format string.
- [ ] `pure = true` is not applied to side-effecting functions.
- [ ] Every descriptor entry ends with `;`.

---

# 17. Minimal language examples for a syntax smoke test

These cases are useful for checking a MAK build after a parser change. Expected parsing follows the current source grammar; do not infer backend correctness from parse acceptance alone.

## 17.1 Literals and names

```mak
0
```

```mak
42
```

```mak
0x2a
```

```mak
0b101010
```

```mak
3.5
```

```mak
"hello\nworld"
```

## 17.2 Application and grouping

```mak
let add x y = x + y;
add 2 3
```

```mak
let add x y = x + y;
add (1 + 2) (3 + 4)
```

## 17.3 Lambdas

```mak
(fn x -> x + 1) 4
```

```mak
(fn (x, y) -> x + y) 2 3
```

This second form demonstrates syntax only. Exact multi-argument behavior depends on how the implementation handles the function's arity and application shape.

## 17.4 Local `let`

```mak
let x = 40;
x + 2
```

## 17.5 Block

```mak
{
    1;
    2;
    3
}
```

The final expression `3` is the block tail.

## 17.6 Case

```mak
case 1 of {
    0 -> 10;
    1 -> 20;
    _ -> 30;
}
```

## 17.7 Composition

```mak
let increment x = x + 1;
let double x = x * 2;
(increment . double) 10
```

## 17.8 External declaration

```mak
let external printf : i32 = "printf" : (ptr, ...) -> i32 {
    from = "<stdio.h>";
};
printf "%d\n" 42
```

This is the canonical format for the intended `printf` use in the examples covered by this document.

---

# 18. Known uncertainty and version boundary

This reference is grounded in the supplied MAK source snapshot dated 10 October 2026. It describes the parser and AST visible in that snapshot; it is not yet a formally ratified language standard.

Several aspects must remain explicitly implementation-defined until the language author chooses and documents a stronger contract:

1. Whether malformed integer prefixes and malformed floating-point exponents should be errors or retain the current permissive behavior.
2. Whether unknown string escapes should be errors, preserve their slash, or continue dropping the slash.
3. Whether descriptor keys other than `pure`, `from`, and `abi` should be rejected rather than parsed and discarded.
4. Whether empty blocks and empty array brackets should continue producing integer zero or be replaced by distinct unit/array semantics.
5. Whether the `unit` identifier should become an alias for `()` or remain a named type.
6. Whether lowercase constructor declaration names should be rejected to align declarations with constructor-pattern parsing.
7. Whether comma-omission in parenthesized lambda parameters and optional `case` separators should remain part of the accepted grammar.
8. Whether arbitrary generic type applications and nested function/tuple/array types are supported through the complete backend rather than only through the type AST.
9. The exact semantic guarantees for comparison/logical operand types, evaluation order, overflow, division, and short-circuiting.
10. The complete representation and call convention for zero-argument functions, closures, and external/global symbols.
11. Whether the internal AST `Quote` node will receive public source syntax.
12. The exact ABI and pointer-depth needs supported by the foreign type grammar.

Where the parser accepts a form due to a broad token loop, that acceptance should not automatically be treated as a designed feature. A language specification should either preserve that behavior intentionally and test it, or narrow the grammar and emit a clear error.

---

# 19. The rules worth memorizing first

If only a small part of this reference is needed while writing ordinary programs, start with these rules:

1. A file consists of top-level declarations, then one final expression.
2. Ordinary declarations start with `let`, type declarations with `type`, and imports with `bring`.
3. Top-level `let`, local `let`, external declarations, and `bring` declarations require their specified semicolons.
4. The standard external spelling is `let external name : MAK_TYPE = "symbol" : FOREIGN_TYPE { ... };`.
5. A lambda begins with `fn` or `\`, followed by identifier parameters and then `->`, `=>`, or a braced body.
6. Ordinary application is written with adjacent expressions: `f x y`.
7. Application binds more tightly than ordinary infix operators: use `f (x + y)` when the grouped expression is the argument.
8. Dot means composition: `f . g` desugars to a function shaped like `\x -> f (g x)`.
9. Binary not-equal is `/=`, not `!=`.
10. `case value of { pattern -> result; ... }` is the main built-in selection form.
11. Constructor patterns are recognized by uppercase initial letters; binders are lowercase-leading identifiers.
12. Tuple types and patterns do not imply tuple value literals.
13. `[T]` is a type form; `[]` is only a parser placeholder that becomes integer zero, and `[1,2]` is not a supported array literal.
14. `()` is not a standalone expression value; after a callee it denotes zero-argument application.
15. Foreign type syntax must match the real ABI. In particular, `printf` needs a format-string pointer before the values it formats.
16. When unsure, add parentheses and semicolons, use uppercase constructors, and keep external declarations explicit.

---

## Closing note

The reliable way to write MAK is to distinguish three things: the syntax the parser accepts, the meaning you intend the program to have, and the behavior the generated executable actually demonstrates. This reference documents the first carefully, highlights where the second and third can diverge, and uses the three `ringest` examples as a compact demonstration of why those distinctions matter.
