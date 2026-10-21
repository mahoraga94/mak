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
    (void)((int64_t)mak_ir_fail_u64("non-numeric binary operation"));
    return 0;
}
