module @mak {
  format = "mLIR 0.1"
  optimized = true

  types {
    type @_a = adt
      ctor @A tag=1 ()
      ctor @B tag=2 (i32, !adt._a)
    type @_b = adt
      ctor @C tag=3 ()
      ctor @D tag=4 (!adt._a)
    type @_c = adt
      ctor @NBad tag=5 ()
      ctor @F tag=6 (i64, !adt._a)
    type @_d = adt
      ctor @NBad tag=7 ()
      ctor @H tag=8 (f64, !adt._a)
    type @_e = adt
      ctor @I tag=9 ()
      ctor @J tag=10 (i64, !adt._a)
    type @_f = adt
      ctor @K tag=11 (f64, !adt._a)
      ctor @L tag=12 ()
  }

  externals {
    extern @mak_ir_ext_0 symbol="getchar" type=fn() -> i32
    extern @mak_ir_ext_1 symbol="putchar" type=fn(i32) -> unit
    extern @mak_ir_ext_2 symbol="printf" type=fn(ptr, ...) -> i32
    extern @mak_ir_ext_3 symbol="strlen" type=fn(ptr) -> u64
    extern @mak_ir_ext_4 symbol="write" type=fn(i32, ptr, u64) -> unit
    extern @mak_ir_ext_5 symbol="sqrt" type=fn(f64) -> f64 pure
    extern @mak_ir_ext_6 symbol="cbrt" type=fn(f64) -> f64 pure
    extern @mak_ir_ext_7 symbol="fabs" type=fn(f64) -> f64 pure
    extern @mak_ir_ext_8 symbol="floor" type=fn(f64) -> f64 pure
    extern @mak_ir_ext_9 symbol="ceil" type=fn(f64) -> f64 pure
    extern @mak_ir_ext_10 symbol="trunc" type=fn(f64) -> f64 pure
    extern @mak_ir_ext_11 symbol="round" type=fn(f64) -> f64 pure
    extern @mak_ir_ext_12 symbol="fmod" type=fn(f64, f64) -> f64 pure
    extern @mak_ir_ext_13 symbol="remainder" type=fn(f64, f64) -> f64 pure
    extern @mak_ir_ext_14 symbol="pow" type=fn(f64, f64) -> f64 pure
    extern @mak_ir_ext_15 symbol="exp" type=fn(f64) -> f64 pure
    extern @mak_ir_ext_16 symbol="exp2" type=fn(f64) -> f64 pure
    extern @mak_ir_ext_17 symbol="expm1" type=fn(f64) -> f64 pure
    extern @mak_ir_ext_18 symbol="log" type=fn(f64) -> f64 pure
    extern @mak_ir_ext_19 symbol="log2" type=fn(f64) -> f64 pure
    extern @mak_ir_ext_20 symbol="log10" type=fn(f64) -> f64 pure
    extern @mak_ir_ext_21 symbol="log1p" type=fn(f64) -> f64 pure
    extern @mak_ir_ext_22 symbol="sin" type=fn(f64) -> f64 pure
    extern @mak_ir_ext_23 symbol="cos" type=fn(f64) -> f64 pure
    extern @mak_ir_ext_24 symbol="tan" type=fn(f64) -> f64 pure
    extern @mak_ir_ext_25 symbol="asin" type=fn(f64) -> f64 pure
    extern @mak_ir_ext_26 symbol="acos" type=fn(f64) -> f64 pure
    extern @mak_ir_ext_27 symbol="atan" type=fn(f64) -> f64 pure
    extern @mak_ir_ext_28 symbol="atan2" type=fn(f64, f64) -> f64 pure
    extern @mak_ir_ext_29 symbol="sinh" type=fn(f64) -> f64 pure
    extern @mak_ir_ext_30 symbol="cosh" type=fn(f64) -> f64 pure
    extern @mak_ir_ext_31 symbol="tanh" type=fn(f64) -> f64 pure
    extern @mak_ir_ext_32 symbol="asinh" type=fn(f64) -> f64 pure
    extern @mak_ir_ext_33 symbol="acosh" type=fn(f64) -> f64 pure
    extern @mak_ir_ext_34 symbol="atanh" type=fn(f64) -> f64 pure
    extern @mak_ir_ext_35 symbol="tgamma" type=fn(f64) -> f64 pure
    extern @mak_ir_ext_36 symbol="lgamma" type=fn(f64) -> f64 pure
    extern @mak_ir_ext_37 symbol="hypot" type=fn(f64, f64) -> f64 pure
    extern @mak_ir_ext_38 symbol="erf" type=fn(f64) -> f64 pure
    extern @mak_ir_ext_39 symbol="erfc" type=fn(f64) -> f64 pure
    extern @mak_ir_ext_40 symbol="logb" type=fn(f64) -> f64 pure
    extern @mak_ir_ext_41 symbol="copysign" type=fn(f64, f64) -> f64 pure
    extern @mak_ir_ext_42 symbol="fdim" type=fn(f64, f64) -> f64 pure
    extern @mak_ir_ext_43 symbol="fmax" type=fn(f64, f64) -> f64 pure
    extern @mak_ir_ext_44 symbol="fmin" type=fn(f64, f64) -> f64 pure
    extern @mak_ir_ext_45 symbol="fma" type=fn(f64, f64, f64) -> f64 pure
    extern @mak_ir_ext_46 symbol="nextafter" type=fn(f64, f64) -> f64 pure
  }

  functions {
    fn @mak_ir_fn_0 ["a"] : (%a0: !adt._a, %a1: !adt._a) -> !adt._a {
      case %a0:!adt._a : !adt._a {
        @k0 => %a1:!adt._a
        @k1(%mak_local_az_0:i32, %mak_local_ba_1:!adt._a) => call @f0(%mak_local_ba_1:!adt._a, make @k1(%mak_local_az_0:i32, %a1:!adt._a) : !adt._a) : !adt._a
      }
    }
    fn @mak_ir_fn_1 ["b"] : (%a0: !adt._a) -> !adt._b {
      case call @z0() : i32 : !adt._b {
        %mak_local_bc_2:i32 => case bin.lt %mak_local_bc_2:i32 const.i64 0 : i32 : !adt._b { 1 => make @k3(call @f0(%a0:!adt._a, make @k0() : !adt._a) : !adt._a) : !adt._b; _ => case bin.eq %mak_local_bc_2:i32 const.i64 10 : i32 : !adt._b { 1 => make @k3(call @f0(%a0:!adt._a, make @k0() : !adt._a) : !adt._a) : !adt._b; _ => call @f1(make @k1(%mak_local_bc_2:i32, %a0:!adt._a) : !adt._a) : !adt._b; }; }
      }
    }
    fn @mak_ir_fn_2 ["e"] : (%a0: !adt._a) -> !adt._a {
      case %a0:!adt._a : !adt._a {
        @k0 => make @k0() : !adt._a
        @k1(%mak_local_bg_3:i32, %mak_local_bh_4:!adt._a) => case case %mak_local_bg_3:i32 : i64 { 9 => const.i64 1; 10 => const.i64 1; 13 => const.i64 1; 32 => const.i64 1; _ => const.i64 0; } : !adt._a { 1 => call @f2(%mak_local_bh_4:!adt._a) : !adt._a; _ => %a0:!adt._a; }
      }
    }
    fn @mak_ir_fn_3 ["o"] : (%a0: !adt._a, %a1: i64) -> !adt._c {
      case %a0:!adt._a : !adt._c {
        @k1(%mak_local_cb_5:i32, %mak_local_cc_6:!adt._a) => case case case bin.eq %mak_local_cb_5:i32 const.i64 95 : i32 : i64 { 1 => const.i64 1; _ => case bin.ge %mak_local_cb_5:i32 const.i64 65 : i32 : i64 { 0 => case bin.ge %mak_local_cb_5:i32 const.i64 97 : i32 : i64 { 0 => const.i64 0; _ => case bin.le %mak_local_cb_5:i32 const.i64 122 : i32 : i64 { 1 => const.i64 1; _ => const.i64 0; }; }; _ => case bin.le %mak_local_cb_5:i32 const.i64 90 : i32 : i64 { 1 => const.i64 1; _ => const.i64 0; }; }; } : i64 { 1 => const.i64 1; _ => case bin.ge %mak_local_cb_5:i32 const.i64 48 : i32 : i64 { 0 => const.i64 0; _ => case bin.le %mak_local_cb_5:i32 const.i64 57 : i32 : i64 { 1 => const.i64 1; _ => const.i64 0; }; }; } : !adt._c { 1 => call @f3(%mak_local_cc_6:!adt._a, bin.rem bin.add bin.mul %a1:i64 const.i64 131 : i64 case bin.ge %mak_local_cb_5:i32 const.i64 65 : i32 : i64 { 0 => %mak_local_cb_5:i32; _ => case bin.le %mak_local_cb_5:i32 const.i64 90 : i32 : i64 { 1 => bin.add %mak_local_cb_5:i32 const.i64 32 : i64; _ => %mak_local_cb_5:i32; }; } : i64 const.i64 1000000007 : i64) : !adt._c; _ => make @k5(%a1:i64, %a0:!adt._a) : !adt._c; }
        @k0 => make @k5(%a1:i64, make @k0() : !adt._a) : !adt._c
      }
    }
    fn @mak_ir_fn_4 ["u"] : (%a0: i64) -> f64 {
      case %a0:i64 : f64 {
        0 => const.f64 1.00000000000000000
        _ => case bin.gt %a0:i64 const.i64 0 : i32 : f64 { 1 => bin.mul const.f64 10.00000000000000000 call @f4(bin.sub %a0:i64 const.i64 1 : i64) : f64 : f64; _ => bin.mul const.f64 0.10000000000000001 call @f4(bin.add %a0:i64 const.i64 1 : i64) : f64 : f64; }
      }
    }
    fn @mak_ir_fn_5 ["v"] : (%a0: !adt._a, %a1: i64, %a2: i64) -> !adt._e {
      case %a0:!adt._a : !adt._e {
        @k1(%mak_local_co_7:i32, %mak_local_cp_8:!adt._a) => case case bin.ge %mak_local_co_7:i32 const.i64 48 : i32 : i64 { 0 => const.i64 0; _ => case bin.le %mak_local_co_7:i32 const.i64 57 : i32 : i64 { 1 => const.i64 1; _ => const.i64 0; }; } : !adt._e { 1 => call @f5(%mak_local_cp_8:!adt._a, bin.sub bin.add bin.mul %a1:i64 const.i64 10 : i64 %mak_local_co_7:i32 : i64 const.i64 48 : i64, const.i64 1) : !adt._e; _ => case %a2:i64 : !adt._e { 1 => make @k9(%a1:i64, %a0:!adt._a) : !adt._e; _ => make @k8() : !adt._e; }; }
        @k0 => case %a2:i64 : !adt._e { 1 => make @k9(%a1:i64, make @k0() : !adt._a) : !adt._e; _ => make @k8() : !adt._e; }
      }
    }
    fn @mak_ir_fn_6 ["w"] : (%a0: !adt._a, %a1: i64) -> !adt._e {
      case call @f2(%a0:!adt._a) : !adt._a : !adt._e {
        @k1(%mak_local_cs_9:i32, %mak_local_ct_10:!adt._a) => case %mak_local_cs_9:i32 : !adt._e { 43 => call @f5(%mak_local_ct_10:!adt._a, const.i64 0, const.i64 0) : !adt._e; 45 => case call @f5(%mak_local_ct_10:!adt._a, const.i64 0, const.i64 0) : !adt._e : !adt._e { @k8 => make @k8() : !adt._e; @k9(%mak_local_cu_11:i64, %mak_local_cv_12:!adt._a) => make @k9(un.neg %mak_local_cu_11:i64 : i64, %mak_local_cv_12:!adt._a) : !adt._e; }; _ => case case bin.ge %mak_local_cs_9:i32 const.i64 48 : i32 : i64 { 0 => const.i64 0; _ => case bin.le %mak_local_cs_9:i32 const.i64 57 : i32 : i64 { 1 => const.i64 1; _ => const.i64 0; }; } : !adt._e { 1 => call @f5(make @k1(%mak_local_cs_9:i32, %mak_local_ct_10:!adt._a) : !adt._a, const.i64 0, const.i64 0) : !adt._e; _ => make @k8() : !adt._e; }; }
        @k0 => make @k8() : !adt._e
      }
    }
    fn @mak_ir_fn_7 ["x"] : (%a0: !adt._a, %a1: f64) -> !adt._d {
      case %a0:!adt._a : !adt._d {
        @k1(%mak_local_cy_13:i32, %mak_local_cz_14:!adt._a) => case case bin.ge %mak_local_cy_13:i32 const.i64 48 : i32 : i64 { 0 => const.i64 0; _ => case bin.le %mak_local_cy_13:i32 const.i64 57 : i32 : i64 { 1 => const.i64 1; _ => const.i64 0; }; } : !adt._d { 1 => call @f7(%mak_local_cz_14:!adt._a, bin.add bin.mul %a1:f64 const.f64 10.00000000000000000 : f64 case %mak_local_cy_13:i32 : f64 { 48 => const.f64 0.00000000000000000; 49 => const.f64 1.00000000000000000; 50 => const.f64 2.00000000000000000; 51 => const.f64 3.00000000000000000; 52 => const.f64 4.00000000000000000; 53 => const.f64 5.00000000000000000; 54 => const.f64 6.00000000000000000; 55 => const.f64 7.00000000000000000; 56 => const.f64 8.00000000000000000; 57 => const.f64 9.00000000000000000; _ => const.f64 0.00000000000000000; } : f64) : !adt._d; _ => make @k7(%a1:f64, %a0:!adt._a) : !adt._d; }
        @k0 => make @k7(%a1:f64, make @k0() : !adt._a) : !adt._d
      }
    }
    fn @mak_ir_fn_8 ["y"] : (%a0: !adt._a, %a1: f64, %a2: f64) -> !adt._d {
      case %a0:!adt._a : !adt._d {
        @k1(%mak_local_dd_15:i32, %mak_local_de_16:!adt._a) => case case bin.ge %mak_local_dd_15:i32 const.i64 48 : i32 : i64 { 0 => const.i64 0; _ => case bin.le %mak_local_dd_15:i32 const.i64 57 : i32 : i64 { 1 => const.i64 1; _ => const.i64 0; }; } : !adt._d { 1 => call @f8(%mak_local_de_16:!adt._a, bin.add %a1:f64 bin.mul case %mak_local_dd_15:i32 : f64 { 48 => const.f64 0.00000000000000000; 49 => const.f64 1.00000000000000000; 50 => const.f64 2.00000000000000000; 51 => const.f64 3.00000000000000000; 52 => const.f64 4.00000000000000000; 53 => const.f64 5.00000000000000000; 54 => const.f64 6.00000000000000000; 55 => const.f64 7.00000000000000000; 56 => const.f64 8.00000000000000000; 57 => const.f64 9.00000000000000000; _ => const.f64 0.00000000000000000; } %a2:f64 : f64 : f64, bin.mul %a2:f64 const.f64 0.10000000000000001 : f64) : !adt._d; _ => case %mak_local_dd_15:i32 : !adt._d { 101 => case call @f6(%mak_local_de_16:!adt._a, const.i64 1) : !adt._e : !adt._d { @k8 => make @k6() : !adt._d; @k9(%mak_local_df_17:i64, %mak_local_dg_18:!adt._a) => make @k7(bin.mul %a1:f64 call @f4(%mak_local_df_17:i64) : f64 : f64, %mak_local_dg_18:!adt._a) : !adt._d; }; 69 => case call @f6(%mak_local_de_16:!adt._a, const.i64 1) : !adt._e : !adt._d { @k8 => make @k6() : !adt._d; @k9(%mak_local_dh_19:i64, %mak_local_di_20:!adt._a) => make @k7(bin.mul %a1:f64 call @f4(%mak_local_dh_19:i64) : f64 : f64, %mak_local_di_20:!adt._a) : !adt._d; }; _ => make @k7(%a1:f64, %a0:!adt._a) : !adt._d; }; }
        @k0 => make @k7(%a1:f64, make @k0() : !adt._a) : !adt._d
      }
    }
    fn @mak_ir_fn_9 ["ae"] : (%a0: i64, %a1: f64, %a2: f64) -> f64 {
      case %a0:i64 : f64 {
        828949774 => call @z28(%a1:f64, %a2:f64) : f64 [pure]
        231190472 => call @z12(%a1:f64, %a2:f64) : f64 [pure]
        1885190 => call @z12(%a1:f64, %a2:f64) : f64 [pure]
        1969694 => call @z12(%a1:f64, %a2:f64) : f64 [pure]
        510764621 => call @z13(%a1:f64, %a2:f64) : f64 [pure]
        1936692 => call @z14(%a1:f64, %a2:f64) : f64 [pure]
        258201902 => call @z14(%a1:f64, bin.div const.f64 1.00000000000000000 %a2:f64 : f64) : f64 [pure]
        548941060 => call @z14(%a1:f64, bin.div const.f64 1.00000000000000000 %a2:f64 : f64) : f64 [pure]
        901947274 => call @z37(%a1:f64, %a2:f64) : f64 [pure]
        1780652 => let %mak_local_eg_21:f64 = call @z7(%a2:f64) : f64 [pure] in case bin.eq %mak_local_eg_21:f64 const.f64 0.00000000000000000 : i32 : f64 { 1 => call @z7(%a1:f64) : f64 [pure]; _ => call @f9(const.i64 1780652, %mak_local_eg_21:f64, call @z12(call @z7(%a1:f64) : f64 [pure], %mak_local_eg_21:f64) : f64 [pure]) : f64; }
        1866466 => let %mak_local_eh_22:f64 = call @f9(const.i64 1780652, %a1:f64, %a2:f64) : f64 in case bin.eq %mak_local_eh_22:f64 const.f64 0.00000000000000000 : i32 : f64 { 1 => const.f64 0.00000000000000000; _ => bin.div call @z7(bin.mul %a1:f64 %a2:f64 : f64) : f64 [pure] %mak_local_eh_22:f64 : f64; }
        1900793 => let %mak_local_ej_23:f64 = call @z11(%a2:f64) : f64 [pure] in case bin.lt %mak_local_ej_23:f64 const.f64 0.00000000000000000 : i32 : f64 { 1 => const.f64 0.00000000000000000; _ => case bin.gt %mak_local_ej_23:f64 call @z11(%a1:f64) : f64 [pure] : i32 : f64 { 1 => const.f64 0.00000000000000000; _ => call @z11(call @z15(bin.sub bin.sub call @z36(bin.add call @z11(%a1:f64) : f64 [pure] const.f64 1.00000000000000000 : f64) : f64 [pure] call @z36(bin.add %mak_local_ej_23:f64 const.f64 1.00000000000000000 : f64) : f64 [pure] : f64 call @z36(bin.add bin.sub call @z11(%a1:f64) : f64 [pure] %mak_local_ej_23:f64 : f64 const.f64 1.00000000000000000 : f64) : f64 [pure] : f64) : f64 [pure]) : f64 [pure]; }; }
        224480257 => call @f9(const.i64 1900793, %a1:f64, %a2:f64) : f64
        1902496 => let %mak_local_el_24:f64 = call @z11(%a2:f64) : f64 [pure] in case bin.lt %mak_local_el_24:f64 const.f64 0.00000000000000000 : i32 : f64 { 1 => const.f64 0.00000000000000000; _ => case bin.gt %mak_local_el_24:f64 call @z11(%a1:f64) : f64 [pure] : i32 : f64 { 1 => const.f64 0.00000000000000000; _ => call @z11(call @z15(bin.sub call @z36(bin.add call @z11(%a1:f64) : f64 [pure] const.f64 1.00000000000000000 : f64) : f64 [pure] call @z36(bin.add bin.sub call @z11(%a1:f64) : f64 [pure] %mak_local_el_24:f64 : f64 const.f64 1.00000000000000000 : f64) : f64 [pure] : f64) : f64 [pure]) : f64 [pure]; }; }
        253534496 => call @f9(const.i64 1902496, %a1:f64, %a2:f64) : f64
        1884414 => case bin.lt %a1:f64 %a2:f64 : i32 : f64 { 1 => %a1:f64; _ => %a2:f64; }
        1883376 => case bin.gt %a1:f64 %a2:f64 : i32 : f64 { 1 => %a1:f64; _ => %a2:f64; }
        262106026 => call @z41(%a1:f64, %a2:f64) : f64 [pure]
        231035246 => call @z42(%a1:f64, %a2:f64) : f64 [pure]
        231188658 => call @z43(%a1:f64, %a2:f64) : f64 [pure]
        231189696 => call @z44(%a1:f64, %a2:f64) : f64 [pure]
        743972013 => call @z46(%a1:f64, %a2:f64) : f64 [pure]
        _ => const.f64 0.00000000000000000
      }
    }
    fn @mak_ir_fn_10 ["al"] : (%a0: f64, %a1: !adt._a) -> !adt._f {
      case call @f2(%a1:!adt._a) : !adt._a : !adt._f {
        @k1(33, %mak_local_ge_25:!adt._a) => call @f10(call @z35(bin.add %a0:f64 const.f64 1.00000000000000000 : f64) : f64 [pure], %mak_local_ge_25:!adt._a) : !adt._f
        @k1(37, %mak_local_gf_26:!adt._a) => case call @f2(%mak_local_gf_26:!adt._a) : !adt._a : !adt._f { @k0 => make @k10(bin.div %a0:f64 const.f64 100.00000000000000000 : f64, make @k0() : !adt._a) : !adt._f; @k1(%mak_local_gg_27:i32, _) => case %mak_local_gg_27:i32 : !adt._f { 41 => make @k10(bin.div %a0:f64 const.f64 100.00000000000000000 : f64, %mak_local_gf_26:!adt._a) : !adt._f; 44 => make @k10(bin.div %a0:f64 const.f64 100.00000000000000000 : f64, %mak_local_gf_26:!adt._a) : !adt._f; 43 => make @k10(bin.div %a0:f64 const.f64 100.00000000000000000 : f64, %mak_local_gf_26:!adt._a) : !adt._f; 45 => make @k10(bin.div %a0:f64 const.f64 100.00000000000000000 : f64, %mak_local_gf_26:!adt._a) : !adt._f; 42 => make @k10(bin.div %a0:f64 const.f64 100.00000000000000000 : f64, %mak_local_gf_26:!adt._a) : !adt._f; 47 => make @k10(bin.div %a0:f64 const.f64 100.00000000000000000 : f64, %mak_local_gf_26:!adt._a) : !adt._f; 94 => make @k10(bin.div %a0:f64 const.f64 100.00000000000000000 : f64, %mak_local_gf_26:!adt._a) : !adt._f; _ => make @k10(%a0:f64, %a1:!adt._a) : !adt._f; }; }
        _ => make @k10(%a0:f64, %a1:!adt._a) : !adt._f
      }
    }
    fn @mak_ir_fn_11 ["an"] : (%a0: !adt._a) -> !adt._f {
      case case case call @f2(%a0:!adt._a) : !adt._a : !adt._f { @k0 => make @k11() : !adt._f; @k1(%mak_local_fs_28:i32, %mak_local_ft_29:!adt._a) => case %mak_local_fs_28:i32 : !adt._f { 40 => case call @f16(%mak_local_ft_29:!adt._a) : !adt._f : !adt._f { @k11 => make @k11() : !adt._f; @k10(%mak_local_fu_30:f64, %mak_local_fv_31:!adt._a) => case call @f2(%mak_local_fv_31:!adt._a) : !adt._a : !adt._f { @k1(41, %mak_local_fw_32:!adt._a) => make @k10(%mak_local_fu_30:f64, %mak_local_fw_32:!adt._a) : !adt._f; _ => make @k11() : !adt._f; }; }; _ => case case bin.eq %mak_local_fs_28:i32 const.i64 95 : i32 : i64 { 1 => const.i64 1; _ => case bin.ge %mak_local_fs_28:i32 const.i64 65 : i32 : i64 { 0 => case bin.ge %mak_local_fs_28:i32 const.i64 97 : i32 : i64 { 0 => const.i64 0; _ => case bin.le %mak_local_fs_28:i32 const.i64 122 : i32 : i64 { 1 => const.i64 1; _ => const.i64 0; }; }; _ => case bin.le %mak_local_fs_28:i32 const.i64 90 : i32 : i64 { 1 => const.i64 1; _ => const.i64 0; }; }; } : !adt._f { 1 => case case call @f2(make @k1(%mak_local_fs_28:i32, %mak_local_ft_29:!adt._a) : !adt._a) : !adt._a : !adt._f { @k1(%mak_local_ce_33:i32, %mak_local_cf_34:!adt._a) => case case bin.eq %mak_local_ce_33:i32 const.i64 95 : i32 : i64 { 1 => const.i64 1; _ => case bin.ge %mak_local_ce_33:i32 const.i64 65 : i32 : i64 { 0 => case bin.ge %mak_local_ce_33:i32 const.i64 97 : i32 : i64 { 0 => const.i64 0; _ => case bin.le %mak_local_ce_33:i32 const.i64 122 : i32 : i64 { 1 => const.i64 1; _ => const.i64 0; }; }; _ => case bin.le %mak_local_ce_33:i32 const.i64 90 : i32 : i64 { 1 => const.i64 1; _ => const.i64 0; }; }; } : !adt._f { 1 => call @f3(make @k1(%mak_local_ce_33:i32, %mak_local_cf_34:!adt._a) : !adt._a, const.i64 0) : !adt._c; _ => runtime-ctor NBad:_f : !adt._f; }; @k0 => runtime-ctor NBad:_f : !adt._f; } : !adt._f { @k11 => make @k11() : !adt._f; @k10(%mak_local_fx_35:f64, %mak_local_fy_36:!adt._a) => case call @f2(%mak_local_fy_36:!adt._a) : !adt._a : !adt._f { @k1(40, %mak_local_fz_37:!adt._a) => make @k11() : !adt._f; _ => make @k11() : !adt._f; }; }; _ => case case call @f2(%a0:!adt._a) : !adt._a : !adt._f { @k0 => runtime-ctor NBad:_f : !adt._f; @k1(%mak_local_dk_56:i32, %mak_local_dl_57:!adt._a) => case case bin.ge %mak_local_dk_56:i32 const.i64 48 : i32 : i64 { 0 => const.i64 0; _ => case bin.le %mak_local_dk_56:i32 const.i64 57 : i32 : i64 { 1 => const.i64 1; _ => const.i64 0; }; } : !adt._f { 1 => case call @f7(make @k1(%mak_local_dk_56:i32, %mak_local_dl_57:!adt._a) : !adt._a, const.f64 0.00000000000000000) : !adt._d : !adt._f { @k6 => runtime-ctor NBad:_f : !adt._f; @k7(%mak_local_dm_58:f64, %mak_local_dn_59:!adt._a) => case %mak_local_dn_59:!adt._a : !adt._d { @k1(46, %mak_local_dp_60:!adt._a) => call @f8(%mak_local_dp_60:!adt._a, %mak_local_dm_58:f64, const.f64 0.10000000000000001) : !adt._d; @k1(%mak_local_dq_61:i32, %mak_local_dr_62:!adt._a) => case %mak_local_dq_61:i32 : !adt._f { 101 => case call @f6(%mak_local_dr_62:!adt._a, const.i64 1) : !adt._e : !adt._f { @k8 => runtime-ctor NBad:_f : !adt._f; @k9(%mak_local_ds_63:i64, %mak_local_dt_64:!adt._a) => make @k7(bin.mul %mak_local_dm_58:f64 call @f4(%mak_local_ds_63:i64) : f64 : f64, %mak_local_dt_64:!adt._a) : !adt._d; }; 69 => case call @f6(%mak_local_dr_62:!adt._a, const.i64 1) : !adt._e : !adt._f { @k8 => runtime-ctor NBad:_f : !adt._f; @k9(%mak_local_du_65:i64, %mak_local_dv_66:!adt._a) => make @k7(bin.mul %mak_local_dm_58:f64 call @f4(%mak_local_du_65:i64) : f64 : f64, %mak_local_dv_66:!adt._a) : !adt._d; }; _ => make @k7(%mak_local_dm_58:f64, %mak_local_dn_59:!adt._a) : !adt._d; }; _ => make @k7(%mak_local_dm_58:f64, %mak_local_dn_59:!adt._a) : !adt._d; }; }; _ => case %mak_local_dk_56:i32 : !adt._f { 46 => case %mak_local_dl_57:!adt._a : !adt._f { @k1(%mak_local_dw_67:i32, _) => case case bin.ge %mak_local_dw_67:i32 const.i64 48 : i32 : i64 { 0 => const.i64 0; _ => case bin.le %mak_local_dw_67:i32 const.i64 57 : i32 : i64 { 1 => const.i64 1; _ => const.i64 0; }; } : !adt._f { 1 => call @f8(%mak_local_dl_57:!adt._a, const.f64 0.00000000000000000, const.f64 0.10000000000000001) : !adt._d; _ => runtime-ctor NBad:_f : !adt._f; }; _ => runtime-ctor NBad:_f : !adt._f; }; _ => runtime-ctor NBad:_f : !adt._f; }; }; } : !adt._f { @k11 => make @k11() : !adt._f; @k10(%mak_local_ga_68:f64, %mak_local_gb_69:!adt._a) => make @k10(%mak_local_ga_68:f64, %mak_local_gb_69:!adt._a) : !adt._f; }; }; }; } : !adt._f { @k11 => make @k11() : !adt._f; @k10(%mak_local_gi_70:f64, %mak_local_gj_71:!adt._a) => call @f10(%mak_local_gi_70:f64, %mak_local_gj_71:!adt._a) : !adt._f; } : !adt._f {
        @k11 => make @k11() : !adt._f
        @k10(%mak_local_gl_72:f64, %mak_local_gm_73:!adt._a) => case call @f2(%mak_local_gm_73:!adt._a) : !adt._a : !adt._f { @k1(94, %mak_local_gn_74:!adt._a) => case call @f12(%mak_local_gn_74:!adt._a) : !adt._f : !adt._f { @k11 => make @k11() : !adt._f; @k10(%mak_local_go_75:f64, %mak_local_gp_76:!adt._a) => make @k10(call @z14(%mak_local_gl_72:f64, %mak_local_go_75:f64) : f64 [pure], %mak_local_gp_76:!adt._a) : !adt._f; }; _ => make @k10(%mak_local_gl_72:f64, %mak_local_gm_73:!adt._a) : !adt._f; }
      }
    }
    fn @mak_ir_fn_12 ["ao"] : (%a0: !adt._a) -> !adt._f {
      case call @f2(%a0:!adt._a) : !adt._a : !adt._f {
        @k0 => make @k11() : !adt._f
        @k1(%mak_local_gr_77:i32, %mak_local_gs_78:!adt._a) => case %mak_local_gr_77:i32 : !adt._f { 43 => call @f12(%mak_local_gs_78:!adt._a) : !adt._f; 45 => case call @f12(%mak_local_gs_78:!adt._a) : !adt._f : !adt._f { @k11 => make @k11() : !adt._f; @k10(%mak_local_gt_79:f64, %mak_local_gu_80:!adt._a) => make @k10(bin.sub const.f64 0.00000000000000000 %mak_local_gt_79:f64 : f64, %mak_local_gu_80:!adt._a) : !adt._f; }; _ => call @f11(%a0:!adt._a) : !adt._f; }
      }
    }
    fn @mak_ir_fn_13 ["ap"] : (%a0: f64, %a1: !adt._a) -> !adt._f {
      case call @f2(%a1:!adt._a) : !adt._a : !adt._f {
        @k0 => make @k10(%a0:f64, make @k0() : !adt._a) : !adt._f
        @k1(%mak_local_gx_81:i32, %mak_local_gy_82:!adt._a) => case %mak_local_gx_81:i32 : !adt._f { 42 => case call @f11(%mak_local_gy_82:!adt._a) : !adt._f : !adt._f { @k11 => make @k11() : !adt._f; @k10(%mak_local_gz_83:f64, %mak_local_ha_84:!adt._a) => call @f13(bin.mul %a0:f64 %mak_local_gz_83:f64 : f64, %mak_local_ha_84:!adt._a) : !adt._f; }; 47 => case %mak_local_gy_82:!adt._a : !adt._f { @k1(47, %mak_local_hb_85:!adt._a) => case call @f11(%mak_local_hb_85:!adt._a) : !adt._f : !adt._f { @k11 => make @k11() : !adt._f; @k10(%mak_local_hc_86:f64, %mak_local_hd_87:!adt._a) => case bin.eq %mak_local_hc_86:f64 const.f64 0.00000000000000000 : i32 : !adt._f { 1 => make @k11() : !adt._f; _ => call @f13(call @z8(bin.div %a0:f64 %mak_local_hc_86:f64 : f64) : f64 [pure], %mak_local_hd_87:!adt._a) : !adt._f; }; }; _ => case call @f11(%mak_local_gy_82:!adt._a) : !adt._f : !adt._f { @k11 => make @k11() : !adt._f; @k10(%mak_local_he_88:f64, %mak_local_hf_89:!adt._a) => case bin.eq %mak_local_he_88:f64 const.f64 0.00000000000000000 : i32 : !adt._f { 1 => make @k11() : !adt._f; _ => call @f13(bin.div %a0:f64 %mak_local_he_88:f64 : f64, %mak_local_hf_89:!adt._a) : !adt._f; }; }; }; 37 => case call @f11(%mak_local_gy_82:!adt._a) : !adt._f : !adt._f { @k11 => make @k11() : !adt._f; @k10(%mak_local_hg_90:f64, %mak_local_hh_91:!adt._a) => case bin.eq %mak_local_hg_90:f64 const.f64 0.00000000000000000 : i32 : !adt._f { 1 => make @k11() : !adt._f; _ => call @f13(call @z12(%a0:f64, %mak_local_hg_90:f64) : f64 [pure], %mak_local_hh_91:!adt._a) : !adt._f; }; }; _ => case case bin.eq %mak_local_gx_81:i32 const.i64 95 : i32 : i64 { 1 => const.i64 1; _ => case bin.ge %mak_local_gx_81:i32 const.i64 65 : i32 : i64 { 0 => case bin.ge %mak_local_gx_81:i32 const.i64 97 : i32 : i64 { 0 => const.i64 0; _ => case bin.le %mak_local_gx_81:i32 const.i64 122 : i32 : i64 { 1 => const.i64 1; _ => const.i64 0; }; }; _ => case bin.le %mak_local_gx_81:i32 const.i64 90 : i32 : i64 { 1 => const.i64 1; _ => const.i64 0; }; }; } : !adt._f { 1 => case call @f11(%a1:!adt._a) : !adt._f : !adt._f { @k11 => make @k10(%a0:f64, %a1:!adt._a) : !adt._f; @k10(%mak_local_hi_92:f64, %mak_local_hj_93:!adt._a) => call @f13(bin.mul %a0:f64 %mak_local_hi_92:f64 : f64, %mak_local_hj_93:!adt._a) : !adt._f; }; _ => case case bin.ge %mak_local_gx_81:i32 const.i64 48 : i32 : i64 { 0 => const.i64 0; _ => case bin.le %mak_local_gx_81:i32 const.i64 57 : i32 : i64 { 1 => const.i64 1; _ => const.i64 0; }; } : !adt._f { 1 => case call @f11(%a1:!adt._a) : !adt._f : !adt._f { @k11 => make @k10(%a0:f64, %a1:!adt._a) : !adt._f; @k10(%mak_local_hk_94:f64, %mak_local_hl_95:!adt._a) => call @f13(bin.mul %a0:f64 %mak_local_hk_94:f64 : f64, %mak_local_hl_95:!adt._a) : !adt._f; }; _ => case %mak_local_gx_81:i32 : !adt._f { 46 => case call @f11(%a1:!adt._a) : !adt._f : !adt._f { @k11 => make @k10(%a0:f64, %a1:!adt._a) : !adt._f; @k10(%mak_local_hm_96:f64, %mak_local_hn_97:!adt._a) => call @f13(bin.mul %a0:f64 %mak_local_hm_96:f64 : f64, %mak_local_hn_97:!adt._a) : !adt._f; }; 40 => case call @f11(%a1:!adt._a) : !adt._f : !adt._f { @k11 => make @k10(%a0:f64, %a1:!adt._a) : !adt._f; @k10(%mak_local_ho_98:f64, %mak_local_hp_99:!adt._a) => call @f13(bin.mul %a0:f64 %mak_local_ho_98:f64 : f64, %mak_local_hp_99:!adt._a) : !adt._f; }; _ => make @k10(%a0:f64, %a1:!adt._a) : !adt._f; }; }; }; }
      }
    }
    fn @mak_ir_fn_14 ["aq"] : (%a0: !adt._a) -> !adt._f {
      case call @f11(%a0:!adt._a) : !adt._f : !adt._f {
        @k11 => make @k11() : !adt._f
        @k10(%mak_local_hr_100:f64, %mak_local_hs_101:!adt._a) => call @f13(%mak_local_hr_100:f64, %mak_local_hs_101:!adt._a) : !adt._f
      }
    }
    fn @mak_ir_fn_15 ["ar"] : (%a0: f64, %a1: !adt._a) -> !adt._f {
      case call @f2(%a1:!adt._a) : !adt._a : !adt._f {
        @k0 => make @k10(%a0:f64, make @k0() : !adt._a) : !adt._f
        @k1(%mak_local_hv_102:i32, %mak_local_hw_103:!adt._a) => case %mak_local_hv_102:i32 : !adt._f { 43 => case call @f14(%mak_local_hw_103:!adt._a) : !adt._f : !adt._f { @k11 => make @k11() : !adt._f; @k10(%mak_local_hx_104:f64, %mak_local_hy_105:!adt._a) => call @f15(bin.add %a0:f64 %mak_local_hx_104:f64 : f64, %mak_local_hy_105:!adt._a) : !adt._f; }; 45 => case call @f14(%mak_local_hw_103:!adt._a) : !adt._f : !adt._f { @k11 => make @k11() : !adt._f; @k10(%mak_local_hz_106:f64, %mak_local_ia_107:!adt._a) => call @f15(bin.sub %a0:f64 %mak_local_hz_106:f64 : f64, %mak_local_ia_107:!adt._a) : !adt._f; }; _ => make @k10(%a0:f64, %a1:!adt._a) : !adt._f; }
      }
    }
    fn @mak_ir_fn_16 ["at"] : (%a0: !adt._a) -> !adt._f {
      case call @f14(%a0:!adt._a) : !adt._f : !adt._f {
        @k11 => make @k11() : !adt._f
        @k10(%mak_local_ic_108:f64, %mak_local_id_109:!adt._a) => call @f15(%mak_local_ic_108:f64, %mak_local_id_109:!adt._a) : !adt._f
      }
    }
    fn @mak_ir_fn_17 ["av"] : () -> unit {
      block {
        yield call @z4(const.i64 1, const.str ">>> ", call @z3(const.str ">>> ") : u64) : unit
        yield case case call @z0() : i32 : !adt._b { %mak_local_bd_110:i32 => case bin.lt %mak_local_bd_110:i32 const.i64 0 : i32 : !adt._b { 1 => make @k2() : !adt._b; _ => case bin.eq %mak_local_bd_110:i32 const.i64 10 : i32 : !adt._b { 1 => make @k3(make @k0() : !adt._a) : !adt._b; _ => call @f1(make @k1(%mak_local_bd_110:i32, make @k0() : !adt._a) : !adt._a) : !adt._b; }; }; } : unit { @k2 => call @z1(const.i64 10) : unit; @k3(%mak_local_ii_111:!adt._a) => case case call @f2(%mak_local_ii_111:!adt._a) : !adt._a : i64 { @k0 => const.i64 0; @k1(101, %mak_local_bm_112:!adt._a) => case %mak_local_bm_112:!adt._a : i64 { @k1(120, %mak_local_bn_113:!adt._a) => case %mak_local_bn_113:!adt._a : i64 { @k1(105, %mak_local_bo_114:!adt._a) => case %mak_local_bo_114:!adt._a : i64 { @k1(116, %mak_local_bp_115:!adt._a) => case call @f2(%mak_local_bp_115:!adt._a) : !adt._a : i64 { @k0 => const.i64 1; _ => const.i64 0; }; _ => const.i64 0; }; _ => const.i64 0; }; _ => const.i64 0; }; _ => const.i64 0; } : unit { 1 => call @z1(const.i64 10) : unit; _ => case case call @f2(%mak_local_ii_111:!adt._a) : !adt._a : i64 { @k1(63, %mak_local_br_116:!adt._a) => case call @f2(%mak_local_br_116:!adt._a) : !adt._a : i64 { @k0 => const.i64 1; _ => const.i64 0; }; @k1(104, %mak_local_bs_117:!adt._a) => case %mak_local_bs_117:!adt._a : i64 { @k1(101, %mak_local_bt_118:!adt._a) => case %mak_local_bt_118:!adt._a : i64 { @k1(108, %mak_local_bu_119:!adt._a) => case %mak_local_bu_119:!adt._a : i64 { @k1(112, %mak_local_bv_120:!adt._a) => case call @f2(%mak_local_bv_120:!adt._a) : !adt._a : i64 { @k0 => const.i64 1; _ => const.i64 0; }; _ => const.i64 0; }; _ => const.i64 0; }; _ => const.i64 0; }; _ => const.i64 0; } : unit { 1 => block { call @z4(const.i64 1, const.str "\n=== MAK SCIENTIFIC CALCULATOR ===\n", call @z3(const.str "\n=== MAK SCIENTIFIC CALCULATOR ===\n") : u64) : unit; call @z4(const.i64 1, const.str "Numbers are f64 (double precision). Output uses up to 17 significant digits.\n", call @z3(const.str "Numbers are f64 (double precision). Output uses up to 17 significant digits.\n") : u64) : unit; call @z4(const.i64 1, const.str "Use radians for trig functions.\n\n", call @z3(const.str "Use radians for trig functions.\n\n") : u64) : unit; call @z4(const.i64 1, const.str "ARITHMETIC\n", call @z3(const.str "ARITHMETIC\n") : u64) : unit; call @z4(const.i64 1, const.str "  a+b       addition\n", call @z3(const.str "  a+b       addition\n") : u64) : unit; call @z4(const.i64 1, const.str "  a-b       subtraction\n", call @z3(const.str "  a-b       subtraction\n") : u64) : unit; call @z4(const.i64 1, const.str "  a*b       multiplication\n", call @z3(const.str "  a*b       multiplication\n") : u64) : unit; call @z4(const.i64 1, const.str "  a/b       REAL division\n", call @z3(const.str "  a/b       REAL division\n") : u64) : unit; call @z4(const.i64 1, const.str "  a//b      FLOOR / INTEGER-STYLE division\n", call @z3(const.str "  a//b      FLOOR / INTEGER-STYLE division\n") : u64) : unit; call @z4(const.i64 1, const.str "  a%b       remainder (fmod)\n", call @z3(const.str "  a%b       remainder (fmod)\n") : u64) : unit; call @z4(const.i64 1, const.str "  a^b       power\n", call @z3(const.str "  a^b       power\n") : u64) : unit; call @z4(const.i64 1, const.str "  2(3+4), 2pi  implicit multiplication\n", call @z3(const.str "  2(3+4), 2pi  implicit multiplication\n") : u64) : unit; call @z4(const.i64 1, const.str "  ( ... )   grouping\n", call @z3(const.str "  ( ... )   grouping\n") : u64) : unit; call @z4(const.i64 1, const.str "  +x  -x    unary plus/minus\n", call @z3(const.str "  +x  -x    unary plus/minus\n") : u64) : unit; call @z4(const.i64 1, const.str "  x!        factorial (gamma(x+1), including non-integer extension)\n", call @z3(const.str "  x!        factorial (gamma(x+1), including non-integer extension)\n") : u64) : unit; call @z4(const.i64 1, const.str "  x%        percentage when % is at the end of a value\n\n", call @z3(const.str "  x%        percentage when % is at the end of a value\n\n") : u64) : unit; call @z4(const.i64 1, const.str "CONSTANTS\n", call @z3(const.str "CONSTANTS\n") : u64) : unit; call @z4(const.i64 1, const.str "  pi        3.141592653589793...\n", call @z3(const.str "  pi        3.141592653589793...\n") : u64) : unit; call @z4(const.i64 1, const.str "  tau       2*pi\n", call @z3(const.str "  tau       2*pi\n") : u64) : unit; call @z4(const.i64 1, const.str "  e         Euler's number\n", call @z3(const.str "  e         Euler's number\n") : u64) : unit; call @z4(const.i64 1, const.str "  phi       golden ratio\n", call @z3(const.str "  phi       golden ratio\n") : u64) : unit; call @z4(const.i64 1, const.str "  sqrt2     square root of 2\n", call @z3(const.str "  sqrt2     square root of 2\n") : u64) : unit; call @z4(const.i64 1, const.str "  ln2       natural log of 2\n", call @z3(const.str "  ln2       natural log of 2\n") : u64) : unit; call @z4(const.i64 1, const.str "  ln10      natural log of 10\n", call @z3(const.str "  ln10      natural log of 10\n") : u64) : unit; call @z4(const.i64 1, const.str "  log2e     log base 2 of e\n", call @z3(const.str "  log2e     log base 2 of e\n") : u64) : unit; call @z4(const.i64 1, const.str "  log10e    log base 10 of e\n\n", call @z3(const.str "  log10e    log base 10 of e\n\n") : u64) : unit; call @z4(const.i64 1, const.str "BASIC FUNCTIONS\n", call @z3(const.str "BASIC FUNCTIONS\n") : u64) : unit; call @z4(const.i64 1, const.str "  abs(x) fabs(x)     absolute value\n", call @z3(const.str "  abs(x) fabs(x)     absolute value\n") : u64) : unit; call @z4(const.i64 1, const.str "  sqrt(x)            square root\n", call @z3(const.str "  sqrt(x)            square root\n") : u64) : unit; call @z4(const.i64 1, const.str "  cbrt(x)            real cube root\n", call @z3(const.str "  cbrt(x)            real cube root\n") : u64) : unit; call @z4(const.i64 1, const.str "  root(x,n)          nth root\n", call @z3(const.str "  root(x,n)          nth root\n") : u64) : unit; call @z4(const.i64 1, const.str "  nthroot(x,n)       alias of root\n", call @z3(const.str "  nthroot(x,n)       alias of root\n") : u64) : unit; call @z4(const.i64 1, const.str "  square(x)          x^2\n", call @z3(const.str "  square(x)          x^2\n") : u64) : unit; call @z4(const.i64 1, const.str "  cube(x)            x^3\n", call @z3(const.str "  cube(x)            x^3\n") : u64) : unit; call @z4(const.i64 1, const.str "  sign(x)            -1, 0, or +1\n", call @z3(const.str "  sign(x)            -1, 0, or +1\n") : u64) : unit; call @z4(const.i64 1, const.str "  frac(x)            fractional part, x-trunc(x)\n", call @z3(const.str "  frac(x)            fractional part, x-trunc(x)\n") : u64) : unit; call @z4(const.i64 1, const.str "  floor(x) ceil(x) trunc(x) round(x)\n", call @z3(const.str "  floor(x) ceil(x) trunc(x) round(x)\n") : u64) : unit; call @z4(const.i64 1, const.str "  min(a,b) max(a,b) clamp(x,lo,hi)\n", call @z3(const.str "  min(a,b) max(a,b) clamp(x,lo,hi)\n") : u64) : unit; call @z4(const.i64 1, const.str "  hypot(x,y)         sqrt(x^2+y^2)\n", call @z3(const.str "  hypot(x,y)         sqrt(x^2+y^2)\n") : u64) : unit; call @z4(const.i64 1, const.str "  copysign(a,b)       magnitude of a with sign of b\n", call @z3(const.str "  copysign(a,b)       magnitude of a with sign of b\n") : u64) : unit; call @z4(const.i64 1, const.str "  fmin fmax fdim      C floating-point min/max/positive-difference\n", call @z3(const.str "  fmin fmax fdim      C floating-point min/max/positive-difference\n") : u64) : unit; call @z4(const.i64 1, const.str "  fma(a,b,c)          correctly-rounded a*b+c (where supported)\n", call @z3(const.str "  fma(a,b,c)          correctly-rounded a*b+c (where supported)\n") : u64) : unit; call @z4(const.i64 1, const.str "  nextafter(a,b)      next representable double from a toward b\n\n", call @z3(const.str "  nextafter(a,b)      next representable double from a toward b\n\n") : u64) : unit; call @z4(const.i64 1, const.str "EXPONENTIALS / LOGS\n", call @z3(const.str "EXPONENTIALS / LOGS\n") : u64) : unit; call @z4(const.i64 1, const.str "  erf(x) erfc(x)      error function / complementary error function\n", call @z3(const.str "  erf(x) erfc(x)      error function / complementary error function\n") : u64) : unit; call @z4(const.i64 1, const.str "  logb(x)             exponent of x in binary scale\n", call @z3(const.str "  logb(x)             exponent of x in binary scale\n") : u64) : unit; call @z4(const.i64 1, const.str "  exp(x)              e^x\n", call @z3(const.str "  exp(x)              e^x\n") : u64) : unit; call @z4(const.i64 1, const.str "  exp2(x)            2^x\n", call @z3(const.str "  exp2(x)            2^x\n") : u64) : unit; call @z4(const.i64 1, const.str "  expm1(x)           e^x-1, accurate near zero\n", call @z3(const.str "  expm1(x)           e^x-1, accurate near zero\n") : u64) : unit; call @z4(const.i64 1, const.str "  log(x) ln(x)       natural logarithm\n", call @z3(const.str "  log(x) ln(x)       natural logarithm\n") : u64) : unit; call @z4(const.i64 1, const.str "  log2(x)             base-2 logarithm\n", call @z3(const.str "  log2(x)             base-2 logarithm\n") : u64) : unit; call @z4(const.i64 1, const.str "  log10(x)            base-10 logarithm\n", call @z3(const.str "  log10(x)            base-10 logarithm\n") : u64) : unit; call @z4(const.i64 1, const.str "  log1p(x)            ln(1+x), accurate near zero\n", call @z3(const.str "  log1p(x)            ln(1+x), accurate near zero\n") : u64) : unit; call @z4(const.i64 1, const.str "  pow(x,y)            x^y\n\n", call @z3(const.str "  pow(x,y)            x^y\n\n") : u64) : unit; call @z4(const.i64 1, const.str "TRIGONOMETRY \xe2\x80\x94 radians\n", call @z3(const.str "TRIGONOMETRY \xe2\x80\x94 radians\n") : u64) : unit; call @z4(const.i64 1, const.str "  sin cos tan\n", call @z3(const.str "  sin cos tan\n") : u64) : unit; call @z4(const.i64 1, const.str "  cot(x)=cos(x)/sin(x)\n", call @z3(const.str "  cot(x)=cos(x)/sin(x)\n") : u64) : unit; call @z4(const.i64 1, const.str "  sec(x)=1/cos(x)\n", call @z3(const.str "  sec(x)=1/cos(x)\n") : u64) : unit; call @z4(const.i64 1, const.str "  csc(x)=1/sin(x)\n", call @z3(const.str "  csc(x)=1/sin(x)\n") : u64) : unit; call @z4(const.i64 1, const.str "  asin acos atan      inverse trig, result in radians\n", call @z3(const.str "  asin acos atan      inverse trig, result in radians\n") : u64) : unit; call @z4(const.i64 1, const.str "  atan2(y,x)          quadrant-aware angle\n", call @z3(const.str "  atan2(y,x)          quadrant-aware angle\n") : u64) : unit; call @z4(const.i64 1, const.str "  acot asec acsc      inverse cot/sec/csc\n\n", call @z3(const.str "  acot asec acsc      inverse cot/sec/csc\n\n") : u64) : unit; call @z4(const.i64 1, const.str "HYPERBOLIC\n", call @z3(const.str "HYPERBOLIC\n") : u64) : unit; call @z4(const.i64 1, const.str "  sinh cosh tanh\n", call @z3(const.str "  sinh cosh tanh\n") : u64) : unit; call @z4(const.i64 1, const.str "  csch sech coth     reciprocal hyperbolic functions\n", call @z3(const.str "  csch sech coth     reciprocal hyperbolic functions\n") : u64) : unit; call @z4(const.i64 1, const.str "  asinh acosh atanh\n\n", call @z3(const.str "  asinh acosh atanh\n\n") : u64) : unit; call @z4(const.i64 1, const.str "ANGLE CONVERSION\n", call @z3(const.str "ANGLE CONVERSION\n") : u64) : unit; call @z4(const.i64 1, const.str "  deg(x)              radians -> degrees\n", call @z3(const.str "  deg(x)              radians -> degrees\n") : u64) : unit; call @z4(const.i64 1, const.str "  degrees(x)          alias\n", call @z3(const.str "  degrees(x)          alias\n") : u64) : unit; call @z4(const.i64 1, const.str "  rad(x)              degrees -> radians\n", call @z3(const.str "  rad(x)              degrees -> radians\n") : u64) : unit; call @z4(const.i64 1, const.str "  radians(x)          alias\n", call @z3(const.str "  radians(x)          alias\n") : u64) : unit; call @z4(const.i64 1, const.str "  Example: sin(rad(30)) = 0.5\n\n", call @z3(const.str "  Example: sin(rad(30)) = 0.5\n\n") : u64) : unit; call @z4(const.i64 1, const.str "NUMBER THEORY / COMBINATORICS\n", call @z3(const.str "NUMBER THEORY / COMBINATORICS\n") : u64) : unit; call @z4(const.i64 1, const.str "  gcd(a,b)             greatest common divisor\n", call @z3(const.str "  gcd(a,b)             greatest common divisor\n") : u64) : unit; call @z4(const.i64 1, const.str "  lcm(a,b)             least common multiple\n", call @z3(const.str "  lcm(a,b)             least common multiple\n") : u64) : unit; call @z4(const.i64 1, const.str "  ncr(n,r) / comb      binomial coefficient\n", call @z3(const.str "  ncr(n,r) / comb      binomial coefficient\n") : u64) : unit; call @z4(const.i64 1, const.str "  npr(n,r) / perm      permutations\n", call @z3(const.str "  npr(n,r) / perm      permutations\n") : u64) : unit; call @z4(const.i64 1, const.str "  factorial(n) / fact  factorial\n", call @z3(const.str "  factorial(n) / fact  factorial\n") : u64) : unit; call @z4(const.i64 1, const.str "  gamma(x)             Gamma(x)\n", call @z3(const.str "  gamma(x)             Gamma(x)\n") : u64) : unit; call @z4(const.i64 1, const.str "  lgamma(x)            ln(|Gamma(x)|)\n\n", call @z3(const.str "  lgamma(x)            ln(|Gamma(x)|)\n\n") : u64) : unit; call @z4(const.i64 1, const.str "REMAINDER FUNCTIONS\n", call @z3(const.str "REMAINDER FUNCTIONS\n") : u64) : unit; call @z4(const.i64 1, const.str "  fmod(a,b) mod(a,b) rem(a,b)  floating remainder\n", call @z3(const.str "  fmod(a,b) mod(a,b) rem(a,b)  floating remainder\n") : u64) : unit; call @z4(const.i64 1, const.str "  remainder(a,b)       IEEE-style remainder\n\n", call @z3(const.str "  remainder(a,b)       IEEE-style remainder\n\n") : u64) : unit; call @z4(const.i64 1, const.str "PRECEDENCE (strongest first)\n", call @z3(const.str "PRECEDENCE (strongest first)\n") : u64) : unit; call @z4(const.i64 1, const.str "  postfix !, final %\n", call @z3(const.str "  postfix !, final %\n") : u64) : unit; call @z4(const.i64 1, const.str "  power ^             right-associative\n", call @z3(const.str "  power ^             right-associative\n") : u64) : unit; call @z4(const.i64 1, const.str "  unary + -\n", call @z3(const.str "  unary + -\n") : u64) : unit; call @z4(const.i64 1, const.str "  * / // %\n", call @z3(const.str "  * / // %\n") : u64) : unit; call @z4(const.i64 1, const.str "  + -\n", call @z3(const.str "  + -\n") : u64) : unit; call @z4(const.i64 1, const.str "  Use parentheses whenever you want explicit grouping.\n\n", call @z3(const.str "  Use parentheses whenever you want explicit grouping.\n\n") : u64) : unit; call @z4(const.i64 1, const.str "EXAMPLES\n", call @z3(const.str "EXAMPLES\n") : u64) : unit; call @z4(const.i64 1, const.str "  22/7                    3.1428571428571428\n", call @z3(const.str "  22/7                    3.1428571428571428\n") : u64) : unit; call @z4(const.i64 1, const.str "  22//7                   3\n", call @z3(const.str "  22//7                   3\n") : u64) : unit; call @z4(const.i64 1, const.str "  2^3^2                   512\n", call @z3(const.str "  2^3^2                   512\n") : u64) : unit; call @z4(const.i64 1, const.str "  sqrt(2)^2               2\n", call @z3(const.str "  sqrt(2)^2               2\n") : u64) : unit; call @z4(const.i64 1, const.str "  sin(pi/2)               1\n", call @z3(const.str "  sin(pi/2)               1\n") : u64) : unit; call @z4(const.i64 1, const.str "  log10(1000)             3\n", call @z3(const.str "  log10(1000)             3\n") : u64) : unit; call @z4(const.i64 1, const.str "  ncr(52,5)               2598960\n", call @z3(const.str "  ncr(52,5)               2598960\n") : u64) : unit; call @z4(const.i64 1, const.str "  5!                      120\n\n", call @z3(const.str "  5!                      120\n\n") : u64) : unit; call @z4(const.i64 1, const.str "IMPORTANT\n", call @z3(const.str "IMPORTANT\n") : u64) : unit; call @z4(const.i64 1, const.str "  22/7  -> 3.1428571428571428...\n", call @z3(const.str "  22/7  -> 3.1428571428571428...\n") : u64) : unit; call @z4(const.i64 1, const.str "  22//7 -> 3\n", call @z3(const.str "  22//7 -> 3\n") : u64) : unit; call @z4(const.i64 1, const.str "  / never becomes integer division.\n", call @z3(const.str "  / never becomes integer division.\n") : u64) : unit; call @z4(const.i64 1, const.str "  // is floor division, so -7//2 = -4.\n", call @z3(const.str "  // is floor division, so -7//2 = -4.\n") : u64) : unit; call @z4(const.i64 1, const.str "  ^ accepts real exponents; e.g. 9^0.5 = 3.\n", call @z3(const.str "  ^ accepts real exponents; e.g. 9^0.5 = 3.\n") : u64) : unit; call @z4(const.i64 1, const.str "  Invalid domains may display nan/inf according to libm.\n", call @z3(const.str "  Invalid domains may display nan/inf according to libm.\n") : u64) : unit; call @z4(const.i64 1, const.str "  Type exit to leave the calculator.\n\n", call @z3(const.str "  Type exit to leave the calculator.\n\n") : u64) : unit; yield call @f17() : unit }; _ => block { case call @f16(%mak_local_ii_111:!adt._a) : !adt._f : i32 { @k11 => call @z4(const.i64 1, const.str "Error: invalid expression\n", call @z3(const.str "Error: invalid expression\n") : u64) : unit; @k10(%mak_local_ig_121:f64, %mak_local_ih_122:!adt._a) => case call @f2(%mak_local_ih_122:!adt._a) : !adt._a : i32 { @k0 => call @z2(const.str "%.17g\n", %mak_local_ig_121:f64) : i32; _ => call @z4(const.i64 1, const.str "Error: unexpected characters\n", call @z3(const.str "Error: unexpected characters\n") : u64) : unit; }; }; yield call @f17() : unit }; }; }; }
      }
    }
    fn @mak_ir_fn_18 ["<lambda>"] : (%a0: i64) -> i64 pure {
      case %a0:i64 : i64 {
        9 => const.i64 1
        10 => const.i64 1
        13 => const.i64 1
        32 => const.i64 1
        _ => const.i64 0
      }
    }
    fn @mak_ir_fn_19 ["<lambda>"] : (%a0: i64) -> i64 pure {
      case case bin.eq %a0:i64 const.i64 95 : i32 : i64 { 1 => const.i64 1; _ => case bin.ge %a0:i64 const.i64 65 : i32 : i64 { 0 => case bin.ge %a0:i64 const.i64 97 : i32 : i64 { 0 => const.i64 0; _ => case bin.le %a0:i64 const.i64 122 : i32 : i64 { 1 => const.i64 1; _ => const.i64 0; }; }; _ => case bin.le %a0:i64 const.i64 90 : i32 : i64 { 1 => const.i64 1; _ => const.i64 0; }; }; } : i64 {
        1 => const.i64 1
        _ => case bin.ge %a0:i64 const.i64 48 : i32 : i64 { 0 => const.i64 0; _ => case bin.le %a0:i64 const.i64 57 : i32 : i64 { 1 => const.i64 1; _ => const.i64 0; }; }
      }
    }
    fn @mak_ir_fn_20 ["<lambda>"] : (%a0: i64) -> i64 pure {
      case bin.ge %a0:i64 const.i64 65 : i32 : i64 {
        0 => %a0:i64
        _ => case bin.le %a0:i64 const.i64 90 : i32 : i64 { 1 => bin.add %a0:i64 const.i64 32 : i64; _ => %a0:i64; }
      }
    }
    fn @mak_ir_fn_21 ["<lambda>"] : (%a0: str) -> !adt._f {
      case case call @f2(%a0:str) : !adt._a : !adt._f { @k0 => make @k11() : !adt._f; @k1(%mak_local_fs_123:i32, %mak_local_ft_124:!adt._a) => case %mak_local_fs_123:i32 : !adt._f { 40 => case call @f16(%mak_local_ft_124:!adt._a) : !adt._f : !adt._f { @k11 => make @k11() : !adt._f; @k10(%mak_local_fu_125:f64, %mak_local_fv_126:!adt._a) => case call @f2(%mak_local_fv_126:!adt._a) : !adt._a : !adt._f { @k1(41, %mak_local_fw_127:!adt._a) => make @k10(%mak_local_fu_125:f64, %mak_local_fw_127:!adt._a) : !adt._f; _ => make @k11() : !adt._f; }; }; _ => case case bin.eq %mak_local_fs_123:i32 const.i64 95 : i32 : i64 { 1 => const.i64 1; _ => case bin.ge %mak_local_fs_123:i32 const.i64 65 : i32 : i64 { 0 => case bin.ge %mak_local_fs_123:i32 const.i64 97 : i32 : i64 { 0 => const.i64 0; _ => case bin.le %mak_local_fs_123:i32 const.i64 122 : i32 : i64 { 1 => const.i64 1; _ => const.i64 0; }; }; _ => case bin.le %mak_local_fs_123:i32 const.i64 90 : i32 : i64 { 1 => const.i64 1; _ => const.i64 0; }; }; } : !adt._f { 1 => case case call @f2(make @k1(%mak_local_fs_123:i32, %mak_local_ft_124:!adt._a) : !adt._a) : !adt._a : !adt._f { @k1(%mak_local_ce_128:i32, %mak_local_cf_129:!adt._a) => case case bin.eq %mak_local_ce_128:i32 const.i64 95 : i32 : i64 { 1 => const.i64 1; _ => case bin.ge %mak_local_ce_128:i32 const.i64 65 : i32 : i64 { 0 => case bin.ge %mak_local_ce_128:i32 const.i64 97 : i32 : i64 { 0 => const.i64 0; _ => case bin.le %mak_local_ce_128:i32 const.i64 122 : i32 : i64 { 1 => const.i64 1; _ => const.i64 0; }; }; _ => case bin.le %mak_local_ce_128:i32 const.i64 90 : i32 : i64 { 1 => const.i64 1; _ => const.i64 0; }; }; } : !adt._f { 1 => call @f3(make @k1(%mak_local_ce_128:i32, %mak_local_cf_129:!adt._a) : !adt._a, const.i64 0) : !adt._c; _ => runtime-ctor NBad:_f : !adt._f; }; @k0 => runtime-ctor NBad:_f : !adt._f; } : !adt._f { @k11 => make @k11() : !adt._f; @k10(%mak_local_fx_130:f64, %mak_local_fy_131:!adt._a) => case call @f2(%mak_local_fy_131:!adt._a) : !adt._a : !adt._f { @k1(40, %mak_local_fz_132:!adt._a) => make @k11() : !adt._f; _ => make @k11() : !adt._f; }; }; _ => case case call @f2(%a0:str) : !adt._a : !adt._f { @k0 => runtime-ctor NBad:_f : !adt._f; @k1(%mak_local_dk_151:i32, %mak_local_dl_152:!adt._a) => case case bin.ge %mak_local_dk_151:i32 const.i64 48 : i32 : i64 { 0 => const.i64 0; _ => case bin.le %mak_local_dk_151:i32 const.i64 57 : i32 : i64 { 1 => const.i64 1; _ => const.i64 0; }; } : !adt._f { 1 => case call @f7(make @k1(%mak_local_dk_151:i32, %mak_local_dl_152:!adt._a) : !adt._a, const.f64 0.00000000000000000) : !adt._d : !adt._f { @k6 => runtime-ctor NBad:_f : !adt._f; @k7(%mak_local_dm_153:f64, %mak_local_dn_154:!adt._a) => case %mak_local_dn_154:!adt._a : !adt._d { @k1(46, %mak_local_dp_155:!adt._a) => call @f8(%mak_local_dp_155:!adt._a, %mak_local_dm_153:f64, const.f64 0.10000000000000001) : !adt._d; @k1(%mak_local_dq_156:i32, %mak_local_dr_157:!adt._a) => case %mak_local_dq_156:i32 : !adt._f { 101 => case call @f6(%mak_local_dr_157:!adt._a, const.i64 1) : !adt._e : !adt._f { @k8 => runtime-ctor NBad:_f : !adt._f; @k9(%mak_local_ds_158:i64, %mak_local_dt_159:!adt._a) => make @k7(bin.mul %mak_local_dm_153:f64 call @f4(%mak_local_ds_158:i64) : f64 : f64, %mak_local_dt_159:!adt._a) : !adt._d; }; 69 => case call @f6(%mak_local_dr_157:!adt._a, const.i64 1) : !adt._e : !adt._f { @k8 => runtime-ctor NBad:_f : !adt._f; @k9(%mak_local_du_160:i64, %mak_local_dv_161:!adt._a) => make @k7(bin.mul %mak_local_dm_153:f64 call @f4(%mak_local_du_160:i64) : f64 : f64, %mak_local_dv_161:!adt._a) : !adt._d; }; _ => make @k7(%mak_local_dm_153:f64, %mak_local_dn_154:!adt._a) : !adt._d; }; _ => make @k7(%mak_local_dm_153:f64, %mak_local_dn_154:!adt._a) : !adt._d; }; }; _ => case %mak_local_dk_151:i32 : !adt._f { 46 => case %mak_local_dl_152:!adt._a : !adt._f { @k1(%mak_local_dw_162:i32, _) => case case bin.ge %mak_local_dw_162:i32 const.i64 48 : i32 : i64 { 0 => const.i64 0; _ => case bin.le %mak_local_dw_162:i32 const.i64 57 : i32 : i64 { 1 => const.i64 1; _ => const.i64 0; }; } : !adt._f { 1 => call @f8(%mak_local_dl_152:!adt._a, const.f64 0.00000000000000000, const.f64 0.10000000000000001) : !adt._d; _ => runtime-ctor NBad:_f : !adt._f; }; _ => runtime-ctor NBad:_f : !adt._f; }; _ => runtime-ctor NBad:_f : !adt._f; }; }; } : !adt._f { @k11 => make @k11() : !adt._f; @k10(%mak_local_ga_163:f64, %mak_local_gb_164:!adt._a) => make @k10(%mak_local_ga_163:f64, %mak_local_gb_164:!adt._a) : !adt._f; }; }; }; } : !adt._f {
        @k11 => make @k11() : !adt._f
        @k10(%mak_local_gi_165:f64, %mak_local_gj_166:!adt._a) => call @f10(%mak_local_gi_165:f64, %mak_local_gj_166:!adt._a) : !adt._f
      }
    }
    fn @mak_ir_fn_22 ["<lambda>"] : (%a0: !adt._a) -> !adt._f {
      case call @f2(%a0:!adt._a) : !adt._a : !adt._f {
        @k0 => make @k11() : !adt._f
        @k1(%mak_local_fs_167:i32, %mak_local_ft_168:!adt._a) => case %mak_local_fs_167:i32 : !adt._f { 40 => case call @f16(%mak_local_ft_168:!adt._a) : !adt._f : !adt._f { @k11 => make @k11() : !adt._f; @k10(%mak_local_fu_169:f64, %mak_local_fv_170:!adt._a) => case call @f2(%mak_local_fv_170:!adt._a) : !adt._a : !adt._f { @k1(41, %mak_local_fw_171:!adt._a) => make @k10(%mak_local_fu_169:f64, %mak_local_fw_171:!adt._a) : !adt._f; _ => make @k11() : !adt._f; }; }; _ => case case bin.eq %mak_local_fs_167:i32 const.i64 95 : i32 : i64 { 1 => const.i64 1; _ => case bin.ge %mak_local_fs_167:i32 const.i64 65 : i32 : i64 { 0 => case bin.ge %mak_local_fs_167:i32 const.i64 97 : i32 : i64 { 0 => const.i64 0; _ => case bin.le %mak_local_fs_167:i32 const.i64 122 : i32 : i64 { 1 => const.i64 1; _ => const.i64 0; }; }; _ => case bin.le %mak_local_fs_167:i32 const.i64 90 : i32 : i64 { 1 => const.i64 1; _ => const.i64 0; }; }; } : !adt._f { 1 => case case call @f2(make @k1(%mak_local_fs_167:i32, %mak_local_ft_168:!adt._a) : !adt._a) : !adt._a : !adt._f { @k1(%mak_local_ce_172:i32, %mak_local_cf_173:!adt._a) => case case bin.eq %mak_local_ce_172:i32 const.i64 95 : i32 : i64 { 1 => const.i64 1; _ => case bin.ge %mak_local_ce_172:i32 const.i64 65 : i32 : i64 { 0 => case bin.ge %mak_local_ce_172:i32 const.i64 97 : i32 : i64 { 0 => const.i64 0; _ => case bin.le %mak_local_ce_172:i32 const.i64 122 : i32 : i64 { 1 => const.i64 1; _ => const.i64 0; }; }; _ => case bin.le %mak_local_ce_172:i32 const.i64 90 : i32 : i64 { 1 => const.i64 1; _ => const.i64 0; }; }; } : !adt._f { 1 => call @f3(make @k1(%mak_local_ce_172:i32, %mak_local_cf_173:!adt._a) : !adt._a, const.i64 0) : !adt._c; _ => runtime-ctor NBad:_f : !adt._f; }; @k0 => runtime-ctor NBad:_f : !adt._f; } : !adt._f { @k11 => make @k11() : !adt._f; @k10(%mak_local_fx_174:f64, %mak_local_fy_175:!adt._a) => case call @f2(%mak_local_fy_175:!adt._a) : !adt._a : !adt._f { @k1(40, %mak_local_fz_176:!adt._a) => make @k11() : !adt._f; _ => make @k11() : !adt._f; }; }; _ => case case call @f2(%a0:!adt._a) : !adt._a : !adt._f { @k0 => runtime-ctor NBad:_f : !adt._f; @k1(%mak_local_dk_195:i32, %mak_local_dl_196:!adt._a) => case case bin.ge %mak_local_dk_195:i32 const.i64 48 : i32 : i64 { 0 => const.i64 0; _ => case bin.le %mak_local_dk_195:i32 const.i64 57 : i32 : i64 { 1 => const.i64 1; _ => const.i64 0; }; } : !adt._f { 1 => case call @f7(make @k1(%mak_local_dk_195:i32, %mak_local_dl_196:!adt._a) : !adt._a, const.f64 0.00000000000000000) : !adt._d : !adt._f { @k6 => runtime-ctor NBad:_f : !adt._f; @k7(%mak_local_dm_197:f64, %mak_local_dn_198:!adt._a) => case %mak_local_dn_198:!adt._a : !adt._d { @k1(46, %mak_local_dp_199:!adt._a) => call @f8(%mak_local_dp_199:!adt._a, %mak_local_dm_197:f64, const.f64 0.10000000000000001) : !adt._d; @k1(%mak_local_dq_200:i32, %mak_local_dr_201:!adt._a) => case %mak_local_dq_200:i32 : !adt._f { 101 => case call @f6(%mak_local_dr_201:!adt._a, const.i64 1) : !adt._e : !adt._f { @k8 => runtime-ctor NBad:_f : !adt._f; @k9(%mak_local_ds_202:i64, %mak_local_dt_203:!adt._a) => make @k7(bin.mul %mak_local_dm_197:f64 call @f4(%mak_local_ds_202:i64) : f64 : f64, %mak_local_dt_203:!adt._a) : !adt._d; }; 69 => case call @f6(%mak_local_dr_201:!adt._a, const.i64 1) : !adt._e : !adt._f { @k8 => runtime-ctor NBad:_f : !adt._f; @k9(%mak_local_du_204:i64, %mak_local_dv_205:!adt._a) => make @k7(bin.mul %mak_local_dm_197:f64 call @f4(%mak_local_du_204:i64) : f64 : f64, %mak_local_dv_205:!adt._a) : !adt._d; }; _ => make @k7(%mak_local_dm_197:f64, %mak_local_dn_198:!adt._a) : !adt._d; }; _ => make @k7(%mak_local_dm_197:f64, %mak_local_dn_198:!adt._a) : !adt._d; }; }; _ => case %mak_local_dk_195:i32 : !adt._f { 46 => case %mak_local_dl_196:!adt._a : !adt._f { @k1(%mak_local_dw_206:i32, _) => case case bin.ge %mak_local_dw_206:i32 const.i64 48 : i32 : i64 { 0 => const.i64 0; _ => case bin.le %mak_local_dw_206:i32 const.i64 57 : i32 : i64 { 1 => const.i64 1; _ => const.i64 0; }; } : !adt._f { 1 => call @f8(%mak_local_dl_196:!adt._a, const.f64 0.00000000000000000, const.f64 0.10000000000000001) : !adt._d; _ => runtime-ctor NBad:_f : !adt._f; }; _ => runtime-ctor NBad:_f : !adt._f; }; _ => runtime-ctor NBad:_f : !adt._f; }; }; } : !adt._f { @k11 => make @k11() : !adt._f; @k10(%mak_local_ga_207:f64, %mak_local_gb_208:!adt._a) => make @k10(%mak_local_ga_207:f64, %mak_local_gb_208:!adt._a) : !adt._f; }; }; }
      }
    }
    fn @mak_ir_fn_23 ["<lambda>"] : (%a0: !adt._a) -> !adt._c {
      case call @f2(%a0:!adt._a) : !adt._a : !adt._c {
        @k1(%mak_local_ce_209:i32, %mak_local_cf_210:!adt._a) => case case bin.eq %mak_local_ce_209:i32 const.i64 95 : i32 : i64 { 1 => const.i64 1; _ => case bin.ge %mak_local_ce_209:i32 const.i64 65 : i32 : i64 { 0 => case bin.ge %mak_local_ce_209:i32 const.i64 97 : i32 : i64 { 0 => const.i64 0; _ => case bin.le %mak_local_ce_209:i32 const.i64 122 : i32 : i64 { 1 => const.i64 1; _ => const.i64 0; }; }; _ => case bin.le %mak_local_ce_209:i32 const.i64 90 : i32 : i64 { 1 => const.i64 1; _ => const.i64 0; }; }; } : !adt._c { 1 => call @f3(make @k1(%mak_local_ce_209:i32, %mak_local_cf_210:!adt._a) : !adt._a, const.i64 0) : !adt._c; _ => make @k4() : !adt._c; }
        @k0 => make @k4() : !adt._c
      }
    }
    fn @mak_ir_fn_24 ["<lambda>"] : (%a0: i64, %a1: i64) -> !adt._f {
      case case %a0:i64 : i64 { 399964812 => const.i64 1; 1764798 => const.i64 1; _ => const.i64 0; } : !adt._f {
        1 => case call @f16(%a1:i64) : !adt._f : !adt._f { @k11 => make @k11() : !adt._f; @k10(%mak_local_ff_211:f64, %mak_local_fg_212:!adt._a) => case call @f2(%mak_local_fg_212:!adt._a) : !adt._a : !adt._f { @k1(44, %mak_local_fh_213:!adt._a) => case call @f16(%mak_local_fh_213:!adt._a) : !adt._f : !adt._f { @k11 => make @k11() : !adt._f; @k10(%mak_local_fi_214:f64, %mak_local_fj_215:!adt._a) => case call @f2(%mak_local_fj_215:!adt._a) : !adt._a : !adt._f { @k1(44, %mak_local_fk_216:!adt._a) => case call @f16(%mak_local_fk_216:!adt._a) : !adt._f : !adt._f { @k11 => make @k11() : !adt._f; @k10(%mak_local_fl_217:f64, %mak_local_fm_218:!adt._a) => case call @f2(%mak_local_fm_218:!adt._a) : !adt._a : !adt._f { @k1(41, %mak_local_fo_219:!adt._a) => case case %a0:i64 : i64 { 399964812 => const.i64 1; 1764798 => const.i64 1; _ => const.i64 0; } : !adt._f { 1 => make @k10(case %a0:i64 : f64 { 399964812 => case bin.lt %mak_local_ff_211:f64 %mak_local_fi_214:f64 : i32 : f64 { 1 => %mak_local_fi_214:f64; _ => case bin.gt %mak_local_ff_211:f64 %mak_local_fl_217:f64 : i32 : f64 { 1 => %mak_local_fl_217:f64; _ => %mak_local_ff_211:f64; }; }; 1764798 => call @z45(%mak_local_ff_211:f64, %mak_local_fi_214:f64, %mak_local_fl_217:f64) : f64 [pure]; _ => %mak_local_ff_211:f64; }, %mak_local_fo_219:!adt._a) : !adt._f; _ => make @k11() : !adt._f; }; _ => make @k11() : !adt._f; }; }; _ => make @k11() : !adt._f; }; }; _ => make @k11() : !adt._f; }; }
        _ => case case %a0:i64 : i64 { 828949774 => const.i64 1; 231190472 => const.i64 1; 1885190 => const.i64 1; 1969694 => const.i64 1; 510764621 => const.i64 1; 1936692 => const.i64 1; 258201902 => const.i64 1; 548941060 => const.i64 1; 901947274 => const.i64 1; 1780652 => const.i64 1; 1866466 => const.i64 1; 1900793 => const.i64 1; 224480257 => const.i64 1; 1902496 => const.i64 1; 253534496 => const.i64 1; 1884414 => const.i64 1; 1883376 => const.i64 1; 262106026 => const.i64 1; 231035246 => const.i64 1; 231188658 => const.i64 1; 231189696 => const.i64 1; 743972013 => const.i64 1; _ => const.i64 0; } : !adt._f { 1 => case call @f16(%a1:i64) : !adt._f : !adt._f { @k11 => make @k11() : !adt._f; @k10(%mak_local_ex_220:f64, %mak_local_ey_221:!adt._a) => case call @f2(%mak_local_ey_221:!adt._a) : !adt._a : !adt._f { @k1(44, %mak_local_ez_222:!adt._a) => case call @f16(%mak_local_ez_222:!adt._a) : !adt._f : !adt._f { @k11 => make @k11() : !adt._f; @k10(%mak_local_fa_223:f64, %mak_local_fb_224:!adt._a) => case call @f2(%mak_local_fb_224:!adt._a) : !adt._a : !adt._f { @k1(41, %mak_local_fc_225:!adt._a) => case case %a0:i64 : i64 { 828949774 => const.i64 1; 231190472 => const.i64 1; 1885190 => const.i64 1; 1969694 => const.i64 1; 510764621 => const.i64 1; 1936692 => const.i64 1; 258201902 => const.i64 1; 548941060 => const.i64 1; 901947274 => const.i64 1; 1780652 => const.i64 1; 1866466 => const.i64 1; 1900793 => const.i64 1; 224480257 => const.i64 1; 1902496 => const.i64 1; 253534496 => const.i64 1; 1884414 => const.i64 1; 1883376 => const.i64 1; 262106026 => const.i64 1; 231035246 => const.i64 1; 231188658 => const.i64 1; 231189696 => const.i64 1; 743972013 => const.i64 1; _ => const.i64 0; } : !adt._f { 1 => make @k10(call @f9(%a0:i64, %mak_local_ex_220:f64, %mak_local_fa_223:f64) : f64, %mak_local_fc_225:!adt._a) : !adt._f; _ => make @k11() : !adt._f; }; _ => make @k11() : !adt._f; }; }; _ => make @k11() : !adt._f; }; }; _ => case case %a0:i64 : i64 { 1677570 => const.i64 1; 230982852 => const.i64 1; 260484708 => const.i64 1; 224257837 => const.i64 1; 1749093 => const.i64 1; 229131233 => const.i64 1; 16199091 => const.i64 1; 1868032 => const.i64 1; 14258 => const.i64 1; 244712242 => const.i64 1; 57303395 => const.i64 1; 57303459 => const.i64 1; 1987380 => const.i64 1; 1713595 => const.i64 1; 2003493 => const.i64 1; 220052207 => const.i64 1; 219778422 => const.i64 1; 220068320 => const.i64 1; 260346884 => const.i64 1; 224481049 => const.i64 1; 262457687 => const.i64 1; 224547597 => const.i64 1; 260276799 => const.i64 1; 224481180 => const.i64 1; 826839025 => const.i64 1; 790973190 => const.i64 1; 828949828 => const.i64 1; 1713596 => const.i64 1; 1986845 => const.i64 1; 1714103 => const.i64 1; 219778423 => const.i64 1; 220051672 => const.i64 1; 219778930 => const.i64 1; 283705086 => const.i64 1; 224308133 => const.i64 1; 420295318 => const.i64 1; 824551211 => const.i64 1; 354247632 => const.i64 1; 230982984 => const.i64 1; 553441405 => const.i64 1; 138294544 => const.i64 1; 1748297 => const.i64 1; 229027006 => const.i64 1; 244712290 => const.i64 1; 260345973 => const.i64 1; 231274442 => const.i64 1; 1729434 => const.i64 1; 1969161 => const.i64 1; 992605559 => const.i64 1; 430838469 => const.i64 1; 184475947 => const.i64 1; 224581785 => const.i64 1; _ => const.i64 0; } : !adt._f { 1 => case call @f16(%a1:i64) : !adt._f : !adt._f { @k11 => make @k11() : !adt._f; @k10(%mak_local_es_226:f64, %mak_local_et_227:!adt._a) => case call @f2(%mak_local_et_227:!adt._a) : !adt._a : !adt._f { @k1(41, %mak_local_eu_228:!adt._a) => case case %a0:i64 : i64 { 1677570 => const.i64 1; 230982852 => const.i64 1; 260484708 => const.i64 1; 224257837 => const.i64 1; 1749093 => const.i64 1; 229131233 => const.i64 1; 16199091 => const.i64 1; 1868032 => const.i64 1; 14258 => const.i64 1; 244712242 => const.i64 1; 57303395 => const.i64 1; 57303459 => const.i64 1; 1987380 => const.i64 1; 1713595 => const.i64 1; 2003493 => const.i64 1; 220052207 => const.i64 1; 219778422 => const.i64 1; 220068320 => const.i64 1; 260346884 => const.i64 1; 224481049 => const.i64 1; 262457687 => const.i64 1; 224547597 => const.i64 1; 260276799 => const.i64 1; 224481180 => const.i64 1; 826839025 => const.i64 1; 790973190 => const.i64 1; 828949828 => const.i64 1; 1713596 => const.i64 1; 1986845 => const.i64 1; 1714103 => const.i64 1; 219778423 => const.i64 1; 220051672 => const.i64 1; 219778930 => const.i64 1; 283705086 => const.i64 1; 224308133 => const.i64 1; 420295318 => const.i64 1; 824551211 => const.i64 1; 354247632 => const.i64 1; 230982984 => const.i64 1; 553441405 => const.i64 1; 138294544 => const.i64 1; 1748297 => const.i64 1; 229027006 => const.i64 1; 244712290 => const.i64 1; 260345973 => const.i64 1; 231274442 => const.i64 1; 1729434 => const.i64 1; 1969161 => const.i64 1; 992605559 => const.i64 1; 430838469 => const.i64 1; 184475947 => const.i64 1; 224581785 => const.i64 1; _ => const.i64 0; } : !adt._f { 1 => make @k10(case %a0:i64 : f64 { 1677570 => call @z7(%mak_local_es_226:f64) : f64 [pure]; 230982852 => call @z7(%mak_local_es_226:f64) : f64 [pure]; 260484708 => call @z5(%mak_local_es_226:f64) : f64 [pure]; 224257837 => call @z6(%mak_local_es_226:f64) : f64 [pure]; 1749093 => call @z15(%mak_local_es_226:f64) : f64 [pure]; 229131233 => call @z16(%mak_local_es_226:f64) : f64 [pure]; 16199091 => call @z17(%mak_local_es_226:f64) : f64 [pure]; 1868032 => call @z18(%mak_local_es_226:f64) : f64 [pure]; 14258 => call @z18(%mak_local_es_226:f64) : f64 [pure]; 244712242 => call @z19(%mak_local_es_226:f64) : f64 [pure]; 57303395 => call @z20(%mak_local_es_226:f64) : f64 [pure]; 57303459 => call @z21(%mak_local_es_226:f64) : f64 [pure]; 1987380 => call @z22(%mak_local_es_226:f64) : f64 [pure]; 1713595 => call @z23(%mak_local_es_226:f64) : f64 [pure]; 2003493 => call @z24(%mak_local_es_226:f64) : f64 [pure]; 220052207 => call @z25(%mak_local_es_226:f64) : f64 [pure]; 219778422 => call @z26(%mak_local_es_226:f64) : f64 [pure]; 220068320 => call @z27(%mak_local_es_226:f64) : f64 [pure]; 260346884 => call @z29(%mak_local_es_226:f64) : f64 [pure]; 224481049 => call @z30(%mak_local_es_226:f64) : f64 [pure]; 262457687 => call @z31(%mak_local_es_226:f64) : f64 [pure]; 224547597 => bin.div const.f64 1.00000000000000000 call @z29(%mak_local_es_226:f64) : f64 [pure] : f64; 260276799 => bin.div const.f64 1.00000000000000000 call @z30(%mak_local_es_226:f64) : f64 [pure] : f64; 224481180 => bin.div call @z30(%mak_local_es_226:f64) : f64 [pure] call @z29(%mak_local_es_226:f64) : f64 [pure] : f64; 826839025 => call @z32(%mak_local_es_226:f64) : f64 [pure]; 790973190 => call @z33(%mak_local_es_226:f64) : f64 [pure]; 828949828 => call @z34(%mak_local_es_226:f64) : f64 [pure]; 1713596 => bin.div call @z23(%mak_local_es_226:f64) : f64 [pure] call @z22(%mak_local_es_226:f64) : f64 [pure] : f64; 1986845 => bin.div const.f64 1.00000000000000000 call @z23(%mak_local_es_226:f64) : f64 [pure] : f64; 1714103 => bin.div const.f64 1.00000000000000000 call @z22(%mak_local_es_226:f64) : f64 [pure] : f64; 219778423 => bin.sub const.f64 1.57079632679489656 call @z27(%mak_local_es_226:f64) : f64 [pure] : f64; 220051672 => call @z26(bin.div const.f64 1.00000000000000000 %mak_local_es_226:f64 : f64) : f64 [pure]; 219778930 => call @z25(bin.div const.f64 1.00000000000000000 %mak_local_es_226:f64 : f64) : f64 [pure]; 283705086 => call @z8(%mak_local_es_226:f64) : f64 [pure]; 224308133 => call @z9(%mak_local_es_226:f64) : f64 [pure]; 420295318 => call @z10(%mak_local_es_226:f64) : f64 [pure]; 824551211 => call @z11(%mak_local_es_226:f64) : f64 [pure]; 354247632 => call @z35(bin.add %mak_local_es_226:f64 const.f64 1.00000000000000000 : f64) : f64 [pure]; 230982984 => call @z35(bin.add %mak_local_es_226:f64 const.f64 1.00000000000000000 : f64) : f64 [pure]; 553441405 => call @z35(%mak_local_es_226:f64) : f64 [pure]; 138294544 => call @z36(%mak_local_es_226:f64) : f64 [pure]; 1748297 => call @z38(%mak_local_es_226:f64) : f64 [pure]; 229027006 => call @z39(%mak_local_es_226:f64) : f64 [pure]; 244712290 => call @z40(%mak_local_es_226:f64) : f64 [pure]; 260345973 => case bin.gt %mak_local_es_226:f64 const.f64 0.00000000000000000 : i32 : f64 { 1 => const.f64 1.00000000000000000; _ => case bin.lt %mak_local_es_226:f64 const.f64 0.00000000000000000 : i32 : f64 { 1 => const.f64 -1.00000000000000000; _ => const.f64 0.00000000000000000; }; }; 231274442 => bin.sub %mak_local_es_226:f64 call @z10(%mak_local_es_226:f64) : f64 [pure] : f64; 1729434 => bin.mul %mak_local_es_226:f64 const.f64 57.29577951308232286 : f64; 1969161 => bin.mul %mak_local_es_226:f64 const.f64 0.01745329251994330 : f64; 992605559 => bin.mul %mak_local_es_226:f64 const.f64 0.01745329251994330 : f64; 430838469 => bin.mul %mak_local_es_226:f64 const.f64 57.29577951308232286 : f64; 184475947 => bin.mul %mak_local_es_226:f64 %mak_local_es_226:f64 : f64; 224581785 => bin.mul bin.mul %mak_local_es_226:f64 %mak_local_es_226:f64 : f64 %mak_local_es_226:f64 : f64; _ => const.f64 0.00000000000000000; }, %mak_local_eu_228:!adt._a) : !adt._f; _ => make @k11() : !adt._f; }; _ => make @k11() : !adt._f; }; }; _ => make @k11() : !adt._f; }; }
      }
    }
    fn @mak_ir_fn_25 ["<lambda>"] : (%a0: i64, %a1: !adt._a) -> !adt._f {
      case call @f16(%a1:!adt._a) : !adt._f : !adt._f {
        @k11 => make @k11() : !adt._f
        @k10(%mak_local_ff_229:f64, %mak_local_fg_230:!adt._a) => case call @f2(%mak_local_fg_230:!adt._a) : !adt._a : !adt._f { @k1(44, %mak_local_fh_231:!adt._a) => case call @f16(%mak_local_fh_231:!adt._a) : !adt._f : !adt._f { @k11 => make @k11() : !adt._f; @k10(%mak_local_fi_232:f64, %mak_local_fj_233:!adt._a) => case call @f2(%mak_local_fj_233:!adt._a) : !adt._a : !adt._f { @k1(44, %mak_local_fk_234:!adt._a) => case call @f16(%mak_local_fk_234:!adt._a) : !adt._f : !adt._f { @k11 => make @k11() : !adt._f; @k10(%mak_local_fl_235:f64, %mak_local_fm_236:!adt._a) => case call @f2(%mak_local_fm_236:!adt._a) : !adt._a : !adt._f { @k1(41, %mak_local_fo_237:!adt._a) => case case %a0:i64 : i64 { 399964812 => const.i64 1; 1764798 => const.i64 1; _ => const.i64 0; } : !adt._f { 1 => make @k10(case %a0:i64 : f64 { 399964812 => case bin.lt %mak_local_ff_229:f64 %mak_local_fi_232:f64 : i32 : f64 { 1 => %mak_local_fi_232:f64; _ => case bin.gt %mak_local_ff_229:f64 %mak_local_fl_235:f64 : i32 : f64 { 1 => %mak_local_fl_235:f64; _ => %mak_local_ff_229:f64; }; }; 1764798 => call @z45(%mak_local_ff_229:f64, %mak_local_fi_232:f64, %mak_local_fl_235:f64) : f64 [pure]; _ => %mak_local_ff_229:f64; }, %mak_local_fo_237:!adt._a) : !adt._f; _ => make @k11() : !adt._f; }; _ => make @k11() : !adt._f; }; }; _ => make @k11() : !adt._f; }; }; _ => make @k11() : !adt._f; }
      }
    }
    fn @mak_ir_fn_26 ["<lambda>"] : (%a0: i64, %a1: i64, %a2: i64, %a3: i64) -> f64 {
      case %a0:i64 : f64 {
        399964812 => case bin.lt %a1:i64 %a2:i64 : i32 : i64 { 1 => %a2:i64; _ => case bin.gt %a1:i64 %a3:i64 : i32 : i64 { 1 => %a3:i64; _ => %a1:i64; }; }
        1764798 => call @z45(%a1:i64, %a2:i64, %a3:i64) : f64 [pure]
        _ => %a1:i64
      }
    }
    fn @mak_ir_fn_27 ["<lambda>"] : (%a0: i64, %a1: !adt._a) -> !adt._f {
      case call @f16(%a1:!adt._a) : !adt._f : !adt._f {
        @k11 => make @k11() : !adt._f
        @k10(%mak_local_ex_238:f64, %mak_local_ey_239:!adt._a) => case call @f2(%mak_local_ey_239:!adt._a) : !adt._a : !adt._f { @k1(44, %mak_local_ez_240:!adt._a) => case call @f16(%mak_local_ez_240:!adt._a) : !adt._f : !adt._f { @k11 => make @k11() : !adt._f; @k10(%mak_local_fa_241:f64, %mak_local_fb_242:!adt._a) => case call @f2(%mak_local_fb_242:!adt._a) : !adt._a : !adt._f { @k1(41, %mak_local_fc_243:!adt._a) => case case %a0:i64 : i64 { 828949774 => const.i64 1; 231190472 => const.i64 1; 1885190 => const.i64 1; 1969694 => const.i64 1; 510764621 => const.i64 1; 1936692 => const.i64 1; 258201902 => const.i64 1; 548941060 => const.i64 1; 901947274 => const.i64 1; 1780652 => const.i64 1; 1866466 => const.i64 1; 1900793 => const.i64 1; 224480257 => const.i64 1; 1902496 => const.i64 1; 253534496 => const.i64 1; 1884414 => const.i64 1; 1883376 => const.i64 1; 262106026 => const.i64 1; 231035246 => const.i64 1; 231188658 => const.i64 1; 231189696 => const.i64 1; 743972013 => const.i64 1; _ => const.i64 0; } : !adt._f { 1 => make @k10(call @f9(%a0:i64, %mak_local_ex_238:f64, %mak_local_fa_241:f64) : f64, %mak_local_fc_243:!adt._a) : !adt._f; _ => make @k11() : !adt._f; }; _ => make @k11() : !adt._f; }; }; _ => make @k11() : !adt._f; }
      }
    }
    fn @mak_ir_fn_28 ["<lambda>"] : (%a0: i64, %a1: !adt._a) -> !adt._f {
      case call @f16(%a1:!adt._a) : !adt._f : !adt._f {
        @k11 => make @k11() : !adt._f
        @k10(%mak_local_es_244:f64, %mak_local_et_245:!adt._a) => case call @f2(%mak_local_et_245:!adt._a) : !adt._a : !adt._f { @k1(41, %mak_local_eu_246:!adt._a) => case case %a0:i64 : i64 { 1677570 => const.i64 1; 230982852 => const.i64 1; 260484708 => const.i64 1; 224257837 => const.i64 1; 1749093 => const.i64 1; 229131233 => const.i64 1; 16199091 => const.i64 1; 1868032 => const.i64 1; 14258 => const.i64 1; 244712242 => const.i64 1; 57303395 => const.i64 1; 57303459 => const.i64 1; 1987380 => const.i64 1; 1713595 => const.i64 1; 2003493 => const.i64 1; 220052207 => const.i64 1; 219778422 => const.i64 1; 220068320 => const.i64 1; 260346884 => const.i64 1; 224481049 => const.i64 1; 262457687 => const.i64 1; 224547597 => const.i64 1; 260276799 => const.i64 1; 224481180 => const.i64 1; 826839025 => const.i64 1; 790973190 => const.i64 1; 828949828 => const.i64 1; 1713596 => const.i64 1; 1986845 => const.i64 1; 1714103 => const.i64 1; 219778423 => const.i64 1; 220051672 => const.i64 1; 219778930 => const.i64 1; 283705086 => const.i64 1; 224308133 => const.i64 1; 420295318 => const.i64 1; 824551211 => const.i64 1; 354247632 => const.i64 1; 230982984 => const.i64 1; 553441405 => const.i64 1; 138294544 => const.i64 1; 1748297 => const.i64 1; 229027006 => const.i64 1; 244712290 => const.i64 1; 260345973 => const.i64 1; 231274442 => const.i64 1; 1729434 => const.i64 1; 1969161 => const.i64 1; 992605559 => const.i64 1; 430838469 => const.i64 1; 184475947 => const.i64 1; 224581785 => const.i64 1; _ => const.i64 0; } : !adt._f { 1 => make @k10(case %a0:i64 : f64 { 1677570 => call @z7(%mak_local_es_244:f64) : f64 [pure]; 230982852 => call @z7(%mak_local_es_244:f64) : f64 [pure]; 260484708 => call @z5(%mak_local_es_244:f64) : f64 [pure]; 224257837 => call @z6(%mak_local_es_244:f64) : f64 [pure]; 1749093 => call @z15(%mak_local_es_244:f64) : f64 [pure]; 229131233 => call @z16(%mak_local_es_244:f64) : f64 [pure]; 16199091 => call @z17(%mak_local_es_244:f64) : f64 [pure]; 1868032 => call @z18(%mak_local_es_244:f64) : f64 [pure]; 14258 => call @z18(%mak_local_es_244:f64) : f64 [pure]; 244712242 => call @z19(%mak_local_es_244:f64) : f64 [pure]; 57303395 => call @z20(%mak_local_es_244:f64) : f64 [pure]; 57303459 => call @z21(%mak_local_es_244:f64) : f64 [pure]; 1987380 => call @z22(%mak_local_es_244:f64) : f64 [pure]; 1713595 => call @z23(%mak_local_es_244:f64) : f64 [pure]; 2003493 => call @z24(%mak_local_es_244:f64) : f64 [pure]; 220052207 => call @z25(%mak_local_es_244:f64) : f64 [pure]; 219778422 => call @z26(%mak_local_es_244:f64) : f64 [pure]; 220068320 => call @z27(%mak_local_es_244:f64) : f64 [pure]; 260346884 => call @z29(%mak_local_es_244:f64) : f64 [pure]; 224481049 => call @z30(%mak_local_es_244:f64) : f64 [pure]; 262457687 => call @z31(%mak_local_es_244:f64) : f64 [pure]; 224547597 => bin.div const.f64 1.00000000000000000 call @z29(%mak_local_es_244:f64) : f64 [pure] : f64; 260276799 => bin.div const.f64 1.00000000000000000 call @z30(%mak_local_es_244:f64) : f64 [pure] : f64; 224481180 => bin.div call @z30(%mak_local_es_244:f64) : f64 [pure] call @z29(%mak_local_es_244:f64) : f64 [pure] : f64; 826839025 => call @z32(%mak_local_es_244:f64) : f64 [pure]; 790973190 => call @z33(%mak_local_es_244:f64) : f64 [pure]; 828949828 => call @z34(%mak_local_es_244:f64) : f64 [pure]; 1713596 => bin.div call @z23(%mak_local_es_244:f64) : f64 [pure] call @z22(%mak_local_es_244:f64) : f64 [pure] : f64; 1986845 => bin.div const.f64 1.00000000000000000 call @z23(%mak_local_es_244:f64) : f64 [pure] : f64; 1714103 => bin.div const.f64 1.00000000000000000 call @z22(%mak_local_es_244:f64) : f64 [pure] : f64; 219778423 => bin.sub const.f64 1.57079632679489656 call @z27(%mak_local_es_244:f64) : f64 [pure] : f64; 220051672 => call @z26(bin.div const.f64 1.00000000000000000 %mak_local_es_244:f64 : f64) : f64 [pure]; 219778930 => call @z25(bin.div const.f64 1.00000000000000000 %mak_local_es_244:f64 : f64) : f64 [pure]; 283705086 => call @z8(%mak_local_es_244:f64) : f64 [pure]; 224308133 => call @z9(%mak_local_es_244:f64) : f64 [pure]; 420295318 => call @z10(%mak_local_es_244:f64) : f64 [pure]; 824551211 => call @z11(%mak_local_es_244:f64) : f64 [pure]; 354247632 => call @z35(bin.add %mak_local_es_244:f64 const.f64 1.00000000000000000 : f64) : f64 [pure]; 230982984 => call @z35(bin.add %mak_local_es_244:f64 const.f64 1.00000000000000000 : f64) : f64 [pure]; 553441405 => call @z35(%mak_local_es_244:f64) : f64 [pure]; 138294544 => call @z36(%mak_local_es_244:f64) : f64 [pure]; 1748297 => call @z38(%mak_local_es_244:f64) : f64 [pure]; 229027006 => call @z39(%mak_local_es_244:f64) : f64 [pure]; 244712290 => call @z40(%mak_local_es_244:f64) : f64 [pure]; 260345973 => case bin.gt %mak_local_es_244:f64 const.f64 0.00000000000000000 : i32 : f64 { 1 => const.f64 1.00000000000000000; _ => case bin.lt %mak_local_es_244:f64 const.f64 0.00000000000000000 : i32 : f64 { 1 => const.f64 -1.00000000000000000; _ => const.f64 0.00000000000000000; }; }; 231274442 => bin.sub %mak_local_es_244:f64 call @z10(%mak_local_es_244:f64) : f64 [pure] : f64; 1729434 => bin.mul %mak_local_es_244:f64 const.f64 57.29577951308232286 : f64; 1969161 => bin.mul %mak_local_es_244:f64 const.f64 0.01745329251994330 : f64; 992605559 => bin.mul %mak_local_es_244:f64 const.f64 0.01745329251994330 : f64; 430838469 => bin.mul %mak_local_es_244:f64 const.f64 57.29577951308232286 : f64; 184475947 => bin.mul %mak_local_es_244:f64 %mak_local_es_244:f64 : f64; 224581785 => bin.mul bin.mul %mak_local_es_244:f64 %mak_local_es_244:f64 : f64 %mak_local_es_244:f64 : f64; _ => const.f64 0.00000000000000000; }, %mak_local_eu_246:!adt._a) : !adt._f; _ => make @k11() : !adt._f; }; _ => make @k11() : !adt._f; }
      }
    }
    fn @mak_ir_fn_29 ["<lambda>"] : (%a0: i64, %a1: i64) -> f64 pure {
      case %a0:i64 : f64 {
        1677570 => call @z7(%a1:i64) : f64 [pure]
        230982852 => call @z7(%a1:i64) : f64 [pure]
        260484708 => call @z5(%a1:i64) : f64 [pure]
        224257837 => call @z6(%a1:i64) : f64 [pure]
        1749093 => call @z15(%a1:i64) : f64 [pure]
        229131233 => call @z16(%a1:i64) : f64 [pure]
        16199091 => call @z17(%a1:i64) : f64 [pure]
        1868032 => call @z18(%a1:i64) : f64 [pure]
        14258 => call @z18(%a1:i64) : f64 [pure]
        244712242 => call @z19(%a1:i64) : f64 [pure]
        57303395 => call @z20(%a1:i64) : f64 [pure]
        57303459 => call @z21(%a1:i64) : f64 [pure]
        1987380 => call @z22(%a1:i64) : f64 [pure]
        1713595 => call @z23(%a1:i64) : f64 [pure]
        2003493 => call @z24(%a1:i64) : f64 [pure]
        220052207 => call @z25(%a1:i64) : f64 [pure]
        219778422 => call @z26(%a1:i64) : f64 [pure]
        220068320 => call @z27(%a1:i64) : f64 [pure]
        260346884 => call @z29(%a1:i64) : f64 [pure]
        224481049 => call @z30(%a1:i64) : f64 [pure]
        262457687 => call @z31(%a1:i64) : f64 [pure]
        224547597 => bin.div const.f64 1.00000000000000000 call @z29(%a1:i64) : f64 [pure] : f64
        260276799 => bin.div const.f64 1.00000000000000000 call @z30(%a1:i64) : f64 [pure] : f64
        224481180 => bin.div call @z30(%a1:i64) : f64 [pure] call @z29(%a1:i64) : f64 [pure] : f64
        826839025 => call @z32(%a1:i64) : f64 [pure]
        790973190 => call @z33(%a1:i64) : f64 [pure]
        828949828 => call @z34(%a1:i64) : f64 [pure]
        1713596 => bin.div call @z23(%a1:i64) : f64 [pure] call @z22(%a1:i64) : f64 [pure] : f64
        1986845 => bin.div const.f64 1.00000000000000000 call @z23(%a1:i64) : f64 [pure] : f64
        1714103 => bin.div const.f64 1.00000000000000000 call @z22(%a1:i64) : f64 [pure] : f64
        219778423 => bin.sub const.f64 1.57079632679489656 call @z27(%a1:i64) : f64 [pure] : f64
        220051672 => call @z26(bin.div const.f64 1.00000000000000000 %a1:i64 : f64) : f64 [pure]
        219778930 => call @z25(bin.div const.f64 1.00000000000000000 %a1:i64 : f64) : f64 [pure]
        283705086 => call @z8(%a1:i64) : f64 [pure]
        224308133 => call @z9(%a1:i64) : f64 [pure]
        420295318 => call @z10(%a1:i64) : f64 [pure]
        824551211 => call @z11(%a1:i64) : f64 [pure]
        354247632 => call @z35(bin.add %a1:i64 const.f64 1.00000000000000000 : f64) : f64 [pure]
        230982984 => call @z35(bin.add %a1:i64 const.f64 1.00000000000000000 : f64) : f64 [pure]
        553441405 => call @z35(%a1:i64) : f64 [pure]
        138294544 => call @z36(%a1:i64) : f64 [pure]
        1748297 => call @z38(%a1:i64) : f64 [pure]
        229027006 => call @z39(%a1:i64) : f64 [pure]
        244712290 => call @z40(%a1:i64) : f64 [pure]
        260345973 => case bin.gt %a1:i64 const.f64 0.00000000000000000 : i32 : f64 { 1 => const.f64 1.00000000000000000; _ => case bin.lt %a1:i64 const.f64 0.00000000000000000 : i32 : f64 { 1 => const.f64 -1.00000000000000000; _ => const.f64 0.00000000000000000; }; }
        231274442 => bin.sub %a1:i64 call @z10(%a1:i64) : f64 [pure] : f64
        1729434 => bin.mul %a1:i64 const.f64 57.29577951308232286 : f64
        1969161 => bin.mul %a1:i64 const.f64 0.01745329251994330 : f64
        992605559 => bin.mul %a1:i64 const.f64 0.01745329251994330 : f64
        430838469 => bin.mul %a1:i64 const.f64 57.29577951308232286 : f64
        184475947 => bin.mul %a1:i64 %a1:i64 : i64
        224581785 => bin.mul bin.mul %a1:i64 %a1:i64 : i64 %a1:i64 : i64
        _ => const.f64 0.00000000000000000
      }
    }
    fn @mak_ir_fn_30 ["<lambda>"] : (%a0: i64) -> i64 pure {
      case %a0:i64 : i64 {
        14777 => const.i64 1
        2003500 => const.i64 1
        101 => const.i64 1
        1935761 => const.i64 1
        621420511 => const.i64 1
        123496560 => const.i64 1
        1867848 => const.i64 1
        244688005 => const.i64 1
        57303579 => const.i64 1
        506744797 => const.i64 1
        _ => const.i64 0
      }
    }
    fn @mak_ir_fn_31 ["<lambda>"] : (%a0: i64) -> f64 pure {
      case %a0:i64 : f64 {
        14777 => const.f64 3.14159265358979312
        2003500 => const.f64 6.28318530717958623
        101 => const.f64 2.71828182845904509
        1935761 => const.f64 1.61803398874989490
        621420511 => const.f64 1.61803398874989490
        123496560 => const.f64 1.41421356237309515
        1867848 => const.f64 0.69314718055994529
        244688005 => const.f64 2.30258509299404590
        57303579 => const.f64 1.44269504088896339
        506744797 => const.f64 0.43429448190325182
        _ => const.f64 0.00000000000000000
      }
    }
    fn @mak_ir_fn_32 ["<lambda>"] : (%a0: !adt._a) -> !adt._d {
      case call @f2(%a0:!adt._a) : !adt._a : !adt._d {
        @k0 => make @k6() : !adt._d
        @k1(%mak_local_dk_247:i32, %mak_local_dl_248:!adt._a) => case case bin.ge %mak_local_dk_247:i32 const.i64 48 : i32 : i64 { 0 => const.i64 0; _ => case bin.le %mak_local_dk_247:i32 const.i64 57 : i32 : i64 { 1 => const.i64 1; _ => const.i64 0; }; } : !adt._d { 1 => case call @f7(make @k1(%mak_local_dk_247:i32, %mak_local_dl_248:!adt._a) : !adt._a, const.f64 0.00000000000000000) : !adt._d : !adt._d { @k6 => make @k6() : !adt._d; @k7(%mak_local_dm_249:f64, %mak_local_dn_250:!adt._a) => case %mak_local_dn_250:!adt._a : !adt._d { @k1(46, %mak_local_dp_251:!adt._a) => call @f8(%mak_local_dp_251:!adt._a, %mak_local_dm_249:f64, const.f64 0.10000000000000001) : !adt._d; @k1(%mak_local_dq_252:i32, %mak_local_dr_253:!adt._a) => case %mak_local_dq_252:i32 : !adt._d { 101 => case call @f6(%mak_local_dr_253:!adt._a, const.i64 1) : !adt._e : !adt._d { @k8 => make @k6() : !adt._d; @k9(%mak_local_ds_254:i64, %mak_local_dt_255:!adt._a) => make @k7(bin.mul %mak_local_dm_249:f64 call @f4(%mak_local_ds_254:i64) : f64 : f64, %mak_local_dt_255:!adt._a) : !adt._d; }; 69 => case call @f6(%mak_local_dr_253:!adt._a, const.i64 1) : !adt._e : !adt._d { @k8 => make @k6() : !adt._d; @k9(%mak_local_du_256:i64, %mak_local_dv_257:!adt._a) => make @k7(bin.mul %mak_local_dm_249:f64 call @f4(%mak_local_du_256:i64) : f64 : f64, %mak_local_dv_257:!adt._a) : !adt._d; }; _ => make @k7(%mak_local_dm_249:f64, %mak_local_dn_250:!adt._a) : !adt._d; }; _ => make @k7(%mak_local_dm_249:f64, %mak_local_dn_250:!adt._a) : !adt._d; }; }; _ => case %mak_local_dk_247:i32 : !adt._d { 46 => case %mak_local_dl_248:!adt._a : !adt._d { @k1(%mak_local_dw_258:i32, _) => case case bin.ge %mak_local_dw_258:i32 const.i64 48 : i32 : i64 { 0 => const.i64 0; _ => case bin.le %mak_local_dw_258:i32 const.i64 57 : i32 : i64 { 1 => const.i64 1; _ => const.i64 0; }; } : !adt._d { 1 => call @f8(%mak_local_dl_248:!adt._a, const.f64 0.00000000000000000, const.f64 0.10000000000000001) : !adt._d; _ => make @k6() : !adt._d; }; _ => make @k6() : !adt._d; }; _ => make @k6() : !adt._d; }; }
      }
    }
    fn @mak_ir_fn_33 ["<lambda>"] : () -> !adt._b {
      case call @z0() : i32 : !adt._b {
        %mak_local_bd_259:i32 => case bin.lt %mak_local_bd_259:i32 const.i64 0 : i32 : !adt._b { 1 => make @k2() : !adt._b; _ => case bin.eq %mak_local_bd_259:i32 const.i64 10 : i32 : !adt._b { 1 => make @k3(make @k0() : !adt._a) : !adt._b; _ => call @f1(make @k1(%mak_local_bd_259:i32, make @k0() : !adt._a) : !adt._a) : !adt._b; }; }
      }
    }
    fn @mak_ir_fn_34 ["<lambda>"] : (%a0: !adt._a) -> i64 {
      case call @f2(%a0:!adt._a) : !adt._a : i64 {
        @k0 => const.i64 0
        @k1(101, %mak_local_bm_260:!adt._a) => case %mak_local_bm_260:!adt._a : i64 { @k1(120, %mak_local_bn_261:!adt._a) => case %mak_local_bn_261:!adt._a : i64 { @k1(105, %mak_local_bo_262:!adt._a) => case %mak_local_bo_262:!adt._a : i64 { @k1(116, %mak_local_bp_263:!adt._a) => case call @f2(%mak_local_bp_263:!adt._a) : !adt._a : i64 { @k0 => const.i64 1; _ => const.i64 0; }; _ => const.i64 0; }; _ => const.i64 0; }; _ => const.i64 0; }
        _ => const.i64 0
      }
    }
    fn @mak_ir_fn_35 ["<lambda>"] : (%a0: !adt._a) -> i64 {
      case call @f2(%a0:!adt._a) : !adt._a : i64 {
        @k1(63, %mak_local_br_264:!adt._a) => case call @f2(%mak_local_br_264:!adt._a) : !adt._a : i64 { @k0 => const.i64 1; _ => const.i64 0; }
        @k1(104, %mak_local_bs_265:!adt._a) => case %mak_local_bs_265:!adt._a : i64 { @k1(101, %mak_local_bt_266:!adt._a) => case %mak_local_bt_266:!adt._a : i64 { @k1(108, %mak_local_bu_267:!adt._a) => case %mak_local_bu_267:!adt._a : i64 { @k1(112, %mak_local_bv_268:!adt._a) => case call @f2(%mak_local_bv_268:!adt._a) : !adt._a : i64 { @k0 => const.i64 1; _ => const.i64 0; }; _ => const.i64 0; }; _ => const.i64 0; }; _ => const.i64 0; }
        _ => const.i64 0
      }
    }
    fn @mak_ir_fn_36 ["<lambda>"] : () -> unit {
      block {
        yield call @z4(const.i64 1, const.str "\n=== MAK SCIENTIFIC CALCULATOR ===\n", call @z3(const.str "\n=== MAK SCIENTIFIC CALCULATOR ===\n") : u64) : unit
        yield call @z4(const.i64 1, const.str "Numbers are f64 (double precision). Output uses up to 17 significant digits.\n", call @z3(const.str "Numbers are f64 (double precision). Output uses up to 17 significant digits.\n") : u64) : unit
        yield call @z4(const.i64 1, const.str "Use radians for trig functions.\n\n", call @z3(const.str "Use radians for trig functions.\n\n") : u64) : unit
        yield call @z4(const.i64 1, const.str "ARITHMETIC\n", call @z3(const.str "ARITHMETIC\n") : u64) : unit
        yield call @z4(const.i64 1, const.str "  a+b       addition\n", call @z3(const.str "  a+b       addition\n") : u64) : unit
        yield call @z4(const.i64 1, const.str "  a-b       subtraction\n", call @z3(const.str "  a-b       subtraction\n") : u64) : unit
        yield call @z4(const.i64 1, const.str "  a*b       multiplication\n", call @z3(const.str "  a*b       multiplication\n") : u64) : unit
        yield call @z4(const.i64 1, const.str "  a/b       REAL division\n", call @z3(const.str "  a/b       REAL division\n") : u64) : unit
        yield call @z4(const.i64 1, const.str "  a//b      FLOOR / INTEGER-STYLE division\n", call @z3(const.str "  a//b      FLOOR / INTEGER-STYLE division\n") : u64) : unit
        yield call @z4(const.i64 1, const.str "  a%b       remainder (fmod)\n", call @z3(const.str "  a%b       remainder (fmod)\n") : u64) : unit
        yield call @z4(const.i64 1, const.str "  a^b       power\n", call @z3(const.str "  a^b       power\n") : u64) : unit
        yield call @z4(const.i64 1, const.str "  2(3+4), 2pi  implicit multiplication\n", call @z3(const.str "  2(3+4), 2pi  implicit multiplication\n") : u64) : unit
        yield call @z4(const.i64 1, const.str "  ( ... )   grouping\n", call @z3(const.str "  ( ... )   grouping\n") : u64) : unit
        yield call @z4(const.i64 1, const.str "  +x  -x    unary plus/minus\n", call @z3(const.str "  +x  -x    unary plus/minus\n") : u64) : unit
        yield call @z4(const.i64 1, const.str "  x!        factorial (gamma(x+1), including non-integer extension)\n", call @z3(const.str "  x!        factorial (gamma(x+1), including non-integer extension)\n") : u64) : unit
        yield call @z4(const.i64 1, const.str "  x%        percentage when % is at the end of a value\n\n", call @z3(const.str "  x%        percentage when % is at the end of a value\n\n") : u64) : unit
        yield call @z4(const.i64 1, const.str "CONSTANTS\n", call @z3(const.str "CONSTANTS\n") : u64) : unit
        yield call @z4(const.i64 1, const.str "  pi        3.141592653589793...\n", call @z3(const.str "  pi        3.141592653589793...\n") : u64) : unit
        yield call @z4(const.i64 1, const.str "  tau       2*pi\n", call @z3(const.str "  tau       2*pi\n") : u64) : unit
        yield call @z4(const.i64 1, const.str "  e         Euler's number\n", call @z3(const.str "  e         Euler's number\n") : u64) : unit
        yield call @z4(const.i64 1, const.str "  phi       golden ratio\n", call @z3(const.str "  phi       golden ratio\n") : u64) : unit
        yield call @z4(const.i64 1, const.str "  sqrt2     square root of 2\n", call @z3(const.str "  sqrt2     square root of 2\n") : u64) : unit
        yield call @z4(const.i64 1, const.str "  ln2       natural log of 2\n", call @z3(const.str "  ln2       natural log of 2\n") : u64) : unit
        yield call @z4(const.i64 1, const.str "  ln10      natural log of 10\n", call @z3(const.str "  ln10      natural log of 10\n") : u64) : unit
        yield call @z4(const.i64 1, const.str "  log2e     log base 2 of e\n", call @z3(const.str "  log2e     log base 2 of e\n") : u64) : unit
        yield call @z4(const.i64 1, const.str "  log10e    log base 10 of e\n\n", call @z3(const.str "  log10e    log base 10 of e\n\n") : u64) : unit
        yield call @z4(const.i64 1, const.str "BASIC FUNCTIONS\n", call @z3(const.str "BASIC FUNCTIONS\n") : u64) : unit
        yield call @z4(const.i64 1, const.str "  abs(x) fabs(x)     absolute value\n", call @z3(const.str "  abs(x) fabs(x)     absolute value\n") : u64) : unit
        yield call @z4(const.i64 1, const.str "  sqrt(x)            square root\n", call @z3(const.str "  sqrt(x)            square root\n") : u64) : unit
        yield call @z4(const.i64 1, const.str "  cbrt(x)            real cube root\n", call @z3(const.str "  cbrt(x)            real cube root\n") : u64) : unit
        yield call @z4(const.i64 1, const.str "  root(x,n)          nth root\n", call @z3(const.str "  root(x,n)          nth root\n") : u64) : unit
        yield call @z4(const.i64 1, const.str "  nthroot(x,n)       alias of root\n", call @z3(const.str "  nthroot(x,n)       alias of root\n") : u64) : unit
        yield call @z4(const.i64 1, const.str "  square(x)          x^2\n", call @z3(const.str "  square(x)          x^2\n") : u64) : unit
        yield call @z4(const.i64 1, const.str "  cube(x)            x^3\n", call @z3(const.str "  cube(x)            x^3\n") : u64) : unit
        yield call @z4(const.i64 1, const.str "  sign(x)            -1, 0, or +1\n", call @z3(const.str "  sign(x)            -1, 0, or +1\n") : u64) : unit
        yield call @z4(const.i64 1, const.str "  frac(x)            fractional part, x-trunc(x)\n", call @z3(const.str "  frac(x)            fractional part, x-trunc(x)\n") : u64) : unit
        yield call @z4(const.i64 1, const.str "  floor(x) ceil(x) trunc(x) round(x)\n", call @z3(const.str "  floor(x) ceil(x) trunc(x) round(x)\n") : u64) : unit
        yield call @z4(const.i64 1, const.str "  min(a,b) max(a,b) clamp(x,lo,hi)\n", call @z3(const.str "  min(a,b) max(a,b) clamp(x,lo,hi)\n") : u64) : unit
        yield call @z4(const.i64 1, const.str "  hypot(x,y)         sqrt(x^2+y^2)\n", call @z3(const.str "  hypot(x,y)         sqrt(x^2+y^2)\n") : u64) : unit
        yield call @z4(const.i64 1, const.str "  copysign(a,b)       magnitude of a with sign of b\n", call @z3(const.str "  copysign(a,b)       magnitude of a with sign of b\n") : u64) : unit
        yield call @z4(const.i64 1, const.str "  fmin fmax fdim      C floating-point min/max/positive-difference\n", call @z3(const.str "  fmin fmax fdim      C floating-point min/max/positive-difference\n") : u64) : unit
        yield call @z4(const.i64 1, const.str "  fma(a,b,c)          correctly-rounded a*b+c (where supported)\n", call @z3(const.str "  fma(a,b,c)          correctly-rounded a*b+c (where supported)\n") : u64) : unit
        yield call @z4(const.i64 1, const.str "  nextafter(a,b)      next representable double from a toward b\n\n", call @z3(const.str "  nextafter(a,b)      next representable double from a toward b\n\n") : u64) : unit
        yield call @z4(const.i64 1, const.str "EXPONENTIALS / LOGS\n", call @z3(const.str "EXPONENTIALS / LOGS\n") : u64) : unit
        yield call @z4(const.i64 1, const.str "  erf(x) erfc(x)      error function / complementary error function\n", call @z3(const.str "  erf(x) erfc(x)      error function / complementary error function\n") : u64) : unit
        yield call @z4(const.i64 1, const.str "  logb(x)             exponent of x in binary scale\n", call @z3(const.str "  logb(x)             exponent of x in binary scale\n") : u64) : unit
        yield call @z4(const.i64 1, const.str "  exp(x)              e^x\n", call @z3(const.str "  exp(x)              e^x\n") : u64) : unit
        yield call @z4(const.i64 1, const.str "  exp2(x)            2^x\n", call @z3(const.str "  exp2(x)            2^x\n") : u64) : unit
        yield call @z4(const.i64 1, const.str "  expm1(x)           e^x-1, accurate near zero\n", call @z3(const.str "  expm1(x)           e^x-1, accurate near zero\n") : u64) : unit
        yield call @z4(const.i64 1, const.str "  log(x) ln(x)       natural logarithm\n", call @z3(const.str "  log(x) ln(x)       natural logarithm\n") : u64) : unit
        yield call @z4(const.i64 1, const.str "  log2(x)             base-2 logarithm\n", call @z3(const.str "  log2(x)             base-2 logarithm\n") : u64) : unit
        yield call @z4(const.i64 1, const.str "  log10(x)            base-10 logarithm\n", call @z3(const.str "  log10(x)            base-10 logarithm\n") : u64) : unit
        yield call @z4(const.i64 1, const.str "  log1p(x)            ln(1+x), accurate near zero\n", call @z3(const.str "  log1p(x)            ln(1+x), accurate near zero\n") : u64) : unit
        yield call @z4(const.i64 1, const.str "  pow(x,y)            x^y\n\n", call @z3(const.str "  pow(x,y)            x^y\n\n") : u64) : unit
        yield call @z4(const.i64 1, const.str "TRIGONOMETRY \xe2\x80\x94 radians\n", call @z3(const.str "TRIGONOMETRY \xe2\x80\x94 radians\n") : u64) : unit
        yield call @z4(const.i64 1, const.str "  sin cos tan\n", call @z3(const.str "  sin cos tan\n") : u64) : unit
        yield call @z4(const.i64 1, const.str "  cot(x)=cos(x)/sin(x)\n", call @z3(const.str "  cot(x)=cos(x)/sin(x)\n") : u64) : unit
        yield call @z4(const.i64 1, const.str "  sec(x)=1/cos(x)\n", call @z3(const.str "  sec(x)=1/cos(x)\n") : u64) : unit
        yield call @z4(const.i64 1, const.str "  csc(x)=1/sin(x)\n", call @z3(const.str "  csc(x)=1/sin(x)\n") : u64) : unit
        yield call @z4(const.i64 1, const.str "  asin acos atan      inverse trig, result in radians\n", call @z3(const.str "  asin acos atan      inverse trig, result in radians\n") : u64) : unit
        yield call @z4(const.i64 1, const.str "  atan2(y,x)          quadrant-aware angle\n", call @z3(const.str "  atan2(y,x)          quadrant-aware angle\n") : u64) : unit
        yield call @z4(const.i64 1, const.str "  acot asec acsc      inverse cot/sec/csc\n\n", call @z3(const.str "  acot asec acsc      inverse cot/sec/csc\n\n") : u64) : unit
        yield call @z4(const.i64 1, const.str "HYPERBOLIC\n", call @z3(const.str "HYPERBOLIC\n") : u64) : unit
        yield call @z4(const.i64 1, const.str "  sinh cosh tanh\n", call @z3(const.str "  sinh cosh tanh\n") : u64) : unit
        yield call @z4(const.i64 1, const.str "  csch sech coth     reciprocal hyperbolic functions\n", call @z3(const.str "  csch sech coth     reciprocal hyperbolic functions\n") : u64) : unit
        yield call @z4(const.i64 1, const.str "  asinh acosh atanh\n\n", call @z3(const.str "  asinh acosh atanh\n\n") : u64) : unit
        yield call @z4(const.i64 1, const.str "ANGLE CONVERSION\n", call @z3(const.str "ANGLE CONVERSION\n") : u64) : unit
        yield call @z4(const.i64 1, const.str "  deg(x)              radians -> degrees\n", call @z3(const.str "  deg(x)              radians -> degrees\n") : u64) : unit
        yield call @z4(const.i64 1, const.str "  degrees(x)          alias\n", call @z3(const.str "  degrees(x)          alias\n") : u64) : unit
        yield call @z4(const.i64 1, const.str "  rad(x)              degrees -> radians\n", call @z3(const.str "  rad(x)              degrees -> radians\n") : u64) : unit
        yield call @z4(const.i64 1, const.str "  radians(x)          alias\n", call @z3(const.str "  radians(x)          alias\n") : u64) : unit
        yield call @z4(const.i64 1, const.str "  Example: sin(rad(30)) = 0.5\n\n", call @z3(const.str "  Example: sin(rad(30)) = 0.5\n\n") : u64) : unit
        yield call @z4(const.i64 1, const.str "NUMBER THEORY / COMBINATORICS\n", call @z3(const.str "NUMBER THEORY / COMBINATORICS\n") : u64) : unit
        yield call @z4(const.i64 1, const.str "  gcd(a,b)             greatest common divisor\n", call @z3(const.str "  gcd(a,b)             greatest common divisor\n") : u64) : unit
        yield call @z4(const.i64 1, const.str "  lcm(a,b)             least common multiple\n", call @z3(const.str "  lcm(a,b)             least common multiple\n") : u64) : unit
        yield call @z4(const.i64 1, const.str "  ncr(n,r) / comb      binomial coefficient\n", call @z3(const.str "  ncr(n,r) / comb      binomial coefficient\n") : u64) : unit
        yield call @z4(const.i64 1, const.str "  npr(n,r) / perm      permutations\n", call @z3(const.str "  npr(n,r) / perm      permutations\n") : u64) : unit
        yield call @z4(const.i64 1, const.str "  factorial(n) / fact  factorial\n", call @z3(const.str "  factorial(n) / fact  factorial\n") : u64) : unit
        yield call @z4(const.i64 1, const.str "  gamma(x)             Gamma(x)\n", call @z3(const.str "  gamma(x)             Gamma(x)\n") : u64) : unit
        yield call @z4(const.i64 1, const.str "  lgamma(x)            ln(|Gamma(x)|)\n\n", call @z3(const.str "  lgamma(x)            ln(|Gamma(x)|)\n\n") : u64) : unit
        yield call @z4(const.i64 1, const.str "REMAINDER FUNCTIONS\n", call @z3(const.str "REMAINDER FUNCTIONS\n") : u64) : unit
        yield call @z4(const.i64 1, const.str "  fmod(a,b) mod(a,b) rem(a,b)  floating remainder\n", call @z3(const.str "  fmod(a,b) mod(a,b) rem(a,b)  floating remainder\n") : u64) : unit
        yield call @z4(const.i64 1, const.str "  remainder(a,b)       IEEE-style remainder\n\n", call @z3(const.str "  remainder(a,b)       IEEE-style remainder\n\n") : u64) : unit
        yield call @z4(const.i64 1, const.str "PRECEDENCE (strongest first)\n", call @z3(const.str "PRECEDENCE (strongest first)\n") : u64) : unit
        yield call @z4(const.i64 1, const.str "  postfix !, final %\n", call @z3(const.str "  postfix !, final %\n") : u64) : unit
        yield call @z4(const.i64 1, const.str "  power ^             right-associative\n", call @z3(const.str "  power ^             right-associative\n") : u64) : unit
        yield call @z4(const.i64 1, const.str "  unary + -\n", call @z3(const.str "  unary + -\n") : u64) : unit
        yield call @z4(const.i64 1, const.str "  * / // %\n", call @z3(const.str "  * / // %\n") : u64) : unit
        yield call @z4(const.i64 1, const.str "  + -\n", call @z3(const.str "  + -\n") : u64) : unit
        yield call @z4(const.i64 1, const.str "  Use parentheses whenever you want explicit grouping.\n\n", call @z3(const.str "  Use parentheses whenever you want explicit grouping.\n\n") : u64) : unit
        yield call @z4(const.i64 1, const.str "EXAMPLES\n", call @z3(const.str "EXAMPLES\n") : u64) : unit
        yield call @z4(const.i64 1, const.str "  22/7                    3.1428571428571428\n", call @z3(const.str "  22/7                    3.1428571428571428\n") : u64) : unit
        yield call @z4(const.i64 1, const.str "  22//7                   3\n", call @z3(const.str "  22//7                   3\n") : u64) : unit
        yield call @z4(const.i64 1, const.str "  2^3^2                   512\n", call @z3(const.str "  2^3^2                   512\n") : u64) : unit
        yield call @z4(const.i64 1, const.str "  sqrt(2)^2               2\n", call @z3(const.str "  sqrt(2)^2               2\n") : u64) : unit
        yield call @z4(const.i64 1, const.str "  sin(pi/2)               1\n", call @z3(const.str "  sin(pi/2)               1\n") : u64) : unit
        yield call @z4(const.i64 1, const.str "  log10(1000)             3\n", call @z3(const.str "  log10(1000)             3\n") : u64) : unit
        yield call @z4(const.i64 1, const.str "  ncr(52,5)               2598960\n", call @z3(const.str "  ncr(52,5)               2598960\n") : u64) : unit
        yield call @z4(const.i64 1, const.str "  5!                      120\n\n", call @z3(const.str "  5!                      120\n\n") : u64) : unit
        yield call @z4(const.i64 1, const.str "IMPORTANT\n", call @z3(const.str "IMPORTANT\n") : u64) : unit
        yield call @z4(const.i64 1, const.str "  22/7  -> 3.1428571428571428...\n", call @z3(const.str "  22/7  -> 3.1428571428571428...\n") : u64) : unit
        yield call @z4(const.i64 1, const.str "  22//7 -> 3\n", call @z3(const.str "  22//7 -> 3\n") : u64) : unit
        yield call @z4(const.i64 1, const.str "  / never becomes integer division.\n", call @z3(const.str "  / never becomes integer division.\n") : u64) : unit
        yield call @z4(const.i64 1, const.str "  // is floor division, so -7//2 = -4.\n", call @z3(const.str "  // is floor division, so -7//2 = -4.\n") : u64) : unit
        yield call @z4(const.i64 1, const.str "  ^ accepts real exponents; e.g. 9^0.5 = 3.\n", call @z3(const.str "  ^ accepts real exponents; e.g. 9^0.5 = 3.\n") : u64) : unit
        yield call @z4(const.i64 1, const.str "  Invalid domains may display nan/inf according to libm.\n", call @z3(const.str "  Invalid domains may display nan/inf according to libm.\n") : u64) : unit
        yield call @z4(const.i64 1, const.str "  Type exit to leave the calculator.\n\n", call @z3(const.str "  Type exit to leave the calculator.\n\n") : u64) : unit
      }
    }
    fn @mak_ir_fn_37 ["<lambda>"] : (%a0: !adt._a) -> i32 {
      case call @f16(%a0:!adt._a) : !adt._f : i32 {
        @k11 => call @z4(const.i64 1, const.str "Error: invalid expression\n", call @z3(const.str "Error: invalid expression\n") : u64) : unit
        @k10(%mak_local_ig_269:f64, %mak_local_ih_270:!adt._a) => case call @f2(%mak_local_ih_270:!adt._a) : !adt._a : i32 { @k0 => call @z2(const.str "%.17g\n", %mak_local_ig_269:f64) : i32; _ => call @z4(const.i64 1, const.str "Error: unexpected characters\n", call @z3(const.str "Error: unexpected characters\n") : u64) : unit; }
      }
    }
    fn @mak_ir_fn_38 ["<lambda>"] : (%a0: f64) -> i32 {
      yield call @z2(const.str "%.17g\n", %a0:f64) : i32
    }
    fn @mak_ir_fn_39 ["<lambda>"] : () -> unit {
      block {
        yield call @z4(const.i64 1, const.str "MAK Scientific Calculator\n", call @z3(const.str "MAK Scientific Calculator\n") : u64) : unit
        yield call @z4(const.i64 1, const.str "Real /, floor //, remainder %, power ^, factorial !\n", call @z3(const.str "Real /, floor //, remainder %, power ^, factorial !\n") : u64) : unit
        yield call @z4(const.i64 1, const.str "Type help for the complete scientific function list.\n", call @z3(const.str "Type help for the complete scientific function list.\n") : u64) : unit
        yield call @z4(const.i64 1, const.str "Example: 22/7 = 3.1428571428571428...\n\n", call @z3(const.str "Example: 22/7 = 3.1428571428571428...\n\n") : u64) : unit
        yield call @f17() : unit
      }
    }
  }

  entry {
    block {
      yield call @z4(const.i64 1, const.str "MAK Scientific Calculator\n", call @z3(const.str "MAK Scientific Calculator\n") : u64) : unit
      yield call @z4(const.i64 1, const.str "Real /, floor //, remainder %, power ^, factorial !\n", call @z3(const.str "Real /, floor //, remainder %, power ^, factorial !\n") : u64) : unit
      yield call @z4(const.i64 1, const.str "Type help for the complete scientific function list.\n", call @z3(const.str "Type help for the complete scientific function list.\n") : u64) : unit
      yield call @z4(const.i64 1, const.str "Example: 22/7 = 3.1428571428571428...\n\n", call @z3(const.str "Example: 22/7 = 3.1428571428571428...\n\n") : u64) : unit
      yield call @f17() : unit
    }
  }
}
