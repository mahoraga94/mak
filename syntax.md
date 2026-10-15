# MAK Syntax Reference

## 1. Source-file structure

```ebnf
program      ::= { declaration } [ expression ] EOF
declaration  ::= let-declaration
               | type-declaration
               | bring-declaration
```

A file contains zero or more declarations followed by at most one final expression. Top-level declarations require `;`. The final expression does not require and generally cannot have a trailing `;`. If the file contains declarations only, the parser supplies an integer `0` as its body.

```mak
let x = 10;
let add a b = a + b;
add x 5
```

## 2. Whitespace, comments, identifiers

### Whitespace

Spaces, tabs, carriage returns, and newlines separate tokens. Newlines do not terminate expressions or define blocks.

### Comments

```mak
// Line comment: continues to the end of the line.

/* Block comment.
   Block comments can nest: /* inner comment */ still in outer comment.
*/
```

An unterminated block comment is a lexer error.

### Identifiers

```ebnf
identifier       ::= identifier-start { identifier-continue }
identifier-start ::= ASCII-letter | "_"
identifier-continue ::= identifier-start | digit
```

Identifiers are ASCII-only and case-sensitive. Uppercase and lowercase names are distinct. A name cannot start with a digit.

The lexically reserved words are:

`let` `fn` `case` `type` `extern` `of` `pass` `bring`

`external` is accepted as an alternative to `extern` after `let`. `global` is a special word in an external declaration. `true` and `false` are not language keywords or dedicated boolean literals; they lex as identifiers. `pass` is lexed as a reserved word but has no declaration or expression form in the parser.

## 3. Literals

### Integer literals

```mak
0
42
1_000_000
0x2a
0X2A
0b101010
0B101010
```

Decimal, hexadecimal (`0x`/`0X`), and binary (`0b`/`0B`) integers are lexed as unsigned 64-bit values. Decimal digits may contain underscores. Negative numbers use unary `-` applied to an integer expression; `-` is not part of the integer token.

### Floating-point literals

```mak
0.0
3.14159
1e3
1E-3
6.02e+23
```

A decimal point is part of a float when at least one digit follows it. An exponent uses `e` or `E`, with an optional sign and digits. Floating literals are represented as `f64` values by the lexer.

### String literals

Both quote styles are accepted:

```mak
"double-quoted string"
'single-quoted string'
"line one\nline two"
'it\'s escaped'
```

Supported escapes:

| Escape | Meaning |
|---|---|
| `\n` | Newline byte |
| `\t` | Tab byte |
| `\r` | Carriage-return byte |
| `\\` | Backslash |
| `\"` | Double quote |
| `\'` | Single quote |
| `\0` | NUL byte |
| `\xHH` | Byte with two hexadecimal digits `HH` |
| `\c` for another character `c` | The backslash is removed and `c` is emitted |

Strings are stored as bytes. Newlines are allowed inside a quoted string if the closing quote is present. An unclosed string is a lexer error.

## 4. Top-level declarations

### `let` value declaration

```ebnf
let-declaration ::= "let" identifier { identifier } "=" expression ";"
```

```mak
let answer = 42;
let add x y = x + y;
let twice = fn (x) -> x * 2;
```

`let name = expression;` declares a value. `let name p1 p2 = expression;` is shorthand for binding `name` to a function with parameters `p1` and `p2`. There is no top-level type-annotation form in the parser.

### `type` declaration

```ebnf
type-declaration ::= "type" identifier { identifier } ";"
                   | "type" identifier { identifier } "{" { constructor } "}" ";"
constructor      ::= identifier { type } [ ";" ]
```

```mak
type Empty;

type Maybe a {
    None;
    Some a;
};

type Result a e {
    Ok a;
    Error e;
};
```

The identifiers after the type name are type parameters. A constructor name is followed by zero or more field types. Constructor fields are written as types, separated by whitespace; a semicolon ends the constructor, and the final constructor may omit its semicolon before `}`. The semicolon after the closing `}` is required. `type Name;` declares a type without constructors.

### `bring` declaration

```ebnf
bring-declaration ::= "bring" string-literal { string-literal } ";"
```

```mak
bring "math.mak";
bring "base.mak" "io.mak";
```

`bring` takes one or more quoted paths. There is no imported-name list or alias syntax in the declaration grammar.

## 5. External declarations

```ebnf
external-declaration ::= "let" ("extern" | "external") identifier
                         [ ":" type ] "=" [ "global" ] string-literal
                         [ ":" foreign-type ] [ descriptor-block ] ";"

descriptor-block ::= "{" { identifier "=" descriptor-value [ ";" ] } "}"
```

Examples:

```mak
let extern putchar : i32 -> i32 = "putchar" : (i32) -> i32;

let extern puts : ptr -> i32 = "puts" : (ptr) -> i32 {
    pure = false;
    from = "stdio.h";
    abi = "C";
};

let extern errno : i32 = global "errno" : i32;
```

Meaning of each part:

- `let extern` / `let external`: declares a foreign symbol.
- `name`: MAK-side name used by source expressions.
- `: type`: optional MAK-side type. If omitted, its type defaults to `!` (bottom type) in the parsed declaration.
- `= "symbol"`: quoted foreign symbol name.
- `global`: marks the symbol as a global rather than an ordinary callable symbol.
- `: foreign-type`: optional C-facing type. If omitted, the foreign type defaults to `void`.
- `pure = true` or `pure = false`: records the purity flag; only the identifier spelling `true` sets it to true.
- `from = "..."`: source/library descriptor string.
- `abi = "..."`: ABI descriptor string.
- Other descriptor keys are parsed with an expression as their value and have no special meaning in the parser.

Descriptor entries may end with semicolons; the final entry does not need one. The descriptor block and the external declaration must be closed/terminated.

### Foreign types

```ebnf
foreign-type ::= foreign-atom { "*" }
               | "(" [ foreign-type { "," foreign-type } [ "," ] ] ")" "->" foreign-type
               | "(" foreign-type { "," foreign-type } "," "..." ")" "->" foreign-type
foreign-atom ::= "i8" | "i16" | "i32" | "i64"
               | "u8" | "u16" | "u32" | "u64"
               | "f32" | "f64" | "ptr" | "void" | identifier
```

```mak
let extern read_byte : i32 -> i32 = "read_byte" : (i32) -> i32;
let extern variadic : i32 = "variadic" : (i32, ...) -> i32;
let extern bytes : [u8] = global "bytes" : u8*;
let extern matrix : [u8] = global "matrix" : u8**;
```

Foreign function arguments are comma-separated. `...` marks variadic arguments. A trailing comma in a foreign argument list is accepted. Repeated pointer stars are accepted: `u8*`, `u8**`, `u8***`.

## 6. Expressions

```ebnf
expression ::= prefix-expression { postfix-or-infix-part }
```

### Atomic expressions

```ebnf
atom ::= integer-literal
       | float-literal
       | string-literal
       | identifier
       | "(" expression ")"
       | block
       | function-expression
       | let-expression
       | case-expression
```

```mak
123
3.5
"hello"
value
(value + 1)
{ 1 + 2 }
```

Parentheses group one expression. Parentheses do not create tuple expressions. Empty expression parentheses `()` are not accepted as an expression literal.

### Function application

```ebnf
application ::= expression atom
```

```mak
f x
add 2 3
f (x + 1)
f xs[i]
```

Application is written by placing an argument atom after the function. Whitespace is conventional, not the operator: `f x` and `f(x)` both apply `f` to the single expression `x`. Immediate lambda invocation is also juxtaposition: `(\x -> x*x) 2` and `(\x -> x*x)2` both apply the anonymous square function to `2`. The space is recommended for readability.

Application chains associate syntactically to the left: `f x y` is grouped as `(f x) y`. The compiler also flattens a contiguous application chain when resolving calls, so a definition with two parameters, `let f x y = ...;`, is ordinarily called as `f x y` with two arguments. Do not infer from the left-associated parse shape alone that every two-step call is a curried unary call.

Parentheses around one argument are grouping, not a special call syntax. A comma-separated nonempty argument list such as `f(x, y)` is not supported. The parser has a special zero-argument spelling `f()`. The expression `f ()` is different: `()` is not an ordinary unit-value expression atom.

### Indexing

```ebnf
indexed-expression ::= atom { "[" expression "]" }
```

```mak
items[i]
items[i][j]
(f x)[index]
f items[i]
```

Indexing is postfix, binds tightly, and may be chained. `value[index]` is parsed into an application of the reserved intrinsic `$mak.index` to the value and index. The index is an expression. Empty brackets `value[]` and standalone array literals `[]` are not accepted.

### Unary expressions

```ebnf
unary-expression ::= "-" expression
                   | "!" expression
                   | "~" expression
```

```mak
-x
!condition
~bits
-f x
```

| Form | Operation |
|---|---|
| `-x` | Numeric negation |
| `!x` | Logical/boolean-style negation operator |
| `~x` | Bitwise complement |

Unary operands include following application and indexing, but stop before infix operators. For example, `-f x` parses as `-(f x)`.

### Anonymous functions

```ebnf
function-expression ::= ("fn" | "\\") parameter-list ("->" | "=>") expression
                      | ("fn" | "\\") parameter-list block
parameter-list     ::= identifier { identifier }
                      | "(" [ identifier { [ "," ] identifier } ] ")"
```

```mak
fn x -> x + 1
fn (x) => x + 1
fn (x, y) { x + y }
fn x y { x * y }
\x -> x * 2
\(x, y) => x + y
fn () -> 42

// Invoke a one-argument lambda immediately:
(\x -> x * x) 9
(\x -> x * x)9

// Two named parameters:
(\x y -> x + y) 20 22
(\(x, y) -> x + y) 20 22
```

`fn` and `\` both introduce anonymous functions. Parameters are identifiers; parenthesized parameters may be separated by commas, and the parser also accepts adjacent identifiers inside parentheses. Without parentheses, the parser consumes consecutive identifiers as parameters until it reaches `->`, `=>`, or `{`. A body is either an expression introduced by `->` or `=>`, or a braced block. Empty parameter lists are accepted as `fn () ...` / `\ () ...`.

A lambda is an expression value. It can be bound to a name, passed as an argument, composed with another function, or immediately invoked. To immediately invoke it, parenthesize the lambda so that the closing parenthesis marks the end of the function expression: `(\x -> x * x) 9`.

### Local `let` expression

```ebnf
let-expression ::= "let" identifier { identifier } "=" expression ";" expression
```

```mak
let answer = 42; answer + 1
let add x y = x + y; add 10 20
let square = fn x -> x * x; square 9
```

A local binding introduces a name for the expression after the semicolon. Parameters after the binding name make the value a function. The `;` after the value is required; the expression after it is the body of the binding.

### Blocks and sequencing

```ebnf
block ::= "{" { expression ";" } [ expression ] "}"
```

```mak
{
    first_action;
    second_action;
    final_value
}

{
    let x = 10;
    let y = 20;
    x + y
}
```

A block uses `{` and `}`. Semicolons separate completed expressions. The final expression is the block result and normally has no semicolon. Local `let` declarations inside a block require their own terminating semicolon. An empty block parses to integer `0`.

### Pattern matching with `case`

```ebnf
case-expression ::= "case" expression "of" "{" { case-arm } "}"
case-arm        ::= pattern ("->" | "=>") expression [ ";" ]
pattern         ::= "_"
                  | identifier
                  | integer-literal
                  | string-literal
                  | constructor-pattern
                  | "(" ")"
                  | "(" pattern { "," pattern } ")"
constructor-pattern ::= uppercase-identifier { pattern-atom }
```

```mak
case value of {
    0 -> "zero";
    1 => "one";
    _ -> "other"
}

case option of {
    None -> 0;
    Some x -> x;
}

case pair of {
    (x, y) -> x + y
}
```

`case` evaluates a scrutinee expression, then matches arms in source order as handled by the compiler. An arm uses `->` or `=>`; its semicolon is optional. Pattern forms:

- `_`: wildcard; matches without introducing a name.
- `name`: lowercase-leading identifier; binds the matched value.
- `INTEGER`: integer literal pattern.
- `STRING`: string literal pattern.
- `Ctor p1 p2`: uppercase-leading constructor name with zero or more subpatterns.
- `(p1, p2, ...)`: tuple pattern.
- `()`: empty tuple pattern.
- `(p)`: parenthesized single pattern, equivalent to `p`.

The parser classifies a pattern identifier by its first ASCII character: uppercase means constructor pattern; otherwise it is a binding, except `_` which is wildcard. Floating-point literal patterns are not accepted.

## 7. Operators

Indexing and application bind most tightly. Application is included inside a unary operand (`-f x` parses as `-(f x)`). Unary operators bind more tightly than infix operators. All listed binary operators except composition `.` associate left-to-right; `.` associates right-to-left. Operator meaning at runtime depends on operand representations and compiler lowering.

| Binding | Operators | Meaning |
|---|---|---|
| Postfix, tightest | `x[i]` | Indexing |
| Application | `f x` | Function application |
| Prefix | `-x` `!x` `~x` | Negation, logical not, bitwise not |
| 9 | `.` | Function composition |
| 7 | `*` `/` `%` | Multiply, divide, remainder |
| 6 | `+` `-` | Add, subtract |
| 5 | `<<` `>>` | Left shift, right shift |
| 4 | `&` `^` `\|` | Bitwise AND, XOR, OR |
| 3 | `==` `/=` `<` `>` `<=` `>=` | Equal, not equal, less/greater comparisons |
| 2 | `&&` | Logical AND |
| 1, loosest | `\|\|` | Logical OR |

### Arithmetic

```mak
a + b
a - b
a * b
a / b
a % b
```

### Shifts and bitwise operators

```mak
bits << count
bits >> count
a & b
a ^ b
a | b
```

### Comparisons

```mak
a == b
a /= b
a < b
a > b
a <= b
a >= b
```

`/=` is not-equal. `!=` is not recognized as an operator.

### Logical operators

```mak
a && b
a || b
```

### Function composition

```mak
f . g
f . g . h
```

`f . g` builds a function equivalent to `fn x -> f (g x)`. Composition binds less tightly than application and is right-associative. The dot is not field/member access.

### Operator spellings not available

There is no parser production for assignment operators such as `+=`, a ternary `?:` expression, `!=`, `::`, `=>` as an infix operator, or a field/member-access operator. `=` is used in declarations and bindings. `=>` is accepted only where the parser accepts an expression arrow, such as a function body or case arm.

## 8. MAK types

Type expressions are used in user-defined constructor fields and external declarations.

```ebnf
type ::= type-atom [ "->" type ]
type-atom ::= primitive-type
            | identifier
            | "[" type "]"
            | "(" ")"
            | "(" type ")"
            | "(" type "," type { "," type } ")"
            | "(" identifier type-atom { type-atom } ")"
            | "!"
```

### Primitive type spellings

| Syntax | Type form |
|---|---|
| `u8` `u16` `u32` `u64` | Unsigned integer type names |
| `i8` `i16` `i32` `i64` | Signed integer type names |
| `f32` `f64` | Floating-point type names |
| `()` | Unit type |
| `!` | Bottom / non-returning type form |

The parser also accepts any other identifier as a named type. In particular, there is no dedicated `bool` type token or boolean literal syntax.

### Named and parameterized types

```mak
MyType
(List a)
(Result value error)
(Ptr [u8])
```

A bare identifier is a named type. Type application uses parentheses with the type constructor name first, for example `(List a)`. Its head must be a named type. A type application is not written as a bare `List a` outside that parenthesized form.

### Array type

```mak
[u8]
[MyType]
[[i32]]
```

`[T]` is the array type form. It does not create an array value literal.

### Tuple types

```mak
()
(i32, i32)
(i32, f64, [u8])
```

`()` is unit. `(T)` groups a type. `(T, U, ...)` is a tuple type and requires at least two elements; `(T,)` is not accepted as a one-element tuple type.

### Function types

```mak
i32 -> i32
(i32, i32) -> i32
u8 -> u64 -> f64
(i32 -> i32) -> i32
```

`A -> B` is a function type. The arrow associates right-to-left: `A -> B -> C` means `A -> (B -> C)`. Parentheses group the left side when it is itself a function type.

### Bottom type

```mak
!
```

`!` is parsed as the bottom type and is used as the default MAK-side type for an external declaration when `: type` is omitted.

## 9. Pattern syntax quick reference

| Pattern | Meaning |
|---|---|
| `_` | Ignore the matched value |
| `x` | Bind the value to `x` |
| `0` | Match integer zero |
| `"text"` | Match a string value |
| `Some x` | Match constructor `Some` and bind its first payload as `x` |
| `Node left right` | Match constructor with two payload patterns |
| `(x, y)` | Match a tuple with two elements |
| `()` | Empty tuple pattern |
| `(x)` | Same as pattern `x` |

A constructor pattern is recognized from an identifier whose first character is uppercase. Its subpatterns are read as following pattern atoms. Patterns do not accept float literals, arithmetic expressions, type annotations, or `name @ pattern` syntax.

## 10. Punctuation reference

| Token | Syntax role |
|---|---|
| `(` `)` | Group expressions; parameter lists; tuple types/patterns; foreign function types |
| `{` `}` | Blocks, type bodies, case arms, external descriptor blocks |
| `[` `]` | Array type brackets; postfix indexing |
| `;` | Terminates top-level declarations; separates block expressions and local bindings; optionally terminates case arms and constructor entries |
| `,` | Separates tuple types/patterns, parenthesized function parameters, and foreign function arguments |
| `:` | Introduces a MAK type or foreign type in an external declaration |
| `.` | Function composition |
| `\` | Anonymous-function introducer |
| `->` | Function body arrow, case-arm arrow, function-type arrow |
| `=>` | Alternate function body arrow and case-arm arrow |
| `=` | Binds a `let` value or external symbol; separates descriptor keys from values |

## 11. Grammar summary

```ebnf
program               ::= { declaration } [ expression ] EOF
declaration           ::= let-declaration | type-declaration | bring-declaration
let-declaration       ::= "let" identifier { identifier } "=" expression ";"
                        | "let" ("extern" | "external") identifier [ ":" type ] "=" [ "global" ] string-literal [ ":" foreign-type ] [ descriptor-block ] ";"
type-declaration      ::= "type" identifier { identifier } ";"
                        | "type" identifier { identifier } "{" { constructor } "}" ";"
constructor           ::= identifier { type } [ ";" ]
bring-declaration     ::= "bring" string-literal { string-literal } ";"
descriptor-block      ::= "{" { identifier "=" descriptor-value [ ";" ] } "}"
expression            ::= prefix-expression { postfix-index | application-argument | binary-tail }
prefix-expression     ::= [ "-" | "!" | "~" ] atom
atom                  ::= integer | float | string | identifier | "(" expression ")" | block | function-expression | let-expression | case-expression
application-argument ::= atom
postfix-index         ::= "[" expression "]"
binary-tail           ::= infix-operator expression
function-expression   ::= ("fn" | "\\") parameters (("->" | "=>") expression | block)
parameters            ::= { identifier } | "(" [ identifier { [ "," ] identifier } ] ")"
let-expression        ::= "let" identifier { identifier } "=" expression ";" expression
block                 ::= "{" { expression ";" } [ expression ] "}"
case-expression       ::= "case" expression "of" "{" { pattern ("->" | "=>") expression [ ";" ] } "}"
pattern               ::= "_" | identifier | integer | string | constructor-pattern | "()" | "(" pattern { "," pattern } ")"
type                  ::= type-atom [ "->" type ]
type-atom             ::= identifier | "[" type "]" | "(" ")" | "(" type ")" | "(" type "," type { "," type } ")" | "(" identifier type-atom { type-atom } ")" | "!"
foreign-type          ::= foreign-atom { "*" } | "(" foreign-arguments ")" "->" foreign-type
foreign-arguments     ::= [ foreign-argument { "," foreign-argument } [ "," ] ]
foreign-argument      ::= foreign-type | "..."
descriptor-value      ::= expression
```

## 12. Unsupported expression forms

The parser does not provide syntax for:

- Array literals such as `[1, 2, 3]`.
- Tuple expressions such as `(a, b)`.
- Empty expression/unit literal `()`.
- Member or field access such as `value.field` (`.` means composition).
- Not-equal written as `!=` (use `/=`).
- `if` / `else`, loops, `while`, `for`, `switch`, or ternary expressions.
- Lambda parameter type annotations, general expression type annotations, or top-level function signatures.
- Named arguments, keyword arguments, or calls with comma-separated argument lists such as `f(a, b)`.
- Float patterns in `case` arms.
- A trailing semicolon after the final top-level expression.

## 26. Function application and function composition — exact semantics

### 26.1 The two forms are different operations

```mak
f g
f . g
```

`f g` is **function application**. It means “call/apply `f` to the value `g`.” In the parser, juxtaposing an expression with the next atom constructs an application node. Conceptually:

```mak
f g
```

means:

```mak
f(g)
```

where the parentheses are explanatory notation, not required MAK syntax. `g` is evaluated/used as the argument value; MAK does not automatically call `g` just because it appears after `f`.

`f . g` is **function composition**. It does not mean “call `f` with `g` as an argument.” The parser lowers the dot expression to a one-argument function equivalent to:

```mak
fn x => f (g x)
```

The exact generated internal parameter name is an implementation detail. Semantically, the composed function accepts an input `x`, applies `g` to `x`, and then applies `f` to the result. The dot operator therefore constructs a function; it does not, by itself, run the composed function on an input.

### 26.2 A concrete numeric example

```mak
let double x = x * 2;
let increment x = x + 1;
let a = double increment;
let b = double . increment;
```

These two right-hand sides are not interchangeable.

- `double increment` passes the function value `increment` as the argument to `double`. The expression is an ordinary application. Whether it is type-correct depends on whether `double` accepts a function value; this `double` expects a numeric value, so this is not a valid way to mean “double the result of increment.”
- `double . increment` constructs a new function. When that function is later given `n`, it computes `double (increment n)`, i.e. `2 * (n + 1)`.

To apply the composed function, provide its input:

```mak
(double . increment) 10
```

The parentheses make the intended grouping explicit: first form the composition, then apply the resulting function to `10`. The intended result is `22`.

### 26.3 Application does not implicitly invoke function-valued arguments

Suppose:

```mak
let f x = x + 10;
let g x = x * 3;
```

Then:

```mak
f 4
```

applies `f` to the integer `4` and yields `14`.

```mak
g 4
```

applies `g` to `4` and yields `12`.

```mak
f g
```

passes the function `g` itself to `f`. It does **not** mean `f (g x)` because no `x` is supplied and there is no composition operator. Since `f` uses its parameter as an integer, this example is incompatible with the intended numeric operation.

```mak
f (g 4)
```

first computes `g 4` and then passes that result to `f`: `g 4` is `12`, so the result is `22`.

```mak
(f . g) 4
```

constructs the composition `fn x => f (g x)` and applies it to `4`, also yielding `22`. For this particular pure example, `f (g 4)` and `(f . g) 4` have the same numerical result, but they are different expression structures: the former directly applies `f` to an already-written expression; the latter creates a reusable function and then applies it.

### 26.4 Application grouping: `f x y` versus `f (x y)`

The parser groups a chain of juxtaposed applications syntactically to the left:

```mak
f x y
```

has this grouping:

```mak
(f x) y
```

It does **not** have this grouping:

```mak
f (x y)
```

The explicitly grouped form `f (x y)` treats `x y` as one argument expression to `f`; `f x y` is a chain whose source-level grouping is left-associated. Parentheses matter because `x y` is then parsed as a separate inner application.

There is an important implementation distinction: call resolution flattens a contiguous application chain and checks its overall arity. For example:

```mak
let add x y = x + y;
add 3 4
```

is the normal form for calling this two-parameter definition with two arguments. Do not explain this by saying `add 3` necessarily returns a one-parameter function and then `4` invokes that result. The syntax tree's left grouping and the compiler's multi-parameter call resolution are different layers. Likewise, do not assume that a nested unary-function value and a single two-parameter function have interchangeable call behavior.

### 26.5 `f . g x` is not the same as `(f . g) x`

Because application binds more tightly than dot composition, the source:

```mak
f . g x
```

parses the right side of the dot as `g x`. Its effective grouping is:

```mak
f . (g x)
```

Composition then attempts to use `f` and the expression `g x` as the two sides of composition. Since the right side is an application result rather than necessarily a function, this commonly does not express the intended operation.

If the intention is “compose `f` with `g`, then apply the composed function to `x`,” write:

```mak
(f . g) x
```

If the intention is “apply `g` to `x`, then pass that result to `f`,” write:

```mak
f (g x)
```

These last two forms describe the same mathematical result when the functions and values have compatible types and the language's evaluation effects do not make the difference observable. The parentheses in `(f . g) x` are crucial because they close the composition before the argument `x` is applied.

### 26.6 Dot composition is not member access

The dot token is not parsed as a field selector, namespace separator, or method-call marker. For example, `value.field` is not a record-field lookup in this grammar. It is parsed using the composition operator's rules, which means it will only make sense if both sides are expressions with compatible function meanings.

Do not read:

```mak
object.method
```

as “the `method` member of `object`.” MAK's parser does not implement that interpretation for `.`.

### 26.7 Composition is right-associative in the parser

Dot has equal left and right binding powers in the Pratt parser. The right-hand expression is parsed with a binding power that allows another dot to be consumed into that right-hand side. Therefore:

```mak
f . g . h
```

groups as:

```mak
f . (g . h)
```

Its usual mathematical behavior for compatible unary functions is `f (g (h x))` on input `x`: apply `h` first, then `g`, then `f`. This is the parser's right-associated grouping, not left-associated `(f . g) . h` syntax. Parentheses can always document the intended grouping explicitly.

### 26.8 Parentheses change the role of an expression

```mak
f g
f (g x)
(f . g) x
f . g
```

Read these as four different structures:

1. `f g`: apply `f` to the value represented by `g`.
2. `f (g x)`: apply `g` to `x`, then pass the result to `f`.
3. `(f . g) x`: construct a composition and apply it to `x`.
4. `f . g`: construct a composed function; no final input is supplied here.

Parentheses around a normal expression do not create a tuple by themselves. `(x)` is the grouped expression `x`; tuple-pattern syntax is a separate matter, and general tuple *expression* literals are not supported by this parser.

### 26.9 Application is whitespace-based, but not arbitrary token concatenation

The parser recognizes application when an expression is followed by a token that can start an atom: an identifier, integer, float, string, parenthesized expression, block, backslash function, `fn`, `let`, or `case`. This is a grammatical rule, not a rule that every adjacent token is an argument.

Examples:

```mak
f x
f 10
f "text"
f (x + 1)
f { x + 1 }
f (fn x => x + 1)
```

Each has one argument expression. A sequence such as `f x y` forms a chain of applications. A binary operator is not simply another application argument: `f x + y` is parsed according to the operator-precedence rules, not as a three-argument call.

Empty-call syntax is a special case:

```mak
f()
```

The parser accepts this as an application of `f` with zero arguments. It does not imply that `f` is automatically called in every context, and it is not equivalent in syntax to `f ()` (where `()` is not an ordinary unit-value expression atom in the expression parser).

### 26.10 Indexing has higher precedence than application

Indexing is postfix and binds more tightly than implicit application. Therefore:

```mak
f xs[i]
```

means applying `f` to `xs[i]`, equivalent in grouping to:

```mak
f (xs[i])
```

The parser lowers indexing into a reserved intrinsic application, conceptually `$mak.index(xs, i)`. Chained indexing:

```mak
xs[i][j]
```

is applied sequentially: first index `xs` by `i`, then index the result by `j`.

An opening `[` cannot begin a standalone expression. Thus `[]` and `[1, 2]` are not array literals in the current expression grammar; brackets are for indexing an existing expression.

### 26.11 Unary operators include application in their operand

The prefix parser parses the unary operand at a binding power that still permits application but stops before dot composition and ordinary infix operators. This means a unary operator can cover an application chain before the parser returns to the composition/binary layer. Use parentheses when the intended operand boundary could be ambiguous.

The supported prefix operators are `-`, `!`, and `~`. They mean numeric negation, logical not, and bitwise not respectively at the syntax/AST level; type and backend validity depend on the operand and later compiler stages.

### 26.12 Precedence relevant to application and composition

From tighter to looser, the parser handles the important layers in this order:

1. Postfix indexing, such as `xs[i]`.
2. Implicit application, such as `f x` and `f x y`.
3. Dot composition, such as `f . g`.
4. Multiplication, division, and remainder: `*`, `/`, `%`.
5. Addition and subtraction: `+`, `-`.
6. Shifts: `<<`, `>>`.
7. Bitwise operators: `&`, `^`, `|` (these share the same binding-power tier in the parser).
8. Comparisons: `==`, `/=`, `<`, `>`, `<=`, `>=`.
9. Logical AND: `&&`.
10. Logical OR: `||`.

The unary layer sits before application/infix parsing but deliberately allows application inside its operand. Parentheses override these defaults. For expressions whose meaning depends on the exact boundary between composition and application, prefer explicit grouping over relying on memory.

### 26.13 Operator spellings that must not be confused

`/=` is the not-equal comparison token. It is not an assignment operator. The slash used in `/=` should not be confused with `/`, which is division. Likewise `.` is composition, not member access. `=` introduces bindings/declarations, while `==` tests equality.

### 26.14 Type compatibility is separate from parsing

The parser can construct an application node without proving that its function operand is callable or that its argument has the correct type. Likewise, it can construct a composition node without proving that the two operands have compatible function types. A syntactically accepted expression can still fail semantic analysis, type-related checks, or code generation.

For composition, the intended ordinary function-type relationship is:

```text
g : A -> B
f : B -> C
f . g : A -> C
```

This notation describes the mathematical contract of composition; it is not a claim that MAK requires or accepts this exact type-annotation syntax everywhere. The intermediate result of `g` must be acceptable as the argument to `f`.

For application, the ordinary relationship is:

```text
f : A -> B
x : A
f x : B
```

A mathematical notation such as `f : A -> (B -> C)` describes a unary function whose result is another function. Do not assume that writing `f x y` automatically invokes that returned function as a second unary call: this implementation flattens contiguous application chains when resolving calls and checks their total arity against known function parameters. The source form `fn x y -> body` is a single function declared with two parameters; it is not the same function shape as `fn x -> fn y -> body`. The type equations here are explanatory notation, not standalone MAK declarations or a guarantee of support for every higher-order call shape.

### 26.15 A compact comparison table

| Source | Syntax operation | Resulting idea |
|---|---|---|
| `f x` | Application | Apply `f` to `x` |
| `f x y` | Left-associated syntax, with contiguous chains flattened during call resolution | Often the call form for a two-parameter function; not proof of currying |
| `f (g x)` | Nested application | Compute `g x`, then pass that value to `f` |
| `f g` | Application | Pass the value/function `g` to `f`; does not invoke `g` automatically |
| `f . g` | Composition | Build a function equivalent to `fn x => f (g x)` |
| `(f . g) x` | Composition, then application | Apply the composed function to `x` |
| `f . (g x)` | Composition with an application expression on the right | Usually not the intended composition of `f` and `g` |
| `f . g x` | Application binds before dot | The right operand of dot is parsed as `g x` |
| `object.field` | Dot operator | Not field/member access |
| `xs[i]` | Indexing | Index `xs` with `i` |
| `f xs[i]` | Index then application | Apply `f` to `xs[i]` |

### 26.16 Minimal decision procedure for reading an expression

When reading an unfamiliar expression, apply these checks in order:

1. Find explicit parentheses and treat their contents as grouped expressions.
2. Attach every postfix `[index]` to the expression immediately before it.
3. Read juxtaposed atoms as application chains: `f x y` means `(f x) y`.
4. Only after application grouping, interpret `.` as composition.
5. Apply arithmetic, comparison, and logical precedence to the remaining operators.
6. Check whether each application receives a value or a function, and whether each composition operand is intended to be a function.
7. Check semantic/type validity separately from whether the parser can construct the syntax tree.

This procedure prevents the most common misreadings: treating `f g` as composition, treating `f . g` as a call, treating dot as member access, or assuming `f . g x` means `(f . g) x`.

## 27. Anonymous functions: practical use, exact boundaries, and common forms

An anonymous function (often called a *lambda*) creates a function value without giving it a top-level name. The parser accepts either `fn` or a backslash (`\`) as the introducer. The arrow may be `->` or `=>`; both spellings have the same role here.

### 27.1 Copyable patterns

```mak
// One parameter, expression body
fn x -> x + 1
\x -> x + 1

// Equivalent parenthesized parameter spelling
fn (x) -> x + 1
\(x) => x + 1

// Two parameters on one function
fn x y -> x + y
fn (x, y) -> x + y
\(x y) -> x + y

// No parameters
fn () -> 42
\() => 42

// A block body; the final expression is the result
fn x {
    let doubled = x * 2;
    doubled + 1
}
```

The parameter list contains identifiers only. It does not allow parameter type annotations, defaults, destructuring patterns, or named-argument declarations. A parenthesized parameter list may use commas or simply place identifiers next to each other. The parser consumes commas when present, so both `fn (x, y) -> ...` and `fn (x y) -> ...` are accepted.

### 27.2 Invoke a lambda immediately

This is the compact inline pattern for creating a function and calling it right away:

```mak
(\x -> x * x) 2
(\x -> x * x)2
```

Both forms mean: create the one-parameter function `\x -> x * x`, then apply it to `2`. The second form works because `)` and `2` remain separate tokens even with no intervening space. Prefer the first form in ordinary code: the space makes the function/argument boundary easier to see. Either expression is intended to compute `4`.

A few more shapes:

```mak
(\n -> n + 1) 41
(\n -> n * 10) 7
(\x y -> x + y) 20 22
(\(x, y) -> x * y) 6 7
```

The first two are one-parameter lambdas, each immediately called with one value. The latter two declare one function with two parameters and call it with two arguments. `\x y -> ...` is one function with two parameters; do not silently rewrite it as `\x -> (\y -> ...)` because those are different function structures.

### 27.3 Parenthesize the lambda to mark where it ends

```mak
(\x -> x * x) 5
(\x -> x + 10) (3 * 4)
(\x -> x + 1) ((\y -> y * 2) 10)
```

The last example first evaluates the inner call to `20`, then calls the outer lambda with `20`, producing `21` when the relevant arithmetic operations are valid.

Do not omit the parentheses around the function in an attempt to write an immediate call:

```mak
\x -> x * x 2
```

This parses as a lambda whose body is `x * (x 2)`: application is allowed inside the body, and the `2` attaches to `x`. It does **not** mean `(\x -> x * x) 2`. If the goal is immediate invocation, parenthesize the lambda expression.

### 27.4 Give a lambda a name when you reuse it

```mak
let square = \x -> x * x;
square 9
```

A top-level declaration ends with a semicolon, while the final program expression `square 9` does not. A local `let` expression uses a semicolon between its value and body:

```mak
let square = \x -> x * x; square 9
```

This local expression binds `square` and then evaluates the body `square 9`. It is not the same syntactic context as a top-level declaration followed by a separate final expression.

### 27.5 Pass a lambda as a value

```mak
some_function (\x -> x + 1)
```

Syntactically, this applies `some_function` to a lambda value. Whether `some_function` accepts a function argument is a separate type/implementation question. Passing a function value does not, on its own, call that function; an actual application must supply an argument to invoke it.

### 27.6 Expression body versus block body

Expression-bodied lambdas use `->` or `=>`:

```mak
\x -> x + 1
\x -> double x + 1
\x -> (double x) + 1
```

A block-bodied lambda puts `{` after its parameter list and does not need an arrow:

```mak
\x {
    let y = x * 2;
    y + 1
}
```

Inside a block, semicolons terminate preceding expressions and the last unterminated expression supplies the block's value. If the final expression has a semicolon, the parser treats it as a completed preceding expression instead of the usual tail-value form.

### 27.7 Zero-parameter lambdas and empty calls

```mak
let forty_two = fn () -> 42;
forty_two()
```

Empty parameter lists are accepted. Invocation uses the special empty-call spelling `forty_two()`. Do not assume `forty_two ()` is equivalent: `()` is not a normal expression value in the expression grammar. Empty tuple/unit forms occur in type and pattern syntax, but there is no standalone unit expression atom.

## 28. Reading application chains without guessing

Application is represented by juxtaposition rather than by a dedicated call token. These are the parser's practical rules:

| Source | How to read it | Important boundary |
|---|---|---|
| `f x` | Apply `f` to `x` | One argument |
| `f(x)` | Same grouping as `f x` | Parentheses group the single expression `x` |
| `f (x + 1)` | Apply `f` to the whole sum | Parentheses keep the sum together |
| `f x y` | Syntactic grouping `(f x) y`; contiguous application chains can be flattened during call resolution | A multi-parameter function is normally called this way |
| `f (g x)` | Apply `f` to the result of `g x` | Inner call is the argument expression |
| `(f x) y` | Parenthesize the first application, then apply the result to `y` | Compiler call resolution can flatten a contiguous chain |
| `f xs[i]` | Apply `f` to `xs[i]` | Indexing binds tightly |
| `f()` | Apply with zero arguments | Special empty-call syntax |
| `f(x, y)` | Not a two-argument call form | Comma-separated call arguments are unsupported |

For a named function with two parameters, write the arguments as a whitespace-separated chain:

```mak
let add x y = x + y;
add 3 4
```

The definition has two parameter identifiers; `add 3 4` supplies two arguments. The parser builds the application syntax left-to-right, but compiler call resolution flattens a contiguous chain and checks total arity against the function's parameter count. Therefore, **the syntactic grouping alone does not prove that MAK is treating every chained application as runtime currying**.

A single-parameter immediate lambda is much less ambiguous:

```mak
(\x -> x * x) 12
```

Do not casually transform `fn x y -> body` into the nested-returning-function expression `fn x -> fn y -> body`. They have different AST shapes, and a chain that resembles mathematical currying may be handled differently by the implementation. When exact behavior matters, use the parameter form declared for the function and do not assume returned/indirect function calls work merely because a corresponding expression can be parsed.

## 29. Composition versus application: expanded examples

Composition `.` creates a new unary function. Application supplies arguments to a function. These are different operations:

```mak
let double x = x * 2;
let increment x = x + 1;

double (increment 10)
(double . increment) 10
```

Both displayed expressions intend to increment `10` and then double the result. The composition expression constructs a reusable function equivalent in meaning to `fn x -> double (increment x)`, then invokes it with `10`, producing `22` when the functions have the expected numeric behavior.

```mak
f g                  // apply f to the value g
f . g                // build x -> f (g x)
(f . g) x            // call the function made by composition
f (g x)              // call g first; pass its result to f
f . g . h            // right-associative: f . (g . h)
```

### 29.1 Dot is not a member selector

`record.field` is not field/member lookup. The parser assigns `.` the meaning of function composition. No general member-access operator is provided by this grammar.

### 29.2 The subtle case `f . g x`

Application binds more tightly than dot, so:

```mak
f . g x
```

is parsed as:

```mak
f . (g x)
```

It is **not** parsed as:

```mak
(f . g) x
```

The first form attempts to compose `f` with the expression `g x`; that right-hand expression must itself be a function suitable for composition. If the intention is “compose `f` and `g`, then apply the composed function to `x`,” include the parentheses:

```mak
(f . g) x
```

### 29.3 Composition associativity

Dot is right-associative:

```mak
f . g . h
```

means:

```mak
f . (g . h)
```

The resulting function applies `h` first, then `g`, then `f`, conceptually `fn x -> f (g (h x))`. This is the opposite of the syntactic grouping of an application chain such as `f x y`, which groups as `(f x) y`.

### 29.4 Composition of lambdas, and then immediate invocation

```mak
(\x -> x * x) . (\x -> x + 1)
((\x -> x * x) . (\x -> x + 1)) 4
```

The first expression constructs a function. The second constructs the composition and applies it to `4`: increment to `5`, then square to `25`, assuming the numeric operations are valid. The parentheses around the lambdas identify the operands; the outer parentheses make it clear that the composition itself is being invoked.

## 30. Precedence and grouping: worked readings

The complete operator ordering is in §7. These readings are useful when checking a complex line:

| Source | Grouping / reading |
|---|---|
| `f x + y` | `(f x) + y` |
| `f (x + y)` | Apply `f` to the whole sum `x + y` |
| `f x * y` | `(f x) * y` |
| `-f x` | `-(f x)`; application remains inside the unary operand |
| `f xs[i]` | `f (xs[i])` |
| `xs[i][j]` | Index the result of `xs[i]` by `j` |
| `a - b - c` | `(a - b) - c` |
| `a / b * c` | `(a / b) * c` |
| `a && b || c` | `(a && b) || c` |
| `f . g x` | `f . (g x)` |
| `(f . g) x` | Apply the composed function to `x` |
| `f . g . h` | `f . (g . h)` |

A reliable reading method is to mark explicit parentheses first, then attach postfix indexes, then group juxtaposed application chains, then resolve composition and the remaining infix operators according to §7. Add parentheses whenever the intended grouping differs from the grammar's default.

## 31. Top-level `let`, local `let`, and blocks are different contexts

```mak
// Top-level declaration, terminated by semicolon:
let n = 10;
n + 1
```

```mak
// Local let-expression: value; then the expression that uses the binding:
let n = 10; n + 1
```

```mak
// Block with local bindings and a tail value:
{
    let width = 6;
    let height = 7;
    width * height
}
```

At file level, the parser reads declarations while the next token starts `let`, `type`, or `bring`, then parses at most one final expression. A file containing only declarations is given a parser-supplied body `0`. The final top-level expression normally has no trailing semicolon.

A local `let` has the shape `let name [parameters] = value; body`. Its semicolon separates the bound value from the body expression; both are required. A block, by contrast, contains zero or more semicolon-terminated expressions followed optionally by a final tail expression. The tail expression supplies the block result. An empty block parses as integer `0`.

## 32. `case` patterns: constructor spelling and tuple patterns

The general shape is:

```mak
case scrutinee of {
    pattern -> expression;
    other_pattern => expression
}
```

Both `->` and `=>` are accepted between a pattern and its arm expression. Arm semicolons are optional, though consistent separators are easier to scan.

```mak
type Maybe a {
    None;
    Some a;
};

let unwrap_or_zero option = case option of {
    None -> 0;
    Some value -> value
};
```

Pattern names are classified by their first ASCII character: an uppercase-leading identifier is interpreted as a constructor pattern; a lowercase-leading identifier binds the matched value; `_` is a wildcard. Constructor subpatterns follow the constructor as adjacent pattern atoms.

```mak
case value of {
    0 -> "zero";
    "ready" -> "text";
    (left, right) -> left + right;
    _ -> "other"
}
```

Tuple patterns such as `(left, right)` and the empty pattern `()` exist even though tuple values and `()` are not supported as ordinary expression literals. A parenthesized single pattern `(x)` groups `x`; it does not make a one-element tuple pattern. Float literal patterns are not accepted.

## 33. Type syntax: more examples and non-examples

Arrows in MAK types associate to the right:

```mak
i32 -> i32 -> i32
```

This parses as `i32 -> (i32 -> i32)`. Parentheses can make grouping explicit. A tuple-type input is written separately:

```mak
(i32, i32) -> i32
```

That syntax differs from `i32 -> i32 -> i32`: the first writes a tuple type on the left of the arrow; the second nests arrows. Type expressions occur in constructor fields and external declaration annotations; this grammar does not provide general type annotations on arbitrary `let` bindings or lambda parameters.

```mak
[u8]             // array type
[[i32]]          // array whose element type is [i32]
(List a)         // named type application
(Result a e)     // named type application with two type arguments
()               // unit type
!                // bottom type
```

A named type application is written with parentheses and the named type head first. Bare `List a` is not this type-application form. `[u8]` is a type, not an array value literal. `(i32)` groups the type `i32`; it is not a one-element tuple type. `(i32,)` is not accepted by the tuple-type grammar.

## 34. Lexical and grammatical traps checklist

| Tempting spelling | Actual rule |
|---|---|
| `true`, `false` | Ordinary identifiers lexically; they are not dedicated boolean literal tokens |
| `!=` | Not recognized; use `/=` for not-equal |
| `a.b` | Dot means composition, not field access |
| `[1, 2]` | No array literal syntax; square brackets postfix-index an existing expression |
| `(a, b)` as an expression | No general tuple-expression syntax, despite tuple types/patterns existing |
| `()` as an expression | No standalone unit expression atom |
| `f(x, y)` | Not a comma-separated call; for a two-parameter function use application like `f x y` |
| `let x: i32 = 1;` | General `let` binding type annotations are not provided by this grammar |
| `\(x: i32) -> x` | Lambda parameter type annotations are not provided |
| `if condition then a else b` | No `if`/`else` expression in this parser grammar; `case` is a supported branching form |
| `x += 1` | No compound-assignment expression form |
| `fn x -> x;` as the final top-level expression | A trailing semicolon is not the terminator for a final top-level expression |
| `pass` | Reserved lexically, but no expression/declaration form is implemented for it in this parser |

## 35. Fast reference: common tasks

```mak
// Bind a value
let count = 3;

// Define a named one-parameter function
let square x = x * x;

// Define a named two-parameter function
let add x y = x + y;

// Call a named function
square 12

// Immediately call an anonymous function
(\x -> x * x) 12

// Same immediate call without the readability space
(\x -> x * x)12

// Pass parenthesized expressions as separate arguments
add (2 + 3) (4 + 5)

// Index a value, then pass the selected item to a function
consume items[i]

// Compose two unary functions and then invoke the composed function
(increment . double) 10

// Pattern matching
case answer of {
    0 -> "zero";
    _ -> "nonzero"
}

// Return the final expression from a block
{
    let n = 6;
    n * n
}
```

The safest habits are: parenthesize immediately invoked lambdas; use whitespace-separated arguments rather than comma-call syntax; distinguish multi-parameter functions from nested unary functions; and parenthesize a composition before applying it when you mean `(f . g) x` rather than `f . (g x)`.

---

# Part IV — Full implementation-oriented reference

## 36. Scope and confidence: grammar versus later compiler stages

This document describes the MAK lexer, parser, representation inference, and C/code-generation paths present in the source snapshot supplied with this reference. It distinguishes four separate questions that should not be conflated:

1. **Lexically accepted**: the byte sequence is tokenized as the documented tokens.
2. **Grammatically accepted**: those tokens form an AST according to `parser.rs`.
3. **Accepted by later analysis**: name lookup, arity, representations, pointer shapes, and other analysis pass the compiler's checks.
4. **Safe and meaningful at runtime**: the program obeys foreign ABI contracts, pointer lifetimes, index bounds, and the contracts of the called code.

A construct may be grammatically accepted but fail later analysis. Conversely, a construct can compile while being unsafe because the compiler cannot prove a foreign pointer valid. Whenever this guide says *parses as*, that statement is about syntax and grouping; it is not automatically a promise of type correctness or runtime safety.

The implementation uses a Pratt-style expression parser. This matters because application, indexing, prefix operators, and infix operators are not all handled by one ordinary mathematical precedence table. Application and indexing are recognized in the parser loop before the infix operator table, and prefix operators parse an operand at a dedicated binding power.

## 37. Complete lexical inventory

### 37.1 Reserved words

Exactly these spellings are lexed as keyword tokens:

| Spelling | Parser role |
|---|---|
| `let` | Top-level binding, local binding, or external declaration introducer |
| `fn` | Anonymous function introducer |
| `case` | Pattern-match expression introducer |
| `type` | Top-level type declaration introducer |
| `extern` | External declaration introducer after top-level `let` |
| `of` | Delimiter between a `case` scrutinee and its arm block |
| `bring` | Top-level source-path declaration introducer |
| `pass` | Reserved token, but no expression/declaration production in the parser |

`external` is not a reserved keyword token. It is an ordinary identifier treated specially only immediately after `let` at top level or in the parser's external-declaration check. It cannot be used as a normal binding name in those specific declaration positions.

`true` and `false` are ordinary identifiers in ordinary source expressions. The external descriptor parser special-cases `pure = <identifier>` and sets the purity flag only when the identifier's text is exactly `true`; every other identifier spelling sets it to false. Do not treat that descriptor special case as boolean literal syntax.

### 37.2 Punctuation and operator tokens

The lexer recognizes the following one- and two-byte punctuation/operator forms:

```text
{ } ( ) [ ] ; , : . \
-> => = + - * / % & | ^ ~ !
== /= < > <= >= << >> && ||
```

The foreign-type parser additionally recognizes three consecutive dot tokens as the variadic marker `...` inside foreign function argument lists. The lexer does not need a dedicated ellipsis token for that syntax.

Notable lexical facts:

- `/=` is the not-equal token.
- `!=` is tokenized as `!` followed by `=` and is not a not-equal operator.
- `=>` is a single arrow token, not an infix operator.
- `->` is a single arrow token.
- The lexer checks two-character forms before one-character forms where required (`==`, `=>`, `/=`, `<=`, `<<`, `>=`, `>>`, `&&`, `||`).
- No assignment operators such as `+=`, `-=`, `*=`, `/=`, or `|=` are defined as assignment syntax. Note that `/=` is already the not-equal operator, so it cannot be used for compound assignment.

### 37.3 Identifier bytes

The scanner uses ASCII byte tables. An identifier's first byte must be `A`–`Z`, `a`–`z`, or `_`. Remaining bytes may also contain decimal digits. Thus:

```mak
alpha
Alpha
_alpha
item7
A9_b
```

are lexically valid identifiers, while `7item` is an integer token followed by an identifier, not one identifier. Non-ASCII bytes are not valid identifier bytes in this scanner and may produce an unexpected-byte error.

Names are case-sensitive: `value`, `Value`, and `VALUE` are three different spellings. The first character's case also has a second role in pattern parsing: an uppercase-leading pattern identifier is treated as a constructor pattern, while a lowercase-leading identifier is a binding pattern. This is a syntactic classification, not merely a naming convention.

## 38. Integer-literal scanning in exact detail

### 38.1 Forms recognized

```mak
0
7
42
1_000_000
0x2a
0X2A
0b101010
0B101010
```

Decimal integer scanning accumulates digits in an unsigned 64-bit value with wrapping arithmetic. Underscores encountered in the decimal-digit run are skipped. The implementation does not validate underscore placement, so do not assume that underscores are limited to conventional digit separators.

Hexadecimal literals begin with `0x` or `0X`; the scanner consumes hexadecimal digits after the prefix and accumulates them in `u64`. Binary literals begin with `0b` or `0B`; only `0` and `1` are consumed after that prefix.

### 38.2 Important scanner edge cases

The scanner implementation is permissive in ways that are easy to miss:

- It does not require at least one valid digit after `0x`, `0X`, `0b`, or `0B` before producing an integer token. For example, the token boundary for `0x` is consumed as an integer value of zero; a following non-hex byte begins the next token.
- Decimal underscore placement is not validated. The scanner skips underscores wherever they occur during the decimal digit scan, including repeated or leading-position underscores after the first digit has already triggered number scanning.
- Hex and binary scanning do not treat underscores as part of the literal. The underscore ends the number token and begins a subsequent token, which is usually not what was intended.
- Integer overflow uses the scanner's wrapping accumulator; a source integer does not necessarily fail lexing just because its mathematical value exceeds `u64`.
- The leading minus sign is a separate unary operator. `-42` is not a negative integer token.

For portable, readable source, use ordinary decimal digit separators and complete hexadecimal/binary digits even where the scanner accepts less conventional input.

### 38.3 Integers are not intrinsically signed source tokens

The lexer stores an integer token as `u64`. The later representation-selection logic may choose a compact integer representation according to the value and context. Therefore a literal token's lexical representation should not be mistaken for a fixed, user-declared source type. MAK has names such as `i32`, `u64`, and others in type/foreign declarations, but the expression grammar provides no suffix such as `42u32` and no expression-level `: u32` type annotation.

## 39. Floating-point scanning in exact detail

### 39.1 Forms recognized

```mak
0.0
3.14159
1e3
1E-3
6.02e+23
12.0E8
```

The scanner first consumes a decimal digit run. A dot belongs to the float token only when the byte after the dot is a decimal digit. An exponent begins with `e` or `E`, may have one `+` or `-`, then consumes any following digits. The scanner does not require that an exponent contain at least one digit.
Underscores are consumed by the integer-part digit loop only. The digits following the decimal point and exponent are scanned without underscore skipping, so do not use underscores in fractional or exponent portions.

### 39.2 Deliberate consequences

- `10.0` is one floating literal.
- `10.` is an integer token `10` followed by the composition token `.`; it is not a complete float literal.
- `.5` begins with the composition token `.` rather than a floating literal, because number scanning is entered only when the first byte is a digit.
- `1e3` is a floating literal.
- `1e` is consumed as a floating-literal spelling by the scanner; Rust's parsing of the collected text fails and the implementation falls back to `0.0`. This is an implementation quirk, not recommended syntax.
- There is no `NaN` or `Infinity` literal token. Such spellings are ordinary identifiers unless provided as named values by some environment.

For predictable code, always put a digit before and after a decimal point and provide exponent digits whenever an exponent marker appears.

## 40. String decoding and byte semantics

### 40.1 Quote delimiters

Both single-quoted and double-quoted strings are accepted. The lexer uses the quote that opened the string as the closing delimiter.

```mak
"double quotes"
'single quotes'
"embedded ' single quote"
'embedded " double quote'
```

### 40.2 Escape processing

The escape decoder recognizes these conventional forms:

| Source characters | Decoded byte/value |
|---|---|
| `\\` | Backslash (`0x5c`) |
| `\n` | Newline (`0x0a`) |
| `\r` | Carriage return (`0x0d`) |
| `\t` | Tab (`0x09`) |
| `\0` | NUL (`0x00`) |
| `\"` | Double quote |
| `\'` | Single quote |
| `\xHH` | Byte produced by two hexadecimal digits, when the decoder sees a complete valid pair |
| backslash followed by another character | The backslash is removed and that character is emitted, unless a specific escape rule applies |

Strings are represented as byte vectors by the AST, not as a language-level Unicode scalar sequence. The lexer operates on bytes. If a source string contains a literal non-ASCII sequence, the bytes of that sequence are retained; no normalization or code-point escape syntax is provided by the grammar.

An actual newline within a quoted string is permitted by the scanning loop as long as the matching closing quote eventually appears. A NUL byte marks end-of-input for the lexer, so an embedded NUL in the input buffer is not ordinary source text in the normal compiler entry path.

An unterminated quoted string is a lexical error. Escape sequences do not make a missing closing quote acceptable.

## 41. Comments and whitespace

### 41.1 Line comments

`//` begins a comment that continues until a newline or the scanner's end marker. The newline itself is processed as whitespace on the next iteration.

### 41.2 Nested block comments

`/*` begins a block comment. The scanner tracks nesting depth, so nested block comments are accepted:

```mak
/* outer
   /* inner */
   still outer
*/
42
```

A closing `*/` decrements the depth. The outer comment finishes when the depth returns to zero. Reaching end-of-input before that happens raises an unterminated-comment lexer error.

### 41.3 Newlines are not separators with grammatical force

Whitespace is ignored between tokens. A newline does not terminate a top-level declaration, end a `let`, finish a block expression, or implicitly insert a semicolon. Where the grammar requires `;`, supply it explicitly.

```mak
let answer =
    40 +
    2;
answer
```

This works because the tokens still form one expression; it is not an indentation-sensitive language.

## 42. Exact top-level parser behavior

The parser first consumes as many top-level declarations as it can recognize from the leading token. The only top-level declaration starters are `let`, `type`, and `bring`. After declarations, it parses at most one expression as the program body. If input ends directly after declarations, the parser synthesizes integer `0` as the body.

That gives the following shapes:

```mak
// Declarations plus a body expression.
let n = 10;
n + 2
```

```mak
// A declaration-only file; parser body defaults to 0.
let n = 10;
```

```mak
// A body expression may be a block.
{
    let n = 10;
    n + 2
}
```

A semicolon is required after each top-level declaration. The final top-level expression is parsed as the body and generally must not be followed by a semicolon, because the parser expects end-of-input once the body has been parsed. There is no rule that turns several bare top-level expressions into a sequence.

The parser has error recovery for malformed declarations and for some expression contexts, but error recovery is not a supported alternative syntax. A program should not rely on the fallback integer body installed after a parse error; the compiler reports the accumulated parser errors.

## 43. `let` binding subtleties

### 43.1 Top-level `let`

```mak
let value = expression;
let function x y = expression;
```

The top-level parser reads an identifier after `let`, then any additional identifier tokens until it reaches `=`. If extra identifiers were collected, the value expression is wrapped in a function node with those parameters. The binding itself is a single declaration, closed by `;`.

### 43.2 Local expression `let`

```mak
let value = expression; body
let function x y = expression; body
```

In a local `let` expression, the mandatory semicolon divides the bound value from the body expression. It does not simply terminate a declaration and continue parsing a peer expression at the same grammar level.

### 43.3 `let` inside a block

```mak
{
    let value = expression;
    body
}
```

The block-specific parser reads the binding and then parses the remaining block contents as the body/scope of that binding. External declarations are explicitly rejected within a block. A local `let` is not a substitute for a C-style mutable-variable declaration: the grammar has no assignment statement.

### 43.4 Parameters are bare names, not patterns

The shortcut `let f x y = ...;` allows identifiers as parameters. It does not support annotations (`let f (x : i32) = ...`), defaults, destructuring parameters, named parameters at call sites, or a parameter type declaration after each identifier.

## 44. Anonymous-function syntax and invocation

All of these are parser-supported function introducers:

```mak
fn x -> x + 1
fn x => x + 1
\x -> x + 1
\x => x + 1
fn (x) -> x + 1
fn (x, y) -> x + y
fn (x y) { x + y }
fn () -> 42
```

The `fn` and backslash forms share the same parameter/body parser. With a parenthesized parameter list, commas are optional separators after each parsed identifier; adjacent identifiers are also accepted. An empty parenthesized list is accepted. Without parentheses, the parser consumes consecutive identifiers as the parameter list until it sees an expression arrow or opening brace.

`->` and `=>` both introduce the body expression. A braced body may be used instead of an arrow:

```mak
fn x {
    let doubled = x * 2;
    doubled + 1
}
```

A zero-parameter callable is written with an empty parenthesized parameter list, and its call is written with the special empty-call syntax:

```mak
let answer = fn () -> 42;
answer()
```

`answer ()` is not the same spelling. The argument parser only gives special treatment to the consecutive `()` token pair immediately following a callee. The parenthesized empty form by itself is not an ordinary expression value.

### 44.1 Immediate invocation patterns

```mak
(\x -> x * x) 12
(\x -> x * x)12
(fn x -> x * x) 12
(fn (x) -> x * x) (3 + 9)
(\x y -> x + y) 20 22
(\(x, y) -> x + y) pair_value
```

The first four examples show a parenthesized lambda expression used as the function expression of an application. The no-space spelling is accepted because juxtaposition, not whitespace itself, creates application. For reliable readability, include a space between the `)` that closes the lambda and the first argument.

The last example is syntactically a lambda with one parameter named `(x, y)`? No: parameter-list patterns do **not** permit tuple destructuring. In a function parameter list, parentheses hold parameter identifiers only. `\(x, y) -> ...` declares two identifier parameters `x` and `y`; it does not declare one tuple parameter. The following argument `pair_value` is therefore the first argument and does not match a source-level tuple-destructuring parameter form. Use ordinary parameters, then pattern-match inside the body if the value being passed has a supported pattern representation.

### 44.2 Curried-looking type arrows versus multiple declared parameters

The type parser treats `A -> B -> C` as `A -> (B -> C)`. Function declarations and lambdas, however, can store a parameter vector with multiple entries. A binding declared as `let add x y = x + y;` is parsed as a function node with two named parameters, and a call written `add 2 3` is ordinarily analyzed as a two-argument call when the compiler flattens the application chain.

Do not infer that this is interchangeable in every context with a value whose type is a chain of unary functions. The parsed parameter count, compiler function arity, and MAK arrow type syntax are distinct layers.

## 45. Application, empty calls, and the absence of tuple calls

### 45.1 Application by juxtaposition

```mak
f x
f x y
f (x + 1)
f xs[i]
f (g x)
```

The parser represents application as an application node and flattens adjacent ordinary application nodes when the compiler analyzes a call. Syntactically, a chain groups left-to-right: `f x y` has the structural grouping `(f x) y` before later call flattening.

### 45.2 Indexing the argument of an application

The parser deliberately parses index suffixes on the next atom before attaching that atom as an argument. Therefore:

```mak
f xs[i]
```

means a call to `f` with `xs[i]` as its argument, rather than an attempt to index the result of `f xs`. To index a call result, parenthesize it:

```mak
(f xs)[i]
```

### 45.3 Empty calls versus empty argument values

```mak
f()
f ()
```

`f()` is recognized as a zero-argument call. `f ()` tries to parse an ordinary empty-parentheses expression as the argument; the ordinary expression-atom parser requires a nonempty expression between parentheses and rejects it. The whitespace is the difference in tokens consumed by the special case.

### 45.4 Calls with multiple arguments

The language does not use comma-separated source expression arguments. Write `f x y`, not `f(x, y)`. A parenthesized expression contains a single expression; the comma token is used in tuple types, tuple patterns, parameter lists, and foreign function type argument lists, not in general calls.

## 46. Function composition: syntax, parser expansion, and use

Composition `.` is not member access and it is not pointer field access. The parser rewrites `lhs . rhs` into an anonymous unary function equivalent to:

```mak
fn $x -> lhs (rhs $x)
```

The actual generated parameter name is an implementation detail; `$x` should be treated as explanatory pseudocode, not a recommended source identifier.

### 46.1 Composition does not immediately execute either function

```mak
let square x = x * x;
let increment x = x + 1;
let square_after_increment = square . increment;
square_after_increment 4
```

The final expression evaluates the composed callable on `4`; conceptually it computes `square (increment 4)`, which is `25` for ordinary arithmetic.

### 46.2 Composition is right-associative

```mak
f . g . h
```

parses as `f . (g . h)`. Under the expansion above, it acts as `f (g (h x))` when called on an input `x`.

### 46.3 Application binds more tightly than composition

```mak
f . g x
```

parses as `f . (g x)`, because `g x` is an application inside the right side of the composition expression. That is not the same as `(f . g) x`. If the intention is to compose first and then apply the resulting function, write:

```mak
(f . g) x
```

### 46.4 Composition requires callable operands when used

The parser can construct composition without proving that either side has a useful function type. Later analysis/runtime will determine whether the operands can actually be applied. A composition expression is not a general-purpose operator for fields, modules, values, or pointers.

## 47. Indexing is syntax sugar for a reserved intrinsic

The parser lowers every `base[index]` suffix to an application of an internal variable named `$mak.index`, with the base and index as arguments. Chained indexing is nested:

```mak
base[i][j]
```

is equivalent in shape to:

```mak
$mak.index ($mak.index base i) j
```

This internal name is compiler syntax, not ordinary public source API. User code should use bracket indexing.

Indexing supports a nonempty expression in brackets and can be chained. It is syntactically permitted on any expression, but the later representation analysis expects a pointer-like base (or a representation it has not yet resolved) and an integer index. A string/pointer may therefore be indexable, whereas a known integer scalar is not a sensible index base.

The language does not define an assignment form for an index. `buffer[i]` is a read expression. Do not assume `buffer[i] = value` mutates memory; the parser has no assignment expression production.

## 48. Pointer and pointer-arithmetic model

### 48.1 Pointer support is a compiler representation/FFI facility, not a complete pointer expression language

MAK's source type syntax has named types, primitive spellings, array types, tuple types, arrow types, parameterized named types, and `!`. It does **not** declare a special MAK pointer type token in `ty_atom`; a bare spelling such as `ptr` in a MAK type annotation is parsed as a named type. By contrast, the foreign type parser gives `ptr` a dedicated C-opaque-pointer meaning and supports C-like stars such as `u8*` and `u8**`.

The compiler's internal representation inference recognizes pointer-like values obtained from strings, foreign data-pointer declarations/returns, generated algebraic-data-type values, and known bindings. This means pointer arithmetic is real at the expression/code-generation level, but its behavior depends on what the compiler knows about the left/right operands and, in particular, the foreign pointer shape.

### 48.2 Supported pointer arithmetic cases

The source parser uses the ordinary arithmetic tokens `+` and `-`; there are no special pointer arithmetic tokens.

| Form | Compiler handling | Notes |
|---|---|---|
| `p + n` | Supported when `p` is pointer-like and `n` is integer-like | Advances by `n` elements if a pointee type is known; otherwise byte steps |
| `n + p` | Supported when `n` is integer-like and `p` is pointer-like | Code generator selects the pointer operand and applies the offset |
| `p - n` | Supported when `p` is pointer-like and `n` is integer-like | Moves backward by `n` elements when pointee information is known |
| `p - q` | Supported when both operands are pointer-like | Produces an integer difference; known typed pointers use element difference, opaque pointers use raw address difference in this code path |
| `n - p` | Not a pointer-arithmetic case handled by the implementation | Do not use |
| `p * n`, `p / n`, `p % n` | Not pointer arithmetic | These are numeric operator tokens; pointers are not valid numeric operands for these operations |
| `p + q` | Not recognized as the supported pointer-plus-offset case unless one operand is integer-like | Two pointers are not defined as additive offsets |

“Supported” above describes code-generation branches that explicitly recognize a pointer representation. It does not grant bounds safety, allocation validity, provenance validity, alignment, or lifetime guarantees.

### 48.3 Typed pointers versus opaque `ptr`

A foreign pointer written as `u8*` carries an element type. Pointer stepping uses C-like element-scaled arithmetic with `uint8_t`-sized elements. A foreign pointer declared as `i32*` steps by `sizeof(i32)` for an integer offset. A repeated pointer such as `u8**` retains nested shape, which supports chained indexing where the source/compiler can recover the nested pointer information.

An opaque foreign `ptr` has no known element type. The implementation deliberately treats it as byte-addressed for pointer offsets and indexing. Therefore adding `1` to an untyped `ptr` means advancing one byte in that fallback path, not one `i32` or one arbitrary object. To obtain typed element stepping, declare the foreign signature with an appropriately typed pointer such as `i32*`, or use a foreign interface whose pointer return/argument shape preserves the element type.

### 48.4 Pointer indexing and dereference

```mak
bytes[i]
words[i]
word_ptr + i
word_ptr[i]
```

The parser turns `p[i]` into the reserved index intrinsic. The direct code generator emits a typed C indexing/dereference expression after it has established the base's pointer representation and inferred pointee information. For a pointer with a recovered C element type, indexing scales by that element type. For an opaque pointer, indexing is byte-addressed as `uint8_t` in the fallback path. For generated MAK algebraic-data-type pointers, a separate path indexes the generated C struct representation.

The index expression must have an integer representation once checked. The base must be pointer-like, unless it is temporarily unresolved during analysis. An invalid known base or non-integer index can be rejected by the direct expression emitter.

### 48.5 Pointer subtraction is not necessarily byte-count subtraction

The code generator distinguishes two situations for `p - q`:

- If it has pointee metadata (or the pointer representation is a generated ADT pointer), it casts both operands to pointers to the known element C type and subtracts them. C pointer subtraction then returns an element count for pointers into the same array/object sequence.
- If it only has opaque pointer representation and no pointer information, it converts addresses to integer-address representations and subtracts those addresses, yielding a raw address difference in the generated path. The unit is therefore effectively bytes, not a portable element count.

Never compare results of these two modes as though they had identical units. The source syntax itself carries no operator-level annotation describing the unit. It depends on pointer-shape information recovered through the relevant external declaration or expression.

### 48.6 No address-of operator or explicit unary dereference

The unary-expression parser recognizes only `-`, `!`, and `~`. In particular:

- `&x` is not address-of syntax. `&` is a binary bitwise operator and requires a left and right operand.
- `*p` is not pointer dereference syntax. In an expression it is parsed as the binary multiplication operator when a left operand is present; a leading `*` is not an accepted prefix expression.
- Use `p[i]` for indexed pointer reads. There is no equivalent of C's generic `*p` unary operator in the expression grammar.

The source also has no explicit cast expression syntax such as `(u8*)p`, no `sizeof` keyword, no pointer literal, and no source-level `null` literal. A named value or foreign function has to supply pointer representations.

### 48.7 Pointer arithmetic safety checklist

Before using a pointer expression, verify all of the following outside the parser's mere syntax guarantee:

1. The pointer is non-null if dereferenced.
2. The pointer comes from a valid allocation/object with a lifetime that covers the read.
3. The index/offset stays within the object or permitted one-past position.
4. Arithmetic cannot overflow or wrap into a different allocation.
5. A typed pointer's declared pointee shape matches the C object actually returned.
6. A pointer returned by one foreign function is released by the allocator/deallocator required by its documented ABI.
7. A pointer difference is computed only for pointers for which the relevant C pointer subtraction semantics are valid; the compiler's opaque-address fallback is not a substitute for a C object model proof.
8. No memory writes are assumed from indexing: the source parser only supplies a read expression and does not define assignment syntax.

### 48.8 Pointer arithmetic worked examples

```mak
// For a foreign `u8*` binding: `p + 1` advances one byte.
p + 1
```

```mak
// For a foreign `i32*` binding: `p + 1` advances one i32 element.
p + 1
```

The surface source is identical; the foreign declaration's shape determines the scaling. These snippets cannot be interpreted fully without knowing the foreign type associated with `p`.

```mak
// Read element i through bracket indexing.
p[i]
```

```mak
// Compute the pointer to element i, then read its value.
(p + i)[0]
```

The latter uses both arithmetic and indexing. For a typed pointer, its offset scales by the known element C type. For an opaque `ptr`, the fallback uses bytes.

```mak
// Integer-first addition is explicitly recognized.
i + p
```

```mak
// Pointer minus offset.
p - i
```

```mak
// Difference of pointer positions; unit depends on pointer metadata.
end_ptr - start_ptr
```

Do not write `i - p`, `p * i`, `p / i`, or `p + q` expecting conventional C pointer arithmetic. The source/compiler does not define those as supported pointer-offset forms.

## 49. Foreign declarations and C ABI boundaries

### 49.1 Main declaration shape

```mak
let extern name : mak_type = "foreign_symbol" : foreign_type;
```

The `external` spelling is also accepted after `let`:

```mak
let external name : mak_type = "foreign_symbol" : foreign_type;
```

The `external` alternate is checked as an ordinary identifier's text in that position. `extern` is a reserved keyword.

### 49.2 MAK-side type and foreign-side type are separate

The optional type before `=` belongs to MAK's source-side declaration. The optional type after the quoted symbol belongs to the foreign/C-facing signature. The two are not the same grammar and have different purposes.

```mak
let extern puts : ptr -> i32 = "puts" : (ptr) -> i32;
```

This form is a syntax example, not a correct semantic signature for the standard C `puts`, which accepts a `const char *` and returns an `int`. The appropriate shape for a C string argument is more closely represented by `(ptr) -> i32` in this foreign grammar, but the source-side annotation also needs to agree with how the compiler treats strings/function calls in the installed runtime. Always validate the actual ABI against the real C declaration.

### 49.3 Foreign atom types

The foreign-type parser recognizes:

`i8`, `i16`, `i32`, `i64`, `u8`, `u16`, `u32`, `u64`, `f32`, `f64`, `ptr`, `void`, and arbitrary named identifiers.

The parser also accepts pointer stars following a foreign atom or named type:

```mak
u8*
i32*
i32**
u8***
```

Each `*` wraps another `PtrTo` layer in the parsed foreign type representation, preserving nested shape.

### 49.4 Foreign function signatures

```mak
(i32) -> i32
(i32, i32) -> i32
(ptr, u64) -> ptr
() -> i32
(i32, ...) -> i32
```

The first parenthesized group is the comma-separated list of foreign arguments; `->` follows the closing `)` and introduces the return type. `...` is supported in an argument list as a variadic marker. A trailing comma is permitted in the argument list by the parser's loop behavior.

Foreign function type arrows are parsed recursively on the return side, so nested return function types are syntactically representable. Whether a given foreign function-pointer shape can be emitted or called safely depends on later code generation and the target ABI.

### 49.5 `global` external objects

```mak
let extern errno : i32 = global "errno" : i32;
let extern raw_bytes : [u8] = global "raw_bytes" : u8*;
```

The soft identifier `global` immediately after `=` marks the declaration as a global object, not an ordinary function symbol. The string afterward is the linked symbol name. The C object's actual declaration and storage shape must match the generated external declaration. Do not declare a C array symbol when the generated interface expects a pointer variable, or vice versa, without accounting for the C-level declaration ABI.

### 49.6 Descriptor blocks

```mak
let extern log_value : i32 -> i32 = "log_value" : (i32) -> i32 {
    pure = false;
    from = "stdint.h";
    abi = "C";
};
```

Descriptor blocks contain identifier keys, `=`, and descriptor values. `pure` is parsed specially as an identifier value. `from` and `abi` require quoted string values and populate dedicated fields. Other descriptor keys are accepted and their values are parsed as expressions, but do not acquire special behavior from the parser alone. A semicolon may follow each entry; the last entry may omit it. The external declaration still requires its terminating semicolon after the descriptor block.
The shown C generator uses a `from` value to add a C header include: a value beginning with `<` or `"` is emitted after `#include` literally, while another value is wrapped in angle brackets. Thus `from = "stdio.h"` results in `#include <stdio.h>`; `from` is not a library-link directive by itself. The `abi` string is recorded as metadata by the parser; do not assume that the descriptor alone selects a compiler calling convention or validates the real ABI.

Descriptor text records metadata; it does not automatically prove that a symbol exists, link a library by itself in every build mode, or validate ABI correctness. Inspect the build/link configuration and downstream compiler behavior before relying on `from` or `abi` metadata operationally.

### 49.7 Foreign-call production checklist

For each foreign declaration, verify:

- The exact symbol name, including platform-specific spelling where relevant.
- The C calling convention / ABI expected by the object and the target.
- Argument count, order, representation, integer width, floating width, and variadic promotions.
- Return type and whether the C function may return `void`.
- Ownership: who allocates and who frees returned/received memory.
- Nullability, bounds, alignment, encoding, and lifetime of every pointer.
- Purity metadata only when the function truly has no observable side effects and can be safely treated as pure by optimization.
- Whether a C global is an object, pointer object, array, thread-local value, or special macro (macros are not linkable symbols).

## 50. User-defined types and constructor declarations

### 50.1 Abstract/no-constructor declarations

```mak
type Opaque;
type Handle state;
```

The semicolon ends the type declaration. The identifiers after the type name are type parameters in the declaration grammar. The parser stores a type declaration with no constructors.

### 50.2 Algebraic data type declarations

```mak
type Maybe a {
    None;
    Some a;
};

type Result value error {
    Ok value;
    Error error;
};
```

After the type name, consecutive identifiers before `{` are stored as type parameter names. Within the braces, each constructor starts with an identifier and is followed by zero or more type expressions until `;` or `}`. A constructor may omit its final semicolon when it is immediately followed by the closing brace. The type declaration itself requires `;` after `}`.

Constructor names conventionally begin uppercase because the pattern parser uses that capitalization to classify constructor patterns. Keep constructor naming consistent across declarations, expressions, and patterns.

### 50.3 Constructor fields are types, not source argument names

In:

```mak
type Node {
    Node i64 Node;
};
```

the tokens after `Node` in the declaration are parsed as field types (`i64`, `Node`), not as parameter names. In contrast, in an expression/pattern:

```mak
Node count child
```

`count` and `child` are expression values or subpatterns. The same surface identifier position means different things according to syntactic context.

### 50.4 Parameterized type use

Type application is parenthesized with the named constructor first:

```mak
(List u8)
(Result (List u8) i32)
(Tree key value)
```

A bare `List u8` is not the accepted type-application spelling in the type parser. Parenthesized type application requires the head to parse as a named type. For example, the parser reports a specific error when the head is a non-named type such as an array type.
A further implementation boundary: while reading arguments inside a parenthesized type application, the parser accepts type-atom starters `identifier`, `(`, and `[`, but not a bare `!` token as the next type argument. Use a parenthesized form such as `(!)` if a bottom type must occupy a type-application argument position. Arguments that are function types should similarly be parenthesized so the application grammar sees them as one type atom.

## 51. Type grammar: all accepted source forms

The parser implements the following shape, with arrow recursion on the right:

```ebnf
type       ::= type-atom [ "->" type ]
type-atom  ::= primitive-or-named-identifier
             | "[" type "]"
             | "("")"
             | "(" type ")"
             | "(" type "," type { "," type } ")"
             | "(" named-type type-atom { type-atom } ")"
             | "!"
```

Examples:

```mak
u8
i64
f64
MyType
[u8]
[[i32]]
()
(i32)
(i32, u8)
(i32, u8, f64)
i32 -> i32
u8 -> u64 -> f64
(i32 -> i32) -> i32
(List u8)
(Result (List u8) i32)
!
```

Important exact boundaries:

- `(T)` is grouping of a type, not a one-element tuple type.
- `(T, U)` starts a tuple type. It requires a second type after the first comma; a one-element `(T,)` form is not accepted.
- `()` is the unit type in a type expression, even though `()` is not a standalone ordinary expression.
- `[T]` is a type form. It is not an array constructor or array literal.
- `A -> B -> C` parses as `A -> (B -> C)` because the right side calls the full type parser recursively.
- `(A -> B) -> C` requires parentheses around the function type on the left.
- `!` is parsed as a bottom-type form.
- The primitive spellings are recognized by identifier text. There is no separate lexical keyword token for primitive types.
- Any other identifier can be a named type, including `ptr`; in MAK-side syntax `ptr` is not special in the way it is in foreign types.

## 52. Pattern matching: precise behavior

### 52.1 Scrutinee and arms

```mak
case scrutinee of {
    pattern -> body;
    other_pattern => other_body
}
```

The parser requires the keyword `of` after the scrutinee, then `{`, then zero or more arms, then `}`. Each arm requires a pattern followed by `->` or `=>`, then a body expression. The semicolon after an arm is optional.

### 52.2 Supported pattern forms

| Pattern | Parser behavior |
|---|---|
| `_` | Wildcard, no binding name |
| `x` | Lowercase/non-uppercase identifier binds a value |
| `0` | Unsigned integer literal pattern |
| `"x"` | String literal pattern |
| `Some x` | Uppercase constructor and one subpattern |
| `Node left right` | Uppercase constructor and two subpatterns |
| `()` | Empty tuple pattern node |
| `(x)` | Parenthesized pattern, returned as `x` |
| `(x, y)` | Tuple pattern with two entries |
| `(x, y, z)` | Tuple pattern with three entries |

Floating-point patterns are not supported. A pattern such as `x + 1` is not a pattern expression. There is no range pattern, alias pattern (`name @ pattern`), guard clause, type annotation pattern, or arbitrary nested operator syntax in the pattern grammar.

### 52.3 Constructor classification uses first-byte capitalization

For an identifier pattern, the parser checks whether the first byte is ASCII uppercase. If it is, the parser consumes subsequent pattern atoms as that constructor's arguments. Otherwise, it creates a variable binding. `_` is recognized specially as wildcard.

As a consequence, constructor naming is syntactically meaningful:

```mak
Some value    // constructor pattern with one subpattern
some          // ordinary binding pattern
_             // wildcard
```

### 52.4 Constructor subpattern greediness

The constructor parser keeps consuming following pattern atoms as constructor arguments while `starts_atom()` is true. Parenthesize nested tuple patterns and be deliberate about arm boundaries when writing constructor patterns. The arrow token ends the constructor argument sequence because it is not a pattern atom.

### 52.5 Exhaustiveness is not a grammar property

The parser accepts the shape of a case expression; coverage, constructor arity, representation compatibility, and later pattern lowering are separate analysis/compiler responsibilities. Include a fallback wildcard when appropriate, but do not assume every case lacking `_` is automatically rejected or automatically safe—the answer depends on the later passes and actual subject type.

## 53. Blocks: result values and sequencing

A block is a braced sequence of expressions separated by semicolons, optionally ending in an expression without a semicolon. The final expression is the block result. When the block is empty, the parser represents it as integer zero. If a one-expression block closes after the semicolon, the parser's block-building logic returns that prior expression as the resulting expression kind rather than always retaining a distinct block node.

```mak
{
    first;
    second;
    final_value
}
```

Use semicolons for sequencing, not indentation. The block parser has a special local-binding path for `let`; the binding scopes over the remaining block contents.

No loop construct is present in the parser. A block is not a loop. It does not repeat its statements, and a semicolon does not change evaluation into an imperative assignment statement.

## 54. Operator reference: semantics at three layers

### 54.1 Syntax-layer table

| Operator | Syntactic category | Binding/association |
|---|---|---|
| `x[i]` | Postfix indexing | Tightest postfix form; chains leftward |
| `f x` | Implicit application | Recognized before infix operators |
| `()` after a callee | Special empty call | Recognized before infix operators |
| `-x` | Prefix unary | Operand can include application; stops before composition and other infix operators |
| `!x` | Prefix unary | Same prefix binding behavior |
| `~x` | Prefix unary | Same prefix binding behavior |
| `.` | Composition | Binding power 9/9; right-associative |
| `* / %` | Multiplicative | 7/8; left-associative |
| `+ -` | Additive | 6/7; left-associative |
| `<< >>` | Shifts | 5/6; left-associative |
| `& ^ |` | Bitwise | 4/5; left-associative at the same precedence level in this parser table |
| `== /= < > <= >=` | Comparisons | 3/4; left-associative syntactically |
| `&&` | Logical AND | 2/3; left-associative |
| `||` | Logical OR | 1/2; left-associative |

### 54.2 Meaning can depend on representation

The operator token is parsed before the compiler knows the final integer width, pointer shape, or inferred representation. The same `+` token can become numeric addition or a recognized pointer offset. Similarly, `%` is a remainder operation and should be used with integer-like operands when integer remainder semantics are desired. Do not rely on operator spelling alone to establish a static type that the expression grammar itself does not encode.

### 54.3 Comparison chains are not mathematical chain syntax

Because the parser places `==`, `/=`, `<`, `>`, `<=`, and `>=` at one precedence level with left association, an expression such as:

```mak
a < b < c
```

is grouped as:

```mak
(a < b) < c
```

It is not a special mathematical range test. Use explicit conjunction and parenthesized comparisons for clarity:

```mak
(a < b) && (b < c)
```

Whether the latter's operands have compatible representations is then checked/lowered separately.

### 54.4 Avoid relying on side-effect order without checking code generation

The parser determines structural grouping, but evaluation order, short-circuit behavior, and optimization depend on the compiler's lowering of the particular expression. The generated direct C path emits `&&` and `||` as C logical operators in the corresponding paths, which generally short-circuit in C. Other optimized or interpreted paths may have separate implementations. For side-effecting foreign calls, prefer explicit sequencing in a block rather than using evaluation order hidden inside arithmetic or comparisons.

## 55. Unsupported syntax: definitive checklist from the parser

The expression/type/declaration parser has no ordinary production for the following forms. Some related syntax may be accepted in another context; that does not make the listed form a feature.

| Unsupported form | Closest supported syntax or explanation |
|---|---|
| `if condition then a else b` | Use `case` on a condition result or a user-defined representation; no `if` keyword production exists |
| `while`, `for`, `loop` | No loop grammar; recursion or a runtime helper must provide repetition |
| `switch` | Use `case` for pattern matching |
| `x = y` as assignment | `=` is used in bindings/declarations; no assignment-expression production |
| `x += y` | No compound assignment tokens |
| `x != y` | Use `/=` for not-equal |
| `array = [1, 2, 3]` | No array literal production |
| `(a, b)` as tuple value | Parentheses group one expression; comma tuples exist in type/pattern grammar, not value expression grammar |
| `()` as unit expression | `()` is recognized as special call syntax only after a callee, and as unit in type/pattern contexts |
| `object.field` | Dot is function composition |
| `*pointer` unary dereference | No unary-star production; use supported indexing `pointer[0]` |
| `&value` address-of | Ampersand is binary bitwise AND, not address-of |
| `value as Type` | No cast expression production |
| `x : Type` expression annotation | No general expression type-annotation production |
| `fn (x : i32) -> ...` | Function parameters are identifier names only |
| `f(a, b)` | Calls take arguments by juxtaposition: `f a b` |
| `f(key = value)` | No named-argument syntax |
| `x?` / optional chaining | No postfix question-mark grammar |
| `x ?? fallback` | No null-coalescing token/production |
| `...` in normal expression | Ellipsis is only collected as a foreign variadic marker in the foreign function type parser |
| `case 1.0 of { ... }` with a float pattern | Float literal patterns are not accepted |
| `name @ pattern` | No alias-pattern form |
| type-level C stars in ordinary MAK `type` syntax | Stars are recognized in `foreign_ty`, not `ty_atom` |
| `bring module as alias;` | Bring accepts one or more string paths only |
| `import { x } from "file";` | No imported-name list form |
| optional top-level declaration semicolon | Every ordinary top-level `let`, `type`, and `bring` declaration requires its semicolon |
| semicolon after final program expression | Parser expects EOF after its one final expression |

## 56. Error-oriented examples: read the diagnostic as a grammar boundary

These intentionally invalid examples are included to make the grammar limits explicit. Do not paste them into a valid source file unchanged.

### 56.1 Missing declaration semicolon

```mak
let x = 1
x
```

The top-level binding grammar requires `;` after the bound expression. A newline does not replace it.

### 56.2 Unsupported array literal

```mak
[1, 2, 3]
```

An expression atom cannot begin with `[`. Square brackets form an array type when the type parser is active, or an index suffix following an expression in expression parsing.

### 56.3 Unsupported comma-separated call

```mak
sum(1, 2)
```

The parser sees `sum` and an opening parenthesis as an argument atom; expression parentheses accept one expression, and comma is not a source-call separator. Use `sum 1 2` when the function is declared to take two parameters.

### 56.4 Unsupported member syntax

```mak
record.field
```

The dot token is parsed as composition, so it tries to compose the left and right expressions. It does not look up a record member.

### 56.5 Unsupported expression tuple

```mak
let pair = (left, right);
```

The ordinary expression parser does not have comma tuples. Tuple types and tuple patterns do not imply tuple values can be constructed with the same comma syntax.

### 56.6 Unsupported unary pointer operators

```mak
*p
&x
```

Neither is parsed as a C-style pointer operator. A leading `*` is not a recognized prefix operator; `&` is a binary token. Use a pointer supplied by a foreign interface and read through a valid `p[index]` form when the representation is supported.

## 57. Production-quality source habits for this grammar

1. Use spaces in application and immediately-called lambdas: write `(\x -> x * x) 7` rather than the harder-to-scan `(\x -> x * x)7`.
2. Parenthesize composition intentionally: prefer `(f . g) x` for applying a composition, and `f . (g x)` when that is the intended right operand.
3. Keep top-level `let`, `type`, and `bring` declarations each on a semicolon-terminated statement.
4. Use `case` explicitly rather than assuming conventional `if` syntax exists.
5. Use the constructor capitalization consistently because parser classification relies on capitalization.
6. Treat raw pointer values as foreign capabilities. The compiler does not infer allocation ownership from `ptr`.
7. Specify typed foreign pointers when the C interface truly returns a pointer to a known element type; use opaque `ptr` when the pointee is genuinely opaque.
8. For effectful calls, sequence them visibly in a block rather than hiding them in operand expressions.
9. Validate FFI signatures against the platform's C headers and build/link options.
10. Keep parser conformance separate from semantic testing; a successfully parsed program can still have a wrong ABI or unsafe memory access.

## 58. Integrated systems-style sample: one source file exercising the language surface

This is a deliberately broad integration sample, not a claim that MAK has C's entire syntax. It uses the documented syntax in one module: top-level bindings, a parameterized data type, external functions and a global pointer, descriptor metadata, function declarations, inline lambdas and immediate invocation, composition, local bindings, blocks, case patterns, indexing, pointer offsets/subtraction, numeric and bitwise operators, comparisons, logical operators, string/numeric literals, nested type forms in a type-only declaration, and comments.

The example intentionally does **not** pretend unsupported forms exist: it does not use `if`, loops, assignment, array literals, tuple-expression construction, field access, unary pointer dereference, or address-of.

```mak
// All source declarations needed by this single module are local; the C helper
// is supplied by the companion C translation unit below.

// A parameterized algebraic data type, with a nullary constructor and two
// unary constructors. Constructor capitalization is significant in patterns.
type Result value error {
    Ok value;
    Error error;
};

// An opaque declaration: no constructor body is declared here.
type RuntimeHandle state;

// A nested parameterized type field documents a buffer-result shape.
// The declaration introduces a constructor; it does not construct a value.
type BufferEnvelope {
    BufferEnvelope (Result [u8] i32);
};

// Standard C symbol. Check the target platform's real declaration and ABI.
let extern putchar : i32 -> i32 = "putchar" : (i32) -> i32 {
    pure = false;
    from = "stdio.h";
    abi = "C";
};

// A C global variable named `sample_bytes` whose object value is a u8*.
// The matching C shim below defines it as a pointer variable.
let extern sample_bytes : [u8] = global "sample_bytes" : u8*;

// An opaque pointer example. `ptr` has no pointee shape in the foreign type.
let extern opaque_bytes : ptr = global "opaque_bytes" : ptr;

// A pointer-taking C function. Its real C implementation is supplied in the
// companion-shim example after this block; the signature is only a declaration.
let extern c_byte_count : ptr -> u64 = "c_byte_count" : (ptr) -> u64 {
    pure = true;
    from = "stdint.h";
    abi = "C";
};

// Arithmetic and bitwise operators with integer-like arguments.
let negate x = -x;
let logical_not x = !x;
let float_probe x = (x + 0.5) * 2.0;
let normalize_byte x = (x % 256 + 256) % 256;
let mix_bits x = ((x << 1) | (x >> 1)) ^ 0x5a;
let keep_low_bits x = x & ~0xff00;

// All numeric operator categories shown explicitly. This helper assumes its
// inputs are compatible integers; it is an operator reference, not a parser
// promise about a particular inferred machine width.
let arithmetic_probe a b = {
    let add = a + b;
    let sub = a - b;
    let mul = a * b;
    let div = a / b;
    let rem = a % b;
    let left_shift = a << b;
    let right_shift = a >> b;
    let bit_and = a & b;
    let bit_xor = a ^ b;
    let bit_or = a | b;
    // Comparisons, not-equal, and logical operators.
    let eq_flag = add == sub;
    let ne_flag = add /= sub;
    let lt_flag = add < mul;
    let gt_flag = div > rem;
    let le_flag = add <= mul;
    let ge_flag = add >= mul;
    let logical_and = (eq_flag == 1) && (ne_flag == 1);
    let logical_or = (lt_flag == 1) || (gt_flag == 1);
    // Use the computed names in a result so the block has a final value.
    (bit_and ^ bit_xor ^ bit_or) + (left_shift - right_shift)
};

// Inline lambda, named lambda, multi-parameter lambda, immediate invocation,
// and normal application are all demonstrated here.
let square = fn x -> x * x;
let increment = \x => x + 1;
let square_after_increment = square . increment;
let immediate_square = (\x -> x * x) 12;
let immediate_add = (fn (x, y) -> x + y) 20 22;
let composed_result = square_after_increment 4;

// Application and composition are intentionally kept separate:
// `transform value` calls transform; `transform . transform` builds a function.
let transform x = normalize_byte (mix_bits x);
let transform_twice = transform . transform;

// Recursively walk a foreign byte pointer. The base case is expressed with
// case rather than a nonexistent `if`. The pointer comes from a C shim and
// must point to at least `remaining` readable bytes.
let checksum_bytes p remaining =
    case remaining == 0 of {
        1 -> 0;
        _ -> (p[0] + checksum_bytes (p + 1) (remaining - 1)) % 65521
    };

// Validate an integer range without a conditional keyword.
let validate_byte x =
    case (x >= 0) && (x < 256) of {
        1 -> Ok x;
        _ -> Error 1
    };

// A consumer of Result uses constructor patterns, a binding pattern, a
// literal pattern, and wildcard fallback. The outer `case` is a data match;
// the inner cases interpret the integer flags produced by comparisons.
let render_status result =
    case result of {
        Ok x -> case x == 0 of {
            1 => putchar 48;
            _ -> putchar 49
        };
        Error code -> case code of {
            0 -> putchar 69;
            1 -> putchar 33;
            _ -> putchar 63
        }
    };

// Explicitly show typed versus opaque pointer stepping and pointer subtraction.
// For sample_bytes (u8*), +1 is one byte. For opaque_bytes (ptr), +1 uses the
// compiler's byte-addressed fallback. `opaque_bytes[index]` is a read.
let inspect_pointers index = {
    let typed_start = sample_bytes;
    let typed_next = typed_start + index;
    let typed_byte = typed_next[0];
    let opaque_start = opaque_bytes;
    let opaque_next = 1 + opaque_start;
    let opaque_byte = opaque_next[0];
    let typed_difference = typed_next - typed_start;
    let opaque_difference = opaque_next - opaque_start;
    normalize_byte (typed_byte + opaque_byte + typed_difference + opaque_difference)
};

// A pattern-only reference for tuple patterns. Since the expression grammar
// has no ordinary tuple literal, `pair_value` must come from a real compatible
// source/FFI binding in a project before this branch is used.
let unpack_pair pair_value =
    case pair_value of {
        (left, right) -> left + right;
        _ -> 0
    };

// Zero-argument functions use empty parameter parentheses and are called with
// the special `function()` syntax, not `function ()`.
let report_one_digit = fn () -> {
    let count = c_byte_count sample_bytes;
    let check = checksum_bytes sample_bytes count;
    let transformed = transform_twice check;
    let normalized = normalize_byte transformed;
    // ASCII '0' is 48; one decimal digit is enough for a minimal smoke test.
    putchar (48 + normalized % 10)
};

// The entrypoint name is recognized by the compiler's function discovery.
// Its body sequences work in a block and returns the last expression value.
let main = fn () -> {
    let _banner = "MAK syntax tour\n";
    let _float_demo = float_probe 1.25;
    let _negative_demo = negate 8;
    let _not_demo = logical_not 0;
    let sample_count = c_byte_count sample_bytes;
    let first = sample_bytes[0];
    let next_pointer = sample_bytes + 1;
    let second = next_pointer[0];
    let gap = next_pointer - sample_bytes;
    let raw_result = validate_byte (first + second + gap);
    let _arithmetic = arithmetic_probe first second;
    let _pointer_probe = inspect_pointers 1;
    let _pattern_probe = render_status raw_result;
    let _pair_demo = immediate_add;
    let _composition_demo = composed_result;
    let _lambda_demo = immediate_square;
    let _transform_demo = transform_twice sample_count;
    report_one_digit()
};
```

### 58.1 Companion C-side contract for the integration sample

The source above declares C symbols; it does not define them. The following is an illustrative C companion that defines the two global pointer variables and the helper function. Integrate it only after validating your compiler's emitted declarations against the target's ABI and build setup.

```c
#include <stdint.h>
#include <stddef.h>

static uint8_t sample_storage[] = { 3, 7, 11, 19, 23 };
uint8_t *sample_bytes = sample_storage;

static uint8_t opaque_storage[] = { 41, 43, 47, 53 };
const void *opaque_bytes = opaque_storage;

uint64_t c_byte_count(const void *p) {
    (void)p;
    return (uint64_t)sizeof sample_storage;
}
```

**Integration caveats:** `opaque_bytes` is declared from MAK as a global foreign `ptr`; the code-generation declaration must match the C object's actual pointer type. `c_byte_count` returns the sample storage length regardless of its input because it is just a compact example of a linked helper; in a real library the function contract should be explicit. The recursive checksum assumes its pointer is readable for the requested count. `unpack_pair` demonstrates grammar for tuple patterns, but requires a real compatible tuple-valued binding to be useful; because MAK has no tuple-expression literal, this helper is intentionally an isolated pattern example. Before calling a sample *production-grade*, add tests, error handling, bounds checks, build integration, and verified C headers. The sample is comprehensive as a syntax tour, not a substitute for those engineering steps.

## 59. Syntax-review checklist for code review

Use this checklist when reviewing a large MAK source file.

### File shape

- [ ] Every ordinary top-level declaration begins with `let`, `type`, or `bring`.
- [ ] Every ordinary top-level declaration ends with `;`.
- [ ] There is at most one bare final program-body expression.
- [ ] The final body expression has no trailing top-level semicolon.

### Names and tokens

- [ ] Identifiers use ASCII letters/underscore initially, then ASCII letters/digits/underscore.
- [ ] Keyword spelling and capitalization are correct.
- [ ] Constructor-pattern identifiers start uppercase; binding-pattern identifiers start lowercase where possible.
- [ ] Not-equal is `/=`, not `!=`.
- [ ] Dot is composition, not member access.

### Expressions

- [ ] Function arguments use juxtaposition, not commas.
- [ ] Parentheses are used for grouping, not assumed tuple-value construction.
- [ ] Inline lambdas are parenthesized before immediate application.
- [ ] Composition is parenthesized where application grouping might be ambiguous.
- [ ] Every index expression is nonempty and integer-like.
- [ ] Unary operators are only `-`, `!`, and `~`.
- [ ] There is no reliance on absent assignment, `if`, or loop syntax.

### Types and constructors

- [ ] Type applications use parenthesized forms such as `(List u8)`.
- [ ] Tuple types have at least two members; `()` is unit type.
- [ ] Array type notation `[T]` is not mistaken for an array literal.
- [ ] Function arrows are read right-associatively.
- [ ] Constructor declaration fields are types, not value parameters.

### Foreign interface and pointers

- [ ] Each external foreign type matches the real foreign declaration.
- [ ] `global` declarations match the C object type exactly.
- [ ] The pointer star depth matches the actual C pointer depth.
- [ ] Typed pointer arithmetic uses a pointer with correct pointee metadata.
- [ ] Opaque `ptr` offsets are treated as byte offsets.
- [ ] Every pointer's lifetime, bounds, alignment, nullability, and ownership are documented.
- [ ] No code assumes indexing syntax can assign to an element.
- [ ] Pointer difference units are known for the pointer type in question.

## 60. Final rule of interpretation

The safest way to understand any MAK fragment is to read it in this order:

1. Tokenize the characters and comments.
2. Apply the grammar for the current context (top-level declaration, expression, type, foreign type, or pattern).
3. Apply the parser's binding rules to determine grouping.
4. Apply name lookup, function arity resolution, representation inference, and type/pointer metadata analysis.
5. Inspect code generation and runtime/foreign contracts for observable behavior.

Do not jump from familiar-looking punctuation directly to familiar language semantics. MAK deliberately reuses characters such as `.`, `*`, `&`, `()`, and `[]` in a smaller or context-dependent grammar. The parser and code generator are the source of truth for what each form means.

---

# Appendix A — Exhaustive parser-grouping gallery

The gallery below is a syntax conformance catalogue, not a collection of type-checked programs. Its identifiers (`a`, `b`, `c`, `d`) are placeholders. Each example demonstrates how the parser groups a sequence of infix operators according to the current Pratt binding powers. Since each individual operator combination might be semantically ill-typed for arbitrary values, the guaranteed claim is the **syntactic grouping** only.

The gallery deliberately includes repeated operators, mixed precedence, comparisons, logical operators, and composition. For the most reliable reading, verify every expression's parenthesized parse tree rather than relying on the usual precedence table of another language.


## Appendix A.1 — Four-operand infix parser matrix
Every entry uses four atom placeholders separated by three infix tokens. The grouping is derived from the Pratt binding powers in the source parser. It is a syntax result, not a claim that the arbitrary values satisfy operator type requirements. Composition nodes are shown as `.` in the parenthesized tree to make their operator grouping visible; operationally, each dot is lowered to a unary composed function.
### A1.0001 — `a <= b - c / d`
- **Source:** `a <= b - c / d`
- **Grouping:** `(a <= (b - (c / d)))`

### A1.0002 — `a && b + c ^ d`
- **Source:** `a && b + c ^ d`
- **Grouping:** `(a && ((b + c) ^ d))`

### A1.0003 — `a % b > c == d`
- **Source:** `a % b > c == d`
- **Grouping:** `(((a % b) > c) == d)`

### A1.0004 — `a ^ b || c - d`
- **Source:** `a ^ b || c - d`
- **Grouping:** `((a ^ b) || (c - d))`

### A1.0005 — `a / b - c <= d`
- **Source:** `a / b - c <= d`
- **Grouping:** `(((a / b) - c) <= d)`

### A1.0006 — `a & b || c & d`
- **Source:** `a & b || c & d`
- **Grouping:** `((a & b) || (c & d))`

### A1.0007 — `a > b & c % d`
- **Source:** `a > b & c % d`
- **Grouping:** `(a > (b & (c % d)))`

### A1.0008 — `a | b & c . d`
- **Source:** `a | b & c . d`
- **Grouping:** `((a | b) & (c . d))`

### A1.0009 — `a . b > c + d`
- **Source:** `a . b > c + d`
- **Grouping:** `((a . b) > (c + d))`

### A1.0010 — `a < b & c || d`
- **Source:** `a < b & c || d`
- **Grouping:** `((a < (b & c)) || d)`

### A1.0011 — `a + b ^ c ^ d`
- **Source:** `a + b ^ c ^ d`
- **Grouping:** `(((a + b) ^ c) ^ d)`

### A1.0012 — `a && b || c >= d`
- **Source:** `a && b || c >= d`
- **Grouping:** `((a && b) || (c >= d))`

### A1.0013 — `a << b < c & d`
- **Source:** `a << b < c & d`
- **Grouping:** `((a << b) < (c & d))`

### A1.0014 — `a - b % c <= d`
- **Source:** `a - b % c <= d`
- **Grouping:** `((a - (b % c)) <= d)`

### A1.0015 — `a && b >= c <= d`
- **Source:** `a && b >= c <= d`
- **Grouping:** `(a && ((b >= c) <= d))`

### A1.0016 — `a % b >= c || d`
- **Source:** `a % b >= c || d`
- **Grouping:** `(((a % b) >= c) || d)`

### A1.0017 — `a / b ^ c . d`
- **Source:** `a / b ^ c . d`
- **Grouping:** `((a / b) ^ (c . d))`

### A1.0018 — `a <= b >> c >> d`
- **Source:** `a <= b >> c >> d`
- **Grouping:** `(a <= ((b >> c) >> d))`

### A1.0019 — `a + b | c <= d`
- **Source:** `a + b | c <= d`
- **Grouping:** `(((a + b) | c) <= d)`

### A1.0020 — `a && b % c % d`
- **Source:** `a && b % c % d`
- **Grouping:** `(a && ((b % c) % d))`

### A1.0021 — `a % b /= c / d`
- **Source:** `a % b /= c / d`
- **Grouping:** `((a % b) /= (c / d))`

### A1.0022 — `a ^ b && c && d`
- **Source:** `a ^ b && c && d`
- **Grouping:** `(((a ^ b) && c) && d)`

### A1.0023 — `a * b - c /= d`
- **Source:** `a * b - c /= d`
- **Grouping:** `(((a * b) - c) /= d)`

### A1.0024 — `a > b >= c <= d`
- **Source:** `a > b >= c <= d`
- **Grouping:** `(((a > b) >= c) <= d)`

### A1.0025 — `a /= b * c || d`
- **Source:** `a /= b * c || d`
- **Grouping:** `((a /= (b * c)) || d)`

### A1.0026 — `a << b >= c <= d`
- **Source:** `a << b >= c <= d`
- **Grouping:** `(((a << b) >= c) <= d)`

### A1.0027 — `a || b > c << d`
- **Source:** `a || b > c << d`
- **Grouping:** `(a || (b > (c << d)))`

### A1.0028 — `a >= b /= c + d`
- **Source:** `a >= b /= c + d`
- **Grouping:** `((a >= b) /= (c + d))`

### A1.0029 — `a + b && c << d`
- **Source:** `a + b && c << d`
- **Grouping:** `((a + b) && (c << d))`

### A1.0030 — `a - b ^ c | d`
- **Source:** `a - b ^ c | d`
- **Grouping:** `(((a - b) ^ c) | d)`

### A1.0031 — `a * b == c + d`
- **Source:** `a * b == c + d`
- **Grouping:** `((a * b) == (c + d))`

### A1.0032 — `a % b % c || d`
- **Source:** `a % b % c || d`
- **Grouping:** `(((a % b) % c) || d)`

### A1.0033 — `a & b / c && d`
- **Source:** `a & b / c && d`
- **Grouping:** `((a & (b / c)) && d)`

### A1.0034 — `a || b && c - d`
- **Source:** `a || b && c - d`
- **Grouping:** `(a || (b && (c - d)))`

### A1.0035 — `a | b | c && d`
- **Source:** `a | b | c && d`
- **Grouping:** `(((a | b) | c) && d)`

### A1.0036 — `a - b * c . d`
- **Source:** `a - b * c . d`
- **Grouping:** `(a - (b * (c . d)))`

### A1.0037 — `a << b % c || d`
- **Source:** `a << b % c || d`
- **Grouping:** `((a << (b % c)) || d)`

### A1.0038 — `a % b /= c /= d`
- **Source:** `a % b /= c /= d`
- **Grouping:** `(((a % b) /= c) /= d)`

### A1.0039 — `a ^ b && c >> d`
- **Source:** `a ^ b && c >> d`
- **Grouping:** `((a ^ b) && (c >> d))`

### A1.0040 — `a + b ^ c / d`
- **Source:** `a + b ^ c / d`
- **Grouping:** `((a + b) ^ (c / d))`

### A1.0041 — `a == b /= c <= d`
- **Source:** `a == b /= c <= d`
- **Grouping:** `(((a == b) /= c) <= d)`

### A1.0042 — `a . b == c >> d`
- **Source:** `a . b == c >> d`
- **Grouping:** `((a . b) == (c >> d))`

### A1.0043 — `a | b == c * d`
- **Source:** `a | b == c * d`
- **Grouping:** `((a | b) == (c * d))`

### A1.0044 — `a <= b >> c + d`
- **Source:** `a <= b >> c + d`
- **Grouping:** `(a <= (b >> (c + d)))`

### A1.0045 — `a ^ b >> c * d`
- **Source:** `a ^ b >> c * d`
- **Grouping:** `(a ^ (b >> (c * d)))`

### A1.0046 — `a || b | c && d`
- **Source:** `a || b | c && d`
- **Grouping:** `(a || ((b | c) && d))`

### A1.0047 — `a + b <= c << d`
- **Source:** `a + b <= c << d`
- **Grouping:** `((a + b) <= (c << d))`

### A1.0048 — `a | b . c > d`
- **Source:** `a | b . c > d`
- **Grouping:** `((a | (b . c)) > d)`

### A1.0049 — `a ^ b % c - d`
- **Source:** `a ^ b % c - d`
- **Grouping:** `(a ^ ((b % c) - d))`

### A1.0050 — `a & b || c | d`
- **Source:** `a & b || c | d`
- **Grouping:** `((a & b) || (c | d))`

### A1.0051 — `a <= b == c % d`
- **Source:** `a <= b == c % d`
- **Grouping:** `((a <= b) == (c % d))`

### A1.0052 — `a * b >= c | d`
- **Source:** `a * b >= c | d`
- **Grouping:** `((a * b) >= (c | d))`

### A1.0053 — `a >= b /= c || d`
- **Source:** `a >= b /= c || d`
- **Grouping:** `(((a >= b) /= c) || d)`

### A1.0054 — `a > b | c > d`
- **Source:** `a > b | c > d`
- **Grouping:** `((a > (b | c)) > d)`

### A1.0055 — `a - b ^ c < d`
- **Source:** `a - b ^ c < d`
- **Grouping:** `(((a - b) ^ c) < d)`

### A1.0056 — `a * b >= c /= d`
- **Source:** `a * b >= c /= d`
- **Grouping:** `(((a * b) >= c) /= d)`

### A1.0057 — `a && b ^ c <= d`
- **Source:** `a && b ^ c <= d`
- **Grouping:** `(a && ((b ^ c) <= d))`

### A1.0058 — `a % b | c || d`
- **Source:** `a % b | c || d`
- **Grouping:** `(((a % b) | c) || d)`

### A1.0059 — `a - b >= c == d`
- **Source:** `a - b >= c == d`
- **Grouping:** `(((a - b) >= c) == d)`

### A1.0060 — `a / b >= c * d`
- **Source:** `a / b >= c * d`
- **Grouping:** `((a / b) >= (c * d))`

### A1.0061 — `a >> b | c ^ d`
- **Source:** `a >> b | c ^ d`
- **Grouping:** `(((a >> b) | c) ^ d)`

### A1.0062 — `a < b > c >> d`
- **Source:** `a < b > c >> d`
- **Grouping:** `((a < b) > (c >> d))`

### A1.0063 — `a && b <= c || d`
- **Source:** `a && b <= c || d`
- **Grouping:** `((a && (b <= c)) || d)`

### A1.0064 — `a /= b > c < d`
- **Source:** `a /= b > c < d`
- **Grouping:** `(((a /= b) > c) < d)`

### A1.0065 — `a < b >= c >= d`
- **Source:** `a < b >= c >= d`
- **Grouping:** `(((a < b) >= c) >= d)`

### A1.0066 — `a & b << c % d`
- **Source:** `a & b << c % d`
- **Grouping:** `(a & (b << (c % d)))`

### A1.0067 — `a ^ b || c << d`
- **Source:** `a ^ b || c << d`
- **Grouping:** `((a ^ b) || (c << d))`

### A1.0068 — `a * b / c >= d`
- **Source:** `a * b / c >= d`
- **Grouping:** `(((a * b) / c) >= d)`

### A1.0069 — `a - b /= c * d`
- **Source:** `a - b /= c * d`
- **Grouping:** `((a - b) /= (c * d))`

### A1.0070 — `a < b * c <= d`
- **Source:** `a < b * c <= d`
- **Grouping:** `((a < (b * c)) <= d)`

### A1.0071 — `a + b && c | d`
- **Source:** `a + b && c | d`
- **Grouping:** `((a + b) && (c | d))`

### A1.0072 — `a < b ^ c && d`
- **Source:** `a < b ^ c && d`
- **Grouping:** `((a < (b ^ c)) && d)`

### A1.0073 — `a > b < c <= d`
- **Source:** `a > b < c <= d`
- **Grouping:** `(((a > b) < c) <= d)`

### A1.0074 — `a /= b - c && d`
- **Source:** `a /= b - c && d`
- **Grouping:** `((a /= (b - c)) && d)`

### A1.0075 — `a + b . c % d`
- **Source:** `a + b . c % d`
- **Grouping:** `(a + ((b . c) % d))`

### A1.0076 — `a + b & c % d`
- **Source:** `a + b & c % d`
- **Grouping:** `((a + b) & (c % d))`

### A1.0077 — `a || b == c % d`
- **Source:** `a || b == c % d`
- **Grouping:** `(a || (b == (c % d)))`

### A1.0078 — `a - b - c & d`
- **Source:** `a - b - c & d`
- **Grouping:** `(((a - b) - c) & d)`

### A1.0079 — `a < b - c >> d`
- **Source:** `a < b - c >> d`
- **Grouping:** `(a < ((b - c) >> d))`

### A1.0080 — `a > b || c >= d`
- **Source:** `a > b || c >= d`
- **Grouping:** `((a > b) || (c >= d))`

### A1.0081 — `a % b < c % d`
- **Source:** `a % b < c % d`
- **Grouping:** `((a % b) < (c % d))`

### A1.0082 — `a | b <= c & d`
- **Source:** `a | b <= c & d`
- **Grouping:** `((a | b) <= (c & d))`

### A1.0083 — `a | b ^ c ^ d`
- **Source:** `a | b ^ c ^ d`
- **Grouping:** `(((a | b) ^ c) ^ d)`

### A1.0084 — `a >> b || c | d`
- **Source:** `a >> b || c | d`
- **Grouping:** `((a >> b) || (c | d))`

### A1.0085 — `a & b & c / d`
- **Source:** `a & b & c / d`
- **Grouping:** `((a & b) & (c / d))`

### A1.0086 — `a & b ^ c >> d`
- **Source:** `a & b ^ c >> d`
- **Grouping:** `((a & b) ^ (c >> d))`

### A1.0087 — `a == b << c & d`
- **Source:** `a == b << c & d`
- **Grouping:** `(a == ((b << c) & d))`

### A1.0088 — `a >> b . c + d`
- **Source:** `a >> b . c + d`
- **Grouping:** `(a >> ((b . c) + d))`

### A1.0089 — `a + b == c * d`
- **Source:** `a + b == c * d`
- **Grouping:** `((a + b) == (c * d))`

### A1.0090 — `a ^ b * c > d`
- **Source:** `a ^ b * c > d`
- **Grouping:** `((a ^ (b * c)) > d)`

### A1.0091 — `a - b > c / d`
- **Source:** `a - b > c / d`
- **Grouping:** `((a - b) > (c / d))`

### A1.0092 — `a . b & c > d`
- **Source:** `a . b & c > d`
- **Grouping:** `(((a . b) & c) > d)`

### A1.0093 — `a && b ^ c == d`
- **Source:** `a && b ^ c == d`
- **Grouping:** `(a && ((b ^ c) == d))`

### A1.0094 — `a < b /= c / d`
- **Source:** `a < b /= c / d`
- **Grouping:** `((a < b) /= (c / d))`

### A1.0095 — `a <= b >= c || d`
- **Source:** `a <= b >= c || d`
- **Grouping:** `(((a <= b) >= c) || d)`

### A1.0096 — `a % b | c <= d`
- **Source:** `a % b | c <= d`
- **Grouping:** `(((a % b) | c) <= d)`

### A1.0097 — `a | b == c > d`
- **Source:** `a | b == c > d`
- **Grouping:** `(((a | b) == c) > d)`

### A1.0098 — `a / b | c <= d`
- **Source:** `a / b | c <= d`
- **Grouping:** `(((a / b) | c) <= d)`

### A1.0099 — `a % b - c % d`
- **Source:** `a % b - c % d`
- **Grouping:** `((a % b) - (c % d))`

### A1.0100 — `a >= b . c ^ d`
- **Source:** `a >= b . c ^ d`
- **Grouping:** `(a >= ((b . c) ^ d))`

### A1.0101 — `a << b == c ^ d`
- **Source:** `a << b == c ^ d`
- **Grouping:** `((a << b) == (c ^ d))`

### A1.0102 — `a * b ^ c - d`
- **Source:** `a * b ^ c - d`
- **Grouping:** `((a * b) ^ (c - d))`

### A1.0103 — `a <= b >> c /= d`
- **Source:** `a <= b >> c /= d`
- **Grouping:** `((a <= (b >> c)) /= d)`

### A1.0104 — `a | b % c <= d`
- **Source:** `a | b % c <= d`
- **Grouping:** `((a | (b % c)) <= d)`

### A1.0105 — `a > b >= c >> d`
- **Source:** `a > b >= c >> d`
- **Grouping:** `((a > b) >= (c >> d))`

### A1.0106 — `a > b - c % d`
- **Source:** `a > b - c % d`
- **Grouping:** `(a > (b - (c % d)))`

### A1.0107 — `a * b < c == d`
- **Source:** `a * b < c == d`
- **Grouping:** `(((a * b) < c) == d)`

### A1.0108 — `a / b * c + d`
- **Source:** `a / b * c + d`
- **Grouping:** `(((a / b) * c) + d)`

### A1.0109 — `a . b | c * d`
- **Source:** `a . b | c * d`
- **Grouping:** `((a . b) | (c * d))`

### A1.0110 — `a - b > c || d`
- **Source:** `a - b > c || d`
- **Grouping:** `(((a - b) > c) || d)`

### A1.0111 — `a << b || c * d`
- **Source:** `a << b || c * d`
- **Grouping:** `((a << b) || (c * d))`

### A1.0112 — `a /= b < c == d`
- **Source:** `a /= b < c == d`
- **Grouping:** `(((a /= b) < c) == d)`

### A1.0113 — `a || b % c >> d`
- **Source:** `a || b % c >> d`
- **Grouping:** `(a || ((b % c) >> d))`

### A1.0114 — `a & b * c | d`
- **Source:** `a & b * c | d`
- **Grouping:** `((a & (b * c)) | d)`

### A1.0115 — `a > b / c /= d`
- **Source:** `a > b / c /= d`
- **Grouping:** `((a > (b / c)) /= d)`

### A1.0116 — `a * b . c | d`
- **Source:** `a * b . c | d`
- **Grouping:** `((a * (b . c)) | d)`

### A1.0117 — `a + b + c && d`
- **Source:** `a + b + c && d`
- **Grouping:** `(((a + b) + c) && d)`

### A1.0118 — `a << b <= c <= d`
- **Source:** `a << b <= c <= d`
- **Grouping:** `(((a << b) <= c) <= d)`

### A1.0119 — `a . b & c << d`
- **Source:** `a . b & c << d`
- **Grouping:** `((a . b) & (c << d))`

### A1.0120 — `a * b + c / d`
- **Source:** `a * b + c / d`
- **Grouping:** `((a * b) + (c / d))`

### A1.0121 — `a >= b > c ^ d`
- **Source:** `a >= b > c ^ d`
- **Grouping:** `((a >= b) > (c ^ d))`

### A1.0122 — `a > b ^ c >> d`
- **Source:** `a > b ^ c >> d`
- **Grouping:** `(a > (b ^ (c >> d)))`

### A1.0123 — `a >> b <= c ^ d`
- **Source:** `a >> b <= c ^ d`
- **Grouping:** `((a >> b) <= (c ^ d))`

### A1.0124 — `a % b - c /= d`
- **Source:** `a % b - c /= d`
- **Grouping:** `(((a % b) - c) /= d)`

### A1.0125 — `a . b * c * d`
- **Source:** `a . b * c * d`
- **Grouping:** `(((a . b) * c) * d)`

### A1.0126 — `a * b > c / d`
- **Source:** `a * b > c / d`
- **Grouping:** `((a * b) > (c / d))`

### A1.0127 — `a > b % c >> d`
- **Source:** `a > b % c >> d`
- **Grouping:** `(a > ((b % c) >> d))`

### A1.0128 — `a >= b && c % d`
- **Source:** `a >= b && c % d`
- **Grouping:** `((a >= b) && (c % d))`

### A1.0129 — `a + b <= c <= d`
- **Source:** `a + b <= c <= d`
- **Grouping:** `(((a + b) <= c) <= d)`

### A1.0130 — `a + b >= c * d`
- **Source:** `a + b >= c * d`
- **Grouping:** `((a + b) >= (c * d))`

### A1.0131 — `a > b == c / d`
- **Source:** `a > b == c / d`
- **Grouping:** `((a > b) == (c / d))`

### A1.0132 — `a - b | c / d`
- **Source:** `a - b | c / d`
- **Grouping:** `((a - b) | (c / d))`

### A1.0133 — `a - b && c /= d`
- **Source:** `a - b && c /= d`
- **Grouping:** `((a - b) && (c /= d))`

### A1.0134 — `a << b < c /= d`
- **Source:** `a << b < c /= d`
- **Grouping:** `(((a << b) < c) /= d)`

### A1.0135 — `a | b < c <= d`
- **Source:** `a | b < c <= d`
- **Grouping:** `(((a | b) < c) <= d)`

### A1.0136 — `a >= b <= c < d`
- **Source:** `a >= b <= c < d`
- **Grouping:** `(((a >= b) <= c) < d)`

### A1.0137 — `a == b <= c || d`
- **Source:** `a == b <= c || d`
- **Grouping:** `(((a == b) <= c) || d)`

### A1.0138 — `a == b & c > d`
- **Source:** `a == b & c > d`
- **Grouping:** `((a == (b & c)) > d)`

### A1.0139 — `a <= b == c | d`
- **Source:** `a <= b == c | d`
- **Grouping:** `((a <= b) == (c | d))`

### A1.0140 — `a /= b . c || d`
- **Source:** `a /= b . c || d`
- **Grouping:** `((a /= (b . c)) || d)`

### A1.0141 — `a + b > c && d`
- **Source:** `a + b > c && d`
- **Grouping:** `(((a + b) > c) && d)`

### A1.0142 — `a || b <= c && d`
- **Source:** `a || b <= c && d`
- **Grouping:** `(a || ((b <= c) && d))`

### A1.0143 — `a >> b > c > d`
- **Source:** `a >> b > c > d`
- **Grouping:** `(((a >> b) > c) > d)`

### A1.0144 — `a & b << c | d`
- **Source:** `a & b << c | d`
- **Grouping:** `((a & (b << c)) | d)`

### A1.0145 — `a && b << c % d`
- **Source:** `a && b << c % d`
- **Grouping:** `(a && (b << (c % d)))`

### A1.0146 — `a && b / c || d`
- **Source:** `a && b / c || d`
- **Grouping:** `((a && (b / c)) || d)`

### A1.0147 — `a ^ b + c <= d`
- **Source:** `a ^ b + c <= d`
- **Grouping:** `((a ^ (b + c)) <= d)`

### A1.0148 — `a > b <= c & d`
- **Source:** `a > b <= c & d`
- **Grouping:** `((a > b) <= (c & d))`

### A1.0149 — `a | b | c . d`
- **Source:** `a | b | c . d`
- **Grouping:** `((a | b) | (c . d))`

### A1.0150 — `a << b % c > d`
- **Source:** `a << b % c > d`
- **Grouping:** `((a << (b % c)) > d)`

### A1.0151 — `a + b || c * d`
- **Source:** `a + b || c * d`
- **Grouping:** `((a + b) || (c * d))`

### A1.0152 — `a <= b < c - d`
- **Source:** `a <= b < c - d`
- **Grouping:** `((a <= b) < (c - d))`

### A1.0153 — `a ^ b >> c /= d`
- **Source:** `a ^ b >> c /= d`
- **Grouping:** `((a ^ (b >> c)) /= d)`

### A1.0154 — `a | b << c || d`
- **Source:** `a | b << c || d`
- **Grouping:** `((a | (b << c)) || d)`

### A1.0155 — `a + b /= c + d`
- **Source:** `a + b /= c + d`
- **Grouping:** `((a + b) /= (c + d))`

### A1.0156 — `a >= b ^ c | d`
- **Source:** `a >= b ^ c | d`
- **Grouping:** `(a >= ((b ^ c) | d))`

### A1.0157 — `a << b && c <= d`
- **Source:** `a << b && c <= d`
- **Grouping:** `((a << b) && (c <= d))`

### A1.0158 — `a == b - c >= d`
- **Source:** `a == b - c >= d`
- **Grouping:** `((a == (b - c)) >= d)`

### A1.0159 — `a /= b << c || d`
- **Source:** `a /= b << c || d`
- **Grouping:** `((a /= (b << c)) || d)`

### A1.0160 — `a + b | c * d`
- **Source:** `a + b | c * d`
- **Grouping:** `((a + b) | (c * d))`

### A1.0161 — `a <= b . c > d`
- **Source:** `a <= b . c > d`
- **Grouping:** `((a <= (b . c)) > d)`

### A1.0162 — `a <= b / c /= d`
- **Source:** `a <= b / c /= d`
- **Grouping:** `((a <= (b / c)) /= d)`

### A1.0163 — `a == b ^ c << d`
- **Source:** `a == b ^ c << d`
- **Grouping:** `(a == (b ^ (c << d)))`

### A1.0164 — `a < b | c ^ d`
- **Source:** `a < b | c ^ d`
- **Grouping:** `(a < ((b | c) ^ d))`

### A1.0165 — `a > b < c % d`
- **Source:** `a > b < c % d`
- **Grouping:** `((a > b) < (c % d))`

### A1.0166 — `a || b || c + d`
- **Source:** `a || b || c + d`
- **Grouping:** `((a || b) || (c + d))`

### A1.0167 — `a * b & c <= d`
- **Source:** `a * b & c <= d`
- **Grouping:** `(((a * b) & c) <= d)`

### A1.0168 — `a < b < c | d`
- **Source:** `a < b < c | d`
- **Grouping:** `((a < b) < (c | d))`

### A1.0169 — `a || b <= c / d`
- **Source:** `a || b <= c / d`
- **Grouping:** `(a || (b <= (c / d)))`

### A1.0170 — `a && b % c <= d`
- **Source:** `a && b % c <= d`
- **Grouping:** `(a && ((b % c) <= d))`

### A1.0171 — `a % b || c << d`
- **Source:** `a % b || c << d`
- **Grouping:** `((a % b) || (c << d))`

### A1.0172 — `a / b && c ^ d`
- **Source:** `a / b && c ^ d`
- **Grouping:** `((a / b) && (c ^ d))`

### A1.0173 — `a / b * c < d`
- **Source:** `a / b * c < d`
- **Grouping:** `(((a / b) * c) < d)`

### A1.0174 — `a >> b . c >> d`
- **Source:** `a >> b . c >> d`
- **Grouping:** `((a >> (b . c)) >> d)`

### A1.0175 — `a >> b % c /= d`
- **Source:** `a >> b % c /= d`
- **Grouping:** `((a >> (b % c)) /= d)`

### A1.0176 — `a < b && c + d`
- **Source:** `a < b && c + d`
- **Grouping:** `((a < b) && (c + d))`

### A1.0177 — `a <= b < c && d`
- **Source:** `a <= b < c && d`
- **Grouping:** `(((a <= b) < c) && d)`

### A1.0178 — `a && b && c << d`
- **Source:** `a && b && c << d`
- **Grouping:** `((a && b) && (c << d))`

### A1.0179 — `a & b > c > d`
- **Source:** `a & b > c > d`
- **Grouping:** `(((a & b) > c) > d)`

### A1.0180 — `a % b <= c <= d`
- **Source:** `a % b <= c <= d`
- **Grouping:** `(((a % b) <= c) <= d)`

### A1.0181 — `a / b % c - d`
- **Source:** `a / b % c - d`
- **Grouping:** `(((a / b) % c) - d)`

### A1.0182 — `a * b /= c && d`
- **Source:** `a * b /= c && d`
- **Grouping:** `(((a * b) /= c) && d)`

### A1.0183 — `a < b > c % d`
- **Source:** `a < b > c % d`
- **Grouping:** `((a < b) > (c % d))`

### A1.0184 — `a < b | c <= d`
- **Source:** `a < b | c <= d`
- **Grouping:** `((a < (b | c)) <= d)`

### A1.0185 — `a <= b == c ^ d`
- **Source:** `a <= b == c ^ d`
- **Grouping:** `((a <= b) == (c ^ d))`

### A1.0186 — `a * b >> c > d`
- **Source:** `a * b >> c > d`
- **Grouping:** `(((a * b) >> c) > d)`

### A1.0187 — `a . b | c | d`
- **Source:** `a . b | c | d`
- **Grouping:** `(((a . b) | c) | d)`

### A1.0188 — `a && b > c & d`
- **Source:** `a && b > c & d`
- **Grouping:** `(a && (b > (c & d)))`

### A1.0189 — `a - b == c | d`
- **Source:** `a - b == c | d`
- **Grouping:** `((a - b) == (c | d))`

### A1.0190 — `a && b < c - d`
- **Source:** `a && b < c - d`
- **Grouping:** `(a && (b < (c - d)))`

### A1.0191 — `a & b * c * d`
- **Source:** `a & b * c * d`
- **Grouping:** `(a & ((b * c) * d))`

### A1.0192 — `a << b == c >> d`
- **Source:** `a << b == c >> d`
- **Grouping:** `((a << b) == (c >> d))`

### A1.0193 — `a >> b >> c + d`
- **Source:** `a >> b >> c + d`
- **Grouping:** `((a >> b) >> (c + d))`

### A1.0194 — `a < b && c > d`
- **Source:** `a < b && c > d`
- **Grouping:** `((a < b) && (c > d))`

### A1.0195 — `a * b . c * d`
- **Source:** `a * b . c * d`
- **Grouping:** `((a * (b . c)) * d)`

### A1.0196 — `a > b || c << d`
- **Source:** `a > b || c << d`
- **Grouping:** `((a > b) || (c << d))`

### A1.0197 — `a >= b / c > d`
- **Source:** `a >= b / c > d`
- **Grouping:** `((a >= (b / c)) > d)`

### A1.0198 — `a /= b . c > d`
- **Source:** `a /= b . c > d`
- **Grouping:** `((a /= (b . c)) > d)`

### A1.0199 — `a + b * c << d`
- **Source:** `a + b * c << d`
- **Grouping:** `((a + (b * c)) << d)`

### A1.0200 — `a > b || c >> d`
- **Source:** `a > b || c >> d`
- **Grouping:** `((a > b) || (c >> d))`

### A1.0201 — `a >> b * c / d`
- **Source:** `a >> b * c / d`
- **Grouping:** `(a >> ((b * c) / d))`

### A1.0202 — `a >> b == c == d`
- **Source:** `a >> b == c == d`
- **Grouping:** `(((a >> b) == c) == d)`

### A1.0203 — `a && b == c && d`
- **Source:** `a && b == c && d`
- **Grouping:** `((a && (b == c)) && d)`

### A1.0204 — `a - b || c * d`
- **Source:** `a - b || c * d`
- **Grouping:** `((a - b) || (c * d))`

### A1.0205 — `a >> b * c <= d`
- **Source:** `a >> b * c <= d`
- **Grouping:** `((a >> (b * c)) <= d)`

### A1.0206 — `a << b . c * d`
- **Source:** `a << b . c * d`
- **Grouping:** `(a << ((b . c) * d))`

### A1.0207 — `a & b | c || d`
- **Source:** `a & b | c || d`
- **Grouping:** `(((a & b) | c) || d)`

### A1.0208 — `a && b ^ c / d`
- **Source:** `a && b ^ c / d`
- **Grouping:** `(a && (b ^ (c / d)))`

### A1.0209 — `a & b + c == d`
- **Source:** `a & b + c == d`
- **Grouping:** `((a & (b + c)) == d)`

### A1.0210 — `a & b && c / d`
- **Source:** `a & b && c / d`
- **Grouping:** `((a & b) && (c / d))`

### A1.0211 — `a >= b < c + d`
- **Source:** `a >= b < c + d`
- **Grouping:** `((a >= b) < (c + d))`

### A1.0212 — `a . b ^ c >= d`
- **Source:** `a . b ^ c >= d`
- **Grouping:** `(((a . b) ^ c) >= d)`

### A1.0213 — `a < b - c /= d`
- **Source:** `a < b - c /= d`
- **Grouping:** `((a < (b - c)) /= d)`

### A1.0214 — `a > b && c /= d`
- **Source:** `a > b && c /= d`
- **Grouping:** `((a > b) && (c /= d))`

### A1.0215 — `a >= b | c > d`
- **Source:** `a >= b | c > d`
- **Grouping:** `((a >= (b | c)) > d)`

### A1.0216 — `a /= b /= c || d`
- **Source:** `a /= b /= c || d`
- **Grouping:** `(((a /= b) /= c) || d)`

### A1.0217 — `a | b || c || d`
- **Source:** `a | b || c || d`
- **Grouping:** `(((a | b) || c) || d)`

### A1.0218 — `a && b . c * d`
- **Source:** `a && b . c * d`
- **Grouping:** `(a && ((b . c) * d))`

### A1.0219 — `a - b | c + d`
- **Source:** `a - b | c + d`
- **Grouping:** `((a - b) | (c + d))`

### A1.0220 — `a == b && c & d`
- **Source:** `a == b && c & d`
- **Grouping:** `((a == b) && (c & d))`

### A1.0221 — `a >= b <= c > d`
- **Source:** `a >= b <= c > d`
- **Grouping:** `(((a >= b) <= c) > d)`

### A1.0222 — `a & b ^ c * d`
- **Source:** `a & b ^ c * d`
- **Grouping:** `((a & b) ^ (c * d))`

### A1.0223 — `a | b && c << d`
- **Source:** `a | b && c << d`
- **Grouping:** `((a | b) && (c << d))`

### A1.0224 — `a + b /= c ^ d`
- **Source:** `a + b /= c ^ d`
- **Grouping:** `((a + b) /= (c ^ d))`

### A1.0225 — `a && b | c - d`
- **Source:** `a && b | c - d`
- **Grouping:** `(a && (b | (c - d)))`

### A1.0226 — `a && b <= c + d`
- **Source:** `a && b <= c + d`
- **Grouping:** `(a && (b <= (c + d)))`

### A1.0227 — `a / b << c <= d`
- **Source:** `a / b << c <= d`
- **Grouping:** `(((a / b) << c) <= d)`

### A1.0228 — `a || b * c . d`
- **Source:** `a || b * c . d`
- **Grouping:** `(a || (b * (c . d)))`

### A1.0229 — `a & b - c || d`
- **Source:** `a & b - c || d`
- **Grouping:** `((a & (b - c)) || d)`

### A1.0230 — `a && b <= c * d`
- **Source:** `a && b <= c * d`
- **Grouping:** `(a && (b <= (c * d)))`

### A1.0231 — `a | b >> c <= d`
- **Source:** `a | b >> c <= d`
- **Grouping:** `((a | (b >> c)) <= d)`

### A1.0232 — `a - b / c * d`
- **Source:** `a - b / c * d`
- **Grouping:** `(a - ((b / c) * d))`

### A1.0233 — `a / b || c >> d`
- **Source:** `a / b || c >> d`
- **Grouping:** `((a / b) || (c >> d))`

### A1.0234 — `a < b + c % d`
- **Source:** `a < b + c % d`
- **Grouping:** `(a < (b + (c % d)))`

### A1.0235 — `a & b || c - d`
- **Source:** `a & b || c - d`
- **Grouping:** `((a & b) || (c - d))`

### A1.0236 — `a /= b * c < d`
- **Source:** `a /= b * c < d`
- **Grouping:** `((a /= (b * c)) < d)`

### A1.0237 — `a | b < c >= d`
- **Source:** `a | b < c >= d`
- **Grouping:** `(((a | b) < c) >= d)`

### A1.0238 — `a | b / c /= d`
- **Source:** `a | b / c /= d`
- **Grouping:** `((a | (b / c)) /= d)`

### A1.0239 — `a >= b > c && d`
- **Source:** `a >= b > c && d`
- **Grouping:** `(((a >= b) > c) && d)`

### A1.0240 — `a >> b || c % d`
- **Source:** `a >> b || c % d`
- **Grouping:** `((a >> b) || (c % d))`

### A1.0241 — `a . b /= c - d`
- **Source:** `a . b /= c - d`
- **Grouping:** `((a . b) /= (c - d))`

### A1.0242 — `a ^ b . c + d`
- **Source:** `a ^ b . c + d`
- **Grouping:** `(a ^ ((b . c) + d))`

### A1.0243 — `a >= b && c | d`
- **Source:** `a >= b && c | d`
- **Grouping:** `((a >= b) && (c | d))`

### A1.0244 — `a /= b == c | d`
- **Source:** `a /= b == c | d`
- **Grouping:** `((a /= b) == (c | d))`

### A1.0245 — `a || b . c % d`
- **Source:** `a || b . c % d`
- **Grouping:** `(a || ((b . c) % d))`

### A1.0246 — `a < b <= c >> d`
- **Source:** `a < b <= c >> d`
- **Grouping:** `((a < b) <= (c >> d))`

### A1.0247 — `a >= b /= c <= d`
- **Source:** `a >= b /= c <= d`
- **Grouping:** `(((a >= b) /= c) <= d)`

### A1.0248 — `a <= b <= c ^ d`
- **Source:** `a <= b <= c ^ d`
- **Grouping:** `((a <= b) <= (c ^ d))`

### A1.0249 — `a || b > c >> d`
- **Source:** `a || b > c >> d`
- **Grouping:** `(a || (b > (c >> d)))`

### A1.0250 — `a == b - c >> d`
- **Source:** `a == b - c >> d`
- **Grouping:** `(a == ((b - c) >> d))`

### A1.0251 — `a <= b ^ c /= d`
- **Source:** `a <= b ^ c /= d`
- **Grouping:** `((a <= (b ^ c)) /= d)`

### A1.0252 — `a | b - c + d`
- **Source:** `a | b - c + d`
- **Grouping:** `(a | ((b - c) + d))`

### A1.0253 — `a / b || c ^ d`
- **Source:** `a / b || c ^ d`
- **Grouping:** `((a / b) || (c ^ d))`

### A1.0254 — `a /= b >> c ^ d`
- **Source:** `a /= b >> c ^ d`
- **Grouping:** `(a /= ((b >> c) ^ d))`

### A1.0255 — `a < b % c << d`
- **Source:** `a < b % c << d`
- **Grouping:** `(a < ((b % c) << d))`

### A1.0256 — `a >= b || c / d`
- **Source:** `a >= b || c / d`
- **Grouping:** `((a >= b) || (c / d))`

### A1.0257 — `a + b ^ c /= d`
- **Source:** `a + b ^ c /= d`
- **Grouping:** `(((a + b) ^ c) /= d)`

### A1.0258 — `a /= b - c /= d`
- **Source:** `a /= b - c /= d`
- **Grouping:** `((a /= (b - c)) /= d)`

### A1.0259 — `a * b < c ^ d`
- **Source:** `a * b < c ^ d`
- **Grouping:** `((a * b) < (c ^ d))`

### A1.0260 — `a >= b <= c | d`
- **Source:** `a >= b <= c | d`
- **Grouping:** `((a >= b) <= (c | d))`

### A1.0261 — `a ^ b /= c <= d`
- **Source:** `a ^ b /= c <= d`
- **Grouping:** `(((a ^ b) /= c) <= d)`

### A1.0262 — `a /= b % c % d`
- **Source:** `a /= b % c % d`
- **Grouping:** `(a /= ((b % c) % d))`

### A1.0263 — `a ^ b > c << d`
- **Source:** `a ^ b > c << d`
- **Grouping:** `((a ^ b) > (c << d))`

### A1.0264 — `a <= b <= c << d`
- **Source:** `a <= b <= c << d`
- **Grouping:** `((a <= b) <= (c << d))`

### A1.0265 — `a >> b == c || d`
- **Source:** `a >> b == c || d`
- **Grouping:** `(((a >> b) == c) || d)`

### A1.0266 — `a - b && c % d`
- **Source:** `a - b && c % d`
- **Grouping:** `((a - b) && (c % d))`

### A1.0267 — `a > b ^ c >= d`
- **Source:** `a > b ^ c >= d`
- **Grouping:** `((a > (b ^ c)) >= d)`

### A1.0268 — `a < b / c < d`
- **Source:** `a < b / c < d`
- **Grouping:** `((a < (b / c)) < d)`

### A1.0269 — `a / b % c < d`
- **Source:** `a / b % c < d`
- **Grouping:** `(((a / b) % c) < d)`

### A1.0270 — `a > b < c == d`
- **Source:** `a > b < c == d`
- **Grouping:** `(((a > b) < c) == d)`

### A1.0271 — `a == b | c || d`
- **Source:** `a == b | c || d`
- **Grouping:** `((a == (b | c)) || d)`

### A1.0272 — `a << b / c > d`
- **Source:** `a << b / c > d`
- **Grouping:** `((a << (b / c)) > d)`

### A1.0273 — `a && b | c ^ d`
- **Source:** `a && b | c ^ d`
- **Grouping:** `(a && ((b | c) ^ d))`

### A1.0274 — `a * b + c <= d`
- **Source:** `a * b + c <= d`
- **Grouping:** `(((a * b) + c) <= d)`

### A1.0275 — `a - b / c /= d`
- **Source:** `a - b / c /= d`
- **Grouping:** `((a - (b / c)) /= d)`

### A1.0276 — `a && b < c == d`
- **Source:** `a && b < c == d`
- **Grouping:** `(a && ((b < c) == d))`

### A1.0277 — `a - b - c /= d`
- **Source:** `a - b - c /= d`
- **Grouping:** `(((a - b) - c) /= d)`

### A1.0278 — `a - b * c == d`
- **Source:** `a - b * c == d`
- **Grouping:** `((a - (b * c)) == d)`

### A1.0279 — `a | b > c % d`
- **Source:** `a | b > c % d`
- **Grouping:** `((a | b) > (c % d))`

### A1.0280 — `a <= b << c * d`
- **Source:** `a <= b << c * d`
- **Grouping:** `(a <= (b << (c * d)))`

### A1.0281 — `a - b < c + d`
- **Source:** `a - b < c + d`
- **Grouping:** `((a - b) < (c + d))`

### A1.0282 — `a <= b >= c <= d`
- **Source:** `a <= b >= c <= d`
- **Grouping:** `(((a <= b) >= c) <= d)`

### A1.0283 — `a || b | c & d`
- **Source:** `a || b | c & d`
- **Grouping:** `(a || ((b | c) & d))`

### A1.0284 — `a == b >= c % d`
- **Source:** `a == b >= c % d`
- **Grouping:** `((a == b) >= (c % d))`

### A1.0285 — `a <= b ^ c & d`
- **Source:** `a <= b ^ c & d`
- **Grouping:** `(a <= ((b ^ c) & d))`

### A1.0286 — `a * b || c | d`
- **Source:** `a * b || c | d`
- **Grouping:** `((a * b) || (c | d))`

### A1.0287 — `a /= b - c ^ d`
- **Source:** `a /= b - c ^ d`
- **Grouping:** `(a /= ((b - c) ^ d))`

### A1.0288 — `a <= b + c % d`
- **Source:** `a <= b + c % d`
- **Grouping:** `(a <= (b + (c % d)))`

### A1.0289 — `a > b << c && d`
- **Source:** `a > b << c && d`
- **Grouping:** `((a > (b << c)) && d)`

### A1.0290 — `a || b << c - d`
- **Source:** `a || b << c - d`
- **Grouping:** `(a || (b << (c - d)))`

### A1.0291 — `a % b /= c && d`
- **Source:** `a % b /= c && d`
- **Grouping:** `(((a % b) /= c) && d)`

### A1.0292 — `a == b >= c >> d`
- **Source:** `a == b >= c >> d`
- **Grouping:** `((a == b) >= (c >> d))`

### A1.0293 — `a >> b - c /= d`
- **Source:** `a >> b - c /= d`
- **Grouping:** `((a >> (b - c)) /= d)`

### A1.0294 — `a | b + c && d`
- **Source:** `a | b + c && d`
- **Grouping:** `((a | (b + c)) && d)`

### A1.0295 — `a == b | c >= d`
- **Source:** `a == b | c >= d`
- **Grouping:** `((a == (b | c)) >= d)`

### A1.0296 — `a < b << c - d`
- **Source:** `a < b << c - d`
- **Grouping:** `(a < (b << (c - d)))`

### A1.0297 — `a >> b % c / d`
- **Source:** `a >> b % c / d`
- **Grouping:** `(a >> ((b % c) / d))`

### A1.0298 — `a == b . c == d`
- **Source:** `a == b . c == d`
- **Grouping:** `((a == (b . c)) == d)`

### A1.0299 — `a << b < c == d`
- **Source:** `a << b < c == d`
- **Grouping:** `(((a << b) < c) == d)`

### A1.0300 — `a + b << c % d`
- **Source:** `a + b << c % d`
- **Grouping:** `((a + b) << (c % d))`

### A1.0301 — `a ^ b >> c + d`
- **Source:** `a ^ b >> c + d`
- **Grouping:** `(a ^ (b >> (c + d)))`

### A1.0302 — `a << b == c == d`
- **Source:** `a << b == c == d`
- **Grouping:** `(((a << b) == c) == d)`

### A1.0303 — `a - b ^ c . d`
- **Source:** `a - b ^ c . d`
- **Grouping:** `((a - b) ^ (c . d))`

### A1.0304 — `a == b - c > d`
- **Source:** `a == b - c > d`
- **Grouping:** `((a == (b - c)) > d)`

### A1.0305 — `a < b ^ c - d`
- **Source:** `a < b ^ c - d`
- **Grouping:** `(a < (b ^ (c - d)))`

### A1.0306 — `a >= b < c % d`
- **Source:** `a >= b < c % d`
- **Grouping:** `((a >= b) < (c % d))`

### A1.0307 — `a /= b & c + d`
- **Source:** `a /= b & c + d`
- **Grouping:** `(a /= (b & (c + d)))`

### A1.0308 — `a << b <= c * d`
- **Source:** `a << b <= c * d`
- **Grouping:** `((a << b) <= (c * d))`

### A1.0309 — `a == b || c == d`
- **Source:** `a == b || c == d`
- **Grouping:** `((a == b) || (c == d))`

### A1.0310 — `a & b >> c + d`
- **Source:** `a & b >> c + d`
- **Grouping:** `(a & (b >> (c + d)))`

### A1.0311 — `a < b >= c % d`
- **Source:** `a < b >= c % d`
- **Grouping:** `((a < b) >= (c % d))`

### A1.0312 — `a >= b / c % d`
- **Source:** `a >= b / c % d`
- **Grouping:** `(a >= ((b / c) % d))`

### A1.0313 — `a << b % c * d`
- **Source:** `a << b % c * d`
- **Grouping:** `(a << ((b % c) * d))`

### A1.0314 — `a / b << c . d`
- **Source:** `a / b << c . d`
- **Grouping:** `((a / b) << (c . d))`

### A1.0315 — `a * b << c - d`
- **Source:** `a * b << c - d`
- **Grouping:** `((a * b) << (c - d))`

### A1.0316 — `a - b ^ c || d`
- **Source:** `a - b ^ c || d`
- **Grouping:** `(((a - b) ^ c) || d)`

### A1.0317 — `a * b & c /= d`
- **Source:** `a * b & c /= d`
- **Grouping:** `(((a * b) & c) /= d)`

### A1.0318 — `a >= b . c < d`
- **Source:** `a >= b . c < d`
- **Grouping:** `((a >= (b . c)) < d)`

### A1.0319 — `a && b > c . d`
- **Source:** `a && b > c . d`
- **Grouping:** `(a && (b > (c . d)))`

### A1.0320 — `a . b >= c == d`
- **Source:** `a . b >= c == d`
- **Grouping:** `(((a . b) >= c) == d)`

### A1.0321 — `a > b % c >= d`
- **Source:** `a > b % c >= d`
- **Grouping:** `((a > (b % c)) >= d)`

### A1.0322 — `a >= b /= c /= d`
- **Source:** `a >= b /= c /= d`
- **Grouping:** `(((a >= b) /= c) /= d)`

### A1.0323 — `a & b - c == d`
- **Source:** `a & b - c == d`
- **Grouping:** `((a & (b - c)) == d)`

### A1.0324 — `a == b ^ c || d`
- **Source:** `a == b ^ c || d`
- **Grouping:** `((a == (b ^ c)) || d)`

### A1.0325 — `a <= b ^ c && d`
- **Source:** `a <= b ^ c && d`
- **Grouping:** `((a <= (b ^ c)) && d)`

### A1.0326 — `a >= b << c << d`
- **Source:** `a >= b << c << d`
- **Grouping:** `(a >= ((b << c) << d))`

### A1.0327 — `a * b <= c & d`
- **Source:** `a * b <= c & d`
- **Grouping:** `((a * b) <= (c & d))`

### A1.0328 — `a % b + c <= d`
- **Source:** `a % b + c <= d`
- **Grouping:** `(((a % b) + c) <= d)`

### A1.0329 — `a && b > c << d`
- **Source:** `a && b > c << d`
- **Grouping:** `(a && (b > (c << d)))`

### A1.0330 — `a - b >= c | d`
- **Source:** `a - b >= c | d`
- **Grouping:** `((a - b) >= (c | d))`

### A1.0331 — `a << b >= c == d`
- **Source:** `a << b >= c == d`
- **Grouping:** `(((a << b) >= c) == d)`

### A1.0332 — `a /= b | c /= d`
- **Source:** `a /= b | c /= d`
- **Grouping:** `((a /= (b | c)) /= d)`

### A1.0333 — `a + b == c < d`
- **Source:** `a + b == c < d`
- **Grouping:** `(((a + b) == c) < d)`

### A1.0334 — `a % b . c > d`
- **Source:** `a % b . c > d`
- **Grouping:** `((a % (b . c)) > d)`

### A1.0335 — `a /= b | c >> d`
- **Source:** `a /= b | c >> d`
- **Grouping:** `(a /= (b | (c >> d)))`

### A1.0336 — `a ^ b || c || d`
- **Source:** `a ^ b || c || d`
- **Grouping:** `(((a ^ b) || c) || d)`

### A1.0337 — `a && b >= c || d`
- **Source:** `a && b >= c || d`
- **Grouping:** `((a && (b >= c)) || d)`

### A1.0338 — `a ^ b == c && d`
- **Source:** `a ^ b == c && d`
- **Grouping:** `(((a ^ b) == c) && d)`

### A1.0339 — `a - b - c == d`
- **Source:** `a - b - c == d`
- **Grouping:** `(((a - b) - c) == d)`

### A1.0340 — `a - b - c <= d`
- **Source:** `a - b - c <= d`
- **Grouping:** `(((a - b) - c) <= d)`

### A1.0341 — `a * b | c > d`
- **Source:** `a * b | c > d`
- **Grouping:** `(((a * b) | c) > d)`

### A1.0342 — `a >= b /= c . d`
- **Source:** `a >= b /= c . d`
- **Grouping:** `((a >= b) /= (c . d))`

### A1.0343 — `a && b . c /= d`
- **Source:** `a && b . c /= d`
- **Grouping:** `(a && ((b . c) /= d))`

### A1.0344 — `a /= b & c << d`
- **Source:** `a /= b & c << d`
- **Grouping:** `(a /= (b & (c << d)))`

### A1.0345 — `a ^ b ^ c == d`
- **Source:** `a ^ b ^ c == d`
- **Grouping:** `(((a ^ b) ^ c) == d)`

### A1.0346 — `a % b || c == d`
- **Source:** `a % b || c == d`
- **Grouping:** `((a % b) || (c == d))`

### A1.0347 — `a >> b >= c . d`
- **Source:** `a >> b >= c . d`
- **Grouping:** `((a >> b) >= (c . d))`

### A1.0348 — `a || b ^ c >= d`
- **Source:** `a || b ^ c >= d`
- **Grouping:** `(a || ((b ^ c) >= d))`

### A1.0349 — `a - b /= c | d`
- **Source:** `a - b /= c | d`
- **Grouping:** `((a - b) /= (c | d))`

### A1.0350 — `a << b << c / d`
- **Source:** `a << b << c / d`
- **Grouping:** `((a << b) << (c / d))`

### A1.0351 — `a == b + c / d`
- **Source:** `a == b + c / d`
- **Grouping:** `(a == (b + (c / d)))`

### A1.0352 — `a << b / c + d`
- **Source:** `a << b / c + d`
- **Grouping:** `(a << ((b / c) + d))`

### A1.0353 — `a / b % c % d`
- **Source:** `a / b % c % d`
- **Grouping:** `(((a / b) % c) % d)`

### A1.0354 — `a & b & c | d`
- **Source:** `a & b & c | d`
- **Grouping:** `(((a & b) & c) | d)`

### A1.0355 — `a . b + c * d`
- **Source:** `a . b + c * d`
- **Grouping:** `((a . b) + (c * d))`

### A1.0356 — `a == b % c >> d`
- **Source:** `a == b % c >> d`
- **Grouping:** `(a == ((b % c) >> d))`

### A1.0357 — `a + b < c > d`
- **Source:** `a + b < c > d`
- **Grouping:** `(((a + b) < c) > d)`

### A1.0358 — `a > b /= c <= d`
- **Source:** `a > b /= c <= d`
- **Grouping:** `(((a > b) /= c) <= d)`

### A1.0359 — `a << b >> c << d`
- **Source:** `a << b >> c << d`
- **Grouping:** `(((a << b) >> c) << d)`

### A1.0360 — `a & b . c << d`
- **Source:** `a & b . c << d`
- **Grouping:** `(a & ((b . c) << d))`

### A1.0361 — `a >= b + c + d`
- **Source:** `a >= b + c + d`
- **Grouping:** `(a >= ((b + c) + d))`

### A1.0362 — `a >= b <= c . d`
- **Source:** `a >= b <= c . d`
- **Grouping:** `((a >= b) <= (c . d))`

### A1.0363 — `a /= b % c << d`
- **Source:** `a /= b % c << d`
- **Grouping:** `(a /= ((b % c) << d))`

### A1.0364 — `a || b % c <= d`
- **Source:** `a || b % c <= d`
- **Grouping:** `(a || ((b % c) <= d))`

### A1.0365 — `a && b | c < d`
- **Source:** `a && b | c < d`
- **Grouping:** `(a && ((b | c) < d))`

### A1.0366 — `a - b < c << d`
- **Source:** `a - b < c << d`
- **Grouping:** `((a - b) < (c << d))`

### A1.0367 — `a / b & c * d`
- **Source:** `a / b & c * d`
- **Grouping:** `((a / b) & (c * d))`

### A1.0368 — `a || b || c >> d`
- **Source:** `a || b || c >> d`
- **Grouping:** `((a || b) || (c >> d))`

### A1.0369 — `a - b | c && d`
- **Source:** `a - b | c && d`
- **Grouping:** `(((a - b) | c) && d)`

### A1.0370 — `a - b && c | d`
- **Source:** `a - b && c | d`
- **Grouping:** `((a - b) && (c | d))`

### A1.0371 — `a << b >= c ^ d`
- **Source:** `a << b >= c ^ d`
- **Grouping:** `((a << b) >= (c ^ d))`

### A1.0372 — `a /= b + c % d`
- **Source:** `a /= b + c % d`
- **Grouping:** `(a /= (b + (c % d)))`

### A1.0373 — `a || b - c + d`
- **Source:** `a || b - c + d`
- **Grouping:** `(a || ((b - c) + d))`

### A1.0374 — `a / b | c || d`
- **Source:** `a / b | c || d`
- **Grouping:** `(((a / b) | c) || d)`

### A1.0375 — `a * b >= c . d`
- **Source:** `a * b >= c . d`
- **Grouping:** `((a * b) >= (c . d))`

### A1.0376 — `a - b > c << d`
- **Source:** `a - b > c << d`
- **Grouping:** `((a - b) > (c << d))`

### A1.0377 — `a + b < c | d`
- **Source:** `a + b < c | d`
- **Grouping:** `((a + b) < (c | d))`

### A1.0378 — `a & b * c % d`
- **Source:** `a & b * c % d`
- **Grouping:** `(a & ((b * c) % d))`

### A1.0379 — `a < b << c * d`
- **Source:** `a < b << c * d`
- **Grouping:** `(a < (b << (c * d)))`

### A1.0380 — `a % b >> c == d`
- **Source:** `a % b >> c == d`
- **Grouping:** `(((a % b) >> c) == d)`

### A1.0381 — `a >= b && c > d`
- **Source:** `a >= b && c > d`
- **Grouping:** `((a >= b) && (c > d))`

### A1.0382 — `a & b >= c >= d`
- **Source:** `a & b >= c >= d`
- **Grouping:** `(((a & b) >= c) >= d)`

### A1.0383 — `a /= b >= c + d`
- **Source:** `a /= b >= c + d`
- **Grouping:** `((a /= b) >= (c + d))`

### A1.0384 — `a & b / c /= d`
- **Source:** `a & b / c /= d`
- **Grouping:** `((a & (b / c)) /= d)`

### A1.0385 — `a < b <= c > d`
- **Source:** `a < b <= c > d`
- **Grouping:** `(((a < b) <= c) > d)`

### A1.0386 — `a /= b * c % d`
- **Source:** `a /= b * c % d`
- **Grouping:** `(a /= ((b * c) % d))`

### A1.0387 — `a >= b * c . d`
- **Source:** `a >= b * c . d`
- **Grouping:** `(a >= (b * (c . d)))`

### A1.0388 — `a ^ b / c == d`
- **Source:** `a ^ b / c == d`
- **Grouping:** `((a ^ (b / c)) == d)`

### A1.0389 — `a && b >= c - d`
- **Source:** `a && b >= c - d`
- **Grouping:** `(a && (b >= (c - d)))`

### A1.0390 — `a <= b || c <= d`
- **Source:** `a <= b || c <= d`
- **Grouping:** `((a <= b) || (c <= d))`

### A1.0391 — `a || b || c . d`
- **Source:** `a || b || c . d`
- **Grouping:** `((a || b) || (c . d))`

### A1.0392 — `a * b > c < d`
- **Source:** `a * b > c < d`
- **Grouping:** `(((a * b) > c) < d)`

### A1.0393 — `a | b && c <= d`
- **Source:** `a | b && c <= d`
- **Grouping:** `((a | b) && (c <= d))`

### A1.0394 — `a - b || c > d`
- **Source:** `a - b || c > d`
- **Grouping:** `((a - b) || (c > d))`

### A1.0395 — `a || b >> c % d`
- **Source:** `a || b >> c % d`
- **Grouping:** `(a || (b >> (c % d)))`

### A1.0396 — `a < b >> c > d`
- **Source:** `a < b >> c > d`
- **Grouping:** `((a < (b >> c)) > d)`

### A1.0397 — `a && b % c >= d`
- **Source:** `a && b % c >= d`
- **Grouping:** `(a && ((b % c) >= d))`

### A1.0398 — `a || b || c ^ d`
- **Source:** `a || b || c ^ d`
- **Grouping:** `((a || b) || (c ^ d))`

### A1.0399 — `a . b || c & d`
- **Source:** `a . b || c & d`
- **Grouping:** `((a . b) || (c & d))`

### A1.0400 — `a & b > c . d`
- **Source:** `a & b > c . d`
- **Grouping:** `((a & b) > (c . d))`

### A1.0401 — `a >= b || c || d`
- **Source:** `a >= b || c || d`
- **Grouping:** `(((a >= b) || c) || d)`

### A1.0402 — `a /= b && c ^ d`
- **Source:** `a /= b && c ^ d`
- **Grouping:** `((a /= b) && (c ^ d))`

### A1.0403 — `a ^ b >= c - d`
- **Source:** `a ^ b >= c - d`
- **Grouping:** `((a ^ b) >= (c - d))`

### A1.0404 — `a && b . c % d`
- **Source:** `a && b . c % d`
- **Grouping:** `(a && ((b . c) % d))`

### A1.0405 — `a ^ b | c / d`
- **Source:** `a ^ b | c / d`
- **Grouping:** `((a ^ b) | (c / d))`

### A1.0406 — `a >= b >= c < d`
- **Source:** `a >= b >= c < d`
- **Grouping:** `(((a >= b) >= c) < d)`

### A1.0407 — `a % b * c >= d`
- **Source:** `a % b * c >= d`
- **Grouping:** `(((a % b) * c) >= d)`

### A1.0408 — `a . b <= c >> d`
- **Source:** `a . b <= c >> d`
- **Grouping:** `((a . b) <= (c >> d))`

### A1.0409 — `a ^ b >> c & d`
- **Source:** `a ^ b >> c & d`
- **Grouping:** `((a ^ (b >> c)) & d)`

### A1.0410 — `a * b << c & d`
- **Source:** `a * b << c & d`
- **Grouping:** `(((a * b) << c) & d)`

### A1.0411 — `a - b > c % d`
- **Source:** `a - b > c % d`
- **Grouping:** `((a - b) > (c % d))`

### A1.0412 — `a >= b >= c >> d`
- **Source:** `a >= b >= c >> d`
- **Grouping:** `((a >= b) >= (c >> d))`

### A1.0413 — `a > b ^ c + d`
- **Source:** `a > b ^ c + d`
- **Grouping:** `(a > (b ^ (c + d)))`

### A1.0414 — `a << b << c && d`
- **Source:** `a << b << c && d`
- **Grouping:** `(((a << b) << c) && d)`

### A1.0415 — `a | b ^ c && d`
- **Source:** `a | b ^ c && d`
- **Grouping:** `(((a | b) ^ c) && d)`

### A1.0416 — `a + b * c && d`
- **Source:** `a + b * c && d`
- **Grouping:** `((a + (b * c)) && d)`

### A1.0417 — `a % b && c + d`
- **Source:** `a % b && c + d`
- **Grouping:** `((a % b) && (c + d))`

### A1.0418 — `a >= b & c + d`
- **Source:** `a >= b & c + d`
- **Grouping:** `(a >= (b & (c + d)))`

### A1.0419 — `a . b - c . d`
- **Source:** `a . b - c . d`
- **Grouping:** `((a . b) - (c . d))`

### A1.0420 — `a + b == c & d`
- **Source:** `a + b == c & d`
- **Grouping:** `((a + b) == (c & d))`

### A1.0421 — `a ^ b < c << d`
- **Source:** `a ^ b < c << d`
- **Grouping:** `((a ^ b) < (c << d))`

### A1.0422 — `a == b >= c <= d`
- **Source:** `a == b >= c <= d`
- **Grouping:** `(((a == b) >= c) <= d)`

### A1.0423 — `a | b >= c ^ d`
- **Source:** `a | b >= c ^ d`
- **Grouping:** `((a | b) >= (c ^ d))`

### A1.0424 — `a % b | c < d`
- **Source:** `a % b | c < d`
- **Grouping:** `(((a % b) | c) < d)`

### A1.0425 — `a & b >> c == d`
- **Source:** `a & b >> c == d`
- **Grouping:** `((a & (b >> c)) == d)`

### A1.0426 — `a . b /= c << d`
- **Source:** `a . b /= c << d`
- **Grouping:** `((a . b) /= (c << d))`

### A1.0427 — `a < b /= c + d`
- **Source:** `a < b /= c + d`
- **Grouping:** `((a < b) /= (c + d))`

### A1.0428 — `a || b & c && d`
- **Source:** `a || b & c && d`
- **Grouping:** `(a || ((b & c) && d))`

### A1.0429 — `a >= b < c << d`
- **Source:** `a >= b < c << d`
- **Grouping:** `((a >= b) < (c << d))`

### A1.0430 — `a /= b * c | d`
- **Source:** `a /= b * c | d`
- **Grouping:** `(a /= ((b * c) | d))`

### A1.0431 — `a || b < c & d`
- **Source:** `a || b < c & d`
- **Grouping:** `(a || (b < (c & d)))`

### A1.0432 — `a < b . c & d`
- **Source:** `a < b . c & d`
- **Grouping:** `(a < ((b . c) & d))`

### A1.0433 — `a | b - c / d`
- **Source:** `a | b - c / d`
- **Grouping:** `(a | (b - (c / d)))`

### A1.0434 — `a >= b < c > d`
- **Source:** `a >= b < c > d`
- **Grouping:** `(((a >= b) < c) > d)`

### A1.0435 — `a == b | c / d`
- **Source:** `a == b | c / d`
- **Grouping:** `(a == (b | (c / d)))`

### A1.0436 — `a < b - c << d`
- **Source:** `a < b - c << d`
- **Grouping:** `(a < ((b - c) << d))`

### A1.0437 — `a . b >= c /= d`
- **Source:** `a . b >= c /= d`
- **Grouping:** `(((a . b) >= c) /= d)`

### A1.0438 — `a << b > c & d`
- **Source:** `a << b > c & d`
- **Grouping:** `((a << b) > (c & d))`

### A1.0439 — `a ^ b | c & d`
- **Source:** `a ^ b | c & d`
- **Grouping:** `(((a ^ b) | c) & d)`

### A1.0440 — `a <= b << c | d`
- **Source:** `a <= b << c | d`
- **Grouping:** `(a <= ((b << c) | d))`

### A1.0441 — `a >= b == c == d`
- **Source:** `a >= b == c == d`
- **Grouping:** `(((a >= b) == c) == d)`

### A1.0442 — `a | b << c << d`
- **Source:** `a | b << c << d`
- **Grouping:** `(a | ((b << c) << d))`

### A1.0443 — `a & b >> c && d`
- **Source:** `a & b >> c && d`
- **Grouping:** `((a & (b >> c)) && d)`

### A1.0444 — `a . b / c | d`
- **Source:** `a . b / c | d`
- **Grouping:** `(((a . b) / c) | d)`

### A1.0445 — `a || b > c ^ d`
- **Source:** `a || b > c ^ d`
- **Grouping:** `(a || (b > (c ^ d)))`

### A1.0446 — `a % b | c << d`
- **Source:** `a % b | c << d`
- **Grouping:** `((a % b) | (c << d))`

### A1.0447 — `a & b <= c >> d`
- **Source:** `a & b <= c >> d`
- **Grouping:** `((a & b) <= (c >> d))`

### A1.0448 — `a > b >= c ^ d`
- **Source:** `a > b >= c ^ d`
- **Grouping:** `((a > b) >= (c ^ d))`

### A1.0449 — `a >= b / c << d`
- **Source:** `a >= b / c << d`
- **Grouping:** `(a >= ((b / c) << d))`

### A1.0450 — `a ^ b || c == d`
- **Source:** `a ^ b || c == d`
- **Grouping:** `((a ^ b) || (c == d))`

### A1.0451 — `a * b | c == d`
- **Source:** `a * b | c == d`
- **Grouping:** `(((a * b) | c) == d)`

### A1.0452 — `a >= b ^ c /= d`
- **Source:** `a >= b ^ c /= d`
- **Grouping:** `((a >= (b ^ c)) /= d)`

### A1.0453 — `a >> b && c >= d`
- **Source:** `a >> b && c >= d`
- **Grouping:** `((a >> b) && (c >= d))`

### A1.0454 — `a + b && c % d`
- **Source:** `a + b && c % d`
- **Grouping:** `((a + b) && (c % d))`

### A1.0455 — `a / b >= c . d`
- **Source:** `a / b >= c . d`
- **Grouping:** `((a / b) >= (c . d))`

### A1.0456 — `a < b /= c || d`
- **Source:** `a < b /= c || d`
- **Grouping:** `(((a < b) /= c) || d)`

### A1.0457 — `a < b < c < d`
- **Source:** `a < b < c < d`
- **Grouping:** `(((a < b) < c) < d)`

### A1.0458 — `a < b || c % d`
- **Source:** `a < b || c % d`
- **Grouping:** `((a < b) || (c % d))`

### A1.0459 — `a & b <= c - d`
- **Source:** `a & b <= c - d`
- **Grouping:** `((a & b) <= (c - d))`

### A1.0460 — `a && b < c >> d`
- **Source:** `a && b < c >> d`
- **Grouping:** `(a && (b < (c >> d)))`

### A1.0461 — `a << b || c <= d`
- **Source:** `a << b || c <= d`
- **Grouping:** `((a << b) || (c <= d))`

### A1.0462 — `a >= b && c ^ d`
- **Source:** `a >= b && c ^ d`
- **Grouping:** `((a >= b) && (c ^ d))`

### A1.0463 — `a == b < c < d`
- **Source:** `a == b < c < d`
- **Grouping:** `(((a == b) < c) < d)`

### A1.0464 — `a >> b >= c ^ d`
- **Source:** `a >> b >= c ^ d`
- **Grouping:** `((a >> b) >= (c ^ d))`

### A1.0465 — `a == b && c > d`
- **Source:** `a == b && c > d`
- **Grouping:** `((a == b) && (c > d))`

### A1.0466 — `a . b / c >> d`
- **Source:** `a . b / c >> d`
- **Grouping:** `(((a . b) / c) >> d)`

### A1.0467 — `a < b . c || d`
- **Source:** `a < b . c || d`
- **Grouping:** `((a < (b . c)) || d)`

### A1.0468 — `a >= b - c & d`
- **Source:** `a >= b - c & d`
- **Grouping:** `(a >= ((b - c) & d))`

### A1.0469 — `a /= b << c == d`
- **Source:** `a /= b << c == d`
- **Grouping:** `((a /= (b << c)) == d)`

### A1.0470 — `a || b ^ c << d`
- **Source:** `a || b ^ c << d`
- **Grouping:** `(a || (b ^ (c << d)))`

### A1.0471 — `a ^ b << c - d`
- **Source:** `a ^ b << c - d`
- **Grouping:** `(a ^ (b << (c - d)))`

### A1.0472 — `a & b . c < d`
- **Source:** `a & b . c < d`
- **Grouping:** `((a & (b . c)) < d)`

### A1.0473 — `a <= b || c * d`
- **Source:** `a <= b || c * d`
- **Grouping:** `((a <= b) || (c * d))`

### A1.0474 — `a + b . c /= d`
- **Source:** `a + b . c /= d`
- **Grouping:** `((a + (b . c)) /= d)`

### A1.0475 — `a ^ b << c & d`
- **Source:** `a ^ b << c & d`
- **Grouping:** `((a ^ (b << c)) & d)`

### A1.0476 — `a || b || c /= d`
- **Source:** `a || b || c /= d`
- **Grouping:** `((a || b) || (c /= d))`

### A1.0477 — `a < b & c >> d`
- **Source:** `a < b & c >> d`
- **Grouping:** `(a < (b & (c >> d)))`

### A1.0478 — `a >= b || c . d`
- **Source:** `a >= b || c . d`
- **Grouping:** `((a >= b) || (c . d))`

### A1.0479 — `a << b > c * d`
- **Source:** `a << b > c * d`
- **Grouping:** `((a << b) > (c * d))`

### A1.0480 — `a ^ b + c >= d`
- **Source:** `a ^ b + c >= d`
- **Grouping:** `((a ^ (b + c)) >= d)`

### A1.0481 — `a - b * c < d`
- **Source:** `a - b * c < d`
- **Grouping:** `((a - (b * c)) < d)`

### A1.0482 — `a % b || c + d`
- **Source:** `a % b || c + d`
- **Grouping:** `((a % b) || (c + d))`

### A1.0483 — `a || b | c < d`
- **Source:** `a || b | c < d`
- **Grouping:** `(a || ((b | c) < d))`

### A1.0484 — `a % b < c == d`
- **Source:** `a % b < c == d`
- **Grouping:** `(((a % b) < c) == d)`

### A1.0485 — `a <= b * c | d`
- **Source:** `a <= b * c | d`
- **Grouping:** `(a <= ((b * c) | d))`

### A1.0486 — `a > b . c ^ d`
- **Source:** `a > b . c ^ d`
- **Grouping:** `(a > ((b . c) ^ d))`

### A1.0487 — `a || b | c + d`
- **Source:** `a || b | c + d`
- **Grouping:** `(a || (b | (c + d)))`

### A1.0488 — `a <= b & c - d`
- **Source:** `a <= b & c - d`
- **Grouping:** `(a <= (b & (c - d)))`

### A1.0489 — `a * b || c - d`
- **Source:** `a * b || c - d`
- **Grouping:** `((a * b) || (c - d))`

### A1.0490 — `a ^ b ^ c >= d`
- **Source:** `a ^ b ^ c >= d`
- **Grouping:** `(((a ^ b) ^ c) >= d)`

### A1.0491 — `a & b <= c + d`
- **Source:** `a & b <= c + d`
- **Grouping:** `((a & b) <= (c + d))`

### A1.0492 — `a || b % c - d`
- **Source:** `a || b % c - d`
- **Grouping:** `(a || ((b % c) - d))`

### A1.0493 — `a % b && c == d`
- **Source:** `a % b && c == d`
- **Grouping:** `((a % b) && (c == d))`

### A1.0494 — `a < b > c . d`
- **Source:** `a < b > c . d`
- **Grouping:** `((a < b) > (c . d))`

### A1.0495 — `a >> b == c % d`
- **Source:** `a >> b == c % d`
- **Grouping:** `((a >> b) == (c % d))`

### A1.0496 — `a /= b - c - d`
- **Source:** `a /= b - c - d`
- **Grouping:** `(a /= ((b - c) - d))`

### A1.0497 — `a > b == c >= d`
- **Source:** `a > b == c >= d`
- **Grouping:** `(((a > b) == c) >= d)`

### A1.0498 — `a == b - c | d`
- **Source:** `a == b - c | d`
- **Grouping:** `(a == ((b - c) | d))`

### A1.0499 — `a >> b - c <= d`
- **Source:** `a >> b - c <= d`
- **Grouping:** `((a >> (b - c)) <= d)`

### A1.0500 — `a && b * c % d`
- **Source:** `a && b * c % d`
- **Grouping:** `(a && ((b * c) % d))`

### A1.0501 — `a % b . c * d`
- **Source:** `a % b . c * d`
- **Grouping:** `((a % (b . c)) * d)`

### A1.0502 — `a . b && c <= d`
- **Source:** `a . b && c <= d`
- **Grouping:** `((a . b) && (c <= d))`

### A1.0503 — `a / b /= c + d`
- **Source:** `a / b /= c + d`
- **Grouping:** `((a / b) /= (c + d))`

### A1.0504 — `a % b + c . d`
- **Source:** `a % b + c . d`
- **Grouping:** `((a % b) + (c . d))`

### A1.0505 — `a << b / c << d`
- **Source:** `a << b / c << d`
- **Grouping:** `((a << (b / c)) << d)`

### A1.0506 — `a >> b && c || d`
- **Source:** `a >> b && c || d`
- **Grouping:** `(((a >> b) && c) || d)`

### A1.0507 — `a - b & c && d`
- **Source:** `a - b & c && d`
- **Grouping:** `(((a - b) & c) && d)`

### A1.0508 — `a << b /= c + d`
- **Source:** `a << b /= c + d`
- **Grouping:** `((a << b) /= (c + d))`

### A1.0509 — `a && b / c >= d`
- **Source:** `a && b / c >= d`
- **Grouping:** `(a && ((b / c) >= d))`

### A1.0510 — `a / b + c < d`
- **Source:** `a / b + c < d`
- **Grouping:** `(((a / b) + c) < d)`

### A1.0511 — `a . b > c <= d`
- **Source:** `a . b > c <= d`
- **Grouping:** `(((a . b) > c) <= d)`

### A1.0512 — `a && b - c == d`
- **Source:** `a && b - c == d`
- **Grouping:** `(a && ((b - c) == d))`

### A1.0513 — `a && b . c ^ d`
- **Source:** `a && b . c ^ d`
- **Grouping:** `(a && ((b . c) ^ d))`

### A1.0514 — `a > b /= c - d`
- **Source:** `a > b /= c - d`
- **Grouping:** `((a > b) /= (c - d))`

### A1.0515 — `a >> b | c || d`
- **Source:** `a >> b | c || d`
- **Grouping:** `(((a >> b) | c) || d)`

### A1.0516 — `a | b | c == d`
- **Source:** `a | b | c == d`
- **Grouping:** `(((a | b) | c) == d)`

### A1.0517 — `a + b * c . d`
- **Source:** `a + b * c . d`
- **Grouping:** `(a + (b * (c . d)))`

### A1.0518 — `a >> b + c >> d`
- **Source:** `a >> b + c >> d`
- **Grouping:** `((a >> (b + c)) >> d)`

### A1.0519 — `a | b | c << d`
- **Source:** `a | b | c << d`
- **Grouping:** `((a | b) | (c << d))`

### A1.0520 — `a % b / c >> d`
- **Source:** `a % b / c >> d`
- **Grouping:** `(((a % b) / c) >> d)`

### A1.0521 — `a & b + c > d`
- **Source:** `a & b + c > d`
- **Grouping:** `((a & (b + c)) > d)`

### A1.0522 — `a >> b && c << d`
- **Source:** `a >> b && c << d`
- **Grouping:** `((a >> b) && (c << d))`

### A1.0523 — `a | b | c | d`
- **Source:** `a | b | c | d`
- **Grouping:** `(((a | b) | c) | d)`

### A1.0524 — `a /= b * c >> d`
- **Source:** `a /= b * c >> d`
- **Grouping:** `(a /= ((b * c) >> d))`

### A1.0525 — `a << b <= c == d`
- **Source:** `a << b <= c == d`
- **Grouping:** `(((a << b) <= c) == d)`

### A1.0526 — `a ^ b >> c && d`
- **Source:** `a ^ b >> c && d`
- **Grouping:** `((a ^ (b >> c)) && d)`

### A1.0527 — `a <= b /= c | d`
- **Source:** `a <= b /= c | d`
- **Grouping:** `((a <= b) /= (c | d))`

### A1.0528 — `a >> b + c /= d`
- **Source:** `a >> b + c /= d`
- **Grouping:** `((a >> (b + c)) /= d)`

### A1.0529 — `a - b ^ c /= d`
- **Source:** `a - b ^ c /= d`
- **Grouping:** `(((a - b) ^ c) /= d)`

### A1.0530 — `a - b == c /= d`
- **Source:** `a - b == c /= d`
- **Grouping:** `(((a - b) == c) /= d)`

### A1.0531 — `a && b && c & d`
- **Source:** `a && b && c & d`
- **Grouping:** `((a && b) && (c & d))`

### A1.0532 — `a >> b < c >> d`
- **Source:** `a >> b < c >> d`
- **Grouping:** `((a >> b) < (c >> d))`

### A1.0533 — `a / b & c + d`
- **Source:** `a / b & c + d`
- **Grouping:** `((a / b) & (c + d))`

### A1.0534 — `a >> b || c << d`
- **Source:** `a >> b || c << d`
- **Grouping:** `((a >> b) || (c << d))`

### A1.0535 — `a <= b + c ^ d`
- **Source:** `a <= b + c ^ d`
- **Grouping:** `(a <= ((b + c) ^ d))`

### A1.0536 — `a . b /= c + d`
- **Source:** `a . b /= c + d`
- **Grouping:** `((a . b) /= (c + d))`

### A1.0537 — `a * b << c * d`
- **Source:** `a * b << c * d`
- **Grouping:** `((a * b) << (c * d))`

### A1.0538 — `a >= b % c / d`
- **Source:** `a >= b % c / d`
- **Grouping:** `(a >= ((b % c) / d))`

### A1.0539 — `a >= b || c | d`
- **Source:** `a >= b || c | d`
- **Grouping:** `((a >= b) || (c | d))`

### A1.0540 — `a > b > c << d`
- **Source:** `a > b > c << d`
- **Grouping:** `((a > b) > (c << d))`

### A1.0541 — `a <= b | c / d`
- **Source:** `a <= b | c / d`
- **Grouping:** `(a <= (b | (c / d)))`

### A1.0542 — `a < b == c >> d`
- **Source:** `a < b == c >> d`
- **Grouping:** `((a < b) == (c >> d))`

### A1.0543 — `a - b || c ^ d`
- **Source:** `a - b || c ^ d`
- **Grouping:** `((a - b) || (c ^ d))`

### A1.0544 — `a / b /= c - d`
- **Source:** `a / b /= c - d`
- **Grouping:** `((a / b) /= (c - d))`

### A1.0545 — `a < b < c == d`
- **Source:** `a < b < c == d`
- **Grouping:** `(((a < b) < c) == d)`

### A1.0546 — `a - b & c < d`
- **Source:** `a - b & c < d`
- **Grouping:** `(((a - b) & c) < d)`

### A1.0547 — `a == b << c == d`
- **Source:** `a == b << c == d`
- **Grouping:** `((a == (b << c)) == d)`

### A1.0548 — `a - b / c >= d`
- **Source:** `a - b / c >= d`
- **Grouping:** `((a - (b / c)) >= d)`

### A1.0549 — `a * b || c == d`
- **Source:** `a * b || c == d`
- **Grouping:** `((a * b) || (c == d))`

### A1.0550 — `a >= b - c | d`
- **Source:** `a >= b - c | d`
- **Grouping:** `(a >= ((b - c) | d))`

### A1.0551 — `a * b == c / d`
- **Source:** `a * b == c / d`
- **Grouping:** `((a * b) == (c / d))`

### A1.0552 — `a /= b <= c && d`
- **Source:** `a /= b <= c && d`
- **Grouping:** `(((a /= b) <= c) && d)`

### A1.0553 — `a % b . c % d`
- **Source:** `a % b . c % d`
- **Grouping:** `((a % (b . c)) % d)`

### A1.0554 — `a <= b >> c || d`
- **Source:** `a <= b >> c || d`
- **Grouping:** `((a <= (b >> c)) || d)`

### A1.0555 — `a ^ b << c * d`
- **Source:** `a ^ b << c * d`
- **Grouping:** `(a ^ (b << (c * d)))`

### A1.0556 — `a / b ^ c + d`
- **Source:** `a / b ^ c + d`
- **Grouping:** `((a / b) ^ (c + d))`

### A1.0557 — `a & b >> c >> d`
- **Source:** `a & b >> c >> d`
- **Grouping:** `(a & ((b >> c) >> d))`

### A1.0558 — `a * b || c / d`
- **Source:** `a * b || c / d`
- **Grouping:** `((a * b) || (c / d))`

### A1.0559 — `a >= b /= c & d`
- **Source:** `a >= b /= c & d`
- **Grouping:** `((a >= b) /= (c & d))`

### A1.0560 — `a | b . c << d`
- **Source:** `a | b . c << d`
- **Grouping:** `(a | ((b . c) << d))`

### A1.0561 — `a >= b <= c & d`
- **Source:** `a >= b <= c & d`
- **Grouping:** `((a >= b) <= (c & d))`

### A1.0562 — `a * b - c >= d`
- **Source:** `a * b - c >= d`
- **Grouping:** `(((a * b) - c) >= d)`

### A1.0563 — `a >> b >> c - d`
- **Source:** `a >> b >> c - d`
- **Grouping:** `((a >> b) >> (c - d))`

### A1.0564 — `a & b . c == d`
- **Source:** `a & b . c == d`
- **Grouping:** `((a & (b . c)) == d)`

### A1.0565 — `a + b << c < d`
- **Source:** `a + b << c < d`
- **Grouping:** `(((a + b) << c) < d)`

### A1.0566 — `a * b << c || d`
- **Source:** `a * b << c || d`
- **Grouping:** `(((a * b) << c) || d)`

### A1.0567 — `a . b > c /= d`
- **Source:** `a . b > c /= d`
- **Grouping:** `(((a . b) > c) /= d)`

### A1.0568 — `a | b ^ c << d`
- **Source:** `a | b ^ c << d`
- **Grouping:** `((a | b) ^ (c << d))`

### A1.0569 — `a > b < c & d`
- **Source:** `a > b < c & d`
- **Grouping:** `((a > b) < (c & d))`

### A1.0570 — `a % b * c && d`
- **Source:** `a % b * c && d`
- **Grouping:** `(((a % b) * c) && d)`

### A1.0571 — `a >= b << c >> d`
- **Source:** `a >= b << c >> d`
- **Grouping:** `(a >= ((b << c) >> d))`

### A1.0572 — `a - b * c + d`
- **Source:** `a - b * c + d`
- **Grouping:** `((a - (b * c)) + d)`

### A1.0573 — `a - b < c >= d`
- **Source:** `a - b < c >= d`
- **Grouping:** `(((a - b) < c) >= d)`

### A1.0574 — `a . b / c || d`
- **Source:** `a . b / c || d`
- **Grouping:** `(((a . b) / c) || d)`

### A1.0575 — `a & b == c < d`
- **Source:** `a & b == c < d`
- **Grouping:** `(((a & b) == c) < d)`

### A1.0576 — `a << b % c & d`
- **Source:** `a << b % c & d`
- **Grouping:** `((a << (b % c)) & d)`

### A1.0577 — `a > b - c - d`
- **Source:** `a > b - c - d`
- **Grouping:** `(a > ((b - c) - d))`

### A1.0578 — `a & b << c && d`
- **Source:** `a & b << c && d`
- **Grouping:** `((a & (b << c)) && d)`

### A1.0579 — `a + b + c << d`
- **Source:** `a + b + c << d`
- **Grouping:** `(((a + b) + c) << d)`

### A1.0580 — `a || b == c / d`
- **Source:** `a || b == c / d`
- **Grouping:** `(a || (b == (c / d)))`

### A1.0581 — `a - b + c || d`
- **Source:** `a - b + c || d`
- **Grouping:** `(((a - b) + c) || d)`

### A1.0582 — `a <= b || c && d`
- **Source:** `a <= b || c && d`
- **Grouping:** `((a <= b) || (c && d))`

### A1.0583 — `a ^ b / c >= d`
- **Source:** `a ^ b / c >= d`
- **Grouping:** `((a ^ (b / c)) >= d)`

### A1.0584 — `a || b == c >> d`
- **Source:** `a || b == c >> d`
- **Grouping:** `(a || (b == (c >> d)))`

### A1.0585 — `a | b /= c >= d`
- **Source:** `a | b /= c >= d`
- **Grouping:** `(((a | b) /= c) >= d)`

### A1.0586 — `a . b >= c & d`
- **Source:** `a . b >= c & d`
- **Grouping:** `((a . b) >= (c & d))`

### A1.0587 — `a && b / c | d`
- **Source:** `a && b / c | d`
- **Grouping:** `(a && ((b / c) | d))`

### A1.0588 — `a % b >> c - d`
- **Source:** `a % b >> c - d`
- **Grouping:** `((a % b) >> (c - d))`

### A1.0589 — `a & b /= c + d`
- **Source:** `a & b /= c + d`
- **Grouping:** `((a & b) /= (c + d))`

### A1.0590 — `a < b < c >> d`
- **Source:** `a < b < c >> d`
- **Grouping:** `((a < b) < (c >> d))`

### A1.0591 — `a && b < c << d`
- **Source:** `a && b < c << d`
- **Grouping:** `(a && (b < (c << d)))`

### A1.0592 — `a /= b >> c >= d`
- **Source:** `a /= b >> c >= d`
- **Grouping:** `((a /= (b >> c)) >= d)`

### A1.0593 — `a . b - c + d`
- **Source:** `a . b - c + d`
- **Grouping:** `(((a . b) - c) + d)`

### A1.0594 — `a > b /= c ^ d`
- **Source:** `a > b /= c ^ d`
- **Grouping:** `((a > b) /= (c ^ d))`

### A1.0595 — `a - b >= c ^ d`
- **Source:** `a - b >= c ^ d`
- **Grouping:** `((a - b) >= (c ^ d))`

### A1.0596 — `a * b * c > d`
- **Source:** `a * b * c > d`
- **Grouping:** `(((a * b) * c) > d)`

### A1.0597 — `a << b && c % d`
- **Source:** `a << b && c % d`
- **Grouping:** `((a << b) && (c % d))`

### A1.0598 — `a . b % c % d`
- **Source:** `a . b % c % d`
- **Grouping:** `(((a . b) % c) % d)`

### A1.0599 — `a >= b | c * d`
- **Source:** `a >= b | c * d`
- **Grouping:** `(a >= (b | (c * d)))`

### A1.0600 — `a - b - c . d`
- **Source:** `a - b - c . d`
- **Grouping:** `((a - b) - (c . d))`

### A1.0601 — `a /= b >= c ^ d`
- **Source:** `a /= b >= c ^ d`
- **Grouping:** `((a /= b) >= (c ^ d))`

### A1.0602 — `a | b /= c + d`
- **Source:** `a | b /= c + d`
- **Grouping:** `((a | b) /= (c + d))`

### A1.0603 — `a || b && c . d`
- **Source:** `a || b && c . d`
- **Grouping:** `(a || (b && (c . d)))`

### A1.0604 — `a * b % c | d`
- **Source:** `a * b % c | d`
- **Grouping:** `(((a * b) % c) | d)`

### A1.0605 — `a - b >= c >> d`
- **Source:** `a - b >= c >> d`
- **Grouping:** `((a - b) >= (c >> d))`

### A1.0606 — `a && b > c * d`
- **Source:** `a && b > c * d`
- **Grouping:** `(a && (b > (c * d)))`

### A1.0607 — `a == b > c + d`
- **Source:** `a == b > c + d`
- **Grouping:** `((a == b) > (c + d))`

### A1.0608 — `a >> b - c / d`
- **Source:** `a >> b - c / d`
- **Grouping:** `(a >> (b - (c / d)))`

### A1.0609 — `a && b * c <= d`
- **Source:** `a && b * c <= d`
- **Grouping:** `(a && ((b * c) <= d))`

### A1.0610 — `a >= b % c | d`
- **Source:** `a >= b % c | d`
- **Grouping:** `(a >= ((b % c) | d))`

### A1.0611 — `a < b % c / d`
- **Source:** `a < b % c / d`
- **Grouping:** `(a < ((b % c) / d))`

### A1.0612 — `a >> b && c - d`
- **Source:** `a >> b && c - d`
- **Grouping:** `((a >> b) && (c - d))`

### A1.0613 — `a /= b == c == d`
- **Source:** `a /= b == c == d`
- **Grouping:** `(((a /= b) == c) == d)`

### A1.0614 — `a % b / c / d`
- **Source:** `a % b / c / d`
- **Grouping:** `(((a % b) / c) / d)`

### A1.0615 — `a * b + c >= d`
- **Source:** `a * b + c >= d`
- **Grouping:** `(((a * b) + c) >= d)`

### A1.0616 — `a & b / c > d`
- **Source:** `a & b / c > d`
- **Grouping:** `((a & (b / c)) > d)`

### A1.0617 — `a ^ b < c % d`
- **Source:** `a ^ b < c % d`
- **Grouping:** `((a ^ b) < (c % d))`

### A1.0618 — `a < b >> c == d`
- **Source:** `a < b >> c == d`
- **Grouping:** `((a < (b >> c)) == d)`

### A1.0619 — `a * b % c && d`
- **Source:** `a * b % c && d`
- **Grouping:** `(((a * b) % c) && d)`

### A1.0620 — `a <= b | c >> d`
- **Source:** `a <= b | c >> d`
- **Grouping:** `(a <= (b | (c >> d)))`

### A1.0621 — `a < b % c % d`
- **Source:** `a < b % c % d`
- **Grouping:** `(a < ((b % c) % d))`

### A1.0622 — `a | b . c / d`
- **Source:** `a | b . c / d`
- **Grouping:** `(a | ((b . c) / d))`

### A1.0623 — `a . b <= c == d`
- **Source:** `a . b <= c == d`
- **Grouping:** `(((a . b) <= c) == d)`

### A1.0624 — `a < b . c < d`
- **Source:** `a < b . c < d`
- **Grouping:** `((a < (b . c)) < d)`

### A1.0625 — `a % b == c == d`
- **Source:** `a % b == c == d`
- **Grouping:** `(((a % b) == c) == d)`

### A1.0626 — `a | b << c <= d`
- **Source:** `a | b << c <= d`
- **Grouping:** `((a | (b << c)) <= d)`

### A1.0627 — `a && b <= c . d`
- **Source:** `a && b <= c . d`
- **Grouping:** `(a && (b <= (c . d)))`

### A1.0628 — `a || b & c == d`
- **Source:** `a || b & c == d`
- **Grouping:** `(a || ((b & c) == d))`

### A1.0629 — `a >> b < c /= d`
- **Source:** `a >> b < c /= d`
- **Grouping:** `(((a >> b) < c) /= d)`

### A1.0630 — `a > b < c + d`
- **Source:** `a > b < c + d`
- **Grouping:** `((a > b) < (c + d))`

### A1.0631 — `a < b << c + d`
- **Source:** `a < b << c + d`
- **Grouping:** `(a < (b << (c + d)))`

### A1.0632 — `a || b / c | d`
- **Source:** `a || b / c | d`
- **Grouping:** `(a || ((b / c) | d))`

### A1.0633 — `a >> b && c > d`
- **Source:** `a >> b && c > d`
- **Grouping:** `((a >> b) && (c > d))`

### A1.0634 — `a + b || c . d`
- **Source:** `a + b || c . d`
- **Grouping:** `((a + b) || (c . d))`

### A1.0635 — `a ^ b && c ^ d`
- **Source:** `a ^ b && c ^ d`
- **Grouping:** `((a ^ b) && (c ^ d))`

### A1.0636 — `a ^ b ^ c ^ d`
- **Source:** `a ^ b ^ c ^ d`
- **Grouping:** `(((a ^ b) ^ c) ^ d)`

### A1.0637 — `a >> b >> c <= d`
- **Source:** `a >> b >> c <= d`
- **Grouping:** `(((a >> b) >> c) <= d)`

### A1.0638 — `a == b > c <= d`
- **Source:** `a == b > c <= d`
- **Grouping:** `(((a == b) > c) <= d)`

### A1.0639 — `a || b . c | d`
- **Source:** `a || b . c | d`
- **Grouping:** `(a || ((b . c) | d))`

### A1.0640 — `a == b >= c >= d`
- **Source:** `a == b >= c >= d`
- **Grouping:** `(((a == b) >= c) >= d)`

### A1.0641 — `a && b || c && d`
- **Source:** `a && b || c && d`
- **Grouping:** `((a && b) || (c && d))`

### A1.0642 — `a > b & c - d`
- **Source:** `a > b & c - d`
- **Grouping:** `(a > (b & (c - d)))`

### A1.0643 — `a - b | c & d`
- **Source:** `a - b | c & d`
- **Grouping:** `(((a - b) | c) & d)`

### A1.0644 — `a < b % c || d`
- **Source:** `a < b % c || d`
- **Grouping:** `((a < (b % c)) || d)`

### A1.0645 — `a > b || c - d`
- **Source:** `a > b || c - d`
- **Grouping:** `((a > b) || (c - d))`

### A1.0646 — `a * b % c >= d`
- **Source:** `a * b % c >= d`
- **Grouping:** `(((a * b) % c) >= d)`

### A1.0647 — `a - b <= c % d`
- **Source:** `a - b <= c % d`
- **Grouping:** `((a - b) <= (c % d))`

### A1.0648 — `a * b - c + d`
- **Source:** `a * b - c + d`
- **Grouping:** `(((a * b) - c) + d)`

### A1.0649 — `a >= b == c + d`
- **Source:** `a >= b == c + d`
- **Grouping:** `((a >= b) == (c + d))`

### A1.0650 — `a && b + c || d`
- **Source:** `a && b + c || d`
- **Grouping:** `((a && (b + c)) || d)`

### A1.0651 — `a <= b . c >> d`
- **Source:** `a <= b . c >> d`
- **Grouping:** `(a <= ((b . c) >> d))`

### A1.0652 — `a << b + c > d`
- **Source:** `a << b + c > d`
- **Grouping:** `((a << (b + c)) > d)`

### A1.0653 — `a || b == c <= d`
- **Source:** `a || b == c <= d`
- **Grouping:** `(a || ((b == c) <= d))`

### A1.0654 — `a > b ^ c << d`
- **Source:** `a > b ^ c << d`
- **Grouping:** `(a > (b ^ (c << d)))`

### A1.0655 — `a - b | c . d`
- **Source:** `a - b | c . d`
- **Grouping:** `((a - b) | (c . d))`

### A1.0656 — `a << b <= c / d`
- **Source:** `a << b <= c / d`
- **Grouping:** `((a << b) <= (c / d))`

### A1.0657 — `a >= b | c | d`
- **Source:** `a >= b | c | d`
- **Grouping:** `(a >= ((b | c) | d))`

### A1.0658 — `a / b > c - d`
- **Source:** `a / b > c - d`
- **Grouping:** `((a / b) > (c - d))`

### A1.0659 — `a <= b >> c & d`
- **Source:** `a <= b >> c & d`
- **Grouping:** `(a <= ((b >> c) & d))`

### A1.0660 — `a % b / c - d`
- **Source:** `a % b / c - d`
- **Grouping:** `(((a % b) / c) - d)`

### A1.0661 — `a > b >> c . d`
- **Source:** `a > b >> c . d`
- **Grouping:** `(a > (b >> (c . d)))`

### A1.0662 — `a >= b % c + d`
- **Source:** `a >= b % c + d`
- **Grouping:** `(a >= ((b % c) + d))`

### A1.0663 — `a /= b && c <= d`
- **Source:** `a /= b && c <= d`
- **Grouping:** `((a /= b) && (c <= d))`

### A1.0664 — `a <= b - c - d`
- **Source:** `a <= b - c - d`
- **Grouping:** `(a <= ((b - c) - d))`

### A1.0665 — `a > b . c /= d`
- **Source:** `a > b . c /= d`
- **Grouping:** `((a > (b . c)) /= d)`

### A1.0666 — `a * b < c . d`
- **Source:** `a * b < c . d`
- **Grouping:** `((a * b) < (c . d))`

### A1.0667 — `a <= b || c >> d`
- **Source:** `a <= b || c >> d`
- **Grouping:** `((a <= b) || (c >> d))`

### A1.0668 — `a < b && c | d`
- **Source:** `a < b && c | d`
- **Grouping:** `((a < b) && (c | d))`

### A1.0669 — `a < b >= c > d`
- **Source:** `a < b >= c > d`
- **Grouping:** `(((a < b) >= c) > d)`

### A1.0670 — `a % b / c | d`
- **Source:** `a % b / c | d`
- **Grouping:** `(((a % b) / c) | d)`

### A1.0671 — `a . b & c >> d`
- **Source:** `a . b & c >> d`
- **Grouping:** `((a . b) & (c >> d))`

### A1.0672 — `a ^ b * c / d`
- **Source:** `a ^ b * c / d`
- **Grouping:** `(a ^ ((b * c) / d))`

### A1.0673 — `a >= b + c >= d`
- **Source:** `a >= b + c >= d`
- **Grouping:** `((a >= (b + c)) >= d)`

### A1.0674 — `a << b > c > d`
- **Source:** `a << b > c > d`
- **Grouping:** `(((a << b) > c) > d)`

### A1.0675 — `a || b - c > d`
- **Source:** `a || b - c > d`
- **Grouping:** `(a || ((b - c) > d))`

### A1.0676 — `a == b == c /= d`
- **Source:** `a == b == c /= d`
- **Grouping:** `(((a == b) == c) /= d)`

### A1.0677 — `a << b - c == d`
- **Source:** `a << b - c == d`
- **Grouping:** `((a << (b - c)) == d)`

### A1.0678 — `a - b || c | d`
- **Source:** `a - b || c | d`
- **Grouping:** `((a - b) || (c | d))`

### A1.0679 — `a % b - c << d`
- **Source:** `a % b - c << d`
- **Grouping:** `(((a % b) - c) << d)`

### A1.0680 — `a | b <= c . d`
- **Source:** `a | b <= c . d`
- **Grouping:** `((a | b) <= (c . d))`

### A1.0681 — `a & b | c >= d`
- **Source:** `a & b | c >= d`
- **Grouping:** `(((a & b) | c) >= d)`

### A1.0682 — `a + b + c >= d`
- **Source:** `a + b + c >= d`
- **Grouping:** `(((a + b) + c) >= d)`

### A1.0683 — `a << b & c == d`
- **Source:** `a << b & c == d`
- **Grouping:** `(((a << b) & c) == d)`

### A1.0684 — `a /= b & c - d`
- **Source:** `a /= b & c - d`
- **Grouping:** `(a /= (b & (c - d)))`

### A1.0685 — `a >> b <= c & d`
- **Source:** `a >> b <= c & d`
- **Grouping:** `((a >> b) <= (c & d))`

### A1.0686 — `a || b - c * d`
- **Source:** `a || b - c * d`
- **Grouping:** `(a || (b - (c * d)))`

### A1.0687 — `a - b - c >= d`
- **Source:** `a - b - c >= d`
- **Grouping:** `(((a - b) - c) >= d)`

### A1.0688 — `a | b >= c < d`
- **Source:** `a | b >= c < d`
- **Grouping:** `(((a | b) >= c) < d)`

### A1.0689 — `a || b == c + d`
- **Source:** `a || b == c + d`
- **Grouping:** `(a || (b == (c + d)))`

### A1.0690 — `a <= b >> c - d`
- **Source:** `a <= b >> c - d`
- **Grouping:** `(a <= (b >> (c - d)))`

### A1.0691 — `a < b / c || d`
- **Source:** `a < b / c || d`
- **Grouping:** `((a < (b / c)) || d)`

### A1.0692 — `a >> b || c || d`
- **Source:** `a >> b || c || d`
- **Grouping:** `(((a >> b) || c) || d)`

### A1.0693 — `a || b < c % d`
- **Source:** `a || b < c % d`
- **Grouping:** `(a || (b < (c % d)))`

### A1.0694 — `a || b >= c && d`
- **Source:** `a || b >= c && d`
- **Grouping:** `(a || ((b >= c) && d))`

### A1.0695 — `a << b . c / d`
- **Source:** `a << b . c / d`
- **Grouping:** `(a << ((b . c) / d))`

### A1.0696 — `a << b && c << d`
- **Source:** `a << b && c << d`
- **Grouping:** `((a << b) && (c << d))`

### A1.0697 — `a << b % c . d`
- **Source:** `a << b % c . d`
- **Grouping:** `(a << (b % (c . d)))`

### A1.0698 — `a >> b > c == d`
- **Source:** `a >> b > c == d`
- **Grouping:** `(((a >> b) > c) == d)`

### A1.0699 — `a % b + c >= d`
- **Source:** `a % b + c >= d`
- **Grouping:** `(((a % b) + c) >= d)`

### A1.0700 — `a - b & c == d`
- **Source:** `a - b & c == d`
- **Grouping:** `(((a - b) & c) == d)`

### A1.0701 — `a >> b < c >= d`
- **Source:** `a >> b < c >= d`
- **Grouping:** `(((a >> b) < c) >= d)`

### A1.0702 — `a <= b * c >= d`
- **Source:** `a <= b * c >= d`
- **Grouping:** `((a <= (b * c)) >= d)`

### A1.0703 — `a > b < c * d`
- **Source:** `a > b < c * d`
- **Grouping:** `((a > b) < (c * d))`

### A1.0704 — `a - b || c || d`
- **Source:** `a - b || c || d`
- **Grouping:** `(((a - b) || c) || d)`

### A1.0705 — `a >> b . c /= d`
- **Source:** `a >> b . c /= d`
- **Grouping:** `((a >> (b . c)) /= d)`

### A1.0706 — `a / b >> c >> d`
- **Source:** `a / b >> c >> d`
- **Grouping:** `(((a / b) >> c) >> d)`

### A1.0707 — `a && b << c / d`
- **Source:** `a && b << c / d`
- **Grouping:** `(a && (b << (c / d)))`

### A1.0708 — `a <= b % c /= d`
- **Source:** `a <= b % c /= d`
- **Grouping:** `((a <= (b % c)) /= d)`

### A1.0709 — `a == b < c ^ d`
- **Source:** `a == b < c ^ d`
- **Grouping:** `((a == b) < (c ^ d))`

### A1.0710 — `a /= b & c < d`
- **Source:** `a /= b & c < d`
- **Grouping:** `((a /= (b & c)) < d)`

### A1.0711 — `a && b + c | d`
- **Source:** `a && b + c | d`
- **Grouping:** `(a && ((b + c) | d))`

### A1.0712 — `a + b / c > d`
- **Source:** `a + b / c > d`
- **Grouping:** `((a + (b / c)) > d)`

### A1.0713 — `a % b | c >= d`
- **Source:** `a % b | c >= d`
- **Grouping:** `(((a % b) | c) >= d)`

### A1.0714 — `a % b + c << d`
- **Source:** `a % b + c << d`
- **Grouping:** `(((a % b) + c) << d)`

### A1.0715 — `a / b <= c / d`
- **Source:** `a / b <= c / d`
- **Grouping:** `((a / b) <= (c / d))`

### A1.0716 — `a / b + c << d`
- **Source:** `a / b + c << d`
- **Grouping:** `(((a / b) + c) << d)`

### A1.0717 — `a <= b >= c + d`
- **Source:** `a <= b >= c + d`
- **Grouping:** `((a <= b) >= (c + d))`

### A1.0718 — `a - b >= c << d`
- **Source:** `a - b >= c << d`
- **Grouping:** `((a - b) >= (c << d))`

### A1.0719 — `a || b . c & d`
- **Source:** `a || b . c & d`
- **Grouping:** `(a || ((b . c) & d))`

### A1.0720 — `a ^ b >= c <= d`
- **Source:** `a ^ b >= c <= d`
- **Grouping:** `(((a ^ b) >= c) <= d)`

### A1.0721 — `a <= b * c >> d`
- **Source:** `a <= b * c >> d`
- **Grouping:** `(a <= ((b * c) >> d))`

### A1.0722 — `a + b | c & d`
- **Source:** `a + b | c & d`
- **Grouping:** `(((a + b) | c) & d)`

### A1.0723 — `a + b / c == d`
- **Source:** `a + b / c == d`
- **Grouping:** `((a + (b / c)) == d)`

### A1.0724 — `a <= b /= c << d`
- **Source:** `a <= b /= c << d`
- **Grouping:** `((a <= b) /= (c << d))`

### A1.0725 — `a << b <= c && d`
- **Source:** `a << b <= c && d`
- **Grouping:** `(((a << b) <= c) && d)`

### A1.0726 — `a /= b == c . d`
- **Source:** `a /= b == c . d`
- **Grouping:** `((a /= b) == (c . d))`

### A1.0727 — `a && b * c . d`
- **Source:** `a && b * c . d`
- **Grouping:** `(a && (b * (c . d)))`

### A1.0728 — `a ^ b <= c & d`
- **Source:** `a ^ b <= c & d`
- **Grouping:** `((a ^ b) <= (c & d))`

### A1.0729 — `a >= b ^ c >= d`
- **Source:** `a >= b ^ c >= d`
- **Grouping:** `((a >= (b ^ c)) >= d)`

### A1.0730 — `a /= b * c . d`
- **Source:** `a /= b * c . d`
- **Grouping:** `(a /= (b * (c . d)))`

### A1.0731 — `a || b ^ c / d`
- **Source:** `a || b ^ c / d`
- **Grouping:** `(a || (b ^ (c / d)))`

### A1.0732 — `a | b / c + d`
- **Source:** `a | b / c + d`
- **Grouping:** `(a | ((b / c) + d))`

### A1.0733 — `a <= b || c >= d`
- **Source:** `a <= b || c >= d`
- **Grouping:** `((a <= b) || (c >= d))`

### A1.0734 — `a || b * c && d`
- **Source:** `a || b * c && d`
- **Grouping:** `(a || ((b * c) && d))`

### A1.0735 — `a & b & c & d`
- **Source:** `a & b & c & d`
- **Grouping:** `(((a & b) & c) & d)`

### A1.0736 — `a <= b ^ c || d`
- **Source:** `a <= b ^ c || d`
- **Grouping:** `((a <= (b ^ c)) || d)`

### A1.0737 — `a /= b ^ c || d`
- **Source:** `a /= b ^ c || d`
- **Grouping:** `((a /= (b ^ c)) || d)`

### A1.0738 — `a . b & c && d`
- **Source:** `a . b & c && d`
- **Grouping:** `(((a . b) & c) && d)`

### A1.0739 — `a <= b ^ c >= d`
- **Source:** `a <= b ^ c >= d`
- **Grouping:** `((a <= (b ^ c)) >= d)`

### A1.0740 — `a || b <= c == d`
- **Source:** `a || b <= c == d`
- **Grouping:** `(a || ((b <= c) == d))`

### A1.0741 — `a * b <= c <= d`
- **Source:** `a * b <= c <= d`
- **Grouping:** `(((a * b) <= c) <= d)`

### A1.0742 — `a || b . c && d`
- **Source:** `a || b . c && d`
- **Grouping:** `(a || ((b . c) && d))`

### A1.0743 — `a < b == c << d`
- **Source:** `a < b == c << d`
- **Grouping:** `((a < b) == (c << d))`

### A1.0744 — `a > b >= c > d`
- **Source:** `a > b >= c > d`
- **Grouping:** `(((a > b) >= c) > d)`

### A1.0745 — `a || b >= c < d`
- **Source:** `a || b >= c < d`
- **Grouping:** `(a || ((b >= c) < d))`

### A1.0746 — `a + b < c / d`
- **Source:** `a + b < c / d`
- **Grouping:** `((a + b) < (c / d))`

### A1.0747 — `a * b * c < d`
- **Source:** `a * b * c < d`
- **Grouping:** `(((a * b) * c) < d)`

### A1.0748 — `a & b || c % d`
- **Source:** `a & b || c % d`
- **Grouping:** `((a & b) || (c % d))`

### A1.0749 — `a - b >> c <= d`
- **Source:** `a - b >> c <= d`
- **Grouping:** `(((a - b) >> c) <= d)`

### A1.0750 — `a > b == c && d`
- **Source:** `a > b == c && d`
- **Grouping:** `(((a > b) == c) && d)`

### A1.0751 — `a == b <= c ^ d`
- **Source:** `a == b <= c ^ d`
- **Grouping:** `((a == b) <= (c ^ d))`

### A1.0752 — `a << b && c - d`
- **Source:** `a << b && c - d`
- **Grouping:** `((a << b) && (c - d))`

### A1.0753 — `a /= b && c < d`
- **Source:** `a /= b && c < d`
- **Grouping:** `((a /= b) && (c < d))`

### A1.0754 — `a + b & c | d`
- **Source:** `a + b & c | d`
- **Grouping:** `(((a + b) & c) | d)`

### A1.0755 — `a < b < c << d`
- **Source:** `a < b < c << d`
- **Grouping:** `((a < b) < (c << d))`

### A1.0756 — `a >= b < c - d`
- **Source:** `a >= b < c - d`
- **Grouping:** `((a >= b) < (c - d))`

### A1.0757 — `a . b / c << d`
- **Source:** `a . b / c << d`
- **Grouping:** `(((a . b) / c) << d)`

### A1.0758 — `a . b - c == d`
- **Source:** `a . b - c == d`
- **Grouping:** `(((a . b) - c) == d)`

### A1.0759 — `a / b && c % d`
- **Source:** `a / b && c % d`
- **Grouping:** `((a / b) && (c % d))`

### A1.0760 — `a /= b ^ c < d`
- **Source:** `a /= b ^ c < d`
- **Grouping:** `((a /= (b ^ c)) < d)`

### A1.0761 — `a || b <= c < d`
- **Source:** `a || b <= c < d`
- **Grouping:** `(a || ((b <= c) < d))`

### A1.0762 — `a / b <= c >= d`
- **Source:** `a / b <= c >= d`
- **Grouping:** `(((a / b) <= c) >= d)`

### A1.0763 — `a | b & c <= d`
- **Source:** `a | b & c <= d`
- **Grouping:** `(((a | b) & c) <= d)`

### A1.0764 — `a . b ^ c <= d`
- **Source:** `a . b ^ c <= d`
- **Grouping:** `(((a . b) ^ c) <= d)`

### A1.0765 — `a | b - c - d`
- **Source:** `a | b - c - d`
- **Grouping:** `(a | ((b - c) - d))`

### A1.0766 — `a * b << c . d`
- **Source:** `a * b << c . d`
- **Grouping:** `((a * b) << (c . d))`

### A1.0767 — `a + b << c << d`
- **Source:** `a + b << c << d`
- **Grouping:** `(((a + b) << c) << d)`

### A1.0768 — `a && b / c * d`
- **Source:** `a && b / c * d`
- **Grouping:** `(a && ((b / c) * d))`

### A1.0769 — `a / b & c . d`
- **Source:** `a / b & c . d`
- **Grouping:** `((a / b) & (c . d))`

### A1.0770 — `a == b < c * d`
- **Source:** `a == b < c * d`
- **Grouping:** `((a == b) < (c * d))`

### A1.0771 — `a | b == c >= d`
- **Source:** `a | b == c >= d`
- **Grouping:** `(((a | b) == c) >= d)`

### A1.0772 — `a <= b * c << d`
- **Source:** `a <= b * c << d`
- **Grouping:** `(a <= ((b * c) << d))`

### A1.0773 — `a % b >> c << d`
- **Source:** `a % b >> c << d`
- **Grouping:** `(((a % b) >> c) << d)`

### A1.0774 — `a % b & c * d`
- **Source:** `a % b & c * d`
- **Grouping:** `((a % b) & (c * d))`

### A1.0775 — `a | b & c == d`
- **Source:** `a | b & c == d`
- **Grouping:** `(((a | b) & c) == d)`

### A1.0776 — `a . b % c >> d`
- **Source:** `a . b % c >> d`
- **Grouping:** `(((a . b) % c) >> d)`

### A1.0777 — `a < b && c % d`
- **Source:** `a < b && c % d`
- **Grouping:** `((a < b) && (c % d))`

### A1.0778 — `a . b / c >= d`
- **Source:** `a . b / c >= d`
- **Grouping:** `(((a . b) / c) >= d)`

### A1.0779 — `a == b + c < d`
- **Source:** `a == b + c < d`
- **Grouping:** `((a == (b + c)) < d)`

### A1.0780 — `a . b & c || d`
- **Source:** `a . b & c || d`
- **Grouping:** `(((a . b) & c) || d)`

### A1.0781 — `a - b + c >= d`
- **Source:** `a - b + c >= d`
- **Grouping:** `(((a - b) + c) >= d)`

### A1.0782 — `a < b && c . d`
- **Source:** `a < b && c . d`
- **Grouping:** `((a < b) && (c . d))`

### A1.0783 — `a / b . c - d`
- **Source:** `a / b . c - d`
- **Grouping:** `((a / (b . c)) - d)`

### A1.0784 — `a <= b . c - d`
- **Source:** `a <= b . c - d`
- **Grouping:** `(a <= ((b . c) - d))`

### A1.0785 — `a * b ^ c || d`
- **Source:** `a * b ^ c || d`
- **Grouping:** `(((a * b) ^ c) || d)`

### A1.0786 — `a - b . c /= d`
- **Source:** `a - b . c /= d`
- **Grouping:** `((a - (b . c)) /= d)`

### A1.0787 — `a & b == c - d`
- **Source:** `a & b == c - d`
- **Grouping:** `((a & b) == (c - d))`

### A1.0788 — `a << b << c | d`
- **Source:** `a << b << c | d`
- **Grouping:** `(((a << b) << c) | d)`

### A1.0789 — `a || b < c ^ d`
- **Source:** `a || b < c ^ d`
- **Grouping:** `(a || (b < (c ^ d)))`

### A1.0790 — `a && b | c >> d`
- **Source:** `a && b | c >> d`
- **Grouping:** `(a && (b | (c >> d)))`

### A1.0791 — `a && b << c && d`
- **Source:** `a && b << c && d`
- **Grouping:** `((a && (b << c)) && d)`

### A1.0792 — `a - b - c || d`
- **Source:** `a - b - c || d`
- **Grouping:** `(((a - b) - c) || d)`

### A1.0793 — `a << b /= c >> d`
- **Source:** `a << b /= c >> d`
- **Grouping:** `((a << b) /= (c >> d))`

### A1.0794 — `a ^ b - c /= d`
- **Source:** `a ^ b - c /= d`
- **Grouping:** `((a ^ (b - c)) /= d)`

### A1.0795 — `a >> b /= c ^ d`
- **Source:** `a >> b /= c ^ d`
- **Grouping:** `((a >> b) /= (c ^ d))`

### A1.0796 — `a - b /= c /= d`
- **Source:** `a - b /= c /= d`
- **Grouping:** `(((a - b) /= c) /= d)`

### A1.0797 — `a * b & c > d`
- **Source:** `a * b & c > d`
- **Grouping:** `(((a * b) & c) > d)`

### A1.0798 — `a < b == c . d`
- **Source:** `a < b == c . d`
- **Grouping:** `((a < b) == (c . d))`

### A1.0799 — `a >> b * c * d`
- **Source:** `a >> b * c * d`
- **Grouping:** `(a >> ((b * c) * d))`

### A1.0800 — `a <= b >= c && d`
- **Source:** `a <= b >= c && d`
- **Grouping:** `(((a <= b) >= c) && d)`

### A1.0801 — `a . b >= c % d`
- **Source:** `a . b >= c % d`
- **Grouping:** `((a . b) >= (c % d))`

### A1.0802 — `a - b . c % d`
- **Source:** `a - b . c % d`
- **Grouping:** `(a - ((b . c) % d))`

### A1.0803 — `a % b >= c % d`
- **Source:** `a % b >= c % d`
- **Grouping:** `((a % b) >= (c % d))`

### A1.0804 — `a >> b | c < d`
- **Source:** `a >> b | c < d`
- **Grouping:** `(((a >> b) | c) < d)`

### A1.0805 — `a >= b + c >> d`
- **Source:** `a >= b + c >> d`
- **Grouping:** `(a >= ((b + c) >> d))`

### A1.0806 — `a / b /= c << d`
- **Source:** `a / b /= c << d`
- **Grouping:** `((a / b) /= (c << d))`

### A1.0807 — `a * b <= c / d`
- **Source:** `a * b <= c / d`
- **Grouping:** `((a * b) <= (c / d))`

### A1.0808 — `a == b - c / d`
- **Source:** `a == b - c / d`
- **Grouping:** `(a == (b - (c / d)))`

### A1.0809 — `a == b && c . d`
- **Source:** `a == b && c . d`
- **Grouping:** `((a == b) && (c . d))`

### A1.0810 — `a /= b ^ c && d`
- **Source:** `a /= b ^ c && d`
- **Grouping:** `((a /= (b ^ c)) && d)`

### A1.0811 — `a < b < c >= d`
- **Source:** `a < b < c >= d`
- **Grouping:** `(((a < b) < c) >= d)`

### A1.0812 — `a && b == c + d`
- **Source:** `a && b == c + d`
- **Grouping:** `(a && (b == (c + d)))`

### A1.0813 — `a << b + c * d`
- **Source:** `a << b + c * d`
- **Grouping:** `(a << (b + (c * d)))`

### A1.0814 — `a / b - c < d`
- **Source:** `a / b - c < d`
- **Grouping:** `(((a / b) - c) < d)`

### A1.0815 — `a /= b >> c == d`
- **Source:** `a /= b >> c == d`
- **Grouping:** `((a /= (b >> c)) == d)`

### A1.0816 — `a << b & c | d`
- **Source:** `a << b & c | d`
- **Grouping:** `(((a << b) & c) | d)`

### A1.0817 — `a > b % c . d`
- **Source:** `a > b % c . d`
- **Grouping:** `(a > (b % (c . d)))`

### A1.0818 — `a <= b == c /= d`
- **Source:** `a <= b == c /= d`
- **Grouping:** `(((a <= b) == c) /= d)`

### A1.0819 — `a + b < c /= d`
- **Source:** `a + b < c /= d`
- **Grouping:** `(((a + b) < c) /= d)`

### A1.0820 — `a > b && c >> d`
- **Source:** `a > b && c >> d`
- **Grouping:** `((a > b) && (c >> d))`

### A1.0821 — `a && b == c * d`
- **Source:** `a && b == c * d`
- **Grouping:** `(a && (b == (c * d)))`

### A1.0822 — `a & b /= c == d`
- **Source:** `a & b /= c == d`
- **Grouping:** `(((a & b) /= c) == d)`

### A1.0823 — `a >> b || c . d`
- **Source:** `a >> b || c . d`
- **Grouping:** `((a >> b) || (c . d))`

### A1.0824 — `a + b > c < d`
- **Source:** `a + b > c < d`
- **Grouping:** `(((a + b) > c) < d)`

### A1.0825 — `a == b >= c || d`
- **Source:** `a == b >= c || d`
- **Grouping:** `(((a == b) >= c) || d)`

### A1.0826 — `a && b && c || d`
- **Source:** `a && b && c || d`
- **Grouping:** `(((a && b) && c) || d)`

### A1.0827 — `a || b & c . d`
- **Source:** `a || b & c . d`
- **Grouping:** `(a || (b & (c . d)))`

### A1.0828 — `a << b >> c >> d`
- **Source:** `a << b >> c >> d`
- **Grouping:** `(((a << b) >> c) >> d)`

### A1.0829 — `a + b + c /= d`
- **Source:** `a + b + c /= d`
- **Grouping:** `(((a + b) + c) /= d)`

### A1.0830 — `a / b /= c | d`
- **Source:** `a / b /= c | d`
- **Grouping:** `((a / b) /= (c | d))`

### A1.0831 — `a << b + c + d`
- **Source:** `a << b + c + d`
- **Grouping:** `(a << ((b + c) + d))`

### A1.0832 — `a | b | c ^ d`
- **Source:** `a | b | c ^ d`
- **Grouping:** `(((a | b) | c) ^ d)`

### A1.0833 — `a || b * c | d`
- **Source:** `a || b * c | d`
- **Grouping:** `(a || ((b * c) | d))`

### A1.0834 — `a << b + c << d`
- **Source:** `a << b + c << d`
- **Grouping:** `((a << (b + c)) << d)`

### A1.0835 — `a <= b & c % d`
- **Source:** `a <= b & c % d`
- **Grouping:** `(a <= (b & (c % d)))`

### A1.0836 — `a /= b - c > d`
- **Source:** `a /= b - c > d`
- **Grouping:** `((a /= (b - c)) > d)`

### A1.0837 — `a > b < c || d`
- **Source:** `a > b < c || d`
- **Grouping:** `(((a > b) < c) || d)`

### A1.0838 — `a . b == c | d`
- **Source:** `a . b == c | d`
- **Grouping:** `((a . b) == (c | d))`

### A1.0839 — `a == b + c . d`
- **Source:** `a == b + c . d`
- **Grouping:** `(a == (b + (c . d)))`

### A1.0840 — `a | b / c || d`
- **Source:** `a | b / c || d`
- **Grouping:** `((a | (b / c)) || d)`

### A1.0841 — `a | b && c >> d`
- **Source:** `a | b && c >> d`
- **Grouping:** `((a | b) && (c >> d))`

### A1.0842 — `a ^ b << c >= d`
- **Source:** `a ^ b << c >= d`
- **Grouping:** `((a ^ (b << c)) >= d)`

### A1.0843 — `a /= b + c && d`
- **Source:** `a /= b + c && d`
- **Grouping:** `((a /= (b + c)) && d)`

### A1.0844 — `a + b * c >> d`
- **Source:** `a + b * c >> d`
- **Grouping:** `((a + (b * c)) >> d)`

### A1.0845 — `a + b == c % d`
- **Source:** `a + b == c % d`
- **Grouping:** `((a + b) == (c % d))`

### A1.0846 — `a ^ b . c && d`
- **Source:** `a ^ b . c && d`
- **Grouping:** `((a ^ (b . c)) && d)`

### A1.0847 — `a || b /= c /= d`
- **Source:** `a || b /= c /= d`
- **Grouping:** `(a || ((b /= c) /= d))`

### A1.0848 — `a ^ b + c || d`
- **Source:** `a ^ b + c || d`
- **Grouping:** `((a ^ (b + c)) || d)`

### A1.0849 — `a > b + c & d`
- **Source:** `a > b + c & d`
- **Grouping:** `(a > ((b + c) & d))`

### A1.0850 — `a & b /= c / d`
- **Source:** `a & b /= c / d`
- **Grouping:** `((a & b) /= (c / d))`

### A1.0851 — `a ^ b && c || d`
- **Source:** `a ^ b && c || d`
- **Grouping:** `(((a ^ b) && c) || d)`

### A1.0852 — `a < b ^ c /= d`
- **Source:** `a < b ^ c /= d`
- **Grouping:** `((a < (b ^ c)) /= d)`

### A1.0853 — `a <= b % c >> d`
- **Source:** `a <= b % c >> d`
- **Grouping:** `(a <= ((b % c) >> d))`

### A1.0854 — `a << b - c >= d`
- **Source:** `a << b - c >= d`
- **Grouping:** `((a << (b - c)) >= d)`

### A1.0855 — `a . b | c /= d`
- **Source:** `a . b | c /= d`
- **Grouping:** `(((a . b) | c) /= d)`

### A1.0856 — `a && b ^ c ^ d`
- **Source:** `a && b ^ c ^ d`
- **Grouping:** `(a && ((b ^ c) ^ d))`

### A1.0857 — `a + b - c * d`
- **Source:** `a + b - c * d`
- **Grouping:** `((a + b) - (c * d))`

### A1.0858 — `a ^ b >> c > d`
- **Source:** `a ^ b >> c > d`
- **Grouping:** `((a ^ (b >> c)) > d)`

### A1.0859 — `a && b /= c >> d`
- **Source:** `a && b /= c >> d`
- **Grouping:** `(a && (b /= (c >> d)))`

### A1.0860 — `a > b << c - d`
- **Source:** `a > b << c - d`
- **Grouping:** `(a > (b << (c - d)))`

### A1.0861 — `a >= b << c / d`
- **Source:** `a >= b << c / d`
- **Grouping:** `(a >= (b << (c / d)))`

### A1.0862 — `a - b >> c && d`
- **Source:** `a - b >> c && d`
- **Grouping:** `(((a - b) >> c) && d)`

### A1.0863 — `a < b * c / d`
- **Source:** `a < b * c / d`
- **Grouping:** `(a < ((b * c) / d))`

### A1.0864 — `a * b / c | d`
- **Source:** `a * b / c | d`
- **Grouping:** `(((a * b) / c) | d)`

### A1.0865 — `a | b . c - d`
- **Source:** `a | b . c - d`
- **Grouping:** `(a | ((b . c) - d))`

### A1.0866 — `a % b || c || d`
- **Source:** `a % b || c || d`
- **Grouping:** `(((a % b) || c) || d)`

### A1.0867 — `a - b >> c ^ d`
- **Source:** `a - b >> c ^ d`
- **Grouping:** `(((a - b) >> c) ^ d)`

### A1.0868 — `a - b * c ^ d`
- **Source:** `a - b * c ^ d`
- **Grouping:** `((a - (b * c)) ^ d)`

### A1.0869 — `a + b >> c == d`
- **Source:** `a + b >> c == d`
- **Grouping:** `(((a + b) >> c) == d)`

### A1.0870 — `a & b / c || d`
- **Source:** `a & b / c || d`
- **Grouping:** `((a & (b / c)) || d)`

### A1.0871 — `a && b >> c || d`
- **Source:** `a && b >> c || d`
- **Grouping:** `((a && (b >> c)) || d)`

### A1.0872 — `a . b % c /= d`
- **Source:** `a . b % c /= d`
- **Grouping:** `(((a . b) % c) /= d)`

### A1.0873 — `a << b % c % d`
- **Source:** `a << b % c % d`
- **Grouping:** `(a << ((b % c) % d))`

### A1.0874 — `a /= b << c && d`
- **Source:** `a /= b << c && d`
- **Grouping:** `((a /= (b << c)) && d)`

### A1.0875 — `a << b >> c > d`
- **Source:** `a << b >> c > d`
- **Grouping:** `(((a << b) >> c) > d)`

### A1.0876 — `a || b /= c & d`
- **Source:** `a || b /= c & d`
- **Grouping:** `(a || (b /= (c & d)))`

### A1.0877 — `a | b >= c > d`
- **Source:** `a | b >= c > d`
- **Grouping:** `(((a | b) >= c) > d)`

### A1.0878 — `a * b . c /= d`
- **Source:** `a * b . c /= d`
- **Grouping:** `((a * (b . c)) /= d)`

### A1.0879 — `a >> b < c == d`
- **Source:** `a >> b < c == d`
- **Grouping:** `(((a >> b) < c) == d)`

### A1.0880 — `a + b << c /= d`
- **Source:** `a + b << c /= d`
- **Grouping:** `(((a + b) << c) /= d)`

### A1.0881 — `a <= b ^ c == d`
- **Source:** `a <= b ^ c == d`
- **Grouping:** `((a <= (b ^ c)) == d)`

### A1.0882 — `a || b >= c - d`
- **Source:** `a || b >= c - d`
- **Grouping:** `(a || (b >= (c - d)))`

### A1.0883 — `a << b && c < d`
- **Source:** `a << b && c < d`
- **Grouping:** `((a << b) && (c < d))`

### A1.0884 — `a < b >> c - d`
- **Source:** `a < b >> c - d`
- **Grouping:** `(a < (b >> (c - d)))`

### A1.0885 — `a ^ b == c == d`
- **Source:** `a ^ b == c == d`
- **Grouping:** `(((a ^ b) == c) == d)`

### A1.0886 — `a >= b >> c | d`
- **Source:** `a >= b >> c | d`
- **Grouping:** `(a >= ((b >> c) | d))`

### A1.0887 — `a >> b - c >= d`
- **Source:** `a >> b - c >= d`
- **Grouping:** `((a >> (b - c)) >= d)`

### A1.0888 — `a <= b | c /= d`
- **Source:** `a <= b | c /= d`
- **Grouping:** `((a <= (b | c)) /= d)`

### A1.0889 — `a & b % c % d`
- **Source:** `a & b % c % d`
- **Grouping:** `(a & ((b % c) % d))`

### A1.0890 — `a <= b < c > d`
- **Source:** `a <= b < c > d`
- **Grouping:** `(((a <= b) < c) > d)`

### A1.0891 — `a > b . c & d`
- **Source:** `a > b . c & d`
- **Grouping:** `(a > ((b . c) & d))`

### A1.0892 — `a * b >> c ^ d`
- **Source:** `a * b >> c ^ d`
- **Grouping:** `(((a * b) >> c) ^ d)`

### A1.0893 — `a * b /= c ^ d`
- **Source:** `a * b /= c ^ d`
- **Grouping:** `((a * b) /= (c ^ d))`

### A1.0894 — `a /= b && c | d`
- **Source:** `a /= b && c | d`
- **Grouping:** `((a /= b) && (c | d))`

### A1.0895 — `a ^ b <= c && d`
- **Source:** `a ^ b <= c && d`
- **Grouping:** `(((a ^ b) <= c) && d)`

### A1.0896 — `a | b ^ c & d`
- **Source:** `a | b ^ c & d`
- **Grouping:** `(((a | b) ^ c) & d)`

### A1.0897 — `a > b > c . d`
- **Source:** `a > b > c . d`
- **Grouping:** `((a > b) > (c . d))`

### A1.0898 — `a < b * c /= d`
- **Source:** `a < b * c /= d`
- **Grouping:** `((a < (b * c)) /= d)`

### A1.0899 — `a * b | c < d`
- **Source:** `a * b | c < d`
- **Grouping:** `(((a * b) | c) < d)`

### A1.0900 — `a % b == c >= d`
- **Source:** `a % b == c >= d`
- **Grouping:** `(((a % b) == c) >= d)`

### A1.0901 — `a * b <= c >= d`
- **Source:** `a * b <= c >= d`
- **Grouping:** `(((a * b) <= c) >= d)`

### A1.0902 — `a == b | c - d`
- **Source:** `a == b | c - d`
- **Grouping:** `(a == (b | (c - d)))`

### A1.0903 — `a . b % c || d`
- **Source:** `a . b % c || d`
- **Grouping:** `(((a . b) % c) || d)`

### A1.0904 — `a & b + c < d`
- **Source:** `a & b + c < d`
- **Grouping:** `((a & (b + c)) < d)`

### A1.0905 — `a << b / c | d`
- **Source:** `a << b / c | d`
- **Grouping:** `((a << (b / c)) | d)`

### A1.0906 — `a & b << c < d`
- **Source:** `a & b << c < d`
- **Grouping:** `((a & (b << c)) < d)`

### A1.0907 — `a == b + c || d`
- **Source:** `a == b + c || d`
- **Grouping:** `((a == (b + c)) || d)`

### A1.0908 — `a . b >= c || d`
- **Source:** `a . b >= c || d`
- **Grouping:** `(((a . b) >= c) || d)`

### A1.0909 — `a && b << c < d`
- **Source:** `a && b << c < d`
- **Grouping:** `(a && ((b << c) < d))`

### A1.0910 — `a ^ b /= c + d`
- **Source:** `a ^ b /= c + d`
- **Grouping:** `((a ^ b) /= (c + d))`

### A1.0911 — `a . b < c | d`
- **Source:** `a . b < c | d`
- **Grouping:** `((a . b) < (c | d))`

### A1.0912 — `a >= b == c < d`
- **Source:** `a >= b == c < d`
- **Grouping:** `(((a >= b) == c) < d)`

### A1.0913 — `a . b <= c | d`
- **Source:** `a . b <= c | d`
- **Grouping:** `((a . b) <= (c | d))`

### A1.0914 — `a - b == c . d`
- **Source:** `a - b == c . d`
- **Grouping:** `((a - b) == (c . d))`

### A1.0915 — `a - b || c == d`
- **Source:** `a - b || c == d`
- **Grouping:** `((a - b) || (c == d))`

### A1.0916 — `a && b > c >> d`
- **Source:** `a && b > c >> d`
- **Grouping:** `(a && (b > (c >> d)))`

### A1.0917 — `a ^ b | c + d`
- **Source:** `a ^ b | c + d`
- **Grouping:** `((a ^ b) | (c + d))`

### A1.0918 — `a >= b & c & d`
- **Source:** `a >= b & c & d`
- **Grouping:** `(a >= ((b & c) & d))`

### A1.0919 — `a /= b <= c || d`
- **Source:** `a /= b <= c || d`
- **Grouping:** `(((a /= b) <= c) || d)`

### A1.0920 — `a >= b && c / d`
- **Source:** `a >= b && c / d`
- **Grouping:** `((a >= b) && (c / d))`

### A1.0921 — `a || b < c && d`
- **Source:** `a || b < c && d`
- **Grouping:** `(a || ((b < c) && d))`

### A1.0922 — `a /= b >> c || d`
- **Source:** `a /= b >> c || d`
- **Grouping:** `((a /= (b >> c)) || d)`

### A1.0923 — `a && b ^ c * d`
- **Source:** `a && b ^ c * d`
- **Grouping:** `(a && (b ^ (c * d)))`

### A1.0924 — `a - b & c . d`
- **Source:** `a - b & c . d`
- **Grouping:** `((a - b) & (c . d))`

### A1.0925 — `a == b == c / d`
- **Source:** `a == b == c / d`
- **Grouping:** `((a == b) == (c / d))`

### A1.0926 — `a >= b > c | d`
- **Source:** `a >= b > c | d`
- **Grouping:** `((a >= b) > (c | d))`

### A1.0927 — `a == b . c >> d`
- **Source:** `a == b . c >> d`
- **Grouping:** `(a == ((b . c) >> d))`

### A1.0928 — `a - b /= c <= d`
- **Source:** `a - b /= c <= d`
- **Grouping:** `(((a - b) /= c) <= d)`

### A1.0929 — `a % b - c <= d`
- **Source:** `a % b - c <= d`
- **Grouping:** `(((a % b) - c) <= d)`

### A1.0930 — `a >> b == c && d`
- **Source:** `a >> b == c && d`
- **Grouping:** `(((a >> b) == c) && d)`

### A1.0931 — `a >= b /= c << d`
- **Source:** `a >= b /= c << d`
- **Grouping:** `((a >= b) /= (c << d))`

### A1.0932 — `a == b /= c /= d`
- **Source:** `a == b /= c /= d`
- **Grouping:** `(((a == b) /= c) /= d)`

### A1.0933 — `a || b - c | d`
- **Source:** `a || b - c | d`
- **Grouping:** `(a || ((b - c) | d))`

### A1.0934 — `a > b > c == d`
- **Source:** `a > b > c == d`
- **Grouping:** `(((a > b) > c) == d)`

### A1.0935 — `a / b >> c + d`
- **Source:** `a / b >> c + d`
- **Grouping:** `((a / b) >> (c + d))`

### A1.0936 — `a ^ b == c >= d`
- **Source:** `a ^ b == c >= d`
- **Grouping:** `(((a ^ b) == c) >= d)`

### A1.0937 — `a >= b | c / d`
- **Source:** `a >= b | c / d`
- **Grouping:** `(a >= (b | (c / d)))`

### A1.0938 — `a && b || c <= d`
- **Source:** `a && b || c <= d`
- **Grouping:** `((a && b) || (c <= d))`

### A1.0939 — `a / b / c % d`
- **Source:** `a / b / c % d`
- **Grouping:** `(((a / b) / c) % d)`

### A1.0940 — `a > b == c % d`
- **Source:** `a > b == c % d`
- **Grouping:** `((a > b) == (c % d))`

### A1.0941 — `a >> b | c && d`
- **Source:** `a >> b | c && d`
- **Grouping:** `(((a >> b) | c) && d)`

### A1.0942 — `a >= b + c . d`
- **Source:** `a >= b + c . d`
- **Grouping:** `(a >= (b + (c . d)))`

### A1.0943 — `a <= b && c >= d`
- **Source:** `a <= b && c >= d`
- **Grouping:** `((a <= b) && (c >= d))`

### A1.0944 — `a . b / c * d`
- **Source:** `a . b / c * d`
- **Grouping:** `(((a . b) / c) * d)`

### A1.0945 — `a - b <= c & d`
- **Source:** `a - b <= c & d`
- **Grouping:** `((a - b) <= (c & d))`

### A1.0946 — `a > b . c % d`
- **Source:** `a > b . c % d`
- **Grouping:** `(a > ((b . c) % d))`

### A1.0947 — `a << b % c == d`
- **Source:** `a << b % c == d`
- **Grouping:** `((a << (b % c)) == d)`

### A1.0948 — `a | b << c < d`
- **Source:** `a | b << c < d`
- **Grouping:** `((a | (b << c)) < d)`

### A1.0949 — `a < b == c - d`
- **Source:** `a < b == c - d`
- **Grouping:** `((a < b) == (c - d))`

### A1.0950 — `a << b * c - d`
- **Source:** `a << b * c - d`
- **Grouping:** `(a << ((b * c) - d))`

### A1.0951 — `a - b /= c >= d`
- **Source:** `a - b /= c >= d`
- **Grouping:** `(((a - b) /= c) >= d)`

### A1.0952 — `a << b || c & d`
- **Source:** `a << b || c & d`
- **Grouping:** `((a << b) || (c & d))`

### A1.0953 — `a << b << c || d`
- **Source:** `a << b << c || d`
- **Grouping:** `(((a << b) << c) || d)`

### A1.0954 — `a >> b ^ c /= d`
- **Source:** `a >> b ^ c /= d`
- **Grouping:** `(((a >> b) ^ c) /= d)`

### A1.0955 — `a ^ b ^ c && d`
- **Source:** `a ^ b ^ c && d`
- **Grouping:** `(((a ^ b) ^ c) && d)`

### A1.0956 — `a <= b << c / d`
- **Source:** `a <= b << c / d`
- **Grouping:** `(a <= (b << (c / d)))`

### A1.0957 — `a < b << c /= d`
- **Source:** `a < b << c /= d`
- **Grouping:** `((a < (b << c)) /= d)`

### A1.0958 — `a . b >> c * d`
- **Source:** `a . b >> c * d`
- **Grouping:** `((a . b) >> (c * d))`

### A1.0959 — `a >> b == c > d`
- **Source:** `a >> b == c > d`
- **Grouping:** `(((a >> b) == c) > d)`

### A1.0960 — `a << b / c >= d`
- **Source:** `a << b / c >= d`
- **Grouping:** `((a << (b / c)) >= d)`

### A1.0961 — `a >= b <= c /= d`
- **Source:** `a >= b <= c /= d`
- **Grouping:** `(((a >= b) <= c) /= d)`

### A1.0962 — `a << b < c || d`
- **Source:** `a << b < c || d`
- **Grouping:** `(((a << b) < c) || d)`

### A1.0963 — `a & b < c . d`
- **Source:** `a & b < c . d`
- **Grouping:** `((a & b) < (c . d))`

### A1.0964 — `a == b + c == d`
- **Source:** `a == b + c == d`
- **Grouping:** `((a == (b + c)) == d)`

### A1.0965 — `a >> b ^ c && d`
- **Source:** `a >> b ^ c && d`
- **Grouping:** `(((a >> b) ^ c) && d)`

### A1.0966 — `a - b <= c - d`
- **Source:** `a - b <= c - d`
- **Grouping:** `((a - b) <= (c - d))`

### A1.0967 — `a /= b <= c /= d`
- **Source:** `a /= b <= c /= d`
- **Grouping:** `(((a /= b) <= c) /= d)`

### A1.0968 — `a - b . c <= d`
- **Source:** `a - b . c <= d`
- **Grouping:** `((a - (b . c)) <= d)`

### A1.0969 — `a & b && c || d`
- **Source:** `a & b && c || d`
- **Grouping:** `(((a & b) && c) || d)`

### A1.0970 — `a + b >= c /= d`
- **Source:** `a + b >= c /= d`
- **Grouping:** `(((a + b) >= c) /= d)`

### A1.0971 — `a > b . c | d`
- **Source:** `a > b . c | d`
- **Grouping:** `(a > ((b . c) | d))`

### A1.0972 — `a > b | c / d`
- **Source:** `a > b | c / d`
- **Grouping:** `(a > (b | (c / d)))`

### A1.0973 — `a /= b && c & d`
- **Source:** `a /= b && c & d`
- **Grouping:** `((a /= b) && (c & d))`

### A1.0974 — `a < b % c & d`
- **Source:** `a < b % c & d`
- **Grouping:** `(a < ((b % c) & d))`

### A1.0975 — `a & b % c + d`
- **Source:** `a & b % c + d`
- **Grouping:** `(a & ((b % c) + d))`

### A1.0976 — `a /= b ^ c - d`
- **Source:** `a /= b ^ c - d`
- **Grouping:** `(a /= (b ^ (c - d)))`

### A1.0977 — `a >= b & c * d`
- **Source:** `a >= b & c * d`
- **Grouping:** `(a >= (b & (c * d)))`

### A1.0978 — `a ^ b . c * d`
- **Source:** `a ^ b . c * d`
- **Grouping:** `(a ^ ((b . c) * d))`

### A1.0979 — `a == b > c * d`
- **Source:** `a == b > c * d`
- **Grouping:** `((a == b) > (c * d))`

### A1.0980 — `a * b && c > d`
- **Source:** `a * b && c > d`
- **Grouping:** `((a * b) && (c > d))`

### A1.0981 — `a . b . c / d`
- **Source:** `a . b . c / d`
- **Grouping:** `((a . (b . c)) / d)`

### A1.0982 — `a <= b | c * d`
- **Source:** `a <= b | c * d`
- **Grouping:** `(a <= (b | (c * d)))`

### A1.0983 — `a | b >> c >> d`
- **Source:** `a | b >> c >> d`
- **Grouping:** `(a | ((b >> c) >> d))`

### A1.0984 — `a ^ b * c & d`
- **Source:** `a ^ b * c & d`
- **Grouping:** `((a ^ (b * c)) & d)`

### A1.0985 — `a . b % c << d`
- **Source:** `a . b % c << d`
- **Grouping:** `(((a . b) % c) << d)`

### A1.0986 — `a >> b /= c >= d`
- **Source:** `a >> b /= c >= d`
- **Grouping:** `(((a >> b) /= c) >= d)`

### A1.0987 — `a * b % c % d`
- **Source:** `a * b % c % d`
- **Grouping:** `(((a * b) % c) % d)`

### A1.0988 — `a % b < c && d`
- **Source:** `a % b < c && d`
- **Grouping:** `(((a % b) < c) && d)`

### A1.0989 — `a & b | c / d`
- **Source:** `a & b | c / d`
- **Grouping:** `((a & b) | (c / d))`

### A1.0990 — `a > b . c << d`
- **Source:** `a > b . c << d`
- **Grouping:** `(a > ((b . c) << d))`

### A1.0991 — `a & b < c % d`
- **Source:** `a & b < c % d`
- **Grouping:** `((a & b) < (c % d))`

### A1.0992 — `a - b + c <= d`
- **Source:** `a - b + c <= d`
- **Grouping:** `(((a - b) + c) <= d)`

### A1.0993 — `a == b - c < d`
- **Source:** `a == b - c < d`
- **Grouping:** `((a == (b - c)) < d)`

### A1.0994 — `a ^ b * c < d`
- **Source:** `a ^ b * c < d`
- **Grouping:** `((a ^ (b * c)) < d)`

### A1.0995 — `a && b > c + d`
- **Source:** `a && b > c + d`
- **Grouping:** `(a && (b > (c + d)))`

### A1.0996 — `a && b && c . d`
- **Source:** `a && b && c . d`
- **Grouping:** `((a && b) && (c . d))`

### A1.0997 — `a /= b | c / d`
- **Source:** `a /= b | c / d`
- **Grouping:** `(a /= (b | (c / d)))`

### A1.0998 — `a || b < c >= d`
- **Source:** `a || b < c >= d`
- **Grouping:** `(a || ((b < c) >= d))`

### A1.0999 — `a /= b % c . d`
- **Source:** `a /= b % c . d`
- **Grouping:** `(a /= (b % (c . d)))`

### A1.1000 — `a << b < c . d`
- **Source:** `a << b < c . d`
- **Grouping:** `((a << b) < (c . d))`

### A1.1001 — `a / b > c . d`
- **Source:** `a / b > c . d`
- **Grouping:** `((a / b) > (c . d))`

### A1.1002 — `a <= b << c ^ d`
- **Source:** `a <= b << c ^ d`
- **Grouping:** `(a <= ((b << c) ^ d))`

### A1.1003 — `a >> b + c > d`
- **Source:** `a >> b + c > d`
- **Grouping:** `((a >> (b + c)) > d)`

### A1.1004 — `a /= b - c == d`
- **Source:** `a /= b - c == d`
- **Grouping:** `((a /= (b - c)) == d)`

### A1.1005 — `a <= b % c < d`
- **Source:** `a <= b % c < d`
- **Grouping:** `((a <= (b % c)) < d)`

### A1.1006 — `a == b == c << d`
- **Source:** `a == b == c << d`
- **Grouping:** `((a == b) == (c << d))`

### A1.1007 — `a <= b <= c - d`
- **Source:** `a <= b <= c - d`
- **Grouping:** `((a <= b) <= (c - d))`

### A1.1008 — `a % b / c <= d`
- **Source:** `a % b / c <= d`
- **Grouping:** `(((a % b) / c) <= d)`

### A1.1009 — `a % b - c ^ d`
- **Source:** `a % b - c ^ d`
- **Grouping:** `(((a % b) - c) ^ d)`

### A1.1010 — `a / b && c + d`
- **Source:** `a / b && c + d`
- **Grouping:** `((a / b) && (c + d))`

### A1.1011 — `a / b + c >= d`
- **Source:** `a / b + c >= d`
- **Grouping:** `(((a / b) + c) >= d)`

### A1.1012 — `a % b == c + d`
- **Source:** `a % b == c + d`
- **Grouping:** `((a % b) == (c + d))`

### A1.1013 — `a % b * c << d`
- **Source:** `a % b * c << d`
- **Grouping:** `(((a % b) * c) << d)`

### A1.1014 — `a >= b << c . d`
- **Source:** `a >= b << c . d`
- **Grouping:** `(a >= (b << (c . d)))`

### A1.1015 — `a * b >> c /= d`
- **Source:** `a * b >> c /= d`
- **Grouping:** `(((a * b) >> c) /= d)`

### A1.1016 — `a % b < c & d`
- **Source:** `a % b < c & d`
- **Grouping:** `((a % b) < (c & d))`

### A1.1017 — `a > b <= c + d`
- **Source:** `a > b <= c + d`
- **Grouping:** `((a > b) <= (c + d))`

### A1.1018 — `a * b /= c . d`
- **Source:** `a * b /= c . d`
- **Grouping:** `((a * b) /= (c . d))`

### A1.1019 — `a + b & c > d`
- **Source:** `a + b & c > d`
- **Grouping:** `(((a + b) & c) > d)`

### A1.1020 — `a | b . c || d`
- **Source:** `a | b . c || d`
- **Grouping:** `((a | (b . c)) || d)`

### A1.1021 — `a & b && c == d`
- **Source:** `a & b && c == d`
- **Grouping:** `((a & b) && (c == d))`

### A1.1022 — `a <= b << c < d`
- **Source:** `a <= b << c < d`
- **Grouping:** `((a <= (b << c)) < d)`

### A1.1023 — `a <= b - c > d`
- **Source:** `a <= b - c > d`
- **Grouping:** `((a <= (b - c)) > d)`

### A1.1024 — `a / b + c | d`
- **Source:** `a / b + c | d`
- **Grouping:** `(((a / b) + c) | d)`

### A1.1025 — `a ^ b || c + d`
- **Source:** `a ^ b || c + d`
- **Grouping:** `((a ^ b) || (c + d))`

### A1.1026 — `a /= b % c == d`
- **Source:** `a /= b % c == d`
- **Grouping:** `((a /= (b % c)) == d)`

### A1.1027 — `a < b == c > d`
- **Source:** `a < b == c > d`
- **Grouping:** `(((a < b) == c) > d)`

### A1.1028 — `a % b <= c || d`
- **Source:** `a % b <= c || d`
- **Grouping:** `(((a % b) <= c) || d)`

### A1.1029 — `a - b << c >> d`
- **Source:** `a - b << c >> d`
- **Grouping:** `(((a - b) << c) >> d)`

### A1.1030 — `a >= b >= c - d`
- **Source:** `a >= b >= c - d`
- **Grouping:** `((a >= b) >= (c - d))`

### A1.1031 — `a > b ^ c % d`
- **Source:** `a > b ^ c % d`
- **Grouping:** `(a > (b ^ (c % d)))`

### A1.1032 — `a || b << c && d`
- **Source:** `a || b << c && d`
- **Grouping:** `(a || ((b << c) && d))`

### A1.1033 — `a ^ b * c | d`
- **Source:** `a ^ b * c | d`
- **Grouping:** `((a ^ (b * c)) | d)`

### A1.1034 — `a == b % c / d`
- **Source:** `a == b % c / d`
- **Grouping:** `(a == ((b % c) / d))`

### A1.1035 — `a * b < c <= d`
- **Source:** `a * b < c <= d`
- **Grouping:** `(((a * b) < c) <= d)`

### A1.1036 — `a || b - c ^ d`
- **Source:** `a || b - c ^ d`
- **Grouping:** `(a || ((b - c) ^ d))`

### A1.1037 — `a >> b && c * d`
- **Source:** `a >> b && c * d`
- **Grouping:** `((a >> b) && (c * d))`

### A1.1038 — `a && b . c == d`
- **Source:** `a && b . c == d`
- **Grouping:** `(a && ((b . c) == d))`

### A1.1039 — `a >> b ^ c >> d`
- **Source:** `a >> b ^ c >> d`
- **Grouping:** `((a >> b) ^ (c >> d))`

### A1.1040 — `a || b . c <= d`
- **Source:** `a || b . c <= d`
- **Grouping:** `(a || ((b . c) <= d))`

### A1.1041 — `a + b - c <= d`
- **Source:** `a + b - c <= d`
- **Grouping:** `(((a + b) - c) <= d)`

### A1.1042 — `a | b + c < d`
- **Source:** `a | b + c < d`
- **Grouping:** `((a | (b + c)) < d)`

### A1.1043 — `a < b > c / d`
- **Source:** `a < b > c / d`
- **Grouping:** `((a < b) > (c / d))`

### A1.1044 — `a + b < c ^ d`
- **Source:** `a + b < c ^ d`
- **Grouping:** `((a + b) < (c ^ d))`

### A1.1045 — `a - b << c ^ d`
- **Source:** `a - b << c ^ d`
- **Grouping:** `(((a - b) << c) ^ d)`

### A1.1046 — `a == b == c < d`
- **Source:** `a == b == c < d`
- **Grouping:** `(((a == b) == c) < d)`

### A1.1047 — `a >> b >= c - d`
- **Source:** `a >> b >= c - d`
- **Grouping:** `((a >> b) >= (c - d))`

### A1.1048 — `a >> b % c . d`
- **Source:** `a >> b % c . d`
- **Grouping:** `(a >> (b % (c . d)))`

### A1.1049 — `a || b >> c << d`
- **Source:** `a || b >> c << d`
- **Grouping:** `(a || ((b >> c) << d))`

### A1.1050 — `a < b == c | d`
- **Source:** `a < b == c | d`
- **Grouping:** `((a < b) == (c | d))`

### A1.1051 — `a ^ b | c ^ d`
- **Source:** `a ^ b | c ^ d`
- **Grouping:** `(((a ^ b) | c) ^ d)`

### A1.1052 — `a & b /= c & d`
- **Source:** `a & b /= c & d`
- **Grouping:** `((a & b) /= (c & d))`

### A1.1053 — `a . b + c | d`
- **Source:** `a . b + c | d`
- **Grouping:** `(((a . b) + c) | d)`

### A1.1054 — `a /= b . c / d`
- **Source:** `a /= b . c / d`
- **Grouping:** `(a /= ((b . c) / d))`

### A1.1055 — `a >= b /= c * d`
- **Source:** `a >= b /= c * d`
- **Grouping:** `((a >= b) /= (c * d))`

### A1.1056 — `a * b % c <= d`
- **Source:** `a * b % c <= d`
- **Grouping:** `(((a * b) % c) <= d)`

### A1.1057 — `a >= b . c == d`
- **Source:** `a >= b . c == d`
- **Grouping:** `((a >= (b . c)) == d)`

### A1.1058 — `a - b <= c >= d`
- **Source:** `a - b <= c >= d`
- **Grouping:** `(((a - b) <= c) >= d)`

### A1.1059 — `a & b > c < d`
- **Source:** `a & b > c < d`
- **Grouping:** `(((a & b) > c) < d)`

### A1.1060 — `a > b && c < d`
- **Source:** `a > b && c < d`
- **Grouping:** `((a > b) && (c < d))`

### A1.1061 — `a << b >= c && d`
- **Source:** `a << b >= c && d`
- **Grouping:** `(((a << b) >= c) && d)`

### A1.1062 — `a << b | c /= d`
- **Source:** `a << b | c /= d`
- **Grouping:** `(((a << b) | c) /= d)`

### A1.1063 — `a >= b - c >= d`
- **Source:** `a >= b - c >= d`
- **Grouping:** `((a >= (b - c)) >= d)`

### A1.1064 — `a /= b || c . d`
- **Source:** `a /= b || c . d`
- **Grouping:** `((a /= b) || (c . d))`

### A1.1065 — `a / b * c << d`
- **Source:** `a / b * c << d`
- **Grouping:** `(((a / b) * c) << d)`

### A1.1066 — `a || b && c /= d`
- **Source:** `a || b && c /= d`
- **Grouping:** `(a || (b && (c /= d)))`

### A1.1067 — `a == b <= c < d`
- **Source:** `a == b <= c < d`
- **Grouping:** `(((a == b) <= c) < d)`

### A1.1068 — `a - b && c > d`
- **Source:** `a - b && c > d`
- **Grouping:** `((a - b) && (c > d))`

### A1.1069 — `a << b >= c || d`
- **Source:** `a << b >= c || d`
- **Grouping:** `(((a << b) >= c) || d)`

### A1.1070 — `a >> b + c % d`
- **Source:** `a >> b + c % d`
- **Grouping:** `(a >> (b + (c % d)))`

### A1.1071 — `a && b % c || d`
- **Source:** `a && b % c || d`
- **Grouping:** `((a && (b % c)) || d)`

### A1.1072 — `a >= b & c >= d`
- **Source:** `a >= b & c >= d`
- **Grouping:** `((a >= (b & c)) >= d)`

### A1.1073 — `a == b /= c == d`
- **Source:** `a == b /= c == d`
- **Grouping:** `(((a == b) /= c) == d)`

### A1.1074 — `a > b % c /= d`
- **Source:** `a > b % c /= d`
- **Grouping:** `((a > (b % c)) /= d)`

### A1.1075 — `a & b - c * d`
- **Source:** `a & b - c * d`
- **Grouping:** `(a & (b - (c * d)))`

### A1.1076 — `a + b & c && d`
- **Source:** `a + b & c && d`
- **Grouping:** `(((a + b) & c) && d)`

### A1.1077 — `a && b | c >= d`
- **Source:** `a && b | c >= d`
- **Grouping:** `(a && ((b | c) >= d))`

### A1.1078 — `a || b . c /= d`
- **Source:** `a || b . c /= d`
- **Grouping:** `(a || ((b . c) /= d))`

### A1.1079 — `a << b / c < d`
- **Source:** `a << b / c < d`
- **Grouping:** `((a << (b / c)) < d)`

### A1.1080 — `a || b - c || d`
- **Source:** `a || b - c || d`
- **Grouping:** `((a || (b - c)) || d)`

### A1.1081 — `a <= b == c . d`
- **Source:** `a <= b == c . d`
- **Grouping:** `((a <= b) == (c . d))`

### A1.1082 — `a > b | c ^ d`
- **Source:** `a > b | c ^ d`
- **Grouping:** `(a > ((b | c) ^ d))`

### A1.1083 — `a >= b >> c - d`
- **Source:** `a >= b >> c - d`
- **Grouping:** `(a >= (b >> (c - d)))`

### A1.1084 — `a /= b < c >> d`
- **Source:** `a /= b < c >> d`
- **Grouping:** `((a /= b) < (c >> d))`

### A1.1085 — `a < b + c - d`
- **Source:** `a < b + c - d`
- **Grouping:** `(a < ((b + c) - d))`

### A1.1086 — `a & b > c << d`
- **Source:** `a & b > c << d`
- **Grouping:** `((a & b) > (c << d))`

### A1.1087 — `a / b - c >= d`
- **Source:** `a / b - c >= d`
- **Grouping:** `(((a / b) - c) >= d)`

### A1.1088 — `a / b == c || d`
- **Source:** `a / b == c || d`
- **Grouping:** `(((a / b) == c) || d)`

### A1.1089 — `a * b < c && d`
- **Source:** `a * b < c && d`
- **Grouping:** `(((a * b) < c) && d)`

### A1.1090 — `a >> b ^ c * d`
- **Source:** `a >> b ^ c * d`
- **Grouping:** `((a >> b) ^ (c * d))`

### A1.1091 — `a << b ^ c == d`
- **Source:** `a << b ^ c == d`
- **Grouping:** `(((a << b) ^ c) == d)`

### A1.1092 — `a * b || c /= d`
- **Source:** `a * b || c /= d`
- **Grouping:** `((a * b) || (c /= d))`

### A1.1093 — `a || b ^ c || d`
- **Source:** `a || b ^ c || d`
- **Grouping:** `((a || (b ^ c)) || d)`

### A1.1094 — `a && b >= c ^ d`
- **Source:** `a && b >= c ^ d`
- **Grouping:** `(a && (b >= (c ^ d)))`

### A1.1095 — `a ^ b || c <= d`
- **Source:** `a ^ b || c <= d`
- **Grouping:** `((a ^ b) || (c <= d))`

### A1.1096 — `a == b <= c / d`
- **Source:** `a == b <= c / d`
- **Grouping:** `((a == b) <= (c / d))`

### A1.1097 — `a ^ b == c /= d`
- **Source:** `a ^ b == c /= d`
- **Grouping:** `(((a ^ b) == c) /= d)`

### A1.1098 — `a ^ b /= c | d`
- **Source:** `a ^ b /= c | d`
- **Grouping:** `((a ^ b) /= (c | d))`

### A1.1099 — `a == b | c + d`
- **Source:** `a == b | c + d`
- **Grouping:** `(a == (b | (c + d)))`

### A1.1100 — `a - b >> c + d`
- **Source:** `a - b >> c + d`
- **Grouping:** `((a - b) >> (c + d))`

### A1.1101 — `a < b << c & d`
- **Source:** `a < b << c & d`
- **Grouping:** `(a < ((b << c) & d))`

### A1.1102 — `a /= b || c >> d`
- **Source:** `a /= b || c >> d`
- **Grouping:** `((a /= b) || (c >> d))`

### A1.1103 — `a / b >= c < d`
- **Source:** `a / b >= c < d`
- **Grouping:** `(((a / b) >= c) < d)`

### A1.1104 — `a << b >= c >> d`
- **Source:** `a << b >= c >> d`
- **Grouping:** `((a << b) >= (c >> d))`

### A1.1105 — `a & b >= c . d`
- **Source:** `a & b >= c . d`
- **Grouping:** `((a & b) >= (c . d))`

### A1.1106 — `a - b == c & d`
- **Source:** `a - b == c & d`
- **Grouping:** `((a - b) == (c & d))`

### A1.1107 — `a /= b > c . d`
- **Source:** `a /= b > c . d`
- **Grouping:** `((a /= b) > (c . d))`

### A1.1108 — `a + b % c & d`
- **Source:** `a + b % c & d`
- **Grouping:** `((a + (b % c)) & d)`

### A1.1109 — `a == b == c >= d`
- **Source:** `a == b == c >= d`
- **Grouping:** `(((a == b) == c) >= d)`

### A1.1110 — `a << b * c <= d`
- **Source:** `a << b * c <= d`
- **Grouping:** `((a << (b * c)) <= d)`

### A1.1111 — `a > b /= c & d`
- **Source:** `a > b /= c & d`
- **Grouping:** `((a > b) /= (c & d))`

### A1.1112 — `a ^ b / c << d`
- **Source:** `a ^ b / c << d`
- **Grouping:** `(a ^ ((b / c) << d))`

### A1.1113 — `a > b == c - d`
- **Source:** `a > b == c - d`
- **Grouping:** `((a > b) == (c - d))`

### A1.1114 — `a + b < c . d`
- **Source:** `a + b < c . d`
- **Grouping:** `((a + b) < (c . d))`

### A1.1115 — `a & b & c . d`
- **Source:** `a & b & c . d`
- **Grouping:** `((a & b) & (c . d))`

### A1.1116 — `a * b == c <= d`
- **Source:** `a * b == c <= d`
- **Grouping:** `(((a * b) == c) <= d)`

### A1.1117 — `a & b & c == d`
- **Source:** `a & b & c == d`
- **Grouping:** `(((a & b) & c) == d)`

### A1.1118 — `a == b + c * d`
- **Source:** `a == b + c * d`
- **Grouping:** `(a == (b + (c * d)))`

### A1.1119 — `a || b % c > d`
- **Source:** `a || b % c > d`
- **Grouping:** `(a || ((b % c) > d))`

### A1.1120 — `a >> b <= c | d`
- **Source:** `a >> b <= c | d`
- **Grouping:** `((a >> b) <= (c | d))`

### A1.1121 — `a > b << c || d`
- **Source:** `a > b << c || d`
- **Grouping:** `((a > (b << c)) || d)`

### A1.1122 — `a >> b % c ^ d`
- **Source:** `a >> b % c ^ d`
- **Grouping:** `((a >> (b % c)) ^ d)`

### A1.1123 — `a - b == c > d`
- **Source:** `a - b == c > d`
- **Grouping:** `(((a - b) == c) > d)`

### A1.1124 — `a + b || c / d`
- **Source:** `a + b || c / d`
- **Grouping:** `((a + b) || (c / d))`

### A1.1125 — `a - b >= c >= d`
- **Source:** `a - b >= c >= d`
- **Grouping:** `(((a - b) >= c) >= d)`

### A1.1126 — `a /= b / c == d`
- **Source:** `a /= b / c == d`
- **Grouping:** `((a /= (b / c)) == d)`

### A1.1127 — `a % b & c ^ d`
- **Source:** `a % b & c ^ d`
- **Grouping:** `(((a % b) & c) ^ d)`

### A1.1128 — `a + b && c / d`
- **Source:** `a + b && c / d`
- **Grouping:** `((a + b) && (c / d))`

### A1.1129 — `a % b > c <= d`
- **Source:** `a % b > c <= d`
- **Grouping:** `(((a % b) > c) <= d)`

### A1.1130 — `a . b | c >> d`
- **Source:** `a . b | c >> d`
- **Grouping:** `((a . b) | (c >> d))`

### A1.1131 — `a * b >= c - d`
- **Source:** `a * b >= c - d`
- **Grouping:** `((a * b) >= (c - d))`

### A1.1132 — `a << b & c * d`
- **Source:** `a << b & c * d`
- **Grouping:** `((a << b) & (c * d))`

### A1.1133 — `a ^ b > c - d`
- **Source:** `a ^ b > c - d`
- **Grouping:** `((a ^ b) > (c - d))`

### A1.1134 — `a << b == c /= d`
- **Source:** `a << b == c /= d`
- **Grouping:** `(((a << b) == c) /= d)`

### A1.1135 — `a <= b >> c ^ d`
- **Source:** `a <= b >> c ^ d`
- **Grouping:** `(a <= ((b >> c) ^ d))`

### A1.1136 — `a ^ b . c < d`
- **Source:** `a ^ b . c < d`
- **Grouping:** `((a ^ (b . c)) < d)`

### A1.1137 — `a / b + c % d`
- **Source:** `a / b + c % d`
- **Grouping:** `((a / b) + (c % d))`

### A1.1138 — `a || b . c - d`
- **Source:** `a || b . c - d`
- **Grouping:** `(a || ((b . c) - d))`

### A1.1139 — `a * b / c /= d`
- **Source:** `a * b / c /= d`
- **Grouping:** `(((a * b) / c) /= d)`

### A1.1140 — `a % b >> c . d`
- **Source:** `a % b >> c . d`
- **Grouping:** `((a % b) >> (c . d))`

### A1.1141 — `a ^ b ^ c << d`
- **Source:** `a ^ b ^ c << d`
- **Grouping:** `((a ^ b) ^ (c << d))`

### A1.1142 — `a . b >= c / d`
- **Source:** `a . b >= c / d`
- **Grouping:** `((a . b) >= (c / d))`

### A1.1143 — `a || b >> c >= d`
- **Source:** `a || b >> c >= d`
- **Grouping:** `(a || ((b >> c) >= d))`

### A1.1144 — `a % b << c || d`
- **Source:** `a % b << c || d`
- **Grouping:** `(((a % b) << c) || d)`

### A1.1145 — `a > b << c / d`
- **Source:** `a > b << c / d`
- **Grouping:** `(a > (b << (c / d)))`

### A1.1146 — `a % b / c + d`
- **Source:** `a % b / c + d`
- **Grouping:** `(((a % b) / c) + d)`

### A1.1147 — `a ^ b >= c % d`
- **Source:** `a ^ b >= c % d`
- **Grouping:** `((a ^ b) >= (c % d))`

### A1.1148 — `a | b / c > d`
- **Source:** `a | b / c > d`
- **Grouping:** `((a | (b / c)) > d)`

### A1.1149 — `a ^ b > c == d`
- **Source:** `a ^ b > c == d`
- **Grouping:** `(((a ^ b) > c) == d)`

### A1.1150 — `a + b <= c || d`
- **Source:** `a + b <= c || d`
- **Grouping:** `(((a + b) <= c) || d)`

### A1.1151 — `a <= b - c <= d`
- **Source:** `a <= b - c <= d`
- **Grouping:** `((a <= (b - c)) <= d)`

### A1.1152 — `a == b / c == d`
- **Source:** `a == b / c == d`
- **Grouping:** `((a == (b / c)) == d)`

### A1.1153 — `a & b % c && d`
- **Source:** `a & b % c && d`
- **Grouping:** `((a & (b % c)) && d)`

### A1.1154 — `a <= b ^ c % d`
- **Source:** `a <= b ^ c % d`
- **Grouping:** `(a <= (b ^ (c % d)))`

### A1.1155 — `a - b | c == d`
- **Source:** `a - b | c == d`
- **Grouping:** `(((a - b) | c) == d)`

### A1.1156 — `a | b / c < d`
- **Source:** `a | b / c < d`
- **Grouping:** `((a | (b / c)) < d)`

### A1.1157 — `a < b - c >= d`
- **Source:** `a < b - c >= d`
- **Grouping:** `((a < (b - c)) >= d)`

### A1.1158 — `a / b - c % d`
- **Source:** `a / b - c % d`
- **Grouping:** `((a / b) - (c % d))`

### A1.1159 — `a ^ b < c == d`
- **Source:** `a ^ b < c == d`
- **Grouping:** `(((a ^ b) < c) == d)`

### A1.1160 — `a == b > c == d`
- **Source:** `a == b > c == d`
- **Grouping:** `(((a == b) > c) == d)`

### A1.1161 — `a /= b /= c ^ d`
- **Source:** `a /= b /= c ^ d`
- **Grouping:** `((a /= b) /= (c ^ d))`

### A1.1162 — `a == b * c & d`
- **Source:** `a == b * c & d`
- **Grouping:** `(a == ((b * c) & d))`

### A1.1163 — `a << b /= c /= d`
- **Source:** `a << b /= c /= d`
- **Grouping:** `(((a << b) /= c) /= d)`

### A1.1164 — `a << b ^ c <= d`
- **Source:** `a << b ^ c <= d`
- **Grouping:** `(((a << b) ^ c) <= d)`

### A1.1165 — `a && b == c == d`
- **Source:** `a && b == c == d`
- **Grouping:** `(a && ((b == c) == d))`

### A1.1166 — `a ^ b << c / d`
- **Source:** `a ^ b << c / d`
- **Grouping:** `(a ^ (b << (c / d)))`

### A1.1167 — `a % b . c || d`
- **Source:** `a % b . c || d`
- **Grouping:** `((a % (b . c)) || d)`

### A1.1168 — `a * b || c + d`
- **Source:** `a * b || c + d`
- **Grouping:** `((a * b) || (c + d))`

### A1.1169 — `a == b >> c || d`
- **Source:** `a == b >> c || d`
- **Grouping:** `((a == (b >> c)) || d)`

### A1.1170 — `a + b <= c * d`
- **Source:** `a + b <= c * d`
- **Grouping:** `((a + b) <= (c * d))`

### A1.1171 — `a << b . c <= d`
- **Source:** `a << b . c <= d`
- **Grouping:** `((a << (b . c)) <= d)`

### A1.1172 — `a << b - c & d`
- **Source:** `a << b - c & d`
- **Grouping:** `((a << (b - c)) & d)`

### A1.1173 — `a - b % c >= d`
- **Source:** `a - b % c >= d`
- **Grouping:** `((a - (b % c)) >= d)`

### A1.1174 — `a + b >= c + d`
- **Source:** `a + b >= c + d`
- **Grouping:** `((a + b) >= (c + d))`

### A1.1175 — `a >= b >= c + d`
- **Source:** `a >= b >= c + d`
- **Grouping:** `((a >= b) >= (c + d))`

### A1.1176 — `a == b * c . d`
- **Source:** `a == b * c . d`
- **Grouping:** `(a == (b * (c . d)))`

### A1.1177 — `a & b /= c - d`
- **Source:** `a & b /= c - d`
- **Grouping:** `((a & b) /= (c - d))`

### A1.1178 — `a | b >= c <= d`
- **Source:** `a | b >= c <= d`
- **Grouping:** `(((a | b) >= c) <= d)`

### A1.1179 — `a >= b == c / d`
- **Source:** `a >= b == c / d`
- **Grouping:** `((a >= b) == (c / d))`

### A1.1180 — `a / b / c /= d`
- **Source:** `a / b / c /= d`
- **Grouping:** `(((a / b) / c) /= d)`

### A1.1181 — `a == b /= c & d`
- **Source:** `a == b /= c & d`
- **Grouping:** `((a == b) /= (c & d))`

### A1.1182 — `a >> b < c & d`
- **Source:** `a >> b < c & d`
- **Grouping:** `((a >> b) < (c & d))`

### A1.1183 — `a >> b == c - d`
- **Source:** `a >> b == c - d`
- **Grouping:** `((a >> b) == (c - d))`

### A1.1184 — `a % b >= c <= d`
- **Source:** `a % b >= c <= d`
- **Grouping:** `(((a % b) >= c) <= d)`

### A1.1185 — `a < b > c > d`
- **Source:** `a < b > c > d`
- **Grouping:** `(((a < b) > c) > d)`

### A1.1186 — `a <= b /= c * d`
- **Source:** `a <= b /= c * d`
- **Grouping:** `((a <= b) /= (c * d))`

### A1.1187 — `a & b /= c * d`
- **Source:** `a & b /= c * d`
- **Grouping:** `((a & b) /= (c * d))`

### A1.1188 — `a * b >= c >> d`
- **Source:** `a * b >= c >> d`
- **Grouping:** `((a * b) >= (c >> d))`

### A1.1189 — `a | b == c . d`
- **Source:** `a | b == c . d`
- **Grouping:** `((a | b) == (c . d))`

### A1.1190 — `a + b % c . d`
- **Source:** `a + b % c . d`
- **Grouping:** `(a + (b % (c . d)))`

### A1.1191 — `a >= b < c | d`
- **Source:** `a >= b < c | d`
- **Grouping:** `((a >= b) < (c | d))`

### A1.1192 — `a > b & c + d`
- **Source:** `a > b & c + d`
- **Grouping:** `(a > (b & (c + d)))`

### A1.1193 — `a /= b . c - d`
- **Source:** `a /= b . c - d`
- **Grouping:** `(a /= ((b . c) - d))`

### A1.1194 — `a << b > c && d`
- **Source:** `a << b > c && d`
- **Grouping:** `(((a << b) > c) && d)`

### A1.1195 — `a > b + c << d`
- **Source:** `a > b + c << d`
- **Grouping:** `(a > ((b + c) << d))`

### A1.1196 — `a < b >> c >= d`
- **Source:** `a < b >> c >= d`
- **Grouping:** `((a < (b >> c)) >= d)`

### A1.1197 — `a << b / c <= d`
- **Source:** `a << b / c <= d`
- **Grouping:** `((a << (b / c)) <= d)`

### A1.1198 — `a . b % c | d`
- **Source:** `a . b % c | d`
- **Grouping:** `(((a . b) % c) | d)`

### A1.1199 — `a >= b % c == d`
- **Source:** `a >= b % c == d`
- **Grouping:** `((a >= (b % c)) == d)`

### A1.1200 — `a * b >> c == d`
- **Source:** `a * b >> c == d`
- **Grouping:** `(((a * b) >> c) == d)`

### A1.1201 — `a >= b <= c >= d`
- **Source:** `a >= b <= c >= d`
- **Grouping:** `(((a >= b) <= c) >= d)`

### A1.1202 — `a == b < c / d`
- **Source:** `a == b < c / d`
- **Grouping:** `((a == b) < (c / d))`

### A1.1203 — `a || b + c /= d`
- **Source:** `a || b + c /= d`
- **Grouping:** `(a || ((b + c) /= d))`

### A1.1204 — `a ^ b == c % d`
- **Source:** `a ^ b == c % d`
- **Grouping:** `((a ^ b) == (c % d))`

### A1.1205 — `a == b /= c >> d`
- **Source:** `a == b /= c >> d`
- **Grouping:** `((a == b) /= (c >> d))`

### A1.1206 — `a /= b + c & d`
- **Source:** `a /= b + c & d`
- **Grouping:** `(a /= ((b + c) & d))`

### A1.1207 — `a & b % c * d`
- **Source:** `a & b % c * d`
- **Grouping:** `(a & ((b % c) * d))`

### A1.1208 — `a || b > c < d`
- **Source:** `a || b > c < d`
- **Grouping:** `(a || ((b > c) < d))`

### A1.1209 — `a / b || c > d`
- **Source:** `a / b || c > d`
- **Grouping:** `((a / b) || (c > d))`

### A1.1210 — `a < b * c >= d`
- **Source:** `a < b * c >= d`
- **Grouping:** `((a < (b * c)) >= d)`

### A1.1211 — `a - b && c + d`
- **Source:** `a - b && c + d`
- **Grouping:** `((a - b) && (c + d))`

### A1.1212 — `a == b || c - d`
- **Source:** `a == b || c - d`
- **Grouping:** `((a == b) || (c - d))`

### A1.1213 — `a <= b && c || d`
- **Source:** `a <= b && c || d`
- **Grouping:** `(((a <= b) && c) || d)`

### A1.1214 — `a == b /= c || d`
- **Source:** `a == b /= c || d`
- **Grouping:** `(((a == b) /= c) || d)`

### A1.1215 — `a == b && c || d`
- **Source:** `a == b && c || d`
- **Grouping:** `(((a == b) && c) || d)`

### A1.1216 — `a * b == c * d`
- **Source:** `a * b == c * d`
- **Grouping:** `((a * b) == (c * d))`

### A1.1217 — `a | b /= c % d`
- **Source:** `a | b /= c % d`
- **Grouping:** `((a | b) /= (c % d))`

### A1.1218 — `a << b || c << d`
- **Source:** `a << b || c << d`
- **Grouping:** `((a << b) || (c << d))`

### A1.1219 — `a | b & c / d`
- **Source:** `a | b & c / d`
- **Grouping:** `((a | b) & (c / d))`

### A1.1220 — `a || b / c * d`
- **Source:** `a || b / c * d`
- **Grouping:** `(a || ((b / c) * d))`

### A1.1221 — `a % b + c ^ d`
- **Source:** `a % b + c ^ d`
- **Grouping:** `(((a % b) + c) ^ d)`

### A1.1222 — `a | b << c & d`
- **Source:** `a | b << c & d`
- **Grouping:** `((a | (b << c)) & d)`

### A1.1223 — `a & b + c << d`
- **Source:** `a & b + c << d`
- **Grouping:** `(a & ((b + c) << d))`

### A1.1224 — `a && b ^ c >> d`
- **Source:** `a && b ^ c >> d`
- **Grouping:** `(a && (b ^ (c >> d)))`

### A1.1225 — `a || b << c /= d`
- **Source:** `a || b << c /= d`
- **Grouping:** `(a || ((b << c) /= d))`

### A1.1226 — `a /= b >> c / d`
- **Source:** `a /= b >> c / d`
- **Grouping:** `(a /= (b >> (c / d)))`

### A1.1227 — `a || b >= c >= d`
- **Source:** `a || b >= c >= d`
- **Grouping:** `(a || ((b >= c) >= d))`

### A1.1228 — `a || b + c < d`
- **Source:** `a || b + c < d`
- **Grouping:** `(a || ((b + c) < d))`

### A1.1229 — `a & b - c /= d`
- **Source:** `a & b - c /= d`
- **Grouping:** `((a & (b - c)) /= d)`

### A1.1230 — `a | b % c && d`
- **Source:** `a | b % c && d`
- **Grouping:** `((a | (b % c)) && d)`

### A1.1231 — `a & b ^ c + d`
- **Source:** `a & b ^ c + d`
- **Grouping:** `((a & b) ^ (c + d))`

### A1.1232 — `a /= b >= c == d`
- **Source:** `a /= b >= c == d`
- **Grouping:** `(((a /= b) >= c) == d)`

### A1.1233 — `a <= b | c + d`
- **Source:** `a <= b | c + d`
- **Grouping:** `(a <= (b | (c + d)))`

### A1.1234 — `a + b && c >= d`
- **Source:** `a + b && c >= d`
- **Grouping:** `((a + b) && (c >= d))`

### A1.1235 — `a / b /= c /= d`
- **Source:** `a / b /= c /= d`
- **Grouping:** `(((a / b) /= c) /= d)`

### A1.1236 — `a /= b || c == d`
- **Source:** `a /= b || c == d`
- **Grouping:** `((a /= b) || (c == d))`

### A1.1237 — `a / b /= c & d`
- **Source:** `a / b /= c & d`
- **Grouping:** `((a / b) /= (c & d))`

### A1.1238 — `a || b - c >= d`
- **Source:** `a || b - c >= d`
- **Grouping:** `(a || ((b - c) >= d))`

### A1.1239 — `a | b >= c + d`
- **Source:** `a | b >= c + d`
- **Grouping:** `((a | b) >= (c + d))`

### A1.1240 — `a * b . c << d`
- **Source:** `a * b . c << d`
- **Grouping:** `((a * (b . c)) << d)`

### A1.1241 — `a ^ b && c < d`
- **Source:** `a ^ b && c < d`
- **Grouping:** `((a ^ b) && (c < d))`

### A1.1242 — `a | b / c - d`
- **Source:** `a | b / c - d`
- **Grouping:** `(a | ((b / c) - d))`

### A1.1243 — `a | b && c . d`
- **Source:** `a | b && c . d`
- **Grouping:** `((a | b) && (c . d))`

### A1.1244 — `a - b && c >= d`
- **Source:** `a - b && c >= d`
- **Grouping:** `((a - b) && (c >= d))`

### A1.1245 — `a == b > c | d`
- **Source:** `a == b > c | d`
- **Grouping:** `((a == b) > (c | d))`

### A1.1246 — `a > b & c . d`
- **Source:** `a > b & c . d`
- **Grouping:** `(a > (b & (c . d)))`

### A1.1247 — `a . b + c >> d`
- **Source:** `a . b + c >> d`
- **Grouping:** `(((a . b) + c) >> d)`

### A1.1248 — `a && b * c == d`
- **Source:** `a && b * c == d`
- **Grouping:** `(a && ((b * c) == d))`

### A1.1249 — `a || b < c > d`
- **Source:** `a || b < c > d`
- **Grouping:** `(a || ((b < c) > d))`

### A1.1250 — `a + b || c > d`
- **Source:** `a + b || c > d`
- **Grouping:** `((a + b) || (c > d))`

### A1.1251 — `a & b >> c | d`
- **Source:** `a & b >> c | d`
- **Grouping:** `((a & (b >> c)) | d)`

### A1.1252 — `a < b ^ c <= d`
- **Source:** `a < b ^ c <= d`
- **Grouping:** `((a < (b ^ c)) <= d)`

### A1.1253 — `a << b / c & d`
- **Source:** `a << b / c & d`
- **Grouping:** `((a << (b / c)) & d)`

### A1.1254 — `a >= b >= c ^ d`
- **Source:** `a >= b >= c ^ d`
- **Grouping:** `((a >= b) >= (c ^ d))`

### A1.1255 — `a == b /= c | d`
- **Source:** `a == b /= c | d`
- **Grouping:** `((a == b) /= (c | d))`

### A1.1256 — `a == b > c || d`
- **Source:** `a == b > c || d`
- **Grouping:** `(((a == b) > c) || d)`

### A1.1257 — `a . b + c && d`
- **Source:** `a . b + c && d`
- **Grouping:** `(((a . b) + c) && d)`

### A1.1258 — `a + b || c == d`
- **Source:** `a + b || c == d`
- **Grouping:** `((a + b) || (c == d))`

### A1.1259 — `a >> b >> c || d`
- **Source:** `a >> b >> c || d`
- **Grouping:** `(((a >> b) >> c) || d)`

### A1.1260 — `a /= b % c < d`
- **Source:** `a /= b % c < d`
- **Grouping:** `((a /= (b % c)) < d)`

### A1.1261 — `a % b | c % d`
- **Source:** `a % b | c % d`
- **Grouping:** `((a % b) | (c % d))`

### A1.1262 — `a + b /= c * d`
- **Source:** `a + b /= c * d`
- **Grouping:** `((a + b) /= (c * d))`

### A1.1263 — `a - b ^ c & d`
- **Source:** `a - b ^ c & d`
- **Grouping:** `(((a - b) ^ c) & d)`

### A1.1264 — `a << b <= c + d`
- **Source:** `a << b <= c + d`
- **Grouping:** `((a << b) <= (c + d))`

### A1.1265 — `a * b ^ c << d`
- **Source:** `a * b ^ c << d`
- **Grouping:** `((a * b) ^ (c << d))`

### A1.1266 — `a <= b << c > d`
- **Source:** `a <= b << c > d`
- **Grouping:** `((a <= (b << c)) > d)`

### A1.1267 — `a | b << c >> d`
- **Source:** `a | b << c >> d`
- **Grouping:** `(a | ((b << c) >> d))`

### A1.1268 — `a * b > c > d`
- **Source:** `a * b > c > d`
- **Grouping:** `(((a * b) > c) > d)`

### A1.1269 — `a | b <= c < d`
- **Source:** `a | b <= c < d`
- **Grouping:** `(((a | b) <= c) < d)`

### A1.1270 — `a . b && c % d`
- **Source:** `a . b && c % d`
- **Grouping:** `((a . b) && (c % d))`

### A1.1271 — `a / b && c * d`
- **Source:** `a / b && c * d`
- **Grouping:** `((a / b) && (c * d))`

### A1.1272 — `a + b % c + d`
- **Source:** `a + b % c + d`
- **Grouping:** `((a + (b % c)) + d)`

### A1.1273 — `a > b . c >> d`
- **Source:** `a > b . c >> d`
- **Grouping:** `(a > ((b . c) >> d))`

### A1.1274 — `a > b /= c == d`
- **Source:** `a > b /= c == d`
- **Grouping:** `(((a > b) /= c) == d)`

### A1.1275 — `a && b >> c * d`
- **Source:** `a && b >> c * d`
- **Grouping:** `(a && (b >> (c * d)))`

### A1.1276 — `a && b < c || d`
- **Source:** `a && b < c || d`
- **Grouping:** `((a && (b < c)) || d)`

### A1.1277 — `a /= b >= c - d`
- **Source:** `a /= b >= c - d`
- **Grouping:** `((a /= b) >= (c - d))`

### A1.1278 — `a - b / c & d`
- **Source:** `a - b / c & d`
- **Grouping:** `((a - (b / c)) & d)`

### A1.1279 — `a + b % c * d`
- **Source:** `a + b % c * d`
- **Grouping:** `(a + ((b % c) * d))`

### A1.1280 — `a . b > c << d`
- **Source:** `a . b > c << d`
- **Grouping:** `((a . b) > (c << d))`

### A1.1281 — `a || b > c / d`
- **Source:** `a || b > c / d`
- **Grouping:** `(a || (b > (c / d)))`

### A1.1282 — `a == b > c >= d`
- **Source:** `a == b > c >= d`
- **Grouping:** `(((a == b) > c) >= d)`

### A1.1283 — `a /= b <= c & d`
- **Source:** `a /= b <= c & d`
- **Grouping:** `((a /= b) <= (c & d))`

### A1.1284 — `a <= b || c > d`
- **Source:** `a <= b || c > d`
- **Grouping:** `((a <= b) || (c > d))`

### A1.1285 — `a | b || c >> d`
- **Source:** `a | b || c >> d`
- **Grouping:** `((a | b) || (c >> d))`

### A1.1286 — `a || b + c % d`
- **Source:** `a || b + c % d`
- **Grouping:** `(a || (b + (c % d)))`

### A1.1287 — `a == b /= c % d`
- **Source:** `a == b /= c % d`
- **Grouping:** `((a == b) /= (c % d))`

### A1.1288 — `a % b ^ c || d`
- **Source:** `a % b ^ c || d`
- **Grouping:** `(((a % b) ^ c) || d)`

### A1.1289 — `a >= b * c >> d`
- **Source:** `a >= b * c >> d`
- **Grouping:** `(a >= ((b * c) >> d))`

### A1.1290 — `a - b == c || d`
- **Source:** `a - b == c || d`
- **Grouping:** `(((a - b) == c) || d)`

### A1.1291 — `a * b << c == d`
- **Source:** `a * b << c == d`
- **Grouping:** `(((a * b) << c) == d)`

### A1.1292 — `a % b . c / d`
- **Source:** `a % b . c / d`
- **Grouping:** `((a % (b . c)) / d)`

### A1.1293 — `a % b < c ^ d`
- **Source:** `a % b < c ^ d`
- **Grouping:** `((a % b) < (c ^ d))`

### A1.1294 — `a >> b > c < d`
- **Source:** `a >> b > c < d`
- **Grouping:** `(((a >> b) > c) < d)`

### A1.1295 — `a - b << c . d`
- **Source:** `a - b << c . d`
- **Grouping:** `((a - b) << (c . d))`

### A1.1296 — `a >> b | c << d`
- **Source:** `a >> b | c << d`
- **Grouping:** `((a >> b) | (c << d))`

### A1.1297 — `a / b && c & d`
- **Source:** `a / b && c & d`
- **Grouping:** `((a / b) && (c & d))`

### A1.1298 — `a / b >> c | d`
- **Source:** `a / b >> c | d`
- **Grouping:** `(((a / b) >> c) | d)`

### A1.1299 — `a /= b >= c % d`
- **Source:** `a /= b >= c % d`
- **Grouping:** `((a /= b) >= (c % d))`

### A1.1300 — `a < b << c % d`
- **Source:** `a < b << c % d`
- **Grouping:** `(a < (b << (c % d)))`

### A1.1301 — `a . b | c + d`
- **Source:** `a . b | c + d`
- **Grouping:** `((a . b) | (c + d))`

### A1.1302 — `a * b * c + d`
- **Source:** `a * b * c + d`
- **Grouping:** `(((a * b) * c) + d)`

### A1.1303 — `a >> b < c <= d`
- **Source:** `a >> b < c <= d`
- **Grouping:** `(((a >> b) < c) <= d)`

### A1.1304 — `a & b >= c % d`
- **Source:** `a & b >= c % d`
- **Grouping:** `((a & b) >= (c % d))`

### A1.1305 — `a == b << c >= d`
- **Source:** `a == b << c >= d`
- **Grouping:** `((a == (b << c)) >= d)`

### A1.1306 — `a + b || c >= d`
- **Source:** `a + b || c >= d`
- **Grouping:** `((a + b) || (c >= d))`

### A1.1307 — `a & b >> c % d`
- **Source:** `a & b >> c % d`
- **Grouping:** `(a & (b >> (c % d)))`

### A1.1308 — `a + b == c / d`
- **Source:** `a + b == c / d`
- **Grouping:** `((a + b) == (c / d))`

### A1.1309 — `a ^ b >= c /= d`
- **Source:** `a ^ b >= c /= d`
- **Grouping:** `(((a ^ b) >= c) /= d)`

### A1.1310 — `a >> b >> c ^ d`
- **Source:** `a >> b >> c ^ d`
- **Grouping:** `(((a >> b) >> c) ^ d)`

### A1.1311 — `a << b <= c || d`
- **Source:** `a << b <= c || d`
- **Grouping:** `(((a << b) <= c) || d)`

### A1.1312 — `a == b / c . d`
- **Source:** `a == b / c . d`
- **Grouping:** `(a == (b / (c . d)))`

### A1.1313 — `a && b <= c | d`
- **Source:** `a && b <= c | d`
- **Grouping:** `(a && (b <= (c | d)))`

### A1.1314 — `a << b ^ c ^ d`
- **Source:** `a << b ^ c ^ d`
- **Grouping:** `(((a << b) ^ c) ^ d)`

### A1.1315 — `a / b ^ c == d`
- **Source:** `a / b ^ c == d`
- **Grouping:** `(((a / b) ^ c) == d)`

### A1.1316 — `a . b /= c / d`
- **Source:** `a . b /= c / d`
- **Grouping:** `((a . b) /= (c / d))`

### A1.1317 — `a - b . c >= d`
- **Source:** `a - b . c >= d`
- **Grouping:** `((a - (b . c)) >= d)`

### A1.1318 — `a . b > c % d`
- **Source:** `a . b > c % d`
- **Grouping:** `((a . b) > (c % d))`

### A1.1319 — `a /= b /= c << d`
- **Source:** `a /= b /= c << d`
- **Grouping:** `((a /= b) /= (c << d))`

### A1.1320 — `a /= b & c % d`
- **Source:** `a /= b & c % d`
- **Grouping:** `(a /= (b & (c % d)))`

### A1.1321 — `a * b & c << d`
- **Source:** `a * b & c << d`
- **Grouping:** `((a * b) & (c << d))`

### A1.1322 — `a /= b + c <= d`
- **Source:** `a /= b + c <= d`
- **Grouping:** `((a /= (b + c)) <= d)`

### A1.1323 — `a >= b - c <= d`
- **Source:** `a >= b - c <= d`
- **Grouping:** `((a >= (b - c)) <= d)`

### A1.1324 — `a == b - c /= d`
- **Source:** `a == b - c /= d`
- **Grouping:** `((a == (b - c)) /= d)`

### A1.1325 — `a / b ^ c & d`
- **Source:** `a / b ^ c & d`
- **Grouping:** `(((a / b) ^ c) & d)`

### A1.1326 — `a <= b == c >> d`
- **Source:** `a <= b == c >> d`
- **Grouping:** `((a <= b) == (c >> d))`

### A1.1327 — `a > b / c % d`
- **Source:** `a > b / c % d`
- **Grouping:** `(a > ((b / c) % d))`

### A1.1328 — `a - b /= c % d`
- **Source:** `a - b /= c % d`
- **Grouping:** `((a - b) /= (c % d))`

### A1.1329 — `a >> b <= c + d`
- **Source:** `a >> b <= c + d`
- **Grouping:** `((a >> b) <= (c + d))`

### A1.1330 — `a . b | c == d`
- **Source:** `a . b | c == d`
- **Grouping:** `(((a . b) | c) == d)`

### A1.1331 — `a ^ b < c ^ d`
- **Source:** `a ^ b < c ^ d`
- **Grouping:** `((a ^ b) < (c ^ d))`

### A1.1332 — `a ^ b >> c <= d`
- **Source:** `a ^ b >> c <= d`
- **Grouping:** `((a ^ (b >> c)) <= d)`

### A1.1333 — `a > b < c - d`
- **Source:** `a > b < c - d`
- **Grouping:** `((a > b) < (c - d))`

### A1.1334 — `a || b . c >= d`
- **Source:** `a || b . c >= d`
- **Grouping:** `(a || ((b . c) >= d))`

### A1.1335 — `a & b - c && d`
- **Source:** `a & b - c && d`
- **Grouping:** `((a & (b - c)) && d)`

### A1.1336 — `a && b << c || d`
- **Source:** `a && b << c || d`
- **Grouping:** `((a && (b << c)) || d)`

### A1.1337 — `a >> b . c > d`
- **Source:** `a >> b . c > d`
- **Grouping:** `((a >> (b . c)) > d)`

### A1.1338 — `a > b + c ^ d`
- **Source:** `a > b + c ^ d`
- **Grouping:** `(a > ((b + c) ^ d))`

### A1.1339 — `a % b + c == d`
- **Source:** `a % b + c == d`
- **Grouping:** `(((a % b) + c) == d)`

### A1.1340 — `a ^ b . c <= d`
- **Source:** `a ^ b . c <= d`
- **Grouping:** `((a ^ (b . c)) <= d)`

### A1.1341 — `a / b > c & d`
- **Source:** `a / b > c & d`
- **Grouping:** `((a / b) > (c & d))`

### A1.1342 — `a + b << c > d`
- **Source:** `a + b << c > d`
- **Grouping:** `(((a + b) << c) > d)`

### A1.1343 — `a && b ^ c || d`
- **Source:** `a && b ^ c || d`
- **Grouping:** `((a && (b ^ c)) || d)`

### A1.1344 — `a - b /= c . d`
- **Source:** `a - b /= c . d`
- **Grouping:** `((a - b) /= (c . d))`

### A1.1345 — `a > b + c >> d`
- **Source:** `a > b + c >> d`
- **Grouping:** `(a > ((b + c) >> d))`

### A1.1346 — `a >= b ^ c > d`
- **Source:** `a >= b ^ c > d`
- **Grouping:** `((a >= (b ^ c)) > d)`

### A1.1347 — `a % b / c & d`
- **Source:** `a % b / c & d`
- **Grouping:** `(((a % b) / c) & d)`

### A1.1348 — `a > b / c & d`
- **Source:** `a > b / c & d`
- **Grouping:** `(a > ((b / c) & d))`

### A1.1349 — `a & b + c >= d`
- **Source:** `a & b + c >= d`
- **Grouping:** `((a & (b + c)) >= d)`

### A1.1350 — `a | b << c % d`
- **Source:** `a | b << c % d`
- **Grouping:** `(a | (b << (c % d)))`

### A1.1351 — `a > b * c && d`
- **Source:** `a > b * c && d`
- **Grouping:** `((a > (b * c)) && d)`

### A1.1352 — `a >> b < c | d`
- **Source:** `a >> b < c | d`
- **Grouping:** `((a >> b) < (c | d))`

### A1.1353 — `a || b >> c + d`
- **Source:** `a || b >> c + d`
- **Grouping:** `(a || (b >> (c + d)))`

### A1.1354 — `a > b == c + d`
- **Source:** `a > b == c + d`
- **Grouping:** `((a > b) == (c + d))`

### A1.1355 — `a >> b || c <= d`
- **Source:** `a >> b || c <= d`
- **Grouping:** `((a >> b) || (c <= d))`

### A1.1356 — `a /= b << c * d`
- **Source:** `a /= b << c * d`
- **Grouping:** `(a /= (b << (c * d)))`

### A1.1357 — `a ^ b > c > d`
- **Source:** `a ^ b > c > d`
- **Grouping:** `(((a ^ b) > c) > d)`

### A1.1358 — `a . b . c + d`
- **Source:** `a . b . c + d`
- **Grouping:** `((a . (b . c)) + d)`

### A1.1359 — `a <= b || c < d`
- **Source:** `a <= b || c < d`
- **Grouping:** `((a <= b) || (c < d))`

### A1.1360 — `a * b / c * d`
- **Source:** `a * b / c * d`
- **Grouping:** `(((a * b) / c) * d)`

### A1.1361 — `a / b << c << d`
- **Source:** `a / b << c << d`
- **Grouping:** `(((a / b) << c) << d)`

### A1.1362 — `a && b /= c + d`
- **Source:** `a && b /= c + d`
- **Grouping:** `(a && (b /= (c + d)))`

### A1.1363 — `a /= b < c > d`
- **Source:** `a /= b < c > d`
- **Grouping:** `(((a /= b) < c) > d)`

### A1.1364 — `a > b - c /= d`
- **Source:** `a > b - c /= d`
- **Grouping:** `((a > (b - c)) /= d)`

### A1.1365 — `a <= b >> c > d`
- **Source:** `a <= b >> c > d`
- **Grouping:** `((a <= (b >> c)) > d)`

### A1.1366 — `a + b % c == d`
- **Source:** `a + b % c == d`
- **Grouping:** `((a + (b % c)) == d)`

### A1.1367 — `a && b && c <= d`
- **Source:** `a && b && c <= d`
- **Grouping:** `((a && b) && (c <= d))`

### A1.1368 — `a . b - c || d`
- **Source:** `a . b - c || d`
- **Grouping:** `(((a . b) - c) || d)`

### A1.1369 — `a * b == c >= d`
- **Source:** `a * b == c >= d`
- **Grouping:** `(((a * b) == c) >= d)`

### A1.1370 — `a & b == c && d`
- **Source:** `a & b == c && d`
- **Grouping:** `(((a & b) == c) && d)`

### A1.1371 — `a <= b < c & d`
- **Source:** `a <= b < c & d`
- **Grouping:** `((a <= b) < (c & d))`

### A1.1372 — `a ^ b < c <= d`
- **Source:** `a ^ b < c <= d`
- **Grouping:** `(((a ^ b) < c) <= d)`

### A1.1373 — `a >= b || c + d`
- **Source:** `a >= b || c + d`
- **Grouping:** `((a >= b) || (c + d))`

### A1.1374 — `a < b * c ^ d`
- **Source:** `a < b * c ^ d`
- **Grouping:** `(a < ((b * c) ^ d))`

### A1.1375 — `a * b ^ c < d`
- **Source:** `a * b ^ c < d`
- **Grouping:** `(((a * b) ^ c) < d)`

### A1.1376 — `a > b > c | d`
- **Source:** `a > b > c | d`
- **Grouping:** `((a > b) > (c | d))`

### A1.1377 — `a << b && c * d`
- **Source:** `a << b && c * d`
- **Grouping:** `((a << b) && (c * d))`

### A1.1378 — `a && b && c >> d`
- **Source:** `a && b && c >> d`
- **Grouping:** `((a && b) && (c >> d))`

### A1.1379 — `a < b /= c << d`
- **Source:** `a < b /= c << d`
- **Grouping:** `((a < b) /= (c << d))`

### A1.1380 — `a | b / c == d`
- **Source:** `a | b / c == d`
- **Grouping:** `((a | (b / c)) == d)`

### A1.1381 — `a == b >= c + d`
- **Source:** `a == b >= c + d`
- **Grouping:** `((a == b) >= (c + d))`

### A1.1382 — `a == b << c >> d`
- **Source:** `a == b << c >> d`
- **Grouping:** `(a == ((b << c) >> d))`

### A1.1383 — `a % b * c == d`
- **Source:** `a % b * c == d`
- **Grouping:** `(((a % b) * c) == d)`

### A1.1384 — `a < b >> c || d`
- **Source:** `a < b >> c || d`
- **Grouping:** `((a < (b >> c)) || d)`

### A1.1385 — `a || b <= c >= d`
- **Source:** `a || b <= c >= d`
- **Grouping:** `(a || ((b <= c) >= d))`

### A1.1386 — `a + b . c >= d`
- **Source:** `a + b . c >= d`
- **Grouping:** `((a + (b . c)) >= d)`

### A1.1387 — `a % b * c <= d`
- **Source:** `a % b * c <= d`
- **Grouping:** `(((a % b) * c) <= d)`

### A1.1388 — `a < b >> c / d`
- **Source:** `a < b >> c / d`
- **Grouping:** `(a < (b >> (c / d)))`

### A1.1389 — `a >> b /= c | d`
- **Source:** `a >> b /= c | d`
- **Grouping:** `((a >> b) /= (c | d))`

### A1.1390 — `a <= b | c . d`
- **Source:** `a <= b | c . d`
- **Grouping:** `(a <= (b | (c . d)))`

### A1.1391 — `a <= b <= c < d`
- **Source:** `a <= b <= c < d`
- **Grouping:** `(((a <= b) <= c) < d)`

### A1.1392 — `a + b % c >= d`
- **Source:** `a + b % c >= d`
- **Grouping:** `((a + (b % c)) >= d)`

### A1.1393 — `a & b << c || d`
- **Source:** `a & b << c || d`
- **Grouping:** `((a & (b << c)) || d)`

### A1.1394 — `a - b | c < d`
- **Source:** `a - b | c < d`
- **Grouping:** `(((a - b) | c) < d)`

### A1.1395 — `a ^ b > c >= d`
- **Source:** `a ^ b > c >= d`
- **Grouping:** `(((a ^ b) > c) >= d)`

### A1.1396 — `a + b & c << d`
- **Source:** `a + b & c << d`
- **Grouping:** `((a + b) & (c << d))`

### A1.1397 — `a == b > c & d`
- **Source:** `a == b > c & d`
- **Grouping:** `((a == b) > (c & d))`

### A1.1398 — `a >= b >> c < d`
- **Source:** `a >= b >> c < d`
- **Grouping:** `((a >= (b >> c)) < d)`

### A1.1399 — `a + b << c >= d`
- **Source:** `a + b << c >= d`
- **Grouping:** `(((a + b) << c) >= d)`

### A1.1400 — `a >= b / c / d`
- **Source:** `a >= b / c / d`
- **Grouping:** `(a >= ((b / c) / d))`

### A1.1401 — `a ^ b > c * d`
- **Source:** `a ^ b > c * d`
- **Grouping:** `((a ^ b) > (c * d))`

### A1.1402 — `a >> b & c < d`
- **Source:** `a >> b & c < d`
- **Grouping:** `(((a >> b) & c) < d)`

### A1.1403 — `a || b + c > d`
- **Source:** `a || b + c > d`
- **Grouping:** `(a || ((b + c) > d))`

### A1.1404 — `a >= b . c << d`
- **Source:** `a >= b . c << d`
- **Grouping:** `(a >= ((b . c) << d))`

### A1.1405 — `a << b || c && d`
- **Source:** `a << b || c && d`
- **Grouping:** `((a << b) || (c && d))`

### A1.1406 — `a > b > c > d`
- **Source:** `a > b > c > d`
- **Grouping:** `(((a > b) > c) > d)`

### A1.1407 — `a & b && c >> d`
- **Source:** `a & b && c >> d`
- **Grouping:** `((a & b) && (c >> d))`

### A1.1408 — `a | b && c | d`
- **Source:** `a | b && c | d`
- **Grouping:** `((a | b) && (c | d))`

### A1.1409 — `a && b < c % d`
- **Source:** `a && b < c % d`
- **Grouping:** `(a && (b < (c % d)))`

### A1.1410 — `a == b == c >> d`
- **Source:** `a == b == c >> d`
- **Grouping:** `((a == b) == (c >> d))`

### A1.1411 — `a * b | c / d`
- **Source:** `a * b | c / d`
- **Grouping:** `((a * b) | (c / d))`

### A1.1412 — `a || b && c * d`
- **Source:** `a || b && c * d`
- **Grouping:** `(a || (b && (c * d)))`

### A1.1413 — `a . b > c && d`
- **Source:** `a . b > c && d`
- **Grouping:** `(((a . b) > c) && d)`

### A1.1414 — `a / b == c % d`
- **Source:** `a / b == c % d`
- **Grouping:** `((a / b) == (c % d))`

### A1.1415 — `a << b * c /= d`
- **Source:** `a << b * c /= d`
- **Grouping:** `((a << (b * c)) /= d)`

### A1.1416 — `a & b > c <= d`
- **Source:** `a & b > c <= d`
- **Grouping:** `(((a & b) > c) <= d)`

### A1.1417 — `a >> b . c * d`
- **Source:** `a >> b . c * d`
- **Grouping:** `(a >> ((b . c) * d))`

### A1.1418 — `a * b ^ c >> d`
- **Source:** `a * b ^ c >> d`
- **Grouping:** `((a * b) ^ (c >> d))`

### A1.1419 — `a + b >= c . d`
- **Source:** `a + b >= c . d`
- **Grouping:** `((a + b) >= (c . d))`

### A1.1420 — `a - b || c - d`
- **Source:** `a - b || c - d`
- **Grouping:** `((a - b) || (c - d))`

### A1.1421 — `a > b - c >= d`
- **Source:** `a > b - c >= d`
- **Grouping:** `((a > (b - c)) >= d)`

### A1.1422 — `a - b ^ c - d`
- **Source:** `a - b ^ c - d`
- **Grouping:** `((a - b) ^ (c - d))`

### A1.1423 — `a == b == c == d`
- **Source:** `a == b == c == d`
- **Grouping:** `(((a == b) == c) == d)`

### A1.1424 — `a < b % c == d`
- **Source:** `a < b % c == d`
- **Grouping:** `((a < (b % c)) == d)`

### A1.1425 — `a + b + c & d`
- **Source:** `a + b + c & d`
- **Grouping:** `(((a + b) + c) & d)`

### A1.1426 — `a /= b && c > d`
- **Source:** `a /= b && c > d`
- **Grouping:** `((a /= b) && (c > d))`

### A1.1427 — `a > b <= c && d`
- **Source:** `a > b <= c && d`
- **Grouping:** `(((a > b) <= c) && d)`

### A1.1428 — `a << b || c /= d`
- **Source:** `a << b || c /= d`
- **Grouping:** `((a << b) || (c /= d))`

### A1.1429 — `a == b >= c - d`
- **Source:** `a == b >= c - d`
- **Grouping:** `((a == b) >= (c - d))`

### A1.1430 — `a <= b >> c << d`
- **Source:** `a <= b >> c << d`
- **Grouping:** `(a <= ((b >> c) << d))`

### A1.1431 — `a || b > c . d`
- **Source:** `a || b > c . d`
- **Grouping:** `(a || (b > (c . d)))`

### A1.1432 — `a >= b | c == d`
- **Source:** `a >= b | c == d`
- **Grouping:** `((a >= (b | c)) == d)`

### A1.1433 — `a - b & c / d`
- **Source:** `a - b & c / d`
- **Grouping:** `((a - b) & (c / d))`

### A1.1434 — `a || b || c && d`
- **Source:** `a || b || c && d`
- **Grouping:** `((a || b) || (c && d))`

### A1.1435 — `a << b == c || d`
- **Source:** `a << b == c || d`
- **Grouping:** `(((a << b) == c) || d)`

### A1.1436 — `a / b & c % d`
- **Source:** `a / b & c % d`
- **Grouping:** `((a / b) & (c % d))`

### A1.1437 — `a >> b >= c % d`
- **Source:** `a >> b >= c % d`
- **Grouping:** `((a >> b) >= (c % d))`

### A1.1438 — `a || b % c << d`
- **Source:** `a || b % c << d`
- **Grouping:** `(a || ((b % c) << d))`

### A1.1439 — `a % b + c /= d`
- **Source:** `a % b + c /= d`
- **Grouping:** `(((a % b) + c) /= d)`

### A1.1440 — `a || b < c == d`
- **Source:** `a || b < c == d`
- **Grouping:** `(a || ((b < c) == d))`

### A1.1441 — `a <= b > c - d`
- **Source:** `a <= b > c - d`
- **Grouping:** `((a <= b) > (c - d))`

### A1.1442 — `a / b + c ^ d`
- **Source:** `a / b + c ^ d`
- **Grouping:** `(((a / b) + c) ^ d)`

### A1.1443 — `a || b > c /= d`
- **Source:** `a || b > c /= d`
- **Grouping:** `(a || ((b > c) /= d))`

### A1.1444 — `a <= b || c & d`
- **Source:** `a <= b || c & d`
- **Grouping:** `((a <= b) || (c & d))`

### A1.1445 — `a == b . c >= d`
- **Source:** `a == b . c >= d`
- **Grouping:** `((a == (b . c)) >= d)`

### A1.1446 — `a > b > c / d`
- **Source:** `a > b > c / d`
- **Grouping:** `((a > b) > (c / d))`

### A1.1447 — `a . b == c / d`
- **Source:** `a . b == c / d`
- **Grouping:** `((a . b) == (c / d))`

### A1.1448 — `a << b || c | d`
- **Source:** `a << b || c | d`
- **Grouping:** `((a << b) || (c | d))`

### A1.1449 — `a && b /= c * d`
- **Source:** `a && b /= c * d`
- **Grouping:** `(a && (b /= (c * d)))`

### A1.1450 — `a ^ b >= c || d`
- **Source:** `a ^ b >= c || d`
- **Grouping:** `(((a ^ b) >= c) || d)`

### A1.1451 — `a >= b * c << d`
- **Source:** `a >= b * c << d`
- **Grouping:** `(a >= ((b * c) << d))`

### A1.1452 — `a | b == c && d`
- **Source:** `a | b == c && d`
- **Grouping:** `(((a | b) == c) && d)`

### A1.1453 — `a - b - c && d`
- **Source:** `a - b - c && d`
- **Grouping:** `(((a - b) - c) && d)`

### A1.1454 — `a >> b >> c < d`
- **Source:** `a >> b >> c < d`
- **Grouping:** `(((a >> b) >> c) < d)`

### A1.1455 — `a << b | c ^ d`
- **Source:** `a << b | c ^ d`
- **Grouping:** `(((a << b) | c) ^ d)`

### A1.1456 — `a << b || c == d`
- **Source:** `a << b || c == d`
- **Grouping:** `((a << b) || (c == d))`

### A1.1457 — `a % b >> c ^ d`
- **Source:** `a % b >> c ^ d`
- **Grouping:** `(((a % b) >> c) ^ d)`

### A1.1458 — `a ^ b / c . d`
- **Source:** `a ^ b / c . d`
- **Grouping:** `(a ^ (b / (c . d)))`

### A1.1459 — `a || b << c . d`
- **Source:** `a || b << c . d`
- **Grouping:** `(a || (b << (c . d)))`

### A1.1460 — `a <= b . c ^ d`
- **Source:** `a <= b . c ^ d`
- **Grouping:** `(a <= ((b . c) ^ d))`

### A1.1461 — `a * b << c ^ d`
- **Source:** `a * b << c ^ d`
- **Grouping:** `(((a * b) << c) ^ d)`

### A1.1462 — `a /= b < c <= d`
- **Source:** `a /= b < c <= d`
- **Grouping:** `(((a /= b) < c) <= d)`

### A1.1463 — `a > b + c - d`
- **Source:** `a > b + c - d`
- **Grouping:** `(a > ((b + c) - d))`

### A1.1464 — `a <= b % c / d`
- **Source:** `a <= b % c / d`
- **Grouping:** `(a <= ((b % c) / d))`

### A1.1465 — `a == b . c & d`
- **Source:** `a == b . c & d`
- **Grouping:** `(a == ((b . c) & d))`

### A1.1466 — `a % b > c - d`
- **Source:** `a % b > c - d`
- **Grouping:** `((a % b) > (c - d))`

### A1.1467 — `a / b || c | d`
- **Source:** `a / b || c | d`
- **Grouping:** `((a / b) || (c | d))`

### A1.1468 — `a || b > c | d`
- **Source:** `a || b > c | d`
- **Grouping:** `(a || (b > (c | d)))`

### A1.1469 — `a - b / c >> d`
- **Source:** `a - b / c >> d`
- **Grouping:** `((a - (b / c)) >> d)`

### A1.1470 — `a == b < c && d`
- **Source:** `a == b < c && d`
- **Grouping:** `(((a == b) < c) && d)`

### A1.1471 — `a + b >= c - d`
- **Source:** `a + b >= c - d`
- **Grouping:** `((a + b) >= (c - d))`

### A1.1472 — `a >= b . c || d`
- **Source:** `a >= b . c || d`
- **Grouping:** `((a >= (b . c)) || d)`

### A1.1473 — `a /= b /= c & d`
- **Source:** `a /= b /= c & d`
- **Grouping:** `((a /= b) /= (c & d))`

### A1.1474 — `a <= b . c << d`
- **Source:** `a <= b . c << d`
- **Grouping:** `(a <= ((b . c) << d))`

### A1.1475 — `a * b * c == d`
- **Source:** `a * b * c == d`
- **Grouping:** `(((a * b) * c) == d)`

### A1.1476 — `a ^ b == c * d`
- **Source:** `a ^ b == c * d`
- **Grouping:** `((a ^ b) == (c * d))`

### A1.1477 — `a <= b < c + d`
- **Source:** `a <= b < c + d`
- **Grouping:** `((a <= b) < (c + d))`

### A1.1478 — `a /= b . c << d`
- **Source:** `a /= b . c << d`
- **Grouping:** `(a /= ((b . c) << d))`

### A1.1479 — `a ^ b * c % d`
- **Source:** `a ^ b * c % d`
- **Grouping:** `(a ^ ((b * c) % d))`

### A1.1480 — `a | b <= c == d`
- **Source:** `a | b <= c == d`
- **Grouping:** `(((a | b) <= c) == d)`

### A1.1481 — `a < b << c | d`
- **Source:** `a < b << c | d`
- **Grouping:** `(a < ((b << c) | d))`

### A1.1482 — `a >= b || c * d`
- **Source:** `a >= b || c * d`
- **Grouping:** `((a >= b) || (c * d))`

### A1.1483 — `a + b | c % d`
- **Source:** `a + b | c % d`
- **Grouping:** `((a + b) | (c % d))`

### A1.1484 — `a * b || c && d`
- **Source:** `a * b || c && d`
- **Grouping:** `((a * b) || (c && d))`

### A1.1485 — `a | b % c / d`
- **Source:** `a | b % c / d`
- **Grouping:** `(a | ((b % c) / d))`

### A1.1486 — `a % b >> c | d`
- **Source:** `a % b >> c | d`
- **Grouping:** `(((a % b) >> c) | d)`

### A1.1487 — `a >> b > c / d`
- **Source:** `a >> b > c / d`
- **Grouping:** `((a >> b) > (c / d))`

### A1.1488 — `a / b == c * d`
- **Source:** `a / b == c * d`
- **Grouping:** `((a / b) == (c * d))`

### A1.1489 — `a == b | c > d`
- **Source:** `a == b | c > d`
- **Grouping:** `((a == (b | c)) > d)`

### A1.1490 — `a & b / c + d`
- **Source:** `a & b / c + d`
- **Grouping:** `(a & ((b / c) + d))`

### A1.1491 — `a + b ^ c >= d`
- **Source:** `a + b ^ c >= d`
- **Grouping:** `(((a + b) ^ c) >= d)`

### A1.1492 — `a & b * c <= d`
- **Source:** `a & b * c <= d`
- **Grouping:** `((a & (b * c)) <= d)`

### A1.1493 — `a + b * c + d`
- **Source:** `a + b * c + d`
- **Grouping:** `((a + (b * c)) + d)`

### A1.1494 — `a % b - c . d`
- **Source:** `a % b - c . d`
- **Grouping:** `((a % b) - (c . d))`

### A1.1495 — `a / b << c == d`
- **Source:** `a / b << c == d`
- **Grouping:** `(((a / b) << c) == d)`

### A1.1496 — `a /= b < c ^ d`
- **Source:** `a /= b < c ^ d`
- **Grouping:** `((a /= b) < (c ^ d))`

### A1.1497 — `a . b > c & d`
- **Source:** `a . b > c & d`
- **Grouping:** `((a . b) > (c & d))`

### A1.1498 — `a > b ^ c <= d`
- **Source:** `a > b ^ c <= d`
- **Grouping:** `((a > (b ^ c)) <= d)`

### A1.1499 — `a <= b . c <= d`
- **Source:** `a <= b . c <= d`
- **Grouping:** `((a <= (b . c)) <= d)`

### A1.1500 — `a && b % c == d`
- **Source:** `a && b % c == d`
- **Grouping:** `(a && ((b % c) == d))`

### A1.1501 — `a || b <= c /= d`
- **Source:** `a || b <= c /= d`
- **Grouping:** `(a || ((b <= c) /= d))`

### A1.1502 — `a >= b * c * d`
- **Source:** `a >= b * c * d`
- **Grouping:** `(a >= ((b * c) * d))`

### A1.1503 — `a /= b > c == d`
- **Source:** `a /= b > c == d`
- **Grouping:** `(((a /= b) > c) == d)`

### A1.1504 — `a | b - c || d`
- **Source:** `a | b - c || d`
- **Grouping:** `((a | (b - c)) || d)`

### A1.1505 — `a + b | c && d`
- **Source:** `a + b | c && d`
- **Grouping:** `(((a + b) | c) && d)`

### A1.1506 — `a <= b + c + d`
- **Source:** `a <= b + c + d`
- **Grouping:** `(a <= ((b + c) + d))`

### A1.1507 — `a + b << c <= d`
- **Source:** `a + b << c <= d`
- **Grouping:** `(((a + b) << c) <= d)`

### A1.1508 — `a - b >> c /= d`
- **Source:** `a - b >> c /= d`
- **Grouping:** `(((a - b) >> c) /= d)`

### A1.1509 — `a == b ^ c . d`
- **Source:** `a == b ^ c . d`
- **Grouping:** `(a == (b ^ (c . d)))`

### A1.1510 — `a < b < c & d`
- **Source:** `a < b < c & d`
- **Grouping:** `((a < b) < (c & d))`

### A1.1511 — `a . b & c - d`
- **Source:** `a . b & c - d`
- **Grouping:** `((a . b) & (c - d))`

### A1.1512 — `a * b == c > d`
- **Source:** `a * b == c > d`
- **Grouping:** `(((a * b) == c) > d)`

### A1.1513 — `a + b << c + d`
- **Source:** `a + b << c + d`
- **Grouping:** `((a + b) << (c + d))`

### A1.1514 — `a . b % c >= d`
- **Source:** `a . b % c >= d`
- **Grouping:** `(((a . b) % c) >= d)`

### A1.1515 — `a / b * c || d`
- **Source:** `a / b * c || d`
- **Grouping:** `(((a / b) * c) || d)`

### A1.1516 — `a && b & c << d`
- **Source:** `a && b & c << d`
- **Grouping:** `(a && (b & (c << d)))`

### A1.1517 — `a <= b * c + d`
- **Source:** `a <= b * c + d`
- **Grouping:** `(a <= ((b * c) + d))`

### A1.1518 — `a < b & c & d`
- **Source:** `a < b & c & d`
- **Grouping:** `(a < ((b & c) & d))`

### A1.1519 — `a ^ b == c - d`
- **Source:** `a ^ b == c - d`
- **Grouping:** `((a ^ b) == (c - d))`

### A1.1520 — `a - b * c / d`
- **Source:** `a - b * c / d`
- **Grouping:** `(a - ((b * c) / d))`

### A1.1521 — `a >= b & c - d`
- **Source:** `a >= b & c - d`
- **Grouping:** `(a >= (b & (c - d)))`

### A1.1522 — `a * b == c || d`
- **Source:** `a * b == c || d`
- **Grouping:** `(((a * b) == c) || d)`

### A1.1523 — `a & b > c - d`
- **Source:** `a & b > c - d`
- **Grouping:** `((a & b) > (c - d))`

### A1.1524 — `a . b >= c > d`
- **Source:** `a . b >= c > d`
- **Grouping:** `(((a . b) >= c) > d)`

### A1.1525 — `a & b - c <= d`
- **Source:** `a & b - c <= d`
- **Grouping:** `((a & (b - c)) <= d)`

### A1.1526 — `a << b - c << d`
- **Source:** `a << b - c << d`
- **Grouping:** `((a << (b - c)) << d)`

### A1.1527 — `a > b >> c & d`
- **Source:** `a > b >> c & d`
- **Grouping:** `(a > ((b >> c) & d))`

### A1.1528 — `a == b . c > d`
- **Source:** `a == b . c > d`
- **Grouping:** `((a == (b . c)) > d)`

### A1.1529 — `a . b >> c | d`
- **Source:** `a . b >> c | d`
- **Grouping:** `(((a . b) >> c) | d)`

### A1.1530 — `a | b >= c == d`
- **Source:** `a | b >= c == d`
- **Grouping:** `(((a | b) >= c) == d)`

### A1.1531 — `a << b == c > d`
- **Source:** `a << b == c > d`
- **Grouping:** `(((a << b) == c) > d)`

### A1.1532 — `a <= b || c % d`
- **Source:** `a <= b || c % d`
- **Grouping:** `((a <= b) || (c % d))`

### A1.1533 — `a <= b /= c & d`
- **Source:** `a <= b /= c & d`
- **Grouping:** `((a <= b) /= (c & d))`

### A1.1534 — `a + b >> c >> d`
- **Source:** `a + b >> c >> d`
- **Grouping:** `(((a + b) >> c) >> d)`

### A1.1535 — `a || b ^ c > d`
- **Source:** `a || b ^ c > d`
- **Grouping:** `(a || ((b ^ c) > d))`

### A1.1536 — `a > b < c << d`
- **Source:** `a > b < c << d`
- **Grouping:** `((a > b) < (c << d))`

### A1.1537 — `a < b | c < d`
- **Source:** `a < b | c < d`
- **Grouping:** `((a < (b | c)) < d)`

### A1.1538 — `a . b == c - d`
- **Source:** `a . b == c - d`
- **Grouping:** `((a . b) == (c - d))`

### A1.1539 — `a / b >> c << d`
- **Source:** `a / b >> c << d`
- **Grouping:** `(((a / b) >> c) << d)`

### A1.1540 — `a >= b + c * d`
- **Source:** `a >= b + c * d`
- **Grouping:** `(a >= (b + (c * d)))`

### A1.1541 — `a /= b || c >= d`
- **Source:** `a /= b || c >= d`
- **Grouping:** `((a /= b) || (c >= d))`

### A1.1542 — `a << b && c >= d`
- **Source:** `a << b && c >= d`
- **Grouping:** `((a << b) && (c >= d))`

### A1.1543 — `a . b | c <= d`
- **Source:** `a . b | c <= d`
- **Grouping:** `(((a . b) | c) <= d)`

### A1.1544 — `a | b < c > d`
- **Source:** `a | b < c > d`
- **Grouping:** `(((a | b) < c) > d)`

### A1.1545 — `a || b == c ^ d`
- **Source:** `a || b == c ^ d`
- **Grouping:** `(a || (b == (c ^ d)))`

### A1.1546 — `a >= b && c /= d`
- **Source:** `a >= b && c /= d`
- **Grouping:** `((a >= b) && (c /= d))`

### A1.1547 — `a - b << c - d`
- **Source:** `a - b << c - d`
- **Grouping:** `((a - b) << (c - d))`

### A1.1548 — `a % b % c . d`
- **Source:** `a % b % c . d`
- **Grouping:** `((a % b) % (c . d))`

### A1.1549 — `a ^ b % c & d`
- **Source:** `a ^ b % c & d`
- **Grouping:** `((a ^ (b % c)) & d)`

### A1.1550 — `a ^ b < c > d`
- **Source:** `a ^ b < c > d`
- **Grouping:** `(((a ^ b) < c) > d)`

### A1.1551 — `a + b /= c >> d`
- **Source:** `a + b /= c >> d`
- **Grouping:** `((a + b) /= (c >> d))`

### A1.1552 — `a % b . c . d`
- **Source:** `a % b . c . d`
- **Grouping:** `(a % (b . (c . d)))`

### A1.1553 — `a << b & c % d`
- **Source:** `a << b & c % d`
- **Grouping:** `((a << b) & (c % d))`

### A1.1554 — `a == b || c * d`
- **Source:** `a == b || c * d`
- **Grouping:** `((a == b) || (c * d))`

### A1.1555 — `a + b || c /= d`
- **Source:** `a + b || c /= d`
- **Grouping:** `((a + b) || (c /= d))`

### A1.1556 — `a + b << c || d`
- **Source:** `a + b << c || d`
- **Grouping:** `(((a + b) << c) || d)`

### A1.1557 — `a + b < c << d`
- **Source:** `a + b < c << d`
- **Grouping:** `((a + b) < (c << d))`

### A1.1558 — `a << b | c <= d`
- **Source:** `a << b | c <= d`
- **Grouping:** `(((a << b) | c) <= d)`

### A1.1559 — `a && b % c /= d`
- **Source:** `a && b % c /= d`
- **Grouping:** `(a && ((b % c) /= d))`

### A1.1560 — `a || b >> c - d`
- **Source:** `a || b >> c - d`
- **Grouping:** `(a || (b >> (c - d)))`

### A1.1561 — `a . b || c ^ d`
- **Source:** `a . b || c ^ d`
- **Grouping:** `((a . b) || (c ^ d))`

### A1.1562 — `a >= b || c < d`
- **Source:** `a >= b || c < d`
- **Grouping:** `((a >= b) || (c < d))`

### A1.1563 — `a <= b & c | d`
- **Source:** `a <= b & c | d`
- **Grouping:** `(a <= ((b & c) | d))`

### A1.1564 — `a && b / c == d`
- **Source:** `a && b / c == d`
- **Grouping:** `(a && ((b / c) == d))`

### A1.1565 — `a << b . c | d`
- **Source:** `a << b . c | d`
- **Grouping:** `((a << (b . c)) | d)`

### A1.1566 — `a % b * c + d`
- **Source:** `a % b * c + d`
- **Grouping:** `(((a % b) * c) + d)`

### A1.1567 — `a % b > c % d`
- **Source:** `a % b > c % d`
- **Grouping:** `((a % b) > (c % d))`

### A1.1568 — `a < b && c ^ d`
- **Source:** `a < b && c ^ d`
- **Grouping:** `((a < b) && (c ^ d))`

### A1.1569 — `a / b > c <= d`
- **Source:** `a / b > c <= d`
- **Grouping:** `(((a / b) > c) <= d)`

### A1.1570 — `a | b - c & d`
- **Source:** `a | b - c & d`
- **Grouping:** `((a | (b - c)) & d)`

### A1.1571 — `a - b >> c >> d`
- **Source:** `a - b >> c >> d`
- **Grouping:** `(((a - b) >> c) >> d)`

### A1.1572 — `a == b . c < d`
- **Source:** `a == b . c < d`
- **Grouping:** `((a == (b . c)) < d)`

### A1.1573 — `a / b / c . d`
- **Source:** `a / b / c . d`
- **Grouping:** `((a / b) / (c . d))`

### A1.1574 — `a + b > c & d`
- **Source:** `a + b > c & d`
- **Grouping:** `((a + b) > (c & d))`

### A1.1575 — `a /= b % c /= d`
- **Source:** `a /= b % c /= d`
- **Grouping:** `((a /= (b % c)) /= d)`

### A1.1576 — `a + b /= c <= d`
- **Source:** `a + b /= c <= d`
- **Grouping:** `(((a + b) /= c) <= d)`

### A1.1577 — `a << b . c & d`
- **Source:** `a << b . c & d`
- **Grouping:** `((a << (b . c)) & d)`

### A1.1578 — `a * b - c < d`
- **Source:** `a * b - c < d`
- **Grouping:** `(((a * b) - c) < d)`

### A1.1579 — `a % b >= c && d`
- **Source:** `a % b >= c && d`
- **Grouping:** `(((a % b) >= c) && d)`

### A1.1580 — `a < b || c << d`
- **Source:** `a < b || c << d`
- **Grouping:** `((a < b) || (c << d))`

### A1.1581 — `a & b & c || d`
- **Source:** `a & b & c || d`
- **Grouping:** `(((a & b) & c) || d)`

### A1.1582 — `a << b - c && d`
- **Source:** `a << b - c && d`
- **Grouping:** `((a << (b - c)) && d)`

### A1.1583 — `a >= b / c || d`
- **Source:** `a >= b / c || d`
- **Grouping:** `((a >= (b / c)) || d)`

### A1.1584 — `a /= b > c >> d`
- **Source:** `a /= b > c >> d`
- **Grouping:** `((a /= b) > (c >> d))`

### A1.1585 — `a && b - c >> d`
- **Source:** `a && b - c >> d`
- **Grouping:** `(a && ((b - c) >> d))`

### A1.1586 — `a < b >= c + d`
- **Source:** `a < b >= c + d`
- **Grouping:** `((a < b) >= (c + d))`

### A1.1587 — `a || b == c /= d`
- **Source:** `a || b == c /= d`
- **Grouping:** `(a || ((b == c) /= d))`

### A1.1588 — `a % b >> c & d`
- **Source:** `a % b >> c & d`
- **Grouping:** `(((a % b) >> c) & d)`

### A1.1589 — `a | b >> c ^ d`
- **Source:** `a | b >> c ^ d`
- **Grouping:** `((a | (b >> c)) ^ d)`

### A1.1590 — `a && b + c >> d`
- **Source:** `a && b + c >> d`
- **Grouping:** `(a && ((b + c) >> d))`

### A1.1591 — `a | b > c > d`
- **Source:** `a | b > c > d`
- **Grouping:** `(((a | b) > c) > d)`

### A1.1592 — `a ^ b | c || d`
- **Source:** `a ^ b | c || d`
- **Grouping:** `(((a ^ b) | c) || d)`

### A1.1593 — `a <= b < c < d`
- **Source:** `a <= b < c < d`
- **Grouping:** `(((a <= b) < c) < d)`

### A1.1594 — `a || b == c - d`
- **Source:** `a || b == c - d`
- **Grouping:** `(a || (b == (c - d)))`

### A1.1595 — `a % b /= c | d`
- **Source:** `a % b /= c | d`
- **Grouping:** `((a % b) /= (c | d))`

### A1.1596 — `a & b ^ c && d`
- **Source:** `a & b ^ c && d`
- **Grouping:** `(((a & b) ^ c) && d)`

### A1.1597 — `a && b % c & d`
- **Source:** `a && b % c & d`
- **Grouping:** `(a && ((b % c) & d))`

### A1.1598 — `a - b + c . d`
- **Source:** `a - b + c . d`
- **Grouping:** `((a - b) + (c . d))`

### A1.1599 — `a | b + c || d`
- **Source:** `a | b + c || d`
- **Grouping:** `((a | (b + c)) || d)`

### A1.1600 — `a & b & c % d`
- **Source:** `a & b & c % d`
- **Grouping:** `((a & b) & (c % d))`

### A1.1601 — `a & b - c >> d`
- **Source:** `a & b - c >> d`
- **Grouping:** `(a & ((b - c) >> d))`

### A1.1602 — `a ^ b <= c << d`
- **Source:** `a ^ b <= c << d`
- **Grouping:** `((a ^ b) <= (c << d))`

### A1.1603 — `a >= b >> c >> d`
- **Source:** `a >= b >> c >> d`
- **Grouping:** `(a >= ((b >> c) >> d))`

### A1.1604 — `a || b || c < d`
- **Source:** `a || b || c < d`
- **Grouping:** `((a || b) || (c < d))`

### A1.1605 — `a <= b > c % d`
- **Source:** `a <= b > c % d`
- **Grouping:** `((a <= b) > (c % d))`

### A1.1606 — `a . b . c > d`
- **Source:** `a . b . c > d`
- **Grouping:** `((a . (b . c)) > d)`

### A1.1607 — `a >> b >= c == d`
- **Source:** `a >> b >= c == d`
- **Grouping:** `(((a >> b) >= c) == d)`

### A1.1608 — `a | b || c <= d`
- **Source:** `a | b || c <= d`
- **Grouping:** `((a | b) || (c <= d))`

### A1.1609 — `a < b < c - d`
- **Source:** `a < b < c - d`
- **Grouping:** `((a < b) < (c - d))`

### A1.1610 — `a < b /= c - d`
- **Source:** `a < b /= c - d`
- **Grouping:** `((a < b) /= (c - d))`

### A1.1611 — `a >= b /= c > d`
- **Source:** `a >= b /= c > d`
- **Grouping:** `(((a >= b) /= c) > d)`

### A1.1612 — `a || b . c + d`
- **Source:** `a || b . c + d`
- **Grouping:** `(a || ((b . c) + d))`

### A1.1613 — `a * b > c << d`
- **Source:** `a * b > c << d`
- **Grouping:** `((a * b) > (c << d))`

### A1.1614 — `a / b % c > d`
- **Source:** `a / b % c > d`
- **Grouping:** `(((a / b) % c) > d)`

### A1.1615 — `a || b . c >> d`
- **Source:** `a || b . c >> d`
- **Grouping:** `(a || ((b . c) >> d))`

### A1.1616 — `a . b * c + d`
- **Source:** `a . b * c + d`
- **Grouping:** `(((a . b) * c) + d)`

### A1.1617 — `a / b . c >= d`
- **Source:** `a / b . c >= d`
- **Grouping:** `((a / (b . c)) >= d)`

### A1.1618 — `a >> b && c ^ d`
- **Source:** `a >> b && c ^ d`
- **Grouping:** `((a >> b) && (c ^ d))`

### A1.1619 — `a | b < c == d`
- **Source:** `a | b < c == d`
- **Grouping:** `(((a | b) < c) == d)`

### A1.1620 — `a & b * c + d`
- **Source:** `a & b * c + d`
- **Grouping:** `(a & ((b * c) + d))`

### A1.1621 — `a == b & c >= d`
- **Source:** `a == b & c >= d`
- **Grouping:** `((a == (b & c)) >= d)`

### A1.1622 — `a * b || c % d`
- **Source:** `a * b || c % d`
- **Grouping:** `((a * b) || (c % d))`

### A1.1623 — `a | b > c ^ d`
- **Source:** `a | b > c ^ d`
- **Grouping:** `((a | b) > (c ^ d))`

### A1.1624 — `a <= b | c & d`
- **Source:** `a <= b | c & d`
- **Grouping:** `(a <= ((b | c) & d))`

### A1.1625 — `a >= b / c | d`
- **Source:** `a >= b / c | d`
- **Grouping:** `(a >= ((b / c) | d))`

### A1.1626 — `a >> b - c > d`
- **Source:** `a >> b - c > d`
- **Grouping:** `((a >> (b - c)) > d)`

### A1.1627 — `a < b | c /= d`
- **Source:** `a < b | c /= d`
- **Grouping:** `((a < (b | c)) /= d)`

### A1.1628 — `a + b + c % d`
- **Source:** `a + b + c % d`
- **Grouping:** `((a + b) + (c % d))`

### A1.1629 — `a << b | c >= d`
- **Source:** `a << b | c >= d`
- **Grouping:** `(((a << b) | c) >= d)`

### A1.1630 — `a <= b >= c < d`
- **Source:** `a <= b >= c < d`
- **Grouping:** `(((a <= b) >= c) < d)`

### A1.1631 — `a - b <= c + d`
- **Source:** `a - b <= c + d`
- **Grouping:** `((a - b) <= (c + d))`

### A1.1632 — `a * b / c % d`
- **Source:** `a * b / c % d`
- **Grouping:** `(((a * b) / c) % d)`

### A1.1633 — `a - b + c < d`
- **Source:** `a - b + c < d`
- **Grouping:** `(((a - b) + c) < d)`

### A1.1634 — `a | b / c | d`
- **Source:** `a | b / c | d`
- **Grouping:** `((a | (b / c)) | d)`

### A1.1635 — `a <= b < c || d`
- **Source:** `a <= b < c || d`
- **Grouping:** `(((a <= b) < c) || d)`

### A1.1636 — `a << b == c + d`
- **Source:** `a << b == c + d`
- **Grouping:** `((a << b) == (c + d))`

### A1.1637 — `a | b ^ c >> d`
- **Source:** `a | b ^ c >> d`
- **Grouping:** `((a | b) ^ (c >> d))`

### A1.1638 — `a << b <= c . d`
- **Source:** `a << b <= c . d`
- **Grouping:** `((a << b) <= (c . d))`

### A1.1639 — `a . b <= c < d`
- **Source:** `a . b <= c < d`
- **Grouping:** `(((a . b) <= c) < d)`

### A1.1640 — `a % b ^ c /= d`
- **Source:** `a % b ^ c /= d`
- **Grouping:** `(((a % b) ^ c) /= d)`

### A1.1641 — `a || b < c | d`
- **Source:** `a || b < c | d`
- **Grouping:** `(a || (b < (c | d)))`

### A1.1642 — `a . b | c << d`
- **Source:** `a . b | c << d`
- **Grouping:** `((a . b) | (c << d))`

### A1.1643 — `a * b >> c / d`
- **Source:** `a * b >> c / d`
- **Grouping:** `((a * b) >> (c / d))`

### A1.1644 — `a >= b + c | d`
- **Source:** `a >= b + c | d`
- **Grouping:** `(a >= ((b + c) | d))`

### A1.1645 — `a % b > c + d`
- **Source:** `a % b > c + d`
- **Grouping:** `((a % b) > (c + d))`

### A1.1646 — `a && b . c << d`
- **Source:** `a && b . c << d`
- **Grouping:** `(a && ((b . c) << d))`

### A1.1647 — `a + b - c && d`
- **Source:** `a + b - c && d`
- **Grouping:** `(((a + b) - c) && d)`

### A1.1648 — `a < b && c - d`
- **Source:** `a < b && c - d`
- **Grouping:** `((a < b) && (c - d))`

### A1.1649 — `a || b >= c / d`
- **Source:** `a || b >= c / d`
- **Grouping:** `(a || (b >= (c / d)))`

### A1.1650 — `a < b == c || d`
- **Source:** `a < b == c || d`
- **Grouping:** `(((a < b) == c) || d)`

### A1.1651 — `a == b >= c ^ d`
- **Source:** `a == b >= c ^ d`
- **Grouping:** `((a == b) >= (c ^ d))`

### A1.1652 — `a * b && c <= d`
- **Source:** `a * b && c <= d`
- **Grouping:** `((a * b) && (c <= d))`

### A1.1653 — `a & b + c >> d`
- **Source:** `a & b + c >> d`
- **Grouping:** `(a & ((b + c) >> d))`

### A1.1654 — `a >= b == c && d`
- **Source:** `a >= b == c && d`
- **Grouping:** `(((a >= b) == c) && d)`

### A1.1655 — `a . b || c >> d`
- **Source:** `a . b || c >> d`
- **Grouping:** `((a . b) || (c >> d))`

### A1.1656 — `a % b ^ c - d`
- **Source:** `a % b ^ c - d`
- **Grouping:** `((a % b) ^ (c - d))`

### A1.1657 — `a /= b . c & d`
- **Source:** `a /= b . c & d`
- **Grouping:** `(a /= ((b . c) & d))`

### A1.1658 — `a | b / c * d`
- **Source:** `a | b / c * d`
- **Grouping:** `(a | ((b / c) * d))`

### A1.1659 — `a == b >= c == d`
- **Source:** `a == b >= c == d`
- **Grouping:** `(((a == b) >= c) == d)`

### A1.1660 — `a + b / c && d`
- **Source:** `a + b / c && d`
- **Grouping:** `((a + (b / c)) && d)`

### A1.1661 — `a . b && c & d`
- **Source:** `a . b && c & d`
- **Grouping:** `((a . b) && (c & d))`

### A1.1662 — `a < b ^ c | d`
- **Source:** `a < b ^ c | d`
- **Grouping:** `(a < ((b ^ c) | d))`

### A1.1663 — `a + b || c ^ d`
- **Source:** `a + b || c ^ d`
- **Grouping:** `((a + b) || (c ^ d))`

### A1.1664 — `a || b + c / d`
- **Source:** `a || b + c / d`
- **Grouping:** `(a || (b + (c / d)))`

### A1.1665 — `a /= b * c / d`
- **Source:** `a /= b * c / d`
- **Grouping:** `(a /= ((b * c) / d))`

### A1.1666 — `a | b * c << d`
- **Source:** `a | b * c << d`
- **Grouping:** `(a | ((b * c) << d))`

### A1.1667 — `a << b * c ^ d`
- **Source:** `a << b * c ^ d`
- **Grouping:** `((a << (b * c)) ^ d)`

### A1.1668 — `a /= b << c . d`
- **Source:** `a /= b << c . d`
- **Grouping:** `(a /= (b << (c . d)))`

### A1.1669 — `a ^ b | c && d`
- **Source:** `a ^ b | c && d`
- **Grouping:** `(((a ^ b) | c) && d)`

### A1.1670 — `a < b && c / d`
- **Source:** `a < b && c / d`
- **Grouping:** `((a < b) && (c / d))`

### A1.1671 — `a <= b >= c > d`
- **Source:** `a <= b >= c > d`
- **Grouping:** `(((a <= b) >= c) > d)`

### A1.1672 — `a > b << c ^ d`
- **Source:** `a > b << c ^ d`
- **Grouping:** `(a > ((b << c) ^ d))`

### A1.1673 — `a || b ^ c == d`
- **Source:** `a || b ^ c == d`
- **Grouping:** `(a || ((b ^ c) == d))`

### A1.1674 — `a >= b | c - d`
- **Source:** `a >= b | c - d`
- **Grouping:** `(a >= (b | (c - d)))`

### A1.1675 — `a << b ^ c << d`
- **Source:** `a << b ^ c << d`
- **Grouping:** `((a << b) ^ (c << d))`

### A1.1676 — `a - b < c <= d`
- **Source:** `a - b < c <= d`
- **Grouping:** `(((a - b) < c) <= d)`

### A1.1677 — `a | b <= c % d`
- **Source:** `a | b <= c % d`
- **Grouping:** `((a | b) <= (c % d))`

### A1.1678 — `a >= b - c << d`
- **Source:** `a >= b - c << d`
- **Grouping:** `(a >= ((b - c) << d))`

### A1.1679 — `a % b >= c + d`
- **Source:** `a % b >= c + d`
- **Grouping:** `((a % b) >= (c + d))`

### A1.1680 — `a ^ b | c < d`
- **Source:** `a ^ b | c < d`
- **Grouping:** `(((a ^ b) | c) < d)`

### A1.1681 — `a + b & c >> d`
- **Source:** `a + b & c >> d`
- **Grouping:** `((a + b) & (c >> d))`

### A1.1682 — `a / b < c < d`
- **Source:** `a / b < c < d`
- **Grouping:** `(((a / b) < c) < d)`

### A1.1683 — `a ^ b > c ^ d`
- **Source:** `a ^ b > c ^ d`
- **Grouping:** `((a ^ b) > (c ^ d))`

### A1.1684 — `a / b - c / d`
- **Source:** `a / b - c / d`
- **Grouping:** `((a / b) - (c / d))`

### A1.1685 — `a < b >> c >> d`
- **Source:** `a < b >> c >> d`
- **Grouping:** `(a < ((b >> c) >> d))`

### A1.1686 — `a - b && c >> d`
- **Source:** `a - b && c >> d`
- **Grouping:** `((a - b) && (c >> d))`

### A1.1687 — `a / b <= c && d`
- **Source:** `a / b <= c && d`
- **Grouping:** `(((a / b) <= c) && d)`

### A1.1688 — `a / b || c - d`
- **Source:** `a / b || c - d`
- **Grouping:** `((a / b) || (c - d))`

### A1.1689 — `a || b % c < d`
- **Source:** `a || b % c < d`
- **Grouping:** `(a || ((b % c) < d))`

### A1.1690 — `a & b / c - d`
- **Source:** `a & b / c - d`
- **Grouping:** `(a & ((b / c) - d))`

### A1.1691 — `a * b - c . d`
- **Source:** `a * b - c . d`
- **Grouping:** `((a * b) - (c . d))`

### A1.1692 — `a + b - c >> d`
- **Source:** `a + b - c >> d`
- **Grouping:** `(((a + b) - c) >> d)`

### A1.1693 — `a . b * c & d`
- **Source:** `a . b * c & d`
- **Grouping:** `(((a . b) * c) & d)`

### A1.1694 — `a / b >= c << d`
- **Source:** `a / b >= c << d`
- **Grouping:** `((a / b) >= (c << d))`

### A1.1695 — `a << b || c - d`
- **Source:** `a << b || c - d`
- **Grouping:** `((a << b) || (c - d))`

### A1.1696 — `a . b /= c >= d`
- **Source:** `a . b /= c >= d`
- **Grouping:** `(((a . b) /= c) >= d)`

### A1.1697 — `a /= b << c < d`
- **Source:** `a /= b << c < d`
- **Grouping:** `((a /= (b << c)) < d)`

### A1.1698 — `a > b * c >= d`
- **Source:** `a > b * c >= d`
- **Grouping:** `((a > (b * c)) >= d)`

### A1.1699 — `a && b + c + d`
- **Source:** `a && b + c + d`
- **Grouping:** `(a && ((b + c) + d))`

### A1.1700 — `a < b == c <= d`
- **Source:** `a < b == c <= d`
- **Grouping:** `(((a < b) == c) <= d)`

### A1.1701 — `a | b & c * d`
- **Source:** `a | b & c * d`
- **Grouping:** `((a | b) & (c * d))`

### A1.1702 — `a / b << c >= d`
- **Source:** `a / b << c >= d`
- **Grouping:** `(((a / b) << c) >= d)`

### A1.1703 — `a << b ^ c && d`
- **Source:** `a << b ^ c && d`
- **Grouping:** `(((a << b) ^ c) && d)`

### A1.1704 — `a < b ^ c / d`
- **Source:** `a < b ^ c / d`
- **Grouping:** `(a < (b ^ (c / d)))`

### A1.1705 — `a == b > c && d`
- **Source:** `a == b > c && d`
- **Grouping:** `(((a == b) > c) && d)`

### A1.1706 — `a << b % c && d`
- **Source:** `a << b % c && d`
- **Grouping:** `((a << (b % c)) && d)`

### A1.1707 — `a % b /= c == d`
- **Source:** `a % b /= c == d`
- **Grouping:** `(((a % b) /= c) == d)`

### A1.1708 — `a . b / c - d`
- **Source:** `a . b / c - d`
- **Grouping:** `(((a . b) / c) - d)`

### A1.1709 — `a && b == c > d`
- **Source:** `a && b == c > d`
- **Grouping:** `(a && ((b == c) > d))`

### A1.1710 — `a + b >> c * d`
- **Source:** `a + b >> c * d`
- **Grouping:** `((a + b) >> (c * d))`

### A1.1711 — `a % b & c . d`
- **Source:** `a % b & c . d`
- **Grouping:** `((a % b) & (c . d))`

### A1.1712 — `a >= b == c * d`
- **Source:** `a >= b == c * d`
- **Grouping:** `((a >= b) == (c * d))`

### A1.1713 — `a - b && c . d`
- **Source:** `a - b && c . d`
- **Grouping:** `((a - b) && (c . d))`

### A1.1714 — `a . b /= c >> d`
- **Source:** `a . b /= c >> d`
- **Grouping:** `((a . b) /= (c >> d))`

### A1.1715 — `a << b . c >> d`
- **Source:** `a << b . c >> d`
- **Grouping:** `((a << (b . c)) >> d)`

### A1.1716 — `a && b > c - d`
- **Source:** `a && b > c - d`
- **Grouping:** `(a && (b > (c - d)))`

### A1.1717 — `a | b % c * d`
- **Source:** `a | b % c * d`
- **Grouping:** `(a | ((b % c) * d))`

### A1.1718 — `a == b . c - d`
- **Source:** `a == b . c - d`
- **Grouping:** `(a == ((b . c) - d))`

### A1.1719 — `a >> b >> c * d`
- **Source:** `a >> b >> c * d`
- **Grouping:** `((a >> b) >> (c * d))`

### A1.1720 — `a & b > c % d`
- **Source:** `a & b > c % d`
- **Grouping:** `((a & b) > (c % d))`

### A1.1721 — `a | b >> c /= d`
- **Source:** `a | b >> c /= d`
- **Grouping:** `((a | (b >> c)) /= d)`

### A1.1722 — `a * b <= c && d`
- **Source:** `a * b <= c && d`
- **Grouping:** `(((a * b) <= c) && d)`

### A1.1723 — `a & b >> c || d`
- **Source:** `a & b >> c || d`
- **Grouping:** `((a & (b >> c)) || d)`

### A1.1724 — `a * b && c / d`
- **Source:** `a * b && c / d`
- **Grouping:** `((a * b) && (c / d))`

### A1.1725 — `a && b & c <= d`
- **Source:** `a && b & c <= d`
- **Grouping:** `(a && ((b & c) <= d))`

### A1.1726 — `a > b | c . d`
- **Source:** `a > b | c . d`
- **Grouping:** `(a > (b | (c . d)))`

### A1.1727 — `a | b >= c /= d`
- **Source:** `a | b >= c /= d`
- **Grouping:** `(((a | b) >= c) /= d)`

### A1.1728 — `a << b - c ^ d`
- **Source:** `a << b - c ^ d`
- **Grouping:** `((a << (b - c)) ^ d)`

### A1.1729 — `a % b << c % d`
- **Source:** `a % b << c % d`
- **Grouping:** `((a % b) << (c % d))`

### A1.1730 — `a % b / c << d`
- **Source:** `a % b / c << d`
- **Grouping:** `(((a % b) / c) << d)`

### A1.1731 — `a > b >= c - d`
- **Source:** `a > b >= c - d`
- **Grouping:** `((a > b) >= (c - d))`

### A1.1732 — `a < b - c < d`
- **Source:** `a < b - c < d`
- **Grouping:** `((a < (b - c)) < d)`

### A1.1733 — `a <= b <= c >> d`
- **Source:** `a <= b <= c >> d`
- **Grouping:** `((a <= b) <= (c >> d))`

### A1.1734 — `a - b | c << d`
- **Source:** `a - b | c << d`
- **Grouping:** `((a - b) | (c << d))`

### A1.1735 — `a & b | c <= d`
- **Source:** `a & b | c <= d`
- **Grouping:** `(((a & b) | c) <= d)`

### A1.1736 — `a == b >= c . d`
- **Source:** `a == b >= c . d`
- **Grouping:** `((a == b) >= (c . d))`

### A1.1737 — `a / b . c && d`
- **Source:** `a / b . c && d`
- **Grouping:** `((a / (b . c)) && d)`

### A1.1738 — `a >> b < c < d`
- **Source:** `a >> b < c < d`
- **Grouping:** `(((a >> b) < c) < d)`

### A1.1739 — `a <= b >= c & d`
- **Source:** `a <= b >= c & d`
- **Grouping:** `((a <= b) >= (c & d))`

### A1.1740 — `a & b /= c % d`
- **Source:** `a & b /= c % d`
- **Grouping:** `((a & b) /= (c % d))`

### A1.1741 — `a >= b > c << d`
- **Source:** `a >= b > c << d`
- **Grouping:** `((a >= b) > (c << d))`

### A1.1742 — `a . b + c % d`
- **Source:** `a . b + c % d`
- **Grouping:** `((a . b) + (c % d))`

### A1.1743 — `a << b == c < d`
- **Source:** `a << b == c < d`
- **Grouping:** `(((a << b) == c) < d)`

### A1.1744 — `a >= b << c + d`
- **Source:** `a >= b << c + d`
- **Grouping:** `(a >= (b << (c + d)))`

### A1.1745 — `a ^ b << c > d`
- **Source:** `a ^ b << c > d`
- **Grouping:** `((a ^ (b << c)) > d)`

### A1.1746 — `a / b && c && d`
- **Source:** `a / b && c && d`
- **Grouping:** `(((a / b) && c) && d)`

### A1.1747 — `a && b - c >= d`
- **Source:** `a && b - c >= d`
- **Grouping:** `(a && ((b - c) >= d))`

### A1.1748 — `a == b . c <= d`
- **Source:** `a == b . c <= d`
- **Grouping:** `((a == (b . c)) <= d)`

### A1.1749 — `a + b || c + d`
- **Source:** `a + b || c + d`
- **Grouping:** `((a + b) || (c + d))`

### A1.1750 — `a || b % c . d`
- **Source:** `a || b % c . d`
- **Grouping:** `(a || (b % (c . d)))`

### A1.1751 — `a >= b + c & d`
- **Source:** `a >= b + c & d`
- **Grouping:** `(a >= ((b + c) & d))`

### A1.1752 — `a && b >= c + d`
- **Source:** `a && b >= c + d`
- **Grouping:** `(a && (b >= (c + d)))`

### A1.1753 — `a >= b <= c % d`
- **Source:** `a >= b <= c % d`
- **Grouping:** `((a >= b) <= (c % d))`

### A1.1754 — `a < b - c / d`
- **Source:** `a < b - c / d`
- **Grouping:** `(a < (b - (c / d)))`

### A1.1755 — `a ^ b || c /= d`
- **Source:** `a ^ b || c /= d`
- **Grouping:** `((a ^ b) || (c /= d))`

### A1.1756 — `a /= b < c + d`
- **Source:** `a /= b < c + d`
- **Grouping:** `((a /= b) < (c + d))`

### A1.1757 — `a && b + c < d`
- **Source:** `a && b + c < d`
- **Grouping:** `(a && ((b + c) < d))`

### A1.1758 — `a - b - c % d`
- **Source:** `a - b - c % d`
- **Grouping:** `((a - b) - (c % d))`

### A1.1759 — `a / b > c >> d`
- **Source:** `a / b > c >> d`
- **Grouping:** `((a / b) > (c >> d))`

### A1.1760 — `a - b >> c | d`
- **Source:** `a - b >> c | d`
- **Grouping:** `(((a - b) >> c) | d)`

### A1.1761 — `a < b % c . d`
- **Source:** `a < b % c . d`
- **Grouping:** `(a < (b % (c . d)))`

### A1.1762 — `a ^ b > c || d`
- **Source:** `a ^ b > c || d`
- **Grouping:** `(((a ^ b) > c) || d)`

### A1.1763 — `a & b % c >= d`
- **Source:** `a & b % c >= d`
- **Grouping:** `((a & (b % c)) >= d)`

### A1.1764 — `a << b | c || d`
- **Source:** `a << b | c || d`
- **Grouping:** `(((a << b) | c) || d)`

### A1.1765 — `a - b & c ^ d`
- **Source:** `a - b & c ^ d`
- **Grouping:** `(((a - b) & c) ^ d)`

### A1.1766 — `a & b && c /= d`
- **Source:** `a & b && c /= d`
- **Grouping:** `((a & b) && (c /= d))`

### A1.1767 — `a || b < c /= d`
- **Source:** `a || b < c /= d`
- **Grouping:** `(a || ((b < c) /= d))`

### A1.1768 — `a - b . c . d`
- **Source:** `a - b . c . d`
- **Grouping:** `(a - (b . (c . d)))`

### A1.1769 — `a && b ^ c & d`
- **Source:** `a && b ^ c & d`
- **Grouping:** `(a && ((b ^ c) & d))`

### A1.1770 — `a /= b <= c >> d`
- **Source:** `a /= b <= c >> d`
- **Grouping:** `((a /= b) <= (c >> d))`

### A1.1771 — `a & b >= c | d`
- **Source:** `a & b >= c | d`
- **Grouping:** `((a & b) >= (c | d))`

### A1.1772 — `a | b >= c | d`
- **Source:** `a | b >= c | d`
- **Grouping:** `((a | b) >= (c | d))`

### A1.1773 — `a >> b - c >> d`
- **Source:** `a >> b - c >> d`
- **Grouping:** `((a >> (b - c)) >> d)`

### A1.1774 — `a + b / c % d`
- **Source:** `a + b / c % d`
- **Grouping:** `(a + ((b / c) % d))`

### A1.1775 — `a && b - c / d`
- **Source:** `a && b - c / d`
- **Grouping:** `(a && (b - (c / d)))`

### A1.1776 — `a . b & c | d`
- **Source:** `a . b & c | d`
- **Grouping:** `(((a . b) & c) | d)`

### A1.1777 — `a % b - c > d`
- **Source:** `a % b - c > d`
- **Grouping:** `(((a % b) - c) > d)`

### A1.1778 — `a << b | c << d`
- **Source:** `a << b | c << d`
- **Grouping:** `((a << b) | (c << d))`

### A1.1779 — `a - b + c >> d`
- **Source:** `a - b + c >> d`
- **Grouping:** `(((a - b) + c) >> d)`

### A1.1780 — `a >> b == c < d`
- **Source:** `a >> b == c < d`
- **Grouping:** `(((a >> b) == c) < d)`

### A1.1781 — `a <= b <= c & d`
- **Source:** `a <= b <= c & d`
- **Grouping:** `((a <= b) <= (c & d))`

### A1.1782 — `a * b && c >= d`
- **Source:** `a * b && c >= d`
- **Grouping:** `((a * b) && (c >= d))`

### A1.1783 — `a >= b || c /= d`
- **Source:** `a >= b || c /= d`
- **Grouping:** `((a >= b) || (c /= d))`

### A1.1784 — `a . b || c <= d`
- **Source:** `a . b || c <= d`
- **Grouping:** `((a . b) || (c <= d))`

### A1.1785 — `a >> b % c - d`
- **Source:** `a >> b % c - d`
- **Grouping:** `(a >> ((b % c) - d))`

### A1.1786 — `a + b /= c == d`
- **Source:** `a + b /= c == d`
- **Grouping:** `(((a + b) /= c) == d)`

### A1.1787 — `a + b == c == d`
- **Source:** `a + b == c == d`
- **Grouping:** `(((a + b) == c) == d)`

### A1.1788 — `a | b ^ c | d`
- **Source:** `a | b ^ c | d`
- **Grouping:** `(((a | b) ^ c) | d)`

### A1.1789 — `a > b << c > d`
- **Source:** `a > b << c > d`
- **Grouping:** `((a > (b << c)) > d)`

### A1.1790 — `a < b + c /= d`
- **Source:** `a < b + c /= d`
- **Grouping:** `((a < (b + c)) /= d)`

### A1.1791 — `a >= b + c / d`
- **Source:** `a >= b + c / d`
- **Grouping:** `(a >= (b + (c / d)))`

### A1.1792 — `a >= b + c || d`
- **Source:** `a >= b + c || d`
- **Grouping:** `((a >= (b + c)) || d)`

### A1.1793 — `a | b < c + d`
- **Source:** `a | b < c + d`
- **Grouping:** `((a | b) < (c + d))`

### A1.1794 — `a < b ^ c < d`
- **Source:** `a < b ^ c < d`
- **Grouping:** `((a < (b ^ c)) < d)`

### A1.1795 — `a || b ^ c && d`
- **Source:** `a || b ^ c && d`
- **Grouping:** `(a || ((b ^ c) && d))`

### A1.1796 — `a | b * c - d`
- **Source:** `a | b * c - d`
- **Grouping:** `(a | ((b * c) - d))`

### A1.1797 — `a % b <= c == d`
- **Source:** `a % b <= c == d`
- **Grouping:** `(((a % b) <= c) == d)`

### A1.1798 — `a || b << c >> d`
- **Source:** `a || b << c >> d`
- **Grouping:** `(a || ((b << c) >> d))`

### A1.1799 — `a <= b - c & d`
- **Source:** `a <= b - c & d`
- **Grouping:** `(a <= ((b - c) & d))`

### A1.1800 — `a <= b <= c . d`
- **Source:** `a <= b <= c . d`
- **Grouping:** `((a <= b) <= (c . d))`

### A1.1801 — `a <= b & c & d`
- **Source:** `a <= b & c & d`
- **Grouping:** `(a <= ((b & c) & d))`

### A1.1802 — `a >= b - c - d`
- **Source:** `a >= b - c - d`
- **Grouping:** `(a >= ((b - c) - d))`

### A1.1803 — `a - b < c && d`
- **Source:** `a - b < c && d`
- **Grouping:** `(((a - b) < c) && d)`

### A1.1804 — `a / b || c /= d`
- **Source:** `a / b || c /= d`
- **Grouping:** `((a / b) || (c /= d))`

### A1.1805 — `a ^ b % c / d`
- **Source:** `a ^ b % c / d`
- **Grouping:** `(a ^ ((b % c) / d))`

### A1.1806 — `a < b . c && d`
- **Source:** `a < b . c && d`
- **Grouping:** `((a < (b . c)) && d)`

### A1.1807 — `a && b % c < d`
- **Source:** `a && b % c < d`
- **Grouping:** `(a && ((b % c) < d))`

### A1.1808 — `a & b . c /= d`
- **Source:** `a & b . c /= d`
- **Grouping:** `((a & (b . c)) /= d)`

### A1.1809 — `a >= b + c < d`
- **Source:** `a >= b + c < d`
- **Grouping:** `((a >= (b + c)) < d)`

### A1.1810 — `a . b && c > d`
- **Source:** `a . b && c > d`
- **Grouping:** `((a . b) && (c > d))`

### A1.1811 — `a & b <= c % d`
- **Source:** `a & b <= c % d`
- **Grouping:** `((a & b) <= (c % d))`

### A1.1812 — `a + b . c ^ d`
- **Source:** `a + b . c ^ d`
- **Grouping:** `((a + (b . c)) ^ d)`

### A1.1813 — `a - b == c << d`
- **Source:** `a - b == c << d`
- **Grouping:** `((a - b) == (c << d))`

### A1.1814 — `a / b / c == d`
- **Source:** `a / b / c == d`
- **Grouping:** `(((a / b) / c) == d)`

### A1.1815 — `a ^ b / c | d`
- **Source:** `a ^ b / c | d`
- **Grouping:** `((a ^ (b / c)) | d)`

### A1.1816 — `a . b - c - d`
- **Source:** `a . b - c - d`
- **Grouping:** `(((a . b) - c) - d)`

### A1.1817 — `a || b <= c & d`
- **Source:** `a || b <= c & d`
- **Grouping:** `(a || (b <= (c & d)))`

### A1.1818 — `a * b & c || d`
- **Source:** `a * b & c || d`
- **Grouping:** `(((a * b) & c) || d)`

### A1.1819 — `a /= b > c >= d`
- **Source:** `a /= b > c >= d`
- **Grouping:** `(((a /= b) > c) >= d)`

### A1.1820 — `a / b | c & d`
- **Source:** `a / b | c & d`
- **Grouping:** `(((a / b) | c) & d)`

### A1.1821 — `a % b << c ^ d`
- **Source:** `a % b << c ^ d`
- **Grouping:** `(((a % b) << c) ^ d)`

### A1.1822 — `a / b || c < d`
- **Source:** `a / b || c < d`
- **Grouping:** `((a / b) || (c < d))`

### A1.1823 — `a << b ^ c & d`
- **Source:** `a << b ^ c & d`
- **Grouping:** `(((a << b) ^ c) & d)`

### A1.1824 — `a <= b << c || d`
- **Source:** `a <= b << c || d`
- **Grouping:** `((a <= (b << c)) || d)`

### A1.1825 — `a & b == c + d`
- **Source:** `a & b == c + d`
- **Grouping:** `((a & b) == (c + d))`

### A1.1826 — `a & b | c && d`
- **Source:** `a & b | c && d`
- **Grouping:** `(((a & b) | c) && d)`

### A1.1827 — `a >> b < c > d`
- **Source:** `a >> b < c > d`
- **Grouping:** `(((a >> b) < c) > d)`

### A1.1828 — `a << b + c | d`
- **Source:** `a << b + c | d`
- **Grouping:** `((a << (b + c)) | d)`

### A1.1829 — `a ^ b || c >= d`
- **Source:** `a ^ b || c >= d`
- **Grouping:** `((a ^ b) || (c >= d))`

### A1.1830 — `a == b & c << d`
- **Source:** `a == b & c << d`
- **Grouping:** `(a == (b & (c << d)))`

### A1.1831 — `a /= b && c / d`
- **Source:** `a /= b && c / d`
- **Grouping:** `((a /= b) && (c / d))`

### A1.1832 — `a / b + c && d`
- **Source:** `a / b + c && d`
- **Grouping:** `(((a / b) + c) && d)`

### A1.1833 — `a <= b / c > d`
- **Source:** `a <= b / c > d`
- **Grouping:** `((a <= (b / c)) > d)`

### A1.1834 — `a / b >> c & d`
- **Source:** `a / b >> c & d`
- **Grouping:** `(((a / b) >> c) & d)`

### A1.1835 — `a && b || c > d`
- **Source:** `a && b || c > d`
- **Grouping:** `((a && b) || (c > d))`

### A1.1836 — `a > b / c == d`
- **Source:** `a > b / c == d`
- **Grouping:** `((a > (b / c)) == d)`

### A1.1837 — `a - b & c > d`
- **Source:** `a - b & c > d`
- **Grouping:** `(((a - b) & c) > d)`

### A1.1838 — `a > b - c . d`
- **Source:** `a > b - c . d`
- **Grouping:** `(a > (b - (c . d)))`

### A1.1839 — `a >= b + c && d`
- **Source:** `a >= b + c && d`
- **Grouping:** `((a >= (b + c)) && d)`

### A1.1840 — `a >> b <= c >> d`
- **Source:** `a >> b <= c >> d`
- **Grouping:** `((a >> b) <= (c >> d))`

### A1.1841 — `a / b + c . d`
- **Source:** `a / b + c . d`
- **Grouping:** `((a / b) + (c . d))`

### A1.1842 — `a . b - c % d`
- **Source:** `a . b - c % d`
- **Grouping:** `((a . b) - (c % d))`

### A1.1843 — `a * b >> c + d`
- **Source:** `a * b >> c + d`
- **Grouping:** `((a * b) >> (c + d))`

### A1.1844 — `a == b && c == d`
- **Source:** `a == b && c == d`
- **Grouping:** `((a == b) && (c == d))`

### A1.1845 — `a >= b || c && d`
- **Source:** `a >= b || c && d`
- **Grouping:** `((a >= b) || (c && d))`

### A1.1846 — `a >> b <= c /= d`
- **Source:** `a >> b <= c /= d`
- **Grouping:** `(((a >> b) <= c) /= d)`

### A1.1847 — `a < b == c && d`
- **Source:** `a < b == c && d`
- **Grouping:** `(((a < b) == c) && d)`

### A1.1848 — `a / b > c || d`
- **Source:** `a / b > c || d`
- **Grouping:** `(((a / b) > c) || d)`

### A1.1849 — `a / b < c | d`
- **Source:** `a / b < c | d`
- **Grouping:** `((a / b) < (c | d))`

### A1.1850 — `a /= b ^ c | d`
- **Source:** `a /= b ^ c | d`
- **Grouping:** `(a /= ((b ^ c) | d))`


## Appendix A.2 — Application, lambda, indexing, and composition clinic
### A2.001 — `f x`
- **Reading:** `(f x)`
- **Why it matters:** Application is juxtaposition; `x` is one argument value.

### A2.002 — `f x y`
- **Reading:** `((f x) y)`
- **Why it matters:** Syntactic association is leftward; compiler call flattening may treat a function declaration with two parameters as a two-argument call.

### A2.003 — `f (x y)`
- **Reading:** `(f (x y))`
- **Why it matters:** Parentheses make the argument the result of `x y`.

### A2.004 — `(f x) y`
- **Reading:** `((f x) y)`
- **Why it matters:** Explicitly grouped left-associated application.

### A2.005 — `f xs[i]`
- **Reading:** `(f (xs[i]))`
- **Why it matters:** The parser indexes the argument atom before attaching it as an application argument.

### A2.006 — `(f xs)[i]`
- **Reading:** `((f xs)[i])`
- **Why it matters:** Parentheses request indexing of the application result.

### A2.007 — `f()`
- **Reading:** `f()`
- **Why it matters:** Special zero-argument call syntax.

### A2.008 — `f ()`
- **Reading:** `REJECT: empty parentheses are not an ordinary expression atom`
- **Why it matters:** This is not the same token grouping as `f()`.

### A2.009 — `(\x -> x*x) 2`
- **Reading:** `(((\x -> (x * x))) 2)`
- **Why it matters:** Parenthesized anonymous function immediately applied to `2`.

### A2.010 — `(\x -> x*x)2`
- **Reading:** `(((\x -> (x * x))) 2)`
- **Why it matters:** Same tokens as above except whitespace is absent; juxtaposition still applies.

### A2.011 — `(fn (x) => x + 1) 9`
- **Reading:** `(((fn (x) => (x + 1))) 9)`
- **Why it matters:** Parenthesized lambda using fat arrow, immediately applied.

### A2.012 — `(\x y -> x+y) 20 22`
- **Reading:** `((((\x y -> (x + y))) 20) 22)`
- **Why it matters:** Two identifier parameters; no tuple destructuring.

### A2.013 — `f . g`
- **Reading:** `(f . g)`
- **Why it matters:** Composition constructs a unary function; it does not call that function.

### A2.014 — `f . g . h`
- **Reading:** `(f . (g . h))`
- **Why it matters:** Dot is right-associative.

### A2.015 — `f . g x`
- **Reading:** `(f . (g x))`
- **Why it matters:** Application binds more tightly than composition.

### A2.016 — `(f . g) x`
- **Reading:** `((f . g) x)`
- **Why it matters:** Parentheses force composition before application.

### A2.017 — `f . (g x)`
- **Reading:** `(f . (g x))`
- **Why it matters:** Explicit right operand makes the grouping visible.

### A2.018 — `-(f x)`
- **Reading:** `(-(f x))`
- **Why it matters:** Unary minus includes the application inside its operand.

### A2.019 — `-f x`
- **Reading:** `(-(f x))`
- **Why it matters:** Application is included inside a prefix operator operand.

### A2.020 — `!predicate x`
- **Reading:** `(!(predicate x))`
- **Why it matters:** Bang is prefix; call application remains inside its operand.

### A2.021 — `~bits`
- **Reading:** `(~bits)`
- **Why it matters:** Bitwise complement is a supported prefix form.

### A2.022 — `a + b * c`
- **Reading:** `(a + (b * c))`
- **Why it matters:** Multiplication binds more tightly than addition.

### A2.023 — `a - b - c`
- **Reading:** `((a - b) - c)`
- **Why it matters:** Subtraction is syntactically left-associative.

### A2.024 — `a < b < c`
- **Reading:** `((a < b) < c)`
- **Why it matters:** This is not a mathematical comparison chain.

### A2.025 — `(a < b) && (b < c)`
- **Reading:** `((a < b) && (b < c))`
- **Why it matters:** Use explicit conjunction for a range check.

### A2.026 — `base[i][j]`
- **Reading:** `(base[i][j])`
- **Why it matters:** Repeated postfix indexing chains from the left.

### A2.027 — `p + i`
- **Reading:** `(p + i)`
- **Why it matters:** Potential pointer offset only if later representation analysis knows `p` is pointer-like.

### A2.028 — `i + p`
- **Reading:** `(i + p)`
- **Why it matters:** Pointer-plus-integer is recognized in either operand order for addition.

### A2.029 — `p - i`
- **Reading:** `(p - i)`
- **Why it matters:** Pointer minus integer is a recognized pointer-offset form.

### A2.030 — `p - q`
- **Reading:** `(p - q)`
- **Why it matters:** Pointer difference if both operands are known pointer-like; result units depend on pointer metadata.

### A2.031 — `p + q`
- **Reading:** `(p + q)`
- **Why it matters:** Not a supported two-pointer addition case.

### A2.032 — `i - p`
- **Reading:** `(i - p)`
- **Why it matters:** Not a supported pointer-offset case.

### A2.033 — `p[0]`
- **Reading:** `(p[0])`
- **Why it matters:** Indexing is the read/dereference spelling for pointer-like bases.

### A2.034 — `p[ i + 1 ]`
- **Reading:** `(p[(i + 1)])`
- **Why it matters:** The index is a full expression.

### A2.035 — `f (p + i)[0]`
- **Reading:** `(f ((p + i)[0]))`
- **Why it matters:** The index suffix attaches to the argument atom before function application.

### A2.036 — `((f p)[i])`
- **Reading:** `((f p)[i])`
- **Why it matters:** Parentheses express indexing the result of a call.

### A2.037 — `let x = 5; x + 1`
- **Reading:** `let-binding then body `(x + 1)``
- **Why it matters:** The semicolon divides local binding value from its body.

### A2.038 — `{ let x = 5; x + 1 }`
- **Reading:** `block-scoped local binding with tail `(x + 1)``
- **Why it matters:** The local `let` scopes over remaining block contents.

### A2.039 — `case flag of { 1 -> yes; _ -> no }`
- **Reading:** `case-expression with two arms`
- **Why it matters:** Both arrows `->` and `=>` are accepted.

### A2.040 — `case option of { None -> 0; Some x -> x }`
- **Reading:** `constructor patterns with arities zero and one`
- **Why it matters:** Uppercase-leading patterns are constructor patterns.

### A2.041 — `(i32, u8)`
- **Reading:** `tuple type, not expression tuple`
- **Why it matters:** Tuple type syntax belongs to type context.

### A2.042 — `(left, right)`
- **Reading:** `REJECT as ordinary expression tuple`
- **Why it matters:** Tuple patterns are accepted in pattern context; tuple values have no generic comma-expression form.

### A2.043 — `type Maybe a { None; Some a; };`
- **Reading:** `type declaration with parameter `a` and two constructors`
- **Why it matters:** Declaration-level semicolons are mandatory at the outer type declaration boundary.

### A2.044 — `let extern read : i32 -> i32 = "read" : (i32) -> i32;`
- **Reading:** `external declaration with separate MAK and foreign types`
- **Why it matters:** Validate signatures against the linked C declaration.

### A2.045 — `let extern data : [u8] = global "data" : u8*;`
- **Reading:** `global object declared as typed foreign pointer`
- **Why it matters:** The C object must match the emitted global declaration.

### A2.046 — `let extern variadic : i32 = "v" : (i32, ...) -> i32;`
- **Reading:** `variadic foreign function type`
- **Why it matters:** Ellipsis works in foreign type context, not general expressions.

### A2.047 — `bring "a.mak" "b.mak";`
- **Reading:** `bring declaration with two paths`
- **Why it matters:** No alias or import-name list exists in this grammar.


## Appendix A.3 — Pointer arithmetic scenario table
### A3.001 — `typed_u8 + 0`
- **Shape:** u8* + integer.
- **Constraint:** Same byte address; an offset of one changes by one byte.

### A3.002 — `typed_u8 + 1`
- **Shape:** u8* + integer.
- **Constraint:** Advances one `u8` element.

### A3.003 — `typed_i32 + 1`
- **Shape:** i32* + integer.
- **Constraint:** Advances one `i32` element, not one byte.

### A3.004 — `typed_i32 + 4`
- **Shape:** i32* + integer.
- **Constraint:** Advances four `i32` elements.

### A3.005 — `typed_i32 - 1`
- **Shape:** i32* - integer.
- **Constraint:** Moves one typed element backward.

### A3.006 — `4 + typed_i32`
- **Shape:** integer + i32*.
- **Constraint:** Addition path recognizes integer on left and pointer on right.

### A3.007 — `typed_i32[0]`
- **Shape:** i32* indexing.
- **Constraint:** Reads element zero, subject to actual pointer validity.

### A3.008 — `typed_i32[3]`
- **Shape:** i32* indexing.
- **Constraint:** Reads element three, subject to bounds/lifetime.

### A3.009 — `(typed_i32 + 3)[0]`
- **Shape:** typed pointer addition then index.
- **Constraint:** Equivalent access position to `typed_i32[3]` when pointer arithmetic is valid.

### A3.010 — `end_i32 - start_i32`
- **Shape:** typed pointer subtraction.
- **Constraint:** C typed pointer subtraction returns element distance in applicable same-array cases.

### A3.011 — `opaque_ptr + 1`
- **Shape:** opaque ptr plus integer.
- **Constraint:** Fallback offset is byte-based because no pointee type is known.

### A3.012 — `opaque_ptr[1]`
- **Shape:** opaque ptr indexing.
- **Constraint:** Fallback indexing uses a byte-sized element.

### A3.013 — `opaque_end - opaque_start`
- **Shape:** opaque pointer subtraction.
- **Constraint:** Without recovered element metadata, generated fallback subtracts integerized addresses.

### A3.014 — `integer - pointer`
- **Shape:** reverse subtraction.
- **Constraint:** Not handled as pointer arithmetic.

### A3.015 — `pointer + pointer`
- **Shape:** two-pointer addition.
- **Constraint:** Not handled as valid pointer addition.

### A3.016 — `pointer * 2`
- **Shape:** multiplication.
- **Constraint:** Not a pointer offset operation.

### A3.017 — `pointer / 2`
- **Shape:** division.
- **Constraint:** Not a pointer offset operation.

### A3.018 — `pointer % 2`
- **Shape:** remainder.
- **Constraint:** Not a pointer offset operation.

### A3.019 — `&a`
- **Shape:** prefix position.
- **Constraint:** Invalid as address-of; `&` is binary only.

### A3.020 — `*p`
- **Shape:** prefix position.
- **Constraint:** Invalid as dereference; unary `*` is not in grammar.

### A3.021 — `p[0]`
- **Shape:** index form.
- **Constraint:** Supported pointer read spelling, assuming pointer-like representation.

### A3.022 — `p[index + 1]`
- **Shape:** index form.
- **Constraint:** Index expression may itself use ordinary operators.

### A3.023 — `p[i][j]`
- **Shape:** nested indexing.
- **Constraint:** The parser nests the reserved index intrinsic.

### A3.024 — `fn p -> p + 1`
- **Shape:** function value.
- **Constraint:** Pointer addition remains subject to metadata of the argument at call time.

### A3.025 — `bump . advance`
- **Shape:** composition.
- **Constraint:** Composition has no special pointer meaning; each operand must be a useful function.


## Appendix A.4 — Prefix/operator boundaries
### A4.001 — `-x`
- **Grouping:** `(-x)`
- **Note:** Unary minus wraps the next application-level operand.

### A4.002 — `-f x`
- **Grouping:** `(-(f x))`
- **Note:** The call `f x` is part of the unary operand.

### A4.003 — `!f x`
- **Grouping:** `(!(f x))`
- **Note:** Logical-style prefix operator also includes application.

### A4.004 — `~f x`
- **Grouping:** `(~(f x))`
- **Note:** Bitwise complement includes the application expression syntactically.

### A4.005 — `(-f) x`
- **Grouping:** `((-f) x)`
- **Note:** Parentheses change the callee grouping.

### A4.006 — `-(a + b)`
- **Grouping:** `(-(a + b))`
- **Note:** Parentheses include an infix arithmetic expression in the operand.

### A4.007 — `(-a) * b`
- **Grouping:** `(((-a)) * b)`
- **Note:** Unary operator applies before multiplication.

### A4.008 — `!a == b`
- **Grouping:** `((!a) == b)`
- **Note:** Prefix `!` binds before comparison.

### A4.009 — `~a & b`
- **Grouping:** `((~a) & b)`
- **Note:** Prefix complement binds before bitwise AND.

### A4.010 — `a * -b`
- **Grouping:** `(a * (-b))`
- **Note:** Prefix minus begins the right operand.

### A4.011 — `a + -b`
- **Grouping:** `(a + (-b))`
- **Note:** Unary minus is allowed on the additive right operand.

### A4.012 — `a - -b`
- **Grouping:** `(a - (-b))`
- **Note:** Binary minus followed by unary minus.

### A4.013 — `a / -b`
- **Grouping:** `(a / (-b))`
- **Note:** Division right operand is unary-negated.

### A4.014 — `a % ~b`
- **Grouping:** `(a % (~b))`
- **Note:** Remainder uses a complemented right operand.

### A4.015 — `!(a && b)`
- **Grouping:** `(!(a && b))`
- **Note:** Parentheses make the intended logical operand explicit.

### A4.016 — `~(a | b)`
- **Grouping:** `(~(a | b))`
- **Note:** Bitwise complement applies to the grouped OR expression.

### A4.017 — `-(f . g)`
- **Grouping:** `(-(f . g))`
- **Note:** Parentheses are needed because prefix parse stops before composition.

### A4.018 — `(-f) . g`
- **Grouping:** `((-f) . g)`
- **Note:** Composition left operand is explicitly the negated value of `f`.

### A4.019 — `f . (-g)`
- **Grouping:** `(f . (-g))`
- **Note:** Composition right operand is explicitly grouped as a prefix expression.

### A4.020 — `f . g x`
- **Grouping:** `(f . (g x))`
- **Note:** Application is available in the right operand of composition.

### A4.021 — `f . (g . h)`
- **Grouping:** `(f . (g . h))`
- **Note:** Composition can be parenthesized to expose right association.

### A4.022 — `((f . g) . h)`
- **Grouping:** `((f . g) . h)`
- **Note:** Parentheses can force the syntactic grouping; actual dot rewrite still builds callable compositions.

