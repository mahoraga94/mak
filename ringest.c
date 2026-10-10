#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

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
    (void)INT64_C(0);
    return 0;
}
