// ============================================================================
// py_scientific_calculator_fixed.mak — MAK scientific calculator / REPL
// ============================================================================
// Build:
//   mak calc.mak -cc -O3 -o calc
//
// Then link the generated C with libm:
//   clang calc.c -O3 -lm -o calc
//
// Numeric model:
//   * ordinary numbers are evaluated as IEEE-754 f64
//   * /  is real floating-point division
//   * // is floor/integer-style division
//   * %  is floating remainder (for integers it behaves like integer modulo)
//   * ^  is general real exponentiation
//
// Examples:
//   22/7
//   22//7
//   2^100
//   sqrt(2)
//   sin(pi/2)
//   log10(1000)
//   5!
//   ncr(52,5)
//   2*(3+4)
//
// Angles for trig functions are in radians.
// Use deg(x) / rad(x), or the pi/tau constants.
//
// The calculator intentionally stays source-level MAK: no handwritten C
// evaluator is used for the expression engine.
// ============================================================================

// ---------- C / libm FFI ---------------------------------------------------

let external getchar : () -> i32 = "getchar" : () -> i32 {
    from = "<stdio.h>"
};

// FIX: Changed return type to ! (bottom) and foreign type to void.
// This indicates putchar is called for side effects only.
let external putchar : i32 -> ! = "putchar" : (i32) -> void {
    from = "<stdio.h>"
};

let external printf : i32 = "printf" : (ptr, ...) -> i32 {
    from = "<stdio.h>"
};

let external strlen : u64 = "strlen" : (ptr) -> u64 {
    from = "<string.h>"
};

// FIX: Changed return type to ! (bottom) and foreign type to void.
let external write : (i32, ptr, u64) -> ! = "write" : (i32, ptr, u64) -> void {
    from = "<unistd.h>"
};

// Basic floating point / transcendental functions.
let external sqrt_c : f64 = "sqrt" : (f64) -> f64 {
    from = "<math.h>"
    pure = true
};

let external cbrt_c : f64 = "cbrt" : (f64) -> f64 {
    from = "<math.h>"
    pure = true
};

let external fabs_c : f64 = "fabs" : (f64) -> f64 {
    from = "<math.h>"
    pure = true
};

let external floor_c : f64 = "floor" : (f64) -> f64 {
    from = "<math.h>"
    pure = true
};

let external ceil_c : f64 = "ceil" : (f64) -> f64 {
    from = "<math.h>"
    pure = true
};

let external trunc_c : f64 = "trunc" : (f64) -> f64 {
    from = "<math.h>"
    pure = true
};

let external round_c : f64 = "round" : (f64) -> f64 {
    from = "<math.h>"
    pure = true
};

let external fmod_c : f64 = "fmod" : (f64, f64) -> f64 {
    from = "<math.h>"
    pure = true
};

let external remainder_c : f64 = "remainder" : (f64, f64) -> f64 {
    from = "<math.h>"
    pure = true
};

let external pow_c : f64 = "pow" : (f64, f64) -> f64 {
    from = "<math.h>"
    pure = true
};

let external exp_c : f64 = "exp" : (f64) -> f64 {
    from = "<math.h>"
    pure = true
};

let external exp2_c : f64 = "exp2" : (f64) -> f64 {
    from = "<math.h>"
    pure = true
};

let external expm1_c : f64 = "expm1" : (f64) -> f64 {
    from = "<math.h>"
    pure = true
};

let external log_c : f64 = "log" : (f64) -> f64 {
    from = "<math.h>"
    pure = true
};

let external log2_c : f64 = "log2" : (f64) -> f64 {
    from = "<math.h>"
    pure = true
};

let external log10_c : f64 = "log10" : (f64) -> f64 {
    from = "<math.h>"
    pure = true
};

let external log1p_c : f64 = "log1p" : (f64) -> f64 {
    from = "<math.h>"
    pure = true
};

// Trigonometric.
let external sin_c : f64 = "sin" : (f64) -> f64 {
    from = "<math.h>"
    pure = true
};

let external cos_c : f64 = "cos" : (f64) -> f64 {
    from = "<math.h>"
    pure = true
};

let external tan_c : f64 = "tan" : (f64) -> f64 {
    from = "<math.h>"
    pure = true
};

let external asin_c : f64 = "asin" : (f64) -> f64 {
    from = "<math.h>"
    pure = true
};

let external acos_c : f64 = "acos" : (f64) -> f64 {
    from = "<math.h>"
    pure = true
};

let external atan_c : f64 = "atan" : (f64) -> f64 {
    from = "<math.h>"
    pure = true
};

let external atan2_c : f64 = "atan2" : (f64, f64) -> f64 {
    from = "<math.h>"
    pure = true
};

// Hyperbolic.
let external sinh_c : f64 = "sinh" : (f64) -> f64 {
    from = "<math.h>"
    pure = true
};

let external cosh_c : f64 = "cosh" : (f64) -> f64 {
    from = "<math.h>"
    pure = true
};

let external tanh_c : f64 = "tanh" : (f64) -> f64 {
    from = "<math.h>"
    pure = true
};

let external asinh_c : f64 = "asinh" : (f64) -> f64 {
    from = "<math.h>"
    pure = true
};

let external acosh_c : f64 = "acosh" : (f64) -> f64 {
    from = "<math.h>"
    pure = true
};

let external atanh_c : f64 = "atanh" : (f64) -> f64 {
    from = "<math.h>"
    pure = true
};

// Gamma family for factorial/generalized factorial.
let external tgamma_c : f64 = "tgamma" : (f64) -> f64 {
    from = "<math.h>"
    pure = true
};

let external lgamma_c : f64 = "lgamma" : (f64) -> f64 {
    from = "<math.h>"
    pure = true
};

let external hypot_c : f64 = "hypot" : (f64, f64) -> f64 {
    from = "<math.h>"
    pure = true
};

let external erf_c : f64 = "erf" : (f64) -> f64 {
    from = "<math.h>"
    pure = true
};

let external erfc_c : f64 = "erfc" : (f64) -> f64 {
    from = "<math.h>"
    pure = true
};

let external logb_c : f64 = "logb" : (f64) -> f64 {
    from = "<math.h>"
    pure = true
};

let external copysign_c : f64 = "copysign" : (f64, f64) -> f64 {
    from = "<math.h>"
    pure = true
};

let external fdim_c : f64 = "fdim" : (f64, f64) -> f64 {
    from = "<math.h>"
    pure = true
};

let external fmax_c : f64 = "fmax" : (f64, f64) -> f64 {
    from = "<math.h>"
    pure = true
};

let external fmin_c : f64 = "fmin" : (f64, f64) -> f64 {
    from = "<math.h>"
    pure = true
};

let external fma_c : f64 = "fma" : (f64, f64, f64) -> f64 {
    from = "<math.h>"
    pure = true
};

let external nextafter_c : f64 = "nextafter" : (f64, f64) -> f64 {
    from = "<math.h>"
    pure = true
};

// ---------- Characters / input ---------------------------------------------

type CharList {
    CNil;
    CCons i32 CharList
};

type LineOut {
    LEof;
    LLine CharList
};

let reverse_chars = fn (xs acc) {
    case xs of {
        CNil -> acc;
        CCons c rest -> reverse_chars rest (CCons c acc)
    }
};

let read_line_tail = fn (acc) {
    case getchar () of {
        c -> case c < 0 of {
            1 -> LLine (reverse_chars acc CNil);
            _ -> case c == 10 of {
                1 -> LLine (reverse_chars acc CNil);
                _ -> read_line_tail (CCons c acc)
            }
        }
    }
};

let read_line = fn () {
    case getchar () of {
        c -> case c < 0 of {
            1 -> LEof;
            _ -> case c == 10 of {
                1 -> LLine CNil;
                _ -> read_line_tail (CCons c CNil)
            }
        }
    }
};

let is_space = fn (c) {
    case c of {
        9 -> 1;
        10 -> 1;
        13 -> 1;
        32 -> 1;
        _ -> 0
    }
};

let skip_space = fn (cs) {
    case cs of {
        CNil -> CNil;
        CCons c rest -> case is_space c of {
            1 -> skip_space rest;
            _ -> cs
        }
    }
};

let emit_char = fn (c) {
    putchar c
};

let emit_text = fn (s) {
    write 1 s (strlen s)
};

let print_f64 = fn (x) {
    printf "%.17g\n" x
};

// ---------- REPL commands --------------------------------------------------

let is_exit = fn (cs) {
    case skip_space cs of {
        CNil -> 0;
        CCons 101 r1 -> case r1 of {
            CCons 120 r2 -> case r2 of {
                CCons 105 r3 -> case r3 of {
                    CCons 116 r4 -> case skip_space r4 of {
                        CNil -> 1;
                        _ -> 0
                    };
                    _ -> 0
                };
                _ -> 0
            };
            _ -> 0
        };
        _ -> 0
    }
};

let is_help = fn (cs) {
    case skip_space cs of {
        CCons 63 r -> case skip_space r of {
            CNil -> 1;
            _ -> 0
        };
        CCons 104 r1 -> case r1 of {
            CCons 101 r2 -> case r2 of {
                CCons 108 r3 -> case r3 of {
                    CCons 112 r4 -> case skip_space r4 of {
                        CNil -> 1;
                        _ -> 0
                    };
                    _ -> 0
                };
                _ -> 0
            };
            _ -> 0
        };
        _ -> 0
    }
};

// ---------- Help -----------------------------------------------------------

let print_help = fn () {
    emit_text "\n=== MAK SCIENTIFIC CALCULATOR ===\n";
    emit_text "Numbers are f64 (double precision). Output uses up to 17 significant digits.\n";
    emit_text "Use radians for trig functions.\n\n";

    emit_text "ARITHMETIC\n";
    emit_text "  a+b       addition\n";
    emit_text "  a-b       subtraction\n";
    emit_text "  a*b       multiplication\n";
    emit_text "  a/b       REAL division\n";
    emit_text "  a//b      FLOOR / INTEGER-STYLE division\n";
    emit_text "  a%b       remainder (fmod)\n";
    emit_text "  a^b       power\n";
    emit_text "  2(3+4), 2pi  implicit multiplication\n";
    emit_text "  ( ... )   grouping\n";
    emit_text "  +x  -x    unary plus/minus\n";
    emit_text "  x!        factorial (gamma(x+1), including non-integer extension)\n";
    emit_text "  x%        percentage when % is at the end of a value\n\n";

    emit_text "CONSTANTS\n";
    emit_text "  pi        3.141592653589793...\n";
    emit_text "  tau       2*pi\n";
    emit_text "  e         Euler's number\n";
    emit_text "  phi       golden ratio\n";
    emit_text "  sqrt2     square root of 2\n";
    emit_text "  ln2       natural log of 2\n";
    emit_text "  ln10      natural log of 10\n";
    emit_text "  log2e     log base 2 of e\n";
    emit_text "  log10e    log base 10 of e\n\n";

    emit_text "BASIC FUNCTIONS\n";
    emit_text "  abs(x) fabs(x)     absolute value\n";
    emit_text "  sqrt(x)            square root\n";
    emit_text "  cbrt(x)            real cube root\n";
    emit_text "  root(x,n)          nth root\n";
    emit_text "  nthroot(x,n)       alias of root\n";
    emit_text "  square(x)          x^2\n";
    emit_text "  cube(x)            x^3\n";
    emit_text "  sign(x)            -1, 0, or +1\n";
    emit_text "  frac(x)            fractional part, x-trunc(x)\n";
    emit_text "  floor(x) ceil(x) trunc(x) round(x)\n";
    emit_text "  min(a,b) max(a,b) clamp(x,lo,hi)\n";
    emit_text "  hypot(x,y)         sqrt(x^2+y^2)\n";
    emit_text "  copysign(a,b)       magnitude of a with sign of b\n";
    emit_text "  fmin fmax fdim      C floating-point min/max/positive-difference\n";
    emit_text "  fma(a,b,c)          correctly-rounded a*b+c (where supported)\n";
    emit_text "  nextafter(a,b)      next representable double from a toward b\n\n";

    emit_text "EXPONENTIALS / LOGS\n";
    emit_text "  erf(x) erfc(x)      error function / complementary error function\n";
    emit_text "  logb(x)             exponent of x in binary scale\n";
    emit_text "  exp(x)              e^x\n";
    emit_text "  exp2(x)            2^x\n";
    emit_text "  expm1(x)           e^x-1, accurate near zero\n";
    emit_text "  log(x) ln(x)       natural logarithm\n";
    emit_text "  log2(x)             base-2 logarithm\n";
    emit_text "  log10(x)            base-10 logarithm\n";
    emit_text "  log1p(x)            ln(1+x), accurate near zero\n";
    emit_text "  pow(x,y)            x^y\n\n";

    emit_text "TRIGONOMETRY — radians\n";
    emit_text "  sin cos tan\n";
    emit_text "  cot(x)=cos(x)/sin(x)\n";
    emit_text "  sec(x)=1/cos(x)\n";
    emit_text "  csc(x)=1/sin(x)\n";
    emit_text "  asin acos atan      inverse trig, result in radians\n";
    emit_text "  atan2(y,x)          quadrant-aware angle\n";
    emit_text "  acot asec acsc      inverse cot/sec/csc\n\n";

    emit_text "HYPERBOLIC\n";
    emit_text "  sinh cosh tanh\n";
    emit_text "  csch sech coth     reciprocal hyperbolic functions\n";
    emit_text "  asinh acosh atanh\n\n";

    emit_text "ANGLE CONVERSION\n";
    emit_text "  deg(x)              radians -> degrees\n";
    emit_text "  degrees(x)          alias\n";
    emit_text "  rad(x)              degrees -> radians\n";
    emit_text "  radians(x)          alias\n";
    emit_text "  Example: sin(rad(30)) = 0.5\n\n";

    emit_text "NUMBER THEORY / COMBINATORICS\n";
    emit_text "  gcd(a,b)             greatest common divisor\n";
    emit_text "  lcm(a,b)             least common multiple\n";
    emit_text "  ncr(n,r) / comb      binomial coefficient\n";
    emit_text "  npr(n,r) / perm      permutations\n";
    emit_text "  factorial(n) / fact  factorial\n";
    emit_text "  gamma(x)             Gamma(x)\n";
    emit_text "  lgamma(x)            ln(|Gamma(x)|)\n\n";

    emit_text "REMAINDER FUNCTIONS\n";
    emit_text "  fmod(a,b) mod(a,b) rem(a,b)  floating remainder\n";
    emit_text "  remainder(a,b)       IEEE-style remainder\n\n";

    emit_text "PRECEDENCE (strongest first)\n";
    emit_text "  postfix !, final %\n";
    emit_text "  power ^             right-associative\n";
    emit_text "  unary + -\n";
    emit_text "  * / // %\n";
    emit_text "  + -\n";
    emit_text "  Use parentheses whenever you want explicit grouping.\n\n";

    emit_text "EXAMPLES\n";
    emit_text "  22/7                    3.1428571428571428\n";
    emit_text "  22//7                   3\n";
    emit_text "  2^3^2                   512\n";
    emit_text "  sqrt(2)^2               2\n";
    emit_text "  sin(pi/2)               1\n";
    emit_text "  log10(1000)             3\n";
    emit_text "  ncr(52,5)               2598960\n";
    emit_text "  5!                      120\n\n";

    emit_text "IMPORTANT\n";
    emit_text "  22/7  -> 3.1428571428571428...\n";
    emit_text "  22//7 -> 3\n";
    emit_text "  / never becomes integer division.\n";
    emit_text "  // is floor division, so -7//2 = -4.\n";
    emit_text "  ^ accepts real exponents; e.g. 9^0.5 = 3.\n";
    emit_text "  Invalid domains may display nan/inf according to libm.\n";
    emit_text "  Type exit to leave the calculator.\n\n"
};

// ---------- Identifier scanner ---------------------------------------------

type NameOut {
    NBad;
    NName i64 CharList
};

let lower_ascii = fn (c) {
    case c >= 65 of {
        0 -> c;
        _ -> case c <= 90 of {
            1 -> c + 32;
            _ -> c
        }
    }
};

let is_name_start = fn (c) {
    case c == 95 of {
        1 -> 1;
        _ -> case c >= 65 of {
            0 -> case c >= 97 of {
                0 -> 0;
                _ -> case c <= 122 of {
                    1 -> 1;
                    _ -> 0
                }
            };
            _ -> case c <= 90 of {
                1 -> 1;
                _ -> 0
            }
        }
    }
};

let is_name_char = fn (c) {
    case is_name_start c of {
        1 -> 1;
        _ -> is_digit c
    }
};

// Polynomial hash reduced modulo 1,000,000,007. The calculator's built-in
// names/constants have been checked collision-free under this modulus.
let hash_name = fn (cs acc) {
    case cs of {
        CCons c rest -> case is_name_char c of {
            1 -> hash_name rest (((acc * 131) + lower_ascii c) % 1000000007);
            _ -> NName acc cs
        };
        CNil -> NName acc CNil
    }
};

let parse_name = fn (cs) {
    case skip_space cs of {
        CCons c rest -> case is_name_start c of {
            1 -> hash_name (CCons c rest) 0;
            _ -> NBad
        };
        CNil -> NBad
    }
};

// ---------- Constants ------------------------------------------------------

let const_value = fn (h) {
    case h of {
        14777 -> 3.1415926535897932384626433832795; // pi
        2003500 -> 6.283185307179586476925286766559; // tau
        101 -> 2.7182818284590452353602874713527; // e
        1935761 -> 1.6180339887498948482045868343656; // phi
        621420511 -> 1.6180339887498948482045868343656; // golden
        123496560 -> 1.4142135623730950488016887242097; // sqrt2
        1867848 -> 0.6931471805599453094172321214582; // ln2
        244688005 -> 2.3025850929940456840179914546844; // ln10
        57303579 -> 1.4426950408889634073599246810019; // log2e
        506744797 -> 0.4342944819032518276511289189166; // log10e
        _ -> 0.0
    }
};

let is_constant = fn (h) {
    case h of {
        14777 -> 1; // pi
        2003500 -> 1; // tau
        101 -> 1; // e
        1935761 -> 1; // phi
        621420511 -> 1; // golden
        123496560 -> 1; // sqrt2
        1867848 -> 1; // ln2
        244688005 -> 1; // ln10
        57303579 -> 1; // log2e
        506744797 -> 1; // log10e
        _ -> 0
    }
};

// ---------- Number parser --------------------------------------------------

type NumOut {
    NBad;
    NNum f64 CharList
};

type ExpOut {
    XBad;
    XNum i64 CharList
};

let is_digit = fn (c) {
    case c >= 48 of {
        0 -> 0;
        _ -> case c <= 57 of {
            1 -> 1;
            _ -> 0
        }
    }
};

let digit_value = fn (c) {
    case c of {
        48 -> 0.0;
        49 -> 1.0;
        50 -> 2.0;
        51 -> 3.0;
        52 -> 4.0;
        53 -> 5.0;
        54 -> 6.0;
        55 -> 7.0;
        56 -> 8.0;
        57 -> 9.0;
        _ -> 0.0
    }
};

// Positive integer power of ten, with negative exponent support.
let pow10 = fn (e) {
    case e of {
        0 -> 1.0;
        _ -> case e > 0 of {
            1 -> 10.0 * pow10 (e - 1);
            _ -> 0.1 * pow10 (e + 1)
        }
    }
};

let read_exp_digits = fn (cs acc found) {
    case cs of {
        CCons c rest -> case is_digit c of {
            1 -> read_exp_digits rest ((acc * 10) + (c - 48)) 1;
            _ -> case found of {
                1 -> XNum acc cs;
                _ -> XBad
            }
        };
        CNil -> case found of {
            1 -> XNum acc CNil;
            _ -> XBad
        }
    }
};

let parse_exponent = fn (cs sign) {
    case skip_space cs of {
        CCons c rest -> case c of {
            43 -> read_exp_digits rest 0 0;
            45 -> case read_exp_digits rest 0 0 of {
                XBad -> XBad;
                XNum v r -> XNum (0 - v) r
            };
            _ -> case is_digit c of {
                1 -> read_exp_digits (CCons c rest) 0 0;
                _ -> XBad
            }
        };
        CNil -> XBad
    }
};

let read_integer_part = fn (cs acc) {
    case cs of {
        CCons c rest -> case is_digit c of {
            1 -> read_integer_part rest (acc * 10.0 + digit_value c);
            _ -> NNum acc cs
        };
        CNil -> NNum acc CNil
    }
};

let read_fraction2 = fn (cs value place) {
    case cs of {
        CCons c rest -> case is_digit c of {
            1 -> read_fraction2 rest (value + digit_value c * place) (place * 0.1);
            _ -> case c of {
                101 -> case parse_exponent rest 1 of {
                    XBad -> NBad;
                    XNum e r -> NNum (value * pow10 e) r
                };
                69 -> case parse_exponent rest 1 of {
                    XBad -> NBad;
                    XNum e r -> NNum (value * pow10 e) r
                };
                _ -> NNum value cs
            }
        };
        CNil -> NNum value CNil
    }
};

let parse_number = fn (cs) {
    case skip_space cs of {
        CNil -> NBad;
        CCons c rest -> case is_digit c of {
            1 -> case read_integer_part (CCons c rest) 0.0 of {
                NBad -> NBad;
                NNum whole after_int -> case after_int of {
                    CCons 46 after_dot -> read_fraction2 after_dot whole 0.1;
                    CCons ec after_e -> case ec of {
                        101 -> case parse_exponent after_e 1 of {
                            XBad -> NBad;
                            XNum e r -> NNum (whole * pow10 e) r
                        };
                        69 -> case parse_exponent after_e 1 of {
                            XBad -> NBad;
                            XNum e r -> NNum (whole * pow10 e) r
                        };
                        _ -> NNum whole after_int
                    };
                    _ -> NNum whole after_int
                }
            };
            _ -> case c of {
                46 -> case rest of {
                    CCons d _ -> case is_digit d of {
                        1 -> read_fraction2 rest 0.0 0.1;
                        _ -> NBad
                    };
                    _ -> NBad
                };
                _ -> NBad
            }
        }
    }
};

// ---------- Function dispatch -----------------------------------------------

let arity1 = fn (h) {
    case h of {
        1677570 -> 1; // absolute value
        230982852 -> 1; // alias for abs
        260484708 -> 1; // square root
        224257837 -> 1; // cube root
        1749093 -> 1; // e^x
        229131233 -> 1; // 2^x
        16199091 -> 1; // e^x-1
        1868032 -> 1; // natural logarithm
        14258 -> 1; // natural logarithm
        244712242 -> 1; // base-2 logarithm
        57303395 -> 1; // base-10 logarithm
        57303459 -> 1; // ln(1+x)
        1987380 -> 1; // sine
        1713595 -> 1; // cosine
        2003493 -> 1; // tangent
        220052207 -> 1; // inverse sine
        219778422 -> 1; // inverse cosine
        220068320 -> 1; // inverse tangent
        260346884 -> 1; // hyperbolic sine
        224481049 -> 1; // hyperbolic cosine
        262457687 -> 1; // hyperbolic tangent
        224547597 -> 1; // hyperbolic cosecant
        260276799 -> 1; // hyperbolic secant
        224481180 -> 1; // hyperbolic cotangent
        826839025 -> 1; // inverse sinh
        790973190 -> 1; // inverse cosh
        828949828 -> 1; // inverse tanh
        1713596 -> 1; // cotangent
        1986845 -> 1; // secant
        1714103 -> 1; // cosecant
        219778423 -> 1; // inverse cotangent
        220051672 -> 1; // inverse secant
        219778930 -> 1; // inverse cosecant
        283705086 -> 1; // floor
        224308133 -> 1; // ceiling
        420295318 -> 1; // truncate toward zero
        824551211 -> 1; // nearest integer-valued double
        354247632 -> 1; // factorial / Gamma(x+1)
        230982984 -> 1; // alias for factorial
        553441405 -> 1; // Gamma function
        138294544 -> 1; // log absolute Gamma
        1748297 -> 1; // error function
        229027006 -> 1; // complementary error function
        244712290 -> 1; // binary exponent
        260345973 -> 1; // sign function
        231274442 -> 1; // fractional part
        1729434 -> 1; // radians to degrees
        1969161 -> 1; // degrees to radians
        992605559 -> 1; // degrees to radians
        430838469 -> 1; // radians to degrees
        184475947 -> 1; // x^2
        224581785 -> 1; // x^3
        _ -> 0
    }
};

let arity2 = fn (h) {
    case h of {
        828949774 -> 1; // quadrant-aware atan
        231190472 -> 1; // floating remainder
        1885190 -> 1; // mod alias
        1969694 -> 1; // rem alias
        510764621 -> 1; // IEEE-style remainder
        1936692 -> 1; // x^y
        258201902 -> 1; // nth root
        548941060 -> 1; // alias for root
        901947274 -> 1; // sqrt(x^2+y^2)
        1780652 -> 1; // greatest common divisor
        1866466 -> 1; // least common multiple
        1900793 -> 1; // binomial coefficient
        224480257 -> 1; // alias for ncr
        1902496 -> 1; // permutations
        253534496 -> 1; // alias for npr
        1884414 -> 1; // minimum
        1883376 -> 1; // maximum
        262106026 -> 1; // magnitude of a with sign of b
        231035246 -> 1; // positive difference
        231188658 -> 1; // floating maximum
        231189696 -> 1; // floating minimum
        743972013 -> 1; // next representable double
        _ -> 0
    }
};

let arity3 = fn (h) {
    case h of {
        399964812 -> 1; // clamp x into [lo,hi]
        1764798 -> 1; // fused multiply-add
        _ -> 0
    }
};

// ---------- Numeric functions ----------------------------------------------

let unary_call = fn (h x) {
    case h of {
        1677570 -> fabs_c x; // abs
        230982852 -> fabs_c x; // fabs
        260484708 -> sqrt_c x; // sqrt
        224257837 -> cbrt_c x; // cbrt
        1749093 -> exp_c x; // exp
        229131233 -> exp2_c x; // exp2
        16199091 -> expm1_c x; // expm1
        1868032 -> log_c x; // log
        14258 -> log_c x; // ln
        244712242 -> log2_c x; // log2
        57303395 -> log10_c x; // log10
        57303459 -> log1p_c x; // log1p
        1987380 -> sin_c x; // sin
        1713595 -> cos_c x; // cos
        2003493 -> tan_c x; // tan
        220052207 -> asin_c x; // asin
        219778422 -> acos_c x; // acos
        220068320 -> atan_c x; // atan
        260346884 -> sinh_c x; // sinh
        224481049 -> cosh_c x; // cosh
        262457687 -> tanh_c x; // tanh
        224547597 -> 1.0 / sinh_c x; // csch
        260276799 -> 1.0 / cosh_c x; // sech
        224481180 -> cosh_c x / sinh_c x; // coth
        826839025 -> asinh_c x; // asinh
        790973190 -> acosh_c x; // acosh
        828949828 -> atanh_c x; // atanh
        1713596 -> cos_c x / sin_c x; // cot
        1986845 -> 1.0 / cos_c x; // sec
        1714103 -> 1.0 / sin_c x; // csc
        219778423 -> 1.57079632679489661923 - atan_c x; // acot
        220051672 -> acos_c (1.0 / x); // asec
        219778930 -> asin_c (1.0 / x); // acsc
        283705086 -> floor_c x; // floor
        224308133 -> ceil_c x; // ceil
        420295318 -> trunc_c x; // trunc
        824551211 -> round_c x; // round
        354247632 -> tgamma_c (x + 1.0); // factorial
        230982984 -> tgamma_c (x + 1.0); // fact
        553441405 -> tgamma_c x; // gamma
        138294544 -> lgamma_c x; // lgamma
        1748297 -> erf_c x; // erf
        229027006 -> erfc_c x; // erfc
        244712290 -> logb_c x; // logb
        260345973 -> case x > 0.0 of { 1 -> 1.0; _ -> case x < 0.0 of { 1 -> -1.0; _ -> 0.0 } }; // sign
        231274442 -> x - trunc_c x; // frac
        1729434 -> x * 57.2957795130823208768; // deg
        1969161 -> x * 0.0174532925199432957692; // rad
        992605559 -> x * 0.0174532925199432957692; // radians
        430838469 -> x * 57.2957795130823208768; // degrees
        184475947 -> x * x; // square
        224581785 -> x * x * x; // cube
        _ -> 0.0
    }
};

let binary_call = fn (h a b) {
    case h of {
        828949774 -> atan2_c a b; // atan2
        231190472 -> fmod_c a b; // fmod
        1885190 -> fmod_c a b; // mod
        1969694 -> fmod_c a b; // rem
        510764621 -> remainder_c a b; // remainder
        1936692 -> pow_c a b; // pow
        258201902 -> pow_c a (1.0 / b); // root
        548941060 -> pow_c a (1.0 / b); // nthroot
        901947274 -> hypot_c a b; // hypot
        1780652 -> { let aa = fabs_c a; let bb = fabs_c b; case bb == 0.0 of { 1 -> aa; _ -> binary_call 1780652 bb (fmod_c aa bb) } }; // gcd
        1866466 -> { let g = binary_call 1780652 a b; case g == 0.0 of { 1 -> 0.0; _ -> fabs_c (a * b) / g } }; // lcm
        1900793 -> { let nn = round_c a; let rr = round_c b; case rr < 0.0 of { 1 -> 0.0; _ -> case rr > nn of { 1 -> 0.0; _ -> round_c (exp_c (lgamma_c (nn + 1.0) - lgamma_c (rr + 1.0) - lgamma_c (nn - rr + 1.0))) } } }; // ncr
        224480257 -> binary_call 1900793 a b; // comb
        1902496 -> { let nn = round_c a; let rr = round_c b; case rr < 0.0 of { 1 -> 0.0; _ -> case rr > nn of { 1 -> 0.0; _ -> round_c (exp_c (lgamma_c (nn + 1.0) - lgamma_c (nn - rr + 1.0))) } } }; // npr
        253534496 -> binary_call 1902496 a b; // perm
        1884414 -> case a < b of { 1 -> a; _ -> b }; // min
        1883376 -> case a > b of { 1 -> a; _ -> b }; // max
        262106026 -> copysign_c a b; // copysign
        231035246 -> fdim_c a b; // fdim
        231188658 -> fmax_c a b; // fmax
        231189696 -> fmin_c a b; // fmin
        743972013 -> nextafter_c a b; // nextafter
        _ -> 0.0
    }
};

let ternary_call = fn (h x a b) {
    case h of {
        399964812 -> case x < a of {
            1 -> a;
            _ -> case x > b of {
                1 -> b;
                _ -> x
            }
        };
        1764798 -> fma_c x a b;
        _ -> x
    }
};

// ---------- Expression parser ----------------------------------------------
//
// expr   := term (('+'|'-') term)*
// term   := power (('*'|'/'|'//'|'%') power)*
// power  := unary ('^' unary)?
// unary  := ('+'|'-') unary | postfix
// postfix:= atom ('!' | '%' postfix-at-end)*
// atom   := number | identifier | identifier '(' args ')' | '(' expr ')'
//
// `//` is checked before `/`.
// `%` is postfix only when it is followed by an expression delimiter/end;
// otherwise it is binary remainder.
// Function application with parentheses keeps scientific-calculator syntax simple:
//   sin(pi/2), pow(2,10), ncr(52,5)

type ExprOut {
    EOK f64 CharList;
    EBad
};

let parse_call1 = fn (h cs) {
    case parse_expr cs of {
        EBad -> EBad;
        EOK a rest -> case skip_space rest of {
            CCons 41 rest2 -> case arity1 h of {
                1 -> EOK (unary_call h a) rest2;
                _ -> EBad
            };
            _ -> EBad
        }
    }
};

let parse_call2 = fn (h cs) {
    case parse_expr cs of {
        EBad -> EBad;
        EOK a r1 -> case skip_space r1 of {
            CCons 44 r2 -> case parse_expr r2 of {
                EBad -> EBad;
                EOK b r3 -> case skip_space r3 of {
                    CCons 41 r4 -> case arity2 h of {
                        1 -> EOK (binary_call h a b) r4;
                        _ -> EBad
                    };
                    _ -> EBad
                }
            };
            _ -> EBad
        }
    }
};

let parse_call3 = fn (h cs) {
    case parse_expr cs of {
        EBad -> EBad;
        EOK a r1 -> case skip_space r1 of {
            CCons 44 r2 -> case parse_expr r2 of {
                EBad -> EBad;
                EOK b r3 -> case skip_space r3 of {
                    CCons 44 r4 -> case parse_expr r4 of {
                        EBad -> EBad;
                        EOK c r5 -> case skip_space r5 of {
                            CCons 41 r6 -> case arity3 h of {
                                1 -> EOK (ternary_call h a b c) r6;
                                _ -> EBad
                            };
                            _ -> EBad
                        }
                    };
                    _ -> EBad
                }
            };
            _ -> EBad
        }
    }
};

let parse_named = fn (h cs) {
    case arity3 h of {
        1 -> parse_call3 h cs;
        _ -> case arity2 h of {
            1 -> parse_call2 h cs;
            _ -> case arity1 h of {
                1 -> parse_call1 h cs;
                _ -> EBad
            }
        }
    }
};

let parse_atom = fn (cs) {
    case skip_space cs of {
        CNil -> EBad;
        CCons c rest -> case c of {
            40 -> case parse_expr rest of {
                EBad -> EBad;
                EOK v r -> case skip_space r of {
                    CCons 41 r2 -> EOK v r2;
                    _ -> EBad
                }
            };
            _ -> case is_name_start c of {
                1 -> case parse_name (CCons c rest) of {
                    NBad -> EBad;
                    NName h r -> case skip_space r of {
                        CCons 40 r2 -> parse_named h r2;
                        _ -> case is_constant h of {
                            1 -> EOK (const_value h) r;
                            _ -> EBad
                        }
                    }
                };
                _ -> case parse_number cs of {
                    NBad -> EBad;
                    NNum v r -> EOK v r
                }
            }
        }
    }
};

let parse_postfix_loop = fn (value cs) {
    case skip_space cs of {
        CCons 33 rest -> parse_postfix_loop (tgamma_c (value + 1.0)) rest;
        CCons 37 rest -> case skip_space rest of {
            CNil -> EOK (value / 100.0) CNil;
            CCons c _ -> case c of {
                41 -> EOK (value / 100.0) rest;
                44 -> EOK (value / 100.0) rest;
                43 -> EOK (value / 100.0) rest;
                45 -> EOK (value / 100.0) rest;
                42 -> EOK (value / 100.0) rest;
                47 -> EOK (value / 100.0) rest;
                94 -> EOK (value / 100.0) rest;
                _ -> EOK value cs
            }
        };
        _ -> EOK value cs
    }
};

let parse_postfix = fn (cs) {
    case parse_atom cs of {
        EBad -> EBad;
        EOK value rest -> parse_postfix_loop value rest
    }
};

let parse_power = fn (cs) {
    case parse_postfix cs of {
        EBad -> EBad;
        EOK left rest -> case skip_space rest of {
            CCons 94 r2 -> case parse_unary r2 of {
                EBad -> EBad;
                EOK exponent r3 -> EOK (pow_c left exponent) r3
            };
            _ -> EOK left rest
        }
    }
};

let parse_unary = fn (cs) {
    case skip_space cs of {
        CNil -> EBad;
        CCons c rest -> case c of {
            43 -> parse_unary rest;
            45 -> case parse_unary rest of {
                EBad -> EBad;
                EOK v r -> EOK (0.0 - v) r
            };
            _ -> parse_power cs
        }
    }
};

let parse_term_loop = fn (left cs) {
    case skip_space cs of {
        CNil -> EOK left CNil;
        CCons c rest -> case c of {
            42 -> case parse_power rest of {
                EBad -> EBad;
                EOK rhs r2 -> parse_term_loop (left * rhs) r2
            };
            47 -> case rest of {
                CCons 47 rest2 -> case parse_power rest2 of {
                    EBad -> EBad;
                    EOK rhs r2 -> case rhs == 0.0 of {
                        1 -> EBad;
                        _ -> parse_term_loop (floor_c (left / rhs)) r2
                    }
                };
                _ -> case parse_power rest of {
                    EBad -> EBad;
                    EOK rhs r2 -> case rhs == 0.0 of {
                        1 -> EBad;
                        _ -> parse_term_loop (left / rhs) r2
                    }
                }
            };
            37 -> case parse_power rest of {
                EBad -> EBad;
                EOK rhs r2 -> case rhs == 0.0 of {
                    1 -> EBad;
                    _ -> parse_term_loop (fmod_c left rhs) r2
                }
            };
            _ -> case is_name_start c of {
                1 -> case parse_power cs of {
                    EBad -> EOK left cs;
                    EOK rhs r2 -> parse_term_loop (left * rhs) r2
                };
                _ -> case is_digit c of {
                    1 -> case parse_power cs of {
                        EBad -> EOK left cs;
                        EOK rhs r2 -> parse_term_loop (left * rhs) r2
                    };
                    _ -> case c of {
                        46 -> case parse_power cs of {
                            EBad -> EOK left cs;
                            EOK rhs r2 -> parse_term_loop (left * rhs) r2
                        };
                        40 -> case parse_power cs of {
                            EBad -> EOK left cs;
                            EOK rhs r2 -> parse_term_loop (left * rhs) r2
                        };
                        _ -> EOK left cs
                    }
                }
            }
        }
    }
};

let parse_term = fn (cs) {
    case parse_power cs of {
        EBad -> EBad;
        EOK left rest -> parse_term_loop left rest
    }
};

let parse_expr_loop = fn (left cs) {
    case skip_space cs of {
        CNil -> EOK left CNil;
        CCons c rest -> case c of {
            43 -> case parse_term rest of {
                EBad -> EBad;
                EOK rhs r2 -> parse_expr_loop (left + rhs) r2
            };
            45 -> case parse_term rest of {
                EBad -> EBad;
                EOK rhs r2 -> parse_expr_loop (left - rhs) r2
            };
            _ -> EOK left cs
        }
    }
};

let parse_expr = fn (cs) {
    case parse_term cs of {
        EBad -> EBad;
        EOK left rest -> parse_expr_loop left rest
    }
};

let evaluate_line = fn (cs) {
    case parse_expr cs of {
        EBad -> emit_text "Error: invalid expression\n";
        EOK value rest -> case skip_space rest of {
            CNil -> print_f64 value;
            _ -> emit_text "Error: unexpected characters\n"
        }
    }
};

// ---------- REPL -----------------------------------------------------------

let repl = fn () {
    emit_text ">>> ";
    case read_line () of {
        LEof -> emit_char 10;
        LLine cs -> case is_exit cs of {
            1 -> emit_char 10;
            _ -> case is_help cs of {
                1 -> {
                    print_help ();
                    repl ()
                };
                _ -> {
                    evaluate_line cs;
                    repl ()
                }
            }
        }
    }
};

let calculator_main = fn () {
    emit_text "MAK Scientific Calculator\n";
    emit_text "Real /, floor //, remainder %, power ^, factorial !\n";
    emit_text "Type help for the complete scientific function list.\n";
    emit_text "Example: 22/7 = 3.1428571428571428...\n\n";
    repl ()
};

calculator_main ()