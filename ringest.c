#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern void mak_ir_ext_0(int32_t) __asm__("putchar");

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
    int32_t arg_0 = (int32_t)((*((const char *)(uintptr_t)("MAK") + (intptr_t)(INT64_C(0)))));
    mak_ir_ext_0(arg_0);
    int32_t arg_1 = (int32_t)((*((const char *)(uintptr_t)("MAK") + (intptr_t)(INT64_C(1)))));
    mak_ir_ext_0(arg_1);
    int32_t arg_2 = (int32_t)((*((const char *)(uintptr_t)(((const char *)(((const char *)(uintptr_t)("MAK")) + (intptr_t)(INT64_C(2))))) + (intptr_t)(INT64_C(0)))));
    mak_ir_ext_0(arg_2);
    mak_ir_ext_0((int32_t)(INT64_C(10)));
    return 0;
}
