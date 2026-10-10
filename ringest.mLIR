module @mak {
  format = "mLIR 0.1"
  optimized = true

  types {
    type @_b = adt
      ctor @C tag=1 ()
      ctor @D tag=2 (unknown, !adt._b)
  }

  externals {
    extern @mak_ir_ext_0 symbol="printf" type=fn(ptr, ...) -> i32
  }

  functions {
    fn @mak_ir_fn_0 ["b"] : (%a0: !adt._b) -> i64 {
      case %a0:!adt._b : i64 {
        @k0 => call @z0(const.str "List is empty\n") : i32
        @k1(%mak_local_g_0:unknown, %mak_local_h_1:!adt._b) => block { call @z0(const.str "List item: %d\n", %mak_local_g_0:unknown) : i32; yield call @f0(%mak_local_h_1:!adt._b) : i64 }
      }
    }
  }

  entry {
    block {
      yield call @z0(const.str "--- Starting Type Verification ---\n") : i32
      yield call @f0(make @k1(const.i64 10, make @k1(const.i64 20, make @k1(const.i64 30, make @k0() : !adt._b) : !adt._b) : !adt._b) : !adt._b) : i64
      yield call @z0(const.str "--- Verification Complete ---\n") : i32
      yield const.i64 0
    }
  }
}
