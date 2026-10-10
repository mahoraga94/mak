module @mak {
  format = "mLIR 0.1"
  optimized = true

  externals {
    extern @mak_ir_ext_0 symbol="putchar" type=fn(i32) -> unit
  }

  entry {
    block {
      yield call @z0(direct-c : u8) : unit
      yield call @z0(direct-c : u8) : unit
      yield call @z0(direct-c : u8) : unit
      yield call @z0(const.i64 10) : unit
    }
  }
}
