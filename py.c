#include <math.h>
#include <stdatomic.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int64_t mak_ir_i64_from_bits(uint64_t bits) { int64_t out; memcpy(&out, &bits, sizeof out); return out; }
int64_t mak_ir_i64_neg_wrap(int64_t a) { return mak_ir_i64_from_bits(UINT64_C(0) - (uint64_t)a); }
int64_t mak_ir_i64_add_wrap(int64_t a, int64_t b) { return mak_ir_i64_from_bits((uint64_t)a + (uint64_t)b); }
int64_t mak_ir_i64_sub_wrap(int64_t a, int64_t b) { return mak_ir_i64_from_bits((uint64_t)a - (uint64_t)b); }
int64_t mak_ir_i64_mul_wrap(int64_t a, int64_t b) { return mak_ir_i64_from_bits((uint64_t)a * (uint64_t)b); }
int64_t mak_ir_i64_div_wrap(int64_t a, int64_t b) { if (a == INT64_MIN && b == -1) return INT64_MIN; return a / b; }
int64_t mak_ir_i64_rem_wrap(int64_t a, int64_t b) { if (a == INT64_MIN && b == -1) return 0; return a % b; }
int64_t mak_ir_i64_shl_wrap(int64_t a, int64_t b) { return mak_ir_i64_from_bits((uint64_t)a << ((uint64_t)b & UINT64_C(63))); }
int64_t mak_ir_i64_shr(int64_t a, int64_t b) { uint64_t n = (uint64_t)b & UINT64_C(63); uint64_t bits; memcpy(&bits, &a, sizeof bits); if (n == 0) return a; return mak_ir_i64_from_bits((bits >> n) | (a < 0 ? (UINT64_MAX << (64 - n)) : UINT64_C(0))); }

enum {
    mak_ir_tag_0 = 1u,
    mak_ir_tag_1 = 2u,
    mak_ir_tag_2 = 3u,
    mak_ir_tag_3 = 4u,
    mak_ir_tag_4 = 5u,
    mak_ir_tag_5 = 6u,
    mak_ir_tag_6 = 7u,
    mak_ir_tag_7 = 8u,
    mak_ir_tag_8 = 9u,
    mak_ir_tag_9 = 10u,
    mak_ir_tag_10 = 11u,
    mak_ir_tag_11 = 12u,
};

struct mak__a {
    uint32_t tag;
    union {
        struct {
            int32_t f0;
            struct mak__a * f1;
        } v1;
    } v;
};

static struct mak__a mak_ir_ctor_obj_0 = { mak_ir_tag_0 };
static struct mak__a mak_ir_default_obj__a = { mak_ir_tag_0 };
struct mak__b {
    uint32_t tag;
    union {
        struct {
            struct mak__a * f0;
        } v3;
    } v;
};

static struct mak__b mak_ir_ctor_obj_2 = { mak_ir_tag_2 };
static struct mak__b mak_ir_default_obj__b = { mak_ir_tag_2 };
struct mak__c {
    uint32_t tag;
    union {
        struct {
            int64_t f0;
            struct mak__a * f1;
        } v5;
    } v;
};

static struct mak__c mak_ir_ctor_obj_4 = { mak_ir_tag_4 };
static struct mak__c mak_ir_default_obj__c = { mak_ir_tag_4 };
struct mak__d {
    uint32_t tag;
    union {
        struct {
            double f0;
            struct mak__a * f1;
        } v7;
    } v;
};

static struct mak__d mak_ir_ctor_obj_6 = { mak_ir_tag_6 };
static struct mak__d mak_ir_default_obj__d = { mak_ir_tag_6 };
struct mak__e {
    uint32_t tag;
    union {
        struct {
            int64_t f0;
            struct mak__a * f1;
        } v9;
    } v;
};

static struct mak__e mak_ir_ctor_obj_8 = { mak_ir_tag_8 };
static struct mak__e mak_ir_default_obj__e = { mak_ir_tag_8 };
struct mak__f {
    uint32_t tag;
    union {
        struct {
            double f0;
            struct mak__a * f1;
        } v10;
    } v;
};

static struct mak__f mak_ir_ctor_obj_11 = { mak_ir_tag_11 };
static struct mak__f mak_ir_default_obj__f = { mak_ir_tag_10 };
#ifndef MAK_ARENA_PAGE_BYTES
#define MAK_ARENA_PAGE_BYTES (64ULL * 1024ULL)
#endif
#ifndef MAK_ARENA_BYTES
#define MAK_ARENA_BYTES (256ULL * 1024ULL * 1024ULL)
#endif
#ifndef MAK_ARENA_MAX_BYTES
#define MAK_ARENA_MAX_BYTES MAK_ARENA_BYTES
#endif
#ifndef MAK_ARENA_HARDEN
#define MAK_ARENA_HARDEN 1
#endif
#ifndef MAK_ARENA_ABORT_ON_CORRUPTION
#define MAK_ARENA_ABORT_ON_CORRUPTION 1
#endif
#ifndef MAK_ARENA_QUARANTINE
#define MAK_ARENA_QUARANTINE 64u
#endif
#ifndef MAK_ARENA_MAX_ALIGN
#define MAK_ARENA_MAX_ALIGN (1ULL << 30)
#endif
#define MAK_IR_HEAP_MAGIC UINT64_C(0x4d414b4845415031)
#define MAK_IR_HEAP_LINK_MAGIC UINT64_C(0x4d414b4c494e4848)
#define MAK_IR_HEAP_TAIL UINT64_C(0x4d414b5441494c48)
struct mak_ir_heap_hdr { uint64_t magic; uint64_t cookie; size_t total; size_t payload_off; size_t size; size_t alignment; };
struct mak_ir_heap_link { uint64_t magic; struct mak_ir_heap_hdr *hdr; };
static inline uint64_t mak_ir_heap_cookie(const struct mak_ir_heap_hdr *h) { uintptr_t x=(uintptr_t)h; return MAK_IR_HEAP_MAGIC ^ (uint64_t)x ^ UINT64_C(0x9e3779b97f4a7c15); }
static inline void mak_ir_heap_fault(const char *reason) { if(MAK_ARENA_ABORT_ON_CORRUPTION) { fprintf(stderr, "[MAK:MEM] heap fault: %s\n", reason); abort(); } }
static inline void *mak_ir_heap_alloc(size_t size, size_t align) {
    if (size == 0) size = 1; if (align == 0) align = _Alignof(max_align_t); if ((align & (align-1)) != 0 || align > MAK_ARENA_MAX_ALIGN) return NULL;
    size_t base=sizeof(struct mak_ir_heap_hdr)+sizeof(struct mak_ir_heap_link); if(base>SIZE_MAX-(align-1)) return NULL; size_t pre=base+(align-1); if(size>SIZE_MAX-pre) return NULL; size_t total=pre+size; if(sizeof(uint64_t)>SIZE_MAX-total) return NULL; total+=sizeof(uint64_t);
    unsigned char *raw=(unsigned char *)malloc(total); if(!raw) return NULL; uintptr_t q=(uintptr_t)raw+sizeof(struct mak_ir_heap_hdr)+sizeof(struct mak_ir_heap_link); uintptr_t mask=(uintptr_t)align-1; if(q>UINTPTR_MAX-mask){free(raw);return NULL;} uintptr_t payload=(q+mask)&~mask; size_t off=(size_t)(payload-(uintptr_t)raw); if(off>total-size-sizeof(uint64_t)-sizeof(struct mak_ir_heap_link)){free(raw);return NULL;} struct mak_ir_heap_hdr *h=(struct mak_ir_heap_hdr *)(void *)raw; struct mak_ir_heap_link *ln=(struct mak_ir_heap_link *)(void *)(payload-sizeof(struct mak_ir_heap_link)); memset(h,0,sizeof(*h)); h->magic=MAK_IR_HEAP_MAGIC; h->total=total; h->payload_off=off; h->size=size; h->alignment=align; h->cookie=mak_ir_heap_cookie(h); ln->magic=MAK_IR_HEAP_LINK_MAGIC; ln->hdr=h; uint64_t *tail=(uint64_t *)(void *)(payload+size); *tail=MAK_IR_HEAP_TAIL ^ (uint64_t)(uintptr_t)h ^ (uint64_t)size; if(MAK_ARENA_HARDEN) memset((void *)payload,0,size); return (void *)payload;
}
static inline void mak_ir_heap_free(void *ptr) { if(!ptr) return; unsigned char *payload=(unsigned char *)ptr; if((uintptr_t)payload<sizeof(struct mak_ir_heap_link)){mak_ir_heap_fault("invalid heap pointer");return;} struct mak_ir_heap_link *ln=(struct mak_ir_heap_link *)(void *)(payload-sizeof(struct mak_ir_heap_link)); if(ln->magic!=MAK_IR_HEAP_LINK_MAGIC || !ln->hdr){mak_ir_heap_fault("invalid heap allocation link");return;} struct mak_ir_heap_hdr *h=ln->hdr; if(h->magic!=MAK_IR_HEAP_MAGIC || h->cookie!=mak_ir_heap_cookie(h) || h->payload_off<sizeof(struct mak_ir_heap_hdr)+sizeof(struct mak_ir_heap_link) || h->payload_off>h->total-h->size-sizeof(uint64_t) || h->alignment==0 || (h->alignment&(h->alignment-1))!=0){mak_ir_heap_fault("invalid heap header");return;} unsigned char *raw=payload-h->payload_off; if(raw!=((unsigned char *)h)){mak_ir_heap_fault("heap base mismatch");return;} uint64_t *tail=(uint64_t *)(void *)(payload+h->size); if(*tail!=(MAK_IR_HEAP_TAIL ^ (uint64_t)(uintptr_t)h ^ (uint64_t)h->size)){mak_ir_heap_fault("heap allocation canary corrupted");return;} if(MAK_ARENA_HARDEN) memset(payload,0xdd,h->size); h->magic=0; h->cookie=0; ln->magic=0; free(raw); }
#if MAK_ARENA_PAGE_BYTES < 4096
#error "MAK_ARENA_PAGE_BYTES must be at least 4096"
#endif
#if MAK_ARENA_MAX_BYTES < MAK_ARENA_PAGE_BYTES
#error "MAK_ARENA_MAX_BYTES must be >= MAK_ARENA_PAGE_BYTES"
#endif
#if MAK_ARENA_QUARANTINE < 1
#error "MAK_ARENA_QUARANTINE must be >= 1"
#endif
#define MAK_IR_MEM_PAGE_MAGIC UINT64_C(0x4d414b5041474531)
#define MAK_IR_MEM_HEAD_MAGIC UINT64_C(0x4d414b4845414431)
#define MAK_IR_MEM_LINK_MAGIC UINT64_C(0x4d414b4c494e4b31)
#define MAK_IR_MEM_TAIL_MAGIC UINT64_C(0x4d414b5441494c31)
#define MAK_IR_MEM_LIVE 1u
#define MAK_IR_MEM_FREE 2u
#define MAK_IR_MEM_QUARANTINED 3u
struct mak_ir_mem_page;
struct mak_ir_mem_hdr;
struct mak_ir_mem_link { uint64_t magic; struct mak_ir_mem_hdr *hdr; };
struct mak_ir_mem_hdr {
    uint64_t magic;
    uint64_t cookie;
    struct mak_ir_mem_page *page;
    size_t total;
    size_t payload_offset;
    size_t size;
    size_t capacity;
    size_t alignment;
    uint32_t state;
    uint32_t class_id;
    struct mak_ir_mem_hdr *prev;
    struct mak_ir_mem_hdr *next;
};
struct mak_ir_mem_page {
    uint64_t magic;
    uint64_t cookie;
    size_t capacity;
    size_t used;
    size_t blocks;
    size_t live;
    size_t free_blocks;
    int dedicated;
    struct mak_ir_mem_page *prev;
    struct mak_ir_mem_page *next;
    max_align_t alignment;
    unsigned char data[];
};
static struct mak_ir_mem_page *mak_ir_mem_pages = NULL;
static struct mak_ir_mem_page *mak_ir_mem_current = NULL;
static struct mak_ir_mem_hdr *mak_ir_mem_free_bins[64] = {0};
static struct mak_ir_mem_hdr *mak_ir_mem_quarantine[MAK_ARENA_QUARANTINE] = {0};
static size_t mak_ir_mem_q_head = 0;
static size_t mak_ir_mem_q_count = 0;
static size_t mak_ir_mem_committed = 0;
static size_t mak_ir_mem_live_bytes = 0;
static size_t mak_ir_mem_peak_bytes = 0;
static uint64_t mak_ir_mem_faults = 0;
static atomic_flag mak_ir_mem_lock = ATOMIC_FLAG_INIT;
static int mak_ir_mem_atexit_registered = 0;
static inline void mak_ir_mem_lock_enter(void) {
    while (atomic_flag_test_and_set_explicit(&mak_ir_mem_lock, memory_order_acquire)) { }
}
static inline void mak_ir_mem_lock_leave(void) {
    atomic_flag_clear_explicit(&mak_ir_mem_lock, memory_order_release);
}
static inline uint64_t mak_ir_mem_mix(uint64_t x) {
    x ^= x >> 30; x *= UINT64_C(0xbf58476d1ce4e5b9);
    x ^= x >> 27; x *= UINT64_C(0x94d049bb133111eb);
    x ^= x >> 31; return x;
}
static inline uint64_t mak_ir_mem_cookie_for(const void *p) {
    return mak_ir_mem_mix(UINT64_C(0x6d616b2d73656375) ^ (uint64_t)(uintptr_t)p ^ (UINT64_C(0x9e3779b97f4a7c15) * (uint64_t)sizeof(void *)));
}
static inline size_t mak_ir_mem_class(size_t n) {
    size_t k = 0; size_t x = n ? n : 1;
    while (x > 1 && k < 63) { x = (x + 1) >> 1; ++k; }
    return k;
}
static inline int mak_ir_mem_add_overflow(size_t a, size_t b, size_t *out) {
    if (b > SIZE_MAX - a) return 1;
    *out = a + b; return 0;
}
static inline uintptr_t mak_ir_mem_align_up_uintptr(uintptr_t x, size_t align, int *ok) {
    uintptr_t mask = (uintptr_t)(align - 1);
    if (x > UINTPTR_MAX - mask) { *ok = 0; return 0; }
    *ok = 1; return (x + mask) & ~mask;
}
static inline void mak_ir_mem_wipe(void *ptr, size_t n, unsigned char byte) {
    volatile unsigned char *p = (volatile unsigned char *)ptr;
    while (n--) *p++ = byte;
}
static inline void mak_ir_mem_fault(const char *reason) {
    ++mak_ir_mem_faults;
#if MAK_ARENA_ABORT_ON_CORRUPTION
    fprintf(stderr, "[MAK] arena integrity failure: %s\n", reason);
    abort();
#else
    (void)reason;
#endif
}
static inline uint64_t mak_ir_mem_tail(const struct mak_ir_mem_hdr *h) {
    return mak_ir_mem_mix(h->cookie ^ (uint64_t)h->size ^ UINT64_C(0xa55aa55af00df00d));
}
static inline unsigned char *mak_ir_mem_payload(const struct mak_ir_mem_hdr *h) {
    return h && h->page ? h->page->data + h->payload_offset : NULL;
}
static inline uint64_t *mak_ir_mem_tail_ptr(const struct mak_ir_mem_hdr *h) {
    return (uint64_t *)(void *)(mak_ir_mem_payload(h) + h->size);
}
static inline int mak_ir_mem_page_contains(const struct mak_ir_mem_page *p, const void *ptr) {
    if (!p || !ptr) return 0;
    if (p->used > p->capacity) return 0;
    uintptr_t lo = (uintptr_t)(const void *)p->data;
    uintptr_t hi = lo + p->used;
    uintptr_t x = (uintptr_t)ptr;
    return hi >= lo && x >= lo && x < hi;
}
static inline void mak_ir_mem_unlink_free(struct mak_ir_mem_hdr *h) {
    if (!h) return;
    size_t c = h->class_id < 64u ? (size_t)h->class_id : 63u;
    if (h->prev) h->prev->next = h->next; else mak_ir_mem_free_bins[c] = h->next;
    if (h->next) h->next->prev = h->prev;
    h->prev = NULL; h->next = NULL;
}
static inline void mak_ir_mem_link_free(struct mak_ir_mem_hdr *h) {
    size_t c = h->class_id < 64u ? (size_t)h->class_id : 63u;
    h->prev = NULL; h->next = mak_ir_mem_free_bins[c];
    if (h->next) h->next->prev = h;
    mak_ir_mem_free_bins[c] = h;
}
static inline int mak_ir_mem_header_valid(const struct mak_ir_mem_hdr *h) {
    if (!h || h->magic != MAK_IR_MEM_HEAD_MAGIC || !h->page) return 0;
    if (h->cookie != mak_ir_mem_cookie_for(h)) return 0;
    if (h->class_id >= 64u || h->capacity == 0 || h->payload_offset < sizeof(struct mak_ir_mem_hdr) + sizeof(struct mak_ir_mem_link)) return 0;
    if (h->payload_offset > h->page->capacity || h->size > h->page->capacity - h->payload_offset) return 0;
    if (h->alignment == 0 || (h->alignment & (h->alignment - 1)) != 0) return 0;
    if (h->size > h->capacity) return 0;
    return 1;
}
static inline int mak_ir_mem_canary_valid(const struct mak_ir_mem_hdr *h) {
    if (!mak_ir_mem_header_valid(h)) return 0;
    return *mak_ir_mem_tail_ptr(h) == mak_ir_mem_tail(h);
}
static void mak_ir_mem_reclaim_empty_locked(void) {
    struct mak_ir_mem_page *p = mak_ir_mem_pages;
    while (p) {
        struct mak_ir_mem_page *next = p->next;
        int clean = 1;
        if (p->magic != MAK_IR_MEM_PAGE_MAGIC || p->used > p->capacity) { p = next; continue; }
        if (p->live == 0 && p->free_blocks == p->blocks) {
            size_t off = 0;
            while (off < p->used) {
                if (p->used - off < sizeof(struct mak_ir_mem_hdr)) { clean = 0; break; }
                struct mak_ir_mem_hdr *h = (struct mak_ir_mem_hdr *)(void *)(p->data + off);
                if (!mak_ir_mem_header_valid(h) || h->state != MAK_IR_MEM_FREE) { clean = 0; break; }
                mak_ir_mem_unlink_free(h);
                if (h->total == 0 || h->total > p->used - off) { clean = 0; break; }
                off += h->total;
            }
            if (!clean || off != p->used) { p = next; continue; }
            if (p->dedicated || p != mak_ir_mem_current) {
                if (p->prev) p->prev->next = p->next; else mak_ir_mem_pages = p->next;
                if (p->next) p->next->prev = p->prev;
                if (mak_ir_mem_current == p) mak_ir_mem_current = p->prev ? p->prev : p->next;
                if (mak_ir_mem_committed >= sizeof(*p) + p->capacity) mak_ir_mem_committed -= sizeof(*p) + p->capacity; else mak_ir_mem_committed = 0;
                free(p);
            } else {
                p->used = 0; p->blocks = 0; p->live = 0; p->free_blocks = 0;
            }
        }
        p = next;
    }
}
static inline struct mak_ir_mem_page *mak_ir_mem_page_new_locked(size_t cap, int dedicated) {
    size_t bytes;
    if (cap < MAK_ARENA_PAGE_BYTES) cap = MAK_ARENA_PAGE_BYTES;
    if (mak_ir_mem_add_overflow(sizeof(struct mak_ir_mem_page), cap, &bytes)) return NULL;
    if (bytes > MAK_ARENA_MAX_BYTES - (mak_ir_mem_committed < MAK_ARENA_MAX_BYTES ? mak_ir_mem_committed : MAK_ARENA_MAX_BYTES)) return NULL;
    struct mak_ir_mem_page *p = (struct mak_ir_mem_page *)malloc(bytes);
    if (!p) return NULL;
    memset(p, 0, sizeof(*p));
    p->magic = MAK_IR_MEM_PAGE_MAGIC;
    p->cookie = mak_ir_mem_cookie_for(p);
    p->capacity = cap; p->dedicated = dedicated;
    p->prev = NULL; p->next = mak_ir_mem_pages;
    if (p->next) p->next->prev = p;
    mak_ir_mem_pages = p; mak_ir_mem_current = p;
    mak_ir_mem_committed += bytes;
    if (mak_ir_mem_committed > mak_ir_mem_peak_bytes) mak_ir_mem_peak_bytes = mak_ir_mem_committed;
    return p;
}
static inline struct mak_ir_mem_hdr *mak_ir_mem_find_free_locked(size_t size, size_t align) {
    size_t c0 = mak_ir_mem_class(size);
    for (size_t c = c0; c < 64; ++c) {
        for (struct mak_ir_mem_hdr *h = mak_ir_mem_free_bins[c]; h; h = h->next) {
            if (h->state != MAK_IR_MEM_FREE || !mak_ir_mem_header_valid(h)) continue;
            if (!mak_ir_mem_canary_valid(h)) { mak_ir_mem_fault("free-block canary corrupted"); continue; }
            if (h->capacity < size || h->alignment < align) continue;
            return h;
        }
    }
    return NULL;
}
static inline void mak_ir_mem_make_live_locked(struct mak_ir_mem_hdr *h, size_t size, size_t align) {
    struct mak_ir_mem_page *p = h->page;
    if (h->state == MAK_IR_MEM_FREE && p->free_blocks) --p->free_blocks;
    h->state = MAK_IR_MEM_LIVE;
    h->size = size;
    h->alignment = h->alignment > align ? h->alignment : align;
    h->class_id = (uint32_t)mak_ir_mem_class(h->capacity);
    *mak_ir_mem_tail_ptr(h) = mak_ir_mem_tail(h);
    if (MAK_ARENA_HARDEN) memset(mak_ir_mem_payload(h), 0, h->capacity);
    ++p->live;
    mak_ir_mem_live_bytes += size;
}
static inline void mak_ir_mem_quarantine_push_locked(struct mak_ir_mem_hdr *h) {
    if (mak_ir_mem_q_count == MAK_ARENA_QUARANTINE) {
        struct mak_ir_mem_hdr *old = mak_ir_mem_quarantine[mak_ir_mem_q_head];
        mak_ir_mem_q_head = (mak_ir_mem_q_head + 1) % MAK_ARENA_QUARANTINE;
        --mak_ir_mem_q_count;
        if (old && old->state == MAK_IR_MEM_QUARANTINED) { old->state = MAK_IR_MEM_FREE; ++old->page->free_blocks; mak_ir_mem_link_free(old); }
    }
    size_t slot = (mak_ir_mem_q_head + mak_ir_mem_q_count) % MAK_ARENA_QUARANTINE;
    mak_ir_mem_quarantine[slot] = h;
    ++mak_ir_mem_q_count;
}
static inline void mak_ir_mem_flush_quarantine_locked(size_t count) {
    while (count && mak_ir_mem_q_count) {
        struct mak_ir_mem_hdr *h = mak_ir_mem_quarantine[mak_ir_mem_q_head];
        mak_ir_mem_quarantine[mak_ir_mem_q_head] = NULL;
        mak_ir_mem_q_head = (mak_ir_mem_q_head + 1) % MAK_ARENA_QUARANTINE;
        --mak_ir_mem_q_count; --count;
        if (h && h->state == MAK_IR_MEM_QUARANTINED) { h->state = MAK_IR_MEM_FREE; ++h->page->free_blocks; mak_ir_mem_link_free(h); }
    }
}
static void mak_ir_arena_shutdown(void);
static inline void mak_ir_arena_register_exit_locked(void) {
    if (!mak_ir_mem_atexit_registered) {
        if (atexit(mak_ir_arena_shutdown) == 0) mak_ir_mem_atexit_registered = 1;
    }
}
static inline void *mak_ir_arena_alloc(size_t size, size_t align) {
    if (size == 0) size = 1;
    if (align == 0) align = _Alignof(max_align_t);
    if ((align & (align - 1)) != 0 || align > MAK_ARENA_MAX_ALIGN) return NULL;
    mak_ir_mem_lock_enter();
    mak_ir_arena_register_exit_locked();
    struct mak_ir_mem_hdr *reuse = mak_ir_mem_find_free_locked(size, align);
    if (reuse) {
        mak_ir_mem_unlink_free(reuse);
        mak_ir_mem_make_live_locked(reuse, size, align);
        void *ret = (void *)mak_ir_mem_payload(reuse);
        mak_ir_mem_lock_leave();
        return ret;
    }
    size_t overhead = sizeof(struct mak_ir_mem_hdr) + sizeof(struct mak_ir_mem_link);
    size_t needed = overhead;
    if (align - 1 > SIZE_MAX - needed) { mak_ir_mem_lock_leave(); return NULL; }
    needed += align - 1;
    if (size > SIZE_MAX - needed) { mak_ir_mem_lock_leave(); return NULL; }
    needed += size;
    if (sizeof(uint64_t) > SIZE_MAX - needed) { mak_ir_mem_lock_leave(); return NULL; }
    needed += sizeof(uint64_t);
    size_t page_cap = MAK_ARENA_PAGE_BYTES;
    int dedicated = (needed > MAK_ARENA_PAGE_BYTES / 2u);
    if (dedicated) {
        page_cap = needed;
        if (page_cap < MAK_ARENA_PAGE_BYTES) page_cap = MAK_ARENA_PAGE_BYTES;
    }
    struct mak_ir_mem_page *p = mak_ir_mem_current;
    if (!dedicated && p && p->magic == MAK_IR_MEM_PAGE_MAGIC) {
        size_t candidate = p->used;
        uintptr_t raw = (uintptr_t)(void *)p->data + candidate + overhead;
        int ok = 1;
        uintptr_t payload = mak_ir_mem_align_up_uintptr(raw, align, &ok);
        if (!ok || payload < (uintptr_t)(void *)p->data) { p = NULL; }
        else {
            uintptr_t page_base = (uintptr_t)(void *)p->data;
            size_t payload_off = (size_t)(payload - page_base);
            size_t end = payload_off;
            if (size > SIZE_MAX - end) p = NULL;
            else { end += size; if (sizeof(uint64_t) > SIZE_MAX - end) p = NULL; else end += sizeof(uint64_t); }
            size_t next_used = end;
            size_t maxa = _Alignof(max_align_t);
            if (maxa > 1 && next_used <= SIZE_MAX - (maxa - 1)) next_used = (next_used + (maxa - 1)) & ~(maxa - 1); else if (maxa > 1) p = NULL;
            if (p && next_used <= p->capacity) {
                size_t off = p->used;
                struct mak_ir_mem_hdr *h = (struct mak_ir_mem_hdr *)(void *)(p->data + off);
                memset(h, 0, sizeof(*h));
                h->magic = MAK_IR_MEM_HEAD_MAGIC; h->cookie = mak_ir_mem_cookie_for(h); h->page = p;
                h->total = next_used - off; h->payload_offset = payload_off; h->size = size; h->capacity = size; h->alignment = align; h->state = MAK_IR_MEM_LIVE; h->class_id = (uint32_t)mak_ir_mem_class(h->capacity);
                struct mak_ir_mem_link *ln = (struct mak_ir_mem_link *)(void *)(p->data + payload_off - sizeof(struct mak_ir_mem_link));
                ln->magic = MAK_IR_MEM_LINK_MAGIC; ln->hdr = h;
                *mak_ir_mem_tail_ptr(h) = mak_ir_mem_tail(h);
                if (MAK_ARENA_HARDEN) memset(mak_ir_mem_payload(h), 0, h->capacity);
                p->used = next_used; ++p->blocks; ++p->live;
                mak_ir_mem_live_bytes += size;
                void *ret = (void *)mak_ir_mem_payload(h);
                mak_ir_mem_lock_leave();
                return ret;
            }
        }
    }
    if (mak_ir_mem_committed > MAK_ARENA_MAX_BYTES || page_cap > MAK_ARENA_MAX_BYTES - mak_ir_mem_committed) {
        mak_ir_mem_flush_quarantine_locked(MAK_ARENA_QUARANTINE);
        mak_ir_mem_reclaim_empty_locked();
    }
    if (mak_ir_mem_committed > MAK_ARENA_MAX_BYTES || page_cap > MAK_ARENA_MAX_BYTES - mak_ir_mem_committed) {
        mak_ir_mem_lock_leave(); return NULL;
    }
    p = mak_ir_mem_page_new_locked(page_cap, dedicated);
    if (!p) { mak_ir_mem_lock_leave(); return NULL; }
    uintptr_t raw = (uintptr_t)(void *)p->data + overhead;
    int ok = 1;
    uintptr_t payload = mak_ir_mem_align_up_uintptr(raw, align, &ok);
    if (!ok || payload < (uintptr_t)(void *)p->data) { mak_ir_mem_lock_leave(); return NULL; }
    size_t payload_off = (size_t)(payload - (uintptr_t)(void *)p->data);
    size_t end = payload_off;
    if (size > SIZE_MAX - end) { mak_ir_mem_lock_leave(); return NULL; }
    end += size; if (sizeof(uint64_t) > SIZE_MAX - end) { mak_ir_mem_lock_leave(); return NULL; } end += sizeof(uint64_t);
    size_t maxa = _Alignof(max_align_t);
    size_t next_used = (maxa > 1) ? ((end + (maxa - 1)) & ~(maxa - 1)) : end;
    if (next_used > p->capacity) { mak_ir_mem_lock_leave(); return NULL; }
    struct mak_ir_mem_hdr *h = (struct mak_ir_mem_hdr *)(void *)p->data;
    memset(h, 0, sizeof(*h));
    h->magic = MAK_IR_MEM_HEAD_MAGIC; h->cookie = mak_ir_mem_cookie_for(h); h->page = p;
    h->total = next_used; h->payload_offset = payload_off; h->size = size; h->capacity = size; h->alignment = align; h->state = MAK_IR_MEM_LIVE; h->class_id = (uint32_t)mak_ir_mem_class(h->capacity);
    struct mak_ir_mem_link *ln = (struct mak_ir_mem_link *)(void *)(p->data + payload_off - sizeof(struct mak_ir_mem_link));
    ln->magic = MAK_IR_MEM_LINK_MAGIC; ln->hdr = h;
    *mak_ir_mem_tail_ptr(h) = mak_ir_mem_tail(h);
    if (MAK_ARENA_HARDEN) memset(mak_ir_mem_payload(h), 0, h->capacity);
    p->used = next_used; ++p->blocks; ++p->live;
    mak_ir_mem_live_bytes += size;
    void *ret = (void *)mak_ir_mem_payload(h);
    mak_ir_mem_lock_leave(); return ret;
}
static inline void mak_ir_arena_free(void *ptr) {
    if (!ptr) return;
    mak_ir_mem_lock_enter();
    struct mak_ir_mem_page *p = NULL;
    for (struct mak_ir_mem_page *it = mak_ir_mem_pages; it; it = it->next) {
        if (it->magic == MAK_IR_MEM_PAGE_MAGIC && mak_ir_mem_page_contains(it, ptr)) { p = it; break; }
    }
    if (!p) { mak_ir_mem_lock_leave(); mak_ir_mem_fault("free of non-arena pointer"); return; }
    uintptr_t x = (uintptr_t)ptr;
    uintptr_t base = (uintptr_t)(void *)p->data;
    if (x < base + sizeof(struct mak_ir_mem_link)) { mak_ir_mem_lock_leave(); mak_ir_mem_fault("invalid arena pointer"); return; }
    struct mak_ir_mem_link *ln = (struct mak_ir_mem_link *)(void *)((unsigned char *)ptr - sizeof(struct mak_ir_mem_link));
    if (ln->magic != MAK_IR_MEM_LINK_MAGIC || !ln->hdr) { mak_ir_mem_lock_leave(); mak_ir_mem_fault("invalid allocation link"); return; }
    struct mak_ir_mem_hdr *h = ln->hdr;
    if (h->page != p || !mak_ir_mem_header_valid(h) || mak_ir_mem_payload(h) != (unsigned char *)ptr) { mak_ir_mem_lock_leave(); mak_ir_mem_fault("allocation header mismatch"); return; }
    if (h->state != MAK_IR_MEM_LIVE) { mak_ir_mem_lock_leave(); mak_ir_mem_fault("double free or stale free"); return; }
    if (!mak_ir_mem_canary_valid(h)) { mak_ir_mem_lock_leave(); mak_ir_mem_fault("allocation canary corrupted"); return; }
    if (p->live == 0 || mak_ir_mem_live_bytes < h->size) { mak_ir_mem_lock_leave(); mak_ir_mem_fault("arena accounting corruption"); return; }
    --p->live;
    mak_ir_mem_live_bytes -= h->size;
    h->state = MAK_IR_MEM_QUARANTINED;
    if (MAK_ARENA_HARDEN) mak_ir_mem_wipe(mak_ir_mem_payload(h), h->capacity, 0xdd);
    mak_ir_mem_quarantine_push_locked(h);
    mak_ir_mem_reclaim_empty_locked();
    mak_ir_mem_lock_leave();
}
static inline void mak_ir_arena_trim(void) {
    mak_ir_mem_lock_enter();
    mak_ir_mem_flush_quarantine_locked(MAK_ARENA_QUARANTINE);
    mak_ir_mem_reclaim_empty_locked();
    mak_ir_mem_lock_leave();
}
static inline int mak_ir_arena_owns(const void *ptr) {
    if (!ptr) return 0;
    mak_ir_mem_lock_enter();
    int result = 0;
    for (struct mak_ir_mem_page *p = mak_ir_mem_pages; p; p = p->next) {
        if (p->magic == MAK_IR_MEM_PAGE_MAGIC && mak_ir_mem_page_contains(p, ptr)) {
            uintptr_t x = (uintptr_t)ptr;
            if (x >= (uintptr_t)(void *)p->data + sizeof(struct mak_ir_mem_link)) {
                struct mak_ir_mem_link *ln = (struct mak_ir_mem_link *)(void *)((unsigned char *)ptr - sizeof(struct mak_ir_mem_link));
                result = ln->magic == MAK_IR_MEM_LINK_MAGIC && ln->hdr && ln->hdr->page == p && ln->hdr->state == MAK_IR_MEM_LIVE && mak_ir_mem_header_valid(ln->hdr) && mak_ir_mem_canary_valid(ln->hdr);
            }
            break;
        }
    }
    mak_ir_mem_lock_leave(); return result;
}
static inline size_t mak_ir_arena_live_bytes(void) {
    mak_ir_mem_lock_enter(); size_t n = mak_ir_mem_live_bytes; mak_ir_mem_lock_leave(); return n;
}
static inline size_t mak_ir_arena_committed_bytes(void) {
    mak_ir_mem_lock_enter(); size_t n = mak_ir_mem_committed; mak_ir_mem_lock_leave(); return n;
}
static void mak_ir_arena_shutdown(void) {
    mak_ir_mem_lock_enter();
    for (size_t i = 0; i < MAK_ARENA_QUARANTINE; ++i) mak_ir_mem_quarantine[i] = NULL;
    mak_ir_mem_q_head = 0; mak_ir_mem_q_count = 0;
    for (struct mak_ir_mem_page *p = mak_ir_mem_pages; p;) {
        struct mak_ir_mem_page *next = p->next; free(p); p = next;
    }
    mak_ir_mem_pages = NULL; mak_ir_mem_current = NULL; mak_ir_mem_committed = 0; mak_ir_mem_live_bytes = 0;
    mak_ir_mem_peak_bytes = 0;
    memset(mak_ir_mem_free_bins, 0, sizeof mak_ir_mem_free_bins);
    mak_ir_mem_lock_leave();
}

extern int32_t mak_ir_ext_0(void) __asm__("getchar");
extern void mak_ir_ext_1(int32_t) __asm__("putchar");
extern int32_t mak_ir_ext_2(const void *, ...) __asm__("printf");
extern uint64_t mak_ir_ext_3(const void *) __asm__("strlen");
extern void mak_ir_ext_4(int32_t, const void *, uint64_t) __asm__("write");
extern double mak_ir_ext_5(double) __asm__("sqrt");
extern double mak_ir_ext_6(double) __asm__("cbrt");
extern double mak_ir_ext_7(double) __asm__("fabs");
extern double mak_ir_ext_8(double) __asm__("floor");
extern double mak_ir_ext_9(double) __asm__("ceil");
extern double mak_ir_ext_10(double) __asm__("trunc");
extern double mak_ir_ext_11(double) __asm__("round");
extern double mak_ir_ext_12(double, double) __asm__("fmod");
extern double mak_ir_ext_13(double, double) __asm__("remainder");
extern double mak_ir_ext_14(double, double) __asm__("pow");
extern double mak_ir_ext_15(double) __asm__("exp");
extern double mak_ir_ext_16(double) __asm__("exp2");
extern double mak_ir_ext_17(double) __asm__("expm1");
extern double mak_ir_ext_18(double) __asm__("log");
extern double mak_ir_ext_19(double) __asm__("log2");
extern double mak_ir_ext_20(double) __asm__("log10");
extern double mak_ir_ext_21(double) __asm__("log1p");
extern double mak_ir_ext_22(double) __asm__("sin");
extern double mak_ir_ext_23(double) __asm__("cos");
extern double mak_ir_ext_24(double) __asm__("tan");
extern double mak_ir_ext_25(double) __asm__("asin");
extern double mak_ir_ext_26(double) __asm__("acos");
extern double mak_ir_ext_27(double) __asm__("atan");
extern double mak_ir_ext_28(double, double) __asm__("atan2");
extern double mak_ir_ext_29(double) __asm__("sinh");
extern double mak_ir_ext_30(double) __asm__("cosh");
extern double mak_ir_ext_31(double) __asm__("tanh");
extern double mak_ir_ext_32(double) __asm__("asinh");
extern double mak_ir_ext_33(double) __asm__("acosh");
extern double mak_ir_ext_34(double) __asm__("atanh");
extern double mak_ir_ext_35(double) __asm__("tgamma");
extern double mak_ir_ext_36(double) __asm__("lgamma");
extern double mak_ir_ext_37(double, double) __asm__("hypot");
extern double mak_ir_ext_38(double) __asm__("erf");
extern double mak_ir_ext_39(double) __asm__("erfc");
extern double mak_ir_ext_40(double) __asm__("logb");
extern double mak_ir_ext_41(double, double) __asm__("copysign");
extern double mak_ir_ext_42(double, double) __asm__("fdim");
extern double mak_ir_ext_43(double, double) __asm__("fmax");
extern double mak_ir_ext_44(double, double) __asm__("fmin");
extern double mak_ir_ext_45(double, double, double) __asm__("fma");
extern double mak_ir_ext_46(double, double) __asm__("nextafter");

static inline void mak_ir_fn_0(struct mak__a * a0, struct mak__a * a1, struct mak__a *out);
static inline void mak_ir_fn_1(struct mak__a * a0, struct mak__b *out);
static inline void mak_ir_fn_2(struct mak__a * a0, struct mak__a *out);
static inline void mak_ir_fn_3(struct mak__a * a0, int64_t a1, struct mak__c *out);
static inline double mak_ir_fn_4(int64_t a0);
static inline void mak_ir_fn_5(struct mak__a * a0, int64_t a1, int64_t a2, struct mak__e *out);
static inline void mak_ir_fn_6(struct mak__a * a0, int64_t a1, struct mak__e *out);
static inline void mak_ir_fn_7(struct mak__a * a0, double a1, struct mak__d *out);
static inline void mak_ir_fn_8(struct mak__a * a0, double a1, double a2, struct mak__d *out);
static inline double mak_ir_fn_9(int64_t a0, double a1, double a2);
static inline void mak_ir_fn_10(double a0, struct mak__a * a1, struct mak__f *out);
static inline void mak_ir_fn_11(struct mak__a * a0, struct mak__f *out);
static inline void mak_ir_fn_12(struct mak__a * a0, struct mak__f *out);
static inline void mak_ir_fn_13(double a0, struct mak__a * a1, struct mak__f *out);
static inline void mak_ir_fn_14(struct mak__a * a0, struct mak__f *out);
static inline void mak_ir_fn_15(double a0, struct mak__a * a1, struct mak__f *out);
static inline void mak_ir_fn_16(struct mak__a * a0, struct mak__f *out);
static inline void mak_ir_fn_17(void);
static inline int64_t mak_ir_fn_18(int64_t a0);
static inline int64_t mak_ir_fn_19(int64_t a0);
static inline int64_t mak_ir_fn_20(int64_t a0);
static inline void mak_ir_fn_21(const char * a0, struct mak__f *out);
static inline void mak_ir_fn_22(struct mak__a * a0, struct mak__f *out);
static inline void mak_ir_fn_23(struct mak__a * a0, struct mak__c *out);
static inline void mak_ir_fn_24(int64_t a0, int64_t a1, struct mak__f *out);
static inline void mak_ir_fn_25(int64_t a0, struct mak__a * a1, struct mak__f *out);
static inline double mak_ir_fn_26(int64_t a0, int64_t a1, int64_t a2, int64_t a3);
static inline void mak_ir_fn_27(int64_t a0, struct mak__a * a1, struct mak__f *out);
static inline void mak_ir_fn_28(int64_t a0, struct mak__a * a1, struct mak__f *out);
static inline double mak_ir_fn_29(int64_t a0, int64_t a1);
static inline int64_t mak_ir_fn_30(int64_t a0);
static inline double mak_ir_fn_31(int64_t a0);
static inline void mak_ir_fn_32(struct mak__a * a0, struct mak__d *out);
static inline void mak_ir_fn_33(struct mak__b *out);
static inline int64_t mak_ir_fn_34(struct mak__a * a0);
static inline int64_t mak_ir_fn_35(struct mak__a * a0);
static inline void mak_ir_fn_36(void);
static inline int32_t mak_ir_fn_37(struct mak__a * a0);
static inline int32_t mak_ir_fn_38(double a0);
static inline void mak_ir_fn_39(void);

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

static inline uint32_t mak_ir_adt_type_of_tag(uint32_t tag) {
    switch (tag) {
    case mak_ir_tag_0: return UINT32_C(1342927517);
    case mak_ir_tag_1: return UINT32_C(1342927517);
    case mak_ir_tag_2: return UINT32_C(1292594660);
    case mak_ir_tag_3: return UINT32_C(1292594660);
    case mak_ir_tag_4: return UINT32_C(1309372279);
    case mak_ir_tag_5: return UINT32_C(1309372279);
    case mak_ir_tag_6: return UINT32_C(1259039422);
    case mak_ir_tag_7: return UINT32_C(1259039422);
    case mak_ir_tag_8: return UINT32_C(1275817041);
    case mak_ir_tag_9: return UINT32_C(1275817041);
    case mak_ir_tag_10: return UINT32_C(1225484184);
    case mak_ir_tag_11: return UINT32_C(1225484184);
    default: return 0u;
    }
}
static inline const void *mak_ir_adt_cast_checked(const void *src, uint32_t dst_type, const void *fallback) {
    if (!src) { mak_ir_recover("null ADT cast"); return fallback; }
    uint32_t src_type = mak_ir_adt_type_of_tag(*(const uint32_t *)src);
    if (src_type == 0u || dst_type == 0u) { mak_ir_recover("unknown ADT runtime type"); return fallback; }
    if (src_type == dst_type) return src;
    mak_ir_recover("ADT runtime cast mismatch");
    return fallback;
}
static inline const void *mak_ir_adt_recover_ptr(const void *fallback, const char *reason) {
    mak_ir_recover(reason);
    return fallback;
}
static inline int mak_ir_adt_copy_checked(void *dst, size_t dst_size, uint32_t dst_type, const void *src) {
    if (!dst || !src) { mak_ir_recover("null ADT value"); return 0; }
    uint32_t src_tag = *(const uint32_t *)src;
    uint32_t src_type = mak_ir_adt_type_of_tag(src_tag);
    if (src_type == 0u || dst_type == 0u) { mak_ir_recover("unknown ADT runtime type"); return 0; }
    if (src_type == dst_type) { memcpy(dst, src, dst_size); return 1; }
    switch (dst_type) {
    case UINT32_C(1342927517): {
        switch (src_tag) {
        case mak_ir_tag_1: {
            struct mak__a *s = (struct mak__a *)(uintptr_t)src;
            struct mak__a *d = (struct mak__a *)dst;
            d->tag = mak_ir_tag_1;
            d->v.v1.f0 = s->v.v1.f0;
            d->v.v1.f1 = s->v.v1.f1;
            return 1;
        }
        default: break;
        }
        break;
    }
    case UINT32_C(1292594660): {
        switch (src_tag) {
        case mak_ir_tag_3: {
            struct mak__b *s = (struct mak__b *)(uintptr_t)src;
            struct mak__b *d = (struct mak__b *)dst;
            d->tag = mak_ir_tag_3;
            d->v.v3.f0 = s->v.v3.f0;
            return 1;
        }
        default: break;
        }
        break;
    }
    case UINT32_C(1309372279): {
        switch (src_tag) {
        case mak_ir_tag_4: {
            struct mak__c *s = (struct mak__c *)(uintptr_t)src;
            struct mak__c *d = (struct mak__c *)dst;
            d->tag = mak_ir_tag_4;
            return 1;
        }
        case mak_ir_tag_5: {
            struct mak__c *s = (struct mak__c *)(uintptr_t)src;
            struct mak__c *d = (struct mak__c *)dst;
            d->tag = mak_ir_tag_5;
            d->v.v5.f0 = s->v.v5.f0;
            d->v.v5.f1 = s->v.v5.f1;
            return 1;
        }
        case mak_ir_tag_6: {
            struct mak__d *s = (struct mak__d *)(uintptr_t)src;
            struct mak__c *d = (struct mak__c *)dst;
            d->tag = mak_ir_tag_4;
            return 1;
        }
        case mak_ir_tag_9: {
            struct mak__e *s = (struct mak__e *)(uintptr_t)src;
            struct mak__c *d = (struct mak__c *)dst;
            d->tag = mak_ir_tag_5;
            d->v.v5.f0 = s->v.v9.f0;
            d->v.v5.f1 = s->v.v9.f1;
            return 1;
        }
        default: break;
        }
        break;
    }
    case UINT32_C(1259039422): {
        switch (src_tag) {
        case mak_ir_tag_4: {
            struct mak__c *s = (struct mak__c *)(uintptr_t)src;
            struct mak__d *d = (struct mak__d *)dst;
            d->tag = mak_ir_tag_6;
            return 1;
        }
        case mak_ir_tag_6: {
            struct mak__d *s = (struct mak__d *)(uintptr_t)src;
            struct mak__d *d = (struct mak__d *)dst;
            d->tag = mak_ir_tag_6;
            return 1;
        }
        case mak_ir_tag_7: {
            struct mak__d *s = (struct mak__d *)(uintptr_t)src;
            struct mak__d *d = (struct mak__d *)dst;
            d->tag = mak_ir_tag_7;
            d->v.v7.f0 = s->v.v7.f0;
            d->v.v7.f1 = s->v.v7.f1;
            return 1;
        }
        case mak_ir_tag_10: {
            struct mak__f *s = (struct mak__f *)(uintptr_t)src;
            struct mak__d *d = (struct mak__d *)dst;
            d->tag = mak_ir_tag_7;
            d->v.v7.f0 = s->v.v10.f0;
            d->v.v7.f1 = s->v.v10.f1;
            return 1;
        }
        default: break;
        }
        break;
    }
    case UINT32_C(1275817041): {
        switch (src_tag) {
        case mak_ir_tag_5: {
            struct mak__c *s = (struct mak__c *)(uintptr_t)src;
            struct mak__e *d = (struct mak__e *)dst;
            d->tag = mak_ir_tag_9;
            d->v.v9.f0 = s->v.v5.f0;
            d->v.v9.f1 = s->v.v5.f1;
            return 1;
        }
        case mak_ir_tag_9: {
            struct mak__e *s = (struct mak__e *)(uintptr_t)src;
            struct mak__e *d = (struct mak__e *)dst;
            d->tag = mak_ir_tag_9;
            d->v.v9.f0 = s->v.v9.f0;
            d->v.v9.f1 = s->v.v9.f1;
            return 1;
        }
        default: break;
        }
        break;
    }
    case UINT32_C(1225484184): {
        switch (src_tag) {
        case mak_ir_tag_7: {
            struct mak__d *s = (struct mak__d *)(uintptr_t)src;
            struct mak__f *d = (struct mak__f *)dst;
            d->tag = mak_ir_tag_10;
            d->v.v10.f0 = s->v.v7.f0;
            d->v.v10.f1 = s->v.v7.f1;
            return 1;
        }
        case mak_ir_tag_10: {
            struct mak__f *s = (struct mak__f *)(uintptr_t)src;
            struct mak__f *d = (struct mak__f *)dst;
            d->tag = mak_ir_tag_10;
            d->v.v10.f0 = s->v.v10.f0;
            d->v.v10.f1 = s->v.v10.f1;
            return 1;
        }
        default: break;
        }
        break;
    }
    default: break;
    }
    mak_ir_recover("ADT runtime representation mismatch");
    return 0;
}

static inline uint64_t mak_rt_f32_bits(float x) { uint32_t u; memcpy(&u, &x, sizeof u); return (uint64_t)u; }
static inline uint64_t mak_rt_f64_bits(double x) { uint64_t u; memcpy(&u, &x, sizeof u); return u; }
static inline float mak_rt_f32_value(uint64_t u) { float x; uint32_t v=(uint32_t)u; memcpy(&x, &v, sizeof x); return x; }
static inline double mak_rt_f64_value(uint64_t u) { double x; memcpy(&x, &u, sizeof x); return x; }
static int mak_rt_fold_eq(const char *a, const char *b) {
    while (*a || *b) { unsigned char ca=(unsigned char)*a++, cb=(unsigned char)*b++; if (ca>='A'&&ca<='Z') ca=(unsigned char)(ca+('a'-'A')); if (cb>='A'&&cb<='Z') cb=(unsigned char)(cb+('a'-'A')); if (ca!=cb) return 0; }
    return 1;
}
static int mak_rt_canon_eq(const char *a, const char *b) { for (;;) { while (*a && !((*a>='0'&&*a<='9')||(*a>='A'&&*a<='Z')||(*a>='a'&&*a<='z')||*a=='_')) ++a; while (*b && !((*b>='0'&&*b<='9')||(*b>='A'&&*b<='Z')||(*b>='a'&&*b<='z')||*b=='_')) ++b; if (!*a || !*b) return *a==*b; unsigned char ca=(unsigned char)*a++, cb=(unsigned char)*b++; if (ca>='A'&&ca<='Z') ca=(unsigned char)(ca+('a'-'A')); if (cb>='A'&&cb<='Z') cb=(unsigned char)(cb+('a'-'A')); if (ca!=cb) return 0; } }
static const char *mak_rt_kind_name(uint8_t k) { switch(k) { case 1:return "integer"; case 2:return "float"; case 3:return "string"; case 4:return "pointer/ADT"; case 5:return "function"; default:return "unknown"; } }
static int mak_rt_kind_compatible(uint8_t actual, uint8_t expected) { return actual==0 || expected==0 || actual==expected; }
static inline uint64_t mak_rt_hash(const char *s) { uint64_t h=1469598103934665603ULL; while(*s){ h^=(unsigned char)*s++; h*=1099511628211ULL; } return h; }
typedef uintptr_t (*mak_rt_ctor)(const uint64_t *, const uint8_t *, size_t);
static uintptr_t mak_rt_wrap_ctor_0(const uint64_t *v,const uint8_t *k,size_t n) {
    (void)k; (void)n;
    return (uintptr_t)&mak_ir_ctor_obj_0;
}
static uintptr_t mak_rt_wrap_ctor_2(const uint64_t *v,const uint8_t *k,size_t n) {
    (void)k; (void)n;
    return (uintptr_t)&mak_ir_ctor_obj_2;
}
static uintptr_t mak_rt_wrap_ctor_4(const uint64_t *v,const uint8_t *k,size_t n) {
    (void)k; (void)n;
    return (uintptr_t)&mak_ir_ctor_obj_4;
}
static uintptr_t mak_rt_wrap_ctor_6(const uint64_t *v,const uint8_t *k,size_t n) {
    (void)k; (void)n;
    return (uintptr_t)&mak_ir_ctor_obj_6;
}
static uintptr_t mak_rt_wrap_ctor_8(const uint64_t *v,const uint8_t *k,size_t n) {
    (void)k; (void)n;
    return (uintptr_t)&mak_ir_ctor_obj_8;
}
static uintptr_t mak_rt_wrap_ctor_11(const uint64_t *v,const uint8_t *k,size_t n) {
    (void)k; (void)n;
    return (uintptr_t)&mak_ir_ctor_obj_11;
}
static const uint8_t mak_rt_ck_0[]={ 0 };
static const uint8_t mak_rt_ck_2[]={ 0 };
static const uint8_t mak_rt_ck_4[]={ 0 };
static const uint8_t mak_rt_ck_6[]={ 0 };
static const uint8_t mak_rt_ck_8[]={ 0 };
static const uint8_t mak_rt_ck_11[]={ 0 };
struct mak_rt_ctor_desc { const char *name; const char *canon; const char *type; size_t arity; const uint8_t *kinds; mak_rt_ctor make; };
static const struct mak_rt_ctor_desc mak_rt_ctors[]={ {"A","a","_a",0, mak_rt_ck_0, mak_rt_wrap_ctor_0},{"C","c","_b",0, mak_rt_ck_2, mak_rt_wrap_ctor_2},{"NBad","nbad","_c",0, mak_rt_ck_4, mak_rt_wrap_ctor_4},{"NBad","nbad","_d",0, mak_rt_ck_6, mak_rt_wrap_ctor_6},{"I","i","_e",0, mak_rt_ck_8, mak_rt_wrap_ctor_8},{"L","l","_f",0, mak_rt_ck_11, mak_rt_wrap_ctor_11} };
static const size_t mak_rt_ctor_count=sizeof(mak_rt_ctors)/sizeof(mak_rt_ctors[0]);
static uintptr_t mak_rt_call_ctor(const char *query, const uint64_t *args, const uint8_t *kinds, size_t n) {
    char name_buf[256]; const char *type_hint=NULL;
    const char *colon=strchr(query,':');
    if(colon) { size_t len=(size_t)(colon-query); if(len>=sizeof name_buf) len=sizeof name_buf-1; memcpy(name_buf,query,len); name_buf[len]=0; type_hint=colon+1; query=name_buf; }
    static _Thread_local const struct mak_rt_ctor_desc *cache[128]; static _Thread_local uint64_t keys[128]; static _Thread_local uint32_t sig[128]; uint64_t h=mak_rt_hash(query)^((uint64_t)n*0x9e3779b97f4a7c15ULL); for(size_t j=0;j<n;j++) h=(h*1099511628211ULL)^kinds[j]; size_t slot=(size_t)h&127u; const struct mak_rt_ctor_desc *hit=cache[slot];
    if(hit && keys[slot]==h && sig[slot]==(uint32_t)n && hit->arity==n && (strcmp(query,hit->name)==0 || mak_rt_fold_eq(query,hit->name) || mak_rt_canon_eq(query,hit->name))) { int ok=1; for(size_t j=0;j<n && ok;j++) if(!mak_rt_kind_compatible(kinds[j],hit->kinds[j])) ok=0; if(ok && (!type_hint || strcmp(type_hint,hit->type)==0)) return hit->make(args,kinds,n); }
    const struct mak_rt_ctor_desc *best=NULL; int best_score=-1,ties=0; for(size_t i=0;i<mak_rt_ctor_count;i++) { const struct mak_rt_ctor_desc *d=&mak_rt_ctors[i]; if(d->arity!=n) continue; if(type_hint && strcmp(type_hint,d->type)!=0) continue; int score=0; if(!strcmp(query,d->name))score+=1000; else if(mak_rt_fold_eq(query,d->name))score+=800; else if(mak_rt_canon_eq(query,d->name))score+=650; for(size_t j=0;j<n;j++) if(mak_rt_kind_compatible(kinds[j],d->kinds[j])) score+=(kinds[j]==d->kinds[j]?20:3); if(score>best_score) {best=d;best_score=score;ties=0;} else if(score==best_score) ++ties; }
    if(!best) { mak_ir_recover("unresolved runtime constructor"); return (uintptr_t)0; }
    if(ties>1) { mak_ir_recover("ambiguous runtime constructor"); return (uintptr_t)0; }
    cache[slot]=best; keys[slot]=h; sig[slot]=(uint32_t)n; return best->make(args,kinds,n); }
static void (*mak_user_a)(struct mak__a *, struct mak__a *, struct mak__a *);
static void (*mak_user_b)(struct mak__a *, struct mak__b *);
static void (*mak_user_e)(struct mak__a *, struct mak__a *);
static void (*mak_user_o)(struct mak__a *, int64_t, struct mak__c *);
static double (*mak_user_u)(int64_t);
static void (*mak_user_v)(struct mak__a *, int64_t, int64_t, struct mak__e *);
static void (*mak_user_w)(struct mak__a *, int64_t, struct mak__e *);
static void (*mak_user_x)(struct mak__a *, double, struct mak__d *);
static void (*mak_user_y)(struct mak__a *, double, double, struct mak__d *);
static double (*mak_user_ae)(int64_t, double, double);
static void (*mak_user_al)(double, struct mak__a *, struct mak__f *);
static void (*mak_user_an)(struct mak__a *, struct mak__f *);
static void (*mak_user_ao)(struct mak__a *, struct mak__f *);
static void (*mak_user_ap)(double, struct mak__a *, struct mak__f *);
static void (*mak_user_aq)(struct mak__a *, struct mak__f *);
static void (*mak_user_ar)(double, struct mak__a *, struct mak__f *);
static void (*mak_user_at)(struct mak__a *, struct mak__f *);
static void (*mak_user_av)(void);

static inline void mak_ir_fn_0(struct mak__a * a0, struct mak__a * a1, struct mak__a *out) {
    for (;;) {
        if ((a0 != NULL && (a0)->tag == mak_ir_tag_0)) {
            *(out) = *(a1);
        }
        else if ((a0 != NULL && (a0)->tag == mak_ir_tag_1)) {
            int32_t mak_local_az_0 = (a0)->v.v1.f0;
            struct mak__a * mak_local_ba_1 = (a0)->v.v1.f1;
            struct mak__a * tailarg_0 = mak_local_ba_1;
            struct mak__a *arena_obj_2 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
            if (!arena_obj_2) {
                mak_ir_recover("ADT object allocation failed");
                arena_obj_2 = &mak_ir_default_obj__a;
             } else {
                (arena_obj_2)->tag = mak_ir_tag_1;
                (arena_obj_2)->v.v1.f0 = mak_local_az_0;
                (arena_obj_2)->v.v1.f1 = a1;
            }
            struct mak__a * tailarg_1 = arena_obj_2;
            a0 = tailarg_0;
            a1 = tailarg_1;
            continue;
        }
        else {
            *(out) = *(&mak_ir_default_obj__a);
        }
        return;
    }
}

static inline void mak_ir_fn_1(struct mak__a * a0, struct mak__b *out) {
    for (;;) {
        int32_t scrut_3 = mak_ir_ext_0();
        if (1) {
            int32_t mak_local_bc_2 = scrut_3;
            int32_t scrut_4 = mak_local_bc_2 < (int32_t)(INT64_C(0));
            if ((scrut_4 == INT64_C(1))) {
                (out)->tag = mak_ir_tag_3;
                struct mak__a *arena_call_5 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
                if (!arena_call_5) {
                    mak_ir_recover("ADT result allocation failed");
                    arena_call_5 = &mak_ir_default_obj__a;
                 } else {
                    mak_ir_fn_0(a0, &mak_ir_ctor_obj_0, (struct mak__a *)(arena_call_5));
                }
                (out)->v.v3.f0 = arena_call_5;
            }
            else if (1) {
                int32_t scrut_6 = mak_local_bc_2 == (int32_t)(INT64_C(10));
                if ((scrut_6 == INT64_C(1))) {
                    (out)->tag = mak_ir_tag_3;
                    struct mak__a *arena_call_7 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
                    if (!arena_call_7) {
                        mak_ir_recover("ADT result allocation failed");
                        arena_call_7 = &mak_ir_default_obj__a;
                     } else {
                        mak_ir_fn_0(a0, &mak_ir_ctor_obj_0, (struct mak__a *)(arena_call_7));
                    }
                    (out)->v.v3.f0 = arena_call_7;
                }
                else if (1) {
                    struct mak__a *arena_obj_9 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
                    if (!arena_obj_9) {
                        mak_ir_recover("ADT object allocation failed");
                        arena_obj_9 = &mak_ir_default_obj__a;
                     } else {
                        (arena_obj_9)->tag = mak_ir_tag_1;
                        (arena_obj_9)->v.v1.f0 = mak_local_bc_2;
                        (arena_obj_9)->v.v1.f1 = a0;
                    }
                    struct mak__a * tailarg_8 = arena_obj_9;
                    a0 = tailarg_8;
                    continue;
                }
            }
        }
        return;
    }
}

static inline void mak_ir_fn_2(struct mak__a * a0, struct mak__a *out) {
    for (;;) {
        if ((a0 != NULL && (a0)->tag == mak_ir_tag_0)) {
            *(out) = mak_ir_ctor_obj_0;
        }
        else if ((a0 != NULL && (a0)->tag == mak_ir_tag_1)) {
            int32_t mak_local_bg_3 = (a0)->v.v1.f0;
            struct mak__a * mak_local_bh_4 = (a0)->v.v1.f1;
            int64_t case_11 = 0;
            switch (mak_local_bg_3) {
                case INT64_C(9): {
                    case_11 = INT64_C(1);
                    break;
                }
                case INT64_C(10): {
                    case_11 = INT64_C(1);
                    break;
                }
                case INT64_C(13): {
                    case_11 = INT64_C(1);
                    break;
                }
                case INT64_C(32): {
                    case_11 = INT64_C(1);
                    break;
                }
                default: {
                    case_11 = INT64_C(0);
                    break;
                }
            }
            int64_t scrut_10 = case_11;
            if ((scrut_10 == INT64_C(1))) {
                struct mak__a * tailarg_12 = mak_local_bh_4;
                a0 = tailarg_12;
                continue;
            }
            else if (1) {
                *(out) = *(a0);
            }
        }
        else {
            *(out) = *(&mak_ir_default_obj__a);
        }
        return;
    }
}

static inline void mak_ir_fn_3(struct mak__a * a0, int64_t a1, struct mak__c *out) {
    for (;;) {
        if ((a0 != NULL && (a0)->tag == mak_ir_tag_1)) {
            int32_t mak_local_cb_5 = (a0)->v.v1.f0;
            struct mak__a * mak_local_cc_6 = (a0)->v.v1.f1;
            int64_t case_14 = 0;
            int64_t case_16 = 0;
            int32_t scrut_17 = mak_local_cb_5 == (int32_t)(INT64_C(95));
            if ((scrut_17 == INT64_C(1))) {
                case_16 = INT64_C(1);
            }
            else if (1) {
                int64_t case_18 = 0;
                int32_t scrut_19 = mak_local_cb_5 >= (int32_t)(INT64_C(65));
                if ((scrut_19 == INT64_C(0))) {
                    int64_t case_20 = 0;
                    int32_t scrut_21 = mak_local_cb_5 >= (int32_t)(INT64_C(97));
                    if ((scrut_21 == INT64_C(0))) {
                        case_20 = INT64_C(0);
                    }
                    else if (1) {
                        int64_t case_22 = 0;
                        int32_t scrut_23 = mak_local_cb_5 <= (int32_t)(INT64_C(122));
                        if ((scrut_23 == INT64_C(1))) {
                            case_22 = INT64_C(1);
                        }
                        else if (1) {
                            case_22 = INT64_C(0);
                        }
                        case_20 = case_22;
                    }
                    case_18 = case_20;
                }
                else if (1) {
                    int64_t case_24 = 0;
                    int32_t scrut_25 = mak_local_cb_5 <= (int32_t)(INT64_C(90));
                    if ((scrut_25 == INT64_C(1))) {
                        case_24 = INT64_C(1);
                    }
                    else if (1) {
                        case_24 = INT64_C(0);
                    }
                    case_18 = case_24;
                }
                case_16 = case_18;
            }
            int64_t scrut_15 = case_16;
            if ((scrut_15 == INT64_C(1))) {
                case_14 = INT64_C(1);
            }
            else if (1) {
                int64_t case_26 = 0;
                int32_t scrut_27 = mak_local_cb_5 >= (int32_t)(INT64_C(48));
                if ((scrut_27 == INT64_C(0))) {
                    case_26 = INT64_C(0);
                }
                else if (1) {
                    int64_t case_28 = 0;
                    int32_t scrut_29 = mak_local_cb_5 <= (int32_t)(INT64_C(57));
                    if ((scrut_29 == INT64_C(1))) {
                        case_28 = INT64_C(1);
                    }
                    else if (1) {
                        case_28 = INT64_C(0);
                    }
                    case_26 = case_28;
                }
                case_14 = case_26;
            }
            int64_t scrut_13 = case_14;
            if ((scrut_13 == INT64_C(1))) {
                struct mak__a * tailarg_30 = mak_local_cc_6;
                int64_t eval_33 = mak_ir_i64_mul_wrap(a1, INT64_C(131));
                int64_t case_35 = 0;
                int32_t scrut_36 = mak_local_cb_5 >= (int32_t)(INT64_C(65));
                if ((scrut_36 == INT64_C(0))) {
                    case_35 = (int64_t)(mak_local_cb_5);
                }
                else if (1) {
                    int64_t case_37 = 0;
                    int32_t scrut_38 = mak_local_cb_5 <= (int32_t)(INT64_C(90));
                    if ((scrut_38 == INT64_C(1))) {
                        case_37 = mak_ir_i64_add_wrap((int64_t)(mak_local_cb_5), INT64_C(32));
                    }
                    else if (1) {
                        case_37 = (int64_t)(mak_local_cb_5);
                    }
                    case_35 = case_37;
                }
                int64_t eval_34 = case_35;
                int64_t eval_32 = mak_ir_i64_add_wrap(eval_33, eval_34);
                int64_t tailarg_31 = mak_ir_i64_rem_wrap(eval_32, INT64_C(1000000007));
                a0 = tailarg_30;
                a1 = tailarg_31;
                continue;
            }
            else if (1) {
                (out)->tag = mak_ir_tag_5;
                (out)->v.v5.f0 = a1;
                (out)->v.v5.f1 = a0;
            }
        }
        else if ((a0 != NULL && (a0)->tag == mak_ir_tag_0)) {
            (out)->tag = mak_ir_tag_5;
            (out)->v.v5.f0 = a1;
            (out)->v.v5.f1 = &mak_ir_ctor_obj_0;
        }
        else {
            *(out) = *(&mak_ir_default_obj__c);
        }
        return;
    }
}

static inline double mak_ir_fn_4(int64_t a0) {
    if ((a0 == INT64_C(0))) {
        return 1.00000000000000000;
    }
    else if (1) {
        int32_t scrut_39 = (int32_t)(a0) > (int32_t)(INT64_C(0));
        if ((scrut_39 == INT64_C(1))) {
            double eval_40 = mak_ir_fn_4(mak_ir_i64_sub_wrap(a0, INT64_C(1)));
            return ((double)(10.00000000000000000) * (double)(eval_40));
        }
        else if (1) {
            double eval_41 = mak_ir_fn_4(mak_ir_i64_add_wrap(a0, INT64_C(1)));
            return ((double)(0.10000000000000001) * (double)(eval_41));
        }
    }
}

static inline void mak_ir_fn_5(struct mak__a * a0, int64_t a1, int64_t a2, struct mak__e *out) {
    for (;;) {
        if ((a0 != NULL && (a0)->tag == mak_ir_tag_1)) {
            int32_t mak_local_co_7 = (a0)->v.v1.f0;
            struct mak__a * mak_local_cp_8 = (a0)->v.v1.f1;
            int64_t case_43 = 0;
            int32_t scrut_44 = mak_local_co_7 >= (int32_t)(INT64_C(48));
            if ((scrut_44 == INT64_C(0))) {
                case_43 = INT64_C(0);
            }
            else if (1) {
                int64_t case_45 = 0;
                int32_t scrut_46 = mak_local_co_7 <= (int32_t)(INT64_C(57));
                if ((scrut_46 == INT64_C(1))) {
                    case_45 = INT64_C(1);
                }
                else if (1) {
                    case_45 = INT64_C(0);
                }
                case_43 = case_45;
            }
            int64_t scrut_42 = case_43;
            if ((scrut_42 == INT64_C(1))) {
                struct mak__a * tailarg_47 = mak_local_cp_8;
                int64_t eval_50 = mak_ir_i64_mul_wrap(a1, INT64_C(10));
                int64_t eval_49 = mak_ir_i64_add_wrap(eval_50, (int64_t)(mak_local_co_7));
                int64_t tailarg_48 = mak_ir_i64_sub_wrap(eval_49, INT64_C(48));
                int64_t tailarg_51 = INT64_C(1);
                a0 = tailarg_47;
                a1 = tailarg_48;
                a2 = tailarg_51;
                continue;
            }
            else if (1) {
                if ((a2 == INT64_C(1))) {
                    (out)->tag = mak_ir_tag_9;
                    (out)->v.v9.f0 = a1;
                    (out)->v.v9.f1 = a0;
                }
                else if (1) {
                    *(out) = mak_ir_ctor_obj_8;
                }
            }
        }
        else if ((a0 != NULL && (a0)->tag == mak_ir_tag_0)) {
            if ((a2 == INT64_C(1))) {
                (out)->tag = mak_ir_tag_9;
                (out)->v.v9.f0 = a1;
                (out)->v.v9.f1 = &mak_ir_ctor_obj_0;
            }
            else if (1) {
                *(out) = mak_ir_ctor_obj_8;
            }
        }
        else {
            *(out) = *(&mak_ir_default_obj__e);
        }
        return;
    }
}

static inline void mak_ir_fn_6(struct mak__a * a0, int64_t a1, struct mak__e *out) {
    struct mak__a *arena_call_53 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
    if (!arena_call_53) {
        mak_ir_recover("ADT result allocation failed");
        arena_call_53 = &mak_ir_default_obj__a;
     } else {
        mak_ir_fn_2(a0, (struct mak__a *)(arena_call_53));
    }
    struct mak__a * scrut_52 = arena_call_53;
    if ((scrut_52 != NULL && (scrut_52)->tag == mak_ir_tag_1)) {
        int32_t mak_local_cs_9 = (scrut_52)->v.v1.f0;
        struct mak__a * mak_local_ct_10 = (scrut_52)->v.v1.f1;
        switch (mak_local_cs_9) {
            case INT64_C(43): {
                mak_ir_fn_5(mak_local_ct_10, INT64_C(0), INT64_C(0), (struct mak__e *)(out));
                break;
            }
            case INT64_C(45): {
                struct mak__e *arena_call_55 = (struct mak__e *)mak_ir_arena_alloc(sizeof(struct mak__e), _Alignof(struct mak__e));
                if (!arena_call_55) {
                    mak_ir_recover("ADT result allocation failed");
                    arena_call_55 = &mak_ir_default_obj__e;
                 } else {
                    mak_ir_fn_5(mak_local_ct_10, INT64_C(0), INT64_C(0), (struct mak__e *)(arena_call_55));
                }
                struct mak__e * scrut_54 = arena_call_55;
                if ((scrut_54 != NULL && (scrut_54)->tag == mak_ir_tag_8)) {
                    *(out) = mak_ir_ctor_obj_8;
                }
                else if ((scrut_54 != NULL && (scrut_54)->tag == mak_ir_tag_9)) {
                    int64_t mak_local_cu_11 = (scrut_54)->v.v9.f0;
                    struct mak__a * mak_local_cv_12 = (scrut_54)->v.v9.f1;
                    (out)->tag = mak_ir_tag_9;
                    (out)->v.v9.f0 = mak_ir_i64_neg_wrap((int64_t)(mak_local_cu_11));
                    (out)->v.v9.f1 = mak_local_cv_12;
                }
                else {
                    *(out) = *(&mak_ir_default_obj__e);
                }
                break;
            }
            default: {
                int64_t case_57 = 0;
                int32_t scrut_58 = mak_local_cs_9 >= (int32_t)(INT64_C(48));
                if ((scrut_58 == INT64_C(0))) {
                    case_57 = INT64_C(0);
                }
                else if (1) {
                    int64_t case_59 = 0;
                    int32_t scrut_60 = mak_local_cs_9 <= (int32_t)(INT64_C(57));
                    if ((scrut_60 == INT64_C(1))) {
                        case_59 = INT64_C(1);
                    }
                    else if (1) {
                        case_59 = INT64_C(0);
                    }
                    case_57 = case_59;
                }
                int64_t scrut_56 = case_57;
                if ((scrut_56 == INT64_C(1))) {
                    struct mak__a *arena_obj_61 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
                    if (!arena_obj_61) {
                        mak_ir_recover("ADT object allocation failed");
                        arena_obj_61 = &mak_ir_default_obj__a;
                     } else {
                        (arena_obj_61)->tag = mak_ir_tag_1;
                        (arena_obj_61)->v.v1.f0 = mak_local_cs_9;
                        (arena_obj_61)->v.v1.f1 = mak_local_ct_10;
                    }
                    mak_ir_fn_5(arena_obj_61, INT64_C(0), INT64_C(0), (struct mak__e *)(out));
                }
                else if (1) {
                    *(out) = mak_ir_ctor_obj_8;
                }
                break;
            }
        }
    }
    else if ((scrut_52 != NULL && (scrut_52)->tag == mak_ir_tag_0)) {
        *(out) = mak_ir_ctor_obj_8;
    }
    else {
        *(out) = *(&mak_ir_default_obj__e);
    }
    return;
}

static inline void mak_ir_fn_7(struct mak__a * a0, double a1, struct mak__d *out) {
    for (;;) {
        if ((a0 != NULL && (a0)->tag == mak_ir_tag_1)) {
            int32_t mak_local_cy_13 = (a0)->v.v1.f0;
            struct mak__a * mak_local_cz_14 = (a0)->v.v1.f1;
            int64_t case_63 = 0;
            int32_t scrut_64 = mak_local_cy_13 >= (int32_t)(INT64_C(48));
            if ((scrut_64 == INT64_C(0))) {
                case_63 = INT64_C(0);
            }
            else if (1) {
                int64_t case_65 = 0;
                int32_t scrut_66 = mak_local_cy_13 <= (int32_t)(INT64_C(57));
                if ((scrut_66 == INT64_C(1))) {
                    case_65 = INT64_C(1);
                }
                else if (1) {
                    case_65 = INT64_C(0);
                }
                case_63 = case_65;
            }
            int64_t scrut_62 = case_63;
            if ((scrut_62 == INT64_C(1))) {
                struct mak__a * tailarg_67 = mak_local_cz_14;
                double eval_69 = ((double)(a1) * (double)(10.00000000000000000));
                double case_71 = 0.0;
                switch (mak_local_cy_13) {
                    case INT64_C(48): {
                        case_71 = 0.00000000000000000;
                        break;
                    }
                    case INT64_C(49): {
                        case_71 = 1.00000000000000000;
                        break;
                    }
                    case INT64_C(50): {
                        case_71 = 2.00000000000000000;
                        break;
                    }
                    case INT64_C(51): {
                        case_71 = 3.00000000000000000;
                        break;
                    }
                    case INT64_C(52): {
                        case_71 = 4.00000000000000000;
                        break;
                    }
                    case INT64_C(53): {
                        case_71 = 5.00000000000000000;
                        break;
                    }
                    case INT64_C(54): {
                        case_71 = 6.00000000000000000;
                        break;
                    }
                    case INT64_C(55): {
                        case_71 = 7.00000000000000000;
                        break;
                    }
                    case INT64_C(56): {
                        case_71 = 8.00000000000000000;
                        break;
                    }
                    case INT64_C(57): {
                        case_71 = 9.00000000000000000;
                        break;
                    }
                    default: {
                        case_71 = 0.00000000000000000;
                        break;
                    }
                }
                double eval_70 = case_71;
                double tailarg_68 = ((double)(eval_69) + (double)(eval_70));
                a0 = tailarg_67;
                a1 = tailarg_68;
                continue;
            }
            else if (1) {
                (out)->tag = mak_ir_tag_7;
                (out)->v.v7.f0 = a1;
                (out)->v.v7.f1 = a0;
            }
        }
        else if ((a0 != NULL && (a0)->tag == mak_ir_tag_0)) {
            (out)->tag = mak_ir_tag_7;
            (out)->v.v7.f0 = a1;
            (out)->v.v7.f1 = &mak_ir_ctor_obj_0;
        }
        else {
            *(out) = *(&mak_ir_default_obj__d);
        }
        return;
    }
}

static inline void mak_ir_fn_8(struct mak__a * a0, double a1, double a2, struct mak__d *out) {
    for (;;) {
        if ((a0 != NULL && (a0)->tag == mak_ir_tag_1)) {
            int32_t mak_local_dd_15 = (a0)->v.v1.f0;
            struct mak__a * mak_local_de_16 = (a0)->v.v1.f1;
            int64_t case_73 = 0;
            int32_t scrut_74 = mak_local_dd_15 >= (int32_t)(INT64_C(48));
            if ((scrut_74 == INT64_C(0))) {
                case_73 = INT64_C(0);
            }
            else if (1) {
                int64_t case_75 = 0;
                int32_t scrut_76 = mak_local_dd_15 <= (int32_t)(INT64_C(57));
                if ((scrut_76 == INT64_C(1))) {
                    case_75 = INT64_C(1);
                }
                else if (1) {
                    case_75 = INT64_C(0);
                }
                case_73 = case_75;
            }
            int64_t scrut_72 = case_73;
            if ((scrut_72 == INT64_C(1))) {
                struct mak__a * tailarg_77 = mak_local_de_16;
                double case_81 = 0.0;
                switch (mak_local_dd_15) {
                    case INT64_C(48): {
                        case_81 = 0.00000000000000000;
                        break;
                    }
                    case INT64_C(49): {
                        case_81 = 1.00000000000000000;
                        break;
                    }
                    case INT64_C(50): {
                        case_81 = 2.00000000000000000;
                        break;
                    }
                    case INT64_C(51): {
                        case_81 = 3.00000000000000000;
                        break;
                    }
                    case INT64_C(52): {
                        case_81 = 4.00000000000000000;
                        break;
                    }
                    case INT64_C(53): {
                        case_81 = 5.00000000000000000;
                        break;
                    }
                    case INT64_C(54): {
                        case_81 = 6.00000000000000000;
                        break;
                    }
                    case INT64_C(55): {
                        case_81 = 7.00000000000000000;
                        break;
                    }
                    case INT64_C(56): {
                        case_81 = 8.00000000000000000;
                        break;
                    }
                    case INT64_C(57): {
                        case_81 = 9.00000000000000000;
                        break;
                    }
                    default: {
                        case_81 = 0.00000000000000000;
                        break;
                    }
                }
                double eval_80 = case_81;
                double eval_79 = ((double)(eval_80) * (double)(a2));
                double tailarg_78 = ((double)(a1) + (double)(eval_79));
                double tailarg_82 = ((double)(a2) * (double)(0.10000000000000001));
                a0 = tailarg_77;
                a1 = tailarg_78;
                a2 = tailarg_82;
                continue;
            }
            else if (1) {
                switch (mak_local_dd_15) {
                    case INT64_C(101): {
                        struct mak__e *arena_call_84 = (struct mak__e *)mak_ir_arena_alloc(sizeof(struct mak__e), _Alignof(struct mak__e));
                        if (!arena_call_84) {
                            mak_ir_recover("ADT result allocation failed");
                            arena_call_84 = &mak_ir_default_obj__e;
                         } else {
                            mak_ir_fn_6(mak_local_de_16, INT64_C(1), (struct mak__e *)(arena_call_84));
                        }
                        struct mak__e * scrut_83 = arena_call_84;
                        if ((scrut_83 != NULL && (scrut_83)->tag == mak_ir_tag_8)) {
                            *(out) = mak_ir_ctor_obj_6;
                        }
                        else if ((scrut_83 != NULL && (scrut_83)->tag == mak_ir_tag_9)) {
                            int64_t mak_local_df_17 = (scrut_83)->v.v9.f0;
                            struct mak__a * mak_local_dg_18 = (scrut_83)->v.v9.f1;
                            (out)->tag = mak_ir_tag_7;
                            double eval_85 = mak_ir_fn_4(mak_local_df_17);
                            (out)->v.v7.f0 = ((double)(a1) * (double)(eval_85));
                            (out)->v.v7.f1 = mak_local_dg_18;
                        }
                        else {
                            *(out) = *(&mak_ir_default_obj__d);
                        }
                        break;
                    }
                    case INT64_C(69): {
                        struct mak__e *arena_call_87 = (struct mak__e *)mak_ir_arena_alloc(sizeof(struct mak__e), _Alignof(struct mak__e));
                        if (!arena_call_87) {
                            mak_ir_recover("ADT result allocation failed");
                            arena_call_87 = &mak_ir_default_obj__e;
                         } else {
                            mak_ir_fn_6(mak_local_de_16, INT64_C(1), (struct mak__e *)(arena_call_87));
                        }
                        struct mak__e * scrut_86 = arena_call_87;
                        if ((scrut_86 != NULL && (scrut_86)->tag == mak_ir_tag_8)) {
                            *(out) = mak_ir_ctor_obj_6;
                        }
                        else if ((scrut_86 != NULL && (scrut_86)->tag == mak_ir_tag_9)) {
                            int64_t mak_local_dh_19 = (scrut_86)->v.v9.f0;
                            struct mak__a * mak_local_di_20 = (scrut_86)->v.v9.f1;
                            (out)->tag = mak_ir_tag_7;
                            double eval_88 = mak_ir_fn_4(mak_local_dh_19);
                            (out)->v.v7.f0 = ((double)(a1) * (double)(eval_88));
                            (out)->v.v7.f1 = mak_local_di_20;
                        }
                        else {
                            *(out) = *(&mak_ir_default_obj__d);
                        }
                        break;
                    }
                    default: {
                        (out)->tag = mak_ir_tag_7;
                        (out)->v.v7.f0 = a1;
                        (out)->v.v7.f1 = a0;
                        break;
                    }
                }
            }
        }
        else if ((a0 != NULL && (a0)->tag == mak_ir_tag_0)) {
            (out)->tag = mak_ir_tag_7;
            (out)->v.v7.f0 = a1;
            (out)->v.v7.f1 = &mak_ir_ctor_obj_0;
        }
        else {
            *(out) = *(&mak_ir_default_obj__d);
        }
        return;
    }
}

static inline double mak_ir_fn_9(int64_t a0, double a1, double a2) {
    for (;;) {
        switch (a0) {
            case INT64_C(828949774): {
                return mak_ir_ext_28(a1, a2);
            }
            case INT64_C(231190472): {
                return mak_ir_ext_12(a1, a2);
            }
            case INT64_C(1885190): {
                return mak_ir_ext_12(a1, a2);
            }
            case INT64_C(1969694): {
                return mak_ir_ext_12(a1, a2);
            }
            case INT64_C(510764621): {
                return mak_ir_ext_13(a1, a2);
            }
            case INT64_C(1936692): {
                return mak_ir_ext_14(a1, a2);
            }
            case INT64_C(258201902): {
                return mak_ir_ext_14(a1, ((double)(1.00000000000000000) / (double)(a2)));
            }
            case INT64_C(548941060): {
                return mak_ir_ext_14(a1, ((double)(1.00000000000000000) / (double)(a2)));
            }
            case INT64_C(901947274): {
                return mak_ir_ext_37(a1, a2);
            }
            case INT64_C(1780652): {
                double mak_local_eg_21 = mak_ir_ext_7(a2);
                int32_t scrut_89 = (int32_t)(mak_local_eg_21) == (int32_t)(0.00000000000000000);
                if ((scrut_89 == INT64_C(1))) {
                    return mak_ir_ext_7(a1);
                }
                else if (1) {
                    int64_t tailarg_90 = INT64_C(1780652);
                    double tailarg_91 = mak_local_eg_21;
                    double tailarg_92 = mak_ir_ext_12(mak_ir_ext_7(a1), mak_local_eg_21);
                    a0 = tailarg_90;
                    a1 = tailarg_91;
                    a2 = tailarg_92;
                    continue;
                }
            }
            case INT64_C(1866466): {
                double mak_local_eh_22 = mak_ir_fn_9(INT64_C(1780652), a1, a2);
                int32_t scrut_93 = (int32_t)(mak_local_eh_22) == (int32_t)(0.00000000000000000);
                if ((scrut_93 == INT64_C(1))) {
                    return 0.00000000000000000;
                }
                else if (1) {
                    double eval_94 = mak_ir_ext_7(((double)(a1) * (double)(a2)));
                    return ((double)(eval_94) / (double)(mak_local_eh_22));
                }
            }
            case INT64_C(1900793): {
                double mak_local_ej_23 = mak_ir_ext_11(a2);
                int32_t scrut_95 = (int32_t)(mak_local_ej_23) < (int32_t)(0.00000000000000000);
                if ((scrut_95 == INT64_C(1))) {
                    return 0.00000000000000000;
                }
                else if (1) {
                    double eval_97 = mak_ir_ext_11(a1);
                    int32_t scrut_96 = (int32_t)(mak_local_ej_23) > (int32_t)(eval_97);
                    if ((scrut_96 == INT64_C(1))) {
                        return 0.00000000000000000;
                    }
                    else if (1) {
                        double eval_100 = mak_ir_ext_11(a1);
                        double eval_99 = mak_ir_ext_36(((double)(eval_100) + (double)(1.00000000000000000)));
                        double eval_101 = mak_ir_ext_36(((double)(mak_local_ej_23) + (double)(1.00000000000000000)));
                        double eval_98 = ((double)(eval_99) - (double)(eval_101));
                        double eval_104 = mak_ir_ext_11(a1);
                        double eval_103 = ((double)(eval_104) - (double)(mak_local_ej_23));
                        double eval_102 = mak_ir_ext_36(((double)(eval_103) + (double)(1.00000000000000000)));
                        return mak_ir_ext_11(mak_ir_ext_15(((double)(eval_98) - (double)(eval_102))));
                    }
                }
            }
            case INT64_C(224480257): {
                int64_t tailarg_105 = INT64_C(1900793);
                double tailarg_106 = a1;
                double tailarg_107 = a2;
                a0 = tailarg_105;
                a1 = tailarg_106;
                a2 = tailarg_107;
                continue;
            }
            case INT64_C(1902496): {
                double mak_local_el_24 = mak_ir_ext_11(a2);
                int32_t scrut_108 = (int32_t)(mak_local_el_24) < (int32_t)(0.00000000000000000);
                if ((scrut_108 == INT64_C(1))) {
                    return 0.00000000000000000;
                }
                else if (1) {
                    double eval_110 = mak_ir_ext_11(a1);
                    int32_t scrut_109 = (int32_t)(mak_local_el_24) > (int32_t)(eval_110);
                    if ((scrut_109 == INT64_C(1))) {
                        return 0.00000000000000000;
                    }
                    else if (1) {
                        double eval_112 = mak_ir_ext_11(a1);
                        double eval_111 = mak_ir_ext_36(((double)(eval_112) + (double)(1.00000000000000000)));
                        double eval_115 = mak_ir_ext_11(a1);
                        double eval_114 = ((double)(eval_115) - (double)(mak_local_el_24));
                        double eval_113 = mak_ir_ext_36(((double)(eval_114) + (double)(1.00000000000000000)));
                        return mak_ir_ext_11(mak_ir_ext_15(((double)(eval_111) - (double)(eval_113))));
                    }
                }
            }
            case INT64_C(253534496): {
                int64_t tailarg_116 = INT64_C(1902496);
                double tailarg_117 = a1;
                double tailarg_118 = a2;
                a0 = tailarg_116;
                a1 = tailarg_117;
                a2 = tailarg_118;
                continue;
            }
            case INT64_C(1884414): {
                int32_t scrut_119 = (int32_t)(a1) < (int32_t)(a2);
                if ((scrut_119 == INT64_C(1))) {
                    return a1;
                }
                else if (1) {
                    return a2;
                }
            }
            case INT64_C(1883376): {
                int32_t scrut_120 = (int32_t)(a1) > (int32_t)(a2);
                if ((scrut_120 == INT64_C(1))) {
                    return a1;
                }
                else if (1) {
                    return a2;
                }
            }
            case INT64_C(262106026): {
                return mak_ir_ext_41(a1, a2);
            }
            case INT64_C(231035246): {
                return mak_ir_ext_42(a1, a2);
            }
            case INT64_C(231188658): {
                return mak_ir_ext_43(a1, a2);
            }
            case INT64_C(231189696): {
                return mak_ir_ext_44(a1, a2);
            }
            case INT64_C(743972013): {
                return mak_ir_ext_46(a1, a2);
            }
            default: {
                return 0.00000000000000000;
            }
        }
    }
}

static inline void mak_ir_fn_10(double a0, struct mak__a * a1, struct mak__f *out) {
    for (;;) {
        struct mak__a *arena_call_122 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
        if (!arena_call_122) {
            mak_ir_recover("ADT result allocation failed");
            arena_call_122 = &mak_ir_default_obj__a;
         } else {
            mak_ir_fn_2(a1, (struct mak__a *)(arena_call_122));
        }
        struct mak__a * scrut_121 = arena_call_122;
        if ((scrut_121 != NULL && (scrut_121)->tag == mak_ir_tag_1) && (scrut_121->v.v1.f0 == INT64_C(33))) {
            struct mak__a * mak_local_ge_25 = (scrut_121)->v.v1.f1;
            double tailarg_123 = mak_ir_ext_35(((double)(a0) + (double)(1.00000000000000000)));
            struct mak__a * tailarg_124 = mak_local_ge_25;
            a0 = tailarg_123;
            a1 = tailarg_124;
            continue;
        }
        else if ((scrut_121 != NULL && (scrut_121)->tag == mak_ir_tag_1) && (scrut_121->v.v1.f0 == INT64_C(37))) {
            struct mak__a * mak_local_gf_26 = (scrut_121)->v.v1.f1;
            struct mak__a *arena_call_126 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
            if (!arena_call_126) {
                mak_ir_recover("ADT result allocation failed");
                arena_call_126 = &mak_ir_default_obj__a;
             } else {
                mak_ir_fn_2(mak_local_gf_26, (struct mak__a *)(arena_call_126));
            }
            struct mak__a * scrut_125 = arena_call_126;
            if ((scrut_125 != NULL && (scrut_125)->tag == mak_ir_tag_0)) {
                (out)->tag = mak_ir_tag_10;
                (out)->v.v10.f0 = ((double)(a0) / (double)(100.00000000000000000));
                (out)->v.v10.f1 = &mak_ir_ctor_obj_0;
            }
            else if ((scrut_125 != NULL && (scrut_125)->tag == mak_ir_tag_1)) {
                int32_t mak_local_gg_27 = (scrut_125)->v.v1.f0;
                switch (mak_local_gg_27) {
                    case INT64_C(41): {
                        (out)->tag = mak_ir_tag_10;
                        (out)->v.v10.f0 = ((double)(a0) / (double)(100.00000000000000000));
                        (out)->v.v10.f1 = mak_local_gf_26;
                        break;
                    }
                    case INT64_C(44): {
                        (out)->tag = mak_ir_tag_10;
                        (out)->v.v10.f0 = ((double)(a0) / (double)(100.00000000000000000));
                        (out)->v.v10.f1 = mak_local_gf_26;
                        break;
                    }
                    case INT64_C(43): {
                        (out)->tag = mak_ir_tag_10;
                        (out)->v.v10.f0 = ((double)(a0) / (double)(100.00000000000000000));
                        (out)->v.v10.f1 = mak_local_gf_26;
                        break;
                    }
                    case INT64_C(45): {
                        (out)->tag = mak_ir_tag_10;
                        (out)->v.v10.f0 = ((double)(a0) / (double)(100.00000000000000000));
                        (out)->v.v10.f1 = mak_local_gf_26;
                        break;
                    }
                    case INT64_C(42): {
                        (out)->tag = mak_ir_tag_10;
                        (out)->v.v10.f0 = ((double)(a0) / (double)(100.00000000000000000));
                        (out)->v.v10.f1 = mak_local_gf_26;
                        break;
                    }
                    case INT64_C(47): {
                        (out)->tag = mak_ir_tag_10;
                        (out)->v.v10.f0 = ((double)(a0) / (double)(100.00000000000000000));
                        (out)->v.v10.f1 = mak_local_gf_26;
                        break;
                    }
                    case INT64_C(94): {
                        (out)->tag = mak_ir_tag_10;
                        (out)->v.v10.f0 = ((double)(a0) / (double)(100.00000000000000000));
                        (out)->v.v10.f1 = mak_local_gf_26;
                        break;
                    }
                    default: {
                        (out)->tag = mak_ir_tag_10;
                        (out)->v.v10.f0 = a0;
                        (out)->v.v10.f1 = a1;
                        break;
                    }
                }
            }
            else {
                *(out) = *(&mak_ir_default_obj__f);
            }
        }
        else if (1) {
            (out)->tag = mak_ir_tag_10;
            (out)->v.v10.f0 = a0;
            (out)->v.v10.f1 = a1;
        }
        return;
    }
}

static inline void mak_ir_fn_11(struct mak__a * a0, struct mak__f *out) {
    struct mak__f *arena_case_128 = (struct mak__f *)mak_ir_arena_alloc(sizeof(struct mak__f), _Alignof(struct mak__f));
    if (!arena_case_128) {
        mak_ir_recover("ADT case-result allocation failed");
        arena_case_128 = &mak_ir_default_obj__f;
     } else {
        struct mak__f *arena_case_130 = (struct mak__f *)mak_ir_arena_alloc(sizeof(struct mak__f), _Alignof(struct mak__f));
        if (!arena_case_130) {
            mak_ir_recover("ADT case-result allocation failed");
            arena_case_130 = &mak_ir_default_obj__f;
         } else {
            struct mak__a *arena_call_132 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
            if (!arena_call_132) {
                mak_ir_recover("ADT result allocation failed");
                arena_call_132 = &mak_ir_default_obj__a;
             } else {
                mak_ir_fn_2(a0, (struct mak__a *)(arena_call_132));
            }
            struct mak__a * scrut_131 = arena_call_132;
            if ((scrut_131 != NULL && (scrut_131)->tag == mak_ir_tag_0)) {
                *(arena_case_130) = mak_ir_ctor_obj_11;
            }
            else if ((scrut_131 != NULL && (scrut_131)->tag == mak_ir_tag_1)) {
                int32_t mak_local_fs_28 = (scrut_131)->v.v1.f0;
                struct mak__a * mak_local_ft_29 = (scrut_131)->v.v1.f1;
                if ((mak_local_fs_28 == INT64_C(40))) {
                    struct mak__f *arena_call_134 = (struct mak__f *)mak_ir_arena_alloc(sizeof(struct mak__f), _Alignof(struct mak__f));
                    if (!arena_call_134) {
                        mak_ir_recover("ADT result allocation failed");
                        arena_call_134 = &mak_ir_default_obj__f;
                     } else {
                        mak_ir_fn_16(mak_local_ft_29, (struct mak__f *)(arena_call_134));
                    }
                    struct mak__f * scrut_133 = arena_call_134;
                    if ((scrut_133 != NULL && (scrut_133)->tag == mak_ir_tag_11)) {
                        *(arena_case_130) = mak_ir_ctor_obj_11;
                    }
                    else if ((scrut_133 != NULL && (scrut_133)->tag == mak_ir_tag_10)) {
                        double mak_local_fu_30 = (scrut_133)->v.v10.f0;
                        struct mak__a * mak_local_fv_31 = (scrut_133)->v.v10.f1;
                        struct mak__a *arena_call_136 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
                        if (!arena_call_136) {
                            mak_ir_recover("ADT result allocation failed");
                            arena_call_136 = &mak_ir_default_obj__a;
                         } else {
                            mak_ir_fn_2(mak_local_fv_31, (struct mak__a *)(arena_call_136));
                        }
                        struct mak__a * scrut_135 = arena_call_136;
                        if ((scrut_135 != NULL && (scrut_135)->tag == mak_ir_tag_1) && (scrut_135->v.v1.f0 == INT64_C(41))) {
                            struct mak__a * mak_local_fw_32 = (scrut_135)->v.v1.f1;
                            (arena_case_130)->tag = mak_ir_tag_10;
                            (arena_case_130)->v.v10.f0 = mak_local_fu_30;
                            (arena_case_130)->v.v10.f1 = mak_local_fw_32;
                        }
                        else if (1) {
                            *(arena_case_130) = mak_ir_ctor_obj_11;
                        }
                    }
                    else {
                        *(arena_case_130) = *(&mak_ir_default_obj__f);
                    }
                }
                else if (1) {
                    int64_t case_138 = 0;
                    int32_t scrut_139 = mak_local_fs_28 == (int32_t)(INT64_C(95));
                    if ((scrut_139 == INT64_C(1))) {
                        case_138 = INT64_C(1);
                    }
                    else if (1) {
                        int64_t case_140 = 0;
                        int32_t scrut_141 = mak_local_fs_28 >= (int32_t)(INT64_C(65));
                        if ((scrut_141 == INT64_C(0))) {
                            int64_t case_142 = 0;
                            int32_t scrut_143 = mak_local_fs_28 >= (int32_t)(INT64_C(97));
                            if ((scrut_143 == INT64_C(0))) {
                                case_142 = INT64_C(0);
                            }
                            else if (1) {
                                int64_t case_144 = 0;
                                int32_t scrut_145 = mak_local_fs_28 <= (int32_t)(INT64_C(122));
                                if ((scrut_145 == INT64_C(1))) {
                                    case_144 = INT64_C(1);
                                }
                                else if (1) {
                                    case_144 = INT64_C(0);
                                }
                                case_142 = case_144;
                            }
                            case_140 = case_142;
                        }
                        else if (1) {
                            int64_t case_146 = 0;
                            int32_t scrut_147 = mak_local_fs_28 <= (int32_t)(INT64_C(90));
                            if ((scrut_147 == INT64_C(1))) {
                                case_146 = INT64_C(1);
                            }
                            else if (1) {
                                case_146 = INT64_C(0);
                            }
                            case_140 = case_146;
                        }
                        case_138 = case_140;
                    }
                    int64_t scrut_137 = case_138;
                    if ((scrut_137 == INT64_C(1))) {
                        struct mak__f *arena_case_149 = (struct mak__f *)mak_ir_arena_alloc(sizeof(struct mak__f), _Alignof(struct mak__f));
                        if (!arena_case_149) {
                            mak_ir_recover("ADT case-result allocation failed");
                            arena_case_149 = &mak_ir_default_obj__f;
                         } else {
                            struct mak__a *arena_call_151 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
                            if (!arena_call_151) {
                                mak_ir_recover("ADT result allocation failed");
                                arena_call_151 = &mak_ir_default_obj__a;
                             } else {
                                struct mak__a *arena_obj_152 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
                                if (!arena_obj_152) {
                                    mak_ir_recover("ADT object allocation failed");
                                    arena_obj_152 = &mak_ir_default_obj__a;
                                 } else {
                                    (arena_obj_152)->tag = mak_ir_tag_1;
                                    (arena_obj_152)->v.v1.f0 = mak_local_fs_28;
                                    (arena_obj_152)->v.v1.f1 = mak_local_ft_29;
                                }
                                mak_ir_fn_2(arena_obj_152, (struct mak__a *)(arena_call_151));
                            }
                            struct mak__a * scrut_150 = arena_call_151;
                            if ((scrut_150 != NULL && (scrut_150)->tag == mak_ir_tag_1)) {
                                int32_t mak_local_ce_33 = (scrut_150)->v.v1.f0;
                                struct mak__a * mak_local_cf_34 = (scrut_150)->v.v1.f1;
                                int64_t case_154 = 0;
                                int32_t scrut_155 = mak_local_ce_33 == (int32_t)(INT64_C(95));
                                if ((scrut_155 == INT64_C(1))) {
                                    case_154 = INT64_C(1);
                                }
                                else if (1) {
                                    int64_t case_156 = 0;
                                    int32_t scrut_157 = mak_local_ce_33 >= (int32_t)(INT64_C(65));
                                    if ((scrut_157 == INT64_C(0))) {
                                        int64_t case_158 = 0;
                                        int32_t scrut_159 = mak_local_ce_33 >= (int32_t)(INT64_C(97));
                                        if ((scrut_159 == INT64_C(0))) {
                                            case_158 = INT64_C(0);
                                        }
                                        else if (1) {
                                            int64_t case_160 = 0;
                                            int32_t scrut_161 = mak_local_ce_33 <= (int32_t)(INT64_C(122));
                                            if ((scrut_161 == INT64_C(1))) {
                                                case_160 = INT64_C(1);
                                            }
                                            else if (1) {
                                                case_160 = INT64_C(0);
                                            }
                                            case_158 = case_160;
                                        }
                                        case_156 = case_158;
                                    }
                                    else if (1) {
                                        int64_t case_162 = 0;
                                        int32_t scrut_163 = mak_local_ce_33 <= (int32_t)(INT64_C(90));
                                        if ((scrut_163 == INT64_C(1))) {
                                            case_162 = INT64_C(1);
                                        }
                                        else if (1) {
                                            case_162 = INT64_C(0);
                                        }
                                        case_156 = case_162;
                                    }
                                    case_154 = case_156;
                                }
                                int64_t scrut_153 = case_154;
                                if ((scrut_153 == INT64_C(1))) {
                                    struct mak__c *arena_call_164 = (struct mak__c *)mak_ir_arena_alloc(sizeof(struct mak__c), _Alignof(struct mak__c));
                                    if (!arena_call_164) {
                                        mak_ir_recover("ADT result allocation failed");
                                        arena_call_164 = &mak_ir_default_obj__c;
                                     } else {
                                        struct mak__a *arena_obj_165 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
                                        if (!arena_obj_165) {
                                            mak_ir_recover("ADT object allocation failed");
                                            arena_obj_165 = &mak_ir_default_obj__a;
                                         } else {
                                            (arena_obj_165)->tag = mak_ir_tag_1;
                                            (arena_obj_165)->v.v1.f0 = mak_local_ce_33;
                                            (arena_obj_165)->v.v1.f1 = mak_local_cf_34;
                                        }
                                        mak_ir_fn_3(arena_obj_165, INT64_C(0), (struct mak__c *)(arena_call_164));
                                    }
                                    if (!mak_ir_adt_copy_checked((void *)(arena_case_149), sizeof(struct mak__f), UINT32_C(1225484184), (const void *)(uintptr_t)(arena_call_164))) {
                                        *(arena_case_149) = *(&mak_ir_default_obj__f);
                                    }
                                }
                                else if (1) {
                                    struct mak__f *rt_heap_obj_166 = (struct mak__f *)(uintptr_t)((struct mak__f *)(uintptr_t)(mak_rt_call_ctor("NBad:_f", (const uint64_t[]){}, (const uint8_t[]){}, 0)));
                                    if (!mak_ir_adt_copy_checked((void *)(arena_case_149), sizeof(struct mak__f), UINT32_C(1225484184), (const void *)(rt_heap_obj_166)) ) {
                                        *(arena_case_149) = *(&mak_ir_default_obj__f);
                                    }
                                }
                            }
                            else if ((scrut_150 != NULL && (scrut_150)->tag == mak_ir_tag_0)) {
                                struct mak__f *rt_heap_obj_167 = (struct mak__f *)(uintptr_t)((struct mak__f *)(uintptr_t)(mak_rt_call_ctor("NBad:_f", (const uint64_t[]){}, (const uint8_t[]){}, 0)));
                                if (!mak_ir_adt_copy_checked((void *)(arena_case_149), sizeof(struct mak__f), UINT32_C(1225484184), (const void *)(rt_heap_obj_167)) ) {
                                    *(arena_case_149) = *(&mak_ir_default_obj__f);
                                }
                            }
                            else {
                                *(arena_case_149) = *(&mak_ir_default_obj__f);
                            }
                        }
                        struct mak__f * scrut_148 = arena_case_149;
                        if ((scrut_148 != NULL && (scrut_148)->tag == mak_ir_tag_11)) {
                            *(arena_case_130) = mak_ir_ctor_obj_11;
                        }
                        else if ((scrut_148 != NULL && (scrut_148)->tag == mak_ir_tag_10)) {
                            double mak_local_fx_35 = (scrut_148)->v.v10.f0;
                            struct mak__a * mak_local_fy_36 = (scrut_148)->v.v10.f1;
                            struct mak__a *arena_call_169 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
                            if (!arena_call_169) {
                                mak_ir_recover("ADT result allocation failed");
                                arena_call_169 = &mak_ir_default_obj__a;
                             } else {
                                mak_ir_fn_2(mak_local_fy_36, (struct mak__a *)(arena_call_169));
                            }
                            struct mak__a * scrut_168 = arena_call_169;
                            if ((scrut_168 != NULL && (scrut_168)->tag == mak_ir_tag_1) && (scrut_168->v.v1.f0 == INT64_C(40))) {
                                struct mak__a * mak_local_fz_37 = (scrut_168)->v.v1.f1;
                                *(arena_case_130) = mak_ir_ctor_obj_11;
                            }
                            else if (1) {
                                *(arena_case_130) = mak_ir_ctor_obj_11;
                            }
                        }
                        else {
                            *(arena_case_130) = *(&mak_ir_default_obj__f);
                        }
                    }
                    else if (1) {
                        struct mak__f *arena_case_171 = (struct mak__f *)mak_ir_arena_alloc(sizeof(struct mak__f), _Alignof(struct mak__f));
                        if (!arena_case_171) {
                            mak_ir_recover("ADT case-result allocation failed");
                            arena_case_171 = &mak_ir_default_obj__f;
                         } else {
                            struct mak__a *arena_call_173 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
                            if (!arena_call_173) {
                                mak_ir_recover("ADT result allocation failed");
                                arena_call_173 = &mak_ir_default_obj__a;
                             } else {
                                mak_ir_fn_2(a0, (struct mak__a *)(arena_call_173));
                            }
                            struct mak__a * scrut_172 = arena_call_173;
                            if ((scrut_172 != NULL && (scrut_172)->tag == mak_ir_tag_0)) {
                                struct mak__f *rt_heap_obj_174 = (struct mak__f *)(uintptr_t)((struct mak__f *)(uintptr_t)(mak_rt_call_ctor("NBad:_f", (const uint64_t[]){}, (const uint8_t[]){}, 0)));
                                if (!mak_ir_adt_copy_checked((void *)(arena_case_171), sizeof(struct mak__f), UINT32_C(1225484184), (const void *)(rt_heap_obj_174)) ) {
                                    *(arena_case_171) = *(&mak_ir_default_obj__f);
                                }
                            }
                            else if ((scrut_172 != NULL && (scrut_172)->tag == mak_ir_tag_1)) {
                                int32_t mak_local_dk_56 = (scrut_172)->v.v1.f0;
                                struct mak__a * mak_local_dl_57 = (scrut_172)->v.v1.f1;
                                int64_t case_176 = 0;
                                int32_t scrut_177 = mak_local_dk_56 >= (int32_t)(INT64_C(48));
                                if ((scrut_177 == INT64_C(0))) {
                                    case_176 = INT64_C(0);
                                }
                                else if (1) {
                                    int64_t case_178 = 0;
                                    int32_t scrut_179 = mak_local_dk_56 <= (int32_t)(INT64_C(57));
                                    if ((scrut_179 == INT64_C(1))) {
                                        case_178 = INT64_C(1);
                                    }
                                    else if (1) {
                                        case_178 = INT64_C(0);
                                    }
                                    case_176 = case_178;
                                }
                                int64_t scrut_175 = case_176;
                                if ((scrut_175 == INT64_C(1))) {
                                    struct mak__d *arena_call_181 = (struct mak__d *)mak_ir_arena_alloc(sizeof(struct mak__d), _Alignof(struct mak__d));
                                    if (!arena_call_181) {
                                        mak_ir_recover("ADT result allocation failed");
                                        arena_call_181 = &mak_ir_default_obj__d;
                                     } else {
                                        struct mak__a *arena_obj_182 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
                                        if (!arena_obj_182) {
                                            mak_ir_recover("ADT object allocation failed");
                                            arena_obj_182 = &mak_ir_default_obj__a;
                                         } else {
                                            (arena_obj_182)->tag = mak_ir_tag_1;
                                            (arena_obj_182)->v.v1.f0 = mak_local_dk_56;
                                            (arena_obj_182)->v.v1.f1 = mak_local_dl_57;
                                        }
                                        mak_ir_fn_7(arena_obj_182, 0.00000000000000000, (struct mak__d *)(arena_call_181));
                                    }
                                    struct mak__d * scrut_180 = arena_call_181;
                                    if ((scrut_180 != NULL && (scrut_180)->tag == mak_ir_tag_6)) {
                                        struct mak__f *rt_heap_obj_183 = (struct mak__f *)(uintptr_t)((struct mak__f *)(uintptr_t)(mak_rt_call_ctor("NBad:_f", (const uint64_t[]){}, (const uint8_t[]){}, 0)));
                                        if (!mak_ir_adt_copy_checked((void *)(arena_case_171), sizeof(struct mak__f), UINT32_C(1225484184), (const void *)(rt_heap_obj_183)) ) {
                                            *(arena_case_171) = *(&mak_ir_default_obj__f);
                                        }
                                    }
                                    else if ((scrut_180 != NULL && (scrut_180)->tag == mak_ir_tag_7)) {
                                        double mak_local_dm_58 = (scrut_180)->v.v7.f0;
                                        struct mak__a * mak_local_dn_59 = (scrut_180)->v.v7.f1;
                                        if ((mak_local_dn_59 != NULL && (mak_local_dn_59)->tag == mak_ir_tag_1) && (mak_local_dn_59->v.v1.f0 == INT64_C(46))) {
                                            struct mak__a * mak_local_dp_60 = (mak_local_dn_59)->v.v1.f1;
                                            struct mak__d *arena_call_184 = (struct mak__d *)mak_ir_arena_alloc(sizeof(struct mak__d), _Alignof(struct mak__d));
                                            if (!arena_call_184) {
                                                mak_ir_recover("ADT result allocation failed");
                                                arena_call_184 = &mak_ir_default_obj__d;
                                             } else {
                                                mak_ir_fn_8(mak_local_dp_60, mak_local_dm_58, 0.10000000000000001, (struct mak__d *)(arena_call_184));
                                            }
                                            if (!mak_ir_adt_copy_checked((void *)(arena_case_171), sizeof(struct mak__f), UINT32_C(1225484184), (const void *)(uintptr_t)(arena_call_184))) {
                                                *(arena_case_171) = *(&mak_ir_default_obj__f);
                                            }
                                        }
                                        else if ((mak_local_dn_59 != NULL && (mak_local_dn_59)->tag == mak_ir_tag_1)) {
                                            int32_t mak_local_dq_61 = (mak_local_dn_59)->v.v1.f0;
                                            struct mak__a * mak_local_dr_62 = (mak_local_dn_59)->v.v1.f1;
                                            switch (mak_local_dq_61) {
                                                case INT64_C(101): {
                                                    struct mak__e *arena_call_186 = (struct mak__e *)mak_ir_arena_alloc(sizeof(struct mak__e), _Alignof(struct mak__e));
                                                    if (!arena_call_186) {
                                                        mak_ir_recover("ADT result allocation failed");
                                                        arena_call_186 = &mak_ir_default_obj__e;
                                                     } else {
                                                        mak_ir_fn_6(mak_local_dr_62, INT64_C(1), (struct mak__e *)(arena_call_186));
                                                    }
                                                    struct mak__e * scrut_185 = arena_call_186;
                                                    if ((scrut_185 != NULL && (scrut_185)->tag == mak_ir_tag_8)) {
                                                        struct mak__f *rt_heap_obj_187 = (struct mak__f *)(uintptr_t)((struct mak__f *)(uintptr_t)(mak_rt_call_ctor("NBad:_f", (const uint64_t[]){}, (const uint8_t[]){}, 0)));
                                                        if (!mak_ir_adt_copy_checked((void *)(arena_case_171), sizeof(struct mak__f), UINT32_C(1225484184), (const void *)(rt_heap_obj_187)) ) {
                                                            *(arena_case_171) = *(&mak_ir_default_obj__f);
                                                        }
                                                    }
                                                    else if ((scrut_185 != NULL && (scrut_185)->tag == mak_ir_tag_9)) {
                                                        int64_t mak_local_ds_63 = (scrut_185)->v.v9.f0;
                                                        struct mak__a * mak_local_dt_64 = (scrut_185)->v.v9.f1;
                                                        struct mak__d *arena_obj_188 = (struct mak__d *)mak_ir_arena_alloc(sizeof(struct mak__d), _Alignof(struct mak__d));
                                                        if (!arena_obj_188) {
                                                            mak_ir_recover("ADT object allocation failed");
                                                            arena_obj_188 = &mak_ir_default_obj__d;
                                                         } else {
                                                            (arena_obj_188)->tag = mak_ir_tag_7;
                                                            double eval_189 = mak_ir_fn_4(mak_local_ds_63);
                                                            (arena_obj_188)->v.v7.f0 = ((double)(mak_local_dm_58) * (double)(eval_189));
                                                            (arena_obj_188)->v.v7.f1 = mak_local_dt_64;
                                                        }
                                                        if (!mak_ir_adt_copy_checked((void *)(arena_case_171), sizeof(struct mak__f), UINT32_C(1225484184), (const void *)(uintptr_t)(arena_obj_188))) {
                                                            *(arena_case_171) = *(&mak_ir_default_obj__f);
                                                        }
                                                    }
                                                    else {
                                                        *(arena_case_171) = *(&mak_ir_default_obj__f);
                                                    }
                                                    break;
                                                }
                                                case INT64_C(69): {
                                                    struct mak__e *arena_call_191 = (struct mak__e *)mak_ir_arena_alloc(sizeof(struct mak__e), _Alignof(struct mak__e));
                                                    if (!arena_call_191) {
                                                        mak_ir_recover("ADT result allocation failed");
                                                        arena_call_191 = &mak_ir_default_obj__e;
                                                     } else {
                                                        mak_ir_fn_6(mak_local_dr_62, INT64_C(1), (struct mak__e *)(arena_call_191));
                                                    }
                                                    struct mak__e * scrut_190 = arena_call_191;
                                                    if ((scrut_190 != NULL && (scrut_190)->tag == mak_ir_tag_8)) {
                                                        struct mak__f *rt_heap_obj_192 = (struct mak__f *)(uintptr_t)((struct mak__f *)(uintptr_t)(mak_rt_call_ctor("NBad:_f", (const uint64_t[]){}, (const uint8_t[]){}, 0)));
                                                        if (!mak_ir_adt_copy_checked((void *)(arena_case_171), sizeof(struct mak__f), UINT32_C(1225484184), (const void *)(rt_heap_obj_192)) ) {
                                                            *(arena_case_171) = *(&mak_ir_default_obj__f);
                                                        }
                                                    }
                                                    else if ((scrut_190 != NULL && (scrut_190)->tag == mak_ir_tag_9)) {
                                                        int64_t mak_local_du_65 = (scrut_190)->v.v9.f0;
                                                        struct mak__a * mak_local_dv_66 = (scrut_190)->v.v9.f1;
                                                        struct mak__d *arena_obj_193 = (struct mak__d *)mak_ir_arena_alloc(sizeof(struct mak__d), _Alignof(struct mak__d));
                                                        if (!arena_obj_193) {
                                                            mak_ir_recover("ADT object allocation failed");
                                                            arena_obj_193 = &mak_ir_default_obj__d;
                                                         } else {
                                                            (arena_obj_193)->tag = mak_ir_tag_7;
                                                            double eval_194 = mak_ir_fn_4(mak_local_du_65);
                                                            (arena_obj_193)->v.v7.f0 = ((double)(mak_local_dm_58) * (double)(eval_194));
                                                            (arena_obj_193)->v.v7.f1 = mak_local_dv_66;
                                                        }
                                                        if (!mak_ir_adt_copy_checked((void *)(arena_case_171), sizeof(struct mak__f), UINT32_C(1225484184), (const void *)(uintptr_t)(arena_obj_193))) {
                                                            *(arena_case_171) = *(&mak_ir_default_obj__f);
                                                        }
                                                    }
                                                    else {
                                                        *(arena_case_171) = *(&mak_ir_default_obj__f);
                                                    }
                                                    break;
                                                }
                                                default: {
                                                    struct mak__d *arena_obj_195 = (struct mak__d *)mak_ir_arena_alloc(sizeof(struct mak__d), _Alignof(struct mak__d));
                                                    if (!arena_obj_195) {
                                                        mak_ir_recover("ADT object allocation failed");
                                                        arena_obj_195 = &mak_ir_default_obj__d;
                                                     } else {
                                                        (arena_obj_195)->tag = mak_ir_tag_7;
                                                        (arena_obj_195)->v.v7.f0 = mak_local_dm_58;
                                                        (arena_obj_195)->v.v7.f1 = mak_local_dn_59;
                                                    }
                                                    if (!mak_ir_adt_copy_checked((void *)(arena_case_171), sizeof(struct mak__f), UINT32_C(1225484184), (const void *)(uintptr_t)(arena_obj_195))) {
                                                        *(arena_case_171) = *(&mak_ir_default_obj__f);
                                                    }
                                                    break;
                                                }
                                            }
                                        }
                                        else if (1) {
                                            struct mak__d *arena_obj_196 = (struct mak__d *)mak_ir_arena_alloc(sizeof(struct mak__d), _Alignof(struct mak__d));
                                            if (!arena_obj_196) {
                                                mak_ir_recover("ADT object allocation failed");
                                                arena_obj_196 = &mak_ir_default_obj__d;
                                             } else {
                                                (arena_obj_196)->tag = mak_ir_tag_7;
                                                (arena_obj_196)->v.v7.f0 = mak_local_dm_58;
                                                (arena_obj_196)->v.v7.f1 = mak_local_dn_59;
                                            }
                                            if (!mak_ir_adt_copy_checked((void *)(arena_case_171), sizeof(struct mak__f), UINT32_C(1225484184), (const void *)(uintptr_t)(arena_obj_196))) {
                                                *(arena_case_171) = *(&mak_ir_default_obj__f);
                                            }
                                        }
                                    }
                                    else {
                                        *(arena_case_171) = *(&mak_ir_default_obj__f);
                                    }
                                }
                                else if (1) {
                                    if ((mak_local_dk_56 == INT64_C(46))) {
                                        if ((mak_local_dl_57 != NULL && (mak_local_dl_57)->tag == mak_ir_tag_1)) {
                                            int32_t mak_local_dw_67 = (mak_local_dl_57)->v.v1.f0;
                                            int64_t case_198 = 0;
                                            int32_t scrut_199 = mak_local_dw_67 >= (int32_t)(INT64_C(48));
                                            if ((scrut_199 == INT64_C(0))) {
                                                case_198 = INT64_C(0);
                                            }
                                            else if (1) {
                                                int64_t case_200 = 0;
                                                int32_t scrut_201 = mak_local_dw_67 <= (int32_t)(INT64_C(57));
                                                if ((scrut_201 == INT64_C(1))) {
                                                    case_200 = INT64_C(1);
                                                }
                                                else if (1) {
                                                    case_200 = INT64_C(0);
                                                }
                                                case_198 = case_200;
                                            }
                                            int64_t scrut_197 = case_198;
                                            if ((scrut_197 == INT64_C(1))) {
                                                struct mak__d *arena_call_202 = (struct mak__d *)mak_ir_arena_alloc(sizeof(struct mak__d), _Alignof(struct mak__d));
                                                if (!arena_call_202) {
                                                    mak_ir_recover("ADT result allocation failed");
                                                    arena_call_202 = &mak_ir_default_obj__d;
                                                 } else {
                                                    mak_ir_fn_8(mak_local_dl_57, 0.00000000000000000, 0.10000000000000001, (struct mak__d *)(arena_call_202));
                                                }
                                                if (!mak_ir_adt_copy_checked((void *)(arena_case_171), sizeof(struct mak__f), UINT32_C(1225484184), (const void *)(uintptr_t)(arena_call_202))) {
                                                    *(arena_case_171) = *(&mak_ir_default_obj__f);
                                                }
                                            }
                                            else if (1) {
                                                struct mak__f *rt_heap_obj_203 = (struct mak__f *)(uintptr_t)((struct mak__f *)(uintptr_t)(mak_rt_call_ctor("NBad:_f", (const uint64_t[]){}, (const uint8_t[]){}, 0)));
                                                if (!mak_ir_adt_copy_checked((void *)(arena_case_171), sizeof(struct mak__f), UINT32_C(1225484184), (const void *)(rt_heap_obj_203)) ) {
                                                    *(arena_case_171) = *(&mak_ir_default_obj__f);
                                                }
                                            }
                                        }
                                        else if (1) {
                                            struct mak__f *rt_heap_obj_204 = (struct mak__f *)(uintptr_t)((struct mak__f *)(uintptr_t)(mak_rt_call_ctor("NBad:_f", (const uint64_t[]){}, (const uint8_t[]){}, 0)));
                                            if (!mak_ir_adt_copy_checked((void *)(arena_case_171), sizeof(struct mak__f), UINT32_C(1225484184), (const void *)(rt_heap_obj_204)) ) {
                                                *(arena_case_171) = *(&mak_ir_default_obj__f);
                                            }
                                        }
                                    }
                                    else if (1) {
                                        struct mak__f *rt_heap_obj_205 = (struct mak__f *)(uintptr_t)((struct mak__f *)(uintptr_t)(mak_rt_call_ctor("NBad:_f", (const uint64_t[]){}, (const uint8_t[]){}, 0)));
                                        if (!mak_ir_adt_copy_checked((void *)(arena_case_171), sizeof(struct mak__f), UINT32_C(1225484184), (const void *)(rt_heap_obj_205)) ) {
                                            *(arena_case_171) = *(&mak_ir_default_obj__f);
                                        }
                                    }
                                }
                            }
                            else {
                                *(arena_case_171) = *(&mak_ir_default_obj__f);
                            }
                        }
                        struct mak__f * scrut_170 = arena_case_171;
                        if ((scrut_170 != NULL && (scrut_170)->tag == mak_ir_tag_11)) {
                            *(arena_case_130) = mak_ir_ctor_obj_11;
                        }
                        else if ((scrut_170 != NULL && (scrut_170)->tag == mak_ir_tag_10)) {
                            double mak_local_ga_68 = (scrut_170)->v.v10.f0;
                            struct mak__a * mak_local_gb_69 = (scrut_170)->v.v10.f1;
                            (arena_case_130)->tag = mak_ir_tag_10;
                            (arena_case_130)->v.v10.f0 = mak_local_ga_68;
                            (arena_case_130)->v.v10.f1 = mak_local_gb_69;
                        }
                        else {
                            *(arena_case_130) = *(&mak_ir_default_obj__f);
                        }
                    }
                }
            }
            else {
                *(arena_case_130) = *(&mak_ir_default_obj__f);
            }
        }
        struct mak__f * scrut_129 = arena_case_130;
        if ((scrut_129 != NULL && (scrut_129)->tag == mak_ir_tag_11)) {
            *(arena_case_128) = mak_ir_ctor_obj_11;
        }
        else if ((scrut_129 != NULL && (scrut_129)->tag == mak_ir_tag_10)) {
            double mak_local_gi_70 = (scrut_129)->v.v10.f0;
            struct mak__a * mak_local_gj_71 = (scrut_129)->v.v10.f1;
            mak_ir_fn_10(mak_local_gi_70, mak_local_gj_71, (struct mak__f *)(arena_case_128));
        }
        else {
            *(arena_case_128) = *(&mak_ir_default_obj__f);
        }
    }
    struct mak__f * scrut_127 = arena_case_128;
    if ((scrut_127 != NULL && (scrut_127)->tag == mak_ir_tag_11)) {
        *(out) = mak_ir_ctor_obj_11;
    }
    else if ((scrut_127 != NULL && (scrut_127)->tag == mak_ir_tag_10)) {
        double mak_local_gl_72 = (scrut_127)->v.v10.f0;
        struct mak__a * mak_local_gm_73 = (scrut_127)->v.v10.f1;
        struct mak__a *arena_call_207 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
        if (!arena_call_207) {
            mak_ir_recover("ADT result allocation failed");
            arena_call_207 = &mak_ir_default_obj__a;
         } else {
            mak_ir_fn_2(mak_local_gm_73, (struct mak__a *)(arena_call_207));
        }
        struct mak__a * scrut_206 = arena_call_207;
        if ((scrut_206 != NULL && (scrut_206)->tag == mak_ir_tag_1) && (scrut_206->v.v1.f0 == INT64_C(94))) {
            struct mak__a * mak_local_gn_74 = (scrut_206)->v.v1.f1;
            struct mak__f *arena_call_209 = (struct mak__f *)mak_ir_arena_alloc(sizeof(struct mak__f), _Alignof(struct mak__f));
            if (!arena_call_209) {
                mak_ir_recover("ADT result allocation failed");
                arena_call_209 = &mak_ir_default_obj__f;
             } else {
                mak_ir_fn_12(mak_local_gn_74, (struct mak__f *)(arena_call_209));
            }
            struct mak__f * scrut_208 = arena_call_209;
            if ((scrut_208 != NULL && (scrut_208)->tag == mak_ir_tag_11)) {
                *(out) = mak_ir_ctor_obj_11;
            }
            else if ((scrut_208 != NULL && (scrut_208)->tag == mak_ir_tag_10)) {
                double mak_local_go_75 = (scrut_208)->v.v10.f0;
                struct mak__a * mak_local_gp_76 = (scrut_208)->v.v10.f1;
                (out)->tag = mak_ir_tag_10;
                (out)->v.v10.f0 = mak_ir_ext_14(mak_local_gl_72, mak_local_go_75);
                (out)->v.v10.f1 = mak_local_gp_76;
            }
            else {
                *(out) = *(&mak_ir_default_obj__f);
            }
        }
        else if (1) {
            (out)->tag = mak_ir_tag_10;
            (out)->v.v10.f0 = mak_local_gl_72;
            (out)->v.v10.f1 = mak_local_gm_73;
        }
    }
    else {
        *(out) = *(&mak_ir_default_obj__f);
    }
    return;
}

static inline void mak_ir_fn_12(struct mak__a * a0, struct mak__f *out) {
    for (;;) {
        struct mak__a *arena_call_211 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
        if (!arena_call_211) {
            mak_ir_recover("ADT result allocation failed");
            arena_call_211 = &mak_ir_default_obj__a;
         } else {
            mak_ir_fn_2(a0, (struct mak__a *)(arena_call_211));
        }
        struct mak__a * scrut_210 = arena_call_211;
        if ((scrut_210 != NULL && (scrut_210)->tag == mak_ir_tag_0)) {
            *(out) = mak_ir_ctor_obj_11;
        }
        else if ((scrut_210 != NULL && (scrut_210)->tag == mak_ir_tag_1)) {
            int32_t mak_local_gr_77 = (scrut_210)->v.v1.f0;
            struct mak__a * mak_local_gs_78 = (scrut_210)->v.v1.f1;
            switch (mak_local_gr_77) {
                case INT64_C(43): {
                    struct mak__a * tailarg_212 = mak_local_gs_78;
                    a0 = tailarg_212;
                    continue;
                }
                case INT64_C(45): {
                    struct mak__f *arena_call_214 = (struct mak__f *)mak_ir_arena_alloc(sizeof(struct mak__f), _Alignof(struct mak__f));
                    if (!arena_call_214) {
                        mak_ir_recover("ADT result allocation failed");
                        arena_call_214 = &mak_ir_default_obj__f;
                     } else {
                        mak_ir_fn_12(mak_local_gs_78, (struct mak__f *)(arena_call_214));
                    }
                    struct mak__f * scrut_213 = arena_call_214;
                    if ((scrut_213 != NULL && (scrut_213)->tag == mak_ir_tag_11)) {
                        *(out) = mak_ir_ctor_obj_11;
                    }
                    else if ((scrut_213 != NULL && (scrut_213)->tag == mak_ir_tag_10)) {
                        double mak_local_gt_79 = (scrut_213)->v.v10.f0;
                        struct mak__a * mak_local_gu_80 = (scrut_213)->v.v10.f1;
                        (out)->tag = mak_ir_tag_10;
                        (out)->v.v10.f0 = ((double)(0.00000000000000000) - (double)(mak_local_gt_79));
                        (out)->v.v10.f1 = mak_local_gu_80;
                    }
                    else {
                        *(out) = *(&mak_ir_default_obj__f);
                    }
                    break;
                }
                default: {
                    mak_ir_fn_11(a0, (struct mak__f *)(out));
                    break;
                }
            }
        }
        else {
            *(out) = *(&mak_ir_default_obj__f);
        }
        return;
    }
}

static inline void mak_ir_fn_13(double a0, struct mak__a * a1, struct mak__f *out) {
    for (;;) {
        struct mak__a *arena_call_216 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
        if (!arena_call_216) {
            mak_ir_recover("ADT result allocation failed");
            arena_call_216 = &mak_ir_default_obj__a;
         } else {
            mak_ir_fn_2(a1, (struct mak__a *)(arena_call_216));
        }
        struct mak__a * scrut_215 = arena_call_216;
        if ((scrut_215 != NULL && (scrut_215)->tag == mak_ir_tag_0)) {
            (out)->tag = mak_ir_tag_10;
            (out)->v.v10.f0 = a0;
            (out)->v.v10.f1 = &mak_ir_ctor_obj_0;
        }
        else if ((scrut_215 != NULL && (scrut_215)->tag == mak_ir_tag_1)) {
            int32_t mak_local_gx_81 = (scrut_215)->v.v1.f0;
            struct mak__a * mak_local_gy_82 = (scrut_215)->v.v1.f1;
            switch (mak_local_gx_81) {
                case INT64_C(42): {
                    struct mak__f *arena_call_218 = (struct mak__f *)mak_ir_arena_alloc(sizeof(struct mak__f), _Alignof(struct mak__f));
                    if (!arena_call_218) {
                        mak_ir_recover("ADT result allocation failed");
                        arena_call_218 = &mak_ir_default_obj__f;
                     } else {
                        mak_ir_fn_11(mak_local_gy_82, (struct mak__f *)(arena_call_218));
                    }
                    struct mak__f * scrut_217 = arena_call_218;
                    if ((scrut_217 != NULL && (scrut_217)->tag == mak_ir_tag_11)) {
                        *(out) = mak_ir_ctor_obj_11;
                    }
                    else if ((scrut_217 != NULL && (scrut_217)->tag == mak_ir_tag_10)) {
                        double mak_local_gz_83 = (scrut_217)->v.v10.f0;
                        struct mak__a * mak_local_ha_84 = (scrut_217)->v.v10.f1;
                        double tailarg_219 = ((double)(a0) * (double)(mak_local_gz_83));
                        struct mak__a * tailarg_220 = mak_local_ha_84;
                        a0 = tailarg_219;
                        a1 = tailarg_220;
                        continue;
                    }
                    else {
                        *(out) = *(&mak_ir_default_obj__f);
                    }
                    break;
                }
                case INT64_C(47): {
                    if ((mak_local_gy_82 != NULL && (mak_local_gy_82)->tag == mak_ir_tag_1) && (mak_local_gy_82->v.v1.f0 == INT64_C(47))) {
                        struct mak__a * mak_local_hb_85 = (mak_local_gy_82)->v.v1.f1;
                        struct mak__f *arena_call_222 = (struct mak__f *)mak_ir_arena_alloc(sizeof(struct mak__f), _Alignof(struct mak__f));
                        if (!arena_call_222) {
                            mak_ir_recover("ADT result allocation failed");
                            arena_call_222 = &mak_ir_default_obj__f;
                         } else {
                            mak_ir_fn_11(mak_local_hb_85, (struct mak__f *)(arena_call_222));
                        }
                        struct mak__f * scrut_221 = arena_call_222;
                        if ((scrut_221 != NULL && (scrut_221)->tag == mak_ir_tag_11)) {
                            *(out) = mak_ir_ctor_obj_11;
                        }
                        else if ((scrut_221 != NULL && (scrut_221)->tag == mak_ir_tag_10)) {
                            double mak_local_hc_86 = (scrut_221)->v.v10.f0;
                            struct mak__a * mak_local_hd_87 = (scrut_221)->v.v10.f1;
                            int32_t scrut_223 = (int32_t)(mak_local_hc_86) == (int32_t)(0.00000000000000000);
                            if ((scrut_223 == INT64_C(1))) {
                                *(out) = mak_ir_ctor_obj_11;
                            }
                            else if (1) {
                                double tailarg_224 = mak_ir_ext_8(((double)(a0) / (double)(mak_local_hc_86)));
                                struct mak__a * tailarg_225 = mak_local_hd_87;
                                a0 = tailarg_224;
                                a1 = tailarg_225;
                                continue;
                            }
                        }
                        else {
                            *(out) = *(&mak_ir_default_obj__f);
                        }
                    }
                    else if (1) {
                        struct mak__f *arena_call_227 = (struct mak__f *)mak_ir_arena_alloc(sizeof(struct mak__f), _Alignof(struct mak__f));
                        if (!arena_call_227) {
                            mak_ir_recover("ADT result allocation failed");
                            arena_call_227 = &mak_ir_default_obj__f;
                         } else {
                            mak_ir_fn_11(mak_local_gy_82, (struct mak__f *)(arena_call_227));
                        }
                        struct mak__f * scrut_226 = arena_call_227;
                        if ((scrut_226 != NULL && (scrut_226)->tag == mak_ir_tag_11)) {
                            *(out) = mak_ir_ctor_obj_11;
                        }
                        else if ((scrut_226 != NULL && (scrut_226)->tag == mak_ir_tag_10)) {
                            double mak_local_he_88 = (scrut_226)->v.v10.f0;
                            struct mak__a * mak_local_hf_89 = (scrut_226)->v.v10.f1;
                            int32_t scrut_228 = (int32_t)(mak_local_he_88) == (int32_t)(0.00000000000000000);
                            if ((scrut_228 == INT64_C(1))) {
                                *(out) = mak_ir_ctor_obj_11;
                            }
                            else if (1) {
                                double tailarg_229 = ((double)(a0) / (double)(mak_local_he_88));
                                struct mak__a * tailarg_230 = mak_local_hf_89;
                                a0 = tailarg_229;
                                a1 = tailarg_230;
                                continue;
                            }
                        }
                        else {
                            *(out) = *(&mak_ir_default_obj__f);
                        }
                    }
                    break;
                }
                case INT64_C(37): {
                    struct mak__f *arena_call_232 = (struct mak__f *)mak_ir_arena_alloc(sizeof(struct mak__f), _Alignof(struct mak__f));
                    if (!arena_call_232) {
                        mak_ir_recover("ADT result allocation failed");
                        arena_call_232 = &mak_ir_default_obj__f;
                     } else {
                        mak_ir_fn_11(mak_local_gy_82, (struct mak__f *)(arena_call_232));
                    }
                    struct mak__f * scrut_231 = arena_call_232;
                    if ((scrut_231 != NULL && (scrut_231)->tag == mak_ir_tag_11)) {
                        *(out) = mak_ir_ctor_obj_11;
                    }
                    else if ((scrut_231 != NULL && (scrut_231)->tag == mak_ir_tag_10)) {
                        double mak_local_hg_90 = (scrut_231)->v.v10.f0;
                        struct mak__a * mak_local_hh_91 = (scrut_231)->v.v10.f1;
                        int32_t scrut_233 = (int32_t)(mak_local_hg_90) == (int32_t)(0.00000000000000000);
                        if ((scrut_233 == INT64_C(1))) {
                            *(out) = mak_ir_ctor_obj_11;
                        }
                        else if (1) {
                            double tailarg_234 = mak_ir_ext_12(a0, mak_local_hg_90);
                            struct mak__a * tailarg_235 = mak_local_hh_91;
                            a0 = tailarg_234;
                            a1 = tailarg_235;
                            continue;
                        }
                    }
                    else {
                        *(out) = *(&mak_ir_default_obj__f);
                    }
                    break;
                }
                default: {
                    int64_t case_237 = 0;
                    int32_t scrut_238 = mak_local_gx_81 == (int32_t)(INT64_C(95));
                    if ((scrut_238 == INT64_C(1))) {
                        case_237 = INT64_C(1);
                    }
                    else if (1) {
                        int64_t case_239 = 0;
                        int32_t scrut_240 = mak_local_gx_81 >= (int32_t)(INT64_C(65));
                        if ((scrut_240 == INT64_C(0))) {
                            int64_t case_241 = 0;
                            int32_t scrut_242 = mak_local_gx_81 >= (int32_t)(INT64_C(97));
                            if ((scrut_242 == INT64_C(0))) {
                                case_241 = INT64_C(0);
                            }
                            else if (1) {
                                int64_t case_243 = 0;
                                int32_t scrut_244 = mak_local_gx_81 <= (int32_t)(INT64_C(122));
                                if ((scrut_244 == INT64_C(1))) {
                                    case_243 = INT64_C(1);
                                }
                                else if (1) {
                                    case_243 = INT64_C(0);
                                }
                                case_241 = case_243;
                            }
                            case_239 = case_241;
                        }
                        else if (1) {
                            int64_t case_245 = 0;
                            int32_t scrut_246 = mak_local_gx_81 <= (int32_t)(INT64_C(90));
                            if ((scrut_246 == INT64_C(1))) {
                                case_245 = INT64_C(1);
                            }
                            else if (1) {
                                case_245 = INT64_C(0);
                            }
                            case_239 = case_245;
                        }
                        case_237 = case_239;
                    }
                    int64_t scrut_236 = case_237;
                    if ((scrut_236 == INT64_C(1))) {
                        struct mak__f *arena_call_248 = (struct mak__f *)mak_ir_arena_alloc(sizeof(struct mak__f), _Alignof(struct mak__f));
                        if (!arena_call_248) {
                            mak_ir_recover("ADT result allocation failed");
                            arena_call_248 = &mak_ir_default_obj__f;
                         } else {
                            mak_ir_fn_11(a1, (struct mak__f *)(arena_call_248));
                        }
                        struct mak__f * scrut_247 = arena_call_248;
                        if ((scrut_247 != NULL && (scrut_247)->tag == mak_ir_tag_11)) {
                            (out)->tag = mak_ir_tag_10;
                            (out)->v.v10.f0 = a0;
                            (out)->v.v10.f1 = a1;
                        }
                        else if ((scrut_247 != NULL && (scrut_247)->tag == mak_ir_tag_10)) {
                            double mak_local_hi_92 = (scrut_247)->v.v10.f0;
                            struct mak__a * mak_local_hj_93 = (scrut_247)->v.v10.f1;
                            double tailarg_249 = ((double)(a0) * (double)(mak_local_hi_92));
                            struct mak__a * tailarg_250 = mak_local_hj_93;
                            a0 = tailarg_249;
                            a1 = tailarg_250;
                            continue;
                        }
                        else {
                            *(out) = *(&mak_ir_default_obj__f);
                        }
                    }
                    else if (1) {
                        int64_t case_252 = 0;
                        int32_t scrut_253 = mak_local_gx_81 >= (int32_t)(INT64_C(48));
                        if ((scrut_253 == INT64_C(0))) {
                            case_252 = INT64_C(0);
                        }
                        else if (1) {
                            int64_t case_254 = 0;
                            int32_t scrut_255 = mak_local_gx_81 <= (int32_t)(INT64_C(57));
                            if ((scrut_255 == INT64_C(1))) {
                                case_254 = INT64_C(1);
                            }
                            else if (1) {
                                case_254 = INT64_C(0);
                            }
                            case_252 = case_254;
                        }
                        int64_t scrut_251 = case_252;
                        if ((scrut_251 == INT64_C(1))) {
                            struct mak__f *arena_call_257 = (struct mak__f *)mak_ir_arena_alloc(sizeof(struct mak__f), _Alignof(struct mak__f));
                            if (!arena_call_257) {
                                mak_ir_recover("ADT result allocation failed");
                                arena_call_257 = &mak_ir_default_obj__f;
                             } else {
                                mak_ir_fn_11(a1, (struct mak__f *)(arena_call_257));
                            }
                            struct mak__f * scrut_256 = arena_call_257;
                            if ((scrut_256 != NULL && (scrut_256)->tag == mak_ir_tag_11)) {
                                (out)->tag = mak_ir_tag_10;
                                (out)->v.v10.f0 = a0;
                                (out)->v.v10.f1 = a1;
                            }
                            else if ((scrut_256 != NULL && (scrut_256)->tag == mak_ir_tag_10)) {
                                double mak_local_hk_94 = (scrut_256)->v.v10.f0;
                                struct mak__a * mak_local_hl_95 = (scrut_256)->v.v10.f1;
                                double tailarg_258 = ((double)(a0) * (double)(mak_local_hk_94));
                                struct mak__a * tailarg_259 = mak_local_hl_95;
                                a0 = tailarg_258;
                                a1 = tailarg_259;
                                continue;
                            }
                            else {
                                *(out) = *(&mak_ir_default_obj__f);
                            }
                        }
                        else if (1) {
                            switch (mak_local_gx_81) {
                                case INT64_C(46): {
                                    struct mak__f *arena_call_261 = (struct mak__f *)mak_ir_arena_alloc(sizeof(struct mak__f), _Alignof(struct mak__f));
                                    if (!arena_call_261) {
                                        mak_ir_recover("ADT result allocation failed");
                                        arena_call_261 = &mak_ir_default_obj__f;
                                     } else {
                                        mak_ir_fn_11(a1, (struct mak__f *)(arena_call_261));
                                    }
                                    struct mak__f * scrut_260 = arena_call_261;
                                    if ((scrut_260 != NULL && (scrut_260)->tag == mak_ir_tag_11)) {
                                        (out)->tag = mak_ir_tag_10;
                                        (out)->v.v10.f0 = a0;
                                        (out)->v.v10.f1 = a1;
                                    }
                                    else if ((scrut_260 != NULL && (scrut_260)->tag == mak_ir_tag_10)) {
                                        double mak_local_hm_96 = (scrut_260)->v.v10.f0;
                                        struct mak__a * mak_local_hn_97 = (scrut_260)->v.v10.f1;
                                        double tailarg_262 = ((double)(a0) * (double)(mak_local_hm_96));
                                        struct mak__a * tailarg_263 = mak_local_hn_97;
                                        a0 = tailarg_262;
                                        a1 = tailarg_263;
                                        continue;
                                    }
                                    else {
                                        *(out) = *(&mak_ir_default_obj__f);
                                    }
                                    break;
                                }
                                case INT64_C(40): {
                                    struct mak__f *arena_call_265 = (struct mak__f *)mak_ir_arena_alloc(sizeof(struct mak__f), _Alignof(struct mak__f));
                                    if (!arena_call_265) {
                                        mak_ir_recover("ADT result allocation failed");
                                        arena_call_265 = &mak_ir_default_obj__f;
                                     } else {
                                        mak_ir_fn_11(a1, (struct mak__f *)(arena_call_265));
                                    }
                                    struct mak__f * scrut_264 = arena_call_265;
                                    if ((scrut_264 != NULL && (scrut_264)->tag == mak_ir_tag_11)) {
                                        (out)->tag = mak_ir_tag_10;
                                        (out)->v.v10.f0 = a0;
                                        (out)->v.v10.f1 = a1;
                                    }
                                    else if ((scrut_264 != NULL && (scrut_264)->tag == mak_ir_tag_10)) {
                                        double mak_local_ho_98 = (scrut_264)->v.v10.f0;
                                        struct mak__a * mak_local_hp_99 = (scrut_264)->v.v10.f1;
                                        double tailarg_266 = ((double)(a0) * (double)(mak_local_ho_98));
                                        struct mak__a * tailarg_267 = mak_local_hp_99;
                                        a0 = tailarg_266;
                                        a1 = tailarg_267;
                                        continue;
                                    }
                                    else {
                                        *(out) = *(&mak_ir_default_obj__f);
                                    }
                                    break;
                                }
                                default: {
                                    (out)->tag = mak_ir_tag_10;
                                    (out)->v.v10.f0 = a0;
                                    (out)->v.v10.f1 = a1;
                                    break;
                                }
                            }
                        }
                    }
                    break;
                }
            }
        }
        else {
            *(out) = *(&mak_ir_default_obj__f);
        }
        return;
    }
}

static inline void mak_ir_fn_14(struct mak__a * a0, struct mak__f *out) {
    struct mak__f *arena_call_269 = (struct mak__f *)mak_ir_arena_alloc(sizeof(struct mak__f), _Alignof(struct mak__f));
    if (!arena_call_269) {
        mak_ir_recover("ADT result allocation failed");
        arena_call_269 = &mak_ir_default_obj__f;
     } else {
        mak_ir_fn_11(a0, (struct mak__f *)(arena_call_269));
    }
    struct mak__f * scrut_268 = arena_call_269;
    if ((scrut_268 != NULL && (scrut_268)->tag == mak_ir_tag_11)) {
        *(out) = mak_ir_ctor_obj_11;
    }
    else if ((scrut_268 != NULL && (scrut_268)->tag == mak_ir_tag_10)) {
        double mak_local_hr_100 = (scrut_268)->v.v10.f0;
        struct mak__a * mak_local_hs_101 = (scrut_268)->v.v10.f1;
        mak_ir_fn_13(mak_local_hr_100, mak_local_hs_101, (struct mak__f *)(out));
    }
    else {
        *(out) = *(&mak_ir_default_obj__f);
    }
    return;
}

static inline void mak_ir_fn_15(double a0, struct mak__a * a1, struct mak__f *out) {
    for (;;) {
        struct mak__a *arena_call_271 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
        if (!arena_call_271) {
            mak_ir_recover("ADT result allocation failed");
            arena_call_271 = &mak_ir_default_obj__a;
         } else {
            mak_ir_fn_2(a1, (struct mak__a *)(arena_call_271));
        }
        struct mak__a * scrut_270 = arena_call_271;
        if ((scrut_270 != NULL && (scrut_270)->tag == mak_ir_tag_0)) {
            (out)->tag = mak_ir_tag_10;
            (out)->v.v10.f0 = a0;
            (out)->v.v10.f1 = &mak_ir_ctor_obj_0;
        }
        else if ((scrut_270 != NULL && (scrut_270)->tag == mak_ir_tag_1)) {
            int32_t mak_local_hv_102 = (scrut_270)->v.v1.f0;
            struct mak__a * mak_local_hw_103 = (scrut_270)->v.v1.f1;
            switch (mak_local_hv_102) {
                case INT64_C(43): {
                    struct mak__f *arena_call_273 = (struct mak__f *)mak_ir_arena_alloc(sizeof(struct mak__f), _Alignof(struct mak__f));
                    if (!arena_call_273) {
                        mak_ir_recover("ADT result allocation failed");
                        arena_call_273 = &mak_ir_default_obj__f;
                     } else {
                        mak_ir_fn_14(mak_local_hw_103, (struct mak__f *)(arena_call_273));
                    }
                    struct mak__f * scrut_272 = arena_call_273;
                    if ((scrut_272 != NULL && (scrut_272)->tag == mak_ir_tag_11)) {
                        *(out) = mak_ir_ctor_obj_11;
                    }
                    else if ((scrut_272 != NULL && (scrut_272)->tag == mak_ir_tag_10)) {
                        double mak_local_hx_104 = (scrut_272)->v.v10.f0;
                        struct mak__a * mak_local_hy_105 = (scrut_272)->v.v10.f1;
                        double tailarg_274 = ((double)(a0) + (double)(mak_local_hx_104));
                        struct mak__a * tailarg_275 = mak_local_hy_105;
                        a0 = tailarg_274;
                        a1 = tailarg_275;
                        continue;
                    }
                    else {
                        *(out) = *(&mak_ir_default_obj__f);
                    }
                    break;
                }
                case INT64_C(45): {
                    struct mak__f *arena_call_277 = (struct mak__f *)mak_ir_arena_alloc(sizeof(struct mak__f), _Alignof(struct mak__f));
                    if (!arena_call_277) {
                        mak_ir_recover("ADT result allocation failed");
                        arena_call_277 = &mak_ir_default_obj__f;
                     } else {
                        mak_ir_fn_14(mak_local_hw_103, (struct mak__f *)(arena_call_277));
                    }
                    struct mak__f * scrut_276 = arena_call_277;
                    if ((scrut_276 != NULL && (scrut_276)->tag == mak_ir_tag_11)) {
                        *(out) = mak_ir_ctor_obj_11;
                    }
                    else if ((scrut_276 != NULL && (scrut_276)->tag == mak_ir_tag_10)) {
                        double mak_local_hz_106 = (scrut_276)->v.v10.f0;
                        struct mak__a * mak_local_ia_107 = (scrut_276)->v.v10.f1;
                        double tailarg_278 = ((double)(a0) - (double)(mak_local_hz_106));
                        struct mak__a * tailarg_279 = mak_local_ia_107;
                        a0 = tailarg_278;
                        a1 = tailarg_279;
                        continue;
                    }
                    else {
                        *(out) = *(&mak_ir_default_obj__f);
                    }
                    break;
                }
                default: {
                    (out)->tag = mak_ir_tag_10;
                    (out)->v.v10.f0 = a0;
                    (out)->v.v10.f1 = a1;
                    break;
                }
            }
        }
        else {
            *(out) = *(&mak_ir_default_obj__f);
        }
        return;
    }
}

static inline void mak_ir_fn_16(struct mak__a * a0, struct mak__f *out) {
    struct mak__f *arena_call_281 = (struct mak__f *)mak_ir_arena_alloc(sizeof(struct mak__f), _Alignof(struct mak__f));
    if (!arena_call_281) {
        mak_ir_recover("ADT result allocation failed");
        arena_call_281 = &mak_ir_default_obj__f;
     } else {
        mak_ir_fn_14(a0, (struct mak__f *)(arena_call_281));
    }
    struct mak__f * scrut_280 = arena_call_281;
    if ((scrut_280 != NULL && (scrut_280)->tag == mak_ir_tag_11)) {
        *(out) = mak_ir_ctor_obj_11;
    }
    else if ((scrut_280 != NULL && (scrut_280)->tag == mak_ir_tag_10)) {
        double mak_local_ic_108 = (scrut_280)->v.v10.f0;
        struct mak__a * mak_local_id_109 = (scrut_280)->v.v10.f1;
        mak_ir_fn_15(mak_local_ic_108, mak_local_id_109, (struct mak__f *)(out));
    }
    else {
        *(out) = *(&mak_ir_default_obj__f);
    }
    return;
}

static inline void mak_ir_fn_17(void) {
    for (;;) {
        mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)(">>> "), mak_ir_ext_3((const void *)(">>> ")));
        struct mak__b *arena_case_283 = (struct mak__b *)mak_ir_arena_alloc(sizeof(struct mak__b), _Alignof(struct mak__b));
        if (!arena_case_283) {
            mak_ir_recover("ADT case-result allocation failed");
            arena_case_283 = &mak_ir_default_obj__b;
         } else {
            int32_t scrut_284 = mak_ir_ext_0();
            if (1) {
                int32_t mak_local_bd_110 = scrut_284;
                int32_t scrut_285 = mak_local_bd_110 < (int32_t)(INT64_C(0));
                if ((scrut_285 == INT64_C(1))) {
                    *(arena_case_283) = mak_ir_ctor_obj_2;
                }
                else if (1) {
                    int32_t scrut_286 = mak_local_bd_110 == (int32_t)(INT64_C(10));
                    if ((scrut_286 == INT64_C(1))) {
                        (arena_case_283)->tag = mak_ir_tag_3;
                        (arena_case_283)->v.v3.f0 = &mak_ir_ctor_obj_0;
                    }
                    else if (1) {
                        struct mak__a *arena_obj_287 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
                        if (!arena_obj_287) {
                            mak_ir_recover("ADT object allocation failed");
                            arena_obj_287 = &mak_ir_default_obj__a;
                         } else {
                            (arena_obj_287)->tag = mak_ir_tag_1;
                            (arena_obj_287)->v.v1.f0 = mak_local_bd_110;
                            (arena_obj_287)->v.v1.f1 = &mak_ir_ctor_obj_0;
                        }
                        mak_ir_fn_1(arena_obj_287, (struct mak__b *)(arena_case_283));
                    }
                }
            }
        }
        struct mak__b * scrut_282 = arena_case_283;
        if ((scrut_282 != NULL && (scrut_282)->tag == mak_ir_tag_2)) {
            mak_ir_ext_1((int32_t)(INT64_C(10)));
            return;
        }
        else if ((scrut_282 != NULL && (scrut_282)->tag == mak_ir_tag_3)) {
            struct mak__a * mak_local_ii_111 = (scrut_282)->v.v3.f0;
            int64_t case_289 = 0;
            struct mak__a *arena_call_291 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
            if (!arena_call_291) {
                mak_ir_recover("ADT result allocation failed");
                arena_call_291 = &mak_ir_default_obj__a;
             } else {
                mak_ir_fn_2(mak_local_ii_111, (struct mak__a *)(arena_call_291));
            }
            struct mak__a * scrut_290 = arena_call_291;
            if ((scrut_290 != NULL && (scrut_290)->tag == mak_ir_tag_0)) {
                case_289 = INT64_C(0);
            }
            else if ((scrut_290 != NULL && (scrut_290)->tag == mak_ir_tag_1) && (scrut_290->v.v1.f0 == INT64_C(101))) {
                struct mak__a * mak_local_bm_112 = (scrut_290)->v.v1.f1;
                int64_t case_292 = 0;
                if ((mak_local_bm_112 != NULL && (mak_local_bm_112)->tag == mak_ir_tag_1) && (mak_local_bm_112->v.v1.f0 == INT64_C(120))) {
                    struct mak__a * mak_local_bn_113 = (mak_local_bm_112)->v.v1.f1;
                    int64_t case_293 = 0;
                    if ((mak_local_bn_113 != NULL && (mak_local_bn_113)->tag == mak_ir_tag_1) && (mak_local_bn_113->v.v1.f0 == INT64_C(105))) {
                        struct mak__a * mak_local_bo_114 = (mak_local_bn_113)->v.v1.f1;
                        int64_t case_294 = 0;
                        if ((mak_local_bo_114 != NULL && (mak_local_bo_114)->tag == mak_ir_tag_1) && (mak_local_bo_114->v.v1.f0 == INT64_C(116))) {
                            struct mak__a * mak_local_bp_115 = (mak_local_bo_114)->v.v1.f1;
                            int64_t case_295 = 0;
                            struct mak__a *arena_call_297 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
                            if (!arena_call_297) {
                                mak_ir_recover("ADT result allocation failed");
                                arena_call_297 = &mak_ir_default_obj__a;
                             } else {
                                mak_ir_fn_2(mak_local_bp_115, (struct mak__a *)(arena_call_297));
                            }
                            struct mak__a * scrut_296 = arena_call_297;
                            if ((scrut_296 != NULL && (scrut_296)->tag == mak_ir_tag_0)) {
                                case_295 = INT64_C(1);
                            }
                            else if (1) {
                                case_295 = INT64_C(0);
                            }
                            case_294 = case_295;
                        }
                        else if (1) {
                            case_294 = INT64_C(0);
                        }
                        case_293 = case_294;
                    }
                    else if (1) {
                        case_293 = INT64_C(0);
                    }
                    case_292 = case_293;
                }
                else if (1) {
                    case_292 = INT64_C(0);
                }
                case_289 = case_292;
            }
            else if (1) {
                case_289 = INT64_C(0);
            }
            int64_t scrut_288 = case_289;
            if ((scrut_288 == INT64_C(1))) {
                mak_ir_ext_1((int32_t)(INT64_C(10)));
                return;
            }
            else if (1) {
                int64_t case_299 = 0;
                struct mak__a *arena_call_301 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
                if (!arena_call_301) {
                    mak_ir_recover("ADT result allocation failed");
                    arena_call_301 = &mak_ir_default_obj__a;
                 } else {
                    mak_ir_fn_2(mak_local_ii_111, (struct mak__a *)(arena_call_301));
                }
                struct mak__a * scrut_300 = arena_call_301;
                if ((scrut_300 != NULL && (scrut_300)->tag == mak_ir_tag_1) && (scrut_300->v.v1.f0 == INT64_C(63))) {
                    struct mak__a * mak_local_br_116 = (scrut_300)->v.v1.f1;
                    int64_t case_302 = 0;
                    struct mak__a *arena_call_304 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
                    if (!arena_call_304) {
                        mak_ir_recover("ADT result allocation failed");
                        arena_call_304 = &mak_ir_default_obj__a;
                     } else {
                        mak_ir_fn_2(mak_local_br_116, (struct mak__a *)(arena_call_304));
                    }
                    struct mak__a * scrut_303 = arena_call_304;
                    if ((scrut_303 != NULL && (scrut_303)->tag == mak_ir_tag_0)) {
                        case_302 = INT64_C(1);
                    }
                    else if (1) {
                        case_302 = INT64_C(0);
                    }
                    case_299 = case_302;
                }
                else if ((scrut_300 != NULL && (scrut_300)->tag == mak_ir_tag_1) && (scrut_300->v.v1.f0 == INT64_C(104))) {
                    struct mak__a * mak_local_bs_117 = (scrut_300)->v.v1.f1;
                    int64_t case_305 = 0;
                    if ((mak_local_bs_117 != NULL && (mak_local_bs_117)->tag == mak_ir_tag_1) && (mak_local_bs_117->v.v1.f0 == INT64_C(101))) {
                        struct mak__a * mak_local_bt_118 = (mak_local_bs_117)->v.v1.f1;
                        int64_t case_306 = 0;
                        if ((mak_local_bt_118 != NULL && (mak_local_bt_118)->tag == mak_ir_tag_1) && (mak_local_bt_118->v.v1.f0 == INT64_C(108))) {
                            struct mak__a * mak_local_bu_119 = (mak_local_bt_118)->v.v1.f1;
                            int64_t case_307 = 0;
                            if ((mak_local_bu_119 != NULL && (mak_local_bu_119)->tag == mak_ir_tag_1) && (mak_local_bu_119->v.v1.f0 == INT64_C(112))) {
                                struct mak__a * mak_local_bv_120 = (mak_local_bu_119)->v.v1.f1;
                                int64_t case_308 = 0;
                                struct mak__a *arena_call_310 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
                                if (!arena_call_310) {
                                    mak_ir_recover("ADT result allocation failed");
                                    arena_call_310 = &mak_ir_default_obj__a;
                                 } else {
                                    mak_ir_fn_2(mak_local_bv_120, (struct mak__a *)(arena_call_310));
                                }
                                struct mak__a * scrut_309 = arena_call_310;
                                if ((scrut_309 != NULL && (scrut_309)->tag == mak_ir_tag_0)) {
                                    case_308 = INT64_C(1);
                                }
                                else if (1) {
                                    case_308 = INT64_C(0);
                                }
                                case_307 = case_308;
                            }
                            else if (1) {
                                case_307 = INT64_C(0);
                            }
                            case_306 = case_307;
                        }
                        else if (1) {
                            case_306 = INT64_C(0);
                        }
                        case_305 = case_306;
                    }
                    else if (1) {
                        case_305 = INT64_C(0);
                    }
                    case_299 = case_305;
                }
                else if (1) {
                    case_299 = INT64_C(0);
                }
                int64_t scrut_298 = case_299;
                if ((scrut_298 == INT64_C(1))) {
                    mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)("\n=== MAK SCIENTIFIC CALCULATOR ===\n"), mak_ir_ext_3((const void *)("\n=== MAK SCIENTIFIC CALCULATOR ===\n")));
                    mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)("Numbers are f64 (double precision). Output uses up to 17 significant digits.\n"), mak_ir_ext_3((const void *)("Numbers are f64 (double precision). Output uses up to 17 significant digits.\n")));
                    mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)("Use radians for trig functions.\n\n"), mak_ir_ext_3((const void *)("Use radians for trig functions.\n\n")));
                    mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)("ARITHMETIC\n"), mak_ir_ext_3((const void *)("ARITHMETIC\n")));
                    mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)("  a+b       addition\n"), mak_ir_ext_3((const void *)("  a+b       addition\n")));
                    mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)("  a-b       subtraction\n"), mak_ir_ext_3((const void *)("  a-b       subtraction\n")));
                    mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)("  a*b       multiplication\n"), mak_ir_ext_3((const void *)("  a*b       multiplication\n")));
                    mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)("  a/b       REAL division\n"), mak_ir_ext_3((const void *)("  a/b       REAL division\n")));
                    mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)("  a//b      FLOOR / INTEGER-STYLE division\n"), mak_ir_ext_3((const void *)("  a//b      FLOOR / INTEGER-STYLE division\n")));
                    mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)("  a%b       remainder (fmod)\n"), mak_ir_ext_3((const void *)("  a%b       remainder (fmod)\n")));
                    mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)("  a^b       power\n"), mak_ir_ext_3((const void *)("  a^b       power\n")));
                    mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)("  2(3+4), 2pi  implicit multiplication\n"), mak_ir_ext_3((const void *)("  2(3+4), 2pi  implicit multiplication\n")));
                    mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)("  ( ... )   grouping\n"), mak_ir_ext_3((const void *)("  ( ... )   grouping\n")));
                    mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)("  +x  -x    unary plus/minus\n"), mak_ir_ext_3((const void *)("  +x  -x    unary plus/minus\n")));
                    mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)("  x!        factorial (gamma(x+1), including non-integer extension)\n"), mak_ir_ext_3((const void *)("  x!        factorial (gamma(x+1), including non-integer extension)\n")));
                    mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)("  x%        percentage when % is at the end of a value\n\n"), mak_ir_ext_3((const void *)("  x%        percentage when % is at the end of a value\n\n")));
                    mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)("CONSTANTS\n"), mak_ir_ext_3((const void *)("CONSTANTS\n")));
                    mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)("  pi        3.141592653589793...\n"), mak_ir_ext_3((const void *)("  pi        3.141592653589793...\n")));
                    mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)("  tau       2*pi\n"), mak_ir_ext_3((const void *)("  tau       2*pi\n")));
                    mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)("  e         Euler's number\n"), mak_ir_ext_3((const void *)("  e         Euler's number\n")));
                    mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)("  phi       golden ratio\n"), mak_ir_ext_3((const void *)("  phi       golden ratio\n")));
                    mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)("  sqrt2     square root of 2\n"), mak_ir_ext_3((const void *)("  sqrt2     square root of 2\n")));
                    mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)("  ln2       natural log of 2\n"), mak_ir_ext_3((const void *)("  ln2       natural log of 2\n")));
                    mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)("  ln10      natural log of 10\n"), mak_ir_ext_3((const void *)("  ln10      natural log of 10\n")));
                    mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)("  log2e     log base 2 of e\n"), mak_ir_ext_3((const void *)("  log2e     log base 2 of e\n")));
                    mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)("  log10e    log base 10 of e\n\n"), mak_ir_ext_3((const void *)("  log10e    log base 10 of e\n\n")));
                    mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)("BASIC FUNCTIONS\n"), mak_ir_ext_3((const void *)("BASIC FUNCTIONS\n")));
                    mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)("  abs(x) fabs(x)     absolute value\n"), mak_ir_ext_3((const void *)("  abs(x) fabs(x)     absolute value\n")));
                    mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)("  sqrt(x)            square root\n"), mak_ir_ext_3((const void *)("  sqrt(x)            square root\n")));
                    mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)("  cbrt(x)            real cube root\n"), mak_ir_ext_3((const void *)("  cbrt(x)            real cube root\n")));
                    mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)("  root(x,n)          nth root\n"), mak_ir_ext_3((const void *)("  root(x,n)          nth root\n")));
                    mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)("  nthroot(x,n)       alias of root\n"), mak_ir_ext_3((const void *)("  nthroot(x,n)       alias of root\n")));
                    mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)("  square(x)          x^2\n"), mak_ir_ext_3((const void *)("  square(x)          x^2\n")));
                    mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)("  cube(x)            x^3\n"), mak_ir_ext_3((const void *)("  cube(x)            x^3\n")));
                    mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)("  sign(x)            -1, 0, or +1\n"), mak_ir_ext_3((const void *)("  sign(x)            -1, 0, or +1\n")));
                    mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)("  frac(x)            fractional part, x-trunc(x)\n"), mak_ir_ext_3((const void *)("  frac(x)            fractional part, x-trunc(x)\n")));
                    mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)("  floor(x) ceil(x) trunc(x) round(x)\n"), mak_ir_ext_3((const void *)("  floor(x) ceil(x) trunc(x) round(x)\n")));
                    mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)("  min(a,b) max(a,b) clamp(x,lo,hi)\n"), mak_ir_ext_3((const void *)("  min(a,b) max(a,b) clamp(x,lo,hi)\n")));
                    mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)("  hypot(x,y)         sqrt(x^2+y^2)\n"), mak_ir_ext_3((const void *)("  hypot(x,y)         sqrt(x^2+y^2)\n")));
                    mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)("  copysign(a,b)       magnitude of a with sign of b\n"), mak_ir_ext_3((const void *)("  copysign(a,b)       magnitude of a with sign of b\n")));
                    mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)("  fmin fmax fdim      C floating-point min/max/positive-difference\n"), mak_ir_ext_3((const void *)("  fmin fmax fdim      C floating-point min/max/positive-difference\n")));
                    mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)("  fma(a,b,c)          correctly-rounded a*b+c (where supported)\n"), mak_ir_ext_3((const void *)("  fma(a,b,c)          correctly-rounded a*b+c (where supported)\n")));
                    mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)("  nextafter(a,b)      next representable double from a toward b\n\n"), mak_ir_ext_3((const void *)("  nextafter(a,b)      next representable double from a toward b\n\n")));
                    mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)("EXPONENTIALS / LOGS\n"), mak_ir_ext_3((const void *)("EXPONENTIALS / LOGS\n")));
                    mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)("  erf(x) erfc(x)      error function / complementary error function\n"), mak_ir_ext_3((const void *)("  erf(x) erfc(x)      error function / complementary error function\n")));
                    mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)("  logb(x)             exponent of x in binary scale\n"), mak_ir_ext_3((const void *)("  logb(x)             exponent of x in binary scale\n")));
                    mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)("  exp(x)              e^x\n"), mak_ir_ext_3((const void *)("  exp(x)              e^x\n")));
                    mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)("  exp2(x)            2^x\n"), mak_ir_ext_3((const void *)("  exp2(x)            2^x\n")));
                    mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)("  expm1(x)           e^x-1, accurate near zero\n"), mak_ir_ext_3((const void *)("  expm1(x)           e^x-1, accurate near zero\n")));
                    mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)("  log(x) ln(x)       natural logarithm\n"), mak_ir_ext_3((const void *)("  log(x) ln(x)       natural logarithm\n")));
                    mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)("  log2(x)             base-2 logarithm\n"), mak_ir_ext_3((const void *)("  log2(x)             base-2 logarithm\n")));
                    mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)("  log10(x)            base-10 logarithm\n"), mak_ir_ext_3((const void *)("  log10(x)            base-10 logarithm\n")));
                    mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)("  log1p(x)            ln(1+x), accurate near zero\n"), mak_ir_ext_3((const void *)("  log1p(x)            ln(1+x), accurate near zero\n")));
                    mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)("  pow(x,y)            x^y\n\n"), mak_ir_ext_3((const void *)("  pow(x,y)            x^y\n\n")));
                    mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)("TRIGONOMETRY \342\200\224 radians\n"), mak_ir_ext_3((const void *)("TRIGONOMETRY \342\200\224 radians\n")));
                    mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)("  sin cos tan\n"), mak_ir_ext_3((const void *)("  sin cos tan\n")));
                    mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)("  cot(x)=cos(x)/sin(x)\n"), mak_ir_ext_3((const void *)("  cot(x)=cos(x)/sin(x)\n")));
                    mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)("  sec(x)=1/cos(x)\n"), mak_ir_ext_3((const void *)("  sec(x)=1/cos(x)\n")));
                    mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)("  csc(x)=1/sin(x)\n"), mak_ir_ext_3((const void *)("  csc(x)=1/sin(x)\n")));
                    mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)("  asin acos atan      inverse trig, result in radians\n"), mak_ir_ext_3((const void *)("  asin acos atan      inverse trig, result in radians\n")));
                    mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)("  atan2(y,x)          quadrant-aware angle\n"), mak_ir_ext_3((const void *)("  atan2(y,x)          quadrant-aware angle\n")));
                    mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)("  acot asec acsc      inverse cot/sec/csc\n\n"), mak_ir_ext_3((const void *)("  acot asec acsc      inverse cot/sec/csc\n\n")));
                    mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)("HYPERBOLIC\n"), mak_ir_ext_3((const void *)("HYPERBOLIC\n")));
                    mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)("  sinh cosh tanh\n"), mak_ir_ext_3((const void *)("  sinh cosh tanh\n")));
                    mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)("  csch sech coth     reciprocal hyperbolic functions\n"), mak_ir_ext_3((const void *)("  csch sech coth     reciprocal hyperbolic functions\n")));
                    mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)("  asinh acosh atanh\n\n"), mak_ir_ext_3((const void *)("  asinh acosh atanh\n\n")));
                    mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)("ANGLE CONVERSION\n"), mak_ir_ext_3((const void *)("ANGLE CONVERSION\n")));
                    mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)("  deg(x)              radians -> degrees\n"), mak_ir_ext_3((const void *)("  deg(x)              radians -> degrees\n")));
                    mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)("  degrees(x)          alias\n"), mak_ir_ext_3((const void *)("  degrees(x)          alias\n")));
                    mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)("  rad(x)              degrees -> radians\n"), mak_ir_ext_3((const void *)("  rad(x)              degrees -> radians\n")));
                    mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)("  radians(x)          alias\n"), mak_ir_ext_3((const void *)("  radians(x)          alias\n")));
                    mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)("  Example: sin(rad(30)) = 0.5\n\n"), mak_ir_ext_3((const void *)("  Example: sin(rad(30)) = 0.5\n\n")));
                    mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)("NUMBER THEORY / COMBINATORICS\n"), mak_ir_ext_3((const void *)("NUMBER THEORY / COMBINATORICS\n")));
                    mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)("  gcd(a,b)             greatest common divisor\n"), mak_ir_ext_3((const void *)("  gcd(a,b)             greatest common divisor\n")));
                    mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)("  lcm(a,b)             least common multiple\n"), mak_ir_ext_3((const void *)("  lcm(a,b)             least common multiple\n")));
                    mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)("  ncr(n,r) / comb      binomial coefficient\n"), mak_ir_ext_3((const void *)("  ncr(n,r) / comb      binomial coefficient\n")));
                    mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)("  npr(n,r) / perm      permutations\n"), mak_ir_ext_3((const void *)("  npr(n,r) / perm      permutations\n")));
                    mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)("  factorial(n) / fact  factorial\n"), mak_ir_ext_3((const void *)("  factorial(n) / fact  factorial\n")));
                    mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)("  gamma(x)             Gamma(x)\n"), mak_ir_ext_3((const void *)("  gamma(x)             Gamma(x)\n")));
                    mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)("  lgamma(x)            ln(|Gamma(x)|)\n\n"), mak_ir_ext_3((const void *)("  lgamma(x)            ln(|Gamma(x)|)\n\n")));
                    mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)("REMAINDER FUNCTIONS\n"), mak_ir_ext_3((const void *)("REMAINDER FUNCTIONS\n")));
                    mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)("  fmod(a,b) mod(a,b) rem(a,b)  floating remainder\n"), mak_ir_ext_3((const void *)("  fmod(a,b) mod(a,b) rem(a,b)  floating remainder\n")));
                    mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)("  remainder(a,b)       IEEE-style remainder\n\n"), mak_ir_ext_3((const void *)("  remainder(a,b)       IEEE-style remainder\n\n")));
                    mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)("PRECEDENCE (strongest first)\n"), mak_ir_ext_3((const void *)("PRECEDENCE (strongest first)\n")));
                    mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)("  postfix !, final %\n"), mak_ir_ext_3((const void *)("  postfix !, final %\n")));
                    mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)("  power ^             right-associative\n"), mak_ir_ext_3((const void *)("  power ^             right-associative\n")));
                    mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)("  unary + -\n"), mak_ir_ext_3((const void *)("  unary + -\n")));
                    mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)("  * / // %\n"), mak_ir_ext_3((const void *)("  * / // %\n")));
                    mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)("  + -\n"), mak_ir_ext_3((const void *)("  + -\n")));
                    mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)("  Use parentheses whenever you want explicit grouping.\n\n"), mak_ir_ext_3((const void *)("  Use parentheses whenever you want explicit grouping.\n\n")));
                    mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)("EXAMPLES\n"), mak_ir_ext_3((const void *)("EXAMPLES\n")));
                    mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)("  22/7                    3.1428571428571428\n"), mak_ir_ext_3((const void *)("  22/7                    3.1428571428571428\n")));
                    mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)("  22//7                   3\n"), mak_ir_ext_3((const void *)("  22//7                   3\n")));
                    mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)("  2^3^2                   512\n"), mak_ir_ext_3((const void *)("  2^3^2                   512\n")));
                    mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)("  sqrt(2)^2               2\n"), mak_ir_ext_3((const void *)("  sqrt(2)^2               2\n")));
                    mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)("  sin(pi/2)               1\n"), mak_ir_ext_3((const void *)("  sin(pi/2)               1\n")));
                    mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)("  log10(1000)             3\n"), mak_ir_ext_3((const void *)("  log10(1000)             3\n")));
                    mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)("  ncr(52,5)               2598960\n"), mak_ir_ext_3((const void *)("  ncr(52,5)               2598960\n")));
                    mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)("  5!                      120\n\n"), mak_ir_ext_3((const void *)("  5!                      120\n\n")));
                    mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)("IMPORTANT\n"), mak_ir_ext_3((const void *)("IMPORTANT\n")));
                    mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)("  22/7  -> 3.1428571428571428...\n"), mak_ir_ext_3((const void *)("  22/7  -> 3.1428571428571428...\n")));
                    mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)("  22//7 -> 3\n"), mak_ir_ext_3((const void *)("  22//7 -> 3\n")));
                    mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)("  / never becomes integer division.\n"), mak_ir_ext_3((const void *)("  / never becomes integer division.\n")));
                    mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)("  // is floor division, so -7//2 = -4.\n"), mak_ir_ext_3((const void *)("  // is floor division, so -7//2 = -4.\n")));
                    mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)("  ^ accepts real exponents; e.g. 9^0.5 = 3.\n"), mak_ir_ext_3((const void *)("  ^ accepts real exponents; e.g. 9^0.5 = 3.\n")));
                    mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)("  Invalid domains may display nan/inf according to libm.\n"), mak_ir_ext_3((const void *)("  Invalid domains may display nan/inf according to libm.\n")));
                    mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)("  Type exit to leave the calculator.\n\n"), mak_ir_ext_3((const void *)("  Type exit to leave the calculator.\n\n")));
                    continue;
                }
                else if (1) {
                    int32_t discard_311 = 0;
                    struct mak__f *arena_call_313 = (struct mak__f *)mak_ir_arena_alloc(sizeof(struct mak__f), _Alignof(struct mak__f));
                    if (!arena_call_313) {
                        mak_ir_recover("ADT result allocation failed");
                        arena_call_313 = &mak_ir_default_obj__f;
                     } else {
                        mak_ir_fn_16(mak_local_ii_111, (struct mak__f *)(arena_call_313));
                    }
                    struct mak__f * scrut_312 = arena_call_313;
                    if ((scrut_312 != NULL && (scrut_312)->tag == mak_ir_tag_11)) {
                        mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)("Error: invalid expression\n"), mak_ir_ext_3((const void *)("Error: invalid expression\n")));
                    }
                    else if ((scrut_312 != NULL && (scrut_312)->tag == mak_ir_tag_10)) {
                        double mak_local_ig_121 = (scrut_312)->v.v10.f0;
                        struct mak__a * mak_local_ih_122 = (scrut_312)->v.v10.f1;
                        int32_t case_314 = 0;
                        struct mak__a *arena_call_316 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
                        if (!arena_call_316) {
                            mak_ir_recover("ADT result allocation failed");
                            arena_call_316 = &mak_ir_default_obj__a;
                         } else {
                            mak_ir_fn_2(mak_local_ih_122, (struct mak__a *)(arena_call_316));
                        }
                        struct mak__a * scrut_315 = arena_call_316;
                        if ((scrut_315 != NULL && (scrut_315)->tag == mak_ir_tag_0)) {
                            case_314 = mak_ir_ext_2((const void *)("%.17g\n"), mak_local_ig_121);
                        }
                        else if (1) {
                            mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)("Error: unexpected characters\n"), mak_ir_ext_3((const void *)("Error: unexpected characters\n")));
                        }
                        discard_311 = case_314;
                    }
                    else {
                        discard_311 = 0;
                    }
                    continue;
                }
            }
        }
        else {
            mak_ir_recover("case had no matching arm");
            return;
        }
    }
}

static inline int64_t mak_ir_fn_18(int64_t a0) {
    switch (a0) {
        case INT64_C(9): {
            return INT64_C(1);
        }
        case INT64_C(10): {
            return INT64_C(1);
        }
        case INT64_C(13): {
            return INT64_C(1);
        }
        case INT64_C(32): {
            return INT64_C(1);
        }
        default: {
            return INT64_C(0);
        }
    }
}

static inline int64_t mak_ir_fn_19(int64_t a0) {
    int64_t case_318 = 0;
    int32_t scrut_319 = (int32_t)(a0) == (int32_t)(INT64_C(95));
    if ((scrut_319 == INT64_C(1))) {
        case_318 = INT64_C(1);
    }
    else if (1) {
        int64_t case_320 = 0;
        int32_t scrut_321 = (int32_t)(a0) >= (int32_t)(INT64_C(65));
        if ((scrut_321 == INT64_C(0))) {
            int64_t case_322 = 0;
            int32_t scrut_323 = (int32_t)(a0) >= (int32_t)(INT64_C(97));
            if ((scrut_323 == INT64_C(0))) {
                case_322 = INT64_C(0);
            }
            else if (1) {
                int64_t case_324 = 0;
                int32_t scrut_325 = (int32_t)(a0) <= (int32_t)(INT64_C(122));
                if ((scrut_325 == INT64_C(1))) {
                    case_324 = INT64_C(1);
                }
                else if (1) {
                    case_324 = INT64_C(0);
                }
                case_322 = case_324;
            }
            case_320 = case_322;
        }
        else if (1) {
            int64_t case_326 = 0;
            int32_t scrut_327 = (int32_t)(a0) <= (int32_t)(INT64_C(90));
            if ((scrut_327 == INT64_C(1))) {
                case_326 = INT64_C(1);
            }
            else if (1) {
                case_326 = INT64_C(0);
            }
            case_320 = case_326;
        }
        case_318 = case_320;
    }
    int64_t scrut_317 = case_318;
    if ((scrut_317 == INT64_C(1))) {
        return INT64_C(1);
    }
    else if (1) {
        int32_t scrut_328 = (int32_t)(a0) >= (int32_t)(INT64_C(48));
        if ((scrut_328 == INT64_C(0))) {
            return INT64_C(0);
        }
        else if (1) {
            int32_t scrut_329 = (int32_t)(a0) <= (int32_t)(INT64_C(57));
            if ((scrut_329 == INT64_C(1))) {
                return INT64_C(1);
            }
            else if (1) {
                return INT64_C(0);
            }
        }
    }
}

static inline int64_t mak_ir_fn_20(int64_t a0) {
    int32_t scrut_330 = (int32_t)(a0) >= (int32_t)(INT64_C(65));
    if ((scrut_330 == INT64_C(0))) {
        return a0;
    }
    else if (1) {
        int32_t scrut_331 = (int32_t)(a0) <= (int32_t)(INT64_C(90));
        if ((scrut_331 == INT64_C(1))) {
            return mak_ir_i64_add_wrap(a0, INT64_C(32));
        }
        else if (1) {
            return a0;
        }
    }
}

static inline void mak_ir_fn_21(const char * a0, struct mak__f *out) {
    struct mak__f *arena_case_333 = (struct mak__f *)mak_ir_arena_alloc(sizeof(struct mak__f), _Alignof(struct mak__f));
    if (!arena_case_333) {
        mak_ir_recover("ADT case-result allocation failed");
        arena_case_333 = &mak_ir_default_obj__f;
     } else {
        struct mak__a *arena_call_335 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
        if (!arena_call_335) {
            mak_ir_recover("ADT result allocation failed");
            arena_call_335 = &mak_ir_default_obj__a;
         } else {
            mak_ir_fn_2((struct mak__a *)mak_ir_adt_recover_ptr((const void *)&mak_ir_default_obj__a, "string used as ADT"), (struct mak__a *)(arena_call_335));
        }
        struct mak__a * scrut_334 = arena_call_335;
        if ((scrut_334 != NULL && (scrut_334)->tag == mak_ir_tag_0)) {
            *(arena_case_333) = mak_ir_ctor_obj_11;
        }
        else if ((scrut_334 != NULL && (scrut_334)->tag == mak_ir_tag_1)) {
            int32_t mak_local_fs_123 = (scrut_334)->v.v1.f0;
            struct mak__a * mak_local_ft_124 = (scrut_334)->v.v1.f1;
            if ((mak_local_fs_123 == INT64_C(40))) {
                struct mak__f *arena_call_337 = (struct mak__f *)mak_ir_arena_alloc(sizeof(struct mak__f), _Alignof(struct mak__f));
                if (!arena_call_337) {
                    mak_ir_recover("ADT result allocation failed");
                    arena_call_337 = &mak_ir_default_obj__f;
                 } else {
                    mak_ir_fn_16(mak_local_ft_124, (struct mak__f *)(arena_call_337));
                }
                struct mak__f * scrut_336 = arena_call_337;
                if ((scrut_336 != NULL && (scrut_336)->tag == mak_ir_tag_11)) {
                    *(arena_case_333) = mak_ir_ctor_obj_11;
                }
                else if ((scrut_336 != NULL && (scrut_336)->tag == mak_ir_tag_10)) {
                    double mak_local_fu_125 = (scrut_336)->v.v10.f0;
                    struct mak__a * mak_local_fv_126 = (scrut_336)->v.v10.f1;
                    struct mak__a *arena_call_339 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
                    if (!arena_call_339) {
                        mak_ir_recover("ADT result allocation failed");
                        arena_call_339 = &mak_ir_default_obj__a;
                     } else {
                        mak_ir_fn_2(mak_local_fv_126, (struct mak__a *)(arena_call_339));
                    }
                    struct mak__a * scrut_338 = arena_call_339;
                    if ((scrut_338 != NULL && (scrut_338)->tag == mak_ir_tag_1) && (scrut_338->v.v1.f0 == INT64_C(41))) {
                        struct mak__a * mak_local_fw_127 = (scrut_338)->v.v1.f1;
                        (arena_case_333)->tag = mak_ir_tag_10;
                        (arena_case_333)->v.v10.f0 = mak_local_fu_125;
                        (arena_case_333)->v.v10.f1 = mak_local_fw_127;
                    }
                    else if (1) {
                        *(arena_case_333) = mak_ir_ctor_obj_11;
                    }
                }
                else {
                    *(arena_case_333) = *(&mak_ir_default_obj__f);
                }
            }
            else if (1) {
                int64_t case_341 = 0;
                int32_t scrut_342 = mak_local_fs_123 == (int32_t)(INT64_C(95));
                if ((scrut_342 == INT64_C(1))) {
                    case_341 = INT64_C(1);
                }
                else if (1) {
                    int64_t case_343 = 0;
                    int32_t scrut_344 = mak_local_fs_123 >= (int32_t)(INT64_C(65));
                    if ((scrut_344 == INT64_C(0))) {
                        int64_t case_345 = 0;
                        int32_t scrut_346 = mak_local_fs_123 >= (int32_t)(INT64_C(97));
                        if ((scrut_346 == INT64_C(0))) {
                            case_345 = INT64_C(0);
                        }
                        else if (1) {
                            int64_t case_347 = 0;
                            int32_t scrut_348 = mak_local_fs_123 <= (int32_t)(INT64_C(122));
                            if ((scrut_348 == INT64_C(1))) {
                                case_347 = INT64_C(1);
                            }
                            else if (1) {
                                case_347 = INT64_C(0);
                            }
                            case_345 = case_347;
                        }
                        case_343 = case_345;
                    }
                    else if (1) {
                        int64_t case_349 = 0;
                        int32_t scrut_350 = mak_local_fs_123 <= (int32_t)(INT64_C(90));
                        if ((scrut_350 == INT64_C(1))) {
                            case_349 = INT64_C(1);
                        }
                        else if (1) {
                            case_349 = INT64_C(0);
                        }
                        case_343 = case_349;
                    }
                    case_341 = case_343;
                }
                int64_t scrut_340 = case_341;
                if ((scrut_340 == INT64_C(1))) {
                    struct mak__f *arena_case_352 = (struct mak__f *)mak_ir_arena_alloc(sizeof(struct mak__f), _Alignof(struct mak__f));
                    if (!arena_case_352) {
                        mak_ir_recover("ADT case-result allocation failed");
                        arena_case_352 = &mak_ir_default_obj__f;
                     } else {
                        struct mak__a *arena_call_354 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
                        if (!arena_call_354) {
                            mak_ir_recover("ADT result allocation failed");
                            arena_call_354 = &mak_ir_default_obj__a;
                         } else {
                            struct mak__a *arena_obj_355 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
                            if (!arena_obj_355) {
                                mak_ir_recover("ADT object allocation failed");
                                arena_obj_355 = &mak_ir_default_obj__a;
                             } else {
                                (arena_obj_355)->tag = mak_ir_tag_1;
                                (arena_obj_355)->v.v1.f0 = mak_local_fs_123;
                                (arena_obj_355)->v.v1.f1 = mak_local_ft_124;
                            }
                            mak_ir_fn_2(arena_obj_355, (struct mak__a *)(arena_call_354));
                        }
                        struct mak__a * scrut_353 = arena_call_354;
                        if ((scrut_353 != NULL && (scrut_353)->tag == mak_ir_tag_1)) {
                            int32_t mak_local_ce_128 = (scrut_353)->v.v1.f0;
                            struct mak__a * mak_local_cf_129 = (scrut_353)->v.v1.f1;
                            int64_t case_357 = 0;
                            int32_t scrut_358 = mak_local_ce_128 == (int32_t)(INT64_C(95));
                            if ((scrut_358 == INT64_C(1))) {
                                case_357 = INT64_C(1);
                            }
                            else if (1) {
                                int64_t case_359 = 0;
                                int32_t scrut_360 = mak_local_ce_128 >= (int32_t)(INT64_C(65));
                                if ((scrut_360 == INT64_C(0))) {
                                    int64_t case_361 = 0;
                                    int32_t scrut_362 = mak_local_ce_128 >= (int32_t)(INT64_C(97));
                                    if ((scrut_362 == INT64_C(0))) {
                                        case_361 = INT64_C(0);
                                    }
                                    else if (1) {
                                        int64_t case_363 = 0;
                                        int32_t scrut_364 = mak_local_ce_128 <= (int32_t)(INT64_C(122));
                                        if ((scrut_364 == INT64_C(1))) {
                                            case_363 = INT64_C(1);
                                        }
                                        else if (1) {
                                            case_363 = INT64_C(0);
                                        }
                                        case_361 = case_363;
                                    }
                                    case_359 = case_361;
                                }
                                else if (1) {
                                    int64_t case_365 = 0;
                                    int32_t scrut_366 = mak_local_ce_128 <= (int32_t)(INT64_C(90));
                                    if ((scrut_366 == INT64_C(1))) {
                                        case_365 = INT64_C(1);
                                    }
                                    else if (1) {
                                        case_365 = INT64_C(0);
                                    }
                                    case_359 = case_365;
                                }
                                case_357 = case_359;
                            }
                            int64_t scrut_356 = case_357;
                            if ((scrut_356 == INT64_C(1))) {
                                struct mak__c *arena_call_367 = (struct mak__c *)mak_ir_arena_alloc(sizeof(struct mak__c), _Alignof(struct mak__c));
                                if (!arena_call_367) {
                                    mak_ir_recover("ADT result allocation failed");
                                    arena_call_367 = &mak_ir_default_obj__c;
                                 } else {
                                    struct mak__a *arena_obj_368 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
                                    if (!arena_obj_368) {
                                        mak_ir_recover("ADT object allocation failed");
                                        arena_obj_368 = &mak_ir_default_obj__a;
                                     } else {
                                        (arena_obj_368)->tag = mak_ir_tag_1;
                                        (arena_obj_368)->v.v1.f0 = mak_local_ce_128;
                                        (arena_obj_368)->v.v1.f1 = mak_local_cf_129;
                                    }
                                    mak_ir_fn_3(arena_obj_368, INT64_C(0), (struct mak__c *)(arena_call_367));
                                }
                                if (!mak_ir_adt_copy_checked((void *)(arena_case_352), sizeof(struct mak__f), UINT32_C(1225484184), (const void *)(uintptr_t)(arena_call_367))) {
                                    *(arena_case_352) = *(&mak_ir_default_obj__f);
                                }
                            }
                            else if (1) {
                                struct mak__f *rt_heap_obj_369 = (struct mak__f *)(uintptr_t)((struct mak__f *)(uintptr_t)(mak_rt_call_ctor("NBad:_f", (const uint64_t[]){}, (const uint8_t[]){}, 0)));
                                if (!mak_ir_adt_copy_checked((void *)(arena_case_352), sizeof(struct mak__f), UINT32_C(1225484184), (const void *)(rt_heap_obj_369)) ) {
                                    *(arena_case_352) = *(&mak_ir_default_obj__f);
                                }
                            }
                        }
                        else if ((scrut_353 != NULL && (scrut_353)->tag == mak_ir_tag_0)) {
                            struct mak__f *rt_heap_obj_370 = (struct mak__f *)(uintptr_t)((struct mak__f *)(uintptr_t)(mak_rt_call_ctor("NBad:_f", (const uint64_t[]){}, (const uint8_t[]){}, 0)));
                            if (!mak_ir_adt_copy_checked((void *)(arena_case_352), sizeof(struct mak__f), UINT32_C(1225484184), (const void *)(rt_heap_obj_370)) ) {
                                *(arena_case_352) = *(&mak_ir_default_obj__f);
                            }
                        }
                        else {
                            *(arena_case_352) = *(&mak_ir_default_obj__f);
                        }
                    }
                    struct mak__f * scrut_351 = arena_case_352;
                    if ((scrut_351 != NULL && (scrut_351)->tag == mak_ir_tag_11)) {
                        *(arena_case_333) = mak_ir_ctor_obj_11;
                    }
                    else if ((scrut_351 != NULL && (scrut_351)->tag == mak_ir_tag_10)) {
                        double mak_local_fx_130 = (scrut_351)->v.v10.f0;
                        struct mak__a * mak_local_fy_131 = (scrut_351)->v.v10.f1;
                        struct mak__a *arena_call_372 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
                        if (!arena_call_372) {
                            mak_ir_recover("ADT result allocation failed");
                            arena_call_372 = &mak_ir_default_obj__a;
                         } else {
                            mak_ir_fn_2(mak_local_fy_131, (struct mak__a *)(arena_call_372));
                        }
                        struct mak__a * scrut_371 = arena_call_372;
                        if ((scrut_371 != NULL && (scrut_371)->tag == mak_ir_tag_1) && (scrut_371->v.v1.f0 == INT64_C(40))) {
                            struct mak__a * mak_local_fz_132 = (scrut_371)->v.v1.f1;
                            *(arena_case_333) = mak_ir_ctor_obj_11;
                        }
                        else if (1) {
                            *(arena_case_333) = mak_ir_ctor_obj_11;
                        }
                    }
                    else {
                        *(arena_case_333) = *(&mak_ir_default_obj__f);
                    }
                }
                else if (1) {
                    struct mak__f *arena_case_374 = (struct mak__f *)mak_ir_arena_alloc(sizeof(struct mak__f), _Alignof(struct mak__f));
                    if (!arena_case_374) {
                        mak_ir_recover("ADT case-result allocation failed");
                        arena_case_374 = &mak_ir_default_obj__f;
                     } else {
                        struct mak__a *arena_call_376 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
                        if (!arena_call_376) {
                            mak_ir_recover("ADT result allocation failed");
                            arena_call_376 = &mak_ir_default_obj__a;
                         } else {
                            mak_ir_fn_2((struct mak__a *)mak_ir_adt_recover_ptr((const void *)&mak_ir_default_obj__a, "string used as ADT"), (struct mak__a *)(arena_call_376));
                        }
                        struct mak__a * scrut_375 = arena_call_376;
                        if ((scrut_375 != NULL && (scrut_375)->tag == mak_ir_tag_0)) {
                            struct mak__f *rt_heap_obj_377 = (struct mak__f *)(uintptr_t)((struct mak__f *)(uintptr_t)(mak_rt_call_ctor("NBad:_f", (const uint64_t[]){}, (const uint8_t[]){}, 0)));
                            if (!mak_ir_adt_copy_checked((void *)(arena_case_374), sizeof(struct mak__f), UINT32_C(1225484184), (const void *)(rt_heap_obj_377)) ) {
                                *(arena_case_374) = *(&mak_ir_default_obj__f);
                            }
                        }
                        else if ((scrut_375 != NULL && (scrut_375)->tag == mak_ir_tag_1)) {
                            int32_t mak_local_dk_151 = (scrut_375)->v.v1.f0;
                            struct mak__a * mak_local_dl_152 = (scrut_375)->v.v1.f1;
                            int64_t case_379 = 0;
                            int32_t scrut_380 = mak_local_dk_151 >= (int32_t)(INT64_C(48));
                            if ((scrut_380 == INT64_C(0))) {
                                case_379 = INT64_C(0);
                            }
                            else if (1) {
                                int64_t case_381 = 0;
                                int32_t scrut_382 = mak_local_dk_151 <= (int32_t)(INT64_C(57));
                                if ((scrut_382 == INT64_C(1))) {
                                    case_381 = INT64_C(1);
                                }
                                else if (1) {
                                    case_381 = INT64_C(0);
                                }
                                case_379 = case_381;
                            }
                            int64_t scrut_378 = case_379;
                            if ((scrut_378 == INT64_C(1))) {
                                struct mak__d *arena_call_384 = (struct mak__d *)mak_ir_arena_alloc(sizeof(struct mak__d), _Alignof(struct mak__d));
                                if (!arena_call_384) {
                                    mak_ir_recover("ADT result allocation failed");
                                    arena_call_384 = &mak_ir_default_obj__d;
                                 } else {
                                    struct mak__a *arena_obj_385 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
                                    if (!arena_obj_385) {
                                        mak_ir_recover("ADT object allocation failed");
                                        arena_obj_385 = &mak_ir_default_obj__a;
                                     } else {
                                        (arena_obj_385)->tag = mak_ir_tag_1;
                                        (arena_obj_385)->v.v1.f0 = mak_local_dk_151;
                                        (arena_obj_385)->v.v1.f1 = mak_local_dl_152;
                                    }
                                    mak_ir_fn_7(arena_obj_385, 0.00000000000000000, (struct mak__d *)(arena_call_384));
                                }
                                struct mak__d * scrut_383 = arena_call_384;
                                if ((scrut_383 != NULL && (scrut_383)->tag == mak_ir_tag_6)) {
                                    struct mak__f *rt_heap_obj_386 = (struct mak__f *)(uintptr_t)((struct mak__f *)(uintptr_t)(mak_rt_call_ctor("NBad:_f", (const uint64_t[]){}, (const uint8_t[]){}, 0)));
                                    if (!mak_ir_adt_copy_checked((void *)(arena_case_374), sizeof(struct mak__f), UINT32_C(1225484184), (const void *)(rt_heap_obj_386)) ) {
                                        *(arena_case_374) = *(&mak_ir_default_obj__f);
                                    }
                                }
                                else if ((scrut_383 != NULL && (scrut_383)->tag == mak_ir_tag_7)) {
                                    double mak_local_dm_153 = (scrut_383)->v.v7.f0;
                                    struct mak__a * mak_local_dn_154 = (scrut_383)->v.v7.f1;
                                    if ((mak_local_dn_154 != NULL && (mak_local_dn_154)->tag == mak_ir_tag_1) && (mak_local_dn_154->v.v1.f0 == INT64_C(46))) {
                                        struct mak__a * mak_local_dp_155 = (mak_local_dn_154)->v.v1.f1;
                                        struct mak__d *arena_call_387 = (struct mak__d *)mak_ir_arena_alloc(sizeof(struct mak__d), _Alignof(struct mak__d));
                                        if (!arena_call_387) {
                                            mak_ir_recover("ADT result allocation failed");
                                            arena_call_387 = &mak_ir_default_obj__d;
                                         } else {
                                            mak_ir_fn_8(mak_local_dp_155, mak_local_dm_153, 0.10000000000000001, (struct mak__d *)(arena_call_387));
                                        }
                                        if (!mak_ir_adt_copy_checked((void *)(arena_case_374), sizeof(struct mak__f), UINT32_C(1225484184), (const void *)(uintptr_t)(arena_call_387))) {
                                            *(arena_case_374) = *(&mak_ir_default_obj__f);
                                        }
                                    }
                                    else if ((mak_local_dn_154 != NULL && (mak_local_dn_154)->tag == mak_ir_tag_1)) {
                                        int32_t mak_local_dq_156 = (mak_local_dn_154)->v.v1.f0;
                                        struct mak__a * mak_local_dr_157 = (mak_local_dn_154)->v.v1.f1;
                                        switch (mak_local_dq_156) {
                                            case INT64_C(101): {
                                                struct mak__e *arena_call_389 = (struct mak__e *)mak_ir_arena_alloc(sizeof(struct mak__e), _Alignof(struct mak__e));
                                                if (!arena_call_389) {
                                                    mak_ir_recover("ADT result allocation failed");
                                                    arena_call_389 = &mak_ir_default_obj__e;
                                                 } else {
                                                    mak_ir_fn_6(mak_local_dr_157, INT64_C(1), (struct mak__e *)(arena_call_389));
                                                }
                                                struct mak__e * scrut_388 = arena_call_389;
                                                if ((scrut_388 != NULL && (scrut_388)->tag == mak_ir_tag_8)) {
                                                    struct mak__f *rt_heap_obj_390 = (struct mak__f *)(uintptr_t)((struct mak__f *)(uintptr_t)(mak_rt_call_ctor("NBad:_f", (const uint64_t[]){}, (const uint8_t[]){}, 0)));
                                                    if (!mak_ir_adt_copy_checked((void *)(arena_case_374), sizeof(struct mak__f), UINT32_C(1225484184), (const void *)(rt_heap_obj_390)) ) {
                                                        *(arena_case_374) = *(&mak_ir_default_obj__f);
                                                    }
                                                }
                                                else if ((scrut_388 != NULL && (scrut_388)->tag == mak_ir_tag_9)) {
                                                    int64_t mak_local_ds_158 = (scrut_388)->v.v9.f0;
                                                    struct mak__a * mak_local_dt_159 = (scrut_388)->v.v9.f1;
                                                    struct mak__d *arena_obj_391 = (struct mak__d *)mak_ir_arena_alloc(sizeof(struct mak__d), _Alignof(struct mak__d));
                                                    if (!arena_obj_391) {
                                                        mak_ir_recover("ADT object allocation failed");
                                                        arena_obj_391 = &mak_ir_default_obj__d;
                                                     } else {
                                                        (arena_obj_391)->tag = mak_ir_tag_7;
                                                        double eval_392 = mak_ir_fn_4(mak_local_ds_158);
                                                        (arena_obj_391)->v.v7.f0 = ((double)(mak_local_dm_153) * (double)(eval_392));
                                                        (arena_obj_391)->v.v7.f1 = mak_local_dt_159;
                                                    }
                                                    if (!mak_ir_adt_copy_checked((void *)(arena_case_374), sizeof(struct mak__f), UINT32_C(1225484184), (const void *)(uintptr_t)(arena_obj_391))) {
                                                        *(arena_case_374) = *(&mak_ir_default_obj__f);
                                                    }
                                                }
                                                else {
                                                    *(arena_case_374) = *(&mak_ir_default_obj__f);
                                                }
                                                break;
                                            }
                                            case INT64_C(69): {
                                                struct mak__e *arena_call_394 = (struct mak__e *)mak_ir_arena_alloc(sizeof(struct mak__e), _Alignof(struct mak__e));
                                                if (!arena_call_394) {
                                                    mak_ir_recover("ADT result allocation failed");
                                                    arena_call_394 = &mak_ir_default_obj__e;
                                                 } else {
                                                    mak_ir_fn_6(mak_local_dr_157, INT64_C(1), (struct mak__e *)(arena_call_394));
                                                }
                                                struct mak__e * scrut_393 = arena_call_394;
                                                if ((scrut_393 != NULL && (scrut_393)->tag == mak_ir_tag_8)) {
                                                    struct mak__f *rt_heap_obj_395 = (struct mak__f *)(uintptr_t)((struct mak__f *)(uintptr_t)(mak_rt_call_ctor("NBad:_f", (const uint64_t[]){}, (const uint8_t[]){}, 0)));
                                                    if (!mak_ir_adt_copy_checked((void *)(arena_case_374), sizeof(struct mak__f), UINT32_C(1225484184), (const void *)(rt_heap_obj_395)) ) {
                                                        *(arena_case_374) = *(&mak_ir_default_obj__f);
                                                    }
                                                }
                                                else if ((scrut_393 != NULL && (scrut_393)->tag == mak_ir_tag_9)) {
                                                    int64_t mak_local_du_160 = (scrut_393)->v.v9.f0;
                                                    struct mak__a * mak_local_dv_161 = (scrut_393)->v.v9.f1;
                                                    struct mak__d *arena_obj_396 = (struct mak__d *)mak_ir_arena_alloc(sizeof(struct mak__d), _Alignof(struct mak__d));
                                                    if (!arena_obj_396) {
                                                        mak_ir_recover("ADT object allocation failed");
                                                        arena_obj_396 = &mak_ir_default_obj__d;
                                                     } else {
                                                        (arena_obj_396)->tag = mak_ir_tag_7;
                                                        double eval_397 = mak_ir_fn_4(mak_local_du_160);
                                                        (arena_obj_396)->v.v7.f0 = ((double)(mak_local_dm_153) * (double)(eval_397));
                                                        (arena_obj_396)->v.v7.f1 = mak_local_dv_161;
                                                    }
                                                    if (!mak_ir_adt_copy_checked((void *)(arena_case_374), sizeof(struct mak__f), UINT32_C(1225484184), (const void *)(uintptr_t)(arena_obj_396))) {
                                                        *(arena_case_374) = *(&mak_ir_default_obj__f);
                                                    }
                                                }
                                                else {
                                                    *(arena_case_374) = *(&mak_ir_default_obj__f);
                                                }
                                                break;
                                            }
                                            default: {
                                                struct mak__d *arena_obj_398 = (struct mak__d *)mak_ir_arena_alloc(sizeof(struct mak__d), _Alignof(struct mak__d));
                                                if (!arena_obj_398) {
                                                    mak_ir_recover("ADT object allocation failed");
                                                    arena_obj_398 = &mak_ir_default_obj__d;
                                                 } else {
                                                    (arena_obj_398)->tag = mak_ir_tag_7;
                                                    (arena_obj_398)->v.v7.f0 = mak_local_dm_153;
                                                    (arena_obj_398)->v.v7.f1 = mak_local_dn_154;
                                                }
                                                if (!mak_ir_adt_copy_checked((void *)(arena_case_374), sizeof(struct mak__f), UINT32_C(1225484184), (const void *)(uintptr_t)(arena_obj_398))) {
                                                    *(arena_case_374) = *(&mak_ir_default_obj__f);
                                                }
                                                break;
                                            }
                                        }
                                    }
                                    else if (1) {
                                        struct mak__d *arena_obj_399 = (struct mak__d *)mak_ir_arena_alloc(sizeof(struct mak__d), _Alignof(struct mak__d));
                                        if (!arena_obj_399) {
                                            mak_ir_recover("ADT object allocation failed");
                                            arena_obj_399 = &mak_ir_default_obj__d;
                                         } else {
                                            (arena_obj_399)->tag = mak_ir_tag_7;
                                            (arena_obj_399)->v.v7.f0 = mak_local_dm_153;
                                            (arena_obj_399)->v.v7.f1 = mak_local_dn_154;
                                        }
                                        if (!mak_ir_adt_copy_checked((void *)(arena_case_374), sizeof(struct mak__f), UINT32_C(1225484184), (const void *)(uintptr_t)(arena_obj_399))) {
                                            *(arena_case_374) = *(&mak_ir_default_obj__f);
                                        }
                                    }
                                }
                                else {
                                    *(arena_case_374) = *(&mak_ir_default_obj__f);
                                }
                            }
                            else if (1) {
                                if ((mak_local_dk_151 == INT64_C(46))) {
                                    if ((mak_local_dl_152 != NULL && (mak_local_dl_152)->tag == mak_ir_tag_1)) {
                                        int32_t mak_local_dw_162 = (mak_local_dl_152)->v.v1.f0;
                                        int64_t case_401 = 0;
                                        int32_t scrut_402 = mak_local_dw_162 >= (int32_t)(INT64_C(48));
                                        if ((scrut_402 == INT64_C(0))) {
                                            case_401 = INT64_C(0);
                                        }
                                        else if (1) {
                                            int64_t case_403 = 0;
                                            int32_t scrut_404 = mak_local_dw_162 <= (int32_t)(INT64_C(57));
                                            if ((scrut_404 == INT64_C(1))) {
                                                case_403 = INT64_C(1);
                                            }
                                            else if (1) {
                                                case_403 = INT64_C(0);
                                            }
                                            case_401 = case_403;
                                        }
                                        int64_t scrut_400 = case_401;
                                        if ((scrut_400 == INT64_C(1))) {
                                            struct mak__d *arena_call_405 = (struct mak__d *)mak_ir_arena_alloc(sizeof(struct mak__d), _Alignof(struct mak__d));
                                            if (!arena_call_405) {
                                                mak_ir_recover("ADT result allocation failed");
                                                arena_call_405 = &mak_ir_default_obj__d;
                                             } else {
                                                mak_ir_fn_8(mak_local_dl_152, 0.00000000000000000, 0.10000000000000001, (struct mak__d *)(arena_call_405));
                                            }
                                            if (!mak_ir_adt_copy_checked((void *)(arena_case_374), sizeof(struct mak__f), UINT32_C(1225484184), (const void *)(uintptr_t)(arena_call_405))) {
                                                *(arena_case_374) = *(&mak_ir_default_obj__f);
                                            }
                                        }
                                        else if (1) {
                                            struct mak__f *rt_heap_obj_406 = (struct mak__f *)(uintptr_t)((struct mak__f *)(uintptr_t)(mak_rt_call_ctor("NBad:_f", (const uint64_t[]){}, (const uint8_t[]){}, 0)));
                                            if (!mak_ir_adt_copy_checked((void *)(arena_case_374), sizeof(struct mak__f), UINT32_C(1225484184), (const void *)(rt_heap_obj_406)) ) {
                                                *(arena_case_374) = *(&mak_ir_default_obj__f);
                                            }
                                        }
                                    }
                                    else if (1) {
                                        struct mak__f *rt_heap_obj_407 = (struct mak__f *)(uintptr_t)((struct mak__f *)(uintptr_t)(mak_rt_call_ctor("NBad:_f", (const uint64_t[]){}, (const uint8_t[]){}, 0)));
                                        if (!mak_ir_adt_copy_checked((void *)(arena_case_374), sizeof(struct mak__f), UINT32_C(1225484184), (const void *)(rt_heap_obj_407)) ) {
                                            *(arena_case_374) = *(&mak_ir_default_obj__f);
                                        }
                                    }
                                }
                                else if (1) {
                                    struct mak__f *rt_heap_obj_408 = (struct mak__f *)(uintptr_t)((struct mak__f *)(uintptr_t)(mak_rt_call_ctor("NBad:_f", (const uint64_t[]){}, (const uint8_t[]){}, 0)));
                                    if (!mak_ir_adt_copy_checked((void *)(arena_case_374), sizeof(struct mak__f), UINT32_C(1225484184), (const void *)(rt_heap_obj_408)) ) {
                                        *(arena_case_374) = *(&mak_ir_default_obj__f);
                                    }
                                }
                            }
                        }
                        else {
                            *(arena_case_374) = *(&mak_ir_default_obj__f);
                        }
                    }
                    struct mak__f * scrut_373 = arena_case_374;
                    if ((scrut_373 != NULL && (scrut_373)->tag == mak_ir_tag_11)) {
                        *(arena_case_333) = mak_ir_ctor_obj_11;
                    }
                    else if ((scrut_373 != NULL && (scrut_373)->tag == mak_ir_tag_10)) {
                        double mak_local_ga_163 = (scrut_373)->v.v10.f0;
                        struct mak__a * mak_local_gb_164 = (scrut_373)->v.v10.f1;
                        (arena_case_333)->tag = mak_ir_tag_10;
                        (arena_case_333)->v.v10.f0 = mak_local_ga_163;
                        (arena_case_333)->v.v10.f1 = mak_local_gb_164;
                    }
                    else {
                        *(arena_case_333) = *(&mak_ir_default_obj__f);
                    }
                }
            }
        }
        else {
            *(arena_case_333) = *(&mak_ir_default_obj__f);
        }
    }
    struct mak__f * scrut_332 = arena_case_333;
    if ((scrut_332 != NULL && (scrut_332)->tag == mak_ir_tag_11)) {
        *(out) = mak_ir_ctor_obj_11;
    }
    else if ((scrut_332 != NULL && (scrut_332)->tag == mak_ir_tag_10)) {
        double mak_local_gi_165 = (scrut_332)->v.v10.f0;
        struct mak__a * mak_local_gj_166 = (scrut_332)->v.v10.f1;
        mak_ir_fn_10(mak_local_gi_165, mak_local_gj_166, (struct mak__f *)(out));
    }
    else {
        *(out) = *(&mak_ir_default_obj__f);
    }
    return;
}

static inline void mak_ir_fn_22(struct mak__a * a0, struct mak__f *out) {
    struct mak__a *arena_call_410 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
    if (!arena_call_410) {
        mak_ir_recover("ADT result allocation failed");
        arena_call_410 = &mak_ir_default_obj__a;
     } else {
        mak_ir_fn_2(a0, (struct mak__a *)(arena_call_410));
    }
    struct mak__a * scrut_409 = arena_call_410;
    if ((scrut_409 != NULL && (scrut_409)->tag == mak_ir_tag_0)) {
        *(out) = mak_ir_ctor_obj_11;
    }
    else if ((scrut_409 != NULL && (scrut_409)->tag == mak_ir_tag_1)) {
        int32_t mak_local_fs_167 = (scrut_409)->v.v1.f0;
        struct mak__a * mak_local_ft_168 = (scrut_409)->v.v1.f1;
        if ((mak_local_fs_167 == INT64_C(40))) {
            struct mak__f *arena_call_412 = (struct mak__f *)mak_ir_arena_alloc(sizeof(struct mak__f), _Alignof(struct mak__f));
            if (!arena_call_412) {
                mak_ir_recover("ADT result allocation failed");
                arena_call_412 = &mak_ir_default_obj__f;
             } else {
                mak_ir_fn_16(mak_local_ft_168, (struct mak__f *)(arena_call_412));
            }
            struct mak__f * scrut_411 = arena_call_412;
            if ((scrut_411 != NULL && (scrut_411)->tag == mak_ir_tag_11)) {
                *(out) = mak_ir_ctor_obj_11;
            }
            else if ((scrut_411 != NULL && (scrut_411)->tag == mak_ir_tag_10)) {
                double mak_local_fu_169 = (scrut_411)->v.v10.f0;
                struct mak__a * mak_local_fv_170 = (scrut_411)->v.v10.f1;
                struct mak__a *arena_call_414 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
                if (!arena_call_414) {
                    mak_ir_recover("ADT result allocation failed");
                    arena_call_414 = &mak_ir_default_obj__a;
                 } else {
                    mak_ir_fn_2(mak_local_fv_170, (struct mak__a *)(arena_call_414));
                }
                struct mak__a * scrut_413 = arena_call_414;
                if ((scrut_413 != NULL && (scrut_413)->tag == mak_ir_tag_1) && (scrut_413->v.v1.f0 == INT64_C(41))) {
                    struct mak__a * mak_local_fw_171 = (scrut_413)->v.v1.f1;
                    (out)->tag = mak_ir_tag_10;
                    (out)->v.v10.f0 = mak_local_fu_169;
                    (out)->v.v10.f1 = mak_local_fw_171;
                }
                else if (1) {
                    *(out) = mak_ir_ctor_obj_11;
                }
            }
            else {
                *(out) = *(&mak_ir_default_obj__f);
            }
        }
        else if (1) {
            int64_t case_416 = 0;
            int32_t scrut_417 = mak_local_fs_167 == (int32_t)(INT64_C(95));
            if ((scrut_417 == INT64_C(1))) {
                case_416 = INT64_C(1);
            }
            else if (1) {
                int64_t case_418 = 0;
                int32_t scrut_419 = mak_local_fs_167 >= (int32_t)(INT64_C(65));
                if ((scrut_419 == INT64_C(0))) {
                    int64_t case_420 = 0;
                    int32_t scrut_421 = mak_local_fs_167 >= (int32_t)(INT64_C(97));
                    if ((scrut_421 == INT64_C(0))) {
                        case_420 = INT64_C(0);
                    }
                    else if (1) {
                        int64_t case_422 = 0;
                        int32_t scrut_423 = mak_local_fs_167 <= (int32_t)(INT64_C(122));
                        if ((scrut_423 == INT64_C(1))) {
                            case_422 = INT64_C(1);
                        }
                        else if (1) {
                            case_422 = INT64_C(0);
                        }
                        case_420 = case_422;
                    }
                    case_418 = case_420;
                }
                else if (1) {
                    int64_t case_424 = 0;
                    int32_t scrut_425 = mak_local_fs_167 <= (int32_t)(INT64_C(90));
                    if ((scrut_425 == INT64_C(1))) {
                        case_424 = INT64_C(1);
                    }
                    else if (1) {
                        case_424 = INT64_C(0);
                    }
                    case_418 = case_424;
                }
                case_416 = case_418;
            }
            int64_t scrut_415 = case_416;
            if ((scrut_415 == INT64_C(1))) {
                struct mak__f *arena_case_427 = (struct mak__f *)mak_ir_arena_alloc(sizeof(struct mak__f), _Alignof(struct mak__f));
                if (!arena_case_427) {
                    mak_ir_recover("ADT case-result allocation failed");
                    arena_case_427 = &mak_ir_default_obj__f;
                 } else {
                    struct mak__a *arena_call_429 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
                    if (!arena_call_429) {
                        mak_ir_recover("ADT result allocation failed");
                        arena_call_429 = &mak_ir_default_obj__a;
                     } else {
                        struct mak__a *arena_obj_430 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
                        if (!arena_obj_430) {
                            mak_ir_recover("ADT object allocation failed");
                            arena_obj_430 = &mak_ir_default_obj__a;
                         } else {
                            (arena_obj_430)->tag = mak_ir_tag_1;
                            (arena_obj_430)->v.v1.f0 = mak_local_fs_167;
                            (arena_obj_430)->v.v1.f1 = mak_local_ft_168;
                        }
                        mak_ir_fn_2(arena_obj_430, (struct mak__a *)(arena_call_429));
                    }
                    struct mak__a * scrut_428 = arena_call_429;
                    if ((scrut_428 != NULL && (scrut_428)->tag == mak_ir_tag_1)) {
                        int32_t mak_local_ce_172 = (scrut_428)->v.v1.f0;
                        struct mak__a * mak_local_cf_173 = (scrut_428)->v.v1.f1;
                        int64_t case_432 = 0;
                        int32_t scrut_433 = mak_local_ce_172 == (int32_t)(INT64_C(95));
                        if ((scrut_433 == INT64_C(1))) {
                            case_432 = INT64_C(1);
                        }
                        else if (1) {
                            int64_t case_434 = 0;
                            int32_t scrut_435 = mak_local_ce_172 >= (int32_t)(INT64_C(65));
                            if ((scrut_435 == INT64_C(0))) {
                                int64_t case_436 = 0;
                                int32_t scrut_437 = mak_local_ce_172 >= (int32_t)(INT64_C(97));
                                if ((scrut_437 == INT64_C(0))) {
                                    case_436 = INT64_C(0);
                                }
                                else if (1) {
                                    int64_t case_438 = 0;
                                    int32_t scrut_439 = mak_local_ce_172 <= (int32_t)(INT64_C(122));
                                    if ((scrut_439 == INT64_C(1))) {
                                        case_438 = INT64_C(1);
                                    }
                                    else if (1) {
                                        case_438 = INT64_C(0);
                                    }
                                    case_436 = case_438;
                                }
                                case_434 = case_436;
                            }
                            else if (1) {
                                int64_t case_440 = 0;
                                int32_t scrut_441 = mak_local_ce_172 <= (int32_t)(INT64_C(90));
                                if ((scrut_441 == INT64_C(1))) {
                                    case_440 = INT64_C(1);
                                }
                                else if (1) {
                                    case_440 = INT64_C(0);
                                }
                                case_434 = case_440;
                            }
                            case_432 = case_434;
                        }
                        int64_t scrut_431 = case_432;
                        if ((scrut_431 == INT64_C(1))) {
                            struct mak__c *arena_call_442 = (struct mak__c *)mak_ir_arena_alloc(sizeof(struct mak__c), _Alignof(struct mak__c));
                            if (!arena_call_442) {
                                mak_ir_recover("ADT result allocation failed");
                                arena_call_442 = &mak_ir_default_obj__c;
                             } else {
                                struct mak__a *arena_obj_443 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
                                if (!arena_obj_443) {
                                    mak_ir_recover("ADT object allocation failed");
                                    arena_obj_443 = &mak_ir_default_obj__a;
                                 } else {
                                    (arena_obj_443)->tag = mak_ir_tag_1;
                                    (arena_obj_443)->v.v1.f0 = mak_local_ce_172;
                                    (arena_obj_443)->v.v1.f1 = mak_local_cf_173;
                                }
                                mak_ir_fn_3(arena_obj_443, INT64_C(0), (struct mak__c *)(arena_call_442));
                            }
                            if (!mak_ir_adt_copy_checked((void *)(arena_case_427), sizeof(struct mak__f), UINT32_C(1225484184), (const void *)(uintptr_t)(arena_call_442))) {
                                *(arena_case_427) = *(&mak_ir_default_obj__f);
                            }
                        }
                        else if (1) {
                            struct mak__f *rt_heap_obj_444 = (struct mak__f *)(uintptr_t)((struct mak__f *)(uintptr_t)(mak_rt_call_ctor("NBad:_f", (const uint64_t[]){}, (const uint8_t[]){}, 0)));
                            if (!mak_ir_adt_copy_checked((void *)(arena_case_427), sizeof(struct mak__f), UINT32_C(1225484184), (const void *)(rt_heap_obj_444)) ) {
                                *(arena_case_427) = *(&mak_ir_default_obj__f);
                            }
                        }
                    }
                    else if ((scrut_428 != NULL && (scrut_428)->tag == mak_ir_tag_0)) {
                        struct mak__f *rt_heap_obj_445 = (struct mak__f *)(uintptr_t)((struct mak__f *)(uintptr_t)(mak_rt_call_ctor("NBad:_f", (const uint64_t[]){}, (const uint8_t[]){}, 0)));
                        if (!mak_ir_adt_copy_checked((void *)(arena_case_427), sizeof(struct mak__f), UINT32_C(1225484184), (const void *)(rt_heap_obj_445)) ) {
                            *(arena_case_427) = *(&mak_ir_default_obj__f);
                        }
                    }
                    else {
                        *(arena_case_427) = *(&mak_ir_default_obj__f);
                    }
                }
                struct mak__f * scrut_426 = arena_case_427;
                if ((scrut_426 != NULL && (scrut_426)->tag == mak_ir_tag_11)) {
                    *(out) = mak_ir_ctor_obj_11;
                }
                else if ((scrut_426 != NULL && (scrut_426)->tag == mak_ir_tag_10)) {
                    double mak_local_fx_174 = (scrut_426)->v.v10.f0;
                    struct mak__a * mak_local_fy_175 = (scrut_426)->v.v10.f1;
                    struct mak__a *arena_call_447 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
                    if (!arena_call_447) {
                        mak_ir_recover("ADT result allocation failed");
                        arena_call_447 = &mak_ir_default_obj__a;
                     } else {
                        mak_ir_fn_2(mak_local_fy_175, (struct mak__a *)(arena_call_447));
                    }
                    struct mak__a * scrut_446 = arena_call_447;
                    if ((scrut_446 != NULL && (scrut_446)->tag == mak_ir_tag_1) && (scrut_446->v.v1.f0 == INT64_C(40))) {
                        struct mak__a * mak_local_fz_176 = (scrut_446)->v.v1.f1;
                        *(out) = mak_ir_ctor_obj_11;
                    }
                    else if (1) {
                        *(out) = mak_ir_ctor_obj_11;
                    }
                }
                else {
                    *(out) = *(&mak_ir_default_obj__f);
                }
            }
            else if (1) {
                struct mak__f *arena_case_449 = (struct mak__f *)mak_ir_arena_alloc(sizeof(struct mak__f), _Alignof(struct mak__f));
                if (!arena_case_449) {
                    mak_ir_recover("ADT case-result allocation failed");
                    arena_case_449 = &mak_ir_default_obj__f;
                 } else {
                    struct mak__a *arena_call_451 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
                    if (!arena_call_451) {
                        mak_ir_recover("ADT result allocation failed");
                        arena_call_451 = &mak_ir_default_obj__a;
                     } else {
                        mak_ir_fn_2(a0, (struct mak__a *)(arena_call_451));
                    }
                    struct mak__a * scrut_450 = arena_call_451;
                    if ((scrut_450 != NULL && (scrut_450)->tag == mak_ir_tag_0)) {
                        struct mak__f *rt_heap_obj_452 = (struct mak__f *)(uintptr_t)((struct mak__f *)(uintptr_t)(mak_rt_call_ctor("NBad:_f", (const uint64_t[]){}, (const uint8_t[]){}, 0)));
                        if (!mak_ir_adt_copy_checked((void *)(arena_case_449), sizeof(struct mak__f), UINT32_C(1225484184), (const void *)(rt_heap_obj_452)) ) {
                            *(arena_case_449) = *(&mak_ir_default_obj__f);
                        }
                    }
                    else if ((scrut_450 != NULL && (scrut_450)->tag == mak_ir_tag_1)) {
                        int32_t mak_local_dk_195 = (scrut_450)->v.v1.f0;
                        struct mak__a * mak_local_dl_196 = (scrut_450)->v.v1.f1;
                        int64_t case_454 = 0;
                        int32_t scrut_455 = mak_local_dk_195 >= (int32_t)(INT64_C(48));
                        if ((scrut_455 == INT64_C(0))) {
                            case_454 = INT64_C(0);
                        }
                        else if (1) {
                            int64_t case_456 = 0;
                            int32_t scrut_457 = mak_local_dk_195 <= (int32_t)(INT64_C(57));
                            if ((scrut_457 == INT64_C(1))) {
                                case_456 = INT64_C(1);
                            }
                            else if (1) {
                                case_456 = INT64_C(0);
                            }
                            case_454 = case_456;
                        }
                        int64_t scrut_453 = case_454;
                        if ((scrut_453 == INT64_C(1))) {
                            struct mak__d *arena_call_459 = (struct mak__d *)mak_ir_arena_alloc(sizeof(struct mak__d), _Alignof(struct mak__d));
                            if (!arena_call_459) {
                                mak_ir_recover("ADT result allocation failed");
                                arena_call_459 = &mak_ir_default_obj__d;
                             } else {
                                struct mak__a *arena_obj_460 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
                                if (!arena_obj_460) {
                                    mak_ir_recover("ADT object allocation failed");
                                    arena_obj_460 = &mak_ir_default_obj__a;
                                 } else {
                                    (arena_obj_460)->tag = mak_ir_tag_1;
                                    (arena_obj_460)->v.v1.f0 = mak_local_dk_195;
                                    (arena_obj_460)->v.v1.f1 = mak_local_dl_196;
                                }
                                mak_ir_fn_7(arena_obj_460, 0.00000000000000000, (struct mak__d *)(arena_call_459));
                            }
                            struct mak__d * scrut_458 = arena_call_459;
                            if ((scrut_458 != NULL && (scrut_458)->tag == mak_ir_tag_6)) {
                                struct mak__f *rt_heap_obj_461 = (struct mak__f *)(uintptr_t)((struct mak__f *)(uintptr_t)(mak_rt_call_ctor("NBad:_f", (const uint64_t[]){}, (const uint8_t[]){}, 0)));
                                if (!mak_ir_adt_copy_checked((void *)(arena_case_449), sizeof(struct mak__f), UINT32_C(1225484184), (const void *)(rt_heap_obj_461)) ) {
                                    *(arena_case_449) = *(&mak_ir_default_obj__f);
                                }
                            }
                            else if ((scrut_458 != NULL && (scrut_458)->tag == mak_ir_tag_7)) {
                                double mak_local_dm_197 = (scrut_458)->v.v7.f0;
                                struct mak__a * mak_local_dn_198 = (scrut_458)->v.v7.f1;
                                if ((mak_local_dn_198 != NULL && (mak_local_dn_198)->tag == mak_ir_tag_1) && (mak_local_dn_198->v.v1.f0 == INT64_C(46))) {
                                    struct mak__a * mak_local_dp_199 = (mak_local_dn_198)->v.v1.f1;
                                    struct mak__d *arena_call_462 = (struct mak__d *)mak_ir_arena_alloc(sizeof(struct mak__d), _Alignof(struct mak__d));
                                    if (!arena_call_462) {
                                        mak_ir_recover("ADT result allocation failed");
                                        arena_call_462 = &mak_ir_default_obj__d;
                                     } else {
                                        mak_ir_fn_8(mak_local_dp_199, mak_local_dm_197, 0.10000000000000001, (struct mak__d *)(arena_call_462));
                                    }
                                    if (!mak_ir_adt_copy_checked((void *)(arena_case_449), sizeof(struct mak__f), UINT32_C(1225484184), (const void *)(uintptr_t)(arena_call_462))) {
                                        *(arena_case_449) = *(&mak_ir_default_obj__f);
                                    }
                                }
                                else if ((mak_local_dn_198 != NULL && (mak_local_dn_198)->tag == mak_ir_tag_1)) {
                                    int32_t mak_local_dq_200 = (mak_local_dn_198)->v.v1.f0;
                                    struct mak__a * mak_local_dr_201 = (mak_local_dn_198)->v.v1.f1;
                                    switch (mak_local_dq_200) {
                                        case INT64_C(101): {
                                            struct mak__e *arena_call_464 = (struct mak__e *)mak_ir_arena_alloc(sizeof(struct mak__e), _Alignof(struct mak__e));
                                            if (!arena_call_464) {
                                                mak_ir_recover("ADT result allocation failed");
                                                arena_call_464 = &mak_ir_default_obj__e;
                                             } else {
                                                mak_ir_fn_6(mak_local_dr_201, INT64_C(1), (struct mak__e *)(arena_call_464));
                                            }
                                            struct mak__e * scrut_463 = arena_call_464;
                                            if ((scrut_463 != NULL && (scrut_463)->tag == mak_ir_tag_8)) {
                                                struct mak__f *rt_heap_obj_465 = (struct mak__f *)(uintptr_t)((struct mak__f *)(uintptr_t)(mak_rt_call_ctor("NBad:_f", (const uint64_t[]){}, (const uint8_t[]){}, 0)));
                                                if (!mak_ir_adt_copy_checked((void *)(arena_case_449), sizeof(struct mak__f), UINT32_C(1225484184), (const void *)(rt_heap_obj_465)) ) {
                                                    *(arena_case_449) = *(&mak_ir_default_obj__f);
                                                }
                                            }
                                            else if ((scrut_463 != NULL && (scrut_463)->tag == mak_ir_tag_9)) {
                                                int64_t mak_local_ds_202 = (scrut_463)->v.v9.f0;
                                                struct mak__a * mak_local_dt_203 = (scrut_463)->v.v9.f1;
                                                struct mak__d *arena_obj_466 = (struct mak__d *)mak_ir_arena_alloc(sizeof(struct mak__d), _Alignof(struct mak__d));
                                                if (!arena_obj_466) {
                                                    mak_ir_recover("ADT object allocation failed");
                                                    arena_obj_466 = &mak_ir_default_obj__d;
                                                 } else {
                                                    (arena_obj_466)->tag = mak_ir_tag_7;
                                                    double eval_467 = mak_ir_fn_4(mak_local_ds_202);
                                                    (arena_obj_466)->v.v7.f0 = ((double)(mak_local_dm_197) * (double)(eval_467));
                                                    (arena_obj_466)->v.v7.f1 = mak_local_dt_203;
                                                }
                                                if (!mak_ir_adt_copy_checked((void *)(arena_case_449), sizeof(struct mak__f), UINT32_C(1225484184), (const void *)(uintptr_t)(arena_obj_466))) {
                                                    *(arena_case_449) = *(&mak_ir_default_obj__f);
                                                }
                                            }
                                            else {
                                                *(arena_case_449) = *(&mak_ir_default_obj__f);
                                            }
                                            break;
                                        }
                                        case INT64_C(69): {
                                            struct mak__e *arena_call_469 = (struct mak__e *)mak_ir_arena_alloc(sizeof(struct mak__e), _Alignof(struct mak__e));
                                            if (!arena_call_469) {
                                                mak_ir_recover("ADT result allocation failed");
                                                arena_call_469 = &mak_ir_default_obj__e;
                                             } else {
                                                mak_ir_fn_6(mak_local_dr_201, INT64_C(1), (struct mak__e *)(arena_call_469));
                                            }
                                            struct mak__e * scrut_468 = arena_call_469;
                                            if ((scrut_468 != NULL && (scrut_468)->tag == mak_ir_tag_8)) {
                                                struct mak__f *rt_heap_obj_470 = (struct mak__f *)(uintptr_t)((struct mak__f *)(uintptr_t)(mak_rt_call_ctor("NBad:_f", (const uint64_t[]){}, (const uint8_t[]){}, 0)));
                                                if (!mak_ir_adt_copy_checked((void *)(arena_case_449), sizeof(struct mak__f), UINT32_C(1225484184), (const void *)(rt_heap_obj_470)) ) {
                                                    *(arena_case_449) = *(&mak_ir_default_obj__f);
                                                }
                                            }
                                            else if ((scrut_468 != NULL && (scrut_468)->tag == mak_ir_tag_9)) {
                                                int64_t mak_local_du_204 = (scrut_468)->v.v9.f0;
                                                struct mak__a * mak_local_dv_205 = (scrut_468)->v.v9.f1;
                                                struct mak__d *arena_obj_471 = (struct mak__d *)mak_ir_arena_alloc(sizeof(struct mak__d), _Alignof(struct mak__d));
                                                if (!arena_obj_471) {
                                                    mak_ir_recover("ADT object allocation failed");
                                                    arena_obj_471 = &mak_ir_default_obj__d;
                                                 } else {
                                                    (arena_obj_471)->tag = mak_ir_tag_7;
                                                    double eval_472 = mak_ir_fn_4(mak_local_du_204);
                                                    (arena_obj_471)->v.v7.f0 = ((double)(mak_local_dm_197) * (double)(eval_472));
                                                    (arena_obj_471)->v.v7.f1 = mak_local_dv_205;
                                                }
                                                if (!mak_ir_adt_copy_checked((void *)(arena_case_449), sizeof(struct mak__f), UINT32_C(1225484184), (const void *)(uintptr_t)(arena_obj_471))) {
                                                    *(arena_case_449) = *(&mak_ir_default_obj__f);
                                                }
                                            }
                                            else {
                                                *(arena_case_449) = *(&mak_ir_default_obj__f);
                                            }
                                            break;
                                        }
                                        default: {
                                            struct mak__d *arena_obj_473 = (struct mak__d *)mak_ir_arena_alloc(sizeof(struct mak__d), _Alignof(struct mak__d));
                                            if (!arena_obj_473) {
                                                mak_ir_recover("ADT object allocation failed");
                                                arena_obj_473 = &mak_ir_default_obj__d;
                                             } else {
                                                (arena_obj_473)->tag = mak_ir_tag_7;
                                                (arena_obj_473)->v.v7.f0 = mak_local_dm_197;
                                                (arena_obj_473)->v.v7.f1 = mak_local_dn_198;
                                            }
                                            if (!mak_ir_adt_copy_checked((void *)(arena_case_449), sizeof(struct mak__f), UINT32_C(1225484184), (const void *)(uintptr_t)(arena_obj_473))) {
                                                *(arena_case_449) = *(&mak_ir_default_obj__f);
                                            }
                                            break;
                                        }
                                    }
                                }
                                else if (1) {
                                    struct mak__d *arena_obj_474 = (struct mak__d *)mak_ir_arena_alloc(sizeof(struct mak__d), _Alignof(struct mak__d));
                                    if (!arena_obj_474) {
                                        mak_ir_recover("ADT object allocation failed");
                                        arena_obj_474 = &mak_ir_default_obj__d;
                                     } else {
                                        (arena_obj_474)->tag = mak_ir_tag_7;
                                        (arena_obj_474)->v.v7.f0 = mak_local_dm_197;
                                        (arena_obj_474)->v.v7.f1 = mak_local_dn_198;
                                    }
                                    if (!mak_ir_adt_copy_checked((void *)(arena_case_449), sizeof(struct mak__f), UINT32_C(1225484184), (const void *)(uintptr_t)(arena_obj_474))) {
                                        *(arena_case_449) = *(&mak_ir_default_obj__f);
                                    }
                                }
                            }
                            else {
                                *(arena_case_449) = *(&mak_ir_default_obj__f);
                            }
                        }
                        else if (1) {
                            if ((mak_local_dk_195 == INT64_C(46))) {
                                if ((mak_local_dl_196 != NULL && (mak_local_dl_196)->tag == mak_ir_tag_1)) {
                                    int32_t mak_local_dw_206 = (mak_local_dl_196)->v.v1.f0;
                                    int64_t case_476 = 0;
                                    int32_t scrut_477 = mak_local_dw_206 >= (int32_t)(INT64_C(48));
                                    if ((scrut_477 == INT64_C(0))) {
                                        case_476 = INT64_C(0);
                                    }
                                    else if (1) {
                                        int64_t case_478 = 0;
                                        int32_t scrut_479 = mak_local_dw_206 <= (int32_t)(INT64_C(57));
                                        if ((scrut_479 == INT64_C(1))) {
                                            case_478 = INT64_C(1);
                                        }
                                        else if (1) {
                                            case_478 = INT64_C(0);
                                        }
                                        case_476 = case_478;
                                    }
                                    int64_t scrut_475 = case_476;
                                    if ((scrut_475 == INT64_C(1))) {
                                        struct mak__d *arena_call_480 = (struct mak__d *)mak_ir_arena_alloc(sizeof(struct mak__d), _Alignof(struct mak__d));
                                        if (!arena_call_480) {
                                            mak_ir_recover("ADT result allocation failed");
                                            arena_call_480 = &mak_ir_default_obj__d;
                                         } else {
                                            mak_ir_fn_8(mak_local_dl_196, 0.00000000000000000, 0.10000000000000001, (struct mak__d *)(arena_call_480));
                                        }
                                        if (!mak_ir_adt_copy_checked((void *)(arena_case_449), sizeof(struct mak__f), UINT32_C(1225484184), (const void *)(uintptr_t)(arena_call_480))) {
                                            *(arena_case_449) = *(&mak_ir_default_obj__f);
                                        }
                                    }
                                    else if (1) {
                                        struct mak__f *rt_heap_obj_481 = (struct mak__f *)(uintptr_t)((struct mak__f *)(uintptr_t)(mak_rt_call_ctor("NBad:_f", (const uint64_t[]){}, (const uint8_t[]){}, 0)));
                                        if (!mak_ir_adt_copy_checked((void *)(arena_case_449), sizeof(struct mak__f), UINT32_C(1225484184), (const void *)(rt_heap_obj_481)) ) {
                                            *(arena_case_449) = *(&mak_ir_default_obj__f);
                                        }
                                    }
                                }
                                else if (1) {
                                    struct mak__f *rt_heap_obj_482 = (struct mak__f *)(uintptr_t)((struct mak__f *)(uintptr_t)(mak_rt_call_ctor("NBad:_f", (const uint64_t[]){}, (const uint8_t[]){}, 0)));
                                    if (!mak_ir_adt_copy_checked((void *)(arena_case_449), sizeof(struct mak__f), UINT32_C(1225484184), (const void *)(rt_heap_obj_482)) ) {
                                        *(arena_case_449) = *(&mak_ir_default_obj__f);
                                    }
                                }
                            }
                            else if (1) {
                                struct mak__f *rt_heap_obj_483 = (struct mak__f *)(uintptr_t)((struct mak__f *)(uintptr_t)(mak_rt_call_ctor("NBad:_f", (const uint64_t[]){}, (const uint8_t[]){}, 0)));
                                if (!mak_ir_adt_copy_checked((void *)(arena_case_449), sizeof(struct mak__f), UINT32_C(1225484184), (const void *)(rt_heap_obj_483)) ) {
                                    *(arena_case_449) = *(&mak_ir_default_obj__f);
                                }
                            }
                        }
                    }
                    else {
                        *(arena_case_449) = *(&mak_ir_default_obj__f);
                    }
                }
                struct mak__f * scrut_448 = arena_case_449;
                if ((scrut_448 != NULL && (scrut_448)->tag == mak_ir_tag_11)) {
                    *(out) = mak_ir_ctor_obj_11;
                }
                else if ((scrut_448 != NULL && (scrut_448)->tag == mak_ir_tag_10)) {
                    double mak_local_ga_207 = (scrut_448)->v.v10.f0;
                    struct mak__a * mak_local_gb_208 = (scrut_448)->v.v10.f1;
                    (out)->tag = mak_ir_tag_10;
                    (out)->v.v10.f0 = mak_local_ga_207;
                    (out)->v.v10.f1 = mak_local_gb_208;
                }
                else {
                    *(out) = *(&mak_ir_default_obj__f);
                }
            }
        }
    }
    else {
        *(out) = *(&mak_ir_default_obj__f);
    }
    return;
}

static inline void mak_ir_fn_23(struct mak__a * a0, struct mak__c *out) {
    struct mak__a *arena_call_485 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
    if (!arena_call_485) {
        mak_ir_recover("ADT result allocation failed");
        arena_call_485 = &mak_ir_default_obj__a;
     } else {
        mak_ir_fn_2(a0, (struct mak__a *)(arena_call_485));
    }
    struct mak__a * scrut_484 = arena_call_485;
    if ((scrut_484 != NULL && (scrut_484)->tag == mak_ir_tag_1)) {
        int32_t mak_local_ce_209 = (scrut_484)->v.v1.f0;
        struct mak__a * mak_local_cf_210 = (scrut_484)->v.v1.f1;
        int64_t case_487 = 0;
        int32_t scrut_488 = mak_local_ce_209 == (int32_t)(INT64_C(95));
        if ((scrut_488 == INT64_C(1))) {
            case_487 = INT64_C(1);
        }
        else if (1) {
            int64_t case_489 = 0;
            int32_t scrut_490 = mak_local_ce_209 >= (int32_t)(INT64_C(65));
            if ((scrut_490 == INT64_C(0))) {
                int64_t case_491 = 0;
                int32_t scrut_492 = mak_local_ce_209 >= (int32_t)(INT64_C(97));
                if ((scrut_492 == INT64_C(0))) {
                    case_491 = INT64_C(0);
                }
                else if (1) {
                    int64_t case_493 = 0;
                    int32_t scrut_494 = mak_local_ce_209 <= (int32_t)(INT64_C(122));
                    if ((scrut_494 == INT64_C(1))) {
                        case_493 = INT64_C(1);
                    }
                    else if (1) {
                        case_493 = INT64_C(0);
                    }
                    case_491 = case_493;
                }
                case_489 = case_491;
            }
            else if (1) {
                int64_t case_495 = 0;
                int32_t scrut_496 = mak_local_ce_209 <= (int32_t)(INT64_C(90));
                if ((scrut_496 == INT64_C(1))) {
                    case_495 = INT64_C(1);
                }
                else if (1) {
                    case_495 = INT64_C(0);
                }
                case_489 = case_495;
            }
            case_487 = case_489;
        }
        int64_t scrut_486 = case_487;
        if ((scrut_486 == INT64_C(1))) {
            struct mak__a *arena_obj_497 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
            if (!arena_obj_497) {
                mak_ir_recover("ADT object allocation failed");
                arena_obj_497 = &mak_ir_default_obj__a;
             } else {
                (arena_obj_497)->tag = mak_ir_tag_1;
                (arena_obj_497)->v.v1.f0 = mak_local_ce_209;
                (arena_obj_497)->v.v1.f1 = mak_local_cf_210;
            }
            mak_ir_fn_3(arena_obj_497, INT64_C(0), (struct mak__c *)(out));
        }
        else if (1) {
            *(out) = mak_ir_ctor_obj_4;
        }
    }
    else if ((scrut_484 != NULL && (scrut_484)->tag == mak_ir_tag_0)) {
        *(out) = mak_ir_ctor_obj_4;
    }
    else {
        *(out) = *(&mak_ir_default_obj__c);
    }
    return;
}

static inline void mak_ir_fn_24(int64_t a0, int64_t a1, struct mak__f *out) {
    int64_t case_499 = 0;
    switch (a0) {
        case INT64_C(399964812): {
            case_499 = INT64_C(1);
            break;
        }
        case INT64_C(1764798): {
            case_499 = INT64_C(1);
            break;
        }
        default: {
            case_499 = INT64_C(0);
            break;
        }
    }
    int64_t scrut_498 = case_499;
    if ((scrut_498 == INT64_C(1))) {
        struct mak__f *arena_call_501 = (struct mak__f *)mak_ir_arena_alloc(sizeof(struct mak__f), _Alignof(struct mak__f));
        if (!arena_call_501) {
            mak_ir_recover("ADT result allocation failed");
            arena_call_501 = &mak_ir_default_obj__f;
         } else {
            mak_ir_fn_16((struct mak__a *)mak_ir_adt_recover_ptr((const void *)&mak_ir_default_obj__a, "integer used as ADT"), (struct mak__f *)(arena_call_501));
        }
        struct mak__f * scrut_500 = arena_call_501;
        if ((scrut_500 != NULL && (scrut_500)->tag == mak_ir_tag_11)) {
            *(out) = mak_ir_ctor_obj_11;
        }
        else if ((scrut_500 != NULL && (scrut_500)->tag == mak_ir_tag_10)) {
            double mak_local_ff_211 = (scrut_500)->v.v10.f0;
            struct mak__a * mak_local_fg_212 = (scrut_500)->v.v10.f1;
            struct mak__a *arena_call_503 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
            if (!arena_call_503) {
                mak_ir_recover("ADT result allocation failed");
                arena_call_503 = &mak_ir_default_obj__a;
             } else {
                mak_ir_fn_2(mak_local_fg_212, (struct mak__a *)(arena_call_503));
            }
            struct mak__a * scrut_502 = arena_call_503;
            if ((scrut_502 != NULL && (scrut_502)->tag == mak_ir_tag_1) && (scrut_502->v.v1.f0 == INT64_C(44))) {
                struct mak__a * mak_local_fh_213 = (scrut_502)->v.v1.f1;
                struct mak__f *arena_call_505 = (struct mak__f *)mak_ir_arena_alloc(sizeof(struct mak__f), _Alignof(struct mak__f));
                if (!arena_call_505) {
                    mak_ir_recover("ADT result allocation failed");
                    arena_call_505 = &mak_ir_default_obj__f;
                 } else {
                    mak_ir_fn_16(mak_local_fh_213, (struct mak__f *)(arena_call_505));
                }
                struct mak__f * scrut_504 = arena_call_505;
                if ((scrut_504 != NULL && (scrut_504)->tag == mak_ir_tag_11)) {
                    *(out) = mak_ir_ctor_obj_11;
                }
                else if ((scrut_504 != NULL && (scrut_504)->tag == mak_ir_tag_10)) {
                    double mak_local_fi_214 = (scrut_504)->v.v10.f0;
                    struct mak__a * mak_local_fj_215 = (scrut_504)->v.v10.f1;
                    struct mak__a *arena_call_507 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
                    if (!arena_call_507) {
                        mak_ir_recover("ADT result allocation failed");
                        arena_call_507 = &mak_ir_default_obj__a;
                     } else {
                        mak_ir_fn_2(mak_local_fj_215, (struct mak__a *)(arena_call_507));
                    }
                    struct mak__a * scrut_506 = arena_call_507;
                    if ((scrut_506 != NULL && (scrut_506)->tag == mak_ir_tag_1) && (scrut_506->v.v1.f0 == INT64_C(44))) {
                        struct mak__a * mak_local_fk_216 = (scrut_506)->v.v1.f1;
                        struct mak__f *arena_call_509 = (struct mak__f *)mak_ir_arena_alloc(sizeof(struct mak__f), _Alignof(struct mak__f));
                        if (!arena_call_509) {
                            mak_ir_recover("ADT result allocation failed");
                            arena_call_509 = &mak_ir_default_obj__f;
                         } else {
                            mak_ir_fn_16(mak_local_fk_216, (struct mak__f *)(arena_call_509));
                        }
                        struct mak__f * scrut_508 = arena_call_509;
                        if ((scrut_508 != NULL && (scrut_508)->tag == mak_ir_tag_11)) {
                            *(out) = mak_ir_ctor_obj_11;
                        }
                        else if ((scrut_508 != NULL && (scrut_508)->tag == mak_ir_tag_10)) {
                            double mak_local_fl_217 = (scrut_508)->v.v10.f0;
                            struct mak__a * mak_local_fm_218 = (scrut_508)->v.v10.f1;
                            struct mak__a *arena_call_511 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
                            if (!arena_call_511) {
                                mak_ir_recover("ADT result allocation failed");
                                arena_call_511 = &mak_ir_default_obj__a;
                             } else {
                                mak_ir_fn_2(mak_local_fm_218, (struct mak__a *)(arena_call_511));
                            }
                            struct mak__a * scrut_510 = arena_call_511;
                            if ((scrut_510 != NULL && (scrut_510)->tag == mak_ir_tag_1) && (scrut_510->v.v1.f0 == INT64_C(41))) {
                                struct mak__a * mak_local_fo_219 = (scrut_510)->v.v1.f1;
                                int64_t case_513 = 0;
                                switch (a0) {
                                    case INT64_C(399964812): {
                                        case_513 = INT64_C(1);
                                        break;
                                    }
                                    case INT64_C(1764798): {
                                        case_513 = INT64_C(1);
                                        break;
                                    }
                                    default: {
                                        case_513 = INT64_C(0);
                                        break;
                                    }
                                }
                                int64_t scrut_512 = case_513;
                                if ((scrut_512 == INT64_C(1))) {
                                    (out)->tag = mak_ir_tag_10;
                                    double case_514 = 0.0;
                                    switch (a0) {
                                        case INT64_C(399964812): {
                                            double case_515 = 0.0;
                                            int32_t scrut_516 = (int32_t)(mak_local_ff_211) < (int32_t)(mak_local_fi_214);
                                            if ((scrut_516 == INT64_C(1))) {
                                                case_515 = mak_local_fi_214;
                                            }
                                            else if (1) {
                                                double case_517 = 0.0;
                                                int32_t scrut_518 = (int32_t)(mak_local_ff_211) > (int32_t)(mak_local_fl_217);
                                                if ((scrut_518 == INT64_C(1))) {
                                                    case_517 = mak_local_fl_217;
                                                }
                                                else if (1) {
                                                    case_517 = mak_local_ff_211;
                                                }
                                                case_515 = case_517;
                                            }
                                            case_514 = case_515;
                                            break;
                                        }
                                        case INT64_C(1764798): {
                                            case_514 = mak_ir_ext_45(mak_local_ff_211, mak_local_fi_214, mak_local_fl_217);
                                            break;
                                        }
                                        default: {
                                            case_514 = mak_local_ff_211;
                                            break;
                                        }
                                    }
                                    (out)->v.v10.f0 = case_514;
                                    (out)->v.v10.f1 = mak_local_fo_219;
                                }
                                else if (1) {
                                    *(out) = mak_ir_ctor_obj_11;
                                }
                            }
                            else if (1) {
                                *(out) = mak_ir_ctor_obj_11;
                            }
                        }
                        else {
                            *(out) = *(&mak_ir_default_obj__f);
                        }
                    }
                    else if (1) {
                        *(out) = mak_ir_ctor_obj_11;
                    }
                }
                else {
                    *(out) = *(&mak_ir_default_obj__f);
                }
            }
            else if (1) {
                *(out) = mak_ir_ctor_obj_11;
            }
        }
        else {
            *(out) = *(&mak_ir_default_obj__f);
        }
    }
    else if (1) {
        int64_t case_520 = 0;
        switch (a0) {
            case INT64_C(828949774): {
                case_520 = INT64_C(1);
                break;
            }
            case INT64_C(231190472): {
                case_520 = INT64_C(1);
                break;
            }
            case INT64_C(1885190): {
                case_520 = INT64_C(1);
                break;
            }
            case INT64_C(1969694): {
                case_520 = INT64_C(1);
                break;
            }
            case INT64_C(510764621): {
                case_520 = INT64_C(1);
                break;
            }
            case INT64_C(1936692): {
                case_520 = INT64_C(1);
                break;
            }
            case INT64_C(258201902): {
                case_520 = INT64_C(1);
                break;
            }
            case INT64_C(548941060): {
                case_520 = INT64_C(1);
                break;
            }
            case INT64_C(901947274): {
                case_520 = INT64_C(1);
                break;
            }
            case INT64_C(1780652): {
                case_520 = INT64_C(1);
                break;
            }
            case INT64_C(1866466): {
                case_520 = INT64_C(1);
                break;
            }
            case INT64_C(1900793): {
                case_520 = INT64_C(1);
                break;
            }
            case INT64_C(224480257): {
                case_520 = INT64_C(1);
                break;
            }
            case INT64_C(1902496): {
                case_520 = INT64_C(1);
                break;
            }
            case INT64_C(253534496): {
                case_520 = INT64_C(1);
                break;
            }
            case INT64_C(1884414): {
                case_520 = INT64_C(1);
                break;
            }
            case INT64_C(1883376): {
                case_520 = INT64_C(1);
                break;
            }
            case INT64_C(262106026): {
                case_520 = INT64_C(1);
                break;
            }
            case INT64_C(231035246): {
                case_520 = INT64_C(1);
                break;
            }
            case INT64_C(231188658): {
                case_520 = INT64_C(1);
                break;
            }
            case INT64_C(231189696): {
                case_520 = INT64_C(1);
                break;
            }
            case INT64_C(743972013): {
                case_520 = INT64_C(1);
                break;
            }
            default: {
                case_520 = INT64_C(0);
                break;
            }
        }
        int64_t scrut_519 = case_520;
        if ((scrut_519 == INT64_C(1))) {
            struct mak__f *arena_call_522 = (struct mak__f *)mak_ir_arena_alloc(sizeof(struct mak__f), _Alignof(struct mak__f));
            if (!arena_call_522) {
                mak_ir_recover("ADT result allocation failed");
                arena_call_522 = &mak_ir_default_obj__f;
             } else {
                mak_ir_fn_16((struct mak__a *)mak_ir_adt_recover_ptr((const void *)&mak_ir_default_obj__a, "integer used as ADT"), (struct mak__f *)(arena_call_522));
            }
            struct mak__f * scrut_521 = arena_call_522;
            if ((scrut_521 != NULL && (scrut_521)->tag == mak_ir_tag_11)) {
                *(out) = mak_ir_ctor_obj_11;
            }
            else if ((scrut_521 != NULL && (scrut_521)->tag == mak_ir_tag_10)) {
                double mak_local_ex_220 = (scrut_521)->v.v10.f0;
                struct mak__a * mak_local_ey_221 = (scrut_521)->v.v10.f1;
                struct mak__a *arena_call_524 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
                if (!arena_call_524) {
                    mak_ir_recover("ADT result allocation failed");
                    arena_call_524 = &mak_ir_default_obj__a;
                 } else {
                    mak_ir_fn_2(mak_local_ey_221, (struct mak__a *)(arena_call_524));
                }
                struct mak__a * scrut_523 = arena_call_524;
                if ((scrut_523 != NULL && (scrut_523)->tag == mak_ir_tag_1) && (scrut_523->v.v1.f0 == INT64_C(44))) {
                    struct mak__a * mak_local_ez_222 = (scrut_523)->v.v1.f1;
                    struct mak__f *arena_call_526 = (struct mak__f *)mak_ir_arena_alloc(sizeof(struct mak__f), _Alignof(struct mak__f));
                    if (!arena_call_526) {
                        mak_ir_recover("ADT result allocation failed");
                        arena_call_526 = &mak_ir_default_obj__f;
                     } else {
                        mak_ir_fn_16(mak_local_ez_222, (struct mak__f *)(arena_call_526));
                    }
                    struct mak__f * scrut_525 = arena_call_526;
                    if ((scrut_525 != NULL && (scrut_525)->tag == mak_ir_tag_11)) {
                        *(out) = mak_ir_ctor_obj_11;
                    }
                    else if ((scrut_525 != NULL && (scrut_525)->tag == mak_ir_tag_10)) {
                        double mak_local_fa_223 = (scrut_525)->v.v10.f0;
                        struct mak__a * mak_local_fb_224 = (scrut_525)->v.v10.f1;
                        struct mak__a *arena_call_528 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
                        if (!arena_call_528) {
                            mak_ir_recover("ADT result allocation failed");
                            arena_call_528 = &mak_ir_default_obj__a;
                         } else {
                            mak_ir_fn_2(mak_local_fb_224, (struct mak__a *)(arena_call_528));
                        }
                        struct mak__a * scrut_527 = arena_call_528;
                        if ((scrut_527 != NULL && (scrut_527)->tag == mak_ir_tag_1) && (scrut_527->v.v1.f0 == INT64_C(41))) {
                            struct mak__a * mak_local_fc_225 = (scrut_527)->v.v1.f1;
                            int64_t case_530 = 0;
                            switch (a0) {
                                case INT64_C(828949774): {
                                    case_530 = INT64_C(1);
                                    break;
                                }
                                case INT64_C(231190472): {
                                    case_530 = INT64_C(1);
                                    break;
                                }
                                case INT64_C(1885190): {
                                    case_530 = INT64_C(1);
                                    break;
                                }
                                case INT64_C(1969694): {
                                    case_530 = INT64_C(1);
                                    break;
                                }
                                case INT64_C(510764621): {
                                    case_530 = INT64_C(1);
                                    break;
                                }
                                case INT64_C(1936692): {
                                    case_530 = INT64_C(1);
                                    break;
                                }
                                case INT64_C(258201902): {
                                    case_530 = INT64_C(1);
                                    break;
                                }
                                case INT64_C(548941060): {
                                    case_530 = INT64_C(1);
                                    break;
                                }
                                case INT64_C(901947274): {
                                    case_530 = INT64_C(1);
                                    break;
                                }
                                case INT64_C(1780652): {
                                    case_530 = INT64_C(1);
                                    break;
                                }
                                case INT64_C(1866466): {
                                    case_530 = INT64_C(1);
                                    break;
                                }
                                case INT64_C(1900793): {
                                    case_530 = INT64_C(1);
                                    break;
                                }
                                case INT64_C(224480257): {
                                    case_530 = INT64_C(1);
                                    break;
                                }
                                case INT64_C(1902496): {
                                    case_530 = INT64_C(1);
                                    break;
                                }
                                case INT64_C(253534496): {
                                    case_530 = INT64_C(1);
                                    break;
                                }
                                case INT64_C(1884414): {
                                    case_530 = INT64_C(1);
                                    break;
                                }
                                case INT64_C(1883376): {
                                    case_530 = INT64_C(1);
                                    break;
                                }
                                case INT64_C(262106026): {
                                    case_530 = INT64_C(1);
                                    break;
                                }
                                case INT64_C(231035246): {
                                    case_530 = INT64_C(1);
                                    break;
                                }
                                case INT64_C(231188658): {
                                    case_530 = INT64_C(1);
                                    break;
                                }
                                case INT64_C(231189696): {
                                    case_530 = INT64_C(1);
                                    break;
                                }
                                case INT64_C(743972013): {
                                    case_530 = INT64_C(1);
                                    break;
                                }
                                default: {
                                    case_530 = INT64_C(0);
                                    break;
                                }
                            }
                            int64_t scrut_529 = case_530;
                            if ((scrut_529 == INT64_C(1))) {
                                (out)->tag = mak_ir_tag_10;
                                (out)->v.v10.f0 = mak_ir_fn_9(a0, mak_local_ex_220, mak_local_fa_223);
                                (out)->v.v10.f1 = mak_local_fc_225;
                            }
                            else if (1) {
                                *(out) = mak_ir_ctor_obj_11;
                            }
                        }
                        else if (1) {
                            *(out) = mak_ir_ctor_obj_11;
                        }
                    }
                    else {
                        *(out) = *(&mak_ir_default_obj__f);
                    }
                }
                else if (1) {
                    *(out) = mak_ir_ctor_obj_11;
                }
            }
            else {
                *(out) = *(&mak_ir_default_obj__f);
            }
        }
        else if (1) {
            int64_t case_532 = 0;
            switch (a0) {
                case INT64_C(1677570): {
                    case_532 = INT64_C(1);
                    break;
                }
                case INT64_C(230982852): {
                    case_532 = INT64_C(1);
                    break;
                }
                case INT64_C(260484708): {
                    case_532 = INT64_C(1);
                    break;
                }
                case INT64_C(224257837): {
                    case_532 = INT64_C(1);
                    break;
                }
                case INT64_C(1749093): {
                    case_532 = INT64_C(1);
                    break;
                }
                case INT64_C(229131233): {
                    case_532 = INT64_C(1);
                    break;
                }
                case INT64_C(16199091): {
                    case_532 = INT64_C(1);
                    break;
                }
                case INT64_C(1868032): {
                    case_532 = INT64_C(1);
                    break;
                }
                case INT64_C(14258): {
                    case_532 = INT64_C(1);
                    break;
                }
                case INT64_C(244712242): {
                    case_532 = INT64_C(1);
                    break;
                }
                case INT64_C(57303395): {
                    case_532 = INT64_C(1);
                    break;
                }
                case INT64_C(57303459): {
                    case_532 = INT64_C(1);
                    break;
                }
                case INT64_C(1987380): {
                    case_532 = INT64_C(1);
                    break;
                }
                case INT64_C(1713595): {
                    case_532 = INT64_C(1);
                    break;
                }
                case INT64_C(2003493): {
                    case_532 = INT64_C(1);
                    break;
                }
                case INT64_C(220052207): {
                    case_532 = INT64_C(1);
                    break;
                }
                case INT64_C(219778422): {
                    case_532 = INT64_C(1);
                    break;
                }
                case INT64_C(220068320): {
                    case_532 = INT64_C(1);
                    break;
                }
                case INT64_C(260346884): {
                    case_532 = INT64_C(1);
                    break;
                }
                case INT64_C(224481049): {
                    case_532 = INT64_C(1);
                    break;
                }
                case INT64_C(262457687): {
                    case_532 = INT64_C(1);
                    break;
                }
                case INT64_C(224547597): {
                    case_532 = INT64_C(1);
                    break;
                }
                case INT64_C(260276799): {
                    case_532 = INT64_C(1);
                    break;
                }
                case INT64_C(224481180): {
                    case_532 = INT64_C(1);
                    break;
                }
                case INT64_C(826839025): {
                    case_532 = INT64_C(1);
                    break;
                }
                case INT64_C(790973190): {
                    case_532 = INT64_C(1);
                    break;
                }
                case INT64_C(828949828): {
                    case_532 = INT64_C(1);
                    break;
                }
                case INT64_C(1713596): {
                    case_532 = INT64_C(1);
                    break;
                }
                case INT64_C(1986845): {
                    case_532 = INT64_C(1);
                    break;
                }
                case INT64_C(1714103): {
                    case_532 = INT64_C(1);
                    break;
                }
                case INT64_C(219778423): {
                    case_532 = INT64_C(1);
                    break;
                }
                case INT64_C(220051672): {
                    case_532 = INT64_C(1);
                    break;
                }
                case INT64_C(219778930): {
                    case_532 = INT64_C(1);
                    break;
                }
                case INT64_C(283705086): {
                    case_532 = INT64_C(1);
                    break;
                }
                case INT64_C(224308133): {
                    case_532 = INT64_C(1);
                    break;
                }
                case INT64_C(420295318): {
                    case_532 = INT64_C(1);
                    break;
                }
                case INT64_C(824551211): {
                    case_532 = INT64_C(1);
                    break;
                }
                case INT64_C(354247632): {
                    case_532 = INT64_C(1);
                    break;
                }
                case INT64_C(230982984): {
                    case_532 = INT64_C(1);
                    break;
                }
                case INT64_C(553441405): {
                    case_532 = INT64_C(1);
                    break;
                }
                case INT64_C(138294544): {
                    case_532 = INT64_C(1);
                    break;
                }
                case INT64_C(1748297): {
                    case_532 = INT64_C(1);
                    break;
                }
                case INT64_C(229027006): {
                    case_532 = INT64_C(1);
                    break;
                }
                case INT64_C(244712290): {
                    case_532 = INT64_C(1);
                    break;
                }
                case INT64_C(260345973): {
                    case_532 = INT64_C(1);
                    break;
                }
                case INT64_C(231274442): {
                    case_532 = INT64_C(1);
                    break;
                }
                case INT64_C(1729434): {
                    case_532 = INT64_C(1);
                    break;
                }
                case INT64_C(1969161): {
                    case_532 = INT64_C(1);
                    break;
                }
                case INT64_C(992605559): {
                    case_532 = INT64_C(1);
                    break;
                }
                case INT64_C(430838469): {
                    case_532 = INT64_C(1);
                    break;
                }
                case INT64_C(184475947): {
                    case_532 = INT64_C(1);
                    break;
                }
                case INT64_C(224581785): {
                    case_532 = INT64_C(1);
                    break;
                }
                default: {
                    case_532 = INT64_C(0);
                    break;
                }
            }
            int64_t scrut_531 = case_532;
            if ((scrut_531 == INT64_C(1))) {
                struct mak__f *arena_call_534 = (struct mak__f *)mak_ir_arena_alloc(sizeof(struct mak__f), _Alignof(struct mak__f));
                if (!arena_call_534) {
                    mak_ir_recover("ADT result allocation failed");
                    arena_call_534 = &mak_ir_default_obj__f;
                 } else {
                    mak_ir_fn_16((struct mak__a *)mak_ir_adt_recover_ptr((const void *)&mak_ir_default_obj__a, "integer used as ADT"), (struct mak__f *)(arena_call_534));
                }
                struct mak__f * scrut_533 = arena_call_534;
                if ((scrut_533 != NULL && (scrut_533)->tag == mak_ir_tag_11)) {
                    *(out) = mak_ir_ctor_obj_11;
                }
                else if ((scrut_533 != NULL && (scrut_533)->tag == mak_ir_tag_10)) {
                    double mak_local_es_226 = (scrut_533)->v.v10.f0;
                    struct mak__a * mak_local_et_227 = (scrut_533)->v.v10.f1;
                    struct mak__a *arena_call_536 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
                    if (!arena_call_536) {
                        mak_ir_recover("ADT result allocation failed");
                        arena_call_536 = &mak_ir_default_obj__a;
                     } else {
                        mak_ir_fn_2(mak_local_et_227, (struct mak__a *)(arena_call_536));
                    }
                    struct mak__a * scrut_535 = arena_call_536;
                    if ((scrut_535 != NULL && (scrut_535)->tag == mak_ir_tag_1) && (scrut_535->v.v1.f0 == INT64_C(41))) {
                        struct mak__a * mak_local_eu_228 = (scrut_535)->v.v1.f1;
                        int64_t case_538 = 0;
                        switch (a0) {
                            case INT64_C(1677570): {
                                case_538 = INT64_C(1);
                                break;
                            }
                            case INT64_C(230982852): {
                                case_538 = INT64_C(1);
                                break;
                            }
                            case INT64_C(260484708): {
                                case_538 = INT64_C(1);
                                break;
                            }
                            case INT64_C(224257837): {
                                case_538 = INT64_C(1);
                                break;
                            }
                            case INT64_C(1749093): {
                                case_538 = INT64_C(1);
                                break;
                            }
                            case INT64_C(229131233): {
                                case_538 = INT64_C(1);
                                break;
                            }
                            case INT64_C(16199091): {
                                case_538 = INT64_C(1);
                                break;
                            }
                            case INT64_C(1868032): {
                                case_538 = INT64_C(1);
                                break;
                            }
                            case INT64_C(14258): {
                                case_538 = INT64_C(1);
                                break;
                            }
                            case INT64_C(244712242): {
                                case_538 = INT64_C(1);
                                break;
                            }
                            case INT64_C(57303395): {
                                case_538 = INT64_C(1);
                                break;
                            }
                            case INT64_C(57303459): {
                                case_538 = INT64_C(1);
                                break;
                            }
                            case INT64_C(1987380): {
                                case_538 = INT64_C(1);
                                break;
                            }
                            case INT64_C(1713595): {
                                case_538 = INT64_C(1);
                                break;
                            }
                            case INT64_C(2003493): {
                                case_538 = INT64_C(1);
                                break;
                            }
                            case INT64_C(220052207): {
                                case_538 = INT64_C(1);
                                break;
                            }
                            case INT64_C(219778422): {
                                case_538 = INT64_C(1);
                                break;
                            }
                            case INT64_C(220068320): {
                                case_538 = INT64_C(1);
                                break;
                            }
                            case INT64_C(260346884): {
                                case_538 = INT64_C(1);
                                break;
                            }
                            case INT64_C(224481049): {
                                case_538 = INT64_C(1);
                                break;
                            }
                            case INT64_C(262457687): {
                                case_538 = INT64_C(1);
                                break;
                            }
                            case INT64_C(224547597): {
                                case_538 = INT64_C(1);
                                break;
                            }
                            case INT64_C(260276799): {
                                case_538 = INT64_C(1);
                                break;
                            }
                            case INT64_C(224481180): {
                                case_538 = INT64_C(1);
                                break;
                            }
                            case INT64_C(826839025): {
                                case_538 = INT64_C(1);
                                break;
                            }
                            case INT64_C(790973190): {
                                case_538 = INT64_C(1);
                                break;
                            }
                            case INT64_C(828949828): {
                                case_538 = INT64_C(1);
                                break;
                            }
                            case INT64_C(1713596): {
                                case_538 = INT64_C(1);
                                break;
                            }
                            case INT64_C(1986845): {
                                case_538 = INT64_C(1);
                                break;
                            }
                            case INT64_C(1714103): {
                                case_538 = INT64_C(1);
                                break;
                            }
                            case INT64_C(219778423): {
                                case_538 = INT64_C(1);
                                break;
                            }
                            case INT64_C(220051672): {
                                case_538 = INT64_C(1);
                                break;
                            }
                            case INT64_C(219778930): {
                                case_538 = INT64_C(1);
                                break;
                            }
                            case INT64_C(283705086): {
                                case_538 = INT64_C(1);
                                break;
                            }
                            case INT64_C(224308133): {
                                case_538 = INT64_C(1);
                                break;
                            }
                            case INT64_C(420295318): {
                                case_538 = INT64_C(1);
                                break;
                            }
                            case INT64_C(824551211): {
                                case_538 = INT64_C(1);
                                break;
                            }
                            case INT64_C(354247632): {
                                case_538 = INT64_C(1);
                                break;
                            }
                            case INT64_C(230982984): {
                                case_538 = INT64_C(1);
                                break;
                            }
                            case INT64_C(553441405): {
                                case_538 = INT64_C(1);
                                break;
                            }
                            case INT64_C(138294544): {
                                case_538 = INT64_C(1);
                                break;
                            }
                            case INT64_C(1748297): {
                                case_538 = INT64_C(1);
                                break;
                            }
                            case INT64_C(229027006): {
                                case_538 = INT64_C(1);
                                break;
                            }
                            case INT64_C(244712290): {
                                case_538 = INT64_C(1);
                                break;
                            }
                            case INT64_C(260345973): {
                                case_538 = INT64_C(1);
                                break;
                            }
                            case INT64_C(231274442): {
                                case_538 = INT64_C(1);
                                break;
                            }
                            case INT64_C(1729434): {
                                case_538 = INT64_C(1);
                                break;
                            }
                            case INT64_C(1969161): {
                                case_538 = INT64_C(1);
                                break;
                            }
                            case INT64_C(992605559): {
                                case_538 = INT64_C(1);
                                break;
                            }
                            case INT64_C(430838469): {
                                case_538 = INT64_C(1);
                                break;
                            }
                            case INT64_C(184475947): {
                                case_538 = INT64_C(1);
                                break;
                            }
                            case INT64_C(224581785): {
                                case_538 = INT64_C(1);
                                break;
                            }
                            default: {
                                case_538 = INT64_C(0);
                                break;
                            }
                        }
                        int64_t scrut_537 = case_538;
                        if ((scrut_537 == INT64_C(1))) {
                            (out)->tag = mak_ir_tag_10;
                            double case_539 = 0.0;
                            switch (a0) {
                                case INT64_C(1677570): {
                                    case_539 = mak_ir_ext_7(mak_local_es_226);
                                    break;
                                }
                                case INT64_C(230982852): {
                                    case_539 = mak_ir_ext_7(mak_local_es_226);
                                    break;
                                }
                                case INT64_C(260484708): {
                                    case_539 = mak_ir_ext_5(mak_local_es_226);
                                    break;
                                }
                                case INT64_C(224257837): {
                                    case_539 = mak_ir_ext_6(mak_local_es_226);
                                    break;
                                }
                                case INT64_C(1749093): {
                                    case_539 = mak_ir_ext_15(mak_local_es_226);
                                    break;
                                }
                                case INT64_C(229131233): {
                                    case_539 = mak_ir_ext_16(mak_local_es_226);
                                    break;
                                }
                                case INT64_C(16199091): {
                                    case_539 = mak_ir_ext_17(mak_local_es_226);
                                    break;
                                }
                                case INT64_C(1868032): {
                                    case_539 = mak_ir_ext_18(mak_local_es_226);
                                    break;
                                }
                                case INT64_C(14258): {
                                    case_539 = mak_ir_ext_18(mak_local_es_226);
                                    break;
                                }
                                case INT64_C(244712242): {
                                    case_539 = mak_ir_ext_19(mak_local_es_226);
                                    break;
                                }
                                case INT64_C(57303395): {
                                    case_539 = mak_ir_ext_20(mak_local_es_226);
                                    break;
                                }
                                case INT64_C(57303459): {
                                    case_539 = mak_ir_ext_21(mak_local_es_226);
                                    break;
                                }
                                case INT64_C(1987380): {
                                    case_539 = mak_ir_ext_22(mak_local_es_226);
                                    break;
                                }
                                case INT64_C(1713595): {
                                    case_539 = mak_ir_ext_23(mak_local_es_226);
                                    break;
                                }
                                case INT64_C(2003493): {
                                    case_539 = mak_ir_ext_24(mak_local_es_226);
                                    break;
                                }
                                case INT64_C(220052207): {
                                    case_539 = mak_ir_ext_25(mak_local_es_226);
                                    break;
                                }
                                case INT64_C(219778422): {
                                    case_539 = mak_ir_ext_26(mak_local_es_226);
                                    break;
                                }
                                case INT64_C(220068320): {
                                    case_539 = mak_ir_ext_27(mak_local_es_226);
                                    break;
                                }
                                case INT64_C(260346884): {
                                    case_539 = mak_ir_ext_29(mak_local_es_226);
                                    break;
                                }
                                case INT64_C(224481049): {
                                    case_539 = mak_ir_ext_30(mak_local_es_226);
                                    break;
                                }
                                case INT64_C(262457687): {
                                    case_539 = mak_ir_ext_31(mak_local_es_226);
                                    break;
                                }
                                case INT64_C(224547597): {
                                    double eval_540 = mak_ir_ext_29(mak_local_es_226);
                                    case_539 = ((double)(1.00000000000000000) / (double)(eval_540));
                                    break;
                                }
                                case INT64_C(260276799): {
                                    double eval_541 = mak_ir_ext_30(mak_local_es_226);
                                    case_539 = ((double)(1.00000000000000000) / (double)(eval_541));
                                    break;
                                }
                                case INT64_C(224481180): {
                                    double eval_542 = mak_ir_ext_30(mak_local_es_226);
                                    double eval_543 = mak_ir_ext_29(mak_local_es_226);
                                    case_539 = ((double)(eval_542) / (double)(eval_543));
                                    break;
                                }
                                case INT64_C(826839025): {
                                    case_539 = mak_ir_ext_32(mak_local_es_226);
                                    break;
                                }
                                case INT64_C(790973190): {
                                    case_539 = mak_ir_ext_33(mak_local_es_226);
                                    break;
                                }
                                case INT64_C(828949828): {
                                    case_539 = mak_ir_ext_34(mak_local_es_226);
                                    break;
                                }
                                case INT64_C(1713596): {
                                    double eval_544 = mak_ir_ext_23(mak_local_es_226);
                                    double eval_545 = mak_ir_ext_22(mak_local_es_226);
                                    case_539 = ((double)(eval_544) / (double)(eval_545));
                                    break;
                                }
                                case INT64_C(1986845): {
                                    double eval_546 = mak_ir_ext_23(mak_local_es_226);
                                    case_539 = ((double)(1.00000000000000000) / (double)(eval_546));
                                    break;
                                }
                                case INT64_C(1714103): {
                                    double eval_547 = mak_ir_ext_22(mak_local_es_226);
                                    case_539 = ((double)(1.00000000000000000) / (double)(eval_547));
                                    break;
                                }
                                case INT64_C(219778423): {
                                    double eval_548 = mak_ir_ext_27(mak_local_es_226);
                                    case_539 = ((double)(1.57079632679489656) - (double)(eval_548));
                                    break;
                                }
                                case INT64_C(220051672): {
                                    case_539 = mak_ir_ext_26(((double)(1.00000000000000000) / (double)(mak_local_es_226)));
                                    break;
                                }
                                case INT64_C(219778930): {
                                    case_539 = mak_ir_ext_25(((double)(1.00000000000000000) / (double)(mak_local_es_226)));
                                    break;
                                }
                                case INT64_C(283705086): {
                                    case_539 = mak_ir_ext_8(mak_local_es_226);
                                    break;
                                }
                                case INT64_C(224308133): {
                                    case_539 = mak_ir_ext_9(mak_local_es_226);
                                    break;
                                }
                                case INT64_C(420295318): {
                                    case_539 = mak_ir_ext_10(mak_local_es_226);
                                    break;
                                }
                                case INT64_C(824551211): {
                                    case_539 = mak_ir_ext_11(mak_local_es_226);
                                    break;
                                }
                                case INT64_C(354247632): {
                                    case_539 = mak_ir_ext_35(((double)(mak_local_es_226) + (double)(1.00000000000000000)));
                                    break;
                                }
                                case INT64_C(230982984): {
                                    case_539 = mak_ir_ext_35(((double)(mak_local_es_226) + (double)(1.00000000000000000)));
                                    break;
                                }
                                case INT64_C(553441405): {
                                    case_539 = mak_ir_ext_35(mak_local_es_226);
                                    break;
                                }
                                case INT64_C(138294544): {
                                    case_539 = mak_ir_ext_36(mak_local_es_226);
                                    break;
                                }
                                case INT64_C(1748297): {
                                    case_539 = mak_ir_ext_38(mak_local_es_226);
                                    break;
                                }
                                case INT64_C(229027006): {
                                    case_539 = mak_ir_ext_39(mak_local_es_226);
                                    break;
                                }
                                case INT64_C(244712290): {
                                    case_539 = mak_ir_ext_40(mak_local_es_226);
                                    break;
                                }
                                case INT64_C(260345973): {
                                    double case_549 = 0.0;
                                    int32_t scrut_550 = (int32_t)(mak_local_es_226) > (int32_t)(0.00000000000000000);
                                    if ((scrut_550 == INT64_C(1))) {
                                        case_549 = 1.00000000000000000;
                                    }
                                    else if (1) {
                                        double case_551 = 0.0;
                                        int32_t scrut_552 = (int32_t)(mak_local_es_226) < (int32_t)(0.00000000000000000);
                                        if ((scrut_552 == INT64_C(1))) {
                                            case_551 = -1.00000000000000000;
                                        }
                                        else if (1) {
                                            case_551 = 0.00000000000000000;
                                        }
                                        case_549 = case_551;
                                    }
                                    case_539 = case_549;
                                    break;
                                }
                                case INT64_C(231274442): {
                                    double eval_553 = mak_ir_ext_10(mak_local_es_226);
                                    case_539 = ((double)(mak_local_es_226) - (double)(eval_553));
                                    break;
                                }
                                case INT64_C(1729434): {
                                    case_539 = ((double)(mak_local_es_226) * (double)(57.29577951308232286));
                                    break;
                                }
                                case INT64_C(1969161): {
                                    case_539 = ((double)(mak_local_es_226) * (double)(0.01745329251994330));
                                    break;
                                }
                                case INT64_C(992605559): {
                                    case_539 = ((double)(mak_local_es_226) * (double)(0.01745329251994330));
                                    break;
                                }
                                case INT64_C(430838469): {
                                    case_539 = ((double)(mak_local_es_226) * (double)(57.29577951308232286));
                                    break;
                                }
                                case INT64_C(184475947): {
                                    case_539 = ((double)(mak_local_es_226) * (double)(mak_local_es_226));
                                    break;
                                }
                                case INT64_C(224581785): {
                                    double eval_554 = ((double)(mak_local_es_226) * (double)(mak_local_es_226));
                                    case_539 = ((double)(eval_554) * (double)(mak_local_es_226));
                                    break;
                                }
                                default: {
                                    case_539 = 0.00000000000000000;
                                    break;
                                }
                            }
                            (out)->v.v10.f0 = case_539;
                            (out)->v.v10.f1 = mak_local_eu_228;
                        }
                        else if (1) {
                            *(out) = mak_ir_ctor_obj_11;
                        }
                    }
                    else if (1) {
                        *(out) = mak_ir_ctor_obj_11;
                    }
                }
                else {
                    *(out) = *(&mak_ir_default_obj__f);
                }
            }
            else if (1) {
                *(out) = mak_ir_ctor_obj_11;
            }
        }
    }
    return;
}

static inline void mak_ir_fn_25(int64_t a0, struct mak__a * a1, struct mak__f *out) {
    struct mak__f *arena_call_556 = (struct mak__f *)mak_ir_arena_alloc(sizeof(struct mak__f), _Alignof(struct mak__f));
    if (!arena_call_556) {
        mak_ir_recover("ADT result allocation failed");
        arena_call_556 = &mak_ir_default_obj__f;
     } else {
        mak_ir_fn_16(a1, (struct mak__f *)(arena_call_556));
    }
    struct mak__f * scrut_555 = arena_call_556;
    if ((scrut_555 != NULL && (scrut_555)->tag == mak_ir_tag_11)) {
        *(out) = mak_ir_ctor_obj_11;
    }
    else if ((scrut_555 != NULL && (scrut_555)->tag == mak_ir_tag_10)) {
        double mak_local_ff_229 = (scrut_555)->v.v10.f0;
        struct mak__a * mak_local_fg_230 = (scrut_555)->v.v10.f1;
        struct mak__a *arena_call_558 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
        if (!arena_call_558) {
            mak_ir_recover("ADT result allocation failed");
            arena_call_558 = &mak_ir_default_obj__a;
         } else {
            mak_ir_fn_2(mak_local_fg_230, (struct mak__a *)(arena_call_558));
        }
        struct mak__a * scrut_557 = arena_call_558;
        if ((scrut_557 != NULL && (scrut_557)->tag == mak_ir_tag_1) && (scrut_557->v.v1.f0 == INT64_C(44))) {
            struct mak__a * mak_local_fh_231 = (scrut_557)->v.v1.f1;
            struct mak__f *arena_call_560 = (struct mak__f *)mak_ir_arena_alloc(sizeof(struct mak__f), _Alignof(struct mak__f));
            if (!arena_call_560) {
                mak_ir_recover("ADT result allocation failed");
                arena_call_560 = &mak_ir_default_obj__f;
             } else {
                mak_ir_fn_16(mak_local_fh_231, (struct mak__f *)(arena_call_560));
            }
            struct mak__f * scrut_559 = arena_call_560;
            if ((scrut_559 != NULL && (scrut_559)->tag == mak_ir_tag_11)) {
                *(out) = mak_ir_ctor_obj_11;
            }
            else if ((scrut_559 != NULL && (scrut_559)->tag == mak_ir_tag_10)) {
                double mak_local_fi_232 = (scrut_559)->v.v10.f0;
                struct mak__a * mak_local_fj_233 = (scrut_559)->v.v10.f1;
                struct mak__a *arena_call_562 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
                if (!arena_call_562) {
                    mak_ir_recover("ADT result allocation failed");
                    arena_call_562 = &mak_ir_default_obj__a;
                 } else {
                    mak_ir_fn_2(mak_local_fj_233, (struct mak__a *)(arena_call_562));
                }
                struct mak__a * scrut_561 = arena_call_562;
                if ((scrut_561 != NULL && (scrut_561)->tag == mak_ir_tag_1) && (scrut_561->v.v1.f0 == INT64_C(44))) {
                    struct mak__a * mak_local_fk_234 = (scrut_561)->v.v1.f1;
                    struct mak__f *arena_call_564 = (struct mak__f *)mak_ir_arena_alloc(sizeof(struct mak__f), _Alignof(struct mak__f));
                    if (!arena_call_564) {
                        mak_ir_recover("ADT result allocation failed");
                        arena_call_564 = &mak_ir_default_obj__f;
                     } else {
                        mak_ir_fn_16(mak_local_fk_234, (struct mak__f *)(arena_call_564));
                    }
                    struct mak__f * scrut_563 = arena_call_564;
                    if ((scrut_563 != NULL && (scrut_563)->tag == mak_ir_tag_11)) {
                        *(out) = mak_ir_ctor_obj_11;
                    }
                    else if ((scrut_563 != NULL && (scrut_563)->tag == mak_ir_tag_10)) {
                        double mak_local_fl_235 = (scrut_563)->v.v10.f0;
                        struct mak__a * mak_local_fm_236 = (scrut_563)->v.v10.f1;
                        struct mak__a *arena_call_566 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
                        if (!arena_call_566) {
                            mak_ir_recover("ADT result allocation failed");
                            arena_call_566 = &mak_ir_default_obj__a;
                         } else {
                            mak_ir_fn_2(mak_local_fm_236, (struct mak__a *)(arena_call_566));
                        }
                        struct mak__a * scrut_565 = arena_call_566;
                        if ((scrut_565 != NULL && (scrut_565)->tag == mak_ir_tag_1) && (scrut_565->v.v1.f0 == INT64_C(41))) {
                            struct mak__a * mak_local_fo_237 = (scrut_565)->v.v1.f1;
                            int64_t case_568 = 0;
                            switch (a0) {
                                case INT64_C(399964812): {
                                    case_568 = INT64_C(1);
                                    break;
                                }
                                case INT64_C(1764798): {
                                    case_568 = INT64_C(1);
                                    break;
                                }
                                default: {
                                    case_568 = INT64_C(0);
                                    break;
                                }
                            }
                            int64_t scrut_567 = case_568;
                            if ((scrut_567 == INT64_C(1))) {
                                (out)->tag = mak_ir_tag_10;
                                double case_569 = 0.0;
                                switch (a0) {
                                    case INT64_C(399964812): {
                                        double case_570 = 0.0;
                                        int32_t scrut_571 = (int32_t)(mak_local_ff_229) < (int32_t)(mak_local_fi_232);
                                        if ((scrut_571 == INT64_C(1))) {
                                            case_570 = mak_local_fi_232;
                                        }
                                        else if (1) {
                                            double case_572 = 0.0;
                                            int32_t scrut_573 = (int32_t)(mak_local_ff_229) > (int32_t)(mak_local_fl_235);
                                            if ((scrut_573 == INT64_C(1))) {
                                                case_572 = mak_local_fl_235;
                                            }
                                            else if (1) {
                                                case_572 = mak_local_ff_229;
                                            }
                                            case_570 = case_572;
                                        }
                                        case_569 = case_570;
                                        break;
                                    }
                                    case INT64_C(1764798): {
                                        case_569 = mak_ir_ext_45(mak_local_ff_229, mak_local_fi_232, mak_local_fl_235);
                                        break;
                                    }
                                    default: {
                                        case_569 = mak_local_ff_229;
                                        break;
                                    }
                                }
                                (out)->v.v10.f0 = case_569;
                                (out)->v.v10.f1 = mak_local_fo_237;
                            }
                            else if (1) {
                                *(out) = mak_ir_ctor_obj_11;
                            }
                        }
                        else if (1) {
                            *(out) = mak_ir_ctor_obj_11;
                        }
                    }
                    else {
                        *(out) = *(&mak_ir_default_obj__f);
                    }
                }
                else if (1) {
                    *(out) = mak_ir_ctor_obj_11;
                }
            }
            else {
                *(out) = *(&mak_ir_default_obj__f);
            }
        }
        else if (1) {
            *(out) = mak_ir_ctor_obj_11;
        }
    }
    else {
        *(out) = *(&mak_ir_default_obj__f);
    }
    return;
}

static inline double mak_ir_fn_26(int64_t a0, int64_t a1, int64_t a2, int64_t a3) {
    switch (a0) {
        case INT64_C(399964812): {
            int32_t scrut_574 = (int32_t)(a1) < (int32_t)(a2);
            if ((scrut_574 == INT64_C(1))) {
                return a2;
            }
            else if (1) {
                int32_t scrut_575 = (int32_t)(a1) > (int32_t)(a3);
                if ((scrut_575 == INT64_C(1))) {
                    return a3;
                }
                else if (1) {
                    return a1;
                }
            }
        }
        case INT64_C(1764798): {
            return mak_ir_ext_45(a1, a2, a3);
        }
        default: {
            return a1;
        }
    }
}

static inline void mak_ir_fn_27(int64_t a0, struct mak__a * a1, struct mak__f *out) {
    struct mak__f *arena_call_577 = (struct mak__f *)mak_ir_arena_alloc(sizeof(struct mak__f), _Alignof(struct mak__f));
    if (!arena_call_577) {
        mak_ir_recover("ADT result allocation failed");
        arena_call_577 = &mak_ir_default_obj__f;
     } else {
        mak_ir_fn_16(a1, (struct mak__f *)(arena_call_577));
    }
    struct mak__f * scrut_576 = arena_call_577;
    if ((scrut_576 != NULL && (scrut_576)->tag == mak_ir_tag_11)) {
        *(out) = mak_ir_ctor_obj_11;
    }
    else if ((scrut_576 != NULL && (scrut_576)->tag == mak_ir_tag_10)) {
        double mak_local_ex_238 = (scrut_576)->v.v10.f0;
        struct mak__a * mak_local_ey_239 = (scrut_576)->v.v10.f1;
        struct mak__a *arena_call_579 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
        if (!arena_call_579) {
            mak_ir_recover("ADT result allocation failed");
            arena_call_579 = &mak_ir_default_obj__a;
         } else {
            mak_ir_fn_2(mak_local_ey_239, (struct mak__a *)(arena_call_579));
        }
        struct mak__a * scrut_578 = arena_call_579;
        if ((scrut_578 != NULL && (scrut_578)->tag == mak_ir_tag_1) && (scrut_578->v.v1.f0 == INT64_C(44))) {
            struct mak__a * mak_local_ez_240 = (scrut_578)->v.v1.f1;
            struct mak__f *arena_call_581 = (struct mak__f *)mak_ir_arena_alloc(sizeof(struct mak__f), _Alignof(struct mak__f));
            if (!arena_call_581) {
                mak_ir_recover("ADT result allocation failed");
                arena_call_581 = &mak_ir_default_obj__f;
             } else {
                mak_ir_fn_16(mak_local_ez_240, (struct mak__f *)(arena_call_581));
            }
            struct mak__f * scrut_580 = arena_call_581;
            if ((scrut_580 != NULL && (scrut_580)->tag == mak_ir_tag_11)) {
                *(out) = mak_ir_ctor_obj_11;
            }
            else if ((scrut_580 != NULL && (scrut_580)->tag == mak_ir_tag_10)) {
                double mak_local_fa_241 = (scrut_580)->v.v10.f0;
                struct mak__a * mak_local_fb_242 = (scrut_580)->v.v10.f1;
                struct mak__a *arena_call_583 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
                if (!arena_call_583) {
                    mak_ir_recover("ADT result allocation failed");
                    arena_call_583 = &mak_ir_default_obj__a;
                 } else {
                    mak_ir_fn_2(mak_local_fb_242, (struct mak__a *)(arena_call_583));
                }
                struct mak__a * scrut_582 = arena_call_583;
                if ((scrut_582 != NULL && (scrut_582)->tag == mak_ir_tag_1) && (scrut_582->v.v1.f0 == INT64_C(41))) {
                    struct mak__a * mak_local_fc_243 = (scrut_582)->v.v1.f1;
                    int64_t case_585 = 0;
                    switch (a0) {
                        case INT64_C(828949774): {
                            case_585 = INT64_C(1);
                            break;
                        }
                        case INT64_C(231190472): {
                            case_585 = INT64_C(1);
                            break;
                        }
                        case INT64_C(1885190): {
                            case_585 = INT64_C(1);
                            break;
                        }
                        case INT64_C(1969694): {
                            case_585 = INT64_C(1);
                            break;
                        }
                        case INT64_C(510764621): {
                            case_585 = INT64_C(1);
                            break;
                        }
                        case INT64_C(1936692): {
                            case_585 = INT64_C(1);
                            break;
                        }
                        case INT64_C(258201902): {
                            case_585 = INT64_C(1);
                            break;
                        }
                        case INT64_C(548941060): {
                            case_585 = INT64_C(1);
                            break;
                        }
                        case INT64_C(901947274): {
                            case_585 = INT64_C(1);
                            break;
                        }
                        case INT64_C(1780652): {
                            case_585 = INT64_C(1);
                            break;
                        }
                        case INT64_C(1866466): {
                            case_585 = INT64_C(1);
                            break;
                        }
                        case INT64_C(1900793): {
                            case_585 = INT64_C(1);
                            break;
                        }
                        case INT64_C(224480257): {
                            case_585 = INT64_C(1);
                            break;
                        }
                        case INT64_C(1902496): {
                            case_585 = INT64_C(1);
                            break;
                        }
                        case INT64_C(253534496): {
                            case_585 = INT64_C(1);
                            break;
                        }
                        case INT64_C(1884414): {
                            case_585 = INT64_C(1);
                            break;
                        }
                        case INT64_C(1883376): {
                            case_585 = INT64_C(1);
                            break;
                        }
                        case INT64_C(262106026): {
                            case_585 = INT64_C(1);
                            break;
                        }
                        case INT64_C(231035246): {
                            case_585 = INT64_C(1);
                            break;
                        }
                        case INT64_C(231188658): {
                            case_585 = INT64_C(1);
                            break;
                        }
                        case INT64_C(231189696): {
                            case_585 = INT64_C(1);
                            break;
                        }
                        case INT64_C(743972013): {
                            case_585 = INT64_C(1);
                            break;
                        }
                        default: {
                            case_585 = INT64_C(0);
                            break;
                        }
                    }
                    int64_t scrut_584 = case_585;
                    if ((scrut_584 == INT64_C(1))) {
                        (out)->tag = mak_ir_tag_10;
                        (out)->v.v10.f0 = mak_ir_fn_9(a0, mak_local_ex_238, mak_local_fa_241);
                        (out)->v.v10.f1 = mak_local_fc_243;
                    }
                    else if (1) {
                        *(out) = mak_ir_ctor_obj_11;
                    }
                }
                else if (1) {
                    *(out) = mak_ir_ctor_obj_11;
                }
            }
            else {
                *(out) = *(&mak_ir_default_obj__f);
            }
        }
        else if (1) {
            *(out) = mak_ir_ctor_obj_11;
        }
    }
    else {
        *(out) = *(&mak_ir_default_obj__f);
    }
    return;
}

static inline void mak_ir_fn_28(int64_t a0, struct mak__a * a1, struct mak__f *out) {
    struct mak__f *arena_call_587 = (struct mak__f *)mak_ir_arena_alloc(sizeof(struct mak__f), _Alignof(struct mak__f));
    if (!arena_call_587) {
        mak_ir_recover("ADT result allocation failed");
        arena_call_587 = &mak_ir_default_obj__f;
     } else {
        mak_ir_fn_16(a1, (struct mak__f *)(arena_call_587));
    }
    struct mak__f * scrut_586 = arena_call_587;
    if ((scrut_586 != NULL && (scrut_586)->tag == mak_ir_tag_11)) {
        *(out) = mak_ir_ctor_obj_11;
    }
    else if ((scrut_586 != NULL && (scrut_586)->tag == mak_ir_tag_10)) {
        double mak_local_es_244 = (scrut_586)->v.v10.f0;
        struct mak__a * mak_local_et_245 = (scrut_586)->v.v10.f1;
        struct mak__a *arena_call_589 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
        if (!arena_call_589) {
            mak_ir_recover("ADT result allocation failed");
            arena_call_589 = &mak_ir_default_obj__a;
         } else {
            mak_ir_fn_2(mak_local_et_245, (struct mak__a *)(arena_call_589));
        }
        struct mak__a * scrut_588 = arena_call_589;
        if ((scrut_588 != NULL && (scrut_588)->tag == mak_ir_tag_1) && (scrut_588->v.v1.f0 == INT64_C(41))) {
            struct mak__a * mak_local_eu_246 = (scrut_588)->v.v1.f1;
            int64_t case_591 = 0;
            switch (a0) {
                case INT64_C(1677570): {
                    case_591 = INT64_C(1);
                    break;
                }
                case INT64_C(230982852): {
                    case_591 = INT64_C(1);
                    break;
                }
                case INT64_C(260484708): {
                    case_591 = INT64_C(1);
                    break;
                }
                case INT64_C(224257837): {
                    case_591 = INT64_C(1);
                    break;
                }
                case INT64_C(1749093): {
                    case_591 = INT64_C(1);
                    break;
                }
                case INT64_C(229131233): {
                    case_591 = INT64_C(1);
                    break;
                }
                case INT64_C(16199091): {
                    case_591 = INT64_C(1);
                    break;
                }
                case INT64_C(1868032): {
                    case_591 = INT64_C(1);
                    break;
                }
                case INT64_C(14258): {
                    case_591 = INT64_C(1);
                    break;
                }
                case INT64_C(244712242): {
                    case_591 = INT64_C(1);
                    break;
                }
                case INT64_C(57303395): {
                    case_591 = INT64_C(1);
                    break;
                }
                case INT64_C(57303459): {
                    case_591 = INT64_C(1);
                    break;
                }
                case INT64_C(1987380): {
                    case_591 = INT64_C(1);
                    break;
                }
                case INT64_C(1713595): {
                    case_591 = INT64_C(1);
                    break;
                }
                case INT64_C(2003493): {
                    case_591 = INT64_C(1);
                    break;
                }
                case INT64_C(220052207): {
                    case_591 = INT64_C(1);
                    break;
                }
                case INT64_C(219778422): {
                    case_591 = INT64_C(1);
                    break;
                }
                case INT64_C(220068320): {
                    case_591 = INT64_C(1);
                    break;
                }
                case INT64_C(260346884): {
                    case_591 = INT64_C(1);
                    break;
                }
                case INT64_C(224481049): {
                    case_591 = INT64_C(1);
                    break;
                }
                case INT64_C(262457687): {
                    case_591 = INT64_C(1);
                    break;
                }
                case INT64_C(224547597): {
                    case_591 = INT64_C(1);
                    break;
                }
                case INT64_C(260276799): {
                    case_591 = INT64_C(1);
                    break;
                }
                case INT64_C(224481180): {
                    case_591 = INT64_C(1);
                    break;
                }
                case INT64_C(826839025): {
                    case_591 = INT64_C(1);
                    break;
                }
                case INT64_C(790973190): {
                    case_591 = INT64_C(1);
                    break;
                }
                case INT64_C(828949828): {
                    case_591 = INT64_C(1);
                    break;
                }
                case INT64_C(1713596): {
                    case_591 = INT64_C(1);
                    break;
                }
                case INT64_C(1986845): {
                    case_591 = INT64_C(1);
                    break;
                }
                case INT64_C(1714103): {
                    case_591 = INT64_C(1);
                    break;
                }
                case INT64_C(219778423): {
                    case_591 = INT64_C(1);
                    break;
                }
                case INT64_C(220051672): {
                    case_591 = INT64_C(1);
                    break;
                }
                case INT64_C(219778930): {
                    case_591 = INT64_C(1);
                    break;
                }
                case INT64_C(283705086): {
                    case_591 = INT64_C(1);
                    break;
                }
                case INT64_C(224308133): {
                    case_591 = INT64_C(1);
                    break;
                }
                case INT64_C(420295318): {
                    case_591 = INT64_C(1);
                    break;
                }
                case INT64_C(824551211): {
                    case_591 = INT64_C(1);
                    break;
                }
                case INT64_C(354247632): {
                    case_591 = INT64_C(1);
                    break;
                }
                case INT64_C(230982984): {
                    case_591 = INT64_C(1);
                    break;
                }
                case INT64_C(553441405): {
                    case_591 = INT64_C(1);
                    break;
                }
                case INT64_C(138294544): {
                    case_591 = INT64_C(1);
                    break;
                }
                case INT64_C(1748297): {
                    case_591 = INT64_C(1);
                    break;
                }
                case INT64_C(229027006): {
                    case_591 = INT64_C(1);
                    break;
                }
                case INT64_C(244712290): {
                    case_591 = INT64_C(1);
                    break;
                }
                case INT64_C(260345973): {
                    case_591 = INT64_C(1);
                    break;
                }
                case INT64_C(231274442): {
                    case_591 = INT64_C(1);
                    break;
                }
                case INT64_C(1729434): {
                    case_591 = INT64_C(1);
                    break;
                }
                case INT64_C(1969161): {
                    case_591 = INT64_C(1);
                    break;
                }
                case INT64_C(992605559): {
                    case_591 = INT64_C(1);
                    break;
                }
                case INT64_C(430838469): {
                    case_591 = INT64_C(1);
                    break;
                }
                case INT64_C(184475947): {
                    case_591 = INT64_C(1);
                    break;
                }
                case INT64_C(224581785): {
                    case_591 = INT64_C(1);
                    break;
                }
                default: {
                    case_591 = INT64_C(0);
                    break;
                }
            }
            int64_t scrut_590 = case_591;
            if ((scrut_590 == INT64_C(1))) {
                (out)->tag = mak_ir_tag_10;
                double case_592 = 0.0;
                switch (a0) {
                    case INT64_C(1677570): {
                        case_592 = mak_ir_ext_7(mak_local_es_244);
                        break;
                    }
                    case INT64_C(230982852): {
                        case_592 = mak_ir_ext_7(mak_local_es_244);
                        break;
                    }
                    case INT64_C(260484708): {
                        case_592 = mak_ir_ext_5(mak_local_es_244);
                        break;
                    }
                    case INT64_C(224257837): {
                        case_592 = mak_ir_ext_6(mak_local_es_244);
                        break;
                    }
                    case INT64_C(1749093): {
                        case_592 = mak_ir_ext_15(mak_local_es_244);
                        break;
                    }
                    case INT64_C(229131233): {
                        case_592 = mak_ir_ext_16(mak_local_es_244);
                        break;
                    }
                    case INT64_C(16199091): {
                        case_592 = mak_ir_ext_17(mak_local_es_244);
                        break;
                    }
                    case INT64_C(1868032): {
                        case_592 = mak_ir_ext_18(mak_local_es_244);
                        break;
                    }
                    case INT64_C(14258): {
                        case_592 = mak_ir_ext_18(mak_local_es_244);
                        break;
                    }
                    case INT64_C(244712242): {
                        case_592 = mak_ir_ext_19(mak_local_es_244);
                        break;
                    }
                    case INT64_C(57303395): {
                        case_592 = mak_ir_ext_20(mak_local_es_244);
                        break;
                    }
                    case INT64_C(57303459): {
                        case_592 = mak_ir_ext_21(mak_local_es_244);
                        break;
                    }
                    case INT64_C(1987380): {
                        case_592 = mak_ir_ext_22(mak_local_es_244);
                        break;
                    }
                    case INT64_C(1713595): {
                        case_592 = mak_ir_ext_23(mak_local_es_244);
                        break;
                    }
                    case INT64_C(2003493): {
                        case_592 = mak_ir_ext_24(mak_local_es_244);
                        break;
                    }
                    case INT64_C(220052207): {
                        case_592 = mak_ir_ext_25(mak_local_es_244);
                        break;
                    }
                    case INT64_C(219778422): {
                        case_592 = mak_ir_ext_26(mak_local_es_244);
                        break;
                    }
                    case INT64_C(220068320): {
                        case_592 = mak_ir_ext_27(mak_local_es_244);
                        break;
                    }
                    case INT64_C(260346884): {
                        case_592 = mak_ir_ext_29(mak_local_es_244);
                        break;
                    }
                    case INT64_C(224481049): {
                        case_592 = mak_ir_ext_30(mak_local_es_244);
                        break;
                    }
                    case INT64_C(262457687): {
                        case_592 = mak_ir_ext_31(mak_local_es_244);
                        break;
                    }
                    case INT64_C(224547597): {
                        double eval_593 = mak_ir_ext_29(mak_local_es_244);
                        case_592 = ((double)(1.00000000000000000) / (double)(eval_593));
                        break;
                    }
                    case INT64_C(260276799): {
                        double eval_594 = mak_ir_ext_30(mak_local_es_244);
                        case_592 = ((double)(1.00000000000000000) / (double)(eval_594));
                        break;
                    }
                    case INT64_C(224481180): {
                        double eval_595 = mak_ir_ext_30(mak_local_es_244);
                        double eval_596 = mak_ir_ext_29(mak_local_es_244);
                        case_592 = ((double)(eval_595) / (double)(eval_596));
                        break;
                    }
                    case INT64_C(826839025): {
                        case_592 = mak_ir_ext_32(mak_local_es_244);
                        break;
                    }
                    case INT64_C(790973190): {
                        case_592 = mak_ir_ext_33(mak_local_es_244);
                        break;
                    }
                    case INT64_C(828949828): {
                        case_592 = mak_ir_ext_34(mak_local_es_244);
                        break;
                    }
                    case INT64_C(1713596): {
                        double eval_597 = mak_ir_ext_23(mak_local_es_244);
                        double eval_598 = mak_ir_ext_22(mak_local_es_244);
                        case_592 = ((double)(eval_597) / (double)(eval_598));
                        break;
                    }
                    case INT64_C(1986845): {
                        double eval_599 = mak_ir_ext_23(mak_local_es_244);
                        case_592 = ((double)(1.00000000000000000) / (double)(eval_599));
                        break;
                    }
                    case INT64_C(1714103): {
                        double eval_600 = mak_ir_ext_22(mak_local_es_244);
                        case_592 = ((double)(1.00000000000000000) / (double)(eval_600));
                        break;
                    }
                    case INT64_C(219778423): {
                        double eval_601 = mak_ir_ext_27(mak_local_es_244);
                        case_592 = ((double)(1.57079632679489656) - (double)(eval_601));
                        break;
                    }
                    case INT64_C(220051672): {
                        case_592 = mak_ir_ext_26(((double)(1.00000000000000000) / (double)(mak_local_es_244)));
                        break;
                    }
                    case INT64_C(219778930): {
                        case_592 = mak_ir_ext_25(((double)(1.00000000000000000) / (double)(mak_local_es_244)));
                        break;
                    }
                    case INT64_C(283705086): {
                        case_592 = mak_ir_ext_8(mak_local_es_244);
                        break;
                    }
                    case INT64_C(224308133): {
                        case_592 = mak_ir_ext_9(mak_local_es_244);
                        break;
                    }
                    case INT64_C(420295318): {
                        case_592 = mak_ir_ext_10(mak_local_es_244);
                        break;
                    }
                    case INT64_C(824551211): {
                        case_592 = mak_ir_ext_11(mak_local_es_244);
                        break;
                    }
                    case INT64_C(354247632): {
                        case_592 = mak_ir_ext_35(((double)(mak_local_es_244) + (double)(1.00000000000000000)));
                        break;
                    }
                    case INT64_C(230982984): {
                        case_592 = mak_ir_ext_35(((double)(mak_local_es_244) + (double)(1.00000000000000000)));
                        break;
                    }
                    case INT64_C(553441405): {
                        case_592 = mak_ir_ext_35(mak_local_es_244);
                        break;
                    }
                    case INT64_C(138294544): {
                        case_592 = mak_ir_ext_36(mak_local_es_244);
                        break;
                    }
                    case INT64_C(1748297): {
                        case_592 = mak_ir_ext_38(mak_local_es_244);
                        break;
                    }
                    case INT64_C(229027006): {
                        case_592 = mak_ir_ext_39(mak_local_es_244);
                        break;
                    }
                    case INT64_C(244712290): {
                        case_592 = mak_ir_ext_40(mak_local_es_244);
                        break;
                    }
                    case INT64_C(260345973): {
                        double case_602 = 0.0;
                        int32_t scrut_603 = (int32_t)(mak_local_es_244) > (int32_t)(0.00000000000000000);
                        if ((scrut_603 == INT64_C(1))) {
                            case_602 = 1.00000000000000000;
                        }
                        else if (1) {
                            double case_604 = 0.0;
                            int32_t scrut_605 = (int32_t)(mak_local_es_244) < (int32_t)(0.00000000000000000);
                            if ((scrut_605 == INT64_C(1))) {
                                case_604 = -1.00000000000000000;
                            }
                            else if (1) {
                                case_604 = 0.00000000000000000;
                            }
                            case_602 = case_604;
                        }
                        case_592 = case_602;
                        break;
                    }
                    case INT64_C(231274442): {
                        double eval_606 = mak_ir_ext_10(mak_local_es_244);
                        case_592 = ((double)(mak_local_es_244) - (double)(eval_606));
                        break;
                    }
                    case INT64_C(1729434): {
                        case_592 = ((double)(mak_local_es_244) * (double)(57.29577951308232286));
                        break;
                    }
                    case INT64_C(1969161): {
                        case_592 = ((double)(mak_local_es_244) * (double)(0.01745329251994330));
                        break;
                    }
                    case INT64_C(992605559): {
                        case_592 = ((double)(mak_local_es_244) * (double)(0.01745329251994330));
                        break;
                    }
                    case INT64_C(430838469): {
                        case_592 = ((double)(mak_local_es_244) * (double)(57.29577951308232286));
                        break;
                    }
                    case INT64_C(184475947): {
                        case_592 = ((double)(mak_local_es_244) * (double)(mak_local_es_244));
                        break;
                    }
                    case INT64_C(224581785): {
                        double eval_607 = ((double)(mak_local_es_244) * (double)(mak_local_es_244));
                        case_592 = ((double)(eval_607) * (double)(mak_local_es_244));
                        break;
                    }
                    default: {
                        case_592 = 0.00000000000000000;
                        break;
                    }
                }
                (out)->v.v10.f0 = case_592;
                (out)->v.v10.f1 = mak_local_eu_246;
            }
            else if (1) {
                *(out) = mak_ir_ctor_obj_11;
            }
        }
        else if (1) {
            *(out) = mak_ir_ctor_obj_11;
        }
    }
    else {
        *(out) = *(&mak_ir_default_obj__f);
    }
    return;
}

static inline double mak_ir_fn_29(int64_t a0, int64_t a1) {
    switch (a0) {
        case INT64_C(1677570): {
            return mak_ir_ext_7(a1);
        }
        case INT64_C(230982852): {
            return mak_ir_ext_7(a1);
        }
        case INT64_C(260484708): {
            return mak_ir_ext_5(a1);
        }
        case INT64_C(224257837): {
            return mak_ir_ext_6(a1);
        }
        case INT64_C(1749093): {
            return mak_ir_ext_15(a1);
        }
        case INT64_C(229131233): {
            return mak_ir_ext_16(a1);
        }
        case INT64_C(16199091): {
            return mak_ir_ext_17(a1);
        }
        case INT64_C(1868032): {
            return mak_ir_ext_18(a1);
        }
        case INT64_C(14258): {
            return mak_ir_ext_18(a1);
        }
        case INT64_C(244712242): {
            return mak_ir_ext_19(a1);
        }
        case INT64_C(57303395): {
            return mak_ir_ext_20(a1);
        }
        case INT64_C(57303459): {
            return mak_ir_ext_21(a1);
        }
        case INT64_C(1987380): {
            return mak_ir_ext_22(a1);
        }
        case INT64_C(1713595): {
            return mak_ir_ext_23(a1);
        }
        case INT64_C(2003493): {
            return mak_ir_ext_24(a1);
        }
        case INT64_C(220052207): {
            return mak_ir_ext_25(a1);
        }
        case INT64_C(219778422): {
            return mak_ir_ext_26(a1);
        }
        case INT64_C(220068320): {
            return mak_ir_ext_27(a1);
        }
        case INT64_C(260346884): {
            return mak_ir_ext_29(a1);
        }
        case INT64_C(224481049): {
            return mak_ir_ext_30(a1);
        }
        case INT64_C(262457687): {
            return mak_ir_ext_31(a1);
        }
        case INT64_C(224547597): {
            double eval_608 = mak_ir_ext_29(a1);
            return ((double)(1.00000000000000000) / (double)(eval_608));
        }
        case INT64_C(260276799): {
            double eval_609 = mak_ir_ext_30(a1);
            return ((double)(1.00000000000000000) / (double)(eval_609));
        }
        case INT64_C(224481180): {
            double eval_610 = mak_ir_ext_30(a1);
            double eval_611 = mak_ir_ext_29(a1);
            return ((double)(eval_610) / (double)(eval_611));
        }
        case INT64_C(826839025): {
            return mak_ir_ext_32(a1);
        }
        case INT64_C(790973190): {
            return mak_ir_ext_33(a1);
        }
        case INT64_C(828949828): {
            return mak_ir_ext_34(a1);
        }
        case INT64_C(1713596): {
            double eval_612 = mak_ir_ext_23(a1);
            double eval_613 = mak_ir_ext_22(a1);
            return ((double)(eval_612) / (double)(eval_613));
        }
        case INT64_C(1986845): {
            double eval_614 = mak_ir_ext_23(a1);
            return ((double)(1.00000000000000000) / (double)(eval_614));
        }
        case INT64_C(1714103): {
            double eval_615 = mak_ir_ext_22(a1);
            return ((double)(1.00000000000000000) / (double)(eval_615));
        }
        case INT64_C(219778423): {
            double eval_616 = mak_ir_ext_27(a1);
            return ((double)(1.57079632679489656) - (double)(eval_616));
        }
        case INT64_C(220051672): {
            return mak_ir_ext_26(((double)(1.00000000000000000) / (double)(a1)));
        }
        case INT64_C(219778930): {
            return mak_ir_ext_25(((double)(1.00000000000000000) / (double)(a1)));
        }
        case INT64_C(283705086): {
            return mak_ir_ext_8(a1);
        }
        case INT64_C(224308133): {
            return mak_ir_ext_9(a1);
        }
        case INT64_C(420295318): {
            return mak_ir_ext_10(a1);
        }
        case INT64_C(824551211): {
            return mak_ir_ext_11(a1);
        }
        case INT64_C(354247632): {
            return mak_ir_ext_35(((double)(a1) + (double)(1.00000000000000000)));
        }
        case INT64_C(230982984): {
            return mak_ir_ext_35(((double)(a1) + (double)(1.00000000000000000)));
        }
        case INT64_C(553441405): {
            return mak_ir_ext_35(a1);
        }
        case INT64_C(138294544): {
            return mak_ir_ext_36(a1);
        }
        case INT64_C(1748297): {
            return mak_ir_ext_38(a1);
        }
        case INT64_C(229027006): {
            return mak_ir_ext_39(a1);
        }
        case INT64_C(244712290): {
            return mak_ir_ext_40(a1);
        }
        case INT64_C(260345973): {
            int32_t scrut_617 = (int32_t)(a1) > (int32_t)(0.00000000000000000);
            if ((scrut_617 == INT64_C(1))) {
                return 1.00000000000000000;
            }
            else if (1) {
                int32_t scrut_618 = (int32_t)(a1) < (int32_t)(0.00000000000000000);
                if ((scrut_618 == INT64_C(1))) {
                    return -1.00000000000000000;
                }
                else if (1) {
                    return 0.00000000000000000;
                }
            }
        }
        case INT64_C(231274442): {
            double eval_619 = mak_ir_ext_10(a1);
            return ((double)(a1) - (double)(eval_619));
        }
        case INT64_C(1729434): {
            return ((double)(a1) * (double)(57.29577951308232286));
        }
        case INT64_C(1969161): {
            return ((double)(a1) * (double)(0.01745329251994330));
        }
        case INT64_C(992605559): {
            return ((double)(a1) * (double)(0.01745329251994330));
        }
        case INT64_C(430838469): {
            return ((double)(a1) * (double)(57.29577951308232286));
        }
        case INT64_C(184475947): {
            return mak_ir_i64_mul_wrap(a1, a1);
        }
        case INT64_C(224581785): {
            int64_t eval_620 = mak_ir_i64_mul_wrap(a1, a1);
            return mak_ir_i64_mul_wrap(eval_620, a1);
        }
        default: {
            return 0.00000000000000000;
        }
    }
}

static inline int64_t mak_ir_fn_30(int64_t a0) {
    switch (a0) {
        case INT64_C(14777): {
            return INT64_C(1);
        }
        case INT64_C(2003500): {
            return INT64_C(1);
        }
        case INT64_C(101): {
            return INT64_C(1);
        }
        case INT64_C(1935761): {
            return INT64_C(1);
        }
        case INT64_C(621420511): {
            return INT64_C(1);
        }
        case INT64_C(123496560): {
            return INT64_C(1);
        }
        case INT64_C(1867848): {
            return INT64_C(1);
        }
        case INT64_C(244688005): {
            return INT64_C(1);
        }
        case INT64_C(57303579): {
            return INT64_C(1);
        }
        case INT64_C(506744797): {
            return INT64_C(1);
        }
        default: {
            return INT64_C(0);
        }
    }
}

static inline double mak_ir_fn_31(int64_t a0) {
    switch (a0) {
        case INT64_C(14777): {
            return 3.14159265358979312;
        }
        case INT64_C(2003500): {
            return 6.28318530717958623;
        }
        case INT64_C(101): {
            return 2.71828182845904509;
        }
        case INT64_C(1935761): {
            return 1.61803398874989490;
        }
        case INT64_C(621420511): {
            return 1.61803398874989490;
        }
        case INT64_C(123496560): {
            return 1.41421356237309515;
        }
        case INT64_C(1867848): {
            return 0.69314718055994529;
        }
        case INT64_C(244688005): {
            return 2.30258509299404590;
        }
        case INT64_C(57303579): {
            return 1.44269504088896339;
        }
        case INT64_C(506744797): {
            return 0.43429448190325182;
        }
        default: {
            return 0.00000000000000000;
        }
    }
}

static inline void mak_ir_fn_32(struct mak__a * a0, struct mak__d *out) {
    struct mak__a *arena_call_622 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
    if (!arena_call_622) {
        mak_ir_recover("ADT result allocation failed");
        arena_call_622 = &mak_ir_default_obj__a;
     } else {
        mak_ir_fn_2(a0, (struct mak__a *)(arena_call_622));
    }
    struct mak__a * scrut_621 = arena_call_622;
    if ((scrut_621 != NULL && (scrut_621)->tag == mak_ir_tag_0)) {
        *(out) = mak_ir_ctor_obj_6;
    }
    else if ((scrut_621 != NULL && (scrut_621)->tag == mak_ir_tag_1)) {
        int32_t mak_local_dk_247 = (scrut_621)->v.v1.f0;
        struct mak__a * mak_local_dl_248 = (scrut_621)->v.v1.f1;
        int64_t case_624 = 0;
        int32_t scrut_625 = mak_local_dk_247 >= (int32_t)(INT64_C(48));
        if ((scrut_625 == INT64_C(0))) {
            case_624 = INT64_C(0);
        }
        else if (1) {
            int64_t case_626 = 0;
            int32_t scrut_627 = mak_local_dk_247 <= (int32_t)(INT64_C(57));
            if ((scrut_627 == INT64_C(1))) {
                case_626 = INT64_C(1);
            }
            else if (1) {
                case_626 = INT64_C(0);
            }
            case_624 = case_626;
        }
        int64_t scrut_623 = case_624;
        if ((scrut_623 == INT64_C(1))) {
            struct mak__d *arena_call_629 = (struct mak__d *)mak_ir_arena_alloc(sizeof(struct mak__d), _Alignof(struct mak__d));
            if (!arena_call_629) {
                mak_ir_recover("ADT result allocation failed");
                arena_call_629 = &mak_ir_default_obj__d;
             } else {
                struct mak__a *arena_obj_630 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
                if (!arena_obj_630) {
                    mak_ir_recover("ADT object allocation failed");
                    arena_obj_630 = &mak_ir_default_obj__a;
                 } else {
                    (arena_obj_630)->tag = mak_ir_tag_1;
                    (arena_obj_630)->v.v1.f0 = mak_local_dk_247;
                    (arena_obj_630)->v.v1.f1 = mak_local_dl_248;
                }
                mak_ir_fn_7(arena_obj_630, 0.00000000000000000, (struct mak__d *)(arena_call_629));
            }
            struct mak__d * scrut_628 = arena_call_629;
            if ((scrut_628 != NULL && (scrut_628)->tag == mak_ir_tag_6)) {
                *(out) = mak_ir_ctor_obj_6;
            }
            else if ((scrut_628 != NULL && (scrut_628)->tag == mak_ir_tag_7)) {
                double mak_local_dm_249 = (scrut_628)->v.v7.f0;
                struct mak__a * mak_local_dn_250 = (scrut_628)->v.v7.f1;
                if ((mak_local_dn_250 != NULL && (mak_local_dn_250)->tag == mak_ir_tag_1) && (mak_local_dn_250->v.v1.f0 == INT64_C(46))) {
                    struct mak__a * mak_local_dp_251 = (mak_local_dn_250)->v.v1.f1;
                    mak_ir_fn_8(mak_local_dp_251, mak_local_dm_249, 0.10000000000000001, (struct mak__d *)(out));
                }
                else if ((mak_local_dn_250 != NULL && (mak_local_dn_250)->tag == mak_ir_tag_1)) {
                    int32_t mak_local_dq_252 = (mak_local_dn_250)->v.v1.f0;
                    struct mak__a * mak_local_dr_253 = (mak_local_dn_250)->v.v1.f1;
                    switch (mak_local_dq_252) {
                        case INT64_C(101): {
                            struct mak__e *arena_call_632 = (struct mak__e *)mak_ir_arena_alloc(sizeof(struct mak__e), _Alignof(struct mak__e));
                            if (!arena_call_632) {
                                mak_ir_recover("ADT result allocation failed");
                                arena_call_632 = &mak_ir_default_obj__e;
                             } else {
                                mak_ir_fn_6(mak_local_dr_253, INT64_C(1), (struct mak__e *)(arena_call_632));
                            }
                            struct mak__e * scrut_631 = arena_call_632;
                            if ((scrut_631 != NULL && (scrut_631)->tag == mak_ir_tag_8)) {
                                *(out) = mak_ir_ctor_obj_6;
                            }
                            else if ((scrut_631 != NULL && (scrut_631)->tag == mak_ir_tag_9)) {
                                int64_t mak_local_ds_254 = (scrut_631)->v.v9.f0;
                                struct mak__a * mak_local_dt_255 = (scrut_631)->v.v9.f1;
                                (out)->tag = mak_ir_tag_7;
                                double eval_633 = mak_ir_fn_4(mak_local_ds_254);
                                (out)->v.v7.f0 = ((double)(mak_local_dm_249) * (double)(eval_633));
                                (out)->v.v7.f1 = mak_local_dt_255;
                            }
                            else {
                                *(out) = *(&mak_ir_default_obj__d);
                            }
                            break;
                        }
                        case INT64_C(69): {
                            struct mak__e *arena_call_635 = (struct mak__e *)mak_ir_arena_alloc(sizeof(struct mak__e), _Alignof(struct mak__e));
                            if (!arena_call_635) {
                                mak_ir_recover("ADT result allocation failed");
                                arena_call_635 = &mak_ir_default_obj__e;
                             } else {
                                mak_ir_fn_6(mak_local_dr_253, INT64_C(1), (struct mak__e *)(arena_call_635));
                            }
                            struct mak__e * scrut_634 = arena_call_635;
                            if ((scrut_634 != NULL && (scrut_634)->tag == mak_ir_tag_8)) {
                                *(out) = mak_ir_ctor_obj_6;
                            }
                            else if ((scrut_634 != NULL && (scrut_634)->tag == mak_ir_tag_9)) {
                                int64_t mak_local_du_256 = (scrut_634)->v.v9.f0;
                                struct mak__a * mak_local_dv_257 = (scrut_634)->v.v9.f1;
                                (out)->tag = mak_ir_tag_7;
                                double eval_636 = mak_ir_fn_4(mak_local_du_256);
                                (out)->v.v7.f0 = ((double)(mak_local_dm_249) * (double)(eval_636));
                                (out)->v.v7.f1 = mak_local_dv_257;
                            }
                            else {
                                *(out) = *(&mak_ir_default_obj__d);
                            }
                            break;
                        }
                        default: {
                            (out)->tag = mak_ir_tag_7;
                            (out)->v.v7.f0 = mak_local_dm_249;
                            (out)->v.v7.f1 = mak_local_dn_250;
                            break;
                        }
                    }
                }
                else if (1) {
                    (out)->tag = mak_ir_tag_7;
                    (out)->v.v7.f0 = mak_local_dm_249;
                    (out)->v.v7.f1 = mak_local_dn_250;
                }
            }
            else {
                *(out) = *(&mak_ir_default_obj__d);
            }
        }
        else if (1) {
            if ((mak_local_dk_247 == INT64_C(46))) {
                if ((mak_local_dl_248 != NULL && (mak_local_dl_248)->tag == mak_ir_tag_1)) {
                    int32_t mak_local_dw_258 = (mak_local_dl_248)->v.v1.f0;
                    int64_t case_638 = 0;
                    int32_t scrut_639 = mak_local_dw_258 >= (int32_t)(INT64_C(48));
                    if ((scrut_639 == INT64_C(0))) {
                        case_638 = INT64_C(0);
                    }
                    else if (1) {
                        int64_t case_640 = 0;
                        int32_t scrut_641 = mak_local_dw_258 <= (int32_t)(INT64_C(57));
                        if ((scrut_641 == INT64_C(1))) {
                            case_640 = INT64_C(1);
                        }
                        else if (1) {
                            case_640 = INT64_C(0);
                        }
                        case_638 = case_640;
                    }
                    int64_t scrut_637 = case_638;
                    if ((scrut_637 == INT64_C(1))) {
                        mak_ir_fn_8(mak_local_dl_248, 0.00000000000000000, 0.10000000000000001, (struct mak__d *)(out));
                    }
                    else if (1) {
                        *(out) = mak_ir_ctor_obj_6;
                    }
                }
                else if (1) {
                    *(out) = mak_ir_ctor_obj_6;
                }
            }
            else if (1) {
                *(out) = mak_ir_ctor_obj_6;
            }
        }
    }
    else {
        *(out) = *(&mak_ir_default_obj__d);
    }
    return;
}

static inline void mak_ir_fn_33(struct mak__b *out) {
    int32_t scrut_642 = mak_ir_ext_0();
    if (1) {
        int32_t mak_local_bd_259 = scrut_642;
        int32_t scrut_643 = mak_local_bd_259 < (int32_t)(INT64_C(0));
        if ((scrut_643 == INT64_C(1))) {
            *(out) = mak_ir_ctor_obj_2;
        }
        else if (1) {
            int32_t scrut_644 = mak_local_bd_259 == (int32_t)(INT64_C(10));
            if ((scrut_644 == INT64_C(1))) {
                (out)->tag = mak_ir_tag_3;
                (out)->v.v3.f0 = &mak_ir_ctor_obj_0;
            }
            else if (1) {
                struct mak__a *arena_obj_645 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
                if (!arena_obj_645) {
                    mak_ir_recover("ADT object allocation failed");
                    arena_obj_645 = &mak_ir_default_obj__a;
                 } else {
                    (arena_obj_645)->tag = mak_ir_tag_1;
                    (arena_obj_645)->v.v1.f0 = mak_local_bd_259;
                    (arena_obj_645)->v.v1.f1 = &mak_ir_ctor_obj_0;
                }
                mak_ir_fn_1(arena_obj_645, (struct mak__b *)(out));
            }
        }
    }
    return;
}

static inline int64_t mak_ir_fn_34(struct mak__a * a0) {
    struct mak__a *arena_call_647 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
    if (!arena_call_647) {
        mak_ir_recover("ADT result allocation failed");
        arena_call_647 = &mak_ir_default_obj__a;
     } else {
        mak_ir_fn_2(a0, (struct mak__a *)(arena_call_647));
    }
    struct mak__a * scrut_646 = arena_call_647;
    if ((scrut_646 != NULL && (scrut_646)->tag == mak_ir_tag_0)) {
        return INT64_C(0);
    }
    else if ((scrut_646 != NULL && (scrut_646)->tag == mak_ir_tag_1) && (scrut_646->v.v1.f0 == INT64_C(101))) {
        struct mak__a * mak_local_bm_260 = (scrut_646)->v.v1.f1;
        if ((mak_local_bm_260 != NULL && (mak_local_bm_260)->tag == mak_ir_tag_1) && (mak_local_bm_260->v.v1.f0 == INT64_C(120))) {
            struct mak__a * mak_local_bn_261 = (mak_local_bm_260)->v.v1.f1;
            if ((mak_local_bn_261 != NULL && (mak_local_bn_261)->tag == mak_ir_tag_1) && (mak_local_bn_261->v.v1.f0 == INT64_C(105))) {
                struct mak__a * mak_local_bo_262 = (mak_local_bn_261)->v.v1.f1;
                if ((mak_local_bo_262 != NULL && (mak_local_bo_262)->tag == mak_ir_tag_1) && (mak_local_bo_262->v.v1.f0 == INT64_C(116))) {
                    struct mak__a * mak_local_bp_263 = (mak_local_bo_262)->v.v1.f1;
                    struct mak__a *arena_call_649 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
                    if (!arena_call_649) {
                        mak_ir_recover("ADT result allocation failed");
                        arena_call_649 = &mak_ir_default_obj__a;
                     } else {
                        mak_ir_fn_2(mak_local_bp_263, (struct mak__a *)(arena_call_649));
                    }
                    struct mak__a * scrut_648 = arena_call_649;
                    if ((scrut_648 != NULL && (scrut_648)->tag == mak_ir_tag_0)) {
                        return INT64_C(1);
                    }
                    else if (1) {
                        return INT64_C(0);
                    }
                }
                else if (1) {
                    return INT64_C(0);
                }
            }
            else if (1) {
                return INT64_C(0);
            }
        }
        else if (1) {
            return INT64_C(0);
        }
    }
    else if (1) {
        return INT64_C(0);
    }
}

static inline int64_t mak_ir_fn_35(struct mak__a * a0) {
    struct mak__a *arena_call_651 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
    if (!arena_call_651) {
        mak_ir_recover("ADT result allocation failed");
        arena_call_651 = &mak_ir_default_obj__a;
     } else {
        mak_ir_fn_2(a0, (struct mak__a *)(arena_call_651));
    }
    struct mak__a * scrut_650 = arena_call_651;
    if ((scrut_650 != NULL && (scrut_650)->tag == mak_ir_tag_1) && (scrut_650->v.v1.f0 == INT64_C(63))) {
        struct mak__a * mak_local_br_264 = (scrut_650)->v.v1.f1;
        struct mak__a *arena_call_653 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
        if (!arena_call_653) {
            mak_ir_recover("ADT result allocation failed");
            arena_call_653 = &mak_ir_default_obj__a;
         } else {
            mak_ir_fn_2(mak_local_br_264, (struct mak__a *)(arena_call_653));
        }
        struct mak__a * scrut_652 = arena_call_653;
        if ((scrut_652 != NULL && (scrut_652)->tag == mak_ir_tag_0)) {
            return INT64_C(1);
        }
        else if (1) {
            return INT64_C(0);
        }
    }
    else if ((scrut_650 != NULL && (scrut_650)->tag == mak_ir_tag_1) && (scrut_650->v.v1.f0 == INT64_C(104))) {
        struct mak__a * mak_local_bs_265 = (scrut_650)->v.v1.f1;
        if ((mak_local_bs_265 != NULL && (mak_local_bs_265)->tag == mak_ir_tag_1) && (mak_local_bs_265->v.v1.f0 == INT64_C(101))) {
            struct mak__a * mak_local_bt_266 = (mak_local_bs_265)->v.v1.f1;
            if ((mak_local_bt_266 != NULL && (mak_local_bt_266)->tag == mak_ir_tag_1) && (mak_local_bt_266->v.v1.f0 == INT64_C(108))) {
                struct mak__a * mak_local_bu_267 = (mak_local_bt_266)->v.v1.f1;
                if ((mak_local_bu_267 != NULL && (mak_local_bu_267)->tag == mak_ir_tag_1) && (mak_local_bu_267->v.v1.f0 == INT64_C(112))) {
                    struct mak__a * mak_local_bv_268 = (mak_local_bu_267)->v.v1.f1;
                    struct mak__a *arena_call_655 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
                    if (!arena_call_655) {
                        mak_ir_recover("ADT result allocation failed");
                        arena_call_655 = &mak_ir_default_obj__a;
                     } else {
                        mak_ir_fn_2(mak_local_bv_268, (struct mak__a *)(arena_call_655));
                    }
                    struct mak__a * scrut_654 = arena_call_655;
                    if ((scrut_654 != NULL && (scrut_654)->tag == mak_ir_tag_0)) {
                        return INT64_C(1);
                    }
                    else if (1) {
                        return INT64_C(0);
                    }
                }
                else if (1) {
                    return INT64_C(0);
                }
            }
            else if (1) {
                return INT64_C(0);
            }
        }
        else if (1) {
            return INT64_C(0);
        }
    }
    else if (1) {
        return INT64_C(0);
    }
}

static inline void mak_ir_fn_36(void) {
    mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)("\n=== MAK SCIENTIFIC CALCULATOR ===\n"), mak_ir_ext_3((const void *)("\n=== MAK SCIENTIFIC CALCULATOR ===\n")));
    mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)("Numbers are f64 (double precision). Output uses up to 17 significant digits.\n"), mak_ir_ext_3((const void *)("Numbers are f64 (double precision). Output uses up to 17 significant digits.\n")));
    mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)("Use radians for trig functions.\n\n"), mak_ir_ext_3((const void *)("Use radians for trig functions.\n\n")));
    mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)("ARITHMETIC\n"), mak_ir_ext_3((const void *)("ARITHMETIC\n")));
    mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)("  a+b       addition\n"), mak_ir_ext_3((const void *)("  a+b       addition\n")));
    mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)("  a-b       subtraction\n"), mak_ir_ext_3((const void *)("  a-b       subtraction\n")));
    mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)("  a*b       multiplication\n"), mak_ir_ext_3((const void *)("  a*b       multiplication\n")));
    mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)("  a/b       REAL division\n"), mak_ir_ext_3((const void *)("  a/b       REAL division\n")));
    mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)("  a//b      FLOOR / INTEGER-STYLE division\n"), mak_ir_ext_3((const void *)("  a//b      FLOOR / INTEGER-STYLE division\n")));
    mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)("  a%b       remainder (fmod)\n"), mak_ir_ext_3((const void *)("  a%b       remainder (fmod)\n")));
    mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)("  a^b       power\n"), mak_ir_ext_3((const void *)("  a^b       power\n")));
    mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)("  2(3+4), 2pi  implicit multiplication\n"), mak_ir_ext_3((const void *)("  2(3+4), 2pi  implicit multiplication\n")));
    mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)("  ( ... )   grouping\n"), mak_ir_ext_3((const void *)("  ( ... )   grouping\n")));
    mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)("  +x  -x    unary plus/minus\n"), mak_ir_ext_3((const void *)("  +x  -x    unary plus/minus\n")));
    mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)("  x!        factorial (gamma(x+1), including non-integer extension)\n"), mak_ir_ext_3((const void *)("  x!        factorial (gamma(x+1), including non-integer extension)\n")));
    mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)("  x%        percentage when % is at the end of a value\n\n"), mak_ir_ext_3((const void *)("  x%        percentage when % is at the end of a value\n\n")));
    mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)("CONSTANTS\n"), mak_ir_ext_3((const void *)("CONSTANTS\n")));
    mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)("  pi        3.141592653589793...\n"), mak_ir_ext_3((const void *)("  pi        3.141592653589793...\n")));
    mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)("  tau       2*pi\n"), mak_ir_ext_3((const void *)("  tau       2*pi\n")));
    mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)("  e         Euler's number\n"), mak_ir_ext_3((const void *)("  e         Euler's number\n")));
    mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)("  phi       golden ratio\n"), mak_ir_ext_3((const void *)("  phi       golden ratio\n")));
    mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)("  sqrt2     square root of 2\n"), mak_ir_ext_3((const void *)("  sqrt2     square root of 2\n")));
    mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)("  ln2       natural log of 2\n"), mak_ir_ext_3((const void *)("  ln2       natural log of 2\n")));
    mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)("  ln10      natural log of 10\n"), mak_ir_ext_3((const void *)("  ln10      natural log of 10\n")));
    mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)("  log2e     log base 2 of e\n"), mak_ir_ext_3((const void *)("  log2e     log base 2 of e\n")));
    mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)("  log10e    log base 10 of e\n\n"), mak_ir_ext_3((const void *)("  log10e    log base 10 of e\n\n")));
    mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)("BASIC FUNCTIONS\n"), mak_ir_ext_3((const void *)("BASIC FUNCTIONS\n")));
    mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)("  abs(x) fabs(x)     absolute value\n"), mak_ir_ext_3((const void *)("  abs(x) fabs(x)     absolute value\n")));
    mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)("  sqrt(x)            square root\n"), mak_ir_ext_3((const void *)("  sqrt(x)            square root\n")));
    mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)("  cbrt(x)            real cube root\n"), mak_ir_ext_3((const void *)("  cbrt(x)            real cube root\n")));
    mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)("  root(x,n)          nth root\n"), mak_ir_ext_3((const void *)("  root(x,n)          nth root\n")));
    mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)("  nthroot(x,n)       alias of root\n"), mak_ir_ext_3((const void *)("  nthroot(x,n)       alias of root\n")));
    mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)("  square(x)          x^2\n"), mak_ir_ext_3((const void *)("  square(x)          x^2\n")));
    mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)("  cube(x)            x^3\n"), mak_ir_ext_3((const void *)("  cube(x)            x^3\n")));
    mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)("  sign(x)            -1, 0, or +1\n"), mak_ir_ext_3((const void *)("  sign(x)            -1, 0, or +1\n")));
    mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)("  frac(x)            fractional part, x-trunc(x)\n"), mak_ir_ext_3((const void *)("  frac(x)            fractional part, x-trunc(x)\n")));
    mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)("  floor(x) ceil(x) trunc(x) round(x)\n"), mak_ir_ext_3((const void *)("  floor(x) ceil(x) trunc(x) round(x)\n")));
    mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)("  min(a,b) max(a,b) clamp(x,lo,hi)\n"), mak_ir_ext_3((const void *)("  min(a,b) max(a,b) clamp(x,lo,hi)\n")));
    mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)("  hypot(x,y)         sqrt(x^2+y^2)\n"), mak_ir_ext_3((const void *)("  hypot(x,y)         sqrt(x^2+y^2)\n")));
    mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)("  copysign(a,b)       magnitude of a with sign of b\n"), mak_ir_ext_3((const void *)("  copysign(a,b)       magnitude of a with sign of b\n")));
    mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)("  fmin fmax fdim      C floating-point min/max/positive-difference\n"), mak_ir_ext_3((const void *)("  fmin fmax fdim      C floating-point min/max/positive-difference\n")));
    mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)("  fma(a,b,c)          correctly-rounded a*b+c (where supported)\n"), mak_ir_ext_3((const void *)("  fma(a,b,c)          correctly-rounded a*b+c (where supported)\n")));
    mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)("  nextafter(a,b)      next representable double from a toward b\n\n"), mak_ir_ext_3((const void *)("  nextafter(a,b)      next representable double from a toward b\n\n")));
    mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)("EXPONENTIALS / LOGS\n"), mak_ir_ext_3((const void *)("EXPONENTIALS / LOGS\n")));
    mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)("  erf(x) erfc(x)      error function / complementary error function\n"), mak_ir_ext_3((const void *)("  erf(x) erfc(x)      error function / complementary error function\n")));
    mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)("  logb(x)             exponent of x in binary scale\n"), mak_ir_ext_3((const void *)("  logb(x)             exponent of x in binary scale\n")));
    mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)("  exp(x)              e^x\n"), mak_ir_ext_3((const void *)("  exp(x)              e^x\n")));
    mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)("  exp2(x)            2^x\n"), mak_ir_ext_3((const void *)("  exp2(x)            2^x\n")));
    mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)("  expm1(x)           e^x-1, accurate near zero\n"), mak_ir_ext_3((const void *)("  expm1(x)           e^x-1, accurate near zero\n")));
    mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)("  log(x) ln(x)       natural logarithm\n"), mak_ir_ext_3((const void *)("  log(x) ln(x)       natural logarithm\n")));
    mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)("  log2(x)             base-2 logarithm\n"), mak_ir_ext_3((const void *)("  log2(x)             base-2 logarithm\n")));
    mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)("  log10(x)            base-10 logarithm\n"), mak_ir_ext_3((const void *)("  log10(x)            base-10 logarithm\n")));
    mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)("  log1p(x)            ln(1+x), accurate near zero\n"), mak_ir_ext_3((const void *)("  log1p(x)            ln(1+x), accurate near zero\n")));
    mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)("  pow(x,y)            x^y\n\n"), mak_ir_ext_3((const void *)("  pow(x,y)            x^y\n\n")));
    mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)("TRIGONOMETRY \342\200\224 radians\n"), mak_ir_ext_3((const void *)("TRIGONOMETRY \342\200\224 radians\n")));
    mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)("  sin cos tan\n"), mak_ir_ext_3((const void *)("  sin cos tan\n")));
    mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)("  cot(x)=cos(x)/sin(x)\n"), mak_ir_ext_3((const void *)("  cot(x)=cos(x)/sin(x)\n")));
    mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)("  sec(x)=1/cos(x)\n"), mak_ir_ext_3((const void *)("  sec(x)=1/cos(x)\n")));
    mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)("  csc(x)=1/sin(x)\n"), mak_ir_ext_3((const void *)("  csc(x)=1/sin(x)\n")));
    mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)("  asin acos atan      inverse trig, result in radians\n"), mak_ir_ext_3((const void *)("  asin acos atan      inverse trig, result in radians\n")));
    mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)("  atan2(y,x)          quadrant-aware angle\n"), mak_ir_ext_3((const void *)("  atan2(y,x)          quadrant-aware angle\n")));
    mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)("  acot asec acsc      inverse cot/sec/csc\n\n"), mak_ir_ext_3((const void *)("  acot asec acsc      inverse cot/sec/csc\n\n")));
    mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)("HYPERBOLIC\n"), mak_ir_ext_3((const void *)("HYPERBOLIC\n")));
    mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)("  sinh cosh tanh\n"), mak_ir_ext_3((const void *)("  sinh cosh tanh\n")));
    mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)("  csch sech coth     reciprocal hyperbolic functions\n"), mak_ir_ext_3((const void *)("  csch sech coth     reciprocal hyperbolic functions\n")));
    mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)("  asinh acosh atanh\n\n"), mak_ir_ext_3((const void *)("  asinh acosh atanh\n\n")));
    mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)("ANGLE CONVERSION\n"), mak_ir_ext_3((const void *)("ANGLE CONVERSION\n")));
    mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)("  deg(x)              radians -> degrees\n"), mak_ir_ext_3((const void *)("  deg(x)              radians -> degrees\n")));
    mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)("  degrees(x)          alias\n"), mak_ir_ext_3((const void *)("  degrees(x)          alias\n")));
    mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)("  rad(x)              degrees -> radians\n"), mak_ir_ext_3((const void *)("  rad(x)              degrees -> radians\n")));
    mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)("  radians(x)          alias\n"), mak_ir_ext_3((const void *)("  radians(x)          alias\n")));
    mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)("  Example: sin(rad(30)) = 0.5\n\n"), mak_ir_ext_3((const void *)("  Example: sin(rad(30)) = 0.5\n\n")));
    mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)("NUMBER THEORY / COMBINATORICS\n"), mak_ir_ext_3((const void *)("NUMBER THEORY / COMBINATORICS\n")));
    mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)("  gcd(a,b)             greatest common divisor\n"), mak_ir_ext_3((const void *)("  gcd(a,b)             greatest common divisor\n")));
    mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)("  lcm(a,b)             least common multiple\n"), mak_ir_ext_3((const void *)("  lcm(a,b)             least common multiple\n")));
    mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)("  ncr(n,r) / comb      binomial coefficient\n"), mak_ir_ext_3((const void *)("  ncr(n,r) / comb      binomial coefficient\n")));
    mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)("  npr(n,r) / perm      permutations\n"), mak_ir_ext_3((const void *)("  npr(n,r) / perm      permutations\n")));
    mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)("  factorial(n) / fact  factorial\n"), mak_ir_ext_3((const void *)("  factorial(n) / fact  factorial\n")));
    mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)("  gamma(x)             Gamma(x)\n"), mak_ir_ext_3((const void *)("  gamma(x)             Gamma(x)\n")));
    mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)("  lgamma(x)            ln(|Gamma(x)|)\n\n"), mak_ir_ext_3((const void *)("  lgamma(x)            ln(|Gamma(x)|)\n\n")));
    mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)("REMAINDER FUNCTIONS\n"), mak_ir_ext_3((const void *)("REMAINDER FUNCTIONS\n")));
    mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)("  fmod(a,b) mod(a,b) rem(a,b)  floating remainder\n"), mak_ir_ext_3((const void *)("  fmod(a,b) mod(a,b) rem(a,b)  floating remainder\n")));
    mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)("  remainder(a,b)       IEEE-style remainder\n\n"), mak_ir_ext_3((const void *)("  remainder(a,b)       IEEE-style remainder\n\n")));
    mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)("PRECEDENCE (strongest first)\n"), mak_ir_ext_3((const void *)("PRECEDENCE (strongest first)\n")));
    mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)("  postfix !, final %\n"), mak_ir_ext_3((const void *)("  postfix !, final %\n")));
    mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)("  power ^             right-associative\n"), mak_ir_ext_3((const void *)("  power ^             right-associative\n")));
    mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)("  unary + -\n"), mak_ir_ext_3((const void *)("  unary + -\n")));
    mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)("  * / // %\n"), mak_ir_ext_3((const void *)("  * / // %\n")));
    mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)("  + -\n"), mak_ir_ext_3((const void *)("  + -\n")));
    mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)("  Use parentheses whenever you want explicit grouping.\n\n"), mak_ir_ext_3((const void *)("  Use parentheses whenever you want explicit grouping.\n\n")));
    mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)("EXAMPLES\n"), mak_ir_ext_3((const void *)("EXAMPLES\n")));
    mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)("  22/7                    3.1428571428571428\n"), mak_ir_ext_3((const void *)("  22/7                    3.1428571428571428\n")));
    mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)("  22//7                   3\n"), mak_ir_ext_3((const void *)("  22//7                   3\n")));
    mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)("  2^3^2                   512\n"), mak_ir_ext_3((const void *)("  2^3^2                   512\n")));
    mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)("  sqrt(2)^2               2\n"), mak_ir_ext_3((const void *)("  sqrt(2)^2               2\n")));
    mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)("  sin(pi/2)               1\n"), mak_ir_ext_3((const void *)("  sin(pi/2)               1\n")));
    mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)("  log10(1000)             3\n"), mak_ir_ext_3((const void *)("  log10(1000)             3\n")));
    mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)("  ncr(52,5)               2598960\n"), mak_ir_ext_3((const void *)("  ncr(52,5)               2598960\n")));
    mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)("  5!                      120\n\n"), mak_ir_ext_3((const void *)("  5!                      120\n\n")));
    mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)("IMPORTANT\n"), mak_ir_ext_3((const void *)("IMPORTANT\n")));
    mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)("  22/7  -> 3.1428571428571428...\n"), mak_ir_ext_3((const void *)("  22/7  -> 3.1428571428571428...\n")));
    mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)("  22//7 -> 3\n"), mak_ir_ext_3((const void *)("  22//7 -> 3\n")));
    mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)("  / never becomes integer division.\n"), mak_ir_ext_3((const void *)("  / never becomes integer division.\n")));
    mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)("  // is floor division, so -7//2 = -4.\n"), mak_ir_ext_3((const void *)("  // is floor division, so -7//2 = -4.\n")));
    mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)("  ^ accepts real exponents; e.g. 9^0.5 = 3.\n"), mak_ir_ext_3((const void *)("  ^ accepts real exponents; e.g. 9^0.5 = 3.\n")));
    mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)("  Invalid domains may display nan/inf according to libm.\n"), mak_ir_ext_3((const void *)("  Invalid domains may display nan/inf according to libm.\n")));
    mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)("  Type exit to leave the calculator.\n\n"), mak_ir_ext_3((const void *)("  Type exit to leave the calculator.\n\n")));
    return;
}

static inline int32_t mak_ir_fn_37(struct mak__a * a0) {
    struct mak__f *arena_call_657 = (struct mak__f *)mak_ir_arena_alloc(sizeof(struct mak__f), _Alignof(struct mak__f));
    if (!arena_call_657) {
        mak_ir_recover("ADT result allocation failed");
        arena_call_657 = &mak_ir_default_obj__f;
     } else {
        mak_ir_fn_16(a0, (struct mak__f *)(arena_call_657));
    }
    struct mak__f * scrut_656 = arena_call_657;
    if ((scrut_656 != NULL && (scrut_656)->tag == mak_ir_tag_11)) {
        mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)("Error: invalid expression\n"), mak_ir_ext_3((const void *)("Error: invalid expression\n")));
    }
    else if ((scrut_656 != NULL && (scrut_656)->tag == mak_ir_tag_10)) {
        double mak_local_ig_269 = (scrut_656)->v.v10.f0;
        struct mak__a * mak_local_ih_270 = (scrut_656)->v.v10.f1;
        struct mak__a *arena_call_659 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
        if (!arena_call_659) {
            mak_ir_recover("ADT result allocation failed");
            arena_call_659 = &mak_ir_default_obj__a;
         } else {
            mak_ir_fn_2(mak_local_ih_270, (struct mak__a *)(arena_call_659));
        }
        struct mak__a * scrut_658 = arena_call_659;
        if ((scrut_658 != NULL && (scrut_658)->tag == mak_ir_tag_0)) {
            return mak_ir_ext_2((const void *)("%.17g\n"), mak_local_ig_269);
        }
        else if (1) {
            mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)("Error: unexpected characters\n"), mak_ir_ext_3((const void *)("Error: unexpected characters\n")));
        }
    }
    else {
        mak_ir_recover("case had no matching arm");
        return 0;
    }
}

static inline int32_t mak_ir_fn_38(double a0) {
    return mak_ir_ext_2((const void *)("%.17g\n"), a0);
}

static inline void mak_ir_fn_39(void) {
    mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)("MAK Scientific Calculator\n"), mak_ir_ext_3((const void *)("MAK Scientific Calculator\n")));
    mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)("Real /, floor //, remainder %, power ^, factorial !\n"), mak_ir_ext_3((const void *)("Real /, floor //, remainder %, power ^, factorial !\n")));
    mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)("Type help for the complete scientific function list.\n"), mak_ir_ext_3((const void *)("Type help for the complete scientific function list.\n")));
    mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)("Example: 22/7 = 3.1428571428571428...\n\n"), mak_ir_ext_3((const void *)("Example: 22/7 = 3.1428571428571428...\n\n")));
    mak_ir_fn_17();
    return;
}


int main(void) {
    mak_user_a = mak_ir_fn_0;
    mak_user_b = mak_ir_fn_1;
    mak_user_e = mak_ir_fn_2;
    mak_user_o = mak_ir_fn_3;
    mak_user_u = mak_ir_fn_4;
    mak_user_v = mak_ir_fn_5;
    mak_user_w = mak_ir_fn_6;
    mak_user_x = mak_ir_fn_7;
    mak_user_y = mak_ir_fn_8;
    mak_user_ae = mak_ir_fn_9;
    mak_user_al = mak_ir_fn_10;
    mak_user_an = mak_ir_fn_11;
    mak_user_ao = mak_ir_fn_12;
    mak_user_ap = mak_ir_fn_13;
    mak_user_aq = mak_ir_fn_14;
    mak_user_ar = mak_ir_fn_15;
    mak_user_at = mak_ir_fn_16;
    mak_user_av = mak_ir_fn_17;
    mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)("MAK Scientific Calculator\n"), mak_ir_ext_3((const void *)("MAK Scientific Calculator\n")));
    mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)("Real /, floor //, remainder %, power ^, factorial !\n"), mak_ir_ext_3((const void *)("Real /, floor //, remainder %, power ^, factorial !\n")));
    mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)("Type help for the complete scientific function list.\n"), mak_ir_ext_3((const void *)("Type help for the complete scientific function list.\n")));
    mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)("Example: 22/7 = 3.1428571428571428...\n\n"), mak_ir_ext_3((const void *)("Example: 22/7 = 3.1428571428571428...\n\n")));
    mak_ir_fn_17();
    mak_ir_arena_shutdown();
    return 0;
}
