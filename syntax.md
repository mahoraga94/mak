# MAK Language Syntax Reference

> **Scope of this document**
>
> This reference describes the MAK syntax implemented by the lexer and parser in the supplied source. It is intentionally specific to that implementation. Where the compiler accepts an unusual form, the notes below distinguish what the parser actually does from the form recommended for everyday code. Features that appear only in internal compiler data structures are not treated as public language syntax.

## Contents

1. [Quick start](#1-quick-start)
2. [Source-file structure](#2-source-file-structure)
3. [Whitespace, comments, and identifiers](#3-whitespace-comments-and-identifiers)
4. [Keywords and punctuation](#4-keywords-and-punctuation)
5. [Numeric literals](#5-numeric-literals)
6. [String literals and escapes](#6-string-literals-and-escapes)
7. [Expressions at a glance](#7-expressions-at-a-glance)
8. [Operator precedence and associativity](#8-operator-precedence-and-associativity)
9. [Variables and application](#9-variables-and-application)
10. [Function expressions](#10-function-expressions)
11. [Let expressions and bindings](#11-let-expressions-and-bindings)
12. [Blocks and sequencing](#12-blocks-and-sequencing)
13. [Unary and binary operators](#13-unary-and-binary-operators)
14. [Function composition with `.`](#14-function-composition-with)
15. [Indexing](#15-indexing)
16. [Algebraic data types](#16-algebraic-data-types)
17. [Case expressions and patterns](#17-case-expressions-and-patterns)
18. [Type syntax](#18-type-syntax)
19. [External declarations and foreign types](#19-external-declarations-and-foreign-types)
20. [Importing declarations with `bring`](#20-importing-declarations-with-bring)
21. [Compact grammar](#21-compact-grammar)
22. [Features that are not source syntax](#22-features-that-are-not-source-syntax)
23. [Compiler-driver syntax](#23-compiler-driver-syntax)
24. [Practical examples](#24-practical-examples)
25. [Implementation notes and safe assumptions](#25-implementation-notes-and-safe-assumptions)

---

## 1. Quick start

A MAK source file contains zero or more top-level declarations followed by one expression. A small program can look like this:

```mak
let square x = x * x;
let sum_of_squares x y = square x + square y;

sum_of_squares 3 4
```

A declaration ends in a semicolon. The final program expression normally does not. Line breaks and indentation improve readability, but they do not delimit declarations or create blocks; punctuation does that work.

A function can also be written explicitly:

```mak
let square = fn (x) -> x * x;
square 9
```

The language uses whitespace-based function application (`square 9`) rather than conventional comma-separated calls. A pair of empty parentheses is a special zero-argument call form (`start()`).

## 2. Source-file structure

### 2.1 Overall shape

```mak
[declaration ...]
program_expression
```

Top-level declarations are recognized at the beginning of the file. The supported declaration forms are:

- `let` value or function bindings;
- `type` declarations;
- `bring` import declarations;
- external declarations, written as `let extern ...` or `let external ...`.

The external declaration is parsed through the `let` declaration form, so it is not written as a bare top-level `extern` declaration.

### 2.2 A file with several declarations

```mak
bring "math.mak";

type Result a {
    Err i32;
    Ok a;
};

let twice x = x + x;
let answer = twice 21;

answer
```

Top-level declarations may be separated by whitespace or newlines, but each declaration has its own required terminating semicolon. The main expression is parsed after the run of declarations.

### 2.3 A file without a final expression

The current parser supplies the integer expression `0` when the file ends immediately after declarations (or when the source is empty). This is a parser fallback, not a separate unit-expression syntax. For clear, portable source, write the intended final expression explicitly.

### 2.4 Semicolons are significant

Semicolons terminate declarations and separate expressions in blocks and case arms. A final top-level expression is normally written without a semicolon:

```mak
let base = 10;
base + 5
```

A semicolon after the top-level expression leaves a trailing token and is rejected by the current parser:

```mak
base + 5;  // Do not use this form as the final top-level expression.
```

Inside a block or case expression, the final semicolon is optional; see those sections for the details.

## 3. Whitespace, comments, and identifiers

### 3.1 Whitespace

Spaces, tabs, carriage returns, and line feeds are ignored between tokens. There is no indentation-sensitive syntax. For example, these declarations have the same token structure:

```mak
let inc x = x + 1;
```

```mak
let
    inc
    x
    =
    x + 1
;
```

The first form is easier to read, but the second illustrates that whitespace itself is not a separator between the name, parameters, and `=`.

### 3.2 Line comments

A line comment starts with `//` and continues until a newline or the end of the input:

```mak
let count = 3; // a short comment
count + 1
```

### 3.3 Block comments

Block comments begin with `/*` and end with `*/`. Block comments can nest:

```mak
/* outer comment
   /* nested comment */
   back in the outer comment
*/
let value = 8;
value
```

An unterminated block comment is a lexical error.

### 3.4 Identifier spelling

The lexer uses ASCII identifier rules:

- The first character must be an ASCII letter (`A`–`Z` or `a`–`z`) or underscore (`_`).
- Later characters may be ASCII letters, digits (`0`–`9`), or underscores.
- Names are case-sensitive.

Examples of valid identifier spellings include `x`, `sum2`, `_temp`, `Result`, and `read_file`.

Examples such as `2nd`, `my-name`, `hello.world`, or an identifier containing a non-ASCII character are not single valid identifiers. A dot is a separate operator token; a hyphen is a minus token.

The words in the keyword table are reserved as tokens, even when a particular keyword has no implemented source construct. `external` and `global` are handled as context-sensitive identifier spellings in the specific declaration positions described later; they are not general-purpose keywords.

## 4. Keywords and punctuation

### 4.1 Reserved keywords

| Keyword | Role in the parser |
|---|---|
| `let` | Value binding, local binding, or the prefix of an external declaration |
| `fn` | Function expression |
| `case` | Pattern-match expression |
| `type` | Type declaration |
| `extern` | External declaration marker after `let` |
| `of` | Introduces the arms of a `case` expression |
| `bring` | Imports declarations from other files |
| `pass` | Reserved by the lexer, but no standalone source construct is implemented in the parser |

`external` is accepted as an alternative spelling for `extern` immediately after `let`. For example, `let external puts = ...;` is recognized in the same declaration position as `let extern puts = ...;`.

### 4.2 Punctuation and multi-character tokens

| Token | Meaning or common use |
|---|---|
| `{` `}` | Blocks, data-type bodies, case-arm bodies |
| `(` `)` | Grouping, function parameter lists, tuple/type grouping, empty-call syntax |
| `[` `]` | Type-level arrays and expression indexing |
| `;` | Declaration terminator or expression separator |
| `,` | Parameter separators, tuple types and patterns, foreign argument separators |
| `:` | Type annotation position in external declarations |
| `.` | Function composition; it is **not** member access |
| `\` | Alternative introducer for a function expression |
| `->` | Function/type arrow and case-arm separator |
| `=>` | Alternative function-body or case-arm arrow |
| `=` | Binding or descriptor assignment |
| `+` `-` `*` `/` `%` | Arithmetic operators (with `-` also unary) |
| `&` `|` `^` | Bitwise AND, OR, and XOR |
| `<<` `>>` | Shifts |
| `==` `/=` `<` `>` `<=` `>=` | Comparisons; `/=` is the not-equal token |
| `&&` `||` | Logical conjunction and disjunction |
| `!` `~` | Logical-not and bitwise-not in expressions; `!` also denotes the bottom type in a type position |

**Not equal is spelled `/=` in this implementation.** The lexer does not define `!=` as a single not-equal operator.

## 5. Numeric literals

The lexer produces integer values stored as unsigned 64-bit values and floating-point values parsed as 64-bit floats. The eventual type of a literal depends on semantic analysis and its context; the spelling alone does not attach an explicit MAK type.

### 5.1 Decimal integers

```mak
0
7
42
1_000_000
```

Underscores are accepted within the decimal digit run and are ignored when accumulating the integer. The lexer does not strictly validate conventional underscore placement, so the examples above show the recommended style rather than every strange spelling the implementation might accept.

### 5.2 Hexadecimal integers

Hexadecimal notation begins with `0x` or `0X`:

```mak
0x0
0x2A
0XFF
```

Hexadecimal digits may be uppercase or lowercase. Underscore separators are not processed in the hexadecimal scanning branch; avoid placing them in these literals.

### 5.3 Binary integers

Binary notation begins with `0b` or `0B`:

```mak
0b0
0b1
0b101010
```

Only `0` and `1` are consumed as binary digits. As with hexadecimal input, do not use underscore separators in this form.

### 5.4 Floating-point literals

A decimal point is part of a floating literal when it is followed immediately by at least one decimal digit. An exponent may use `e` or `E` and an optional sign:

```mak
0.5
12.75
3.0e8
1.25E-3
6e2
```

The forms `12.` and `.5` are not conventional floating-literal forms in this lexer: a float's decimal point must follow digits and itself be followed by a digit. Use `12.0` rather than `12.`.

The current lexer is permissive about exponent syntax and does not insist that exponent digits follow `e` or `e+`. A failed floating-point conversion falls back to `0.0`. Do not rely on malformed numeric spellings; use complete exponents such as `1e3` or `1e-3`.

### 5.5 Negative numbers

There is no separate negative-number token. A leading minus is parsed as the unary operator applied to a numeric expression:

```mak
-12
-0.25
```

This matters when a negative expression is used as an application argument. Write `f (-1)` instead of `f -1`, because `-` is also a binary subtraction operator and is not the start of an application atom.

### 5.6 Literal limits and suffixes

The lexer does not support C-style numeric suffixes such as `42u32` or `1.0f`; those spellings are not single numeric tokens. Integer accumulation uses a 64-bit value and the source lexer does not issue a dedicated overflow diagnostic. Keep literals within the intended range and express type requirements using the language's type system rather than suffixes.

## 6. String literals and escapes

### 6.1 Quote styles

Both single-quoted and double-quoted strings are recognized:

```mak
"hello"
'hello'
"a string with 'single quotes' inside"
' a string with "double quotes" inside '
```

A string ends at the next matching quote. To include that quote character within the string, escape it.

### 6.2 Escape sequences

For ordinary string expressions and string patterns, the decoder recognizes these escapes:

| Escape | Decoded value |
|---|---|
| `\n` | Newline |
| `\t` | Tab |
| `\r` | Carriage return |
| `\\` | Backslash |
| `\"` | Double quote |
| `\'` | Single quote |
| `\0` | A zero byte |
| `\xHH` | One byte represented by two hexadecimal digits `H` |

Examples:

```mak
let newline = "first line\nsecond line";
let tabbed = 'name\tvalue';
let byte = "\x41";
let quote = "say \"hello\"";
quote
```

For an otherwise unrecognized escape in these decoded string values, the decoder drops the backslash and retains the following character. Prefer the explicit escapes in the table rather than depending on that fallback. The `bring` path is decoded too. By contrast, the current parser reads the external symbol name and the `from`/`abi` descriptor strings directly from the raw string body; avoid escape sequences in those fields unless you have verified how the target compiler uses them.

The lexer stores string contents as bytes. Source code is scanned byte-by-byte, so do not assume that the syntax layer performs Unicode normalization or validates every string as a sequence of Unicode scalar values.

### 6.3 String-pattern matching

String literals may also appear as patterns in `case` expressions:

```mak
case input of {
    "yes" -> 1;
    "no"  -> 0;
    _     -> -1
}
```

### 6.4 Embedded zero byte

The source terminator is a NUL byte, so a literal source NUL is not a normal character inside source text. The `\0` escape is the supported way to represent a zero byte in a decoded string value.

## 7. Expressions at a glance

The parser supports these source-level expression families:

| Form | Example |
|---|---|
| Integer, float, or string literal | `12`, `2.5`, `"text"` |
| Variable reference | `count` |
| Parenthesized expression | `(x + y)` |
| Block expression | `{ let x = 1; x + 2 }` |
| Function expression | `fn (x) -> x + 1` |
| Backslash function expression | `\x -> x + 1` |
| Local `let` expression | `let x = 4; x * x` |
| `case` expression | `case n of { 0 -> 1; _ -> 2 }` |
| Function application | `f x`, `f x y`, `f()` |
| Indexing | `items[i]`, `matrix[r][c]` |
| Unary expression | `-x`, `!flag`, `~mask` |
| Binary expression | `x + y`, `x == y`, `x && y` |
| Function composition | `f . g` |

Some familiar-looking constructs are deliberately absent from the parser's expression forms. There are no array-literal brackets, tuple expression literals, record literals, member-access expressions, or dedicated `if`, `for`, or `while` keywords in the source grammar described here.

## 8. Operator precedence and associativity

The table below runs from tighter binding to looser binding. Application is unusual because it is implicit: adjacent atoms are combined before the parser considers an infix operator. Unary operators parse an operand that can include application, but stop before the dot/composition layer and binary operators.

| Binding strength | Form | Associativity / note |
|---:|---|---|
| Tightest | Postfix indexing: `x[i]` | Repeated postfix indexing, e.g. `x[i][j]` |
| Very tight | Implicit application: `f x` | Repeated applications are built left-to-right: `(f x) y` |
| Very tight | Empty call: `f()` | Special zero-argument application form |
| Prefix | `-x`, `!x`, `~x` | Prefix operand includes application, so `-f x` parses as `-(f x)` |
| 9 | `.` | Function composition; right-associative in the parser |
| 7 | `*`, `/`, `%` | Left-associative |
| 6 | `+`, `-` | Left-associative |
| 5 | `<<`, `>>` | Left-associative |
| 4 | `&`, `^`, `|` | Same precedence level; left-associative |
| 3 | `==`, `/=`, `<`, `>`, `<=`, `>=` | Same precedence level; left-associative |
| 2 | `&&` | Left-associative |
| 1 | `||` | Left-associative |

### 8.1 Parentheses for clarity

Because application is based on adjacent atoms and unary operators absorb application into their operand, parentheses are the clearest way to make a complex argument explicit:

```mak
f (x + 1)
f (-x)
(f . g) value
```

### 8.2 Operators do not all mean the same kind of operation

`&`, `|`, and `^` are the bitwise operators. `&&` and `||` are the logical operators. The parser assigns them different tokens and different precedence levels.

The parser accepts the comparison operators at one shared precedence level; avoid long, unparenthesized comparison chains when the grouping matters. Write `(a < b) && (b < c)` rather than relying on a chained comparison to express a mathematical relation.

## 9. Variables and application

### 9.1 Variable references

An identifier used as an expression is a variable reference:

```mak
let width = 8;
width * 2
```

A referenced name must be available as a local binding, a top-level value/external binding, or a data constructor recognized by semantic analysis. An arbitrary unknown identifier is not automatically a string, enum value, or implicit global.

### 9.2 Implicit function application

A function is applied by putting its argument after it:

```mak
inc 5
add 10 20
map transform values
```

The parser consumes one following atom per application step. Thus `f x y` is grouped as repeated application, conceptually `(f x) y`.

An argument that contains a binary operator should usually be parenthesized:

```mak
scale (amount + bonus)
```

Without parentheses, the operator may be parsed as an operator on the application as a whole.

### 9.3 Grouped arguments are not comma-separated calls

Parentheses group one expression; they do not form a normal multi-argument call list:

```mak
f (x + y)     // One grouped argument
f x y         // Two successive applications
f()           // Special empty-call form
```

`f(x, y)` is not the ordinary way to call a two-argument function and is not supported as a comma-separated argument list. Commas belong to parameter lists, tuple types/patterns, and foreign argument lists.

### 9.4 The special `f()` form

The parser recognizes an empty pair of parentheses directly after an expression as a zero-argument call:

```mak
start()
factory()  // Then another application can follow if appropriate.
```

This special case applies to an empty `()`. A non-empty `(expr)` after a function is parsed as an ordinary grouped application argument, not as a parenthesized argument list.

## 10. Function expressions

A function can be introduced by `fn` or by a backslash (`\`). Function parameters are identifiers. A function body can be a braced block or an expression introduced by `->` or `=>`.

### 10.1 `fn` with a braced body

```mak
fn (x) {
    x + 1
}
```

With parentheses, parameter names may be separated by commas; the parser also accepts adjacent names without commas:

```mak
fn (left, right) { left + right }
fn (left right) { left + right }
```

The comma-separated spelling is recommended for readability.

### 10.2 `fn` with an expression body

```mak
fn (x) -> x + 1
fn (x) => x + 1
fn (left, right) -> left + right
```

`->` and `=>` are both accepted before a function expression's body. These arrows do not have identical status everywhere: type function arrows use `->` only.

### 10.3 Parameters without parentheses

The parser also accepts an unparenthesized run of parameter identifiers:

```mak
fn x -> x + 1
fn x y -> x + y
```

For unparenthesized parameters, write names separated by whitespace; commas are not part of this form.

### 10.4 Backslash form

A backslash is an alternative function-expression introducer:

```mak
\x -> x + 1
\(x, y) => x * y
\x { x + 1 }
```

It uses the same parameter parsing and body rules as `fn`.

### 10.5 No return-type annotation on a function expression

The function-expression grammar does not include a return-type annotation between the parameter list and the body. Types can appear in type declarations and external declarations, but not as a colon annotation in an ordinary `fn` parameter list.

## 11. Let expressions and bindings

MAK has top-level `let` declarations and expression-level `let` bindings. Both forms can introduce a plain value or use a list of parameter names to define a function.

### 11.1 Top-level value binding

```mak
let answer = 42;
answer
```

A top-level `let` declaration requires a semicolon after the value.

### 11.2 Top-level function shorthand

```mak
let add x y = x + y;
let multiply x y = x * y;
add 3 (multiply 2 4)
```

The function-shorthand form is parsed by collecting identifiers between the binding name and `=`. The value expression is then wrapped in a function with those parameters. No commas are used in this declaration shorthand.

A simpler example is:

```mak
let double x = x * 2;
let add_one x = x + 1;
add_one (double 10)
```

### 11.3 Local `let` expression

A local binding has the shape `let name = value; body`:

```mak
let x = 6; x * x
```

The binding's value ends at the semicolon. The expression after the semicolon is the body in which the new name is available.

### 11.4 Local function shorthand

```mak
let multiply x y = x * y; multiply 6 7
```

The parser treats the parameter names before `=` as a function definition, just as it does for the top-level shorthand.

### 11.5 Shadowing and nesting

A local `let` is an expression, so it can be nested and used wherever an expression is accepted:

```mak
let x = 10;
let y = let x = 3; x + 1;
x + y
```

The inner `x` is scoped to its local body; the outer `x` remains available after that body. Name-resolution checks are performed after parsing.

### 11.6 A binding is not an assignment statement

The grammar does not define a general reassignment operator. `=` belongs to declarations and descriptor entries. In expression position, use `let` to introduce a binding instead of writing assignment-like code such as `x = x + 1`.

## 12. Blocks and sequencing

A block is an expression enclosed in braces:

```mak
{
    let first = 10;
    let second = 20;
    first + second
}
```

A block can contain expressions separated by semicolons. Its final expression supplies the block's value.

### 12.1 Empty blocks

The parser lowers an empty block `{}` to the integer expression `0`. It does not parse it as a unit literal.

### 12.2 Semicolon-separated expressions

```mak
{
    log_value 1;
    log_value 2;
    100
}
```

The semicolons separate expressions before the final value. If the block has preceding expressions and a final expression without a semicolon, the latter is the tail expression.

The parser also treats a final semicolon-terminated expression as the tail when the closing brace immediately follows it. Still, the clearest style is to omit the semicolon after the intended final value.

### 12.3 `let` inside a block

Inside a braced block, a local `let` binding extends over the rest of the block:

```mak
{
    let x = 12;
    let y = x + 1;
    x * y
}
```

A local `let` requires a semicolon after its value. The rest of the enclosing block becomes its body. In contrast to an ordinary expression statement, a `let` form directly structures the remaining block as a scoped body.

### 12.4 Blocks are expressions

A block may appear where any other expression may appear, including as a function body or an application argument:

```mak
let compute = fn (x) {
    let doubled = x * 2;
    doubled + 3
};
compute 9
```

## 13. Unary and binary operators

### 13.1 Unary operators

| Operator | Parser-level operation | Example |
|---|---|---|
| `-` | Numeric negation | `-x` |
| `!` | Logical-not | `!flag` |
| `~` | Bitwise complement | `~mask` |

Unary operators are prefix operators. The parser keeps application within the unary operand, so `-f x` groups as `-(f x)`. Parenthesize when you mean to negate a value first and then pass it to another function.

### 13.2 Arithmetic operators

| Operator | Conventional reading | Example |
|---|---|---|
| `*` | Multiplication | `a * b` |
| `/` | Division | `a / b` |
| `%` | Remainder | `a % b` |
| `+` | Addition | `a + b` |
| `-` | Subtraction | `a - b` |

The parser establishes precedence and builds the corresponding operator expression. Operand types and runtime behavior are governed by semantic analysis and code generation.

### 13.3 Shifts and bitwise operators

| Operator | Conventional reading |
|---|---|
| `<<` | Left shift |
| `>>` | Right shift |
| `&` | Bitwise AND |
| `^` | Bitwise XOR |
| `|` | Bitwise OR |

Example:

```mak
let low_byte value = value & 0xFF;
low_byte 0x1234
```

### 13.4 Comparisons

| Operator | Reading |
|---|---|
| `==` | Equal |
| `/=` | Not equal |
| `<` | Less than |
| `>` | Greater than |
| `<=` | Less than or equal |
| `>=` | Greater than or equal |

Example:

```mak
let is_small x = x < 10;
is_small 7
```

The lexer and AST recognize these operators, but this syntax reference does not assume a particular boolean literal spelling: `true` and `false` are not lexer keywords or dedicated literal tokens in the current parser.

### 13.5 Logical operators

| Operator | Reading |
|---|---|
| `&&` | Logical conjunction |
| `||` | Logical disjunction |

`&&` binds more tightly than `||`. When the intended grouping is important, use parentheses:

```mak
(a < b) && (b < c)
(a == b) || (a == c)
```

### 13.6 Operators that are not defined here

The lexer/parser does not define assignment operators such as `+=`, a dedicated `!=` comparison token, increment/decrement operators, exponentiation, a ternary operator, or field-access syntax. Do not infer their support from similar-looking languages.

## 14. Function composition with `.`

The dot token is a function-composition operator, not member access. The parser translates:

```mak
f . g
```

into the equivalent function shape:

```mak
fn ($x) -> f (g $x)
```

In other words, the right-hand function is applied first and its result is passed to the left-hand function.

Composition is right-associative in the current parser. For example, `f . g . h` groups as `f . (g . h)`. Because `.` is not a field separator, expressions such as `record.field` are not parsed as ordinary property lookup.

Use whitespace around dot in source code for readability:

```mak
let inc x = x + 1;
let twice x = x * 2;
let twice_after_inc = twice . inc;
twice_after_inc 4
```

The example defines the composed function, then applies it to `4`.

## 15. Indexing

### 15.1 Basic indexing

Indexing is written as a postfix expression:

```mak
items[index]
```

The indexing expression is parsed as an application of a compiler-provided intrinsic called `$mak.index`. That internal name is an implementation detail; source programs should use bracket syntax rather than writing the intrinsic name directly.

### 15.2 Chained indexing

Index suffixes can be repeated:

```mak
matrix[row][column]
```

The expression is grouped as indexing the result of the first indexing operation. This is the syntactic form used for multi-level pointer or array-like access in supported contexts.

### 15.3 Index expression

The bracket contents must be a non-empty expression:

```mak
values[i + 1]
values[lookup key]
```

The index is closed by `]`. An empty index is rejected.

### 15.4 No array literals

Square brackets do not introduce array literal expressions. These forms are not supported as expression literals:

```mak
[]
[1, 2, 3]
```

Square brackets *are* used in type syntax to express an array type, such as `[u8]`; see [Type syntax](#18-type-syntax).

## 16. Algebraic data types

A `type` declaration introduces a type name and optionally a set of constructors. The parser supports type parameter names after the type name.

### 16.1 Type declaration with constructors

```mak
type Option a {
    None;
    Some a;
};
```

Each constructor starts with an identifier, followed by zero or more constructor argument types, and is normally terminated with a semicolon. The closing type brace is followed by a semicolon as well.

A multi-constructor example:

```mak
type Shape {
    Circle f64;
    Rectangle f64 f64;
    Point;
};
```

The example shows the declaration grammar only. Whether a given combination of types is accepted and how constructors are represented is determined by semantic analysis and code generation.

### 16.2 Type parameters

Type parameter names are listed after the declared type name:

```mak
type Pair a b {
    PairValue a b;
};
```

The parser collects consecutive identifiers as parameters. There is no comma requirement in this position.

### 16.3 Abstract or constructor-free type declaration

A declaration may end with a semicolon without a body:

```mak
type Handle;
type OpaqueValue a;
```

This syntax records a type declaration with no constructors in the parser's AST. It is useful as an opaque/abstract declaration shape, but additional semantic or backend constraints may apply.

### 16.4 Constructor arguments are types

Constructor fields are written as a sequence of type expressions, not `name: type` fields:

```mak
type Either a b {
    Left a;
    Right b;
};
```

The grammar does not provide record-style named fields. Separate constructors with semicolons so the parser can tell where one constructor's argument types end and the next constructor begins.

### 16.5 Constructor naming in patterns

The pattern parser uses the first character of an identifier to decide whether it is a constructor pattern or a variable binding:

- An identifier beginning with an uppercase ASCII letter is parsed as a constructor pattern.
- A lowercase identifier is parsed as a binding pattern.
- `_` is parsed as a wildcard.

Use uppercase constructor names consistently (for example, `Some`, `None`, or `Rectangle`) if they are to be matched in `case` expressions.

## 17. Case expressions and patterns

A `case` expression evaluates a scrutinee and lists patterns with corresponding branch expressions:

```mak
case value of {
    0 -> 100;
    _ -> 0
}
```

The arrows `->` and `=>` are both accepted between an arm's pattern and its body.

### 17.1 General form

```mak
case expression of {
    pattern -> expression;
    pattern -> expression;
    pattern -> final_expression
}
```

The semicolon after each arm is accepted but optional immediately before `}`. In ordinary multi-arm code, include semicolons between arms so the boundary is unambiguous.

### 17.2 Wildcard pattern

The underscore pattern matches without introducing a named binding:

```mak
case value of {
    _ -> 0
}
```

### 17.3 Binding pattern

A lowercase identifier binds the matched value for use inside that arm's body:

```mak
case value of {
    result -> result + 1
}
```

This is syntactically a binding pattern. Whether the example has the intended meaning depends on the scrutinee type and semantic rules.

### 17.4 Integer and string patterns

Integer and string literals can appear directly as patterns:

```mak
case code of {
    0 -> "zero";
    1 -> "one";
    _ -> "other"
}
```

Float literals are not included in the parser's pattern forms. Negative integer patterns are not a dedicated pattern form either, because a pattern can consume an integer token but does not parse a unary minus.

### 17.5 Constructor patterns

A constructor pattern begins with an uppercase identifier. It may be followed by patterns for its fields:

```mak
case option of {
    None -> 0;
    Some value -> value
}
```

For a constructor with multiple fields:

```mak
case shape of {
    Circle radius -> radius;
    Rectangle width height -> width * height;
    Point -> 0
}
```

The constructor's declared field types and the number of supplied subpatterns must agree with the type's meaning; parsing a pattern does not by itself guarantee that it is a valid match.

### 17.6 Tuple patterns

Parenthesized tuple patterns use commas:

```mak
case pair of {
    (left, right) -> left + right
}
```

The parser also recognizes the empty tuple pattern `()`. A single pattern in parentheses is grouping rather than a one-element tuple:

```mak
(x)       // The pattern x, grouped.
(x, y)    // A two-element tuple pattern.
()        // Empty tuple pattern.
```

A trailing comma such as `(x,)` is not accepted by the current parser.

### 17.7 Pattern forms at a glance

| Pattern | Example | Meaning at syntax level |
|---|---|---|
| Wildcard | `_` | Match without a binding |
| Binding | `value` | Bind the value to a name |
| Integer literal | `0` | Literal pattern |
| String literal | `"ok"` | Literal pattern |
| Constructor | `Some x` | Constructor and nested pattern(s) |
| Tuple | `(x, y)` | Tuple pattern |

There is no parser production for float patterns, list/array patterns, or an explicit OR-pattern such as `A x | B x`.

## 18. Type syntax

Type syntax is used in data-type constructors and external declaration annotations. Ordinary `let` bindings and function-expression parameters do not have a general inline type-annotation form in the parser.

### 18.1 Primitive types

The parser recognizes these primitive type names:

| Type | Category implied by the spelling |
|---|---|
| `u8` | Unsigned 8-bit integer |
| `u16` | Unsigned 16-bit integer |
| `u32` | Unsigned 32-bit integer |
| `u64` | Unsigned 64-bit integer |
| `i8` | Signed 8-bit integer |
| `i16` | Signed 16-bit integer |
| `i32` | Signed 32-bit integer |
| `i64` | Signed 64-bit integer |
| `f32` | 32-bit floating-point type |
| `f64` | 64-bit floating-point type |
| `()` | Unit type |
| `!` | Bottom type |

These spellings are recognized as primitive types in a type position. In an ordinary expression, the same identifier-shaped words are parsed as variable names unless another language rule applies.

### 18.2 Named types

An identifier that is not one of the primitive spellings is parsed as a named type:

```mak
Widget
Result
MyHandle
```

This includes declared type names and type-parameter names.

### 18.3 Array types

Square brackets form an array type:

```mak
[u8]
[i32]
[[u8]]
```

The element may itself be any supported type expression. The brackets here are type syntax; they do not provide expression-level array construction.

### 18.4 Tuple types

Tuple types use a comma-separated sequence in parentheses:

```mak
(i32, i32)
(u8, [u8], f64)
```

At least two elements are required by the tuple branch in the parser. `()` is instead the primitive unit type, and `(T)` is grouping around a single type.

### 18.5 Parenthesized types

A type can be grouped with parentheses:

```mak
(i32 -> i32)
([u8])
```

The parentheses around one type do not create a tuple.

### 18.6 Type application

A parameterized type application is written as a parenthesized type name followed by one or more type atoms:

```mak
(List a)
(Option i32)
(Either [u8] i32)
```

The head of this form must be a named type. The parentheses matter: a bare sequence such as `Option a` is not parsed as a type application by the type parser. Within an application, arguments are type atoms, so wrap a more complex argument in parentheses or use its bracketed type form where appropriate.

### 18.7 Function types

The arrow `->` forms a function type:

```mak
i32 -> i32
u8 -> i64 -> f64
(i32, i32) -> i32
```

Function types associate to the right. Thus `u8 -> i64 -> f64` means `u8 -> (i64 -> f64)`.

The type grammar uses `->`, not `=>`.

### 18.8 Bottom type

The token `!` is parsed as a bottom type:

```mak
! 
i32 -> !
```

Its exact typing and backend significance are compiler semantics, not just a punctuation rule. In particular, the current external-declaration parser uses this type internally when the MAK-side type annotation is omitted; see [External declarations](#19-external-declarations-and-foreign-types).

### 18.9 Type syntax examples

```mak
type Maybe a {
    Nothing;
    Just a;
};

type Callback a b;

type Pair a b {
    PairValue a b;
};

let extern compare_i32 : i32 -> i32 -> i32 = "compare_i32" : (i32, i32) -> i32;
0
```

The external example also illustrates that MAK-side function types and foreign function signatures are different grammars; see the next section.

## 19. External declarations and foreign types

External declarations connect a MAK name to a symbol outside the MAK source program. They start with `let extern` or `let external`.

### 19.1 General form

```mak
let extern name [: MakType] = [global] "symbol" [: ForeignType] [ { descriptors } ];
```

Square brackets in this template indicate optional parts; they are not literal tokens.

- `name` is the source-level name used by MAK expressions.
- `MakType` is an optional MAK type annotation.
- `global` marks an external global symbol rather than an ordinary external function/value.
- `"symbol"` is the external symbol name.
- `ForeignType` is an optional low-level foreign signature/type.
- The descriptor block is optional.
- The declaration ends with a semicolon.

A function example:

```mak
let extern putchar : i32 -> i32 = "putchar" : (i32) -> i32;
putchar 65
```

A declared source name does not have to be spelled the same way as the linked symbol:

```mak
let extern print_char : i32 -> i32 = "putchar" : (i32) -> i32;
print_char 65
```

### 19.2 MAK-side and foreign-side types

The optional type before `=` is a MAK type. The optional type after the symbol string's colon is a foreign type. They serve different roles and should not be confused.

```mak
let extern read_value : i32 -> i32 = "c_read_value" : (i32) -> i32;
```

Here the MAK-side declaration is written as a MAK function type, while the foreign side is written using the foreign function-signature grammar.

### 19.3 The `global` marker

The optional identifier `global` appears immediately before the symbol string:

```mak
let extern errno_value : i32 = global "errno" : i32;
```

The marker is recognized only in this declaration position. It is used to mark the external as a global symbol; it is not a general source keyword.

### 19.4 Omitting the MAK-side annotation

The parser accepts an external declaration without `: MakType`:

```mak
let extern stop_now = "stop_now" : () -> void;
```

In the current implementation, omission stores the bottom type (`!`) as the MAK-side type, and downstream code uses that information when determining whether the external is non-returning. Therefore, do not omit the MAK-side annotation for an ordinary returning external unless the compiler behavior is what you intend. Prefer an explicit annotation for normal functions and values.

### 19.5 Omitting the foreign type

If the foreign type after the symbol string is omitted, the parser defaults it to `void`:

```mak
let extern opaque_call : ! = "opaque_call";
```

This is syntactically allowed, but may not describe the actual foreign symbol adequately. Use an explicit foreign type when the ABI needs arguments, a return value, or a pointer type.

### 19.6 Foreign type atoms

The built-in foreign type atoms are:

- `i8`, `i16`, `i32`, `i64`;
- `u8`, `u16`, `u32`, `u64`;
- `f32`, `f64`;
- `ptr`;
- `void`;
- a named identifier for a foreign named type.

Examples:

```mak
ptr
i32
void
MyCStruct
```

Foreign types are separate from MAK types. For example, `ptr` is a foreign type atom; it is not one of the primitive MAK types listed in the previous section.

### 19.7 Pointer types

A foreign type atom may be followed by one or more pointer stars:

```mak
u8*
i32**
MyCStruct***
```

Each `*` adds one pointer layer. Repeated pointer stars are intentionally retained in the parsed type representation.

### 19.8 Foreign function signatures

A foreign function type uses a parenthesized, comma-separated argument list, followed by `->` and a return type:

```mak
(i32) -> i32
(i32, i32) -> i32
(ptr, u64) -> void
() -> i32
```

Unlike ordinary MAK application, foreign argument lists use parentheses and commas. A return type is required after the arrow.

### 19.9 Variadic foreign functions

A foreign function argument list may contain `...` to mark a variadic argument position:

```mak
(i32, ...) -> i32
```

The parser recognizes three consecutive dot tokens for this form. Whether a particular variadic signature is valid for the target ABI and code generator is a separate question; use the form only when it matches the foreign declaration being bound.

### 19.10 Descriptor blocks

A descriptor block follows the foreign type (or the symbol name if no foreign type is written):

```mak
let extern write_text : i32 -> i32 = "write_text" : (ptr) -> i32 {
    pure = false;
    from = "stdio.h";
    abi = "C";
};
```

The parser recognizes these descriptor keys:

| Key | Expected value syntax | Parsed purpose |
|---|---|---|
| `pure` | Identifier, conventionally `true` or `false` | Marks whether the external is declared pure |
| `from` | String literal | Header/source hint; the C generator uses this value to emit an include |
| `abi` | String literal | Stored ABI descriptor; do not assume all backends give it special behavior |

Semicolons between descriptor entries are accepted and recommended. The `pure` value is read as an identifier; only the exact spelling `true` sets the parsed flag to true. Use `pure = true;` or `pure = false;` rather than a quoted value.

The parser also consumes unknown descriptor keys with an expression value, but the supplied implementation does not establish a stable public meaning for arbitrary keys. Prefer `pure`, `from`, and `abi` unless you are deliberately working with compiler internals.

### 19.11 Include descriptors

For `from`, the C generator emits an include. If the string starts with `<` or `"`, it is used as an include operand; otherwise the generator wraps it in angle brackets. Typical spellings are:

```mak
from = "stdio.h";
from = "<sys/types.h>";
```

Check the generated C if you use a nonstandard include spelling. The descriptor's source syntax is a MAK string literal, while the generated include is C syntax.

## 20. Importing declarations with `bring`

A `bring` declaration imports declarations from one or more source files:

```mak
bring "math.mak";
bring "lists.mak" "option.mak";
```

The declaration must contain at least one string literal and must end with a semicolon. Multiple paths are written as a sequence of string literals, without commas.

### 20.1 Relative paths

A relative import path is resolved relative to the directory of the file containing that `bring` declaration. Absolute paths are also accepted by the loader.

### 20.2 What is brought in

The loader reads and parses imported files, then collects their declarations. The root file's final expression remains the program body; an imported file's body is not substituted for the root program body. Repeated files are guarded by a visited-path set so the loader does not keep importing the same canonical path indefinitely.

### 20.3 Example layout

`main.mak`:

```mak
bring "arithmetic.mak";

let result = add 20 22;
result
```

`arithmetic.mak`:

```mak
let add x y = x + y;
0
```

The declaration `add` is made available to the importing program. The trailing `0` in the imported file is not the root program's result.

## 21. Compact grammar

The following EBNF is a reading aid for the surface syntax. Expression application and precedence are implemented by a precedence parser, so the precedence table remains authoritative where this compact grammar is ambiguous.

```ebnf
program              = { top_decl }, [ expression ], EOF ;

top_decl             = let_decl
                     | type_decl
                     | bring_decl ;

let_decl             = "let", ( external_decl | value_decl ) ;
value_decl           = identifier, { identifier }, "=", expression, ";" ;

external_decl        = ( "extern" | "external" ), identifier,
                       [ ":", mak_type ], "=", [ "global" ], string,
                       [ ":", foreign_type ], [ descriptor_block ], ";" ;

bring_decl           = "bring", string, { string }, ";" ;

type_decl            = "type", identifier, { identifier }, ";"
                     | "type", identifier, { identifier }, "{",
                       { constructor_decl }, "}", ";" ;
constructor_decl     = identifier, { mak_type }, [ ";" ] ;

descriptor_block     = "{", { descriptor_entry, [ ";" ] }, "}" ;
descriptor_entry     = identifier, "=", descriptor_value ;

descriptor_value     = identifier | string | expression ;

expression           = literal
                     | identifier
                     | "(", expression, ")"
                     | block
                     | function_expr
                     | let_expr
                     | case_expr
                     | expression, "[", expression, "]"
                     | expression, "(", ")"
                     | expression, expression
                     | unary_expr
                     | expression, binary_operator, expression
                     | expression, ".", expression ;

literal              = integer | float | string ;

function_expr        = ( "fn" | "\\" ), parameter_list,
                       ( "->", expression
                       | "=>", expression
                       | block ) ;
parameter_list       = [ identifier, { identifier } ]
                     | "(", [ identifier, { [ "," ], identifier } ], ")" ;

let_expr             = "let", identifier, { identifier }, "=",
                       expression, ";", expression ;

block                = "{", { expression, ";" }, [ expression ], "}" ;

case_expr            = "case", expression, "of", "{", { case_arm }, "}" ;
case_arm             = pattern, ( "->" | "=>" ), expression, [ ";" ] ;

pattern              = "_"
                     | binding_identifier
                     | integer
                     | string
                     | constructor_identifier, { pattern }
                     | "(", ")"
                     | "(", pattern, { ",", pattern }, ")" ;

mak_type             = mak_type_atom, [ "->", mak_type ] ;
mak_type_atom        = primitive_type
                     | identifier
                     | "[", mak_type, "]"
                     | "!"
                     | "(", ")"
                     | "(", mak_type, ")"
                     | "(", mak_type, ",", mak_type, { ",", mak_type }, ")"
                     | "(", identifier, mak_type_atom, { mak_type_atom }, ")" ;

foreign_type         = foreign_function_type | foreign_type_atom ;
foreign_function_type = "(", [ foreign_arg, { ",", foreign_arg } ], ")",
                        "->", foreign_type ;
foreign_arg          = "..." | foreign_type ;
foreign_type_atom    = foreign_atom, { "*" } ;
foreign_atom         = primitive_foreign_type | "ptr" | "void" | identifier ;

unary_expr           = unary_operator, expression ;
unary_operator       = "-" | "!" | "~" ;
binary_operator      = "*" | "/" | "%" | "+" | "-" | "<<" | ">>"
                     | "&" | "^" | "|" | "==" | "/=" | "<" | ">"
                     | "<=" | ">=" | "&&" | "||" ;
primitive_type       = "u8" | "u16" | "u32" | "u64"
                     | "i8" | "i16" | "i32" | "i64" | "f32" | "f64" ;
primitive_foreign_type = primitive_type ;

binding_identifier   = identifier ; (* lowercase-initial, excluding `_` *)
constructor_identifier = identifier ; (* uppercase-initial *)
identifier           = letter_or_underscore, { letter_or_digit_or_underscore } ;
integer              = decimal_integer | hexadecimal_integer | binary_integer ;
decimal_integer      = digit, { digit | "_" } ;
hexadecimal_integer  = ( "0x" | "0X" ), { hex_digit } ;
binary_integer       = ( "0b" | "0B" ), { "0" | "1" } ;
float                = decimal_float ;
decimal_float        = digit, { digit }, ".", digit, { digit }, [ exponent ]
                     | digit, { digit }, exponent ;
exponent             = ( "e" | "E" ), [ "+" | "-" ], { digit } ;
string               = quoted_string ;
quoted_string        = "\"", { string_character | escape_sequence }, "\""
                     | "'", { string_character | escape_sequence }, "'" ;
letter_or_underscore = letter | "_" ;
letter_or_digit_or_underscore = letter | digit | "_" ;
letter               = "A".."Z" | "a".."z" ;
digit                = "0".."9" ;
hex_digit            = digit | "a".."f" | "A".."F" ;
```

This grammar intentionally abstracts over the exact expression binding-power implementation. It also omits lexer recovery behavior and some permissive edge cases. In particular, the `let`-declaration syntax is parsed from a leading `let` token; an external declaration's required `let` prefix is reflected in the grammar.

## 22. Features that are not source syntax

The compiler's internal data structures contain a few items that should not be mistaken for supported source constructs.

- **`pass`:** recognized as a keyword by the lexer, but the parser does not define a `pass` expression or declaration.
- **`$mak.index`:** an internal intrinsic name produced when the parser reads `base[index]`. Write bracket indexing in source code instead.
- **Quote AST node:** an internal expression node exists in compiler data structures, but the parser shown in the supplied source has no source-level quoting token or grammar production for it.
- **Boolean literals:** `true` and `false` are not dedicated lexer tokens. The descriptor syntax uses them as ordinary identifiers in the `pure` field, and the parser does not provide them as general boolean literal forms.
- **Unit expression:** `()` is recognized as the unit *type* (and as an empty tuple pattern) but is not an expression literal. An expression parenthesis must contain an expression; `{}` currently falls back to integer `0`.
- **Tuple expressions:** tuple types and tuple patterns are parsed, but the expression AST/parser does not provide a tuple-literal expression form.
- **Array literals:** `[T]` is a type and `value[index]` is indexing. A standalone `[]` or `[a, b]` literal is not accepted.
- **Member access:** `.` is function composition. It is not a struct/object field selector.
- **General assignment:** `=` is used in declarations and external descriptor entries; there is no general expression assignment production.

These notes describe the parser and lexer present in the supplied source, not a claim about features that might be added in a later version.

## 23. Compiler-driver syntax

The source also defines a command-line compiler driver named `mak`. This section documents the CLI shape separately from MAK source syntax.

### 23.1 Invocation

```text
mak [OPTIONS] <INPUT.mak>
```

One input source file is accepted directly. Additional source files are normally brought in with `bring` declarations. `--` can be used to end option parsing before the input path.

### 23.2 Output and compilation options

| Option | Meaning |
|---|---|
| `-o PATH`, `--output PATH` | Output artifact path or binary name |
| `-cc`, `--compile` | Generate C and invoke the backend compiler |
| `-bcc=NAME`, `--backend NAME` | Select a backend: `clang`, `gcc`, `cc`, or `zig`; `zig` is used as `zig cc` |
| `-t TARGET`, `--target TARGET` | Target name or target triple; default is `native` |
| `--linker NAME` | Set the linker through a `-fuse-ld=NAME` option; shorthand forms include `-ld`, `-lld`, and `-ldd` |

### 23.3 C compiler and linker arguments

| Option | Meaning |
|---|---|
| `-C FLAG`, `--cflag FLAG` | Add a C compiler flag; may be repeated |
| `--link-arg ARG` | Add a linker argument; may be repeated |
| `-D NAME[=VALUE]`, `--define NAME[=VALUE]` | Add a C preprocessor definition |
| `-I DIR`, `--include DIR` | Add an include-search directory |
| `-L DIR`, `--lib-path DIR` | Add a library-search directory |
| `-l NAME`, `--lib NAME` | Link a library; a leading `-l` is tolerated |

### 23.4 Driver and diagnostic options

| Option | Meaning |
|---|---|
| `-v`, `--verbose` | Print compiler phases, timing, and the backend command |
| `-q`, `--quiet` | Suppress normal success messages |
| `--dry-run` | Perform setup and code generation without invoking the backend compiler |
| `-h`, `--help` | Show usage help |
| `-V`, `--version` | Show the MAK driver version |

### 23.5 Output naming

Examples:

```sh
mak main.mak
# Generates main.c

mak main.mak -o generated.c
# Generates generated.c

mak main.mak -cc -o app
# Generates app.c and builds app

mak main.mak -o optimized.mLIR
# Emits MAK LIR as optimized.mLIR
```

An output path ending in `.c` is treated as a C output path. `.lir` and `.mlir` extensions are normalized to `.mLIR`. With `-cc`, use a binary output name rather than an LIR output path.

### 23.6 Targets

The driver accepts target names/triples and provides aliases including `x64`, `amd64`, `x86_64`, `arm64`, `aarch64`, `win64`, `x86`, `i686`, `arm`, `armv7`, and `riscv64`. `native`, `host`, and `local` resolve to the host target. Cross-target support depends on the selected backend and the cross-compiler tools installed on the machine.

## 24. Practical examples

The examples in this section combine only constructs described above. They are syntax illustrations; exact type correctness and runtime behavior still depend on the compiler's semantic rules.

### 24.1 Values and functions

```mak
let base = 10;
let increment x = x + 1;
let add x y = x + y;

add (increment base) 5
```

### 24.2 Local bindings and blocks

```mak
let calculate x = {
    let doubled = x * 2;
    let adjusted = doubled + 3;
    adjusted
};

calculate 8
```

### 24.3 A generic-looking data type and match

```mak
type Option a {
    None;
    Some a;
};

let fallback option default_value =
    case option of {
        None -> default_value;
        Some value -> value
    };

fallback (Some 7) 0
```

The syntax of a type application is different from constructor expression syntax: `(Option a)` is a type application, while `Some value` is a constructor-pattern/application-looking expression. Constructors are declarations in their own right; use the forms accepted by semantic analysis for constructing values.

### 24.4 Composition

```mak
let increment x = x + 1;
let double x = x * 2;
let double_after_increment = double . increment;

double_after_increment 20
```

### 24.5 Foreign declaration

```mak
let extern putchar : i32 -> i32 = "putchar" : (i32) -> i32 {
    from = "stdio.h";
    pure = false;
};

putchar 65
```

The MAK type annotation and foreign signature are deliberately both present. `from` supplies the include hint consumed by the C generator.

### 24.6 Importing a helper file

```mak
bring "helpers.mak";

let output = helper_function 123;
output
```

In this shape, `helpers.mak` supplies declarations, while the importing file supplies the final program expression.

## 25. Implementation notes and safe assumptions

Use this section as a checklist when reading or writing MAK source.

1. **Use semicolons deliberately.** Every top-level declaration requires one. Local `let` bindings also require one after the bound value. The top-level result expression should normally have no trailing semicolon.
2. **Do not import syntax from another language by analogy.** MAK uses `/=` for not-equal, `.` for composition, whitespace for application, and has no generic comma-separated call syntax.
3. **Parenthesize application arguments that contain operators.** `f (x + y)` and `f (-x)` make the parse explicit.
4. **Use `fn` or `\` for function expressions.** A function body can use `{ ... }`, `-> expression`, or `=> expression`.
5. **Use uppercase constructor names for pattern matching.** The parser decides between constructor and binding patterns from the initial character's case.
6. **Separate data constructors with semicolons.** Constructor arguments are read as type expressions up to a semicolon or the closing type brace.
7. **Remember that MAK types and foreign types are different.** The type before `=` in an external declaration describes the MAK-level binding; the type after the symbol string describes the foreign value/signature.
8. **Annotate ordinary returning externals explicitly.** Without a MAK-side annotation, the parser records the bottom type, which downstream code uses as non-returning metadata.
9. **Avoid malformed-number edge cases.** Use complete decimal points and exponents, and avoid relying on overflow or invalid exponent fallback behavior.
10. **Treat undocumented descriptor keys and internal AST nodes as implementation details.** The parser may accept some extra shapes without promising stable source-language meaning.

---

*End of MAK syntax reference.*
