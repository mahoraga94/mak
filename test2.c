#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int64_t mak_ir_i64_from_bits(uint64_t bits) { int64_t out; memcpy(&out, &bits, sizeof out); return out; }
int64_t mak_ir_i64_neg_wrap(int64_t a) { return mak_ir_i64_from_bits(UINT64_C(0) - (uint64_t)a); }
int64_t mak_ir_i64_add_wrap(int64_t a, int64_t b) { return mak_ir_i64_from_bits((uint64_t)a + (uint64_t)b); }
int64_t mak_ir_i64_sub_wrap(int64_t a, int64_t b) { return mak_ir_i64_from_bits((uint64_t)a - (uint64_t)b); }
int64_t mak_ir_i64_mul_wrap(int64_t a, int64_t b) { return mak_ir_i64_from_bits((uint64_t)a * (uint64_t)b); }
int64_t mak_ir_i64_div_wrap(int64_t a, int64_t b) { if (a == INT64_MIN && b == -1) return INT64_MIN; return a / b; }
int64_t mak_ir_i64_rem_wrap(int64_t a, int64_t b) { if (a == INT64_MIN && b == -1) return 0; return a % b; }
int64_t mak_ir_i64_shl_wrap(int64_t a, int64_t b) { return mak_ir_i64_from_bits((uint64_t)a << ((uint64_t)b & UINT64_C(63))); }
int64_t mak_ir_i64_shr(int64_t a, int64_t b) { uint64_t n = (uint64_t)b & UINT64_C(63); uint64_t bits; memcpy(&bits, &a, sizeof bits); if (n == 0) return a; return mak_ir_i64_from_bits((bits >> n) | (a < 0 ? (UINT64_MAX << (64 - n)) : UINT64_C(0))); }

extern int32_t mak_ir_ext_0(const void *, ...) __asm__("printf");

#ifndef MAK_RUNTIME_STRICT
#define MAK_RUNTIME_STRICT 0
#endif
static uint64_t mak_rt_recovery_count = 0;
static inline void mak_ir_recover(const char *reason) {
    (void)reason; ++mak_rt_recovery_count;
#if MAK_RUNTIME_STRICT
    fprintf(stderr, "[MAK] runtime failure: %s\n", reason); abort();
#endif
}
static inline uint64_t mak_ir_fail_u64(const char *reason) { mak_ir_recover(reason); return 0; }


int main(void) {
    mak_ir_ext_0((const void *)("01 literal -1: %ld\n"), INT64_C(-1));
    mak_ir_ext_0((const void *)("02 0-1: %ld\n"), INT64_C(-1));
    mak_ir_ext_0((const void *)("03 -1-1: %ld\n"), INT64_C(-2));
    mak_ir_ext_0((const void *)("04 -(1+1): %ld\n"), INT64_C(-2));
    mak_ir_ext_0((const void *)("05 --1: %ld\n"), INT64_C(1));
    mak_ir_ext_0((const void *)("06 ---1: %ld\n"), INT64_C(-1));
    mak_ir_ext_0((const void *)("07 -(-1): %ld\n"), INT64_C(1));
    mak_ir_ext_0((const void *)("08 -5+2: %ld\n"), INT64_C(-3));
    mak_ir_ext_0((const void *)("09 2+(-5): %ld\n"), INT64_C(-3));
    mak_ir_ext_0((const void *)("10 -5-(-2): %ld\n"), INT64_C(-3));
    mak_ir_ext_0((const void *)("11 -2-(-5): %ld\n"), INT64_C(3));
    mak_ir_ext_0((const void *)("12 (-2)*3: %ld\n"), INT64_C(-6));
    mak_ir_ext_0((const void *)("13 (-2)*(-3): %ld\n"), INT64_C(6));
    mak_ir_ext_0((const void *)("14 -(2*3): %ld\n"), INT64_C(-6));
    mak_ir_ext_0((const void *)("15 -1-1-1: %ld\n"), INT64_C(-3));
    mak_ir_ext_0((const void *)("16 -1-(1-2): %ld\n"), INT64_C(0));
    mak_ir_ext_0((const void *)("17 -(1-2): %ld\n"), INT64_C(1));
    mak_ir_ext_0((const void *)("18 -1+2*3: %ld\n"), INT64_C(5));
    mak_ir_ext_0((const void *)("19 (-1+2)*3: %ld\n"), INT64_C(3));
    mak_ir_ext_0((const void *)("20 -7/2: %ld\n"), INT64_C(-3));
    mak_ir_ext_0((const void *)("21 -7 remainder 2: %ld\n"), INT64_C(-1));
    mak_ir_ext_0((const void *)("22 7/(-2): %ld\n"), INT64_C(-3));
    mak_ir_ext_0((const void *)("23 7 remainder -2: %ld\n"), INT64_C(1));
    mak_ir_ext_0((const void *)("24 (-7)/(-2): %ld\n"), INT64_C(3));
    mak_ir_ext_0((const void *)("25 -7 remainder -2: %ld\n"), INT64_C(-1));
    mak_ir_ext_0((const void *)("26 (0-2)*3: %ld\n"), INT64_C(-6));
    mak_ir_ext_0((const void *)("27 0-(-1): %ld\n"), INT64_C(1));
    mak_ir_ext_0((const void *)("28 -(0-1): %ld\n"), INT64_C(1));
    mak_ir_ext_0((const void *)("29 (-3)*(-4)-20: %ld\n"), INT64_C(-8));
    mak_ir_ext_0((const void *)("30 i64 minimum: %ld\n"), INT64_MIN);
    mak_ir_ext_0((const void *)("31 max + 1: %ld\n"), INT64_MIN);
    mak_ir_ext_0((const void *)("32 min - 1: %ld\n"), INT64_C(9223372036854775807));
    mak_ir_ext_0((const void *)("33 min * 2: %ld\n"), INT64_C(0));
    mak_ir_ext_0((const void *)("34 max * 2: %ld\n"), INT64_C(-2));
    return 0;
}
