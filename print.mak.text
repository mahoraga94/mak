// =====================================================================
// pureprintf.mak
// =====================================================================
// A printf in pure MAK. Uses libc putchar for output; all formatting,
// width/padding, sign handling, base conversion, and float rendering
// are implemented in MAK itself.
//
// Supported directives:
//   %d  %i          signed integer
//   %u              unsigned integer
//   %x  %X          hex, lower / upper
//   %o              octal
//   %b              binary (extension)
//   %c              character
//   %s              NUL-terminated string
//   %p              pointer, formatted as 0x...
//   %f              fixed-point f64 (default precision 6)
//   %%              literal %
//
// Flags:   - (left align), 0 (zero pad), + (force sign), ' ' (space sign)
// Width:   any decimal, e.g. %5d, %-10s, %08x
// Prec:    .N — max length for %s, fractional digits for %f
//
// Note on type inference:
//   The spec-parsing helpers take `fmt` and only use it via `fmt[i]`.
//   MAK's representation inference cannot see through `fmt[i]` alone, so
//   without help it defaults `fmt` to i64 and `direct_index_expr` fails,
//   emitting `mak_ir_fail_u64("invalid index expression")` at every index.
//   Calling `strlen fmt` at the top of each of these helpers forces the
//   analyzer to record `Rep::Ptr` for `fmt`, which makes `fmt[i]` lower
//   to a real `uint8_t` dereference in the emitted C.
// =====================================================================

let extern putchar : i32 -> i32 = "putchar" : (i32) -> i32 {
    from = "<stdio.h>";
    pure = false
};

let extern strlen : u64 = "strlen" : (ptr) -> u64 {
    from = "<string.h>";
    pure = true
};

let extern trunc_c : f64 = "trunc" : (f64) -> f64 {
    from = "<math.h>";
    pure = true
};

let extern round_c : f64 = "round" : (f64) -> f64 {
    from = "<math.h>";
    pure = true
};

let extern fabs_c : f64 = "fabs" : (f64) -> f64 {
    from = "<math.h>";
    pure = true
};

// =====================================================================
// Types
// =====================================================================
type Buf {
    BNil;
    BCons i32 Buf;
};

type FmtArg {
    AInt i64;
    AUint u64;
    AHexLow u64;
    AHexUp u64;
    AOct u64;
    ABin u64;
    ACh i32;
    AStr str;
    APtr ptr;
    AF64 f64;
};

type ArgList {
    ArgsNil;
    ArgsCons FmtArg ArgList;
};

// flags bits: 1 = left-align, 2 = zero-pad, 4 = force-sign, 8 = space-sign
type Spec {
    MkSpec i64 i64 i64 i64;   // flags, width, precision (-1 = unspecified), conv-char
};

type SpecPair {
    MkSpecPair Spec i64;
};

type NumRes {
    MkNumRes i64 i64;
};

// =====================================================================
// Buffer primitives
// =====================================================================
let breverse_go src acc =
    case src of {
        BNil -> acc;
        BCons c rest -> breverse_go rest (BCons c acc)
    };

let breverse b = breverse_go b BNil;

let bcount_go src n =
    case src of {
        BNil -> n;
        BCons _ rest -> bcount_go rest (n + 1)
    };

let bcount b = bcount_go b 0;

let bput b =
    case b of {
        BNil -> 0;
        BCons c rest -> {
            putchar c;
            bput rest
        }
    };

let bconcat a b =
    case a of {
        BNil -> b;
        BCons c rest -> BCons c (bconcat rest b)
    };

let brepeat c k =
    case k <= 0 of {
        1 -> BNil;
        _ -> BCons c (brepeat c (k - 1))
    };

// =====================================================================
// Integer digit conversion
// =====================================================================
let rev_digits_go v base upper acc =
    case v == 0 of {
        1 -> acc;
        _ -> {
            let d = v % base;
            let c = case d < 10 of {
                1 -> 48 + d;
                _ -> case upper of {
                    1 -> 55 + d;   // 'A' + (d - 10)
                    _ -> 87 + d    // 'a' + (d - 10)
                }
            };
            rev_digits_go (v / base) base upper (BCons c acc)
        }
    };

let rev_digits n base upper =
    case n == 0 of {
        1 -> BCons 48 BNil;
        _ -> breverse (rev_digits_go n base upper BNil)
    };

let iabs v = case v < 0 of {
    1 -> 0 - v;
    _ -> v
};

let has_flag flags bit = case flags & bit of {
    0 -> 0;
    _ -> 1
};

let sign_char flags n = case n < 0 of {
    1 -> 45;
    _ -> case has_flag flags 4 of {
        1 -> 43;
        _ -> case has_flag flags 8 of {
            1 -> 32;
            _ -> 0
        }
    }
};

let sign_rev sc = case sc == 0 of {
    1 -> BNil;
    _ -> BCons sc BNil
};

// =====================================================================
// Signed / unsigned integer formatting
// =====================================================================
let int_rev flags width n =
    let digits_rev = rev_digits (iabs n) 10 0;
    let dl = bcount digits_rev;
    let sc = sign_char flags n;
    let sl = case sc == 0 of { 1 -> 0; _ -> 1 };
    let content_len = dl + sl;
    let padding = case width <= content_len of {
        1 -> 0;
        _ -> width - content_len
    };
    let sr = sign_rev sc;
    case has_flag flags 1 of {
        1 -> bconcat (brepeat 32 padding) (bconcat digits_rev sr);
        _ -> case has_flag flags 2 of {
            1 -> bconcat digits_rev (bconcat (brepeat 48 padding) sr);
            _ -> bconcat digits_rev (bconcat sr (brepeat 32 padding))
        }
    };

let uint_rev flags width base upper n =
    let digits_rev = rev_digits n base upper;
    let dl = bcount digits_rev;
    let padding = case width <= dl of {
        1 -> 0;
        _ -> width - dl
    };
    case has_flag flags 1 of {
        1 -> bconcat (brepeat 32 padding) digits_rev;
        _ -> case has_flag flags 2 of {
            1 -> bconcat digits_rev (brepeat 48 padding);
            _ -> bconcat digits_rev (brepeat 32 padding)
        }
    };

// =====================================================================
// String handling
// =====================================================================
let str_rev_go s i acc =
    let _ = strlen s;
    case i < 0 of {
        1 -> acc;
        _ -> str_rev_go s (i - 1) (BCons (s[i]) acc)
    };

let str_rev s prec =
    let n = strlen s;
    let limit = case prec < 0 of {
        1 -> n;
        _ -> case prec < n of {
            1 -> prec;
            _ -> n
        }
    };
    breverse (str_rev_go s (limit - 1) BNil);

let str_fmt flags width prec s =
    let body = str_rev s prec;
    let bl = bcount body;
    let padding = case width <= bl of {
        1 -> 0;
        _ -> width - bl
    };
    case has_flag flags 1 of {
        1 -> bconcat (brepeat 32 padding) body;
        _ -> bconcat body (brepeat 32 padding)
    };

// =====================================================================
// Float formatting
// =====================================================================
let force_i64 x = x + 0;

let ten_to_p_go k acc =
    case k <= 0 of {
        1 -> acc;
        _ -> ten_to_p_go (k - 1) (acc * 10.0)
    };

let ten_to_p p = ten_to_p_go p 1.0;

let f64_abs_rev f p =
    case p <= 0 of {
        1 -> rev_digits (force_i64 (round_c f)) 10 0;
        _ -> {
            let scale = ten_to_p p;
            let f_trunc = trunc_c f;
            let f_frac = (f - f_trunc) * scale;
            let ip = force_i64 f_trunc;
            let fp = force_i64 (round_c f_frac);
            let int_digits_rev = rev_digits ip 10 0;
            let frac_digits_rev = rev_digits fp 10 0;
            let frac_len = bcount frac_digits_rev;
            let extra_zeros = case frac_len >= p of {
                1 -> 0;
                _ -> p - frac_len
            };
            let frac_full = bconcat frac_digits_rev (brepeat 48 extra_zeros);
            bconcat frac_full (bconcat (BCons 46 BNil) int_digits_rev)
        }
    };

let f64_rev flags width prec f =
    let p = case prec < 0 of { 1 -> 6; _ -> prec };
    let body = f64_abs_rev (fabs_c f) p;
    let bl = bcount body;
    let sc = sign_char flags (case f < 0.0 of { 1 -> -1; _ -> 0 });
    let sl = case sc == 0 of { 1 -> 0; _ -> 1 };
    let total_len = bl + sl;
    let padding = case width <= total_len of {
        1 -> 0;
        _ -> width - total_len
    };
    let sr = sign_rev sc;
    case has_flag flags 1 of {
        1 -> bconcat (brepeat 32 padding) (bconcat body sr);
        _ -> case has_flag flags 2 of {
            1 -> bconcat body (bconcat (brepeat 48 padding) sr);
            _ -> bconcat body (bconcat sr (brepeat 32 padding))
        }
    };

// =====================================================================
// Spec parser (acyclic to satisfy type inference)
// =====================================================================
let is_digit c = case c >= 48 of {
    0 -> 0;
    _ -> case c <= 57 of {
        1 -> 1;
        _ -> 0
    }
};

let read_num_go fmt j acc =
    let _ = strlen fmt;
    case is_digit (fmt[j]) of {
        1 -> read_num_go fmt (j + 1) (acc * 10 + (fmt[j]) - 48);
        _ -> MkNumRes acc j
    };

let read_num fmt i =
    let _ = strlen fmt;
    read_num_go fmt i 0;

let spec_prec_state fmt j acc w =
    let _ = strlen fmt;
    case fmt[j] == 46 of {
        1 -> {
            let r = read_num fmt (j + 1);
            case r of {
                MkNumRes v k -> MkSpecPair (MkSpec acc w v (fmt[k])) (k + 1)
            }
        };
        _ -> MkSpecPair (MkSpec acc w (0 - 1) (fmt[j])) (j + 1)
    };

let spec_width_state fmt j acc =
    let _ = strlen fmt;
    case is_digit (fmt[j]) of {
        1 -> {
            let r = read_num fmt j;
            case r of {
                MkNumRes v k -> spec_prec_state fmt k acc v
            }
        };
        _ -> spec_prec_state fmt j acc 0
    };

let parse_flags fmt j acc =
    let _ = strlen fmt;
    case fmt[j] of {
        45 -> parse_flags fmt (j + 1) (acc | 1);
        48 -> parse_flags fmt (j + 1) (acc | 2);
        43 -> parse_flags fmt (j + 1) (acc | 4);
        32 -> parse_flags fmt (j + 1) (acc | 8);
        _ -> MkNumRes acc j
    };

let parse_spec fmt i =
    let _ = strlen fmt;
    let fr = parse_flags fmt i 0;
    case fr of {
        MkNumRes flags j -> spec_width_state fmt j flags
    };

// =====================================================================
// Applying a spec to an argument
// =====================================================================
let apply_dispatch conv flags width prec arg acc =
    case conv of {
        100 -> case arg of {
            AInt n -> bconcat (int_rev flags width n) acc;
            _ -> acc
        };
        105 -> case arg of {
            AInt n -> bconcat (int_rev flags width n) acc;
            _ -> acc
        };
        117 -> case arg of {
            AUint n -> bconcat (uint_rev flags width 10 0 n) acc;
            _ -> acc
        };
        120 -> case arg of {
            AHexLow n -> bconcat (uint_rev flags width 16 0 n) acc;
            AUint n -> bconcat (uint_rev flags width 16 0 n) acc;
            _ -> acc
        };
        88 -> case arg of {
            AHexUp n -> bconcat (uint_rev flags width 16 1 n) acc;
            AUint n -> bconcat (uint_rev flags width 16 1 n) acc;
            _ -> acc
        };
        111 -> case arg of {
            AOct n -> bconcat (uint_rev flags width 8 0 n) acc;
            AUint n -> bconcat (uint_rev flags width 8 0 n) acc;
            _ -> acc
        };
        98 -> case arg of {
            ABin n -> bconcat (uint_rev flags width 2 0 n) acc;
            AUint n -> bconcat (uint_rev flags width 2 0 n) acc;
            _ -> acc
        };
        99 -> case arg of {
            ACh c -> BCons c acc;
            _ -> acc
        };
        115 -> case arg of {
            AStr s -> bconcat (str_fmt flags width prec s) acc;
            _ -> acc
        };
        112 -> case arg of {
            APtr p -> bconcat (bconcat (rev_digits p 16 0) (BCons 120 (BCons 48 BNil))) acc;
            _ -> acc
        };
        102 -> case arg of {
            AF64 f -> bconcat (f64_rev flags width prec f) acc;
            _ -> acc
        };
        _ -> acc
    };

let apply_spec spec arg acc =
    case spec of {
        MkSpec flags width prec conv -> apply_dispatch conv flags width prec arg acc
    };

// =====================================================================
// Main format loop
// =====================================================================
let format_loop fmt i args acc =
    let _ = strlen fmt;
    case fmt[i] of {
        0 -> acc;
        37 -> {
            let sp = parse_spec fmt (i + 1);
            case sp of {
                MkSpecPair spec next_i -> case spec of {
                    MkSpec _ _ _ conv -> case conv == 37 of {
                        1 -> format_loop fmt next_i args (BCons 37 acc);
                        _ -> case args of {
                            ArgsNil -> format_loop fmt next_i args (BCons 63 acc);
                            ArgsCons arg rest ->
                                format_loop fmt next_i rest (apply_spec spec arg acc)
                        }
                    }
                }
            }
        };
        _ -> format_loop fmt (i + 1) args (BCons (fmt[i]) acc)
    };

// =====================================================================
// Public API
// =====================================================================
let pure_printf fmt args =
    let _ = strlen fmt;
    bput (breverse (format_loop fmt 0 args BNil));

let pure_println fmt args =
    let _ = strlen fmt;
    bput (breverse (BCons 10 (format_loop fmt 0 args BNil)));

// =====================================================================
// Demo
// =====================================================================
let main =
    fn () -> {
        pure_println "==================================================" ArgsNil;
        pure_println "  pureprintf.mak — a printf in pure MAK" ArgsNil;
        pure_println "==================================================" ArgsNil;
        pure_println "" ArgsNil;
        pure_println "Basic:" ArgsNil;
        pure_println "  name=%s age=%d grade=%c"
            (ArgsCons (AStr "Alice")
            (ArgsCons (AInt 30)
            (ArgsCons (ACh 65) ArgsNil)));
        pure_println "" ArgsNil;
        pure_println "Widths and flags:" ArgsNil;
        pure_println "  right: [%10d]" (ArgsCons (AInt 42) ArgsNil);
        pure_println "  left:  [%-10d]" (ArgsCons (AInt 42) ArgsNil);
        pure_println "  zero:  [%010d]" (ArgsCons (AInt 42) ArgsNil);
        pure_println "  plus:  [%+d]"   (ArgsCons (AInt 42) ArgsNil);
        pure_println "  space: [% d]"   (ArgsCons (AInt 42) ArgsNil);
        pure_println "  neg:   [%d]"    (ArgsCons (AInt (0 - 7)) ArgsNil);
        pure_println "" ArgsNil;
        pure_println "Bases:" ArgsNil;
        pure_println "  hex:  0x%x" (ArgsCons (AHexLow 3735928559) ArgsNil);
        pure_println "  HEX:  0x%X" (ArgsCons (AHexUp 3735928559) ArgsNil);
        pure_println "  oct:  %o"   (ArgsCons (AOct 493) ArgsNil);
        pure_println "  bin:  %b"   (ArgsCons (ABin 170) ArgsNil);
        pure_println "  ptr:  %p"   (ArgsCons (APtr 48879) ArgsNil);
        pure_println "" ArgsNil;
        pure_println "Floats:" ArgsNil;
        pure_println "  pi        = %f"     (ArgsCons (AF64 3.14159265358979) ArgsNil);
        pure_println "  pi(2)     = %.2f"   (ArgsCons (AF64 3.14159265358979) ArgsNil);
        pure_println "  pi(0)     = %.0f"   (ArgsCons (AF64 3.14159265358979) ArgsNil);
        pure_println "  -pi(3)    = %.3f"   (ArgsCons (AF64 (0.0 - 3.14159265)) ArgsNil);
        pure_println "  width10   = [%10.4f]" (ArgsCons (AF64 2.5) ArgsNil);
        pure_println "" ArgsNil;
        pure_println "Strings with precision / width:" ArgsNil;
        pure_println "  [%.3s]"  (ArgsCons (AStr "abcdefg") ArgsNil);
        pure_println "  [%10s]"  (ArgsCons (AStr "hi") ArgsNil);
        pure_println "  [%-10s]" (ArgsCons (AStr "hi") ArgsNil);
        pure_println "" ArgsNil;
        pure_println "Recap: %s has %d items at %p, total %.2f"
            (ArgsCons (AStr "Order #1234")
            (ArgsCons (AInt 5)
            (ArgsCons (APtr 305419896)
            (ArgsCons (AF64 99.95) ArgsNil))));
        pure_println "" ArgsNil;
        pure_println "Done." ArgsNil
    };

main()