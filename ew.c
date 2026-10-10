#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

enum {
    mak_ir_tag_0 = 1u,
    mak_ir_tag_1 = 2u,
    mak_ir_tag_2 = 3u,
    mak_ir_tag_3 = 4u,
    mak_ir_tag_4 = 5u,
    mak_ir_tag_5 = 6u,
    mak_ir_tag_6 = 7u,
    mak_ir_tag_7 = 8u,
};

struct mak__a {
    uint32_t tag;
    union {
        struct {
            int64_t f0;
            struct mak__a * f1;
        } v1;
    } v;
};

static struct mak__a mak_ir_ctor_obj_0 = { mak_ir_tag_0 };
static struct mak__a mak_ir_fallback_obj__a = { mak_ir_tag_0 };
struct mak__b {
    uint32_t tag;
    union {
        struct {
            struct mak__a * f0;
        } v3;
    } v;
};

static struct mak__b mak_ir_ctor_obj_2 = { mak_ir_tag_2 };
static struct mak__b mak_ir_fallback_obj__b = { mak_ir_tag_2 };
struct mak__d {
    uint32_t tag;
    union {
        struct {
            int64_t f0;
            int64_t f1;
        } v5;
    } v;
};

static struct mak__d mak_ir_ctor_obj_4 = { mak_ir_tag_4 };
static struct mak__d mak_ir_fallback_obj__d = { mak_ir_tag_4 };
struct mak__f {
    uint32_t tag;
    union {
        struct {
            int64_t f0;
            int64_t f1;
        } v6;
    } v;
};

static struct mak__f mak_ir_ctor_obj_7 = { mak_ir_tag_7 };
static struct mak__f mak_ir_fallback_obj__f = { mak_ir_tag_6 };
#ifndef MAK_ARENA_BYTES
#define MAK_ARENA_BYTES (64ULL * 1024ULL * 1024ULL)
#endif
static unsigned char mak_ir_arena[MAK_ARENA_BYTES];
static size_t mak_ir_arena_pos = 0;
static void *mak_ir_arena_alloc(size_t size, size_t align) {
    if (align == 0) align = 1;
    if ((align & (align - 1)) != 0) align = sizeof(void *);
    size_t mask = align - 1;
    if (mak_ir_arena_pos > SIZE_MAX - mask) return NULL;
    size_t start = (mak_ir_arena_pos + mask) & ~mask;
    if (start > MAK_ARENA_BYTES || size > MAK_ARENA_BYTES - start) return NULL;
    void *p = (void *)(mak_ir_arena + start);
    mak_ir_arena_pos = start + size;
    return p;
}

static void mak_ir_recover(const char *reason) {
#ifdef MAK_RUNTIME_RECOVER
    fprintf(stderr, "[MAK recovery] %s\n", reason);
#else
    (void)reason;
#endif
}

extern int32_t mak_ir_ext_0(void) __asm__("getchar");
extern int32_t mak_ir_ext_1(int32_t) __asm__("putchar");
extern uint64_t mak_ir_ext_2(const void *) __asm__("strlen");
extern int64_t mak_ir_ext_3(int32_t, const void *, uint64_t) __asm__("write");

static void mak_ir_fn_0(struct mak__a * a0, struct mak__a * a1, struct mak__a *out);
static void mak_ir_fn_1(struct mak__a * a0, struct mak__b *out);
static int64_t mak_ir_fn_2(int64_t a0);
static int64_t mak_ir_fn_3(const char * a0);
static void mak_ir_fn_4(int64_t a0, struct mak__a * a1, struct mak__a *out);
static int64_t mak_ir_fn_5(struct mak__a * a0);
static void mak_ir_fn_6(int64_t a0, int64_t a1, struct mak__d *out);
static int64_t mak_ir_fn_7(int64_t a0, int64_t a1);
static void mak_ir_fn_8(int64_t a0, struct mak__f *out);
static void mak_ir_fn_9(int64_t a0, int64_t a1, struct mak__f *out);
static void mak_ir_fn_10(int64_t a0, struct mak__f *out);
static void mak_ir_fn_11(int64_t a0, int64_t a1, struct mak__f *out);
static void mak_ir_fn_12(int64_t a0, struct mak__f *out);
static int32_t mak_ir_fn_13(void);

static void mak_ir_fn_0(struct mak__a * a0, struct mak__a * a1, struct mak__a *out) {
    for (;;) {
        *(out) = *(&mak_ir_fallback_obj__a);
        if ((a0 != NULL && (a0)->tag == mak_ir_tag_0)) {
            *(out) = *(a1);
        }
        else if ((a0 != NULL && (a0)->tag == mak_ir_tag_1)) {
            int64_t mak_local_z_0 = (a0)->v.v1.f0;
            struct mak__a * mak_local_aa_1 = (a0)->v.v1.f1;
            struct mak__a * tailarg_0 = mak_local_aa_1;
            struct mak__a *arena_obj_2 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
            if (!arena_obj_2) {
                mak_ir_recover("ADT arena exhausted");
                arena_obj_2 = &mak_ir_fallback_obj__a;
             } else {
                (arena_obj_2)->tag = mak_ir_tag_1;
                (arena_obj_2)->v.v1.f0 = mak_local_z_0;
                (arena_obj_2)->v.v1.f1 = a1;
            }
            struct mak__a * tailarg_1 = arena_obj_2;
            a0 = tailarg_0;
            a1 = tailarg_1;
            continue;
        }
        else {
            mak_ir_recover("ADT case had no matching arm");
            /* deterministic ADT default already initialized */
        }
        return;
    }
}

static void mak_ir_fn_1(struct mak__a * a0, struct mak__b *out) {
    for (;;) {
        int32_t mak_local_ad_2 = mak_ir_ext_0();
        int32_t scrut_3 = (mak_local_ad_2 < (int32_t)(INT64_C(0)));
        *(out) = *(&mak_ir_fallback_obj__b);
        switch (scrut_3) {
            case INT64_C(1): {
                (out)->tag = mak_ir_tag_3;
                struct mak__a *arena_call_4 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
                if (!arena_call_4) {
                    mak_ir_recover("ADT arena exhausted");
                    arena_call_4 = &mak_ir_fallback_obj__a;
                 } else {
                    mak_ir_fn_0(a0, &mak_ir_ctor_obj_0, (struct mak__a *)(arena_call_4));
                }
                (out)->v.v3.f0 = arena_call_4;
                break;
            }
            default: {
                int32_t scrut_5 = (mak_local_ad_2 == (int32_t)(INT64_C(10)));
                *(out) = *(&mak_ir_fallback_obj__b);
                switch (scrut_5) {
                    case INT64_C(1): {
                        (out)->tag = mak_ir_tag_3;
                        struct mak__a *arena_call_6 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
                        if (!arena_call_6) {
                            mak_ir_recover("ADT arena exhausted");
                            arena_call_6 = &mak_ir_fallback_obj__a;
                         } else {
                            mak_ir_fn_0(a0, &mak_ir_ctor_obj_0, (struct mak__a *)(arena_call_6));
                        }
                        (out)->v.v3.f0 = arena_call_6;
                        break;
                    }
                    default: {
                        struct mak__a *arena_obj_8 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
                        if (!arena_obj_8) {
                            mak_ir_recover("ADT arena exhausted");
                            arena_obj_8 = &mak_ir_fallback_obj__a;
                         } else {
                            (arena_obj_8)->tag = mak_ir_tag_1;
                            (arena_obj_8)->v.v1.f0 = (int64_t)(mak_local_ad_2);
                            (arena_obj_8)->v.v1.f1 = a0;
                        }
                        struct mak__a * tailarg_7 = arena_obj_8;
                        a0 = tailarg_7;
                        continue;
                    }
                }
                break;
            }
        }
        return;
    }
}

static int64_t mak_ir_fn_2(int64_t a0) {
    for (;;) {
        if (0) {
            return (int64_t)((uintptr_t)(&mak_ir_ctor_obj_0));
        }
        else if (0) {
            int64_t mak_local_ag_3 = ((struct mak__a *)((uintptr_t)(a0)))->v.v1.f0;
            struct mak__a * mak_local_ah_4 = ((struct mak__a *)((uintptr_t)(a0)))->v.v1.f1;
            int64_t case_10 = 0;
            case_10 = 0;
            switch (mak_local_ag_3) {
                case INT64_C(9): {
                    case_10 = INT64_C(1);
                    break;
                }
                case INT64_C(10): {
                    case_10 = INT64_C(1);
                    break;
                }
                case INT64_C(13): {
                    case_10 = INT64_C(1);
                    break;
                }
                case INT64_C(32): {
                    case_10 = INT64_C(1);
                    break;
                }
                default: {
                    case_10 = INT64_C(0);
                    break;
                }
            }
            int64_t scrut_9 = case_10;
            switch (scrut_9) {
                case INT64_C(1): {
                    struct mak__a * tailarg_11 = mak_local_ah_4;
                    a0 = (int64_t)((uintptr_t)(tailarg_11));
                    continue;
                }
                default: {
                    return a0;
                }
            }
        }
        else {
            mak_ir_recover("case had no matching arm");
            return 0;
        }
    }
}

static int64_t mak_ir_fn_3(const char * a0) {
    for (;;) {
        return mak_ir_ext_3((int32_t)(INT64_C(1)), (const void *)(a0), mak_ir_ext_2((const void *)(a0)));
    }
}

static void mak_ir_fn_4(int64_t a0, struct mak__a * a1, struct mak__a *out) {
    for (;;) {
        int32_t scrut_12 = ((int32_t)(a0) == (int32_t)(INT64_C(0)));
        *(out) = *(&mak_ir_fallback_obj__a);
        switch (scrut_12) {
            case INT64_C(1): {
                *(out) = *(a1);
                break;
            }
            default: {
                int64_t tailarg_13 = (a0 / INT64_C(10));
                struct mak__a * tailarg_14 = &mak_ir_fallback_obj__a;
                a0 = tailarg_13;
                a1 = tailarg_14;
                continue;
            }
        }
        return;
    }
}

static int64_t mak_ir_fn_5(struct mak__a * a0) {
    for (;;) {
        if ((a0 != NULL && (a0)->tag == mak_ir_tag_0)) {
            return INT64_C(0);
        }
        else if ((a0 != NULL && (a0)->tag == mak_ir_tag_1)) {
            int64_t mak_local_an_5 = (a0)->v.v1.f0;
            struct mak__a * mak_local_ap_6 = (a0)->v.v1.f1;
            mak_ir_ext_1((int32_t)(mak_local_an_5));
            struct mak__a * tailarg_15 = mak_local_ap_6;
            a0 = tailarg_15;
            continue;
        }
        else {
            mak_ir_recover("case had no matching arm");
            return 0;
        }
    }
}

static void mak_ir_fn_6(int64_t a0, int64_t a1, struct mak__d *out) {
    for (;;) {
        *(out) = *(&mak_ir_fallback_obj__d);
        if (0) {
            (out)->tag = mak_ir_tag_5;
            (out)->v.v5.f0 = a1;
            int64_t arg_16 = (int64_t)((uintptr_t)(&mak_ir_ctor_obj_0));
            (out)->v.v5.f1 = arg_16;
        }
        else if (0) {
            int64_t mak_local_at_7 = ((struct mak__a *)((uintptr_t)(a0)))->v.v1.f0;
            struct mak__a * mak_local_at_8 = ((struct mak__a *)((uintptr_t)(a0)))->v.v1.f1;
            int64_t case_18 = 0;
            int32_t scrut_19 = ((int32_t)((uintptr_t)(mak_local_at_8)) >= (int32_t)(INT64_C(48)));
            case_18 = 0;
            switch (scrut_19) {
                case INT64_C(0): {
                    case_18 = INT64_C(0);
                    break;
                }
                default: {
                    int64_t case_20 = 0;
                    int32_t scrut_21 = ((int32_t)((uintptr_t)(mak_local_at_8)) <= (int32_t)(INT64_C(57)));
                    case_20 = 0;
                    switch (scrut_21) {
                        case INT64_C(1): {
                            case_20 = INT64_C(1);
                            break;
                        }
                        default: {
                            case_20 = INT64_C(0);
                            break;
                        }
                    }
                    case_18 = case_20;
                    break;
                }
            }
            int64_t scrut_17 = case_18;
            *(out) = *(&mak_ir_fallback_obj__d);
            switch (scrut_17) {
                case INT64_C(1): {
                    int64_t tailarg_22 = (int64_t)((uintptr_t)(mak_local_at_8));
                    int64_t eval_25 = (a1 * INT64_C(10));
                    int64_t eval_24 = (eval_25 + (int64_t)((uintptr_t)(mak_local_at_8)));
                    int64_t tailarg_23 = (eval_24 - INT64_C(48));
                    a0 = tailarg_22;
                    a1 = tailarg_23;
                    continue;
                }
                default: {
                    (out)->tag = mak_ir_tag_5;
                    (out)->v.v5.f0 = a1;
                    (out)->v.v5.f1 = a0;
                    break;
                }
            }
        }
        else {
            mak_ir_recover("ADT case had no matching arm");
            /* deterministic ADT default already initialized */
        }
        return;
    }
}

static int64_t mak_ir_fn_7(int64_t a0, int64_t a1) {
    for (;;) {
        int32_t scrut_26 = ((int32_t)(a1) <= (int32_t)(INT64_C(0)));
        switch (scrut_26) {
            case INT64_C(1): {
                return INT64_C(1);
            }
            default: {
                int64_t mak_local_ax_9 = mak_ir_fn_7(a0, (a1 >> INT64_C(1)));
                int64_t scrut_27 = (a1 & INT64_C(1));
                switch (scrut_27) {
                    case INT64_C(0): {
                        return (mak_local_ax_9 * mak_local_ax_9);
                    }
                    default: {
                        int64_t eval_28 = (mak_local_ax_9 * mak_local_ax_9);
                        return (eval_28 * a0);
                    }
                }
            }
        }
    }
}

static void mak_ir_fn_8(int64_t a0, struct mak__f *out) {
    for (;;) {
        int64_t scrut_29 = mak_ir_fn_2(a0);
        *(out) = *(&mak_ir_fallback_obj__f);
        if (0) {
            *(out) = mak_ir_ctor_obj_7;
        }
        else if (0) {
            int64_t mak_local_az_10 = ((struct mak__a *)((uintptr_t)(scrut_29)))->v.v1.f0;
            struct mak__a * mak_local_ba_11 = ((struct mak__a *)((uintptr_t)(scrut_29)))->v.v1.f1;
            *(out) = *(&mak_ir_fallback_obj__f);
            switch (mak_local_az_10) {
                case INT64_C(43): {
                    int64_t tailarg_30 = (int64_t)((uintptr_t)(mak_local_ba_11));
                    a0 = tailarg_30;
                    continue;
                }
                case INT64_C(45): {
                    struct mak__f *arena_call_32 = (struct mak__f *)mak_ir_arena_alloc(sizeof(struct mak__f), _Alignof(struct mak__f));
                    if (!arena_call_32) {
                        mak_ir_recover("ADT arena exhausted");
                        arena_call_32 = &mak_ir_fallback_obj__f;
                     } else {
                        mak_ir_fn_8((int64_t)((uintptr_t)(mak_local_ba_11)), (struct mak__f *)(arena_call_32));
                    }
                    struct mak__f * scrut_31 = arena_call_32;
                    *(out) = *(&mak_ir_fallback_obj__f);
                    if ((scrut_31 != NULL && (scrut_31)->tag == mak_ir_tag_7)) {
                        *(out) = mak_ir_ctor_obj_7;
                    }
                    else if ((scrut_31 != NULL && (scrut_31)->tag == mak_ir_tag_6)) {
                        int64_t mak_local_bb_12 = (scrut_31)->v.v6.f0;
                        int64_t mak_local_bd_13 = (scrut_31)->v.v6.f1;
                        (out)->tag = mak_ir_tag_6;
                        (out)->v.v6.f0 = (INT64_C(0) - mak_local_bb_12);
                        (out)->v.v6.f1 = mak_local_bd_13;
                    }
                    else {
                        mak_ir_recover("ADT case had no matching arm");
                        /* deterministic ADT default already initialized */
                    }
                    break;
                }
                default: {
                    struct mak__f *arena_case_34 = (struct mak__f *)mak_ir_arena_alloc(sizeof(struct mak__f), _Alignof(struct mak__f));
                    if (!arena_case_34) {
                        mak_ir_recover("ADT arena exhausted");
                        arena_case_34 = &mak_ir_fallback_obj__f;
                     } else {
                        int64_t scrut_35 = mak_ir_fn_2(a0);
                        *(arena_case_34) = *(&mak_ir_fallback_obj__f);
                        if (0) {
                            *(arena_case_34) = mak_ir_ctor_obj_7;
                        }
                        else if (0) {
                            int64_t mak_local_bf_14 = ((struct mak__a *)((uintptr_t)(scrut_35)))->v.v1.f0;
                            struct mak__a * mak_local_bg_15 = ((struct mak__a *)((uintptr_t)(scrut_35)))->v.v1.f1;
                            *(arena_case_34) = *(&mak_ir_fallback_obj__f);
                            switch (mak_local_bf_14) {
                                case INT64_C(40): {
                                    struct mak__f *arena_call_37 = (struct mak__f *)mak_ir_arena_alloc(sizeof(struct mak__f), _Alignof(struct mak__f));
                                    if (!arena_call_37) {
                                        mak_ir_recover("ADT arena exhausted");
                                        arena_call_37 = &mak_ir_fallback_obj__f;
                                     } else {
                                        mak_ir_fn_12((int64_t)((uintptr_t)(mak_local_bg_15)), (struct mak__f *)(arena_call_37));
                                    }
                                    struct mak__f * scrut_36 = arena_call_37;
                                    *(arena_case_34) = *(&mak_ir_fallback_obj__f);
                                    if ((scrut_36 != NULL && (scrut_36)->tag == mak_ir_tag_7)) {
                                        *(arena_case_34) = mak_ir_ctor_obj_7;
                                    }
                                    else if ((scrut_36 != NULL && (scrut_36)->tag == mak_ir_tag_6)) {
                                        int64_t mak_local_bh_16 = (scrut_36)->v.v6.f0;
                                        int64_t mak_local_bj_17 = (scrut_36)->v.v6.f1;
                                        int64_t scrut_38 = mak_ir_fn_2(mak_local_bj_17);
                                        *(arena_case_34) = *(&mak_ir_fallback_obj__f);
                                        if (0) {
                                            int64_t mak_local_bk_18 = ((struct mak__a *)((uintptr_t)(scrut_38)))->v.v1.f0;
                                            struct mak__a * mak_local_bl_19 = ((struct mak__a *)((uintptr_t)(scrut_38)))->v.v1.f1;
                                            *(arena_case_34) = *(&mak_ir_fallback_obj__f);
                                            switch (mak_local_bk_18) {
                                                case INT64_C(41): {
                                                    (arena_case_34)->tag = mak_ir_tag_6;
                                                    (arena_case_34)->v.v6.f0 = mak_local_bh_16;
                                                    (arena_case_34)->v.v6.f1 = (int64_t)((uintptr_t)(mak_local_bl_19));
                                                    break;
                                                }
                                                default: {
                                                    *(arena_case_34) = mak_ir_ctor_obj_7;
                                                    break;
                                                }
                                            }
                                        }
                                        else if (0) {
                                            *(arena_case_34) = mak_ir_ctor_obj_7;
                                        }
                                        else {
                                            mak_ir_recover("ADT case had no matching arm");
                                            /* deterministic ADT default already initialized */
                                        }
                                    }
                                    else {
                                        mak_ir_recover("ADT case had no matching arm");
                                        /* deterministic ADT default already initialized */
                                    }
                                    break;
                                }
                                default: {
                                    struct mak__d *arena_case_40 = (struct mak__d *)mak_ir_arena_alloc(sizeof(struct mak__d), _Alignof(struct mak__d));
                                    if (!arena_case_40) {
                                        mak_ir_recover("ADT arena exhausted");
                                        arena_case_40 = &mak_ir_fallback_obj__d;
                                     } else {
                                        int64_t scrut_41 = mak_ir_fn_2(a0);
                                        *(arena_case_40) = *(&mak_ir_fallback_obj__d);
                                        if (0) {
                                            *(arena_case_40) = mak_ir_ctor_obj_4;
                                        }
                                        else if (0) {
                                            int64_t mak_local_bm_20 = ((struct mak__a *)((uintptr_t)(scrut_41)))->v.v1.f0;
                                            struct mak__a * mak_local_bn_21 = ((struct mak__a *)((uintptr_t)(scrut_41)))->v.v1.f1;
                                            int64_t case_43 = 0;
                                            int32_t scrut_44 = ((int32_t)(mak_local_bm_20) >= (int32_t)(INT64_C(48)));
                                            case_43 = 0;
                                            switch (scrut_44) {
                                                case INT64_C(0): {
                                                    case_43 = INT64_C(0);
                                                    break;
                                                }
                                                default: {
                                                    int64_t case_45 = 0;
                                                    int32_t scrut_46 = ((int32_t)(mak_local_bm_20) <= (int32_t)(INT64_C(57)));
                                                    case_45 = 0;
                                                    switch (scrut_46) {
                                                        case INT64_C(1): {
                                                            case_45 = INT64_C(1);
                                                            break;
                                                        }
                                                        default: {
                                                            case_45 = INT64_C(0);
                                                            break;
                                                        }
                                                    }
                                                    case_43 = case_45;
                                                    break;
                                                }
                                            }
                                            int64_t scrut_42 = case_43;
                                            *(arena_case_40) = *(&mak_ir_fallback_obj__d);
                                            switch (scrut_42) {
                                                case INT64_C(1): {
                                                    struct mak__a *arena_obj_47 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
                                                    if (!arena_obj_47) {
                                                        mak_ir_recover("ADT arena exhausted");
                                                        arena_obj_47 = &mak_ir_fallback_obj__a;
                                                     } else {
                                                        (arena_obj_47)->tag = mak_ir_tag_1;
                                                        (arena_obj_47)->v.v1.f0 = mak_local_bm_20;
                                                        (arena_obj_47)->v.v1.f1 = mak_local_bn_21;
                                                    }
                                                    int64_t arg_48 = (int64_t)((uintptr_t)(arena_obj_47));
                                                    mak_ir_fn_6(arg_48, INT64_C(0), (struct mak__d *)(arena_case_40));
                                                    break;
                                                }
                                                default: {
                                                    *(arena_case_40) = mak_ir_ctor_obj_4;
                                                    break;
                                                }
                                            }
                                        }
                                        else {
                                            mak_ir_recover("ADT case had no matching arm");
                                            /* deterministic ADT default already initialized */
                                        }
                                    }
                                    struct mak__d * scrut_39 = arena_case_40;
                                    *(arena_case_34) = *(&mak_ir_fallback_obj__f);
                                    if ((scrut_39 != NULL && (scrut_39)->tag == mak_ir_tag_4)) {
                                        *(arena_case_34) = mak_ir_ctor_obj_7;
                                    }
                                    else if ((scrut_39 != NULL && (scrut_39)->tag == mak_ir_tag_5)) {
                                        int64_t mak_local_bp_22 = (scrut_39)->v.v5.f0;
                                        int64_t mak_local_bq_23 = (scrut_39)->v.v5.f1;
                                        (arena_case_34)->tag = mak_ir_tag_6;
                                        (arena_case_34)->v.v6.f0 = mak_local_bp_22;
                                        (arena_case_34)->v.v6.f1 = mak_local_bq_23;
                                    }
                                    else {
                                        mak_ir_recover("ADT case had no matching arm");
                                        /* deterministic ADT default already initialized */
                                    }
                                    break;
                                }
                            }
                        }
                        else {
                            mak_ir_recover("ADT case had no matching arm");
                            /* deterministic ADT default already initialized */
                        }
                    }
                    struct mak__f * scrut_33 = arena_case_34;
                    *(out) = *(&mak_ir_fallback_obj__f);
                    if ((scrut_33 != NULL && (scrut_33)->tag == mak_ir_tag_7)) {
                        *(out) = mak_ir_ctor_obj_7;
                    }
                    else if ((scrut_33 != NULL && (scrut_33)->tag == mak_ir_tag_6)) {
                        int64_t mak_local_br_24 = (scrut_33)->v.v6.f0;
                        int64_t mak_local_bs_25 = (scrut_33)->v.v6.f1;
                        int64_t scrut_49 = mak_ir_fn_2(mak_local_bs_25);
                        *(out) = *(&mak_ir_fallback_obj__f);
                        if (0) {
                            int64_t mak_local_bt_26 = ((struct mak__a *)((uintptr_t)(scrut_49)))->v.v1.f0;
                            struct mak__a * mak_local_bv_27 = ((struct mak__a *)((uintptr_t)(scrut_49)))->v.v1.f1;
                            *(out) = *(&mak_ir_fallback_obj__f);
                            switch (mak_local_bt_26) {
                                case INT64_C(94): {
                                    struct mak__f *arena_call_51 = (struct mak__f *)mak_ir_arena_alloc(sizeof(struct mak__f), _Alignof(struct mak__f));
                                    if (!arena_call_51) {
                                        mak_ir_recover("ADT arena exhausted");
                                        arena_call_51 = &mak_ir_fallback_obj__f;
                                     } else {
                                        mak_ir_fn_8((int64_t)((uintptr_t)(mak_local_bv_27)), (struct mak__f *)(arena_call_51));
                                    }
                                    struct mak__f * scrut_50 = arena_call_51;
                                    *(out) = *(&mak_ir_fallback_obj__f);
                                    if ((scrut_50 != NULL && (scrut_50)->tag == mak_ir_tag_7)) {
                                        *(out) = mak_ir_ctor_obj_7;
                                    }
                                    else if ((scrut_50 != NULL && (scrut_50)->tag == mak_ir_tag_6)) {
                                        int64_t mak_local_bw_28 = (scrut_50)->v.v6.f0;
                                        int64_t mak_local_bx_29 = (scrut_50)->v.v6.f1;
                                        int32_t scrut_52 = ((int32_t)(mak_local_bw_28) < (int32_t)(INT64_C(0)));
                                        *(out) = *(&mak_ir_fallback_obj__f);
                                        switch (scrut_52) {
                                            case INT64_C(1): {
                                                *(out) = mak_ir_ctor_obj_7;
                                                break;
                                            }
                                            default: {
                                                (out)->tag = mak_ir_tag_6;
                                                (out)->v.v6.f0 = mak_ir_fn_7(mak_local_br_24, mak_local_bw_28);
                                                (out)->v.v6.f1 = mak_local_bx_29;
                                                break;
                                            }
                                        }
                                    }
                                    else {
                                        mak_ir_recover("ADT case had no matching arm");
                                        /* deterministic ADT default already initialized */
                                    }
                                    break;
                                }
                                default: {
                                    (out)->tag = mak_ir_tag_6;
                                    (out)->v.v6.f0 = mak_local_br_24;
                                    (out)->v.v6.f1 = mak_local_bs_25;
                                    break;
                                }
                            }
                        }
                        else if (0) {
                            (out)->tag = mak_ir_tag_6;
                            (out)->v.v6.f0 = mak_local_br_24;
                            (out)->v.v6.f1 = mak_local_bs_25;
                        }
                        else {
                            mak_ir_recover("ADT case had no matching arm");
                            /* deterministic ADT default already initialized */
                        }
                    }
                    else {
                        mak_ir_recover("ADT case had no matching arm");
                        /* deterministic ADT default already initialized */
                    }
                    break;
                }
            }
        }
        else {
            mak_ir_recover("ADT case had no matching arm");
            /* deterministic ADT default already initialized */
        }
        return;
    }
}

static void mak_ir_fn_9(int64_t a0, int64_t a1, struct mak__f *out) {
    for (;;) {
        int64_t scrut_53 = mak_ir_fn_2(a1);
        *(out) = *(&mak_ir_fallback_obj__f);
        if (0) {
            (out)->tag = mak_ir_tag_6;
            (out)->v.v6.f0 = a0;
            int64_t arg_54 = (int64_t)((uintptr_t)(&mak_ir_ctor_obj_0));
            (out)->v.v6.f1 = arg_54;
        }
        else if (0) {
            int64_t mak_local_da_30 = ((struct mak__a *)((uintptr_t)(scrut_53)))->v.v1.f0;
            struct mak__a * mak_local_db_31 = ((struct mak__a *)((uintptr_t)(scrut_53)))->v.v1.f1;
            *(out) = *(&mak_ir_fallback_obj__f);
            switch (mak_local_da_30) {
                case INT64_C(42): {
                    struct mak__f *arena_call_56 = (struct mak__f *)mak_ir_arena_alloc(sizeof(struct mak__f), _Alignof(struct mak__f));
                    if (!arena_call_56) {
                        mak_ir_recover("ADT arena exhausted");
                        arena_call_56 = &mak_ir_fallback_obj__f;
                     } else {
                        mak_ir_fn_8((int64_t)((uintptr_t)(mak_local_db_31)), (struct mak__f *)(arena_call_56));
                    }
                    struct mak__f * scrut_55 = arena_call_56;
                    *(out) = *(&mak_ir_fallback_obj__f);
                    if ((scrut_55 != NULL && (scrut_55)->tag == mak_ir_tag_7)) {
                        *(out) = mak_ir_ctor_obj_7;
                    }
                    else if ((scrut_55 != NULL && (scrut_55)->tag == mak_ir_tag_6)) {
                        int64_t mak_local_dd_32 = (scrut_55)->v.v6.f0;
                        int64_t mak_local_df_33 = (scrut_55)->v.v6.f1;
                        int64_t tailarg_57 = (a0 * mak_local_dd_32);
                        int64_t tailarg_58 = mak_local_df_33;
                        a0 = tailarg_57;
                        a1 = tailarg_58;
                        continue;
                    }
                    else {
                        mak_ir_recover("ADT case had no matching arm");
                        /* deterministic ADT default already initialized */
                    }
                    break;
                }
                case INT64_C(47): {
                    struct mak__f *arena_call_60 = (struct mak__f *)mak_ir_arena_alloc(sizeof(struct mak__f), _Alignof(struct mak__f));
                    if (!arena_call_60) {
                        mak_ir_recover("ADT arena exhausted");
                        arena_call_60 = &mak_ir_fallback_obj__f;
                     } else {
                        mak_ir_fn_8((int64_t)((uintptr_t)(mak_local_db_31)), (struct mak__f *)(arena_call_60));
                    }
                    struct mak__f * scrut_59 = arena_call_60;
                    *(out) = *(&mak_ir_fallback_obj__f);
                    if ((scrut_59 != NULL && (scrut_59)->tag == mak_ir_tag_7)) {
                        *(out) = mak_ir_ctor_obj_7;
                    }
                    else if ((scrut_59 != NULL && (scrut_59)->tag == mak_ir_tag_6)) {
                        int64_t mak_local_dg_34 = (scrut_59)->v.v6.f0;
                        int64_t mak_local_dh_35 = (scrut_59)->v.v6.f1;
                        int32_t scrut_61 = ((int32_t)(mak_local_dg_34) == (int32_t)(INT64_C(0)));
                        *(out) = *(&mak_ir_fallback_obj__f);
                        switch (scrut_61) {
                            case INT64_C(1): {
                                *(out) = mak_ir_ctor_obj_7;
                                break;
                            }
                            default: {
                                int64_t tailarg_62 = (a0 / mak_local_dg_34);
                                int64_t tailarg_63 = mak_local_dh_35;
                                a0 = tailarg_62;
                                a1 = tailarg_63;
                                continue;
                            }
                        }
                    }
                    else {
                        mak_ir_recover("ADT case had no matching arm");
                        /* deterministic ADT default already initialized */
                    }
                    break;
                }
                case INT64_C(37): {
                    struct mak__f *arena_call_65 = (struct mak__f *)mak_ir_arena_alloc(sizeof(struct mak__f), _Alignof(struct mak__f));
                    if (!arena_call_65) {
                        mak_ir_recover("ADT arena exhausted");
                        arena_call_65 = &mak_ir_fallback_obj__f;
                     } else {
                        mak_ir_fn_8((int64_t)((uintptr_t)(mak_local_db_31)), (struct mak__f *)(arena_call_65));
                    }
                    struct mak__f * scrut_64 = arena_call_65;
                    *(out) = *(&mak_ir_fallback_obj__f);
                    if ((scrut_64 != NULL && (scrut_64)->tag == mak_ir_tag_7)) {
                        *(out) = mak_ir_ctor_obj_7;
                    }
                    else if ((scrut_64 != NULL && (scrut_64)->tag == mak_ir_tag_6)) {
                        int64_t mak_local_dj_36 = (scrut_64)->v.v6.f0;
                        int64_t mak_local_dk_37 = (scrut_64)->v.v6.f1;
                        int32_t scrut_66 = ((int32_t)(mak_local_dj_36) == (int32_t)(INT64_C(0)));
                        *(out) = *(&mak_ir_fallback_obj__f);
                        switch (scrut_66) {
                            case INT64_C(1): {
                                *(out) = mak_ir_ctor_obj_7;
                                break;
                            }
                            default: {
                                int64_t tailarg_67 = (a0 % mak_local_dj_36);
                                int64_t tailarg_68 = mak_local_dk_37;
                                a0 = tailarg_67;
                                a1 = tailarg_68;
                                continue;
                            }
                        }
                    }
                    else {
                        mak_ir_recover("ADT case had no matching arm");
                        /* deterministic ADT default already initialized */
                    }
                    break;
                }
                default: {
                    (out)->tag = mak_ir_tag_6;
                    (out)->v.v6.f0 = a0;
                    (out)->v.v6.f1 = a1;
                    break;
                }
            }
        }
        else {
            mak_ir_recover("ADT case had no matching arm");
            /* deterministic ADT default already initialized */
        }
        return;
    }
}

static void mak_ir_fn_10(int64_t a0, struct mak__f *out) {
    for (;;) {
        struct mak__f *arena_call_70 = (struct mak__f *)mak_ir_arena_alloc(sizeof(struct mak__f), _Alignof(struct mak__f));
        if (!arena_call_70) {
            mak_ir_recover("ADT arena exhausted");
            arena_call_70 = &mak_ir_fallback_obj__f;
         } else {
            mak_ir_fn_8(a0, (struct mak__f *)(arena_call_70));
        }
        struct mak__f * scrut_69 = arena_call_70;
        *(out) = *(&mak_ir_fallback_obj__f);
        if ((scrut_69 != NULL && (scrut_69)->tag == mak_ir_tag_7)) {
            *(out) = mak_ir_ctor_obj_7;
        }
        else if ((scrut_69 != NULL && (scrut_69)->tag == mak_ir_tag_6)) {
            int64_t mak_local_dm_38 = (scrut_69)->v.v6.f0;
            int64_t mak_local_dn_39 = (scrut_69)->v.v6.f1;
            mak_ir_fn_9(mak_local_dm_38, mak_local_dn_39, (struct mak__f *)(out));
        }
        else {
            mak_ir_recover("ADT case had no matching arm");
            /* deterministic ADT default already initialized */
        }
        return;
    }
}

static void mak_ir_fn_11(int64_t a0, int64_t a1, struct mak__f *out) {
    for (;;) {
        int64_t scrut_71 = mak_ir_fn_2(a1);
        *(out) = *(&mak_ir_fallback_obj__f);
        if (0) {
            (out)->tag = mak_ir_tag_6;
            (out)->v.v6.f0 = a1;
            int64_t arg_72 = (int64_t)((uintptr_t)(&mak_ir_ctor_obj_0));
            (out)->v.v6.f1 = arg_72;
        }
        else if (0) {
            int64_t mak_local_dr_40 = ((struct mak__a *)((uintptr_t)(scrut_71)))->v.v1.f0;
            struct mak__a * mak_local_ds_41 = ((struct mak__a *)((uintptr_t)(scrut_71)))->v.v1.f1;
            *(out) = *(&mak_ir_fallback_obj__f);
            switch (mak_local_dr_40) {
                case INT64_C(43): {
                    struct mak__f *arena_call_74 = (struct mak__f *)mak_ir_arena_alloc(sizeof(struct mak__f), _Alignof(struct mak__f));
                    if (!arena_call_74) {
                        mak_ir_recover("ADT arena exhausted");
                        arena_call_74 = &mak_ir_fallback_obj__f;
                     } else {
                        mak_ir_fn_10((int64_t)((uintptr_t)(mak_local_ds_41)), (struct mak__f *)(arena_call_74));
                    }
                    struct mak__f * scrut_73 = arena_call_74;
                    *(out) = *(&mak_ir_fallback_obj__f);
                    if ((scrut_73 != NULL && (scrut_73)->tag == mak_ir_tag_7)) {
                        *(out) = mak_ir_ctor_obj_7;
                    }
                    else if ((scrut_73 != NULL && (scrut_73)->tag == mak_ir_tag_6)) {
                        int64_t mak_local_dt_42 = (scrut_73)->v.v6.f0;
                        int64_t mak_local_dv_43 = (scrut_73)->v.v6.f1;
                        int64_t tailarg_75 = (a1 + mak_local_dt_42);
                        int64_t tailarg_76 = mak_local_dv_43;
                        a0 = tailarg_75;
                        a1 = tailarg_76;
                        continue;
                    }
                    else {
                        mak_ir_recover("ADT case had no matching arm");
                        /* deterministic ADT default already initialized */
                    }
                    break;
                }
                case INT64_C(45): {
                    struct mak__f *arena_call_78 = (struct mak__f *)mak_ir_arena_alloc(sizeof(struct mak__f), _Alignof(struct mak__f));
                    if (!arena_call_78) {
                        mak_ir_recover("ADT arena exhausted");
                        arena_call_78 = &mak_ir_fallback_obj__f;
                     } else {
                        mak_ir_fn_10((int64_t)((uintptr_t)(mak_local_ds_41)), (struct mak__f *)(arena_call_78));
                    }
                    struct mak__f * scrut_77 = arena_call_78;
                    *(out) = *(&mak_ir_fallback_obj__f);
                    if ((scrut_77 != NULL && (scrut_77)->tag == mak_ir_tag_7)) {
                        *(out) = mak_ir_ctor_obj_7;
                    }
                    else if ((scrut_77 != NULL && (scrut_77)->tag == mak_ir_tag_6)) {
                        int64_t mak_local_dw_44 = (scrut_77)->v.v6.f0;
                        int64_t mak_local_dx_45 = (scrut_77)->v.v6.f1;
                        int64_t tailarg_79 = (a1 - mak_local_dw_44);
                        int64_t tailarg_80 = mak_local_dx_45;
                        a0 = tailarg_79;
                        a1 = tailarg_80;
                        continue;
                    }
                    else {
                        mak_ir_recover("ADT case had no matching arm");
                        /* deterministic ADT default already initialized */
                    }
                    break;
                }
                default: {
                    (out)->tag = mak_ir_tag_6;
                    (out)->v.v6.f0 = a1;
                    (out)->v.v6.f1 = a1;
                    break;
                }
            }
        }
        else {
            mak_ir_recover("ADT case had no matching arm");
            /* deterministic ADT default already initialized */
        }
        return;
    }
}

static void mak_ir_fn_12(int64_t a0, struct mak__f *out) {
    for (;;) {
        struct mak__f *arena_call_82 = (struct mak__f *)mak_ir_arena_alloc(sizeof(struct mak__f), _Alignof(struct mak__f));
        if (!arena_call_82) {
            mak_ir_recover("ADT arena exhausted");
            arena_call_82 = &mak_ir_fallback_obj__f;
         } else {
            mak_ir_fn_10(a0, (struct mak__f *)(arena_call_82));
        }
        struct mak__f * scrut_81 = arena_call_82;
        *(out) = *(&mak_ir_fallback_obj__f);
        if ((scrut_81 != NULL && (scrut_81)->tag == mak_ir_tag_7)) {
            *(out) = mak_ir_ctor_obj_7;
        }
        else if ((scrut_81 != NULL && (scrut_81)->tag == mak_ir_tag_6)) {
            int64_t mak_local_dz_46 = (scrut_81)->v.v6.f0;
            int64_t mak_local_fa_47 = (scrut_81)->v.v6.f1;
            mak_ir_fn_11(mak_local_dz_46, mak_local_fa_47, (struct mak__f *)(out));
        }
        else {
            mak_ir_recover("ADT case had no matching arm");
            /* deterministic ADT default already initialized */
        }
        return;
    }
}

static int32_t mak_ir_fn_13(void) {
    for (;;) {
        mak_ir_ext_3((int32_t)(INT64_C(1)), (const void *)(">>> "), mak_ir_ext_2((const void *)(">>> ")));
        int32_t mak_local_fb_48 = mak_ir_ext_0();
        struct mak__b *arena_case_84 = (struct mak__b *)mak_ir_arena_alloc(sizeof(struct mak__b), _Alignof(struct mak__b));
        if (!arena_case_84) {
            mak_ir_recover("ADT arena exhausted");
            arena_case_84 = &mak_ir_fallback_obj__b;
         } else {
            int32_t scrut_85 = (mak_local_fb_48 < (int32_t)(INT64_C(0)));
            *(arena_case_84) = *(&mak_ir_fallback_obj__b);
            switch (scrut_85) {
                case INT64_C(1): {
                    *(arena_case_84) = mak_ir_ctor_obj_2;
                    break;
                }
                default: {
                    int32_t scrut_86 = (mak_local_fb_48 == (int32_t)(INT64_C(10)));
                    *(arena_case_84) = *(&mak_ir_fallback_obj__b);
                    switch (scrut_86) {
                        case INT64_C(1): {
                            (arena_case_84)->tag = mak_ir_tag_3;
                            (arena_case_84)->v.v3.f0 = &mak_ir_ctor_obj_0;
                            break;
                        }
                        default: {
                            struct mak__a *arena_obj_87 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
                            if (!arena_obj_87) {
                                mak_ir_recover("ADT arena exhausted");
                                arena_obj_87 = &mak_ir_fallback_obj__a;
                             } else {
                                (arena_obj_87)->tag = mak_ir_tag_1;
                                (arena_obj_87)->v.v1.f0 = (int64_t)(mak_local_fb_48);
                                (arena_obj_87)->v.v1.f1 = &mak_ir_ctor_obj_0;
                            }
                            mak_ir_fn_1(arena_obj_87, (struct mak__b *)(arena_case_84));
                            break;
                        }
                    }
                    break;
                }
            }
        }
        struct mak__b * scrut_83 = arena_case_84;
        if ((scrut_83 != NULL && (scrut_83)->tag == mak_ir_tag_2)) {
            return mak_ir_ext_1((int32_t)(INT64_C(10)));
        }
        else if ((scrut_83 != NULL && (scrut_83)->tag == mak_ir_tag_3)) {
            struct mak__a * mak_local_fd_49 = (scrut_83)->v.v3.f0;
            int64_t discard_88 = 0;
            struct mak__f *arena_call_90 = (struct mak__f *)mak_ir_arena_alloc(sizeof(struct mak__f), _Alignof(struct mak__f));
            if (!arena_call_90) {
                mak_ir_recover("ADT arena exhausted");
                arena_call_90 = &mak_ir_fallback_obj__f;
             } else {
                mak_ir_fn_12((int64_t)((uintptr_t)(mak_local_fd_49)), (struct mak__f *)(arena_call_90));
            }
            struct mak__f * scrut_89 = arena_call_90;
            discard_88 = 0;
            if ((scrut_89 != NULL && (scrut_89)->tag == mak_ir_tag_7)) {
                discard_88 = mak_ir_fn_3("Error: invalid expression\n");
            }
            else if ((scrut_89 != NULL && (scrut_89)->tag == mak_ir_tag_6)) {
                int64_t mak_local_ff_50 = (scrut_89)->v.v6.f0;
                int64_t mak_local_fg_51 = (scrut_89)->v.v6.f1;
                int64_t case_91 = 0;
                int64_t scrut_92 = mak_ir_fn_2(mak_local_fg_51);
                case_91 = 0;
                if (0) {
                    int64_t discard_93 = 0;
                    int32_t scrut_94 = ((int32_t)(mak_local_ff_50) == (int32_t)(INT64_C(0)));
                    discard_93 = 0;
                    switch (scrut_94) {
                        case INT64_C(1): {
                            discard_93 = mak_ir_ext_3((int32_t)(INT64_C(1)), (const void *)("0"), mak_ir_ext_2((const void *)("0")));
                            break;
                        }
                        default: {
                            int64_t case_95 = 0;
                            int32_t scrut_96 = ((int32_t)(mak_local_ff_50) < (int32_t)(INT64_C(0)));
                            case_95 = 0;
                            switch (scrut_96) {
                                case INT64_C(1): {
                                    mak_ir_ext_1((int32_t)(INT64_C(45)));
                                    struct mak__a *arena_call_97 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
                                    if (!arena_call_97) {
                                        mak_ir_recover("ADT arena exhausted");
                                        arena_call_97 = &mak_ir_fallback_obj__a;
                                     } else {
                                        mak_ir_fn_4((INT64_C(0) - mak_local_ff_50), &mak_ir_ctor_obj_0, (struct mak__a *)(arena_call_97));
                                    }
                                    case_95 = mak_ir_fn_5(arena_call_97);
                                    break;
                                }
                                default: {
                                    struct mak__a *arena_call_98 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
                                    if (!arena_call_98) {
                                        mak_ir_recover("ADT arena exhausted");
                                        arena_call_98 = &mak_ir_fallback_obj__a;
                                     } else {
                                        mak_ir_fn_4(mak_local_ff_50, &mak_ir_ctor_obj_0, (struct mak__a *)(arena_call_98));
                                    }
                                    case_95 = mak_ir_fn_5(arena_call_98);
                                    break;
                                }
                            }
                            discard_93 = case_95;
                            break;
                        }
                    }
                    case_91 = (int64_t)(mak_ir_ext_1((int32_t)(INT64_C(10))));
                }
                else if (1) {
                    case_91 = mak_ir_fn_3("Error: unexpected characters\n");
                }
                discard_88 = case_91;
            }
            else {
                mak_ir_recover("scalar case had no matching arm");
                /* deterministic scalar default already initialized */
            }
            continue;
        }
        else {
            mak_ir_recover("case had no matching arm");
            return 0;
        }
    }
}


int main(void) {
    mak_ir_fn_3("MAK Math Evaluator\n");
    mak_ir_fn_3("Operators: + - * / % ^ and parentheses\n");
    mak_ir_fn_3("Example: (12 + 3) * 4 - 2^3\n\n");
    mak_ir_fn_13();
    return 0;
}
