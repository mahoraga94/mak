#include <math.h>
#include <stdatomic.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
    mak_ir_tag_12 = 13u,
    mak_ir_tag_13 = 14u,
    mak_ir_tag_14 = 15u,
    mak_ir_tag_15 = 16u,
    mak_ir_tag_16 = 17u,
};

struct mak__a {
    uint32_t tag;
    union {
        struct {
            int32_t f0;
            struct mak__a * f1;
        } v1;
        struct {
            int64_t f0;
        } v2;
        struct {
            uint64_t f0;
        } v3;
        struct {
            uint64_t f0;
        } v4;
        struct {
            uint64_t f0;
        } v5;
        struct {
            uint64_t f0;
        } v6;
        struct {
            uint64_t f0;
        } v7;
        struct {
            int32_t f0;
        } v8;
        struct {
            const char * f0;
        } v9;
        struct {
            const void * f0;
        } v10;
        struct {
            double f0;
        } v11;
        struct {
            struct mak__b * f0;
            struct mak__c * f1;
        } v13;
        struct {
            int64_t f0;
            int64_t f1;
            int64_t f2;
            int64_t f3;
        } v14;
        struct {
            struct mak__d * f0;
            int64_t f1;
        } v15;
        struct {
            int64_t f0;
            int64_t f1;
        } v16;
    } v;
};

static struct mak__a mak_ir_ctor_obj_0 = { mak_ir_tag_0 };
static struct mak__a mak_ir_default_obj__a = { mak_ir_tag_0 };
struct mak__b {
    uint32_t tag;
    union {
        struct {
            int32_t f0;
            struct mak__a * f1;
        } v1;
        struct {
            int64_t f0;
        } v2;
        struct {
            uint64_t f0;
        } v3;
        struct {
            uint64_t f0;
        } v4;
        struct {
            uint64_t f0;
        } v5;
        struct {
            uint64_t f0;
        } v6;
        struct {
            uint64_t f0;
        } v7;
        struct {
            int32_t f0;
        } v8;
        struct {
            const char * f0;
        } v9;
        struct {
            const void * f0;
        } v10;
        struct {
            double f0;
        } v11;
        struct {
            struct mak__b * f0;
            struct mak__c * f1;
        } v13;
        struct {
            int64_t f0;
            int64_t f1;
            int64_t f2;
            int64_t f3;
        } v14;
        struct {
            struct mak__d * f0;
            int64_t f1;
        } v15;
        struct {
            int64_t f0;
            int64_t f1;
        } v16;
    } v;
};

static struct mak__b mak_ir_default_obj__b = { mak_ir_tag_2 };
struct mak__c {
    uint32_t tag;
    union {
        struct {
            int32_t f0;
            struct mak__a * f1;
        } v1;
        struct {
            int64_t f0;
        } v2;
        struct {
            uint64_t f0;
        } v3;
        struct {
            uint64_t f0;
        } v4;
        struct {
            uint64_t f0;
        } v5;
        struct {
            uint64_t f0;
        } v6;
        struct {
            uint64_t f0;
        } v7;
        struct {
            int32_t f0;
        } v8;
        struct {
            const char * f0;
        } v9;
        struct {
            const void * f0;
        } v10;
        struct {
            double f0;
        } v11;
        struct {
            struct mak__b * f0;
            struct mak__c * f1;
        } v13;
        struct {
            int64_t f0;
            int64_t f1;
            int64_t f2;
            int64_t f3;
        } v14;
        struct {
            struct mak__d * f0;
            int64_t f1;
        } v15;
        struct {
            int64_t f0;
            int64_t f1;
        } v16;
    } v;
};

static struct mak__c mak_ir_ctor_obj_12 = { mak_ir_tag_12 };
static struct mak__c mak_ir_default_obj__c = { mak_ir_tag_12 };
struct mak__d {
    uint32_t tag;
    struct {
        int64_t f0;
        int64_t f1;
        int64_t f2;
        int64_t f3;
    } v;
};

static struct mak__d mak_ir_default_obj__d = { mak_ir_tag_14 };
struct mak__e {
    uint32_t tag;
    struct {
        struct mak__d * f0;
        int64_t f1;
    } v;
};

static struct mak__e mak_ir_default_obj__e = { mak_ir_tag_15 };
struct mak__f {
    uint32_t tag;
    struct {
        int64_t f0;
        int64_t f1;
    } v;
};

static struct mak__f mak_ir_default_obj__f = { mak_ir_tag_16 };
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

extern int32_t mak_ir_ext_0(int32_t) __asm__("putchar");
extern uint64_t mak_ir_ext_1(const void *) __asm__("strlen");
extern double mak_ir_ext_2(double) __asm__("trunc");
extern double mak_ir_ext_3(double) __asm__("round");
extern double mak_ir_ext_4(double) __asm__("fabs");

static inline void mak_ir_fn_0(struct mak__a * a0, struct mak__a * a1, struct mak__a *out);
static inline int64_t mak_ir_fn_1(struct mak__a * a0, int64_t a1);
static inline int64_t mak_ir_fn_2(struct mak__a * a0);
static inline void mak_ir_fn_3(struct mak__a * a0, struct mak__a * a1, struct mak__a *out);
static inline void mak_ir_fn_4(int64_t a0, int64_t a1, struct mak__a *out);
static inline void mak_ir_fn_5(double a0, double a1, int64_t a2, struct mak__a * a3, struct mak__a *out);
static inline void mak_ir_fn_6(double a0, int64_t a1, int64_t a2, struct mak__a *out);
static inline int64_t mak_ir_fn_7(int64_t a0, int64_t a1);
static inline int64_t mak_ir_fn_8(int64_t a0, int64_t a1);
static inline void mak_ir_fn_9(int64_t a0, int64_t a1, int64_t a2, struct mak__a *out);
static inline void mak_ir_fn_10(int64_t a0, int64_t a1, int64_t a2, int64_t a3, double a4, struct mak__a *out);
static inline void mak_ir_fn_11(const void * a0, int64_t a1, struct mak__a * a2, struct mak__a *out);
static inline double mak_ir_fn_12(int64_t a0, double a1);
static inline void mak_ir_fn_13(double a0, int64_t a1, struct mak__a *out);
static inline void mak_ir_fn_14(const void * a0, int64_t a1, int64_t a2, struct mak__f *out);
static inline void mak_ir_fn_15(const void * a0, int64_t a1, struct mak__f *out);
static inline void mak_ir_fn_16(const void * a0, int64_t a1, int64_t a2, int64_t a3, struct mak__e *out);
static inline void mak_ir_fn_17(const void * a0, int64_t a1, int64_t a2, struct mak__f *out);
static inline void mak_ir_fn_18(const char * a0, int64_t a1, struct mak__c * a2, struct mak__a * a3, struct mak__a *out);
static inline int64_t mak_ir_fn_19(const char * a0, struct mak__c * a1);
static inline double mak_ir_fn_20(int64_t a0);
static inline int64_t mak_ir_fn_21(const void * a0, int64_t a1);
static inline void mak_ir_fn_22(const void * a0, int64_t a1, int64_t a2, struct mak__e *out);
static inline int64_t mak_ir_fn_23(struct mak__d * a0, int64_t a1, int64_t a2);
static inline void mak_ir_fn_24(int64_t a0, int64_t a1, int64_t a2, int64_t a3, struct mak__b * a4, int64_t a5, struct mak__a *out);
static inline void mak_ir_fn_25(int64_t a0, int64_t a1, int64_t a2, int64_t a3, struct mak__a *out);
static inline void mak_ir_fn_26(struct mak__a * a0, int64_t a1, struct mak__a *out);
static inline void mak_ir_fn_27(int64_t a0, int64_t a1, int64_t a2, double a3, struct mak__a *out);
static inline int64_t mak_ir_fn_28(void);

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
    case mak_ir_tag_4: return UINT32_C(1292594660);
    case mak_ir_tag_5: return UINT32_C(1292594660);
    case mak_ir_tag_6: return UINT32_C(1292594660);
    case mak_ir_tag_7: return UINT32_C(1292594660);
    case mak_ir_tag_8: return UINT32_C(1292594660);
    case mak_ir_tag_9: return UINT32_C(1292594660);
    case mak_ir_tag_10: return UINT32_C(1292594660);
    case mak_ir_tag_11: return UINT32_C(1292594660);
    case mak_ir_tag_12: return UINT32_C(1309372279);
    case mak_ir_tag_13: return UINT32_C(1309372279);
    case mak_ir_tag_14: return UINT32_C(1259039422);
    case mak_ir_tag_15: return UINT32_C(1275817041);
    case mak_ir_tag_16: return UINT32_C(1225484184);
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
        case mak_ir_tag_2: {
            struct mak__b *s = (struct mak__b *)(uintptr_t)src;
            struct mak__b *d = (struct mak__b *)dst;
            d->tag = mak_ir_tag_2;
            d->v.v2.f0 = s->v.v2.f0;
            return 1;
        }
        case mak_ir_tag_8: {
            struct mak__b *s = (struct mak__b *)(uintptr_t)src;
            struct mak__b *d = (struct mak__b *)dst;
            d->tag = mak_ir_tag_8;
            d->v.v8.f0 = s->v.v8.f0;
            return 1;
        }
        case mak_ir_tag_9: {
            struct mak__b *s = (struct mak__b *)(uintptr_t)src;
            struct mak__b *d = (struct mak__b *)dst;
            d->tag = mak_ir_tag_9;
            d->v.v9.f0 = s->v.v9.f0;
            return 1;
        }
        case mak_ir_tag_10: {
            struct mak__b *s = (struct mak__b *)(uintptr_t)src;
            struct mak__b *d = (struct mak__b *)dst;
            d->tag = mak_ir_tag_10;
            d->v.v10.f0 = s->v.v10.f0;
            return 1;
        }
        case mak_ir_tag_11: {
            struct mak__b *s = (struct mak__b *)(uintptr_t)src;
            struct mak__b *d = (struct mak__b *)dst;
            d->tag = mak_ir_tag_11;
            d->v.v11.f0 = s->v.v11.f0;
            return 1;
        }
        default: break;
        }
        break;
    }
    case UINT32_C(1309372279): {
        switch (src_tag) {
        case mak_ir_tag_13: {
            struct mak__c *s = (struct mak__c *)(uintptr_t)src;
            struct mak__c *d = (struct mak__c *)dst;
            d->tag = mak_ir_tag_13;
            d->v.v13.f0 = s->v.v13.f0;
            d->v.v13.f1 = s->v.v13.f1;
            return 1;
        }
        default: break;
        }
        break;
    }
    case UINT32_C(1259039422): {
        switch (src_tag) {
        case mak_ir_tag_14: {
            struct mak__d *s = (struct mak__d *)(uintptr_t)src;
            struct mak__d *d = (struct mak__d *)dst;
            d->tag = mak_ir_tag_14;
            d->v.f0 = s->v.f0;
            d->v.f1 = s->v.f1;
            d->v.f2 = s->v.f2;
            d->v.f3 = s->v.f3;
            return 1;
        }
        default: break;
        }
        break;
    }
    case UINT32_C(1275817041): {
        switch (src_tag) {
        case mak_ir_tag_15: {
            struct mak__e *s = (struct mak__e *)(uintptr_t)src;
            struct mak__e *d = (struct mak__e *)dst;
            d->tag = mak_ir_tag_15;
            d->v.f0 = s->v.f0;
            d->v.f1 = s->v.f1;
            return 1;
        }
        default: break;
        }
        break;
    }
    case UINT32_C(1225484184): {
        switch (src_tag) {
        case mak_ir_tag_16: {
            struct mak__f *s = (struct mak__f *)(uintptr_t)src;
            struct mak__f *d = (struct mak__f *)dst;
            d->tag = mak_ir_tag_16;
            d->v.f0 = s->v.f0;
            d->v.f1 = s->v.f1;
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

static void (*mak_user_a)(struct mak__a *, struct mak__a *, struct mak__a *);
static int64_t (*mak_user_c)(struct mak__a *, int64_t);
static int64_t (*mak_user_e)(struct mak__a *);
static void (*mak_user_f)(struct mak__a *, struct mak__a *, struct mak__a *);
static void (*mak_user_g)(int64_t, int64_t, struct mak__a *);
static void (*mak_user_h)(double, double, int64_t, struct mak__a *, struct mak__a *);
static void (*mak_user_i)(double, int64_t, int64_t, struct mak__a *);
static int64_t (*mak_user_k)(int64_t, int64_t);
static int64_t (*mak_user_l)(int64_t, int64_t);
static void (*mak_user_n)(int64_t, int64_t, int64_t, struct mak__a *);
static void (*mak_user_o)(int64_t, int64_t, int64_t, int64_t, double, struct mak__a *);
static void (*mak_user_p)(const void *, int64_t, struct mak__a *, struct mak__a *);
static double (*mak_user_t)(int64_t, double);
static void (*mak_user_v)(double, int64_t, struct mak__a *);
static void (*mak_user_y)(const void *, int64_t, int64_t, struct mak__f *);
static void (*mak_user_z)(const void *, int64_t, struct mak__f *);
static void (*mak_user_aa)(const void *, int64_t, int64_t, int64_t, struct mak__e *);
static void (*mak_user_ac)(const void *, int64_t, int64_t, struct mak__f *);
static void (*mak_user_ag)(const char *, int64_t, struct mak__c *, struct mak__a *, struct mak__a *);
static int64_t (*mak_user_ah)(const char *, struct mak__c *);

static inline void mak_ir_fn_0(struct mak__a * a0, struct mak__a * a1, struct mak__a *out) {
    for (;;) {
        if ((a0 != NULL && (a0)->tag == mak_ir_tag_0)) {
            *(out) = *(a1);
        }
        else if ((a0 != NULL && (a0)->tag == mak_ir_tag_1)) {
            int32_t mak_local_al_0 = (a0)->v.v1.f0;
            struct mak__a * mak_local_am_1 = (a0)->v.v1.f1;
            struct mak__a * tailarg_0 = mak_local_am_1;
            struct mak__a *arena_obj_2 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
            if (!arena_obj_2) {
                mak_ir_recover("ADT object allocation failed");
                arena_obj_2 = &mak_ir_default_obj__a;
             } else {
                (arena_obj_2)->tag = mak_ir_tag_1;
                (arena_obj_2)->v.v1.f0 = mak_local_al_0;
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

static inline int64_t mak_ir_fn_1(struct mak__a * a0, int64_t a1) {
    for (;;) {
        if ((a0 != NULL && (a0)->tag == mak_ir_tag_0)) {
            return a1;
        }
        else if ((a0 != NULL && (a0)->tag == mak_ir_tag_1)) {
            struct mak__a * mak_local_aq_2 = (a0)->v.v1.f1;
            struct mak__a * tailarg_3 = mak_local_aq_2;
            int64_t tailarg_4 = (a1 + INT64_C(1));
            a0 = tailarg_3;
            a1 = tailarg_4;
            continue;
        }
        else {
            mak_ir_recover("case had no matching arm");
            return 0;
        }
    }
}

static inline int64_t mak_ir_fn_2(struct mak__a * a0) {
    for (;;) {
        if ((a0 != NULL && (a0)->tag == mak_ir_tag_0)) {
            return INT64_C(0);
        }
        else if ((a0 != NULL && (a0)->tag == mak_ir_tag_1)) {
            int32_t mak_local_au_3 = (a0)->v.v1.f0;
            struct mak__a * mak_local_av_4 = (a0)->v.v1.f1;
            mak_ir_ext_0(mak_local_au_3);
            struct mak__a * tailarg_5 = mak_local_av_4;
            a0 = tailarg_5;
            continue;
        }
        else {
            mak_ir_recover("case had no matching arm");
            return 0;
        }
    }
}

static inline void mak_ir_fn_3(struct mak__a * a0, struct mak__a * a1, struct mak__a *out) {
    if ((a0 != NULL && (a0)->tag == mak_ir_tag_0)) {
        *(out) = *(a1);
    }
    else if ((a0 != NULL && (a0)->tag == mak_ir_tag_1)) {
        int32_t mak_local_ay_5 = (a0)->v.v1.f0;
        struct mak__a * mak_local_az_6 = (a0)->v.v1.f1;
        (out)->tag = mak_ir_tag_1;
        (out)->v.v1.f0 = mak_local_ay_5;
        struct mak__a *arena_call_6 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
        if (!arena_call_6) {
            mak_ir_recover("ADT result allocation failed");
            arena_call_6 = &mak_ir_default_obj__a;
         } else {
            mak_ir_fn_3(mak_local_az_6, a1, (struct mak__a *)(arena_call_6));
        }
        (out)->v.v1.f1 = arena_call_6;
    }
    else {
        *(out) = *(&mak_ir_default_obj__a);
    }
    return;
}

static inline void mak_ir_fn_4(int64_t a0, int64_t a1, struct mak__a *out) {
    int32_t scrut_7 = (int32_t)(a1) <= (int32_t)(INT64_C(0));
    if ((scrut_7 == INT64_C(1))) {
        *(out) = mak_ir_ctor_obj_0;
    }
    else if (1) {
        (out)->tag = mak_ir_tag_1;
        (out)->v.v1.f0 = (int32_t)(a0);
        struct mak__a *arena_call_8 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
        if (!arena_call_8) {
            mak_ir_recover("ADT result allocation failed");
            arena_call_8 = &mak_ir_default_obj__a;
         } else {
            mak_ir_fn_4(a0, (a1 - INT64_C(1)), (struct mak__a *)(arena_call_8));
        }
        (out)->v.v1.f1 = arena_call_8;
    }
    return;
}

static inline void mak_ir_fn_5(double a0, double a1, int64_t a2, struct mak__a * a3, struct mak__a *out) {
    for (;;) {
        int32_t scrut_9 = (int32_t)(a0) == (int32_t)(INT64_C(0));
        if ((scrut_9 == INT64_C(1))) {
            *(out) = *(a3);
        }
        else if (1) {
            double tailarg_10 = ((double)(a0) / (double)(a1));
            double tailarg_11 = a1;
            int64_t tailarg_12 = a2;
            struct mak__a *arena_obj_14 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
            if (!arena_obj_14) {
                mak_ir_recover("ADT object allocation failed");
                arena_obj_14 = &mak_ir_default_obj__a;
             } else {
                (arena_obj_14)->tag = mak_ir_tag_1;
                int64_t mak_local_bg_7 = ((int64_t)(a0) % (int64_t)(a1));
                int64_t case_15 = 0;
                int32_t scrut_16 = (int32_t)(mak_local_bg_7) < (int32_t)(INT64_C(10));
                if ((scrut_16 == INT64_C(1))) {
                    case_15 = (mak_local_bg_7 + INT64_C(48));
                }
                else if (1) {
                    int64_t case_17 = 0;
                    if ((a2 == INT64_C(1))) {
                        case_17 = (mak_local_bg_7 + INT64_C(55));
                    }
                    else if (1) {
                        case_17 = (mak_local_bg_7 + INT64_C(87));
                    }
                    case_15 = case_17;
                }
                int32_t arg_18 = (int32_t)(case_15);
                (arena_obj_14)->v.v1.f0 = arg_18;
                (arena_obj_14)->v.v1.f1 = a3;
            }
            struct mak__a * tailarg_13 = arena_obj_14;
            a0 = tailarg_10;
            a1 = tailarg_11;
            a2 = tailarg_12;
            a3 = tailarg_13;
            continue;
        }
        return;
    }
}

static inline void mak_ir_fn_6(double a0, int64_t a1, int64_t a2, struct mak__a *out) {
    int32_t scrut_19 = (int32_t)(a0) == (int32_t)(INT64_C(0));
    if ((scrut_19 == INT64_C(1))) {
        (out)->tag = mak_ir_tag_1;
        (out)->v.v1.f0 = (int32_t)(INT64_C(48));
        (out)->v.v1.f1 = &mak_ir_ctor_obj_0;
    }
    else if (1) {
        struct mak__a *arena_call_20 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
        if (!arena_call_20) {
            mak_ir_recover("ADT result allocation failed");
            arena_call_20 = &mak_ir_default_obj__a;
         } else {
            mak_ir_fn_5(a0, a1, a2, &mak_ir_ctor_obj_0, (struct mak__a *)(arena_call_20));
        }
        mak_ir_fn_0(arena_call_20, &mak_ir_ctor_obj_0, (struct mak__a *)(out));
    }
    return;
}

static inline int64_t mak_ir_fn_7(int64_t a0, int64_t a1) {
    int64_t scrut_21 = (a0 & a1);
    if ((scrut_21 == INT64_C(0))) {
        return INT64_C(0);
    }
    else if (1) {
        return INT64_C(1);
    }
}

static inline int64_t mak_ir_fn_8(int64_t a0, int64_t a1) {
    int32_t scrut_22 = (int32_t)(a1) < (int32_t)(INT64_C(0));
    if ((scrut_22 == INT64_C(1))) {
        return INT64_C(45);
    }
    else if (1) {
        int64_t case_24 = 0;
        int64_t scrut_25 = (a0 & INT64_C(4));
        if ((scrut_25 == INT64_C(0))) {
            case_24 = INT64_C(0);
        }
        else if (1) {
            case_24 = INT64_C(1);
        }
        int64_t scrut_23 = case_24;
        if ((scrut_23 == INT64_C(1))) {
            return INT64_C(43);
        }
        else if (1) {
            int64_t case_27 = 0;
            int64_t scrut_28 = (a0 & INT64_C(8));
            if ((scrut_28 == INT64_C(0))) {
                case_27 = INT64_C(0);
            }
            else if (1) {
                case_27 = INT64_C(1);
            }
            int64_t scrut_26 = case_27;
            if ((scrut_26 == INT64_C(1))) {
                return INT64_C(32);
            }
            else if (1) {
                return INT64_C(0);
            }
        }
    }
}

static inline void mak_ir_fn_9(int64_t a0, int64_t a1, int64_t a2, struct mak__a *out) {
    int64_t mak_local_bw_8 = mak_ir_fn_8(a0, a2);
    struct mak__a *letobj_29 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
    if (!letobj_29) {
        mak_ir_recover("ADT local allocation failed");
        letobj_29 = &mak_ir_default_obj__a;
     } else {
        *(letobj_29) = *(&mak_ir_default_obj__a);
        int32_t scrut_30 = (int32_t)(mak_local_bw_8) == (int32_t)(INT64_C(0));
        if ((scrut_30 == INT64_C(1))) {
            *(letobj_29) = mak_ir_ctor_obj_0;
        }
        else if (1) {
            (letobj_29)->tag = mak_ir_tag_1;
            (letobj_29)->v.v1.f0 = (int32_t)(mak_local_bw_8);
            (letobj_29)->v.v1.f1 = &mak_ir_ctor_obj_0;
        }
    }
    struct mak__a * mak_local_ca_9 = letobj_29;
    int64_t case_32 = 0;
    int64_t scrut_33 = (a0 & INT64_C(1));
    if ((scrut_33 == INT64_C(0))) {
        case_32 = INT64_C(0);
    }
    else if (1) {
        case_32 = INT64_C(1);
    }
    int64_t scrut_31 = case_32;
    if ((scrut_31 == INT64_C(1))) {
        struct mak__a *arena_call_34 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
        if (!arena_call_34) {
            mak_ir_recover("ADT result allocation failed");
            arena_call_34 = &mak_ir_default_obj__a;
         } else {
            int64_t case_35 = 0;
            struct mak__a *arena_call_39 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
            if (!arena_call_39) {
                mak_ir_recover("ADT result allocation failed");
                arena_call_39 = &mak_ir_default_obj__a;
             } else {
                int64_t case_40 = 0;
                int32_t scrut_41 = (int32_t)(a2) < (int32_t)(INT64_C(0));
                if ((scrut_41 == INT64_C(1))) {
                    case_40 = (-a2);
                }
                else if (1) {
                    case_40 = a2;
                }
                double arg_42 = case_40;
                mak_ir_fn_6(arg_42, INT64_C(10), INT64_C(0), (struct mak__a *)(arena_call_39));
            }
            int64_t eval_38 = mak_ir_fn_1(arena_call_39, INT64_C(0));
            int64_t case_44 = 0;
            int32_t scrut_45 = (int32_t)(mak_local_bw_8) == (int32_t)(INT64_C(0));
            if ((scrut_45 == INT64_C(1))) {
                case_44 = INT64_C(0);
            }
            else if (1) {
                case_44 = INT64_C(1);
            }
            int64_t eval_43 = case_44;
            int64_t eval_37 = (eval_38 + eval_43);
            int32_t scrut_36 = (int32_t)(a1) <= (int32_t)(eval_37);
            if ((scrut_36 == INT64_C(1))) {
                case_35 = INT64_C(0);
            }
            else if (1) {
                struct mak__a *arena_call_48 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
                if (!arena_call_48) {
                    mak_ir_recover("ADT result allocation failed");
                    arena_call_48 = &mak_ir_default_obj__a;
                 } else {
                    int64_t case_49 = 0;
                    int32_t scrut_50 = (int32_t)(a2) < (int32_t)(INT64_C(0));
                    if ((scrut_50 == INT64_C(1))) {
                        case_49 = (-a2);
                    }
                    else if (1) {
                        case_49 = a2;
                    }
                    double arg_51 = case_49;
                    mak_ir_fn_6(arg_51, INT64_C(10), INT64_C(0), (struct mak__a *)(arena_call_48));
                }
                int64_t eval_47 = mak_ir_fn_1(arena_call_48, INT64_C(0));
                int64_t case_53 = 0;
                int32_t scrut_54 = (int32_t)(mak_local_bw_8) == (int32_t)(INT64_C(0));
                if ((scrut_54 == INT64_C(1))) {
                    case_53 = INT64_C(0);
                }
                else if (1) {
                    case_53 = INT64_C(1);
                }
                int64_t eval_52 = case_53;
                int64_t eval_46 = (eval_47 + eval_52);
                case_35 = (a1 - eval_46);
            }
            mak_ir_fn_4(INT64_C(32), case_35, (struct mak__a *)(arena_call_34));
        }
        struct mak__a *arena_call_55 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
        if (!arena_call_55) {
            mak_ir_recover("ADT result allocation failed");
            arena_call_55 = &mak_ir_default_obj__a;
         } else {
            struct mak__a *arena_call_56 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
            if (!arena_call_56) {
                mak_ir_recover("ADT result allocation failed");
                arena_call_56 = &mak_ir_default_obj__a;
             } else {
                int64_t case_57 = 0;
                int32_t scrut_58 = (int32_t)(a2) < (int32_t)(INT64_C(0));
                if ((scrut_58 == INT64_C(1))) {
                    case_57 = (-a2);
                }
                else if (1) {
                    case_57 = a2;
                }
                double arg_59 = case_57;
                mak_ir_fn_6(arg_59, INT64_C(10), INT64_C(0), (struct mak__a *)(arena_call_56));
            }
            mak_ir_fn_3(arena_call_56, mak_local_ca_9, (struct mak__a *)(arena_call_55));
        }
        mak_ir_fn_3(arena_call_34, arena_call_55, (struct mak__a *)(out));
    }
    else if (1) {
        int64_t case_61 = 0;
        int64_t scrut_62 = (a0 & INT64_C(2));
        if ((scrut_62 == INT64_C(0))) {
            case_61 = INT64_C(0);
        }
        else if (1) {
            case_61 = INT64_C(1);
        }
        int64_t scrut_60 = case_61;
        if ((scrut_60 == INT64_C(1))) {
            struct mak__a *arena_call_63 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
            if (!arena_call_63) {
                mak_ir_recover("ADT result allocation failed");
                arena_call_63 = &mak_ir_default_obj__a;
             } else {
                int64_t case_64 = 0;
                int32_t scrut_65 = (int32_t)(a2) < (int32_t)(INT64_C(0));
                if ((scrut_65 == INT64_C(1))) {
                    case_64 = (-a2);
                }
                else if (1) {
                    case_64 = a2;
                }
                double arg_66 = case_64;
                mak_ir_fn_6(arg_66, INT64_C(10), INT64_C(0), (struct mak__a *)(arena_call_63));
            }
            struct mak__a *arena_call_67 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
            if (!arena_call_67) {
                mak_ir_recover("ADT result allocation failed");
                arena_call_67 = &mak_ir_default_obj__a;
             } else {
                struct mak__a *arena_call_68 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
                if (!arena_call_68) {
                    mak_ir_recover("ADT result allocation failed");
                    arena_call_68 = &mak_ir_default_obj__a;
                 } else {
                    int64_t case_69 = 0;
                    struct mak__a *arena_call_73 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
                    if (!arena_call_73) {
                        mak_ir_recover("ADT result allocation failed");
                        arena_call_73 = &mak_ir_default_obj__a;
                     } else {
                        int64_t case_74 = 0;
                        int32_t scrut_75 = (int32_t)(a2) < (int32_t)(INT64_C(0));
                        if ((scrut_75 == INT64_C(1))) {
                            case_74 = (-a2);
                        }
                        else if (1) {
                            case_74 = a2;
                        }
                        double arg_76 = case_74;
                        mak_ir_fn_6(arg_76, INT64_C(10), INT64_C(0), (struct mak__a *)(arena_call_73));
                    }
                    int64_t eval_72 = mak_ir_fn_1(arena_call_73, INT64_C(0));
                    int64_t case_78 = 0;
                    int32_t scrut_79 = (int32_t)(mak_local_bw_8) == (int32_t)(INT64_C(0));
                    if ((scrut_79 == INT64_C(1))) {
                        case_78 = INT64_C(0);
                    }
                    else if (1) {
                        case_78 = INT64_C(1);
                    }
                    int64_t eval_77 = case_78;
                    int64_t eval_71 = (eval_72 + eval_77);
                    int32_t scrut_70 = (int32_t)(a1) <= (int32_t)(eval_71);
                    if ((scrut_70 == INT64_C(1))) {
                        case_69 = INT64_C(0);
                    }
                    else if (1) {
                        struct mak__a *arena_call_82 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
                        if (!arena_call_82) {
                            mak_ir_recover("ADT result allocation failed");
                            arena_call_82 = &mak_ir_default_obj__a;
                         } else {
                            int64_t case_83 = 0;
                            int32_t scrut_84 = (int32_t)(a2) < (int32_t)(INT64_C(0));
                            if ((scrut_84 == INT64_C(1))) {
                                case_83 = (-a2);
                            }
                            else if (1) {
                                case_83 = a2;
                            }
                            double arg_85 = case_83;
                            mak_ir_fn_6(arg_85, INT64_C(10), INT64_C(0), (struct mak__a *)(arena_call_82));
                        }
                        int64_t eval_81 = mak_ir_fn_1(arena_call_82, INT64_C(0));
                        int64_t case_87 = 0;
                        int32_t scrut_88 = (int32_t)(mak_local_bw_8) == (int32_t)(INT64_C(0));
                        if ((scrut_88 == INT64_C(1))) {
                            case_87 = INT64_C(0);
                        }
                        else if (1) {
                            case_87 = INT64_C(1);
                        }
                        int64_t eval_86 = case_87;
                        int64_t eval_80 = (eval_81 + eval_86);
                        case_69 = (a1 - eval_80);
                    }
                    mak_ir_fn_4(INT64_C(48), case_69, (struct mak__a *)(arena_call_68));
                }
                mak_ir_fn_3(arena_call_68, mak_local_ca_9, (struct mak__a *)(arena_call_67));
            }
            mak_ir_fn_3(arena_call_63, arena_call_67, (struct mak__a *)(out));
        }
        else if (1) {
            struct mak__a *arena_call_89 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
            if (!arena_call_89) {
                mak_ir_recover("ADT result allocation failed");
                arena_call_89 = &mak_ir_default_obj__a;
             } else {
                int64_t case_90 = 0;
                int32_t scrut_91 = (int32_t)(a2) < (int32_t)(INT64_C(0));
                if ((scrut_91 == INT64_C(1))) {
                    case_90 = (-a2);
                }
                else if (1) {
                    case_90 = a2;
                }
                double arg_92 = case_90;
                mak_ir_fn_6(arg_92, INT64_C(10), INT64_C(0), (struct mak__a *)(arena_call_89));
            }
            struct mak__a *arena_call_93 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
            if (!arena_call_93) {
                mak_ir_recover("ADT result allocation failed");
                arena_call_93 = &mak_ir_default_obj__a;
             } else {
                struct mak__a *arena_call_94 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
                if (!arena_call_94) {
                    mak_ir_recover("ADT result allocation failed");
                    arena_call_94 = &mak_ir_default_obj__a;
                 } else {
                    int64_t case_95 = 0;
                    struct mak__a *arena_call_99 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
                    if (!arena_call_99) {
                        mak_ir_recover("ADT result allocation failed");
                        arena_call_99 = &mak_ir_default_obj__a;
                     } else {
                        int64_t case_100 = 0;
                        int32_t scrut_101 = (int32_t)(a2) < (int32_t)(INT64_C(0));
                        if ((scrut_101 == INT64_C(1))) {
                            case_100 = (-a2);
                        }
                        else if (1) {
                            case_100 = a2;
                        }
                        double arg_102 = case_100;
                        mak_ir_fn_6(arg_102, INT64_C(10), INT64_C(0), (struct mak__a *)(arena_call_99));
                    }
                    int64_t eval_98 = mak_ir_fn_1(arena_call_99, INT64_C(0));
                    int64_t case_104 = 0;
                    int32_t scrut_105 = (int32_t)(mak_local_bw_8) == (int32_t)(INT64_C(0));
                    if ((scrut_105 == INT64_C(1))) {
                        case_104 = INT64_C(0);
                    }
                    else if (1) {
                        case_104 = INT64_C(1);
                    }
                    int64_t eval_103 = case_104;
                    int64_t eval_97 = (eval_98 + eval_103);
                    int32_t scrut_96 = (int32_t)(a1) <= (int32_t)(eval_97);
                    if ((scrut_96 == INT64_C(1))) {
                        case_95 = INT64_C(0);
                    }
                    else if (1) {
                        struct mak__a *arena_call_108 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
                        if (!arena_call_108) {
                            mak_ir_recover("ADT result allocation failed");
                            arena_call_108 = &mak_ir_default_obj__a;
                         } else {
                            int64_t case_109 = 0;
                            int32_t scrut_110 = (int32_t)(a2) < (int32_t)(INT64_C(0));
                            if ((scrut_110 == INT64_C(1))) {
                                case_109 = (-a2);
                            }
                            else if (1) {
                                case_109 = a2;
                            }
                            double arg_111 = case_109;
                            mak_ir_fn_6(arg_111, INT64_C(10), INT64_C(0), (struct mak__a *)(arena_call_108));
                        }
                        int64_t eval_107 = mak_ir_fn_1(arena_call_108, INT64_C(0));
                        int64_t case_113 = 0;
                        int32_t scrut_114 = (int32_t)(mak_local_bw_8) == (int32_t)(INT64_C(0));
                        if ((scrut_114 == INT64_C(1))) {
                            case_113 = INT64_C(0);
                        }
                        else if (1) {
                            case_113 = INT64_C(1);
                        }
                        int64_t eval_112 = case_113;
                        int64_t eval_106 = (eval_107 + eval_112);
                        case_95 = (a1 - eval_106);
                    }
                    mak_ir_fn_4(INT64_C(32), case_95, (struct mak__a *)(arena_call_94));
                }
                mak_ir_fn_3(mak_local_ca_9, arena_call_94, (struct mak__a *)(arena_call_93));
            }
            mak_ir_fn_3(arena_call_89, arena_call_93, (struct mak__a *)(out));
        }
    }
    return;
}

static inline void mak_ir_fn_10(int64_t a0, int64_t a1, int64_t a2, int64_t a3, double a4, struct mak__a *out) {
    struct mak__a *letobj_115 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
    if (!letobj_115) {
        mak_ir_recover("ADT local allocation failed");
        letobj_115 = &mak_ir_default_obj__a;
     } else {
        *(letobj_115) = *(&mak_ir_default_obj__a);
        mak_ir_fn_6(a4, a2, a3, (struct mak__a *)(letobj_115));
    }
    struct mak__a * mak_local_cg_10 = letobj_115;
    int64_t mak_local_ch_11 = mak_ir_fn_1(mak_local_cg_10, INT64_C(0));
    int64_t case_116 = 0;
    int32_t scrut_117 = (int32_t)(a1) <= (int32_t)(mak_local_ch_11);
    if ((scrut_117 == INT64_C(1))) {
        case_116 = INT64_C(0);
    }
    else if (1) {
        case_116 = (a1 - mak_local_ch_11);
    }
    int64_t mak_local_ci_12 = case_116;
    int64_t case_119 = 0;
    int64_t scrut_120 = (a0 & INT64_C(1));
    if ((scrut_120 == INT64_C(0))) {
        case_119 = INT64_C(0);
    }
    else if (1) {
        case_119 = INT64_C(1);
    }
    int64_t scrut_118 = case_119;
    if ((scrut_118 == INT64_C(1))) {
        struct mak__a *arena_call_121 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
        if (!arena_call_121) {
            mak_ir_recover("ADT result allocation failed");
            arena_call_121 = &mak_ir_default_obj__a;
         } else {
            mak_ir_fn_4(INT64_C(32), mak_local_ci_12, (struct mak__a *)(arena_call_121));
        }
        mak_ir_fn_3(arena_call_121, mak_local_cg_10, (struct mak__a *)(out));
    }
    else if (1) {
        int64_t case_123 = 0;
        int64_t scrut_124 = (a0 & INT64_C(2));
        if ((scrut_124 == INT64_C(0))) {
            case_123 = INT64_C(0);
        }
        else if (1) {
            case_123 = INT64_C(1);
        }
        int64_t scrut_122 = case_123;
        if ((scrut_122 == INT64_C(1))) {
            struct mak__a *arena_call_125 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
            if (!arena_call_125) {
                mak_ir_recover("ADT result allocation failed");
                arena_call_125 = &mak_ir_default_obj__a;
             } else {
                mak_ir_fn_4(INT64_C(48), mak_local_ci_12, (struct mak__a *)(arena_call_125));
            }
            mak_ir_fn_3(mak_local_cg_10, arena_call_125, (struct mak__a *)(out));
        }
        else if (1) {
            struct mak__a *arena_call_126 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
            if (!arena_call_126) {
                mak_ir_recover("ADT result allocation failed");
                arena_call_126 = &mak_ir_default_obj__a;
             } else {
                mak_ir_fn_4(INT64_C(32), mak_local_ci_12, (struct mak__a *)(arena_call_126));
            }
            mak_ir_fn_3(mak_local_cg_10, arena_call_126, (struct mak__a *)(out));
        }
    }
    return;
}

static inline void mak_ir_fn_11(const void * a0, int64_t a1, struct mak__a * a2, struct mak__a *out) {
    for (;;) {
        int32_t scrut_127 = (int32_t)(a1) < (int32_t)(INT64_C(0));
        if ((scrut_127 == INT64_C(1))) {
            *(out) = *(a2);
        }
        else if (1) {
            const void * tailarg_128 = a0;
            int64_t tailarg_129 = (a1 - INT64_C(1));
            struct mak__a *arena_obj_131 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
            if (!arena_obj_131) {
                mak_ir_recover("ADT object allocation failed");
                arena_obj_131 = &mak_ir_default_obj__a;
             } else {
                (arena_obj_131)->tag = mak_ir_tag_1;
                int32_t arg_132 = (int32_t)((*((uint8_t *)(uintptr_t)(a0) + (intptr_t)(a1))));
                (arena_obj_131)->v.v1.f0 = arg_132;
                (arena_obj_131)->v.v1.f1 = a2;
            }
            struct mak__a * tailarg_130 = arena_obj_131;
            a0 = tailarg_128;
            a1 = tailarg_129;
            a2 = tailarg_130;
            continue;
        }
        return;
    }
}

static inline double mak_ir_fn_12(int64_t a0, double a1) {
    for (;;) {
        int32_t scrut_133 = (int32_t)(a0) <= (int32_t)(INT64_C(0));
        if ((scrut_133 == INT64_C(1))) {
            return a1;
        }
        else if (1) {
            int64_t tailarg_134 = (a0 - INT64_C(1));
            double tailarg_135 = ((double)(a1) * (double)(10.00000000000000000));
            a0 = tailarg_134;
            a1 = tailarg_135;
            continue;
        }
    }
}

static inline void mak_ir_fn_13(double a0, int64_t a1, struct mak__a *out) {
    int32_t scrut_136 = (int32_t)(a1) <= (int32_t)(INT64_C(0));
    if ((scrut_136 == INT64_C(1))) {
        mak_ir_fn_6(mak_ir_ext_3(a0), INT64_C(10), INT64_C(0), (struct mak__a *)(out));
    }
    else if (1) {
        double mak_local_df_13 = mak_ir_ext_2(a0);
        struct mak__a *letobj_137 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
        if (!letobj_137) {
            mak_ir_recover("ADT local allocation failed");
            letobj_137 = &mak_ir_default_obj__a;
         } else {
            *(letobj_137) = *(&mak_ir_default_obj__a);
            double eval_138 = ((double)(a0) - (double)(mak_local_df_13));
            double eval_139 = mak_ir_fn_12(a1, 1.00000000000000000);
            mak_ir_fn_6(mak_ir_ext_3(((double)(eval_138) * (double)(eval_139))), INT64_C(10), INT64_C(0), (struct mak__a *)(letobj_137));
        }
        struct mak__a * mak_local_dk_14 = letobj_137;
        struct mak__a *arena_call_140 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
        if (!arena_call_140) {
            mak_ir_recover("ADT result allocation failed");
            arena_call_140 = &mak_ir_default_obj__a;
         } else {
            struct mak__a *arena_call_141 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
            if (!arena_call_141) {
                mak_ir_recover("ADT result allocation failed");
                arena_call_141 = &mak_ir_default_obj__a;
             } else {
                int64_t mak_local_dl_15 = mak_ir_fn_1(mak_local_dk_14, INT64_C(0));
                int64_t case_142 = 0;
                int32_t scrut_143 = (int32_t)(mak_local_dl_15) >= (int32_t)(a1);
                if ((scrut_143 == INT64_C(1))) {
                    case_142 = INT64_C(0);
                }
                else if (1) {
                    case_142 = (a1 - mak_local_dl_15);
                }
                mak_ir_fn_4(INT64_C(48), case_142, (struct mak__a *)(arena_call_141));
            }
            mak_ir_fn_3(mak_local_dk_14, arena_call_141, (struct mak__a *)(arena_call_140));
        }
        struct mak__a *arena_call_144 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
        if (!arena_call_144) {
            mak_ir_recover("ADT result allocation failed");
            arena_call_144 = &mak_ir_default_obj__a;
         } else {
            struct mak__a *arena_obj_145 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
            if (!arena_obj_145) {
                mak_ir_recover("ADT object allocation failed");
                arena_obj_145 = &mak_ir_default_obj__a;
             } else {
                (arena_obj_145)->tag = mak_ir_tag_1;
                (arena_obj_145)->v.v1.f0 = (int32_t)(INT64_C(46));
                (arena_obj_145)->v.v1.f1 = &mak_ir_ctor_obj_0;
            }
            struct mak__a *arena_call_146 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
            if (!arena_call_146) {
                mak_ir_recover("ADT result allocation failed");
                arena_call_146 = &mak_ir_default_obj__a;
             } else {
                mak_ir_fn_6(mak_local_df_13, INT64_C(10), INT64_C(0), (struct mak__a *)(arena_call_146));
            }
            mak_ir_fn_3(arena_obj_145, arena_call_146, (struct mak__a *)(arena_call_144));
        }
        mak_ir_fn_3(arena_call_140, arena_call_144, (struct mak__a *)(out));
    }
    return;
}

static inline void mak_ir_fn_14(const void * a0, int64_t a1, int64_t a2, struct mak__f *out) {
    for (;;) {
        uint8_t mak_local___mak_inline_0_16 = (*((uint8_t *)(uintptr_t)(a0) + (intptr_t)(a1)));
        int64_t case_148 = 0;
        int32_t scrut_149 = (int32_t)(mak_local___mak_inline_0_16) >= (int32_t)(INT64_C(48));
        if ((scrut_149 == INT64_C(0))) {
            case_148 = INT64_C(0);
        }
        else if (1) {
            int64_t case_150 = 0;
            int32_t scrut_151 = (int32_t)(mak_local___mak_inline_0_16) <= (int32_t)(INT64_C(57));
            if ((scrut_151 == INT64_C(1))) {
                case_150 = INT64_C(1);
            }
            else if (1) {
                case_150 = INT64_C(0);
            }
            case_148 = case_150;
        }
        int64_t scrut_147 = case_148;
        if ((scrut_147 == INT64_C(1))) {
            const void * tailarg_152 = a0;
            int64_t tailarg_153 = (a1 + INT64_C(1));
            int64_t eval_156 = (a2 * INT64_C(10));
            uint8_t eval_157 = (*((uint8_t *)(uintptr_t)(a0) + (intptr_t)(a1)));
            int64_t eval_155 = (eval_156 + (int64_t)(eval_157));
            int64_t tailarg_154 = (eval_155 - INT64_C(48));
            a0 = tailarg_152;
            a1 = tailarg_153;
            a2 = tailarg_154;
            continue;
        }
        else if (1) {
            (out)->tag = mak_ir_tag_16;
            (out)->v.f0 = a2;
            (out)->v.f1 = a1;
        }
        return;
    }
}

static inline void mak_ir_fn_15(const void * a0, int64_t a1, struct mak__f *out) {
    mak_ir_fn_14(a0, a1, INT64_C(0), (struct mak__f *)(out));
    return;
}

static inline void mak_ir_fn_16(const void * a0, int64_t a1, int64_t a2, int64_t a3, struct mak__e *out) {
    uint8_t eval_159 = (*((uint8_t *)(uintptr_t)(a0) + (intptr_t)(a1)));
    int32_t scrut_158 = (int32_t)(eval_159) == (int32_t)(INT64_C(46));
    if ((scrut_158 == INT64_C(1))) {
        struct mak__f *arena_call_161 = (struct mak__f *)mak_ir_arena_alloc(sizeof(struct mak__f), _Alignof(struct mak__f));
        if (!arena_call_161) {
            mak_ir_recover("ADT result allocation failed");
            arena_call_161 = &mak_ir_default_obj__f;
         } else {
            mak_ir_fn_15(a0, (a1 + INT64_C(1)), (struct mak__f *)(arena_call_161));
        }
        struct mak__f * scrut_160 = arena_call_161;
        if ((scrut_160 != NULL && (scrut_160)->tag == mak_ir_tag_16)) {
            int64_t mak_local_ep_17 = (scrut_160)->v.f0;
            int64_t mak_local_eq_18 = (scrut_160)->v.f1;
            (out)->tag = mak_ir_tag_15;
            struct mak__d *arena_obj_162 = (struct mak__d *)mak_ir_arena_alloc(sizeof(struct mak__d), _Alignof(struct mak__d));
            if (!arena_obj_162) {
                mak_ir_recover("ADT object allocation failed");
                arena_obj_162 = &mak_ir_default_obj__d;
             } else {
                (arena_obj_162)->tag = mak_ir_tag_14;
                (arena_obj_162)->v.f0 = a2;
                (arena_obj_162)->v.f1 = a3;
                (arena_obj_162)->v.f2 = mak_local_ep_17;
                int64_t arg_163 = (int64_t)((*((uint8_t *)(uintptr_t)(a0) + (intptr_t)(mak_local_eq_18))));
                (arena_obj_162)->v.f3 = arg_163;
            }
            (out)->v.f0 = arena_obj_162;
            (out)->v.f1 = (mak_local_eq_18 + INT64_C(1));
        }
        else {
            *(out) = *(&mak_ir_default_obj__e);
        }
    }
    else if (1) {
        (out)->tag = mak_ir_tag_15;
        struct mak__d *arena_obj_164 = (struct mak__d *)mak_ir_arena_alloc(sizeof(struct mak__d), _Alignof(struct mak__d));
        if (!arena_obj_164) {
            mak_ir_recover("ADT object allocation failed");
            arena_obj_164 = &mak_ir_default_obj__d;
         } else {
            (arena_obj_164)->tag = mak_ir_tag_14;
            (arena_obj_164)->v.f0 = a2;
            (arena_obj_164)->v.f1 = a3;
            (arena_obj_164)->v.f2 = (int64_t)(((int64_t)UINT64_C(18446744073709551615)));
            int64_t arg_165 = (int64_t)((*((uint8_t *)(uintptr_t)(a0) + (intptr_t)(a1))));
            (arena_obj_164)->v.f3 = arg_165;
        }
        (out)->v.f0 = arena_obj_164;
        (out)->v.f1 = (a1 + INT64_C(1));
    }
    return;
}

static inline void mak_ir_fn_17(const void * a0, int64_t a1, int64_t a2, struct mak__f *out) {
    for (;;) {
        uint8_t scrut_166 = (*((uint8_t *)(uintptr_t)(a0) + (intptr_t)(a1)));
        switch (scrut_166) {
            case INT64_C(45): {
                const void * tailarg_167 = a0;
                int64_t tailarg_168 = (a1 + INT64_C(1));
                int64_t tailarg_169 = (a2 | INT64_C(1));
                a0 = tailarg_167;
                a1 = tailarg_168;
                a2 = tailarg_169;
                continue;
            }
            case INT64_C(48): {
                const void * tailarg_170 = a0;
                int64_t tailarg_171 = (a1 + INT64_C(1));
                int64_t tailarg_172 = (a2 | INT64_C(2));
                a0 = tailarg_170;
                a1 = tailarg_171;
                a2 = tailarg_172;
                continue;
            }
            case INT64_C(43): {
                const void * tailarg_173 = a0;
                int64_t tailarg_174 = (a1 + INT64_C(1));
                int64_t tailarg_175 = (a2 | INT64_C(4));
                a0 = tailarg_173;
                a1 = tailarg_174;
                a2 = tailarg_175;
                continue;
            }
            case INT64_C(32): {
                const void * tailarg_176 = a0;
                int64_t tailarg_177 = (a1 + INT64_C(1));
                int64_t tailarg_178 = (a2 | INT64_C(8));
                a0 = tailarg_176;
                a1 = tailarg_177;
                a2 = tailarg_178;
                continue;
            }
            default: {
                (out)->tag = mak_ir_tag_16;
                (out)->v.f0 = a2;
                (out)->v.f1 = a1;
                break;
            }
        }
        return;
    }
}

static inline void mak_ir_fn_18(const char * a0, int64_t a1, struct mak__c * a2, struct mak__a * a3, struct mak__a *out) {
    for (;;) {
        uint8_t scrut_179 = (*((uint8_t *)(uintptr_t)(a0) + (intptr_t)(a1)));
        switch (scrut_179) {
            case INT64_C(0): {
                *(out) = *(a3);
                break;
            }
            case INT64_C(37): {
                struct mak__e *arena_case_181 = (struct mak__e *)mak_ir_arena_alloc(sizeof(struct mak__e), _Alignof(struct mak__e));
                if (!arena_case_181) {
                    mak_ir_recover("ADT case-result allocation failed");
                    arena_case_181 = &mak_ir_default_obj__e;
                 } else {
                    struct mak__f *arena_call_183 = (struct mak__f *)mak_ir_arena_alloc(sizeof(struct mak__f), _Alignof(struct mak__f));
                    if (!arena_call_183) {
                        mak_ir_recover("ADT result allocation failed");
                        arena_call_183 = &mak_ir_default_obj__f;
                     } else {
                        mak_ir_fn_17((const void *)(a0), (a1 + INT64_C(1)), INT64_C(0), (struct mak__f *)(arena_call_183));
                    }
                    struct mak__f * scrut_182 = arena_call_183;
                    if ((scrut_182 != NULL && (scrut_182)->tag == mak_ir_tag_16)) {
                        int64_t mak_local_fg_19 = (scrut_182)->v.f0;
                        int64_t mak_local_fh_20 = (scrut_182)->v.f1;
                        uint8_t mak_local___mak_inline_1_21 = (*((uint8_t *)(uintptr_t)(a0) + (intptr_t)(mak_local_fh_20)));
                        int64_t case_185 = 0;
                        int32_t scrut_186 = (int32_t)(mak_local___mak_inline_1_21) >= (int32_t)(INT64_C(48));
                        if ((scrut_186 == INT64_C(0))) {
                            case_185 = INT64_C(0);
                        }
                        else if (1) {
                            int64_t case_187 = 0;
                            int32_t scrut_188 = (int32_t)(mak_local___mak_inline_1_21) <= (int32_t)(INT64_C(57));
                            if ((scrut_188 == INT64_C(1))) {
                                case_187 = INT64_C(1);
                            }
                            else if (1) {
                                case_187 = INT64_C(0);
                            }
                            case_185 = case_187;
                        }
                        int64_t scrut_184 = case_185;
                        if ((scrut_184 == INT64_C(1))) {
                            struct mak__f *arena_call_190 = (struct mak__f *)mak_ir_arena_alloc(sizeof(struct mak__f), _Alignof(struct mak__f));
                            if (!arena_call_190) {
                                mak_ir_recover("ADT result allocation failed");
                                arena_call_190 = &mak_ir_default_obj__f;
                             } else {
                                mak_ir_fn_15((const void *)(a0), mak_local_fh_20, (struct mak__f *)(arena_call_190));
                            }
                            struct mak__f * scrut_189 = arena_call_190;
                            if ((scrut_189 != NULL && (scrut_189)->tag == mak_ir_tag_16)) {
                                int64_t mak_local_ew_22 = (scrut_189)->v.f0;
                                int64_t mak_local_ex_23 = (scrut_189)->v.f1;
                                mak_ir_fn_16((const void *)(a0), mak_local_ex_23, mak_local_fg_19, mak_local_ew_22, (struct mak__e *)(arena_case_181));
                            }
                            else {
                                *(arena_case_181) = *(&mak_ir_default_obj__e);
                            }
                        }
                        else if (1) {
                            mak_ir_fn_16((const void *)(a0), mak_local_fh_20, mak_local_fg_19, INT64_C(0), (struct mak__e *)(arena_case_181));
                        }
                    }
                    else {
                        *(arena_case_181) = *(&mak_ir_default_obj__e);
                    }
                }
                struct mak__e * scrut_180 = arena_case_181;
                if ((scrut_180 != NULL && (scrut_180)->tag == mak_ir_tag_15)) {
                    struct mak__d * mak_local_gr_24 = (scrut_180)->v.f0;
                    int64_t mak_local_gs_25 = (scrut_180)->v.f1;
                    if ((mak_local_gr_24 != NULL && (mak_local_gr_24)->tag == mak_ir_tag_14)) {
                        int64_t mak_local_gt_26 = (mak_local_gr_24)->v.f3;
                        int32_t scrut_191 = (int32_t)(mak_local_gt_26) == (int32_t)(INT64_C(37));
                        if ((scrut_191 == INT64_C(1))) {
                            const char * tailarg_192 = a0;
                            int64_t tailarg_193 = mak_local_gs_25;
                            struct mak__c * tailarg_194 = a2;
                            struct mak__a *arena_obj_196 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
                            if (!arena_obj_196) {
                                mak_ir_recover("ADT object allocation failed");
                                arena_obj_196 = &mak_ir_default_obj__a;
                             } else {
                                (arena_obj_196)->tag = mak_ir_tag_1;
                                (arena_obj_196)->v.v1.f0 = (int32_t)(INT64_C(37));
                                (arena_obj_196)->v.v1.f1 = a3;
                            }
                            struct mak__a * tailarg_195 = arena_obj_196;
                            a0 = tailarg_192;
                            a1 = tailarg_193;
                            a2 = tailarg_194;
                            a3 = tailarg_195;
                            continue;
                        }
                        else if (1) {
                            if ((a2 != NULL && (a2)->tag == mak_ir_tag_12)) {
                                const char * tailarg_197 = a0;
                                int64_t tailarg_198 = mak_local_gs_25;
                                struct mak__c * tailarg_199 = a2;
                                struct mak__a *arena_obj_201 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
                                if (!arena_obj_201) {
                                    mak_ir_recover("ADT object allocation failed");
                                    arena_obj_201 = &mak_ir_default_obj__a;
                                 } else {
                                    (arena_obj_201)->tag = mak_ir_tag_1;
                                    (arena_obj_201)->v.v1.f0 = (int32_t)(INT64_C(63));
                                    (arena_obj_201)->v.v1.f1 = a3;
                                }
                                struct mak__a * tailarg_200 = arena_obj_201;
                                a0 = tailarg_197;
                                a1 = tailarg_198;
                                a2 = tailarg_199;
                                a3 = tailarg_200;
                                continue;
                            }
                            else if ((a2 != NULL && (a2)->tag == mak_ir_tag_13)) {
                                struct mak__b * mak_local_gu_27 = (a2)->v.v13.f0;
                                struct mak__c * mak_local_gv_28 = (a2)->v.v13.f1;
                                const char * tailarg_202 = a0;
                                int64_t tailarg_203 = mak_local_gs_25;
                                struct mak__c * tailarg_204 = mak_local_gv_28;
                                struct mak__a *arena_case_206 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
                                if (!arena_case_206) {
                                    mak_ir_recover("ADT case-result allocation failed");
                                    arena_case_206 = &mak_ir_default_obj__a;
                                 } else {
                                    if ((mak_local_gr_24 != NULL && (mak_local_gr_24)->tag == mak_ir_tag_14)) {
                                        int64_t mak_local_gh_29 = (mak_local_gr_24)->v.f0;
                                        int64_t mak_local_gi_30 = (mak_local_gr_24)->v.f1;
                                        int64_t mak_local_gj_31 = (mak_local_gr_24)->v.f2;
                                        int64_t mak_local_gk_32 = (mak_local_gr_24)->v.f3;
                                        switch (mak_local_gk_32) {
                                            case INT64_C(100): {
                                                if ((mak_local_gu_27 != NULL && (mak_local_gu_27)->tag == mak_ir_tag_2)) {
                                                    int64_t mak_local_fp_33 = (mak_local_gu_27)->v.v2.f0;
                                                    struct mak__a *arena_call_207 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
                                                    if (!arena_call_207) {
                                                        mak_ir_recover("ADT result allocation failed");
                                                        arena_call_207 = &mak_ir_default_obj__a;
                                                     } else {
                                                        mak_ir_fn_9(mak_local_gh_29, mak_local_gi_30, mak_local_fp_33, (struct mak__a *)(arena_call_207));
                                                    }
                                                    mak_ir_fn_3(arena_call_207, a3, (struct mak__a *)(arena_case_206));
                                                }
                                                else if (1) {
                                                    *(arena_case_206) = *(a3);
                                                }
                                                break;
                                            }
                                            case INT64_C(105): {
                                                if ((mak_local_gu_27 != NULL && (mak_local_gu_27)->tag == mak_ir_tag_2)) {
                                                    int64_t mak_local_fq_34 = (mak_local_gu_27)->v.v2.f0;
                                                    struct mak__a *arena_call_208 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
                                                    if (!arena_call_208) {
                                                        mak_ir_recover("ADT result allocation failed");
                                                        arena_call_208 = &mak_ir_default_obj__a;
                                                     } else {
                                                        mak_ir_fn_9(mak_local_gh_29, mak_local_gi_30, mak_local_fq_34, (struct mak__a *)(arena_call_208));
                                                    }
                                                    mak_ir_fn_3(arena_call_208, a3, (struct mak__a *)(arena_case_206));
                                                }
                                                else if (1) {
                                                    *(arena_case_206) = *(a3);
                                                }
                                                break;
                                            }
                                            case INT64_C(117): {
                                                if ((mak_local_gu_27 != NULL && (mak_local_gu_27)->tag == mak_ir_tag_3)) {
                                                    uint64_t mak_local_fr_35 = (mak_local_gu_27)->v.v3.f0;
                                                    struct mak__a *arena_call_209 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
                                                    if (!arena_call_209) {
                                                        mak_ir_recover("ADT result allocation failed");
                                                        arena_call_209 = &mak_ir_default_obj__a;
                                                     } else {
                                                        mak_ir_fn_10(mak_local_gh_29, mak_local_gi_30, INT64_C(10), INT64_C(0), mak_local_fr_35, (struct mak__a *)(arena_call_209));
                                                    }
                                                    mak_ir_fn_3(arena_call_209, a3, (struct mak__a *)(arena_case_206));
                                                }
                                                else if (1) {
                                                    *(arena_case_206) = *(a3);
                                                }
                                                break;
                                            }
                                            case INT64_C(120): {
                                                if ((mak_local_gu_27 != NULL && (mak_local_gu_27)->tag == mak_ir_tag_4)) {
                                                    uint64_t mak_local_fs_36 = (mak_local_gu_27)->v.v4.f0;
                                                    struct mak__a *arena_call_210 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
                                                    if (!arena_call_210) {
                                                        mak_ir_recover("ADT result allocation failed");
                                                        arena_call_210 = &mak_ir_default_obj__a;
                                                     } else {
                                                        mak_ir_fn_10(mak_local_gh_29, mak_local_gi_30, INT64_C(16), INT64_C(0), mak_local_fs_36, (struct mak__a *)(arena_call_210));
                                                    }
                                                    mak_ir_fn_3(arena_call_210, a3, (struct mak__a *)(arena_case_206));
                                                }
                                                else if ((mak_local_gu_27 != NULL && (mak_local_gu_27)->tag == mak_ir_tag_3)) {
                                                    uint64_t mak_local_ft_37 = (mak_local_gu_27)->v.v3.f0;
                                                    struct mak__a *arena_call_211 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
                                                    if (!arena_call_211) {
                                                        mak_ir_recover("ADT result allocation failed");
                                                        arena_call_211 = &mak_ir_default_obj__a;
                                                     } else {
                                                        mak_ir_fn_10(mak_local_gh_29, mak_local_gi_30, INT64_C(16), INT64_C(0), mak_local_ft_37, (struct mak__a *)(arena_call_211));
                                                    }
                                                    mak_ir_fn_3(arena_call_211, a3, (struct mak__a *)(arena_case_206));
                                                }
                                                else if (1) {
                                                    *(arena_case_206) = *(a3);
                                                }
                                                break;
                                            }
                                            case INT64_C(88): {
                                                if ((mak_local_gu_27 != NULL && (mak_local_gu_27)->tag == mak_ir_tag_5)) {
                                                    uint64_t mak_local_fu_38 = (mak_local_gu_27)->v.v5.f0;
                                                    struct mak__a *arena_call_212 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
                                                    if (!arena_call_212) {
                                                        mak_ir_recover("ADT result allocation failed");
                                                        arena_call_212 = &mak_ir_default_obj__a;
                                                     } else {
                                                        mak_ir_fn_10(mak_local_gh_29, mak_local_gi_30, INT64_C(16), INT64_C(1), mak_local_fu_38, (struct mak__a *)(arena_call_212));
                                                    }
                                                    mak_ir_fn_3(arena_call_212, a3, (struct mak__a *)(arena_case_206));
                                                }
                                                else if ((mak_local_gu_27 != NULL && (mak_local_gu_27)->tag == mak_ir_tag_3)) {
                                                    uint64_t mak_local_fv_39 = (mak_local_gu_27)->v.v3.f0;
                                                    struct mak__a *arena_call_213 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
                                                    if (!arena_call_213) {
                                                        mak_ir_recover("ADT result allocation failed");
                                                        arena_call_213 = &mak_ir_default_obj__a;
                                                     } else {
                                                        mak_ir_fn_10(mak_local_gh_29, mak_local_gi_30, INT64_C(16), INT64_C(1), mak_local_fv_39, (struct mak__a *)(arena_call_213));
                                                    }
                                                    mak_ir_fn_3(arena_call_213, a3, (struct mak__a *)(arena_case_206));
                                                }
                                                else if (1) {
                                                    *(arena_case_206) = *(a3);
                                                }
                                                break;
                                            }
                                            case INT64_C(111): {
                                                if ((mak_local_gu_27 != NULL && (mak_local_gu_27)->tag == mak_ir_tag_6)) {
                                                    uint64_t mak_local_fw_40 = (mak_local_gu_27)->v.v6.f0;
                                                    struct mak__a *arena_call_214 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
                                                    if (!arena_call_214) {
                                                        mak_ir_recover("ADT result allocation failed");
                                                        arena_call_214 = &mak_ir_default_obj__a;
                                                     } else {
                                                        mak_ir_fn_10(mak_local_gh_29, mak_local_gi_30, INT64_C(8), INT64_C(0), mak_local_fw_40, (struct mak__a *)(arena_call_214));
                                                    }
                                                    mak_ir_fn_3(arena_call_214, a3, (struct mak__a *)(arena_case_206));
                                                }
                                                else if ((mak_local_gu_27 != NULL && (mak_local_gu_27)->tag == mak_ir_tag_3)) {
                                                    uint64_t mak_local_fx_41 = (mak_local_gu_27)->v.v3.f0;
                                                    struct mak__a *arena_call_215 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
                                                    if (!arena_call_215) {
                                                        mak_ir_recover("ADT result allocation failed");
                                                        arena_call_215 = &mak_ir_default_obj__a;
                                                     } else {
                                                        mak_ir_fn_10(mak_local_gh_29, mak_local_gi_30, INT64_C(8), INT64_C(0), mak_local_fx_41, (struct mak__a *)(arena_call_215));
                                                    }
                                                    mak_ir_fn_3(arena_call_215, a3, (struct mak__a *)(arena_case_206));
                                                }
                                                else if (1) {
                                                    *(arena_case_206) = *(a3);
                                                }
                                                break;
                                            }
                                            case INT64_C(98): {
                                                if ((mak_local_gu_27 != NULL && (mak_local_gu_27)->tag == mak_ir_tag_7)) {
                                                    uint64_t mak_local_fy_42 = (mak_local_gu_27)->v.v7.f0;
                                                    struct mak__a *arena_call_216 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
                                                    if (!arena_call_216) {
                                                        mak_ir_recover("ADT result allocation failed");
                                                        arena_call_216 = &mak_ir_default_obj__a;
                                                     } else {
                                                        mak_ir_fn_10(mak_local_gh_29, mak_local_gi_30, INT64_C(2), INT64_C(0), mak_local_fy_42, (struct mak__a *)(arena_call_216));
                                                    }
                                                    mak_ir_fn_3(arena_call_216, a3, (struct mak__a *)(arena_case_206));
                                                }
                                                else if ((mak_local_gu_27 != NULL && (mak_local_gu_27)->tag == mak_ir_tag_3)) {
                                                    uint64_t mak_local_fz_43 = (mak_local_gu_27)->v.v3.f0;
                                                    struct mak__a *arena_call_217 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
                                                    if (!arena_call_217) {
                                                        mak_ir_recover("ADT result allocation failed");
                                                        arena_call_217 = &mak_ir_default_obj__a;
                                                     } else {
                                                        mak_ir_fn_10(mak_local_gh_29, mak_local_gi_30, INT64_C(2), INT64_C(0), mak_local_fz_43, (struct mak__a *)(arena_call_217));
                                                    }
                                                    mak_ir_fn_3(arena_call_217, a3, (struct mak__a *)(arena_case_206));
                                                }
                                                else if (1) {
                                                    *(arena_case_206) = *(a3);
                                                }
                                                break;
                                            }
                                            case INT64_C(99): {
                                                if ((mak_local_gu_27 != NULL && (mak_local_gu_27)->tag == mak_ir_tag_8)) {
                                                    int32_t mak_local_ga_44 = (mak_local_gu_27)->v.v8.f0;
                                                    (arena_case_206)->tag = mak_ir_tag_1;
                                                    (arena_case_206)->v.v1.f0 = mak_local_ga_44;
                                                    (arena_case_206)->v.v1.f1 = a3;
                                                }
                                                else if (1) {
                                                    *(arena_case_206) = *(a3);
                                                }
                                                break;
                                            }
                                            case INT64_C(115): {
                                                if ((mak_local_gu_27 != NULL && (mak_local_gu_27)->tag == mak_ir_tag_9)) {
                                                    const char * mak_local_gb_45 = (mak_local_gu_27)->v.v9.f0;
                                                    struct mak__a *letobj_218 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
                                                    if (!letobj_218) {
                                                        mak_ir_recover("ADT local allocation failed");
                                                        letobj_218 = &mak_ir_default_obj__a;
                                                     } else {
                                                        *(letobj_218) = *(&mak_ir_default_obj__a);
                                                        struct mak__a *arena_call_219 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
                                                        if (!arena_call_219) {
                                                            mak_ir_recover("ADT result allocation failed");
                                                            arena_call_219 = &mak_ir_default_obj__a;
                                                         } else {
                                                            uint64_t mak_local_cp_46 = mak_ir_ext_1((const void *)(mak_local_gb_45));
                                                            int64_t case_221 = 0;
                                                            int32_t scrut_222 = (int32_t)(mak_local_gj_31) < (int32_t)(INT64_C(0));
                                                            if ((scrut_222 == INT64_C(1))) {
                                                                case_221 = (int64_t)(mak_local_cp_46);
                                                            }
                                                            else if (1) {
                                                                int64_t case_223 = 0;
                                                                int32_t scrut_224 = (int32_t)(mak_local_gj_31) < (int32_t)(mak_local_cp_46);
                                                                if ((scrut_224 == INT64_C(1))) {
                                                                    case_223 = mak_local_gj_31;
                                                                }
                                                                else if (1) {
                                                                    case_223 = (int64_t)(mak_local_cp_46);
                                                                }
                                                                case_221 = case_223;
                                                            }
                                                            int64_t eval_220 = case_221;
                                                            mak_ir_fn_11((const void *)(mak_local_gb_45), (eval_220 - INT64_C(1)), &mak_ir_ctor_obj_0, (struct mak__a *)(arena_call_219));
                                                        }
                                                        mak_ir_fn_0(arena_call_219, &mak_ir_ctor_obj_0, (struct mak__a *)(letobj_218));
                                                    }
                                                    struct mak__a * mak_local_cv_47 = letobj_218;
                                                    int64_t mak_local_cw_48 = mak_ir_fn_1(mak_local_cv_47, INT64_C(0));
                                                    int64_t case_225 = 0;
                                                    int32_t scrut_226 = (int32_t)(mak_local_gi_30) <= (int32_t)(mak_local_cw_48);
                                                    if ((scrut_226 == INT64_C(1))) {
                                                        case_225 = INT64_C(0);
                                                    }
                                                    else if (1) {
                                                        case_225 = (mak_local_gi_30 - mak_local_cw_48);
                                                    }
                                                    int64_t mak_local_cx_49 = case_225;
                                                    struct mak__a *arena_case_227 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
                                                    if (!arena_case_227) {
                                                        mak_ir_recover("ADT case-result allocation failed");
                                                        arena_case_227 = &mak_ir_default_obj__a;
                                                     } else {
                                                        int64_t case_229 = 0;
                                                        int64_t scrut_230 = (mak_local_gh_29 & INT64_C(1));
                                                        if ((scrut_230 == INT64_C(0))) {
                                                            case_229 = INT64_C(0);
                                                        }
                                                        else if (1) {
                                                            case_229 = INT64_C(1);
                                                        }
                                                        int64_t scrut_228 = case_229;
                                                        if ((scrut_228 == INT64_C(1))) {
                                                            struct mak__a *arena_call_231 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
                                                            if (!arena_call_231) {
                                                                mak_ir_recover("ADT result allocation failed");
                                                                arena_call_231 = &mak_ir_default_obj__a;
                                                             } else {
                                                                mak_ir_fn_4(INT64_C(32), mak_local_cx_49, (struct mak__a *)(arena_call_231));
                                                            }
                                                            mak_ir_fn_3(arena_call_231, mak_local_cv_47, (struct mak__a *)(arena_case_227));
                                                        }
                                                        else if (1) {
                                                            struct mak__a *arena_call_232 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
                                                            if (!arena_call_232) {
                                                                mak_ir_recover("ADT result allocation failed");
                                                                arena_call_232 = &mak_ir_default_obj__a;
                                                             } else {
                                                                mak_ir_fn_4(INT64_C(32), mak_local_cx_49, (struct mak__a *)(arena_call_232));
                                                            }
                                                            mak_ir_fn_3(mak_local_cv_47, arena_call_232, (struct mak__a *)(arena_case_227));
                                                        }
                                                    }
                                                    mak_ir_fn_3(arena_case_227, a3, (struct mak__a *)(arena_case_206));
                                                }
                                                else if (1) {
                                                    *(arena_case_206) = *(a3);
                                                }
                                                break;
                                            }
                                            case INT64_C(112): {
                                                if ((mak_local_gu_27 != NULL && (mak_local_gu_27)->tag == mak_ir_tag_10)) {
                                                    const void * mak_local_gc_50 = (mak_local_gu_27)->v.v10.f0;
                                                    struct mak__a *arena_call_233 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
                                                    if (!arena_call_233) {
                                                        mak_ir_recover("ADT result allocation failed");
                                                        arena_call_233 = &mak_ir_default_obj__a;
                                                     } else {
                                                        struct mak__a *arena_call_234 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
                                                        if (!arena_call_234) {
                                                            mak_ir_recover("ADT result allocation failed");
                                                            arena_call_234 = &mak_ir_default_obj__a;
                                                         } else {
                                                            mak_ir_fn_6(((double)(uintptr_t)(mak_local_gc_50)), INT64_C(16), INT64_C(0), (struct mak__a *)(arena_call_234));
                                                        }
                                                        struct mak__a *arena_obj_235 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
                                                        if (!arena_obj_235) {
                                                            mak_ir_recover("ADT object allocation failed");
                                                            arena_obj_235 = &mak_ir_default_obj__a;
                                                         } else {
                                                            (arena_obj_235)->tag = mak_ir_tag_1;
                                                            (arena_obj_235)->v.v1.f0 = (int32_t)(INT64_C(120));
                                                            struct mak__a *arena_obj_236 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
                                                            if (!arena_obj_236) {
                                                                mak_ir_recover("ADT object allocation failed");
                                                                arena_obj_236 = &mak_ir_default_obj__a;
                                                             } else {
                                                                (arena_obj_236)->tag = mak_ir_tag_1;
                                                                (arena_obj_236)->v.v1.f0 = (int32_t)(INT64_C(48));
                                                                (arena_obj_236)->v.v1.f1 = &mak_ir_ctor_obj_0;
                                                            }
                                                            (arena_obj_235)->v.v1.f1 = arena_obj_236;
                                                        }
                                                        mak_ir_fn_3(arena_call_234, arena_obj_235, (struct mak__a *)(arena_call_233));
                                                    }
                                                    mak_ir_fn_3(arena_call_233, a3, (struct mak__a *)(arena_case_206));
                                                }
                                                else if (1) {
                                                    *(arena_case_206) = *(a3);
                                                }
                                                break;
                                            }
                                            case INT64_C(102): {
                                                if ((mak_local_gu_27 != NULL && (mak_local_gu_27)->tag == mak_ir_tag_11)) {
                                                    double mak_local_gd_51 = (mak_local_gu_27)->v.v11.f0;
                                                    int64_t case_237 = 0;
                                                    int32_t scrut_238 = (int32_t)(mak_local_gd_51) < (int32_t)(0.00000000000000000);
                                                    if ((scrut_238 == INT64_C(1))) {
                                                        case_237 = (int64_t)((uint64_t)(((int64_t)UINT64_C(18446744073709551615))));
                                                    }
                                                    else if (1) {
                                                        case_237 = INT64_C(0);
                                                    }
                                                    int64_t mak_local_dw_52 = mak_ir_fn_8(mak_local_gh_29, case_237);
                                                    struct mak__a *letobj_239 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
                                                    if (!letobj_239) {
                                                        mak_ir_recover("ADT local allocation failed");
                                                        letobj_239 = &mak_ir_default_obj__a;
                                                     } else {
                                                        *(letobj_239) = *(&mak_ir_default_obj__a);
                                                        int32_t scrut_240 = (int32_t)(mak_local_dw_52) == (int32_t)(INT64_C(0));
                                                        if ((scrut_240 == INT64_C(1))) {
                                                            *(letobj_239) = mak_ir_ctor_obj_0;
                                                        }
                                                        else if (1) {
                                                            (letobj_239)->tag = mak_ir_tag_1;
                                                            (letobj_239)->v.v1.f0 = (int32_t)(mak_local_dw_52);
                                                            (letobj_239)->v.v1.f1 = &mak_ir_ctor_obj_0;
                                                        }
                                                    }
                                                    struct mak__a * mak_local_ea_53 = letobj_239;
                                                    struct mak__a *arena_case_241 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
                                                    if (!arena_case_241) {
                                                        mak_ir_recover("ADT case-result allocation failed");
                                                        arena_case_241 = &mak_ir_default_obj__a;
                                                     } else {
                                                        int64_t case_243 = 0;
                                                        int64_t scrut_244 = (mak_local_gh_29 & INT64_C(1));
                                                        if ((scrut_244 == INT64_C(0))) {
                                                            case_243 = INT64_C(0);
                                                        }
                                                        else if (1) {
                                                            case_243 = INT64_C(1);
                                                        }
                                                        int64_t scrut_242 = case_243;
                                                        if ((scrut_242 == INT64_C(1))) {
                                                            struct mak__a *arena_call_245 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
                                                            if (!arena_call_245) {
                                                                mak_ir_recover("ADT result allocation failed");
                                                                arena_call_245 = &mak_ir_default_obj__a;
                                                             } else {
                                                                int64_t case_246 = 0;
                                                                struct mak__a *arena_call_250 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
                                                                if (!arena_call_250) {
                                                                    mak_ir_recover("ADT result allocation failed");
                                                                    arena_call_250 = &mak_ir_default_obj__a;
                                                                 } else {
                                                                    int64_t case_251 = 0;
                                                                    int32_t scrut_252 = (int32_t)(mak_local_gj_31) < (int32_t)(INT64_C(0));
                                                                    if ((scrut_252 == INT64_C(1))) {
                                                                        case_251 = INT64_C(6);
                                                                    }
                                                                    else if (1) {
                                                                        case_251 = mak_local_gj_31;
                                                                    }
                                                                    mak_ir_fn_13(mak_ir_ext_4(mak_local_gd_51), case_251, (struct mak__a *)(arena_call_250));
                                                                }
                                                                int64_t eval_249 = mak_ir_fn_1(arena_call_250, INT64_C(0));
                                                                int64_t case_254 = 0;
                                                                int32_t scrut_255 = (int32_t)(mak_local_dw_52) == (int32_t)(INT64_C(0));
                                                                if ((scrut_255 == INT64_C(1))) {
                                                                    case_254 = INT64_C(0);
                                                                }
                                                                else if (1) {
                                                                    case_254 = INT64_C(1);
                                                                }
                                                                int64_t eval_253 = case_254;
                                                                int64_t eval_248 = (eval_249 + eval_253);
                                                                int32_t scrut_247 = (int32_t)(mak_local_gi_30) <= (int32_t)(eval_248);
                                                                if ((scrut_247 == INT64_C(1))) {
                                                                    case_246 = INT64_C(0);
                                                                }
                                                                else if (1) {
                                                                    struct mak__a *arena_call_258 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
                                                                    if (!arena_call_258) {
                                                                        mak_ir_recover("ADT result allocation failed");
                                                                        arena_call_258 = &mak_ir_default_obj__a;
                                                                     } else {
                                                                        int64_t case_259 = 0;
                                                                        int32_t scrut_260 = (int32_t)(mak_local_gj_31) < (int32_t)(INT64_C(0));
                                                                        if ((scrut_260 == INT64_C(1))) {
                                                                            case_259 = INT64_C(6);
                                                                        }
                                                                        else if (1) {
                                                                            case_259 = mak_local_gj_31;
                                                                        }
                                                                        mak_ir_fn_13(mak_ir_ext_4(mak_local_gd_51), case_259, (struct mak__a *)(arena_call_258));
                                                                    }
                                                                    int64_t eval_257 = mak_ir_fn_1(arena_call_258, INT64_C(0));
                                                                    int64_t case_262 = 0;
                                                                    int32_t scrut_263 = (int32_t)(mak_local_dw_52) == (int32_t)(INT64_C(0));
                                                                    if ((scrut_263 == INT64_C(1))) {
                                                                        case_262 = INT64_C(0);
                                                                    }
                                                                    else if (1) {
                                                                        case_262 = INT64_C(1);
                                                                    }
                                                                    int64_t eval_261 = case_262;
                                                                    int64_t eval_256 = (eval_257 + eval_261);
                                                                    case_246 = (mak_local_gi_30 - eval_256);
                                                                }
                                                                mak_ir_fn_4(INT64_C(32), case_246, (struct mak__a *)(arena_call_245));
                                                            }
                                                            struct mak__a *arena_call_264 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
                                                            if (!arena_call_264) {
                                                                mak_ir_recover("ADT result allocation failed");
                                                                arena_call_264 = &mak_ir_default_obj__a;
                                                             } else {
                                                                struct mak__a *arena_call_265 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
                                                                if (!arena_call_265) {
                                                                    mak_ir_recover("ADT result allocation failed");
                                                                    arena_call_265 = &mak_ir_default_obj__a;
                                                                 } else {
                                                                    int64_t case_266 = 0;
                                                                    int32_t scrut_267 = (int32_t)(mak_local_gj_31) < (int32_t)(INT64_C(0));
                                                                    if ((scrut_267 == INT64_C(1))) {
                                                                        case_266 = INT64_C(6);
                                                                    }
                                                                    else if (1) {
                                                                        case_266 = mak_local_gj_31;
                                                                    }
                                                                    mak_ir_fn_13(mak_ir_ext_4(mak_local_gd_51), case_266, (struct mak__a *)(arena_call_265));
                                                                }
                                                                mak_ir_fn_3(arena_call_265, mak_local_ea_53, (struct mak__a *)(arena_call_264));
                                                            }
                                                            mak_ir_fn_3(arena_call_245, arena_call_264, (struct mak__a *)(arena_case_241));
                                                        }
                                                        else if (1) {
                                                            int64_t case_269 = 0;
                                                            int64_t scrut_270 = (mak_local_gh_29 & INT64_C(2));
                                                            if ((scrut_270 == INT64_C(0))) {
                                                                case_269 = INT64_C(0);
                                                            }
                                                            else if (1) {
                                                                case_269 = INT64_C(1);
                                                            }
                                                            int64_t scrut_268 = case_269;
                                                            if ((scrut_268 == INT64_C(1))) {
                                                                struct mak__a *arena_call_271 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
                                                                if (!arena_call_271) {
                                                                    mak_ir_recover("ADT result allocation failed");
                                                                    arena_call_271 = &mak_ir_default_obj__a;
                                                                 } else {
                                                                    int64_t case_272 = 0;
                                                                    int32_t scrut_273 = (int32_t)(mak_local_gj_31) < (int32_t)(INT64_C(0));
                                                                    if ((scrut_273 == INT64_C(1))) {
                                                                        case_272 = INT64_C(6);
                                                                    }
                                                                    else if (1) {
                                                                        case_272 = mak_local_gj_31;
                                                                    }
                                                                    mak_ir_fn_13(mak_ir_ext_4(mak_local_gd_51), case_272, (struct mak__a *)(arena_call_271));
                                                                }
                                                                struct mak__a *arena_call_274 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
                                                                if (!arena_call_274) {
                                                                    mak_ir_recover("ADT result allocation failed");
                                                                    arena_call_274 = &mak_ir_default_obj__a;
                                                                 } else {
                                                                    struct mak__a *arena_call_275 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
                                                                    if (!arena_call_275) {
                                                                        mak_ir_recover("ADT result allocation failed");
                                                                        arena_call_275 = &mak_ir_default_obj__a;
                                                                     } else {
                                                                        int64_t case_276 = 0;
                                                                        struct mak__a *arena_call_280 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
                                                                        if (!arena_call_280) {
                                                                            mak_ir_recover("ADT result allocation failed");
                                                                            arena_call_280 = &mak_ir_default_obj__a;
                                                                         } else {
                                                                            int64_t case_281 = 0;
                                                                            int32_t scrut_282 = (int32_t)(mak_local_gj_31) < (int32_t)(INT64_C(0));
                                                                            if ((scrut_282 == INT64_C(1))) {
                                                                                case_281 = INT64_C(6);
                                                                            }
                                                                            else if (1) {
                                                                                case_281 = mak_local_gj_31;
                                                                            }
                                                                            mak_ir_fn_13(mak_ir_ext_4(mak_local_gd_51), case_281, (struct mak__a *)(arena_call_280));
                                                                        }
                                                                        int64_t eval_279 = mak_ir_fn_1(arena_call_280, INT64_C(0));
                                                                        int64_t case_284 = 0;
                                                                        int32_t scrut_285 = (int32_t)(mak_local_dw_52) == (int32_t)(INT64_C(0));
                                                                        if ((scrut_285 == INT64_C(1))) {
                                                                            case_284 = INT64_C(0);
                                                                        }
                                                                        else if (1) {
                                                                            case_284 = INT64_C(1);
                                                                        }
                                                                        int64_t eval_283 = case_284;
                                                                        int64_t eval_278 = (eval_279 + eval_283);
                                                                        int32_t scrut_277 = (int32_t)(mak_local_gi_30) <= (int32_t)(eval_278);
                                                                        if ((scrut_277 == INT64_C(1))) {
                                                                            case_276 = INT64_C(0);
                                                                        }
                                                                        else if (1) {
                                                                            struct mak__a *arena_call_288 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
                                                                            if (!arena_call_288) {
                                                                                mak_ir_recover("ADT result allocation failed");
                                                                                arena_call_288 = &mak_ir_default_obj__a;
                                                                             } else {
                                                                                int64_t case_289 = 0;
                                                                                int32_t scrut_290 = (int32_t)(mak_local_gj_31) < (int32_t)(INT64_C(0));
                                                                                if ((scrut_290 == INT64_C(1))) {
                                                                                    case_289 = INT64_C(6);
                                                                                }
                                                                                else if (1) {
                                                                                    case_289 = mak_local_gj_31;
                                                                                }
                                                                                mak_ir_fn_13(mak_ir_ext_4(mak_local_gd_51), case_289, (struct mak__a *)(arena_call_288));
                                                                            }
                                                                            int64_t eval_287 = mak_ir_fn_1(arena_call_288, INT64_C(0));
                                                                            int64_t case_292 = 0;
                                                                            int32_t scrut_293 = (int32_t)(mak_local_dw_52) == (int32_t)(INT64_C(0));
                                                                            if ((scrut_293 == INT64_C(1))) {
                                                                                case_292 = INT64_C(0);
                                                                            }
                                                                            else if (1) {
                                                                                case_292 = INT64_C(1);
                                                                            }
                                                                            int64_t eval_291 = case_292;
                                                                            int64_t eval_286 = (eval_287 + eval_291);
                                                                            case_276 = (mak_local_gi_30 - eval_286);
                                                                        }
                                                                        mak_ir_fn_4(INT64_C(48), case_276, (struct mak__a *)(arena_call_275));
                                                                    }
                                                                    mak_ir_fn_3(arena_call_275, mak_local_ea_53, (struct mak__a *)(arena_call_274));
                                                                }
                                                                mak_ir_fn_3(arena_call_271, arena_call_274, (struct mak__a *)(arena_case_241));
                                                            }
                                                            else if (1) {
                                                                struct mak__a *arena_call_294 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
                                                                if (!arena_call_294) {
                                                                    mak_ir_recover("ADT result allocation failed");
                                                                    arena_call_294 = &mak_ir_default_obj__a;
                                                                 } else {
                                                                    int64_t case_295 = 0;
                                                                    int32_t scrut_296 = (int32_t)(mak_local_gj_31) < (int32_t)(INT64_C(0));
                                                                    if ((scrut_296 == INT64_C(1))) {
                                                                        case_295 = INT64_C(6);
                                                                    }
                                                                    else if (1) {
                                                                        case_295 = mak_local_gj_31;
                                                                    }
                                                                    mak_ir_fn_13(mak_ir_ext_4(mak_local_gd_51), case_295, (struct mak__a *)(arena_call_294));
                                                                }
                                                                struct mak__a *arena_call_297 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
                                                                if (!arena_call_297) {
                                                                    mak_ir_recover("ADT result allocation failed");
                                                                    arena_call_297 = &mak_ir_default_obj__a;
                                                                 } else {
                                                                    struct mak__a *arena_call_298 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
                                                                    if (!arena_call_298) {
                                                                        mak_ir_recover("ADT result allocation failed");
                                                                        arena_call_298 = &mak_ir_default_obj__a;
                                                                     } else {
                                                                        int64_t case_299 = 0;
                                                                        struct mak__a *arena_call_303 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
                                                                        if (!arena_call_303) {
                                                                            mak_ir_recover("ADT result allocation failed");
                                                                            arena_call_303 = &mak_ir_default_obj__a;
                                                                         } else {
                                                                            int64_t case_304 = 0;
                                                                            int32_t scrut_305 = (int32_t)(mak_local_gj_31) < (int32_t)(INT64_C(0));
                                                                            if ((scrut_305 == INT64_C(1))) {
                                                                                case_304 = INT64_C(6);
                                                                            }
                                                                            else if (1) {
                                                                                case_304 = mak_local_gj_31;
                                                                            }
                                                                            mak_ir_fn_13(mak_ir_ext_4(mak_local_gd_51), case_304, (struct mak__a *)(arena_call_303));
                                                                        }
                                                                        int64_t eval_302 = mak_ir_fn_1(arena_call_303, INT64_C(0));
                                                                        int64_t case_307 = 0;
                                                                        int32_t scrut_308 = (int32_t)(mak_local_dw_52) == (int32_t)(INT64_C(0));
                                                                        if ((scrut_308 == INT64_C(1))) {
                                                                            case_307 = INT64_C(0);
                                                                        }
                                                                        else if (1) {
                                                                            case_307 = INT64_C(1);
                                                                        }
                                                                        int64_t eval_306 = case_307;
                                                                        int64_t eval_301 = (eval_302 + eval_306);
                                                                        int32_t scrut_300 = (int32_t)(mak_local_gi_30) <= (int32_t)(eval_301);
                                                                        if ((scrut_300 == INT64_C(1))) {
                                                                            case_299 = INT64_C(0);
                                                                        }
                                                                        else if (1) {
                                                                            struct mak__a *arena_call_311 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
                                                                            if (!arena_call_311) {
                                                                                mak_ir_recover("ADT result allocation failed");
                                                                                arena_call_311 = &mak_ir_default_obj__a;
                                                                             } else {
                                                                                int64_t case_312 = 0;
                                                                                int32_t scrut_313 = (int32_t)(mak_local_gj_31) < (int32_t)(INT64_C(0));
                                                                                if ((scrut_313 == INT64_C(1))) {
                                                                                    case_312 = INT64_C(6);
                                                                                }
                                                                                else if (1) {
                                                                                    case_312 = mak_local_gj_31;
                                                                                }
                                                                                mak_ir_fn_13(mak_ir_ext_4(mak_local_gd_51), case_312, (struct mak__a *)(arena_call_311));
                                                                            }
                                                                            int64_t eval_310 = mak_ir_fn_1(arena_call_311, INT64_C(0));
                                                                            int64_t case_315 = 0;
                                                                            int32_t scrut_316 = (int32_t)(mak_local_dw_52) == (int32_t)(INT64_C(0));
                                                                            if ((scrut_316 == INT64_C(1))) {
                                                                                case_315 = INT64_C(0);
                                                                            }
                                                                            else if (1) {
                                                                                case_315 = INT64_C(1);
                                                                            }
                                                                            int64_t eval_314 = case_315;
                                                                            int64_t eval_309 = (eval_310 + eval_314);
                                                                            case_299 = (mak_local_gi_30 - eval_309);
                                                                        }
                                                                        mak_ir_fn_4(INT64_C(32), case_299, (struct mak__a *)(arena_call_298));
                                                                    }
                                                                    mak_ir_fn_3(mak_local_ea_53, arena_call_298, (struct mak__a *)(arena_call_297));
                                                                }
                                                                mak_ir_fn_3(arena_call_294, arena_call_297, (struct mak__a *)(arena_case_241));
                                                            }
                                                        }
                                                    }
                                                    mak_ir_fn_3(arena_case_241, a3, (struct mak__a *)(arena_case_206));
                                                }
                                                else if (1) {
                                                    *(arena_case_206) = *(a3);
                                                }
                                                break;
                                            }
                                            default: {
                                                *(arena_case_206) = *(a3);
                                                break;
                                            }
                                        }
                                    }
                                    else {
                                        *(arena_case_206) = *(&mak_ir_default_obj__a);
                                    }
                                }
                                struct mak__a * tailarg_205 = arena_case_206;
                                a0 = tailarg_202;
                                a1 = tailarg_203;
                                a2 = tailarg_204;
                                a3 = tailarg_205;
                                continue;
                            }
                            else {
                                *(out) = *(&mak_ir_default_obj__a);
                            }
                        }
                    }
                    else {
                        *(out) = *(&mak_ir_default_obj__a);
                    }
                }
                else {
                    *(out) = *(&mak_ir_default_obj__a);
                }
                break;
            }
            default: {
                const char * tailarg_317 = a0;
                int64_t tailarg_318 = (a1 + INT64_C(1));
                struct mak__c * tailarg_319 = a2;
                struct mak__a *arena_obj_321 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
                if (!arena_obj_321) {
                    mak_ir_recover("ADT object allocation failed");
                    arena_obj_321 = &mak_ir_default_obj__a;
                 } else {
                    (arena_obj_321)->tag = mak_ir_tag_1;
                    int32_t arg_322 = (int32_t)((*((uint8_t *)(uintptr_t)(a0) + (intptr_t)(a1))));
                    (arena_obj_321)->v.v1.f0 = arg_322;
                    (arena_obj_321)->v.v1.f1 = a3;
                }
                struct mak__a * tailarg_320 = arena_obj_321;
                a0 = tailarg_317;
                a1 = tailarg_318;
                a2 = tailarg_319;
                a3 = tailarg_320;
                continue;
            }
        }
        return;
    }
}

static inline int64_t mak_ir_fn_19(const char * a0, struct mak__c * a1) {
    struct mak__a *arena_call_323 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
    if (!arena_call_323) {
        mak_ir_recover("ADT result allocation failed");
        arena_call_323 = &mak_ir_default_obj__a;
     } else {
        struct mak__a *arena_obj_324 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
        if (!arena_obj_324) {
            mak_ir_recover("ADT object allocation failed");
            arena_obj_324 = &mak_ir_default_obj__a;
         } else {
            (arena_obj_324)->tag = mak_ir_tag_1;
            (arena_obj_324)->v.v1.f0 = (int32_t)(INT64_C(10));
            struct mak__a *arena_call_325 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
            if (!arena_call_325) {
                mak_ir_recover("ADT result allocation failed");
                arena_call_325 = &mak_ir_default_obj__a;
             } else {
                mak_ir_fn_18(a0, INT64_C(0), a1, &mak_ir_ctor_obj_0, (struct mak__a *)(arena_call_325));
            }
            (arena_obj_324)->v.v1.f1 = arena_call_325;
        }
        mak_ir_fn_0(arena_obj_324, &mak_ir_ctor_obj_0, (struct mak__a *)(arena_call_323));
    }
    return mak_ir_fn_2(arena_call_323);
}

static inline double mak_ir_fn_20(int64_t a0) {
    return mak_ir_fn_12(a0, 1.00000000000000000);
}

static inline int64_t mak_ir_fn_21(const void * a0, int64_t a1) {
    struct mak__f *arena_call_327 = (struct mak__f *)mak_ir_arena_alloc(sizeof(struct mak__f), _Alignof(struct mak__f));
    if (!arena_call_327) {
        mak_ir_recover("ADT result allocation failed");
        arena_call_327 = &mak_ir_default_obj__f;
     } else {
        mak_ir_fn_17(a0, a1, INT64_C(0), (struct mak__f *)(arena_call_327));
    }
    struct mak__f * scrut_326 = arena_call_327;
    if ((scrut_326 != NULL && (scrut_326)->tag == mak_ir_tag_16)) {
        int64_t mak_local_fg_54 = (scrut_326)->v.f0;
        int64_t mak_local_fh_55 = (scrut_326)->v.f1;
        uint8_t mak_local___mak_inline_1_56 = (*((uint8_t *)(uintptr_t)(a0) + (intptr_t)(mak_local_fh_55)));
        int64_t case_329 = 0;
        int32_t scrut_330 = (int32_t)(mak_local___mak_inline_1_56) >= (int32_t)(INT64_C(48));
        if ((scrut_330 == INT64_C(0))) {
            case_329 = INT64_C(0);
        }
        else if (1) {
            int64_t case_331 = 0;
            int32_t scrut_332 = (int32_t)(mak_local___mak_inline_1_56) <= (int32_t)(INT64_C(57));
            if ((scrut_332 == INT64_C(1))) {
                case_331 = INT64_C(1);
            }
            else if (1) {
                case_331 = INT64_C(0);
            }
            case_329 = case_331;
        }
        int64_t scrut_328 = case_329;
        if ((scrut_328 == INT64_C(1))) {
            struct mak__f *arena_call_334 = (struct mak__f *)mak_ir_arena_alloc(sizeof(struct mak__f), _Alignof(struct mak__f));
            if (!arena_call_334) {
                mak_ir_recover("ADT result allocation failed");
                arena_call_334 = &mak_ir_default_obj__f;
             } else {
                mak_ir_fn_15(a0, mak_local_fh_55, (struct mak__f *)(arena_call_334));
            }
            struct mak__f * scrut_333 = arena_call_334;
            if ((scrut_333 != NULL && (scrut_333)->tag == mak_ir_tag_16)) {
                int64_t mak_local_ew_57 = (scrut_333)->v.f0;
                int64_t mak_local_ex_58 = (scrut_333)->v.f1;
                struct mak__e *arena_call_335 = (struct mak__e *)mak_ir_arena_alloc(sizeof(struct mak__e), _Alignof(struct mak__e));
                if (!arena_call_335) {
                    mak_ir_recover("ADT result allocation failed");
                    arena_call_335 = &mak_ir_default_obj__e;
                 } else {
                    mak_ir_fn_16(a0, mak_local_ex_58, mak_local_fg_54, mak_local_ew_57, (struct mak__e *)(arena_call_335));
                }
                return (int64_t)((uintptr_t)(arena_call_335));
            }
            else {
                mak_ir_recover("case had no matching arm");
                return 0;
            }
        }
        else if (1) {
            struct mak__e *arena_call_336 = (struct mak__e *)mak_ir_arena_alloc(sizeof(struct mak__e), _Alignof(struct mak__e));
            if (!arena_call_336) {
                mak_ir_recover("ADT result allocation failed");
                arena_call_336 = &mak_ir_default_obj__e;
             } else {
                mak_ir_fn_16(a0, mak_local_fh_55, mak_local_fg_54, INT64_C(0), (struct mak__e *)(arena_call_336));
            }
            return (int64_t)((uintptr_t)(arena_call_336));
        }
    }
    else {
        mak_ir_recover("case had no matching arm");
        return 0;
    }
}

static inline void mak_ir_fn_22(const void * a0, int64_t a1, int64_t a2, struct mak__e *out) {
    uint8_t mak_local___mak_inline_1_59 = (*((uint8_t *)(uintptr_t)(a0) + (intptr_t)(a1)));
    int64_t case_338 = 0;
    int32_t scrut_339 = (int32_t)(mak_local___mak_inline_1_59) >= (int32_t)(INT64_C(48));
    if ((scrut_339 == INT64_C(0))) {
        case_338 = INT64_C(0);
    }
    else if (1) {
        int64_t case_340 = 0;
        int32_t scrut_341 = (int32_t)(mak_local___mak_inline_1_59) <= (int32_t)(INT64_C(57));
        if ((scrut_341 == INT64_C(1))) {
            case_340 = INT64_C(1);
        }
        else if (1) {
            case_340 = INT64_C(0);
        }
        case_338 = case_340;
    }
    int64_t scrut_337 = case_338;
    if ((scrut_337 == INT64_C(1))) {
        struct mak__f *arena_call_343 = (struct mak__f *)mak_ir_arena_alloc(sizeof(struct mak__f), _Alignof(struct mak__f));
        if (!arena_call_343) {
            mak_ir_recover("ADT result allocation failed");
            arena_call_343 = &mak_ir_default_obj__f;
         } else {
            mak_ir_fn_15(a0, a1, (struct mak__f *)(arena_call_343));
        }
        struct mak__f * scrut_342 = arena_call_343;
        if ((scrut_342 != NULL && (scrut_342)->tag == mak_ir_tag_16)) {
            int64_t mak_local_ew_60 = (scrut_342)->v.f0;
            int64_t mak_local_ex_61 = (scrut_342)->v.f1;
            mak_ir_fn_16(a0, mak_local_ex_61, a2, mak_local_ew_60, (struct mak__e *)(out));
        }
        else {
            *(out) = *(&mak_ir_default_obj__e);
        }
    }
    else if (1) {
        mak_ir_fn_16(a0, a1, a2, INT64_C(0), (struct mak__e *)(out));
    }
    return;
}

static inline int64_t mak_ir_fn_23(struct mak__d * a0, int64_t a1, int64_t a2) {
    if ((a0 != NULL && (a0)->tag == mak_ir_tag_14)) {
        int64_t mak_local_gh_62 = (a0)->v.f0;
        int64_t mak_local_gi_63 = (a0)->v.f1;
        int64_t mak_local_gj_64 = (a0)->v.f2;
        int64_t mak_local_gk_65 = (a0)->v.f3;
        switch (mak_local_gk_65) {
            case INT64_C(100): {
                if (0) {
                    int64_t mak_local_fp_66 = ((struct mak__b *)mak_ir_adt_recover_ptr((const void *)&mak_ir_default_obj__b, "integer used as ADT"))->v.v2.f0;
                    struct mak__a *arena_call_344 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
                    if (!arena_call_344) {
                        mak_ir_recover("ADT result allocation failed");
                        arena_call_344 = &mak_ir_default_obj__a;
                     } else {
                        struct mak__a *arena_call_345 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
                        if (!arena_call_345) {
                            mak_ir_recover("ADT result allocation failed");
                            arena_call_345 = &mak_ir_default_obj__a;
                         } else {
                            mak_ir_fn_9(mak_local_gh_62, mak_local_gi_63, mak_local_fp_66, (struct mak__a *)(arena_call_345));
                        }
                        mak_ir_fn_3(arena_call_345, (struct mak__a *)mak_ir_adt_recover_ptr((const void *)&mak_ir_default_obj__a, "integer used as ADT"), (struct mak__a *)(arena_call_344));
                    }
                    return (int64_t)((uintptr_t)(arena_call_344));
                }
                else if (1) {
                    return a2;
                }
            }
            case INT64_C(105): {
                if (0) {
                    int64_t mak_local_fq_67 = ((struct mak__b *)mak_ir_adt_recover_ptr((const void *)&mak_ir_default_obj__b, "integer used as ADT"))->v.v2.f0;
                    struct mak__a *arena_call_346 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
                    if (!arena_call_346) {
                        mak_ir_recover("ADT result allocation failed");
                        arena_call_346 = &mak_ir_default_obj__a;
                     } else {
                        struct mak__a *arena_call_347 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
                        if (!arena_call_347) {
                            mak_ir_recover("ADT result allocation failed");
                            arena_call_347 = &mak_ir_default_obj__a;
                         } else {
                            mak_ir_fn_9(mak_local_gh_62, mak_local_gi_63, mak_local_fq_67, (struct mak__a *)(arena_call_347));
                        }
                        mak_ir_fn_3(arena_call_347, (struct mak__a *)mak_ir_adt_recover_ptr((const void *)&mak_ir_default_obj__a, "integer used as ADT"), (struct mak__a *)(arena_call_346));
                    }
                    return (int64_t)((uintptr_t)(arena_call_346));
                }
                else if (1) {
                    return a2;
                }
            }
            case INT64_C(117): {
                if (0) {
                    uint64_t mak_local_fr_68 = ((struct mak__b *)mak_ir_adt_recover_ptr((const void *)&mak_ir_default_obj__b, "integer used as ADT"))->v.v3.f0;
                    struct mak__a *arena_call_348 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
                    if (!arena_call_348) {
                        mak_ir_recover("ADT result allocation failed");
                        arena_call_348 = &mak_ir_default_obj__a;
                     } else {
                        struct mak__a *arena_call_349 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
                        if (!arena_call_349) {
                            mak_ir_recover("ADT result allocation failed");
                            arena_call_349 = &mak_ir_default_obj__a;
                         } else {
                            mak_ir_fn_10(mak_local_gh_62, mak_local_gi_63, INT64_C(10), INT64_C(0), mak_local_fr_68, (struct mak__a *)(arena_call_349));
                        }
                        mak_ir_fn_3(arena_call_349, (struct mak__a *)mak_ir_adt_recover_ptr((const void *)&mak_ir_default_obj__a, "integer used as ADT"), (struct mak__a *)(arena_call_348));
                    }
                    return (int64_t)((uintptr_t)(arena_call_348));
                }
                else if (1) {
                    return a2;
                }
            }
            case INT64_C(120): {
                if (0) {
                    uint64_t mak_local_fs_69 = ((struct mak__b *)mak_ir_adt_recover_ptr((const void *)&mak_ir_default_obj__b, "integer used as ADT"))->v.v4.f0;
                    struct mak__a *arena_call_350 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
                    if (!arena_call_350) {
                        mak_ir_recover("ADT result allocation failed");
                        arena_call_350 = &mak_ir_default_obj__a;
                     } else {
                        struct mak__a *arena_call_351 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
                        if (!arena_call_351) {
                            mak_ir_recover("ADT result allocation failed");
                            arena_call_351 = &mak_ir_default_obj__a;
                         } else {
                            mak_ir_fn_10(mak_local_gh_62, mak_local_gi_63, INT64_C(16), INT64_C(0), mak_local_fs_69, (struct mak__a *)(arena_call_351));
                        }
                        mak_ir_fn_3(arena_call_351, (struct mak__a *)mak_ir_adt_recover_ptr((const void *)&mak_ir_default_obj__a, "integer used as ADT"), (struct mak__a *)(arena_call_350));
                    }
                    return (int64_t)((uintptr_t)(arena_call_350));
                }
                else if (0) {
                    uint64_t mak_local_ft_70 = ((struct mak__b *)mak_ir_adt_recover_ptr((const void *)&mak_ir_default_obj__b, "integer used as ADT"))->v.v3.f0;
                    struct mak__a *arena_call_352 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
                    if (!arena_call_352) {
                        mak_ir_recover("ADT result allocation failed");
                        arena_call_352 = &mak_ir_default_obj__a;
                     } else {
                        struct mak__a *arena_call_353 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
                        if (!arena_call_353) {
                            mak_ir_recover("ADT result allocation failed");
                            arena_call_353 = &mak_ir_default_obj__a;
                         } else {
                            mak_ir_fn_10(mak_local_gh_62, mak_local_gi_63, INT64_C(16), INT64_C(0), mak_local_ft_70, (struct mak__a *)(arena_call_353));
                        }
                        mak_ir_fn_3(arena_call_353, (struct mak__a *)mak_ir_adt_recover_ptr((const void *)&mak_ir_default_obj__a, "integer used as ADT"), (struct mak__a *)(arena_call_352));
                    }
                    return (int64_t)((uintptr_t)(arena_call_352));
                }
                else if (1) {
                    return a2;
                }
            }
            case INT64_C(88): {
                if (0) {
                    uint64_t mak_local_fu_71 = ((struct mak__b *)mak_ir_adt_recover_ptr((const void *)&mak_ir_default_obj__b, "integer used as ADT"))->v.v5.f0;
                    struct mak__a *arena_call_354 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
                    if (!arena_call_354) {
                        mak_ir_recover("ADT result allocation failed");
                        arena_call_354 = &mak_ir_default_obj__a;
                     } else {
                        struct mak__a *arena_call_355 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
                        if (!arena_call_355) {
                            mak_ir_recover("ADT result allocation failed");
                            arena_call_355 = &mak_ir_default_obj__a;
                         } else {
                            mak_ir_fn_10(mak_local_gh_62, mak_local_gi_63, INT64_C(16), INT64_C(1), mak_local_fu_71, (struct mak__a *)(arena_call_355));
                        }
                        mak_ir_fn_3(arena_call_355, (struct mak__a *)mak_ir_adt_recover_ptr((const void *)&mak_ir_default_obj__a, "integer used as ADT"), (struct mak__a *)(arena_call_354));
                    }
                    return (int64_t)((uintptr_t)(arena_call_354));
                }
                else if (0) {
                    uint64_t mak_local_fv_72 = ((struct mak__b *)mak_ir_adt_recover_ptr((const void *)&mak_ir_default_obj__b, "integer used as ADT"))->v.v3.f0;
                    struct mak__a *arena_call_356 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
                    if (!arena_call_356) {
                        mak_ir_recover("ADT result allocation failed");
                        arena_call_356 = &mak_ir_default_obj__a;
                     } else {
                        struct mak__a *arena_call_357 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
                        if (!arena_call_357) {
                            mak_ir_recover("ADT result allocation failed");
                            arena_call_357 = &mak_ir_default_obj__a;
                         } else {
                            mak_ir_fn_10(mak_local_gh_62, mak_local_gi_63, INT64_C(16), INT64_C(1), mak_local_fv_72, (struct mak__a *)(arena_call_357));
                        }
                        mak_ir_fn_3(arena_call_357, (struct mak__a *)mak_ir_adt_recover_ptr((const void *)&mak_ir_default_obj__a, "integer used as ADT"), (struct mak__a *)(arena_call_356));
                    }
                    return (int64_t)((uintptr_t)(arena_call_356));
                }
                else if (1) {
                    return a2;
                }
            }
            case INT64_C(111): {
                if (0) {
                    uint64_t mak_local_fw_73 = ((struct mak__b *)mak_ir_adt_recover_ptr((const void *)&mak_ir_default_obj__b, "integer used as ADT"))->v.v6.f0;
                    struct mak__a *arena_call_358 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
                    if (!arena_call_358) {
                        mak_ir_recover("ADT result allocation failed");
                        arena_call_358 = &mak_ir_default_obj__a;
                     } else {
                        struct mak__a *arena_call_359 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
                        if (!arena_call_359) {
                            mak_ir_recover("ADT result allocation failed");
                            arena_call_359 = &mak_ir_default_obj__a;
                         } else {
                            mak_ir_fn_10(mak_local_gh_62, mak_local_gi_63, INT64_C(8), INT64_C(0), mak_local_fw_73, (struct mak__a *)(arena_call_359));
                        }
                        mak_ir_fn_3(arena_call_359, (struct mak__a *)mak_ir_adt_recover_ptr((const void *)&mak_ir_default_obj__a, "integer used as ADT"), (struct mak__a *)(arena_call_358));
                    }
                    return (int64_t)((uintptr_t)(arena_call_358));
                }
                else if (0) {
                    uint64_t mak_local_fx_74 = ((struct mak__b *)mak_ir_adt_recover_ptr((const void *)&mak_ir_default_obj__b, "integer used as ADT"))->v.v3.f0;
                    struct mak__a *arena_call_360 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
                    if (!arena_call_360) {
                        mak_ir_recover("ADT result allocation failed");
                        arena_call_360 = &mak_ir_default_obj__a;
                     } else {
                        struct mak__a *arena_call_361 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
                        if (!arena_call_361) {
                            mak_ir_recover("ADT result allocation failed");
                            arena_call_361 = &mak_ir_default_obj__a;
                         } else {
                            mak_ir_fn_10(mak_local_gh_62, mak_local_gi_63, INT64_C(8), INT64_C(0), mak_local_fx_74, (struct mak__a *)(arena_call_361));
                        }
                        mak_ir_fn_3(arena_call_361, (struct mak__a *)mak_ir_adt_recover_ptr((const void *)&mak_ir_default_obj__a, "integer used as ADT"), (struct mak__a *)(arena_call_360));
                    }
                    return (int64_t)((uintptr_t)(arena_call_360));
                }
                else if (1) {
                    return a2;
                }
            }
            case INT64_C(98): {
                if (0) {
                    uint64_t mak_local_fy_75 = ((struct mak__b *)mak_ir_adt_recover_ptr((const void *)&mak_ir_default_obj__b, "integer used as ADT"))->v.v7.f0;
                    struct mak__a *arena_call_362 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
                    if (!arena_call_362) {
                        mak_ir_recover("ADT result allocation failed");
                        arena_call_362 = &mak_ir_default_obj__a;
                     } else {
                        struct mak__a *arena_call_363 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
                        if (!arena_call_363) {
                            mak_ir_recover("ADT result allocation failed");
                            arena_call_363 = &mak_ir_default_obj__a;
                         } else {
                            mak_ir_fn_10(mak_local_gh_62, mak_local_gi_63, INT64_C(2), INT64_C(0), mak_local_fy_75, (struct mak__a *)(arena_call_363));
                        }
                        mak_ir_fn_3(arena_call_363, (struct mak__a *)mak_ir_adt_recover_ptr((const void *)&mak_ir_default_obj__a, "integer used as ADT"), (struct mak__a *)(arena_call_362));
                    }
                    return (int64_t)((uintptr_t)(arena_call_362));
                }
                else if (0) {
                    uint64_t mak_local_fz_76 = ((struct mak__b *)mak_ir_adt_recover_ptr((const void *)&mak_ir_default_obj__b, "integer used as ADT"))->v.v3.f0;
                    struct mak__a *arena_call_364 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
                    if (!arena_call_364) {
                        mak_ir_recover("ADT result allocation failed");
                        arena_call_364 = &mak_ir_default_obj__a;
                     } else {
                        struct mak__a *arena_call_365 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
                        if (!arena_call_365) {
                            mak_ir_recover("ADT result allocation failed");
                            arena_call_365 = &mak_ir_default_obj__a;
                         } else {
                            mak_ir_fn_10(mak_local_gh_62, mak_local_gi_63, INT64_C(2), INT64_C(0), mak_local_fz_76, (struct mak__a *)(arena_call_365));
                        }
                        mak_ir_fn_3(arena_call_365, (struct mak__a *)mak_ir_adt_recover_ptr((const void *)&mak_ir_default_obj__a, "integer used as ADT"), (struct mak__a *)(arena_call_364));
                    }
                    return (int64_t)((uintptr_t)(arena_call_364));
                }
                else if (1) {
                    return a2;
                }
            }
            case INT64_C(99): {
                if (0) {
                    int32_t mak_local_ga_77 = ((struct mak__b *)mak_ir_adt_recover_ptr((const void *)&mak_ir_default_obj__b, "integer used as ADT"))->v.v8.f0;
                    struct mak__a *arena_obj_366 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
                    if (!arena_obj_366) {
                        mak_ir_recover("ADT object allocation failed");
                        arena_obj_366 = &mak_ir_default_obj__a;
                     } else {
                        (arena_obj_366)->tag = mak_ir_tag_1;
                        (arena_obj_366)->v.v1.f0 = mak_local_ga_77;
                        (arena_obj_366)->v.v1.f1 = (struct mak__a *)mak_ir_adt_recover_ptr((const void *)&mak_ir_default_obj__a, "integer used as ADT");
                    }
                    return (int64_t)((uintptr_t)(arena_obj_366));
                }
                else if (1) {
                    return a2;
                }
            }
            case INT64_C(115): {
                if (0) {
                    const char * mak_local_gb_78 = ((struct mak__b *)mak_ir_adt_recover_ptr((const void *)&mak_ir_default_obj__b, "integer used as ADT"))->v.v9.f0;
                    struct mak__a *arena_call_367 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
                    if (!arena_call_367) {
                        mak_ir_recover("ADT result allocation failed");
                        arena_call_367 = &mak_ir_default_obj__a;
                     } else {
                        struct mak__a *letobj_368 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
                        if (!letobj_368) {
                            mak_ir_recover("ADT local allocation failed");
                            letobj_368 = &mak_ir_default_obj__a;
                         } else {
                            *(letobj_368) = *(&mak_ir_default_obj__a);
                            struct mak__a *arena_call_369 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
                            if (!arena_call_369) {
                                mak_ir_recover("ADT result allocation failed");
                                arena_call_369 = &mak_ir_default_obj__a;
                             } else {
                                uint64_t mak_local_cp_79 = mak_ir_ext_1((const void *)(mak_local_gb_78));
                                int64_t case_371 = 0;
                                int32_t scrut_372 = (int32_t)(mak_local_gj_64) < (int32_t)(INT64_C(0));
                                if ((scrut_372 == INT64_C(1))) {
                                    case_371 = (int64_t)(mak_local_cp_79);
                                }
                                else if (1) {
                                    int64_t case_373 = 0;
                                    int32_t scrut_374 = (int32_t)(mak_local_gj_64) < (int32_t)(mak_local_cp_79);
                                    if ((scrut_374 == INT64_C(1))) {
                                        case_373 = mak_local_gj_64;
                                    }
                                    else if (1) {
                                        case_373 = (int64_t)(mak_local_cp_79);
                                    }
                                    case_371 = case_373;
                                }
                                int64_t eval_370 = case_371;
                                mak_ir_fn_11((const void *)(mak_local_gb_78), (eval_370 - INT64_C(1)), &mak_ir_ctor_obj_0, (struct mak__a *)(arena_call_369));
                            }
                            mak_ir_fn_0(arena_call_369, &mak_ir_ctor_obj_0, (struct mak__a *)(letobj_368));
                        }
                        struct mak__a * mak_local_cv_80 = letobj_368;
                        int64_t mak_local_cw_81 = mak_ir_fn_1(mak_local_cv_80, INT64_C(0));
                        int64_t case_375 = 0;
                        int32_t scrut_376 = (int32_t)(mak_local_gi_63) <= (int32_t)(mak_local_cw_81);
                        if ((scrut_376 == INT64_C(1))) {
                            case_375 = INT64_C(0);
                        }
                        else if (1) {
                            case_375 = (mak_local_gi_63 - mak_local_cw_81);
                        }
                        int64_t mak_local_cx_82 = case_375;
                        struct mak__a *arena_case_377 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
                        if (!arena_case_377) {
                            mak_ir_recover("ADT case-result allocation failed");
                            arena_case_377 = &mak_ir_default_obj__a;
                         } else {
                            int64_t case_379 = 0;
                            int64_t scrut_380 = (mak_local_gh_62 & INT64_C(1));
                            if ((scrut_380 == INT64_C(0))) {
                                case_379 = INT64_C(0);
                            }
                            else if (1) {
                                case_379 = INT64_C(1);
                            }
                            int64_t scrut_378 = case_379;
                            if ((scrut_378 == INT64_C(1))) {
                                struct mak__a *arena_call_381 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
                                if (!arena_call_381) {
                                    mak_ir_recover("ADT result allocation failed");
                                    arena_call_381 = &mak_ir_default_obj__a;
                                 } else {
                                    mak_ir_fn_4(INT64_C(32), mak_local_cx_82, (struct mak__a *)(arena_call_381));
                                }
                                mak_ir_fn_3(arena_call_381, mak_local_cv_80, (struct mak__a *)(arena_case_377));
                            }
                            else if (1) {
                                struct mak__a *arena_call_382 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
                                if (!arena_call_382) {
                                    mak_ir_recover("ADT result allocation failed");
                                    arena_call_382 = &mak_ir_default_obj__a;
                                 } else {
                                    mak_ir_fn_4(INT64_C(32), mak_local_cx_82, (struct mak__a *)(arena_call_382));
                                }
                                mak_ir_fn_3(mak_local_cv_80, arena_call_382, (struct mak__a *)(arena_case_377));
                            }
                        }
                        mak_ir_fn_3(arena_case_377, (struct mak__a *)mak_ir_adt_recover_ptr((const void *)&mak_ir_default_obj__a, "integer used as ADT"), (struct mak__a *)(arena_call_367));
                    }
                    return (int64_t)((uintptr_t)(arena_call_367));
                }
                else if (1) {
                    return a2;
                }
            }
            case INT64_C(112): {
                if (0) {
                    const void * mak_local_gc_83 = ((struct mak__b *)mak_ir_adt_recover_ptr((const void *)&mak_ir_default_obj__b, "integer used as ADT"))->v.v10.f0;
                    struct mak__a *arena_call_383 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
                    if (!arena_call_383) {
                        mak_ir_recover("ADT result allocation failed");
                        arena_call_383 = &mak_ir_default_obj__a;
                     } else {
                        struct mak__a *arena_call_384 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
                        if (!arena_call_384) {
                            mak_ir_recover("ADT result allocation failed");
                            arena_call_384 = &mak_ir_default_obj__a;
                         } else {
                            struct mak__a *arena_call_385 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
                            if (!arena_call_385) {
                                mak_ir_recover("ADT result allocation failed");
                                arena_call_385 = &mak_ir_default_obj__a;
                             } else {
                                mak_ir_fn_6(((double)(uintptr_t)(mak_local_gc_83)), INT64_C(16), INT64_C(0), (struct mak__a *)(arena_call_385));
                            }
                            struct mak__a *arena_obj_386 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
                            if (!arena_obj_386) {
                                mak_ir_recover("ADT object allocation failed");
                                arena_obj_386 = &mak_ir_default_obj__a;
                             } else {
                                (arena_obj_386)->tag = mak_ir_tag_1;
                                (arena_obj_386)->v.v1.f0 = (int32_t)(INT64_C(120));
                                struct mak__a *arena_obj_387 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
                                if (!arena_obj_387) {
                                    mak_ir_recover("ADT object allocation failed");
                                    arena_obj_387 = &mak_ir_default_obj__a;
                                 } else {
                                    (arena_obj_387)->tag = mak_ir_tag_1;
                                    (arena_obj_387)->v.v1.f0 = (int32_t)(INT64_C(48));
                                    (arena_obj_387)->v.v1.f1 = &mak_ir_ctor_obj_0;
                                }
                                (arena_obj_386)->v.v1.f1 = arena_obj_387;
                            }
                            mak_ir_fn_3(arena_call_385, arena_obj_386, (struct mak__a *)(arena_call_384));
                        }
                        mak_ir_fn_3(arena_call_384, (struct mak__a *)mak_ir_adt_recover_ptr((const void *)&mak_ir_default_obj__a, "integer used as ADT"), (struct mak__a *)(arena_call_383));
                    }
                    return (int64_t)((uintptr_t)(arena_call_383));
                }
                else if (1) {
                    return a2;
                }
            }
            case INT64_C(102): {
                if (0) {
                    double mak_local_gd_84 = ((struct mak__b *)mak_ir_adt_recover_ptr((const void *)&mak_ir_default_obj__b, "integer used as ADT"))->v.v11.f0;
                    struct mak__a *arena_call_388 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
                    if (!arena_call_388) {
                        mak_ir_recover("ADT result allocation failed");
                        arena_call_388 = &mak_ir_default_obj__a;
                     } else {
                        int64_t case_389 = 0;
                        int32_t scrut_390 = (int32_t)(mak_local_gd_84) < (int32_t)(0.00000000000000000);
                        if ((scrut_390 == INT64_C(1))) {
                            case_389 = (int64_t)((uint64_t)(((int64_t)UINT64_C(18446744073709551615))));
                        }
                        else if (1) {
                            case_389 = INT64_C(0);
                        }
                        int64_t mak_local_dw_85 = mak_ir_fn_8(mak_local_gh_62, case_389);
                        struct mak__a *letobj_391 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
                        if (!letobj_391) {
                            mak_ir_recover("ADT local allocation failed");
                            letobj_391 = &mak_ir_default_obj__a;
                         } else {
                            *(letobj_391) = *(&mak_ir_default_obj__a);
                            int32_t scrut_392 = (int32_t)(mak_local_dw_85) == (int32_t)(INT64_C(0));
                            if ((scrut_392 == INT64_C(1))) {
                                *(letobj_391) = mak_ir_ctor_obj_0;
                            }
                            else if (1) {
                                (letobj_391)->tag = mak_ir_tag_1;
                                (letobj_391)->v.v1.f0 = (int32_t)(mak_local_dw_85);
                                (letobj_391)->v.v1.f1 = &mak_ir_ctor_obj_0;
                            }
                        }
                        struct mak__a * mak_local_ea_86 = letobj_391;
                        struct mak__a *arena_case_393 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
                        if (!arena_case_393) {
                            mak_ir_recover("ADT case-result allocation failed");
                            arena_case_393 = &mak_ir_default_obj__a;
                         } else {
                            int64_t case_395 = 0;
                            int64_t scrut_396 = (mak_local_gh_62 & INT64_C(1));
                            if ((scrut_396 == INT64_C(0))) {
                                case_395 = INT64_C(0);
                            }
                            else if (1) {
                                case_395 = INT64_C(1);
                            }
                            int64_t scrut_394 = case_395;
                            if ((scrut_394 == INT64_C(1))) {
                                struct mak__a *arena_call_397 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
                                if (!arena_call_397) {
                                    mak_ir_recover("ADT result allocation failed");
                                    arena_call_397 = &mak_ir_default_obj__a;
                                 } else {
                                    int64_t case_398 = 0;
                                    struct mak__a *arena_call_402 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
                                    if (!arena_call_402) {
                                        mak_ir_recover("ADT result allocation failed");
                                        arena_call_402 = &mak_ir_default_obj__a;
                                     } else {
                                        int64_t case_403 = 0;
                                        int32_t scrut_404 = (int32_t)(mak_local_gj_64) < (int32_t)(INT64_C(0));
                                        if ((scrut_404 == INT64_C(1))) {
                                            case_403 = INT64_C(6);
                                        }
                                        else if (1) {
                                            case_403 = mak_local_gj_64;
                                        }
                                        mak_ir_fn_13(mak_ir_ext_4(mak_local_gd_84), case_403, (struct mak__a *)(arena_call_402));
                                    }
                                    int64_t eval_401 = mak_ir_fn_1(arena_call_402, INT64_C(0));
                                    int64_t case_406 = 0;
                                    int32_t scrut_407 = (int32_t)(mak_local_dw_85) == (int32_t)(INT64_C(0));
                                    if ((scrut_407 == INT64_C(1))) {
                                        case_406 = INT64_C(0);
                                    }
                                    else if (1) {
                                        case_406 = INT64_C(1);
                                    }
                                    int64_t eval_405 = case_406;
                                    int64_t eval_400 = (eval_401 + eval_405);
                                    int32_t scrut_399 = (int32_t)(mak_local_gi_63) <= (int32_t)(eval_400);
                                    if ((scrut_399 == INT64_C(1))) {
                                        case_398 = INT64_C(0);
                                    }
                                    else if (1) {
                                        struct mak__a *arena_call_410 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
                                        if (!arena_call_410) {
                                            mak_ir_recover("ADT result allocation failed");
                                            arena_call_410 = &mak_ir_default_obj__a;
                                         } else {
                                            int64_t case_411 = 0;
                                            int32_t scrut_412 = (int32_t)(mak_local_gj_64) < (int32_t)(INT64_C(0));
                                            if ((scrut_412 == INT64_C(1))) {
                                                case_411 = INT64_C(6);
                                            }
                                            else if (1) {
                                                case_411 = mak_local_gj_64;
                                            }
                                            mak_ir_fn_13(mak_ir_ext_4(mak_local_gd_84), case_411, (struct mak__a *)(arena_call_410));
                                        }
                                        int64_t eval_409 = mak_ir_fn_1(arena_call_410, INT64_C(0));
                                        int64_t case_414 = 0;
                                        int32_t scrut_415 = (int32_t)(mak_local_dw_85) == (int32_t)(INT64_C(0));
                                        if ((scrut_415 == INT64_C(1))) {
                                            case_414 = INT64_C(0);
                                        }
                                        else if (1) {
                                            case_414 = INT64_C(1);
                                        }
                                        int64_t eval_413 = case_414;
                                        int64_t eval_408 = (eval_409 + eval_413);
                                        case_398 = (mak_local_gi_63 - eval_408);
                                    }
                                    mak_ir_fn_4(INT64_C(32), case_398, (struct mak__a *)(arena_call_397));
                                }
                                struct mak__a *arena_call_416 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
                                if (!arena_call_416) {
                                    mak_ir_recover("ADT result allocation failed");
                                    arena_call_416 = &mak_ir_default_obj__a;
                                 } else {
                                    struct mak__a *arena_call_417 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
                                    if (!arena_call_417) {
                                        mak_ir_recover("ADT result allocation failed");
                                        arena_call_417 = &mak_ir_default_obj__a;
                                     } else {
                                        int64_t case_418 = 0;
                                        int32_t scrut_419 = (int32_t)(mak_local_gj_64) < (int32_t)(INT64_C(0));
                                        if ((scrut_419 == INT64_C(1))) {
                                            case_418 = INT64_C(6);
                                        }
                                        else if (1) {
                                            case_418 = mak_local_gj_64;
                                        }
                                        mak_ir_fn_13(mak_ir_ext_4(mak_local_gd_84), case_418, (struct mak__a *)(arena_call_417));
                                    }
                                    mak_ir_fn_3(arena_call_417, mak_local_ea_86, (struct mak__a *)(arena_call_416));
                                }
                                mak_ir_fn_3(arena_call_397, arena_call_416, (struct mak__a *)(arena_case_393));
                            }
                            else if (1) {
                                int64_t case_421 = 0;
                                int64_t scrut_422 = (mak_local_gh_62 & INT64_C(2));
                                if ((scrut_422 == INT64_C(0))) {
                                    case_421 = INT64_C(0);
                                }
                                else if (1) {
                                    case_421 = INT64_C(1);
                                }
                                int64_t scrut_420 = case_421;
                                if ((scrut_420 == INT64_C(1))) {
                                    struct mak__a *arena_call_423 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
                                    if (!arena_call_423) {
                                        mak_ir_recover("ADT result allocation failed");
                                        arena_call_423 = &mak_ir_default_obj__a;
                                     } else {
                                        int64_t case_424 = 0;
                                        int32_t scrut_425 = (int32_t)(mak_local_gj_64) < (int32_t)(INT64_C(0));
                                        if ((scrut_425 == INT64_C(1))) {
                                            case_424 = INT64_C(6);
                                        }
                                        else if (1) {
                                            case_424 = mak_local_gj_64;
                                        }
                                        mak_ir_fn_13(mak_ir_ext_4(mak_local_gd_84), case_424, (struct mak__a *)(arena_call_423));
                                    }
                                    struct mak__a *arena_call_426 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
                                    if (!arena_call_426) {
                                        mak_ir_recover("ADT result allocation failed");
                                        arena_call_426 = &mak_ir_default_obj__a;
                                     } else {
                                        struct mak__a *arena_call_427 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
                                        if (!arena_call_427) {
                                            mak_ir_recover("ADT result allocation failed");
                                            arena_call_427 = &mak_ir_default_obj__a;
                                         } else {
                                            int64_t case_428 = 0;
                                            struct mak__a *arena_call_432 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
                                            if (!arena_call_432) {
                                                mak_ir_recover("ADT result allocation failed");
                                                arena_call_432 = &mak_ir_default_obj__a;
                                             } else {
                                                int64_t case_433 = 0;
                                                int32_t scrut_434 = (int32_t)(mak_local_gj_64) < (int32_t)(INT64_C(0));
                                                if ((scrut_434 == INT64_C(1))) {
                                                    case_433 = INT64_C(6);
                                                }
                                                else if (1) {
                                                    case_433 = mak_local_gj_64;
                                                }
                                                mak_ir_fn_13(mak_ir_ext_4(mak_local_gd_84), case_433, (struct mak__a *)(arena_call_432));
                                            }
                                            int64_t eval_431 = mak_ir_fn_1(arena_call_432, INT64_C(0));
                                            int64_t case_436 = 0;
                                            int32_t scrut_437 = (int32_t)(mak_local_dw_85) == (int32_t)(INT64_C(0));
                                            if ((scrut_437 == INT64_C(1))) {
                                                case_436 = INT64_C(0);
                                            }
                                            else if (1) {
                                                case_436 = INT64_C(1);
                                            }
                                            int64_t eval_435 = case_436;
                                            int64_t eval_430 = (eval_431 + eval_435);
                                            int32_t scrut_429 = (int32_t)(mak_local_gi_63) <= (int32_t)(eval_430);
                                            if ((scrut_429 == INT64_C(1))) {
                                                case_428 = INT64_C(0);
                                            }
                                            else if (1) {
                                                struct mak__a *arena_call_440 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
                                                if (!arena_call_440) {
                                                    mak_ir_recover("ADT result allocation failed");
                                                    arena_call_440 = &mak_ir_default_obj__a;
                                                 } else {
                                                    int64_t case_441 = 0;
                                                    int32_t scrut_442 = (int32_t)(mak_local_gj_64) < (int32_t)(INT64_C(0));
                                                    if ((scrut_442 == INT64_C(1))) {
                                                        case_441 = INT64_C(6);
                                                    }
                                                    else if (1) {
                                                        case_441 = mak_local_gj_64;
                                                    }
                                                    mak_ir_fn_13(mak_ir_ext_4(mak_local_gd_84), case_441, (struct mak__a *)(arena_call_440));
                                                }
                                                int64_t eval_439 = mak_ir_fn_1(arena_call_440, INT64_C(0));
                                                int64_t case_444 = 0;
                                                int32_t scrut_445 = (int32_t)(mak_local_dw_85) == (int32_t)(INT64_C(0));
                                                if ((scrut_445 == INT64_C(1))) {
                                                    case_444 = INT64_C(0);
                                                }
                                                else if (1) {
                                                    case_444 = INT64_C(1);
                                                }
                                                int64_t eval_443 = case_444;
                                                int64_t eval_438 = (eval_439 + eval_443);
                                                case_428 = (mak_local_gi_63 - eval_438);
                                            }
                                            mak_ir_fn_4(INT64_C(48), case_428, (struct mak__a *)(arena_call_427));
                                        }
                                        mak_ir_fn_3(arena_call_427, mak_local_ea_86, (struct mak__a *)(arena_call_426));
                                    }
                                    mak_ir_fn_3(arena_call_423, arena_call_426, (struct mak__a *)(arena_case_393));
                                }
                                else if (1) {
                                    struct mak__a *arena_call_446 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
                                    if (!arena_call_446) {
                                        mak_ir_recover("ADT result allocation failed");
                                        arena_call_446 = &mak_ir_default_obj__a;
                                     } else {
                                        int64_t case_447 = 0;
                                        int32_t scrut_448 = (int32_t)(mak_local_gj_64) < (int32_t)(INT64_C(0));
                                        if ((scrut_448 == INT64_C(1))) {
                                            case_447 = INT64_C(6);
                                        }
                                        else if (1) {
                                            case_447 = mak_local_gj_64;
                                        }
                                        mak_ir_fn_13(mak_ir_ext_4(mak_local_gd_84), case_447, (struct mak__a *)(arena_call_446));
                                    }
                                    struct mak__a *arena_call_449 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
                                    if (!arena_call_449) {
                                        mak_ir_recover("ADT result allocation failed");
                                        arena_call_449 = &mak_ir_default_obj__a;
                                     } else {
                                        struct mak__a *arena_call_450 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
                                        if (!arena_call_450) {
                                            mak_ir_recover("ADT result allocation failed");
                                            arena_call_450 = &mak_ir_default_obj__a;
                                         } else {
                                            int64_t case_451 = 0;
                                            struct mak__a *arena_call_455 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
                                            if (!arena_call_455) {
                                                mak_ir_recover("ADT result allocation failed");
                                                arena_call_455 = &mak_ir_default_obj__a;
                                             } else {
                                                int64_t case_456 = 0;
                                                int32_t scrut_457 = (int32_t)(mak_local_gj_64) < (int32_t)(INT64_C(0));
                                                if ((scrut_457 == INT64_C(1))) {
                                                    case_456 = INT64_C(6);
                                                }
                                                else if (1) {
                                                    case_456 = mak_local_gj_64;
                                                }
                                                mak_ir_fn_13(mak_ir_ext_4(mak_local_gd_84), case_456, (struct mak__a *)(arena_call_455));
                                            }
                                            int64_t eval_454 = mak_ir_fn_1(arena_call_455, INT64_C(0));
                                            int64_t case_459 = 0;
                                            int32_t scrut_460 = (int32_t)(mak_local_dw_85) == (int32_t)(INT64_C(0));
                                            if ((scrut_460 == INT64_C(1))) {
                                                case_459 = INT64_C(0);
                                            }
                                            else if (1) {
                                                case_459 = INT64_C(1);
                                            }
                                            int64_t eval_458 = case_459;
                                            int64_t eval_453 = (eval_454 + eval_458);
                                            int32_t scrut_452 = (int32_t)(mak_local_gi_63) <= (int32_t)(eval_453);
                                            if ((scrut_452 == INT64_C(1))) {
                                                case_451 = INT64_C(0);
                                            }
                                            else if (1) {
                                                struct mak__a *arena_call_463 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
                                                if (!arena_call_463) {
                                                    mak_ir_recover("ADT result allocation failed");
                                                    arena_call_463 = &mak_ir_default_obj__a;
                                                 } else {
                                                    int64_t case_464 = 0;
                                                    int32_t scrut_465 = (int32_t)(mak_local_gj_64) < (int32_t)(INT64_C(0));
                                                    if ((scrut_465 == INT64_C(1))) {
                                                        case_464 = INT64_C(6);
                                                    }
                                                    else if (1) {
                                                        case_464 = mak_local_gj_64;
                                                    }
                                                    mak_ir_fn_13(mak_ir_ext_4(mak_local_gd_84), case_464, (struct mak__a *)(arena_call_463));
                                                }
                                                int64_t eval_462 = mak_ir_fn_1(arena_call_463, INT64_C(0));
                                                int64_t case_467 = 0;
                                                int32_t scrut_468 = (int32_t)(mak_local_dw_85) == (int32_t)(INT64_C(0));
                                                if ((scrut_468 == INT64_C(1))) {
                                                    case_467 = INT64_C(0);
                                                }
                                                else if (1) {
                                                    case_467 = INT64_C(1);
                                                }
                                                int64_t eval_466 = case_467;
                                                int64_t eval_461 = (eval_462 + eval_466);
                                                case_451 = (mak_local_gi_63 - eval_461);
                                            }
                                            mak_ir_fn_4(INT64_C(32), case_451, (struct mak__a *)(arena_call_450));
                                        }
                                        mak_ir_fn_3(mak_local_ea_86, arena_call_450, (struct mak__a *)(arena_call_449));
                                    }
                                    mak_ir_fn_3(arena_call_446, arena_call_449, (struct mak__a *)(arena_case_393));
                                }
                            }
                        }
                        mak_ir_fn_3(arena_case_393, (struct mak__a *)mak_ir_adt_recover_ptr((const void *)&mak_ir_default_obj__a, "integer used as ADT"), (struct mak__a *)(arena_call_388));
                    }
                    return (int64_t)((uintptr_t)(arena_call_388));
                }
                else if (1) {
                    return a2;
                }
            }
            default: {
                return a2;
            }
        }
    }
    else {
        mak_ir_recover("case had no matching arm");
        return 0;
    }
}

static inline void mak_ir_fn_24(int64_t a0, int64_t a1, int64_t a2, int64_t a3, struct mak__b * a4, int64_t a5, struct mak__a *out) {
    switch (a0) {
        case INT64_C(100): {
            if ((a4 != NULL && (a4)->tag == mak_ir_tag_2)) {
                int64_t mak_local_fp_87 = (a4)->v.v2.f0;
                struct mak__a *arena_call_469 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
                if (!arena_call_469) {
                    mak_ir_recover("ADT result allocation failed");
                    arena_call_469 = &mak_ir_default_obj__a;
                 } else {
                    mak_ir_fn_9(a1, a2, mak_local_fp_87, (struct mak__a *)(arena_call_469));
                }
                mak_ir_fn_3(arena_call_469, (struct mak__a *)mak_ir_adt_recover_ptr((const void *)&mak_ir_default_obj__a, "integer used as ADT"), (struct mak__a *)(out));
            }
            else if (1) {
                (void)(a5);
                mak_ir_recover("non-ADT expression used as ADT result");
                *(out) = *(&mak_ir_default_obj__a);
            }
            break;
        }
        case INT64_C(105): {
            if ((a4 != NULL && (a4)->tag == mak_ir_tag_2)) {
                int64_t mak_local_fq_88 = (a4)->v.v2.f0;
                struct mak__a *arena_call_470 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
                if (!arena_call_470) {
                    mak_ir_recover("ADT result allocation failed");
                    arena_call_470 = &mak_ir_default_obj__a;
                 } else {
                    mak_ir_fn_9(a1, a2, mak_local_fq_88, (struct mak__a *)(arena_call_470));
                }
                mak_ir_fn_3(arena_call_470, (struct mak__a *)mak_ir_adt_recover_ptr((const void *)&mak_ir_default_obj__a, "integer used as ADT"), (struct mak__a *)(out));
            }
            else if (1) {
                (void)(a5);
                mak_ir_recover("non-ADT expression used as ADT result");
                *(out) = *(&mak_ir_default_obj__a);
            }
            break;
        }
        case INT64_C(117): {
            if ((a4 != NULL && (a4)->tag == mak_ir_tag_3)) {
                uint64_t mak_local_fr_89 = (a4)->v.v3.f0;
                struct mak__a *arena_call_471 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
                if (!arena_call_471) {
                    mak_ir_recover("ADT result allocation failed");
                    arena_call_471 = &mak_ir_default_obj__a;
                 } else {
                    mak_ir_fn_10(a1, a2, INT64_C(10), INT64_C(0), mak_local_fr_89, (struct mak__a *)(arena_call_471));
                }
                mak_ir_fn_3(arena_call_471, (struct mak__a *)mak_ir_adt_recover_ptr((const void *)&mak_ir_default_obj__a, "integer used as ADT"), (struct mak__a *)(out));
            }
            else if (1) {
                (void)(a5);
                mak_ir_recover("non-ADT expression used as ADT result");
                *(out) = *(&mak_ir_default_obj__a);
            }
            break;
        }
        case INT64_C(120): {
            if ((a4 != NULL && (a4)->tag == mak_ir_tag_4)) {
                uint64_t mak_local_fs_90 = (a4)->v.v4.f0;
                struct mak__a *arena_call_472 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
                if (!arena_call_472) {
                    mak_ir_recover("ADT result allocation failed");
                    arena_call_472 = &mak_ir_default_obj__a;
                 } else {
                    mak_ir_fn_10(a1, a2, INT64_C(16), INT64_C(0), mak_local_fs_90, (struct mak__a *)(arena_call_472));
                }
                mak_ir_fn_3(arena_call_472, (struct mak__a *)mak_ir_adt_recover_ptr((const void *)&mak_ir_default_obj__a, "integer used as ADT"), (struct mak__a *)(out));
            }
            else if ((a4 != NULL && (a4)->tag == mak_ir_tag_3)) {
                uint64_t mak_local_ft_91 = (a4)->v.v3.f0;
                struct mak__a *arena_call_473 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
                if (!arena_call_473) {
                    mak_ir_recover("ADT result allocation failed");
                    arena_call_473 = &mak_ir_default_obj__a;
                 } else {
                    mak_ir_fn_10(a1, a2, INT64_C(16), INT64_C(0), mak_local_ft_91, (struct mak__a *)(arena_call_473));
                }
                mak_ir_fn_3(arena_call_473, (struct mak__a *)mak_ir_adt_recover_ptr((const void *)&mak_ir_default_obj__a, "integer used as ADT"), (struct mak__a *)(out));
            }
            else if (1) {
                (void)(a5);
                mak_ir_recover("non-ADT expression used as ADT result");
                *(out) = *(&mak_ir_default_obj__a);
            }
            break;
        }
        case INT64_C(88): {
            if ((a4 != NULL && (a4)->tag == mak_ir_tag_5)) {
                uint64_t mak_local_fu_92 = (a4)->v.v5.f0;
                struct mak__a *arena_call_474 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
                if (!arena_call_474) {
                    mak_ir_recover("ADT result allocation failed");
                    arena_call_474 = &mak_ir_default_obj__a;
                 } else {
                    mak_ir_fn_10(a1, a2, INT64_C(16), INT64_C(1), mak_local_fu_92, (struct mak__a *)(arena_call_474));
                }
                mak_ir_fn_3(arena_call_474, (struct mak__a *)mak_ir_adt_recover_ptr((const void *)&mak_ir_default_obj__a, "integer used as ADT"), (struct mak__a *)(out));
            }
            else if ((a4 != NULL && (a4)->tag == mak_ir_tag_3)) {
                uint64_t mak_local_fv_93 = (a4)->v.v3.f0;
                struct mak__a *arena_call_475 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
                if (!arena_call_475) {
                    mak_ir_recover("ADT result allocation failed");
                    arena_call_475 = &mak_ir_default_obj__a;
                 } else {
                    mak_ir_fn_10(a1, a2, INT64_C(16), INT64_C(1), mak_local_fv_93, (struct mak__a *)(arena_call_475));
                }
                mak_ir_fn_3(arena_call_475, (struct mak__a *)mak_ir_adt_recover_ptr((const void *)&mak_ir_default_obj__a, "integer used as ADT"), (struct mak__a *)(out));
            }
            else if (1) {
                (void)(a5);
                mak_ir_recover("non-ADT expression used as ADT result");
                *(out) = *(&mak_ir_default_obj__a);
            }
            break;
        }
        case INT64_C(111): {
            if ((a4 != NULL && (a4)->tag == mak_ir_tag_6)) {
                uint64_t mak_local_fw_94 = (a4)->v.v6.f0;
                struct mak__a *arena_call_476 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
                if (!arena_call_476) {
                    mak_ir_recover("ADT result allocation failed");
                    arena_call_476 = &mak_ir_default_obj__a;
                 } else {
                    mak_ir_fn_10(a1, a2, INT64_C(8), INT64_C(0), mak_local_fw_94, (struct mak__a *)(arena_call_476));
                }
                mak_ir_fn_3(arena_call_476, (struct mak__a *)mak_ir_adt_recover_ptr((const void *)&mak_ir_default_obj__a, "integer used as ADT"), (struct mak__a *)(out));
            }
            else if ((a4 != NULL && (a4)->tag == mak_ir_tag_3)) {
                uint64_t mak_local_fx_95 = (a4)->v.v3.f0;
                struct mak__a *arena_call_477 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
                if (!arena_call_477) {
                    mak_ir_recover("ADT result allocation failed");
                    arena_call_477 = &mak_ir_default_obj__a;
                 } else {
                    mak_ir_fn_10(a1, a2, INT64_C(8), INT64_C(0), mak_local_fx_95, (struct mak__a *)(arena_call_477));
                }
                mak_ir_fn_3(arena_call_477, (struct mak__a *)mak_ir_adt_recover_ptr((const void *)&mak_ir_default_obj__a, "integer used as ADT"), (struct mak__a *)(out));
            }
            else if (1) {
                (void)(a5);
                mak_ir_recover("non-ADT expression used as ADT result");
                *(out) = *(&mak_ir_default_obj__a);
            }
            break;
        }
        case INT64_C(98): {
            if ((a4 != NULL && (a4)->tag == mak_ir_tag_7)) {
                uint64_t mak_local_fy_96 = (a4)->v.v7.f0;
                struct mak__a *arena_call_478 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
                if (!arena_call_478) {
                    mak_ir_recover("ADT result allocation failed");
                    arena_call_478 = &mak_ir_default_obj__a;
                 } else {
                    mak_ir_fn_10(a1, a2, INT64_C(2), INT64_C(0), mak_local_fy_96, (struct mak__a *)(arena_call_478));
                }
                mak_ir_fn_3(arena_call_478, (struct mak__a *)mak_ir_adt_recover_ptr((const void *)&mak_ir_default_obj__a, "integer used as ADT"), (struct mak__a *)(out));
            }
            else if ((a4 != NULL && (a4)->tag == mak_ir_tag_3)) {
                uint64_t mak_local_fz_97 = (a4)->v.v3.f0;
                struct mak__a *arena_call_479 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
                if (!arena_call_479) {
                    mak_ir_recover("ADT result allocation failed");
                    arena_call_479 = &mak_ir_default_obj__a;
                 } else {
                    mak_ir_fn_10(a1, a2, INT64_C(2), INT64_C(0), mak_local_fz_97, (struct mak__a *)(arena_call_479));
                }
                mak_ir_fn_3(arena_call_479, (struct mak__a *)mak_ir_adt_recover_ptr((const void *)&mak_ir_default_obj__a, "integer used as ADT"), (struct mak__a *)(out));
            }
            else if (1) {
                (void)(a5);
                mak_ir_recover("non-ADT expression used as ADT result");
                *(out) = *(&mak_ir_default_obj__a);
            }
            break;
        }
        case INT64_C(99): {
            if ((a4 != NULL && (a4)->tag == mak_ir_tag_8)) {
                int32_t mak_local_ga_98 = (a4)->v.v8.f0;
                (out)->tag = mak_ir_tag_1;
                (out)->v.v1.f0 = mak_local_ga_98;
                (out)->v.v1.f1 = (struct mak__a *)mak_ir_adt_recover_ptr((const void *)&mak_ir_default_obj__a, "integer used as ADT");
            }
            else if (1) {
                (void)(a5);
                mak_ir_recover("non-ADT expression used as ADT result");
                *(out) = *(&mak_ir_default_obj__a);
            }
            break;
        }
        case INT64_C(115): {
            if ((a4 != NULL && (a4)->tag == mak_ir_tag_9)) {
                const char * mak_local_gb_99 = (a4)->v.v9.f0;
                struct mak__a *letobj_480 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
                if (!letobj_480) {
                    mak_ir_recover("ADT local allocation failed");
                    letobj_480 = &mak_ir_default_obj__a;
                 } else {
                    *(letobj_480) = *(&mak_ir_default_obj__a);
                    struct mak__a *arena_call_481 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
                    if (!arena_call_481) {
                        mak_ir_recover("ADT result allocation failed");
                        arena_call_481 = &mak_ir_default_obj__a;
                     } else {
                        uint64_t mak_local_cp_100 = mak_ir_ext_1((const void *)(mak_local_gb_99));
                        int64_t case_483 = 0;
                        int32_t scrut_484 = (int32_t)(a3) < (int32_t)(INT64_C(0));
                        if ((scrut_484 == INT64_C(1))) {
                            case_483 = (int64_t)(mak_local_cp_100);
                        }
                        else if (1) {
                            int64_t case_485 = 0;
                            int32_t scrut_486 = (int32_t)(a3) < (int32_t)(mak_local_cp_100);
                            if ((scrut_486 == INT64_C(1))) {
                                case_485 = a3;
                            }
                            else if (1) {
                                case_485 = (int64_t)(mak_local_cp_100);
                            }
                            case_483 = case_485;
                        }
                        int64_t eval_482 = case_483;
                        mak_ir_fn_11((const void *)(mak_local_gb_99), (eval_482 - INT64_C(1)), &mak_ir_ctor_obj_0, (struct mak__a *)(arena_call_481));
                    }
                    mak_ir_fn_0(arena_call_481, &mak_ir_ctor_obj_0, (struct mak__a *)(letobj_480));
                }
                struct mak__a * mak_local_cv_101 = letobj_480;
                int64_t mak_local_cw_102 = mak_ir_fn_1(mak_local_cv_101, INT64_C(0));
                int64_t case_487 = 0;
                int32_t scrut_488 = (int32_t)(a2) <= (int32_t)(mak_local_cw_102);
                if ((scrut_488 == INT64_C(1))) {
                    case_487 = INT64_C(0);
                }
                else if (1) {
                    case_487 = (a2 - mak_local_cw_102);
                }
                int64_t mak_local_cx_103 = case_487;
                struct mak__a *arena_case_489 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
                if (!arena_case_489) {
                    mak_ir_recover("ADT case-result allocation failed");
                    arena_case_489 = &mak_ir_default_obj__a;
                 } else {
                    int64_t case_491 = 0;
                    int64_t scrut_492 = (a1 & INT64_C(1));
                    if ((scrut_492 == INT64_C(0))) {
                        case_491 = INT64_C(0);
                    }
                    else if (1) {
                        case_491 = INT64_C(1);
                    }
                    int64_t scrut_490 = case_491;
                    if ((scrut_490 == INT64_C(1))) {
                        struct mak__a *arena_call_493 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
                        if (!arena_call_493) {
                            mak_ir_recover("ADT result allocation failed");
                            arena_call_493 = &mak_ir_default_obj__a;
                         } else {
                            mak_ir_fn_4(INT64_C(32), mak_local_cx_103, (struct mak__a *)(arena_call_493));
                        }
                        mak_ir_fn_3(arena_call_493, mak_local_cv_101, (struct mak__a *)(arena_case_489));
                    }
                    else if (1) {
                        struct mak__a *arena_call_494 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
                        if (!arena_call_494) {
                            mak_ir_recover("ADT result allocation failed");
                            arena_call_494 = &mak_ir_default_obj__a;
                         } else {
                            mak_ir_fn_4(INT64_C(32), mak_local_cx_103, (struct mak__a *)(arena_call_494));
                        }
                        mak_ir_fn_3(mak_local_cv_101, arena_call_494, (struct mak__a *)(arena_case_489));
                    }
                }
                mak_ir_fn_3(arena_case_489, (struct mak__a *)mak_ir_adt_recover_ptr((const void *)&mak_ir_default_obj__a, "integer used as ADT"), (struct mak__a *)(out));
            }
            else if (1) {
                (void)(a5);
                mak_ir_recover("non-ADT expression used as ADT result");
                *(out) = *(&mak_ir_default_obj__a);
            }
            break;
        }
        case INT64_C(112): {
            if ((a4 != NULL && (a4)->tag == mak_ir_tag_10)) {
                const void * mak_local_gc_104 = (a4)->v.v10.f0;
                struct mak__a *arena_call_495 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
                if (!arena_call_495) {
                    mak_ir_recover("ADT result allocation failed");
                    arena_call_495 = &mak_ir_default_obj__a;
                 } else {
                    struct mak__a *arena_call_496 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
                    if (!arena_call_496) {
                        mak_ir_recover("ADT result allocation failed");
                        arena_call_496 = &mak_ir_default_obj__a;
                     } else {
                        mak_ir_fn_6(((double)(uintptr_t)(mak_local_gc_104)), INT64_C(16), INT64_C(0), (struct mak__a *)(arena_call_496));
                    }
                    struct mak__a *arena_obj_497 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
                    if (!arena_obj_497) {
                        mak_ir_recover("ADT object allocation failed");
                        arena_obj_497 = &mak_ir_default_obj__a;
                     } else {
                        (arena_obj_497)->tag = mak_ir_tag_1;
                        (arena_obj_497)->v.v1.f0 = (int32_t)(INT64_C(120));
                        struct mak__a *arena_obj_498 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
                        if (!arena_obj_498) {
                            mak_ir_recover("ADT object allocation failed");
                            arena_obj_498 = &mak_ir_default_obj__a;
                         } else {
                            (arena_obj_498)->tag = mak_ir_tag_1;
                            (arena_obj_498)->v.v1.f0 = (int32_t)(INT64_C(48));
                            (arena_obj_498)->v.v1.f1 = &mak_ir_ctor_obj_0;
                        }
                        (arena_obj_497)->v.v1.f1 = arena_obj_498;
                    }
                    mak_ir_fn_3(arena_call_496, arena_obj_497, (struct mak__a *)(arena_call_495));
                }
                mak_ir_fn_3(arena_call_495, (struct mak__a *)mak_ir_adt_recover_ptr((const void *)&mak_ir_default_obj__a, "integer used as ADT"), (struct mak__a *)(out));
            }
            else if (1) {
                (void)(a5);
                mak_ir_recover("non-ADT expression used as ADT result");
                *(out) = *(&mak_ir_default_obj__a);
            }
            break;
        }
        case INT64_C(102): {
            if ((a4 != NULL && (a4)->tag == mak_ir_tag_11)) {
                double mak_local_gd_105 = (a4)->v.v11.f0;
                int64_t case_499 = 0;
                int32_t scrut_500 = (int32_t)(mak_local_gd_105) < (int32_t)(0.00000000000000000);
                if ((scrut_500 == INT64_C(1))) {
                    case_499 = (int64_t)((uint64_t)(((int64_t)UINT64_C(18446744073709551615))));
                }
                else if (1) {
                    case_499 = INT64_C(0);
                }
                int64_t mak_local_dw_106 = mak_ir_fn_8(a1, case_499);
                struct mak__a *letobj_501 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
                if (!letobj_501) {
                    mak_ir_recover("ADT local allocation failed");
                    letobj_501 = &mak_ir_default_obj__a;
                 } else {
                    *(letobj_501) = *(&mak_ir_default_obj__a);
                    int32_t scrut_502 = (int32_t)(mak_local_dw_106) == (int32_t)(INT64_C(0));
                    if ((scrut_502 == INT64_C(1))) {
                        *(letobj_501) = mak_ir_ctor_obj_0;
                    }
                    else if (1) {
                        (letobj_501)->tag = mak_ir_tag_1;
                        (letobj_501)->v.v1.f0 = (int32_t)(mak_local_dw_106);
                        (letobj_501)->v.v1.f1 = &mak_ir_ctor_obj_0;
                    }
                }
                struct mak__a * mak_local_ea_107 = letobj_501;
                struct mak__a *arena_case_503 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
                if (!arena_case_503) {
                    mak_ir_recover("ADT case-result allocation failed");
                    arena_case_503 = &mak_ir_default_obj__a;
                 } else {
                    int64_t case_505 = 0;
                    int64_t scrut_506 = (a1 & INT64_C(1));
                    if ((scrut_506 == INT64_C(0))) {
                        case_505 = INT64_C(0);
                    }
                    else if (1) {
                        case_505 = INT64_C(1);
                    }
                    int64_t scrut_504 = case_505;
                    if ((scrut_504 == INT64_C(1))) {
                        struct mak__a *arena_call_507 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
                        if (!arena_call_507) {
                            mak_ir_recover("ADT result allocation failed");
                            arena_call_507 = &mak_ir_default_obj__a;
                         } else {
                            int64_t case_508 = 0;
                            struct mak__a *arena_call_512 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
                            if (!arena_call_512) {
                                mak_ir_recover("ADT result allocation failed");
                                arena_call_512 = &mak_ir_default_obj__a;
                             } else {
                                int64_t case_513 = 0;
                                int32_t scrut_514 = (int32_t)(a3) < (int32_t)(INT64_C(0));
                                if ((scrut_514 == INT64_C(1))) {
                                    case_513 = INT64_C(6);
                                }
                                else if (1) {
                                    case_513 = a3;
                                }
                                mak_ir_fn_13(mak_ir_ext_4(mak_local_gd_105), case_513, (struct mak__a *)(arena_call_512));
                            }
                            int64_t eval_511 = mak_ir_fn_1(arena_call_512, INT64_C(0));
                            int64_t case_516 = 0;
                            int32_t scrut_517 = (int32_t)(mak_local_dw_106) == (int32_t)(INT64_C(0));
                            if ((scrut_517 == INT64_C(1))) {
                                case_516 = INT64_C(0);
                            }
                            else if (1) {
                                case_516 = INT64_C(1);
                            }
                            int64_t eval_515 = case_516;
                            int64_t eval_510 = (eval_511 + eval_515);
                            int32_t scrut_509 = (int32_t)(a2) <= (int32_t)(eval_510);
                            if ((scrut_509 == INT64_C(1))) {
                                case_508 = INT64_C(0);
                            }
                            else if (1) {
                                struct mak__a *arena_call_520 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
                                if (!arena_call_520) {
                                    mak_ir_recover("ADT result allocation failed");
                                    arena_call_520 = &mak_ir_default_obj__a;
                                 } else {
                                    int64_t case_521 = 0;
                                    int32_t scrut_522 = (int32_t)(a3) < (int32_t)(INT64_C(0));
                                    if ((scrut_522 == INT64_C(1))) {
                                        case_521 = INT64_C(6);
                                    }
                                    else if (1) {
                                        case_521 = a3;
                                    }
                                    mak_ir_fn_13(mak_ir_ext_4(mak_local_gd_105), case_521, (struct mak__a *)(arena_call_520));
                                }
                                int64_t eval_519 = mak_ir_fn_1(arena_call_520, INT64_C(0));
                                int64_t case_524 = 0;
                                int32_t scrut_525 = (int32_t)(mak_local_dw_106) == (int32_t)(INT64_C(0));
                                if ((scrut_525 == INT64_C(1))) {
                                    case_524 = INT64_C(0);
                                }
                                else if (1) {
                                    case_524 = INT64_C(1);
                                }
                                int64_t eval_523 = case_524;
                                int64_t eval_518 = (eval_519 + eval_523);
                                case_508 = (a2 - eval_518);
                            }
                            mak_ir_fn_4(INT64_C(32), case_508, (struct mak__a *)(arena_call_507));
                        }
                        struct mak__a *arena_call_526 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
                        if (!arena_call_526) {
                            mak_ir_recover("ADT result allocation failed");
                            arena_call_526 = &mak_ir_default_obj__a;
                         } else {
                            struct mak__a *arena_call_527 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
                            if (!arena_call_527) {
                                mak_ir_recover("ADT result allocation failed");
                                arena_call_527 = &mak_ir_default_obj__a;
                             } else {
                                int64_t case_528 = 0;
                                int32_t scrut_529 = (int32_t)(a3) < (int32_t)(INT64_C(0));
                                if ((scrut_529 == INT64_C(1))) {
                                    case_528 = INT64_C(6);
                                }
                                else if (1) {
                                    case_528 = a3;
                                }
                                mak_ir_fn_13(mak_ir_ext_4(mak_local_gd_105), case_528, (struct mak__a *)(arena_call_527));
                            }
                            mak_ir_fn_3(arena_call_527, mak_local_ea_107, (struct mak__a *)(arena_call_526));
                        }
                        mak_ir_fn_3(arena_call_507, arena_call_526, (struct mak__a *)(arena_case_503));
                    }
                    else if (1) {
                        int64_t case_531 = 0;
                        int64_t scrut_532 = (a1 & INT64_C(2));
                        if ((scrut_532 == INT64_C(0))) {
                            case_531 = INT64_C(0);
                        }
                        else if (1) {
                            case_531 = INT64_C(1);
                        }
                        int64_t scrut_530 = case_531;
                        if ((scrut_530 == INT64_C(1))) {
                            struct mak__a *arena_call_533 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
                            if (!arena_call_533) {
                                mak_ir_recover("ADT result allocation failed");
                                arena_call_533 = &mak_ir_default_obj__a;
                             } else {
                                int64_t case_534 = 0;
                                int32_t scrut_535 = (int32_t)(a3) < (int32_t)(INT64_C(0));
                                if ((scrut_535 == INT64_C(1))) {
                                    case_534 = INT64_C(6);
                                }
                                else if (1) {
                                    case_534 = a3;
                                }
                                mak_ir_fn_13(mak_ir_ext_4(mak_local_gd_105), case_534, (struct mak__a *)(arena_call_533));
                            }
                            struct mak__a *arena_call_536 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
                            if (!arena_call_536) {
                                mak_ir_recover("ADT result allocation failed");
                                arena_call_536 = &mak_ir_default_obj__a;
                             } else {
                                struct mak__a *arena_call_537 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
                                if (!arena_call_537) {
                                    mak_ir_recover("ADT result allocation failed");
                                    arena_call_537 = &mak_ir_default_obj__a;
                                 } else {
                                    int64_t case_538 = 0;
                                    struct mak__a *arena_call_542 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
                                    if (!arena_call_542) {
                                        mak_ir_recover("ADT result allocation failed");
                                        arena_call_542 = &mak_ir_default_obj__a;
                                     } else {
                                        int64_t case_543 = 0;
                                        int32_t scrut_544 = (int32_t)(a3) < (int32_t)(INT64_C(0));
                                        if ((scrut_544 == INT64_C(1))) {
                                            case_543 = INT64_C(6);
                                        }
                                        else if (1) {
                                            case_543 = a3;
                                        }
                                        mak_ir_fn_13(mak_ir_ext_4(mak_local_gd_105), case_543, (struct mak__a *)(arena_call_542));
                                    }
                                    int64_t eval_541 = mak_ir_fn_1(arena_call_542, INT64_C(0));
                                    int64_t case_546 = 0;
                                    int32_t scrut_547 = (int32_t)(mak_local_dw_106) == (int32_t)(INT64_C(0));
                                    if ((scrut_547 == INT64_C(1))) {
                                        case_546 = INT64_C(0);
                                    }
                                    else if (1) {
                                        case_546 = INT64_C(1);
                                    }
                                    int64_t eval_545 = case_546;
                                    int64_t eval_540 = (eval_541 + eval_545);
                                    int32_t scrut_539 = (int32_t)(a2) <= (int32_t)(eval_540);
                                    if ((scrut_539 == INT64_C(1))) {
                                        case_538 = INT64_C(0);
                                    }
                                    else if (1) {
                                        struct mak__a *arena_call_550 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
                                        if (!arena_call_550) {
                                            mak_ir_recover("ADT result allocation failed");
                                            arena_call_550 = &mak_ir_default_obj__a;
                                         } else {
                                            int64_t case_551 = 0;
                                            int32_t scrut_552 = (int32_t)(a3) < (int32_t)(INT64_C(0));
                                            if ((scrut_552 == INT64_C(1))) {
                                                case_551 = INT64_C(6);
                                            }
                                            else if (1) {
                                                case_551 = a3;
                                            }
                                            mak_ir_fn_13(mak_ir_ext_4(mak_local_gd_105), case_551, (struct mak__a *)(arena_call_550));
                                        }
                                        int64_t eval_549 = mak_ir_fn_1(arena_call_550, INT64_C(0));
                                        int64_t case_554 = 0;
                                        int32_t scrut_555 = (int32_t)(mak_local_dw_106) == (int32_t)(INT64_C(0));
                                        if ((scrut_555 == INT64_C(1))) {
                                            case_554 = INT64_C(0);
                                        }
                                        else if (1) {
                                            case_554 = INT64_C(1);
                                        }
                                        int64_t eval_553 = case_554;
                                        int64_t eval_548 = (eval_549 + eval_553);
                                        case_538 = (a2 - eval_548);
                                    }
                                    mak_ir_fn_4(INT64_C(48), case_538, (struct mak__a *)(arena_call_537));
                                }
                                mak_ir_fn_3(arena_call_537, mak_local_ea_107, (struct mak__a *)(arena_call_536));
                            }
                            mak_ir_fn_3(arena_call_533, arena_call_536, (struct mak__a *)(arena_case_503));
                        }
                        else if (1) {
                            struct mak__a *arena_call_556 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
                            if (!arena_call_556) {
                                mak_ir_recover("ADT result allocation failed");
                                arena_call_556 = &mak_ir_default_obj__a;
                             } else {
                                int64_t case_557 = 0;
                                int32_t scrut_558 = (int32_t)(a3) < (int32_t)(INT64_C(0));
                                if ((scrut_558 == INT64_C(1))) {
                                    case_557 = INT64_C(6);
                                }
                                else if (1) {
                                    case_557 = a3;
                                }
                                mak_ir_fn_13(mak_ir_ext_4(mak_local_gd_105), case_557, (struct mak__a *)(arena_call_556));
                            }
                            struct mak__a *arena_call_559 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
                            if (!arena_call_559) {
                                mak_ir_recover("ADT result allocation failed");
                                arena_call_559 = &mak_ir_default_obj__a;
                             } else {
                                struct mak__a *arena_call_560 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
                                if (!arena_call_560) {
                                    mak_ir_recover("ADT result allocation failed");
                                    arena_call_560 = &mak_ir_default_obj__a;
                                 } else {
                                    int64_t case_561 = 0;
                                    struct mak__a *arena_call_565 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
                                    if (!arena_call_565) {
                                        mak_ir_recover("ADT result allocation failed");
                                        arena_call_565 = &mak_ir_default_obj__a;
                                     } else {
                                        int64_t case_566 = 0;
                                        int32_t scrut_567 = (int32_t)(a3) < (int32_t)(INT64_C(0));
                                        if ((scrut_567 == INT64_C(1))) {
                                            case_566 = INT64_C(6);
                                        }
                                        else if (1) {
                                            case_566 = a3;
                                        }
                                        mak_ir_fn_13(mak_ir_ext_4(mak_local_gd_105), case_566, (struct mak__a *)(arena_call_565));
                                    }
                                    int64_t eval_564 = mak_ir_fn_1(arena_call_565, INT64_C(0));
                                    int64_t case_569 = 0;
                                    int32_t scrut_570 = (int32_t)(mak_local_dw_106) == (int32_t)(INT64_C(0));
                                    if ((scrut_570 == INT64_C(1))) {
                                        case_569 = INT64_C(0);
                                    }
                                    else if (1) {
                                        case_569 = INT64_C(1);
                                    }
                                    int64_t eval_568 = case_569;
                                    int64_t eval_563 = (eval_564 + eval_568);
                                    int32_t scrut_562 = (int32_t)(a2) <= (int32_t)(eval_563);
                                    if ((scrut_562 == INT64_C(1))) {
                                        case_561 = INT64_C(0);
                                    }
                                    else if (1) {
                                        struct mak__a *arena_call_573 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
                                        if (!arena_call_573) {
                                            mak_ir_recover("ADT result allocation failed");
                                            arena_call_573 = &mak_ir_default_obj__a;
                                         } else {
                                            int64_t case_574 = 0;
                                            int32_t scrut_575 = (int32_t)(a3) < (int32_t)(INT64_C(0));
                                            if ((scrut_575 == INT64_C(1))) {
                                                case_574 = INT64_C(6);
                                            }
                                            else if (1) {
                                                case_574 = a3;
                                            }
                                            mak_ir_fn_13(mak_ir_ext_4(mak_local_gd_105), case_574, (struct mak__a *)(arena_call_573));
                                        }
                                        int64_t eval_572 = mak_ir_fn_1(arena_call_573, INT64_C(0));
                                        int64_t case_577 = 0;
                                        int32_t scrut_578 = (int32_t)(mak_local_dw_106) == (int32_t)(INT64_C(0));
                                        if ((scrut_578 == INT64_C(1))) {
                                            case_577 = INT64_C(0);
                                        }
                                        else if (1) {
                                            case_577 = INT64_C(1);
                                        }
                                        int64_t eval_576 = case_577;
                                        int64_t eval_571 = (eval_572 + eval_576);
                                        case_561 = (a2 - eval_571);
                                    }
                                    mak_ir_fn_4(INT64_C(32), case_561, (struct mak__a *)(arena_call_560));
                                }
                                mak_ir_fn_3(mak_local_ea_107, arena_call_560, (struct mak__a *)(arena_call_559));
                            }
                            mak_ir_fn_3(arena_call_556, arena_call_559, (struct mak__a *)(arena_case_503));
                        }
                    }
                }
                mak_ir_fn_3(arena_case_503, (struct mak__a *)mak_ir_adt_recover_ptr((const void *)&mak_ir_default_obj__a, "integer used as ADT"), (struct mak__a *)(out));
            }
            else if (1) {
                (void)(a5);
                mak_ir_recover("non-ADT expression used as ADT result");
                *(out) = *(&mak_ir_default_obj__a);
            }
            break;
        }
        default: {
            (void)(a5);
            mak_ir_recover("non-ADT expression used as ADT result");
            *(out) = *(&mak_ir_default_obj__a);
            break;
        }
    }
    return;
}

static inline void mak_ir_fn_25(int64_t a0, int64_t a1, int64_t a2, int64_t a3, struct mak__a *out) {
    struct mak__a *letobj_579 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
    if (!letobj_579) {
        mak_ir_recover("ADT local allocation failed");
        letobj_579 = &mak_ir_default_obj__a;
     } else {
        *(letobj_579) = *(&mak_ir_default_obj__a);
        struct mak__a *arena_call_580 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
        if (!arena_call_580) {
            mak_ir_recover("ADT result allocation failed");
            arena_call_580 = &mak_ir_default_obj__a;
         } else {
            uint64_t mak_local_cp_108 = mak_ir_ext_1((const void *)((uintptr_t)(a3)));
            int64_t case_582 = 0;
            int32_t scrut_583 = (int32_t)(a2) < (int32_t)(INT64_C(0));
            if ((scrut_583 == INT64_C(1))) {
                case_582 = (int64_t)(mak_local_cp_108);
            }
            else if (1) {
                int64_t case_584 = 0;
                int32_t scrut_585 = (int32_t)(a2) < (int32_t)(mak_local_cp_108);
                if ((scrut_585 == INT64_C(1))) {
                    case_584 = a2;
                }
                else if (1) {
                    case_584 = (int64_t)(mak_local_cp_108);
                }
                case_582 = case_584;
            }
            int64_t eval_581 = case_582;
            mak_ir_fn_11((const void *)((uintptr_t)(a3)), (eval_581 - INT64_C(1)), &mak_ir_ctor_obj_0, (struct mak__a *)(arena_call_580));
        }
        mak_ir_fn_0(arena_call_580, &mak_ir_ctor_obj_0, (struct mak__a *)(letobj_579));
    }
    struct mak__a * mak_local_cv_109 = letobj_579;
    int64_t mak_local_cw_110 = mak_ir_fn_1(mak_local_cv_109, INT64_C(0));
    int64_t case_586 = 0;
    int32_t scrut_587 = (int32_t)(a1) <= (int32_t)(mak_local_cw_110);
    if ((scrut_587 == INT64_C(1))) {
        case_586 = INT64_C(0);
    }
    else if (1) {
        case_586 = (a1 - mak_local_cw_110);
    }
    int64_t mak_local_cx_111 = case_586;
    int64_t case_589 = 0;
    int64_t scrut_590 = (a0 & INT64_C(1));
    if ((scrut_590 == INT64_C(0))) {
        case_589 = INT64_C(0);
    }
    else if (1) {
        case_589 = INT64_C(1);
    }
    int64_t scrut_588 = case_589;
    if ((scrut_588 == INT64_C(1))) {
        struct mak__a *arena_call_591 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
        if (!arena_call_591) {
            mak_ir_recover("ADT result allocation failed");
            arena_call_591 = &mak_ir_default_obj__a;
         } else {
            mak_ir_fn_4(INT64_C(32), mak_local_cx_111, (struct mak__a *)(arena_call_591));
        }
        mak_ir_fn_3(arena_call_591, mak_local_cv_109, (struct mak__a *)(out));
    }
    else if (1) {
        struct mak__a *arena_call_592 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
        if (!arena_call_592) {
            mak_ir_recover("ADT result allocation failed");
            arena_call_592 = &mak_ir_default_obj__a;
         } else {
            mak_ir_fn_4(INT64_C(32), mak_local_cx_111, (struct mak__a *)(arena_call_592));
        }
        mak_ir_fn_3(mak_local_cv_109, arena_call_592, (struct mak__a *)(out));
    }
    return;
}

static inline void mak_ir_fn_26(struct mak__a * a0, int64_t a1, struct mak__a *out) {
    struct mak__a *arena_call_593 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
    if (!arena_call_593) {
        mak_ir_recover("ADT result allocation failed");
        arena_call_593 = &mak_ir_default_obj__a;
     } else {
        uint64_t mak_local_cp_112 = mak_ir_ext_1((const void *)(a0));
        int64_t case_595 = 0;
        int32_t scrut_596 = (int32_t)(a1) < (int32_t)(INT64_C(0));
        if ((scrut_596 == INT64_C(1))) {
            case_595 = (int64_t)(mak_local_cp_112);
        }
        else if (1) {
            int64_t case_597 = 0;
            int32_t scrut_598 = (int32_t)(a1) < (int32_t)(mak_local_cp_112);
            if ((scrut_598 == INT64_C(1))) {
                case_597 = a1;
            }
            else if (1) {
                case_597 = (int64_t)(mak_local_cp_112);
            }
            case_595 = case_597;
        }
        int64_t eval_594 = case_595;
        mak_ir_fn_11((const void *)(a0), (eval_594 - INT64_C(1)), &mak_ir_ctor_obj_0, (struct mak__a *)(arena_call_593));
    }
    mak_ir_fn_0(arena_call_593, &mak_ir_ctor_obj_0, (struct mak__a *)(out));
    return;
}

static inline void mak_ir_fn_27(int64_t a0, int64_t a1, int64_t a2, double a3, struct mak__a *out) {
    int64_t case_599 = 0;
    int32_t scrut_600 = (int32_t)(a3) < (int32_t)(0.00000000000000000);
    if ((scrut_600 == INT64_C(1))) {
        case_599 = (int64_t)((uint64_t)(((int64_t)UINT64_C(18446744073709551615))));
    }
    else if (1) {
        case_599 = INT64_C(0);
    }
    int64_t mak_local_dw_113 = mak_ir_fn_8(a0, case_599);
    struct mak__a *letobj_601 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
    if (!letobj_601) {
        mak_ir_recover("ADT local allocation failed");
        letobj_601 = &mak_ir_default_obj__a;
     } else {
        *(letobj_601) = *(&mak_ir_default_obj__a);
        int32_t scrut_602 = (int32_t)(mak_local_dw_113) == (int32_t)(INT64_C(0));
        if ((scrut_602 == INT64_C(1))) {
            *(letobj_601) = mak_ir_ctor_obj_0;
        }
        else if (1) {
            (letobj_601)->tag = mak_ir_tag_1;
            (letobj_601)->v.v1.f0 = (int32_t)(mak_local_dw_113);
            (letobj_601)->v.v1.f1 = &mak_ir_ctor_obj_0;
        }
    }
    struct mak__a * mak_local_ea_114 = letobj_601;
    int64_t case_604 = 0;
    int64_t scrut_605 = (a0 & INT64_C(1));
    if ((scrut_605 == INT64_C(0))) {
        case_604 = INT64_C(0);
    }
    else if (1) {
        case_604 = INT64_C(1);
    }
    int64_t scrut_603 = case_604;
    if ((scrut_603 == INT64_C(1))) {
        struct mak__a *arena_call_606 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
        if (!arena_call_606) {
            mak_ir_recover("ADT result allocation failed");
            arena_call_606 = &mak_ir_default_obj__a;
         } else {
            int64_t case_607 = 0;
            struct mak__a *arena_call_611 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
            if (!arena_call_611) {
                mak_ir_recover("ADT result allocation failed");
                arena_call_611 = &mak_ir_default_obj__a;
             } else {
                int64_t case_612 = 0;
                int32_t scrut_613 = (int32_t)(a2) < (int32_t)(INT64_C(0));
                if ((scrut_613 == INT64_C(1))) {
                    case_612 = INT64_C(6);
                }
                else if (1) {
                    case_612 = a2;
                }
                mak_ir_fn_13(mak_ir_ext_4(a3), case_612, (struct mak__a *)(arena_call_611));
            }
            int64_t eval_610 = mak_ir_fn_1(arena_call_611, INT64_C(0));
            int64_t case_615 = 0;
            int32_t scrut_616 = (int32_t)(mak_local_dw_113) == (int32_t)(INT64_C(0));
            if ((scrut_616 == INT64_C(1))) {
                case_615 = INT64_C(0);
            }
            else if (1) {
                case_615 = INT64_C(1);
            }
            int64_t eval_614 = case_615;
            int64_t eval_609 = (eval_610 + eval_614);
            int32_t scrut_608 = (int32_t)(a1) <= (int32_t)(eval_609);
            if ((scrut_608 == INT64_C(1))) {
                case_607 = INT64_C(0);
            }
            else if (1) {
                struct mak__a *arena_call_619 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
                if (!arena_call_619) {
                    mak_ir_recover("ADT result allocation failed");
                    arena_call_619 = &mak_ir_default_obj__a;
                 } else {
                    int64_t case_620 = 0;
                    int32_t scrut_621 = (int32_t)(a2) < (int32_t)(INT64_C(0));
                    if ((scrut_621 == INT64_C(1))) {
                        case_620 = INT64_C(6);
                    }
                    else if (1) {
                        case_620 = a2;
                    }
                    mak_ir_fn_13(mak_ir_ext_4(a3), case_620, (struct mak__a *)(arena_call_619));
                }
                int64_t eval_618 = mak_ir_fn_1(arena_call_619, INT64_C(0));
                int64_t case_623 = 0;
                int32_t scrut_624 = (int32_t)(mak_local_dw_113) == (int32_t)(INT64_C(0));
                if ((scrut_624 == INT64_C(1))) {
                    case_623 = INT64_C(0);
                }
                else if (1) {
                    case_623 = INT64_C(1);
                }
                int64_t eval_622 = case_623;
                int64_t eval_617 = (eval_618 + eval_622);
                case_607 = (a1 - eval_617);
            }
            mak_ir_fn_4(INT64_C(32), case_607, (struct mak__a *)(arena_call_606));
        }
        struct mak__a *arena_call_625 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
        if (!arena_call_625) {
            mak_ir_recover("ADT result allocation failed");
            arena_call_625 = &mak_ir_default_obj__a;
         } else {
            struct mak__a *arena_call_626 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
            if (!arena_call_626) {
                mak_ir_recover("ADT result allocation failed");
                arena_call_626 = &mak_ir_default_obj__a;
             } else {
                int64_t case_627 = 0;
                int32_t scrut_628 = (int32_t)(a2) < (int32_t)(INT64_C(0));
                if ((scrut_628 == INT64_C(1))) {
                    case_627 = INT64_C(6);
                }
                else if (1) {
                    case_627 = a2;
                }
                mak_ir_fn_13(mak_ir_ext_4(a3), case_627, (struct mak__a *)(arena_call_626));
            }
            mak_ir_fn_3(arena_call_626, mak_local_ea_114, (struct mak__a *)(arena_call_625));
        }
        mak_ir_fn_3(arena_call_606, arena_call_625, (struct mak__a *)(out));
    }
    else if (1) {
        int64_t case_630 = 0;
        int64_t scrut_631 = (a0 & INT64_C(2));
        if ((scrut_631 == INT64_C(0))) {
            case_630 = INT64_C(0);
        }
        else if (1) {
            case_630 = INT64_C(1);
        }
        int64_t scrut_629 = case_630;
        if ((scrut_629 == INT64_C(1))) {
            struct mak__a *arena_call_632 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
            if (!arena_call_632) {
                mak_ir_recover("ADT result allocation failed");
                arena_call_632 = &mak_ir_default_obj__a;
             } else {
                int64_t case_633 = 0;
                int32_t scrut_634 = (int32_t)(a2) < (int32_t)(INT64_C(0));
                if ((scrut_634 == INT64_C(1))) {
                    case_633 = INT64_C(6);
                }
                else if (1) {
                    case_633 = a2;
                }
                mak_ir_fn_13(mak_ir_ext_4(a3), case_633, (struct mak__a *)(arena_call_632));
            }
            struct mak__a *arena_call_635 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
            if (!arena_call_635) {
                mak_ir_recover("ADT result allocation failed");
                arena_call_635 = &mak_ir_default_obj__a;
             } else {
                struct mak__a *arena_call_636 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
                if (!arena_call_636) {
                    mak_ir_recover("ADT result allocation failed");
                    arena_call_636 = &mak_ir_default_obj__a;
                 } else {
                    int64_t case_637 = 0;
                    struct mak__a *arena_call_641 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
                    if (!arena_call_641) {
                        mak_ir_recover("ADT result allocation failed");
                        arena_call_641 = &mak_ir_default_obj__a;
                     } else {
                        int64_t case_642 = 0;
                        int32_t scrut_643 = (int32_t)(a2) < (int32_t)(INT64_C(0));
                        if ((scrut_643 == INT64_C(1))) {
                            case_642 = INT64_C(6);
                        }
                        else if (1) {
                            case_642 = a2;
                        }
                        mak_ir_fn_13(mak_ir_ext_4(a3), case_642, (struct mak__a *)(arena_call_641));
                    }
                    int64_t eval_640 = mak_ir_fn_1(arena_call_641, INT64_C(0));
                    int64_t case_645 = 0;
                    int32_t scrut_646 = (int32_t)(mak_local_dw_113) == (int32_t)(INT64_C(0));
                    if ((scrut_646 == INT64_C(1))) {
                        case_645 = INT64_C(0);
                    }
                    else if (1) {
                        case_645 = INT64_C(1);
                    }
                    int64_t eval_644 = case_645;
                    int64_t eval_639 = (eval_640 + eval_644);
                    int32_t scrut_638 = (int32_t)(a1) <= (int32_t)(eval_639);
                    if ((scrut_638 == INT64_C(1))) {
                        case_637 = INT64_C(0);
                    }
                    else if (1) {
                        struct mak__a *arena_call_649 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
                        if (!arena_call_649) {
                            mak_ir_recover("ADT result allocation failed");
                            arena_call_649 = &mak_ir_default_obj__a;
                         } else {
                            int64_t case_650 = 0;
                            int32_t scrut_651 = (int32_t)(a2) < (int32_t)(INT64_C(0));
                            if ((scrut_651 == INT64_C(1))) {
                                case_650 = INT64_C(6);
                            }
                            else if (1) {
                                case_650 = a2;
                            }
                            mak_ir_fn_13(mak_ir_ext_4(a3), case_650, (struct mak__a *)(arena_call_649));
                        }
                        int64_t eval_648 = mak_ir_fn_1(arena_call_649, INT64_C(0));
                        int64_t case_653 = 0;
                        int32_t scrut_654 = (int32_t)(mak_local_dw_113) == (int32_t)(INT64_C(0));
                        if ((scrut_654 == INT64_C(1))) {
                            case_653 = INT64_C(0);
                        }
                        else if (1) {
                            case_653 = INT64_C(1);
                        }
                        int64_t eval_652 = case_653;
                        int64_t eval_647 = (eval_648 + eval_652);
                        case_637 = (a1 - eval_647);
                    }
                    mak_ir_fn_4(INT64_C(48), case_637, (struct mak__a *)(arena_call_636));
                }
                mak_ir_fn_3(arena_call_636, mak_local_ea_114, (struct mak__a *)(arena_call_635));
            }
            mak_ir_fn_3(arena_call_632, arena_call_635, (struct mak__a *)(out));
        }
        else if (1) {
            struct mak__a *arena_call_655 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
            if (!arena_call_655) {
                mak_ir_recover("ADT result allocation failed");
                arena_call_655 = &mak_ir_default_obj__a;
             } else {
                int64_t case_656 = 0;
                int32_t scrut_657 = (int32_t)(a2) < (int32_t)(INT64_C(0));
                if ((scrut_657 == INT64_C(1))) {
                    case_656 = INT64_C(6);
                }
                else if (1) {
                    case_656 = a2;
                }
                mak_ir_fn_13(mak_ir_ext_4(a3), case_656, (struct mak__a *)(arena_call_655));
            }
            struct mak__a *arena_call_658 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
            if (!arena_call_658) {
                mak_ir_recover("ADT result allocation failed");
                arena_call_658 = &mak_ir_default_obj__a;
             } else {
                struct mak__a *arena_call_659 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
                if (!arena_call_659) {
                    mak_ir_recover("ADT result allocation failed");
                    arena_call_659 = &mak_ir_default_obj__a;
                 } else {
                    int64_t case_660 = 0;
                    struct mak__a *arena_call_664 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
                    if (!arena_call_664) {
                        mak_ir_recover("ADT result allocation failed");
                        arena_call_664 = &mak_ir_default_obj__a;
                     } else {
                        int64_t case_665 = 0;
                        int32_t scrut_666 = (int32_t)(a2) < (int32_t)(INT64_C(0));
                        if ((scrut_666 == INT64_C(1))) {
                            case_665 = INT64_C(6);
                        }
                        else if (1) {
                            case_665 = a2;
                        }
                        mak_ir_fn_13(mak_ir_ext_4(a3), case_665, (struct mak__a *)(arena_call_664));
                    }
                    int64_t eval_663 = mak_ir_fn_1(arena_call_664, INT64_C(0));
                    int64_t case_668 = 0;
                    int32_t scrut_669 = (int32_t)(mak_local_dw_113) == (int32_t)(INT64_C(0));
                    if ((scrut_669 == INT64_C(1))) {
                        case_668 = INT64_C(0);
                    }
                    else if (1) {
                        case_668 = INT64_C(1);
                    }
                    int64_t eval_667 = case_668;
                    int64_t eval_662 = (eval_663 + eval_667);
                    int32_t scrut_661 = (int32_t)(a1) <= (int32_t)(eval_662);
                    if ((scrut_661 == INT64_C(1))) {
                        case_660 = INT64_C(0);
                    }
                    else if (1) {
                        struct mak__a *arena_call_672 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
                        if (!arena_call_672) {
                            mak_ir_recover("ADT result allocation failed");
                            arena_call_672 = &mak_ir_default_obj__a;
                         } else {
                            int64_t case_673 = 0;
                            int32_t scrut_674 = (int32_t)(a2) < (int32_t)(INT64_C(0));
                            if ((scrut_674 == INT64_C(1))) {
                                case_673 = INT64_C(6);
                            }
                            else if (1) {
                                case_673 = a2;
                            }
                            mak_ir_fn_13(mak_ir_ext_4(a3), case_673, (struct mak__a *)(arena_call_672));
                        }
                        int64_t eval_671 = mak_ir_fn_1(arena_call_672, INT64_C(0));
                        int64_t case_676 = 0;
                        int32_t scrut_677 = (int32_t)(mak_local_dw_113) == (int32_t)(INT64_C(0));
                        if ((scrut_677 == INT64_C(1))) {
                            case_676 = INT64_C(0);
                        }
                        else if (1) {
                            case_676 = INT64_C(1);
                        }
                        int64_t eval_675 = case_676;
                        int64_t eval_670 = (eval_671 + eval_675);
                        case_660 = (a1 - eval_670);
                    }
                    mak_ir_fn_4(INT64_C(32), case_660, (struct mak__a *)(arena_call_659));
                }
                mak_ir_fn_3(mak_local_ea_114, arena_call_659, (struct mak__a *)(arena_call_658));
            }
            mak_ir_fn_3(arena_call_655, arena_call_658, (struct mak__a *)(out));
        }
    }
    return;
}

static inline int64_t mak_ir_fn_28(void) {
    mak_ir_fn_19("==================================================", &mak_ir_ctor_obj_12);
    mak_ir_fn_19("  pureprintf.mak \342\200\224 a printf in pure MAK", &mak_ir_ctor_obj_12);
    mak_ir_fn_19("==================================================", &mak_ir_ctor_obj_12);
    mak_ir_fn_19("", &mak_ir_ctor_obj_12);
    mak_ir_fn_19("Basic:", &mak_ir_ctor_obj_12);
    struct mak__c *arena_obj_678 = (struct mak__c *)mak_ir_arena_alloc(sizeof(struct mak__c), _Alignof(struct mak__c));
    if (!arena_obj_678) {
        mak_ir_recover("ADT object allocation failed");
        arena_obj_678 = &mak_ir_default_obj__c;
     } else {
        (arena_obj_678)->tag = mak_ir_tag_13;
        struct mak__b *arena_obj_679 = (struct mak__b *)mak_ir_arena_alloc(sizeof(struct mak__b), _Alignof(struct mak__b));
        if (!arena_obj_679) {
            mak_ir_recover("ADT object allocation failed");
            arena_obj_679 = &mak_ir_default_obj__b;
         } else {
            (arena_obj_679)->tag = mak_ir_tag_9;
            (arena_obj_679)->v.v9.f0 = "Alice";
        }
        (arena_obj_678)->v.v13.f0 = arena_obj_679;
        struct mak__c *arena_obj_680 = (struct mak__c *)mak_ir_arena_alloc(sizeof(struct mak__c), _Alignof(struct mak__c));
        if (!arena_obj_680) {
            mak_ir_recover("ADT object allocation failed");
            arena_obj_680 = &mak_ir_default_obj__c;
         } else {
            (arena_obj_680)->tag = mak_ir_tag_13;
            struct mak__b *arena_obj_681 = (struct mak__b *)mak_ir_arena_alloc(sizeof(struct mak__b), _Alignof(struct mak__b));
            if (!arena_obj_681) {
                mak_ir_recover("ADT object allocation failed");
                arena_obj_681 = &mak_ir_default_obj__b;
             } else {
                (arena_obj_681)->tag = mak_ir_tag_2;
                (arena_obj_681)->v.v2.f0 = INT64_C(30);
            }
            (arena_obj_680)->v.v13.f0 = arena_obj_681;
            struct mak__c *arena_obj_682 = (struct mak__c *)mak_ir_arena_alloc(sizeof(struct mak__c), _Alignof(struct mak__c));
            if (!arena_obj_682) {
                mak_ir_recover("ADT object allocation failed");
                arena_obj_682 = &mak_ir_default_obj__c;
             } else {
                (arena_obj_682)->tag = mak_ir_tag_13;
                struct mak__b *arena_obj_683 = (struct mak__b *)mak_ir_arena_alloc(sizeof(struct mak__b), _Alignof(struct mak__b));
                if (!arena_obj_683) {
                    mak_ir_recover("ADT object allocation failed");
                    arena_obj_683 = &mak_ir_default_obj__b;
                 } else {
                    (arena_obj_683)->tag = mak_ir_tag_8;
                    (arena_obj_683)->v.v8.f0 = (int32_t)(INT64_C(65));
                }
                (arena_obj_682)->v.v13.f0 = arena_obj_683;
                (arena_obj_682)->v.v13.f1 = &mak_ir_ctor_obj_12;
            }
            (arena_obj_680)->v.v13.f1 = arena_obj_682;
        }
        (arena_obj_678)->v.v13.f1 = arena_obj_680;
    }
    mak_ir_fn_19("  name=%s age=%d grade=%c", arena_obj_678);
    mak_ir_fn_19("", &mak_ir_ctor_obj_12);
    mak_ir_fn_19("Widths and flags:", &mak_ir_ctor_obj_12);
    struct mak__c *arena_obj_684 = (struct mak__c *)mak_ir_arena_alloc(sizeof(struct mak__c), _Alignof(struct mak__c));
    if (!arena_obj_684) {
        mak_ir_recover("ADT object allocation failed");
        arena_obj_684 = &mak_ir_default_obj__c;
     } else {
        (arena_obj_684)->tag = mak_ir_tag_13;
        struct mak__b *arena_obj_685 = (struct mak__b *)mak_ir_arena_alloc(sizeof(struct mak__b), _Alignof(struct mak__b));
        if (!arena_obj_685) {
            mak_ir_recover("ADT object allocation failed");
            arena_obj_685 = &mak_ir_default_obj__b;
         } else {
            (arena_obj_685)->tag = mak_ir_tag_2;
            (arena_obj_685)->v.v2.f0 = INT64_C(42);
        }
        (arena_obj_684)->v.v13.f0 = arena_obj_685;
        (arena_obj_684)->v.v13.f1 = &mak_ir_ctor_obj_12;
    }
    mak_ir_fn_19("  right: [%10d]", arena_obj_684);
    struct mak__c *arena_obj_686 = (struct mak__c *)mak_ir_arena_alloc(sizeof(struct mak__c), _Alignof(struct mak__c));
    if (!arena_obj_686) {
        mak_ir_recover("ADT object allocation failed");
        arena_obj_686 = &mak_ir_default_obj__c;
     } else {
        (arena_obj_686)->tag = mak_ir_tag_13;
        struct mak__b *arena_obj_687 = (struct mak__b *)mak_ir_arena_alloc(sizeof(struct mak__b), _Alignof(struct mak__b));
        if (!arena_obj_687) {
            mak_ir_recover("ADT object allocation failed");
            arena_obj_687 = &mak_ir_default_obj__b;
         } else {
            (arena_obj_687)->tag = mak_ir_tag_2;
            (arena_obj_687)->v.v2.f0 = INT64_C(42);
        }
        (arena_obj_686)->v.v13.f0 = arena_obj_687;
        (arena_obj_686)->v.v13.f1 = &mak_ir_ctor_obj_12;
    }
    mak_ir_fn_19("  left:  [%-10d]", arena_obj_686);
    struct mak__c *arena_obj_688 = (struct mak__c *)mak_ir_arena_alloc(sizeof(struct mak__c), _Alignof(struct mak__c));
    if (!arena_obj_688) {
        mak_ir_recover("ADT object allocation failed");
        arena_obj_688 = &mak_ir_default_obj__c;
     } else {
        (arena_obj_688)->tag = mak_ir_tag_13;
        struct mak__b *arena_obj_689 = (struct mak__b *)mak_ir_arena_alloc(sizeof(struct mak__b), _Alignof(struct mak__b));
        if (!arena_obj_689) {
            mak_ir_recover("ADT object allocation failed");
            arena_obj_689 = &mak_ir_default_obj__b;
         } else {
            (arena_obj_689)->tag = mak_ir_tag_2;
            (arena_obj_689)->v.v2.f0 = INT64_C(42);
        }
        (arena_obj_688)->v.v13.f0 = arena_obj_689;
        (arena_obj_688)->v.v13.f1 = &mak_ir_ctor_obj_12;
    }
    mak_ir_fn_19("  zero:  [%010d]", arena_obj_688);
    struct mak__c *arena_obj_690 = (struct mak__c *)mak_ir_arena_alloc(sizeof(struct mak__c), _Alignof(struct mak__c));
    if (!arena_obj_690) {
        mak_ir_recover("ADT object allocation failed");
        arena_obj_690 = &mak_ir_default_obj__c;
     } else {
        (arena_obj_690)->tag = mak_ir_tag_13;
        struct mak__b *arena_obj_691 = (struct mak__b *)mak_ir_arena_alloc(sizeof(struct mak__b), _Alignof(struct mak__b));
        if (!arena_obj_691) {
            mak_ir_recover("ADT object allocation failed");
            arena_obj_691 = &mak_ir_default_obj__b;
         } else {
            (arena_obj_691)->tag = mak_ir_tag_2;
            (arena_obj_691)->v.v2.f0 = INT64_C(42);
        }
        (arena_obj_690)->v.v13.f0 = arena_obj_691;
        (arena_obj_690)->v.v13.f1 = &mak_ir_ctor_obj_12;
    }
    mak_ir_fn_19("  plus:  [%+d]", arena_obj_690);
    struct mak__c *arena_obj_692 = (struct mak__c *)mak_ir_arena_alloc(sizeof(struct mak__c), _Alignof(struct mak__c));
    if (!arena_obj_692) {
        mak_ir_recover("ADT object allocation failed");
        arena_obj_692 = &mak_ir_default_obj__c;
     } else {
        (arena_obj_692)->tag = mak_ir_tag_13;
        struct mak__b *arena_obj_693 = (struct mak__b *)mak_ir_arena_alloc(sizeof(struct mak__b), _Alignof(struct mak__b));
        if (!arena_obj_693) {
            mak_ir_recover("ADT object allocation failed");
            arena_obj_693 = &mak_ir_default_obj__b;
         } else {
            (arena_obj_693)->tag = mak_ir_tag_2;
            (arena_obj_693)->v.v2.f0 = INT64_C(42);
        }
        (arena_obj_692)->v.v13.f0 = arena_obj_693;
        (arena_obj_692)->v.v13.f1 = &mak_ir_ctor_obj_12;
    }
    mak_ir_fn_19("  space: [% d]", arena_obj_692);
    struct mak__c *arena_obj_694 = (struct mak__c *)mak_ir_arena_alloc(sizeof(struct mak__c), _Alignof(struct mak__c));
    if (!arena_obj_694) {
        mak_ir_recover("ADT object allocation failed");
        arena_obj_694 = &mak_ir_default_obj__c;
     } else {
        (arena_obj_694)->tag = mak_ir_tag_13;
        struct mak__b *arena_obj_695 = (struct mak__b *)mak_ir_arena_alloc(sizeof(struct mak__b), _Alignof(struct mak__b));
        if (!arena_obj_695) {
            mak_ir_recover("ADT object allocation failed");
            arena_obj_695 = &mak_ir_default_obj__b;
         } else {
            (arena_obj_695)->tag = mak_ir_tag_2;
            (arena_obj_695)->v.v2.f0 = (int64_t)(((int64_t)UINT64_C(18446744073709551609)));
        }
        (arena_obj_694)->v.v13.f0 = arena_obj_695;
        (arena_obj_694)->v.v13.f1 = &mak_ir_ctor_obj_12;
    }
    mak_ir_fn_19("  neg:   [%d]", arena_obj_694);
    mak_ir_fn_19("", &mak_ir_ctor_obj_12);
    mak_ir_fn_19("Bases:", &mak_ir_ctor_obj_12);
    struct mak__c *arena_obj_696 = (struct mak__c *)mak_ir_arena_alloc(sizeof(struct mak__c), _Alignof(struct mak__c));
    if (!arena_obj_696) {
        mak_ir_recover("ADT object allocation failed");
        arena_obj_696 = &mak_ir_default_obj__c;
     } else {
        (arena_obj_696)->tag = mak_ir_tag_13;
        struct mak__b *arena_obj_697 = (struct mak__b *)mak_ir_arena_alloc(sizeof(struct mak__b), _Alignof(struct mak__b));
        if (!arena_obj_697) {
            mak_ir_recover("ADT object allocation failed");
            arena_obj_697 = &mak_ir_default_obj__b;
         } else {
            (arena_obj_697)->tag = mak_ir_tag_4;
            (arena_obj_697)->v.v4.f0 = (uint64_t)(INT64_C(3735928559));
        }
        (arena_obj_696)->v.v13.f0 = arena_obj_697;
        (arena_obj_696)->v.v13.f1 = &mak_ir_ctor_obj_12;
    }
    mak_ir_fn_19("  hex:  0x%x", arena_obj_696);
    struct mak__c *arena_obj_698 = (struct mak__c *)mak_ir_arena_alloc(sizeof(struct mak__c), _Alignof(struct mak__c));
    if (!arena_obj_698) {
        mak_ir_recover("ADT object allocation failed");
        arena_obj_698 = &mak_ir_default_obj__c;
     } else {
        (arena_obj_698)->tag = mak_ir_tag_13;
        struct mak__b *arena_obj_699 = (struct mak__b *)mak_ir_arena_alloc(sizeof(struct mak__b), _Alignof(struct mak__b));
        if (!arena_obj_699) {
            mak_ir_recover("ADT object allocation failed");
            arena_obj_699 = &mak_ir_default_obj__b;
         } else {
            (arena_obj_699)->tag = mak_ir_tag_5;
            (arena_obj_699)->v.v5.f0 = (uint64_t)(INT64_C(3735928559));
        }
        (arena_obj_698)->v.v13.f0 = arena_obj_699;
        (arena_obj_698)->v.v13.f1 = &mak_ir_ctor_obj_12;
    }
    mak_ir_fn_19("  HEX:  0x%X", arena_obj_698);
    struct mak__c *arena_obj_700 = (struct mak__c *)mak_ir_arena_alloc(sizeof(struct mak__c), _Alignof(struct mak__c));
    if (!arena_obj_700) {
        mak_ir_recover("ADT object allocation failed");
        arena_obj_700 = &mak_ir_default_obj__c;
     } else {
        (arena_obj_700)->tag = mak_ir_tag_13;
        struct mak__b *arena_obj_701 = (struct mak__b *)mak_ir_arena_alloc(sizeof(struct mak__b), _Alignof(struct mak__b));
        if (!arena_obj_701) {
            mak_ir_recover("ADT object allocation failed");
            arena_obj_701 = &mak_ir_default_obj__b;
         } else {
            (arena_obj_701)->tag = mak_ir_tag_6;
            (arena_obj_701)->v.v6.f0 = (uint64_t)(INT64_C(493));
        }
        (arena_obj_700)->v.v13.f0 = arena_obj_701;
        (arena_obj_700)->v.v13.f1 = &mak_ir_ctor_obj_12;
    }
    mak_ir_fn_19("  oct:  %o", arena_obj_700);
    struct mak__c *arena_obj_702 = (struct mak__c *)mak_ir_arena_alloc(sizeof(struct mak__c), _Alignof(struct mak__c));
    if (!arena_obj_702) {
        mak_ir_recover("ADT object allocation failed");
        arena_obj_702 = &mak_ir_default_obj__c;
     } else {
        (arena_obj_702)->tag = mak_ir_tag_13;
        struct mak__b *arena_obj_703 = (struct mak__b *)mak_ir_arena_alloc(sizeof(struct mak__b), _Alignof(struct mak__b));
        if (!arena_obj_703) {
            mak_ir_recover("ADT object allocation failed");
            arena_obj_703 = &mak_ir_default_obj__b;
         } else {
            (arena_obj_703)->tag = mak_ir_tag_7;
            (arena_obj_703)->v.v7.f0 = (uint64_t)(INT64_C(170));
        }
        (arena_obj_702)->v.v13.f0 = arena_obj_703;
        (arena_obj_702)->v.v13.f1 = &mak_ir_ctor_obj_12;
    }
    mak_ir_fn_19("  bin:  %b", arena_obj_702);
    struct mak__c *arena_obj_704 = (struct mak__c *)mak_ir_arena_alloc(sizeof(struct mak__c), _Alignof(struct mak__c));
    if (!arena_obj_704) {
        mak_ir_recover("ADT object allocation failed");
        arena_obj_704 = &mak_ir_default_obj__c;
     } else {
        (arena_obj_704)->tag = mak_ir_tag_13;
        struct mak__b *arena_obj_705 = (struct mak__b *)mak_ir_arena_alloc(sizeof(struct mak__b), _Alignof(struct mak__b));
        if (!arena_obj_705) {
            mak_ir_recover("ADT object allocation failed");
            arena_obj_705 = &mak_ir_default_obj__b;
         } else {
            (arena_obj_705)->tag = mak_ir_tag_10;
            (arena_obj_705)->v.v10.f0 = (const void *)((uintptr_t)(INT64_C(48879)));
        }
        (arena_obj_704)->v.v13.f0 = arena_obj_705;
        (arena_obj_704)->v.v13.f1 = &mak_ir_ctor_obj_12;
    }
    mak_ir_fn_19("  ptr:  %p", arena_obj_704);
    mak_ir_fn_19("", &mak_ir_ctor_obj_12);
    mak_ir_fn_19("Floats:", &mak_ir_ctor_obj_12);
    struct mak__c *arena_obj_706 = (struct mak__c *)mak_ir_arena_alloc(sizeof(struct mak__c), _Alignof(struct mak__c));
    if (!arena_obj_706) {
        mak_ir_recover("ADT object allocation failed");
        arena_obj_706 = &mak_ir_default_obj__c;
     } else {
        (arena_obj_706)->tag = mak_ir_tag_13;
        struct mak__b *arena_obj_707 = (struct mak__b *)mak_ir_arena_alloc(sizeof(struct mak__b), _Alignof(struct mak__b));
        if (!arena_obj_707) {
            mak_ir_recover("ADT object allocation failed");
            arena_obj_707 = &mak_ir_default_obj__b;
         } else {
            (arena_obj_707)->tag = mak_ir_tag_11;
            (arena_obj_707)->v.v11.f0 = 3.14159265358979001;
        }
        (arena_obj_706)->v.v13.f0 = arena_obj_707;
        (arena_obj_706)->v.v13.f1 = &mak_ir_ctor_obj_12;
    }
    mak_ir_fn_19("  pi        = %f", arena_obj_706);
    struct mak__c *arena_obj_708 = (struct mak__c *)mak_ir_arena_alloc(sizeof(struct mak__c), _Alignof(struct mak__c));
    if (!arena_obj_708) {
        mak_ir_recover("ADT object allocation failed");
        arena_obj_708 = &mak_ir_default_obj__c;
     } else {
        (arena_obj_708)->tag = mak_ir_tag_13;
        struct mak__b *arena_obj_709 = (struct mak__b *)mak_ir_arena_alloc(sizeof(struct mak__b), _Alignof(struct mak__b));
        if (!arena_obj_709) {
            mak_ir_recover("ADT object allocation failed");
            arena_obj_709 = &mak_ir_default_obj__b;
         } else {
            (arena_obj_709)->tag = mak_ir_tag_11;
            (arena_obj_709)->v.v11.f0 = 3.14159265358979001;
        }
        (arena_obj_708)->v.v13.f0 = arena_obj_709;
        (arena_obj_708)->v.v13.f1 = &mak_ir_ctor_obj_12;
    }
    mak_ir_fn_19("  pi(2)     = %.2f", arena_obj_708);
    struct mak__c *arena_obj_710 = (struct mak__c *)mak_ir_arena_alloc(sizeof(struct mak__c), _Alignof(struct mak__c));
    if (!arena_obj_710) {
        mak_ir_recover("ADT object allocation failed");
        arena_obj_710 = &mak_ir_default_obj__c;
     } else {
        (arena_obj_710)->tag = mak_ir_tag_13;
        struct mak__b *arena_obj_711 = (struct mak__b *)mak_ir_arena_alloc(sizeof(struct mak__b), _Alignof(struct mak__b));
        if (!arena_obj_711) {
            mak_ir_recover("ADT object allocation failed");
            arena_obj_711 = &mak_ir_default_obj__b;
         } else {
            (arena_obj_711)->tag = mak_ir_tag_11;
            (arena_obj_711)->v.v11.f0 = 3.14159265358979001;
        }
        (arena_obj_710)->v.v13.f0 = arena_obj_711;
        (arena_obj_710)->v.v13.f1 = &mak_ir_ctor_obj_12;
    }
    mak_ir_fn_19("  pi(0)     = %.0f", arena_obj_710);
    struct mak__c *arena_obj_712 = (struct mak__c *)mak_ir_arena_alloc(sizeof(struct mak__c), _Alignof(struct mak__c));
    if (!arena_obj_712) {
        mak_ir_recover("ADT object allocation failed");
        arena_obj_712 = &mak_ir_default_obj__c;
     } else {
        (arena_obj_712)->tag = mak_ir_tag_13;
        struct mak__b *arena_obj_713 = (struct mak__b *)mak_ir_arena_alloc(sizeof(struct mak__b), _Alignof(struct mak__b));
        if (!arena_obj_713) {
            mak_ir_recover("ADT object allocation failed");
            arena_obj_713 = &mak_ir_default_obj__b;
         } else {
            (arena_obj_713)->tag = mak_ir_tag_11;
            (arena_obj_713)->v.v11.f0 = -3.14159265000000021;
        }
        (arena_obj_712)->v.v13.f0 = arena_obj_713;
        (arena_obj_712)->v.v13.f1 = &mak_ir_ctor_obj_12;
    }
    mak_ir_fn_19("  -pi(3)    = %.3f", arena_obj_712);
    struct mak__c *arena_obj_714 = (struct mak__c *)mak_ir_arena_alloc(sizeof(struct mak__c), _Alignof(struct mak__c));
    if (!arena_obj_714) {
        mak_ir_recover("ADT object allocation failed");
        arena_obj_714 = &mak_ir_default_obj__c;
     } else {
        (arena_obj_714)->tag = mak_ir_tag_13;
        struct mak__b *arena_obj_715 = (struct mak__b *)mak_ir_arena_alloc(sizeof(struct mak__b), _Alignof(struct mak__b));
        if (!arena_obj_715) {
            mak_ir_recover("ADT object allocation failed");
            arena_obj_715 = &mak_ir_default_obj__b;
         } else {
            (arena_obj_715)->tag = mak_ir_tag_11;
            (arena_obj_715)->v.v11.f0 = 2.50000000000000000;
        }
        (arena_obj_714)->v.v13.f0 = arena_obj_715;
        (arena_obj_714)->v.v13.f1 = &mak_ir_ctor_obj_12;
    }
    mak_ir_fn_19("  width10   = [%10.4f]", arena_obj_714);
    mak_ir_fn_19("", &mak_ir_ctor_obj_12);
    mak_ir_fn_19("Strings with precision / width:", &mak_ir_ctor_obj_12);
    struct mak__c *arena_obj_716 = (struct mak__c *)mak_ir_arena_alloc(sizeof(struct mak__c), _Alignof(struct mak__c));
    if (!arena_obj_716) {
        mak_ir_recover("ADT object allocation failed");
        arena_obj_716 = &mak_ir_default_obj__c;
     } else {
        (arena_obj_716)->tag = mak_ir_tag_13;
        struct mak__b *arena_obj_717 = (struct mak__b *)mak_ir_arena_alloc(sizeof(struct mak__b), _Alignof(struct mak__b));
        if (!arena_obj_717) {
            mak_ir_recover("ADT object allocation failed");
            arena_obj_717 = &mak_ir_default_obj__b;
         } else {
            (arena_obj_717)->tag = mak_ir_tag_9;
            (arena_obj_717)->v.v9.f0 = "abcdefg";
        }
        (arena_obj_716)->v.v13.f0 = arena_obj_717;
        (arena_obj_716)->v.v13.f1 = &mak_ir_ctor_obj_12;
    }
    mak_ir_fn_19("  [%.3s]", arena_obj_716);
    struct mak__c *arena_obj_718 = (struct mak__c *)mak_ir_arena_alloc(sizeof(struct mak__c), _Alignof(struct mak__c));
    if (!arena_obj_718) {
        mak_ir_recover("ADT object allocation failed");
        arena_obj_718 = &mak_ir_default_obj__c;
     } else {
        (arena_obj_718)->tag = mak_ir_tag_13;
        struct mak__b *arena_obj_719 = (struct mak__b *)mak_ir_arena_alloc(sizeof(struct mak__b), _Alignof(struct mak__b));
        if (!arena_obj_719) {
            mak_ir_recover("ADT object allocation failed");
            arena_obj_719 = &mak_ir_default_obj__b;
         } else {
            (arena_obj_719)->tag = mak_ir_tag_9;
            (arena_obj_719)->v.v9.f0 = "hi";
        }
        (arena_obj_718)->v.v13.f0 = arena_obj_719;
        (arena_obj_718)->v.v13.f1 = &mak_ir_ctor_obj_12;
    }
    mak_ir_fn_19("  [%10s]", arena_obj_718);
    struct mak__c *arena_obj_720 = (struct mak__c *)mak_ir_arena_alloc(sizeof(struct mak__c), _Alignof(struct mak__c));
    if (!arena_obj_720) {
        mak_ir_recover("ADT object allocation failed");
        arena_obj_720 = &mak_ir_default_obj__c;
     } else {
        (arena_obj_720)->tag = mak_ir_tag_13;
        struct mak__b *arena_obj_721 = (struct mak__b *)mak_ir_arena_alloc(sizeof(struct mak__b), _Alignof(struct mak__b));
        if (!arena_obj_721) {
            mak_ir_recover("ADT object allocation failed");
            arena_obj_721 = &mak_ir_default_obj__b;
         } else {
            (arena_obj_721)->tag = mak_ir_tag_9;
            (arena_obj_721)->v.v9.f0 = "hi";
        }
        (arena_obj_720)->v.v13.f0 = arena_obj_721;
        (arena_obj_720)->v.v13.f1 = &mak_ir_ctor_obj_12;
    }
    mak_ir_fn_19("  [%-10s]", arena_obj_720);
    mak_ir_fn_19("", &mak_ir_ctor_obj_12);
    struct mak__c *arena_obj_722 = (struct mak__c *)mak_ir_arena_alloc(sizeof(struct mak__c), _Alignof(struct mak__c));
    if (!arena_obj_722) {
        mak_ir_recover("ADT object allocation failed");
        arena_obj_722 = &mak_ir_default_obj__c;
     } else {
        (arena_obj_722)->tag = mak_ir_tag_13;
        struct mak__b *arena_obj_723 = (struct mak__b *)mak_ir_arena_alloc(sizeof(struct mak__b), _Alignof(struct mak__b));
        if (!arena_obj_723) {
            mak_ir_recover("ADT object allocation failed");
            arena_obj_723 = &mak_ir_default_obj__b;
         } else {
            (arena_obj_723)->tag = mak_ir_tag_9;
            (arena_obj_723)->v.v9.f0 = "Order #1234";
        }
        (arena_obj_722)->v.v13.f0 = arena_obj_723;
        struct mak__c *arena_obj_724 = (struct mak__c *)mak_ir_arena_alloc(sizeof(struct mak__c), _Alignof(struct mak__c));
        if (!arena_obj_724) {
            mak_ir_recover("ADT object allocation failed");
            arena_obj_724 = &mak_ir_default_obj__c;
         } else {
            (arena_obj_724)->tag = mak_ir_tag_13;
            struct mak__b *arena_obj_725 = (struct mak__b *)mak_ir_arena_alloc(sizeof(struct mak__b), _Alignof(struct mak__b));
            if (!arena_obj_725) {
                mak_ir_recover("ADT object allocation failed");
                arena_obj_725 = &mak_ir_default_obj__b;
             } else {
                (arena_obj_725)->tag = mak_ir_tag_2;
                (arena_obj_725)->v.v2.f0 = INT64_C(5);
            }
            (arena_obj_724)->v.v13.f0 = arena_obj_725;
            struct mak__c *arena_obj_726 = (struct mak__c *)mak_ir_arena_alloc(sizeof(struct mak__c), _Alignof(struct mak__c));
            if (!arena_obj_726) {
                mak_ir_recover("ADT object allocation failed");
                arena_obj_726 = &mak_ir_default_obj__c;
             } else {
                (arena_obj_726)->tag = mak_ir_tag_13;
                struct mak__b *arena_obj_727 = (struct mak__b *)mak_ir_arena_alloc(sizeof(struct mak__b), _Alignof(struct mak__b));
                if (!arena_obj_727) {
                    mak_ir_recover("ADT object allocation failed");
                    arena_obj_727 = &mak_ir_default_obj__b;
                 } else {
                    (arena_obj_727)->tag = mak_ir_tag_10;
                    (arena_obj_727)->v.v10.f0 = (const void *)((uintptr_t)(INT64_C(305419896)));
                }
                (arena_obj_726)->v.v13.f0 = arena_obj_727;
                struct mak__c *arena_obj_728 = (struct mak__c *)mak_ir_arena_alloc(sizeof(struct mak__c), _Alignof(struct mak__c));
                if (!arena_obj_728) {
                    mak_ir_recover("ADT object allocation failed");
                    arena_obj_728 = &mak_ir_default_obj__c;
                 } else {
                    (arena_obj_728)->tag = mak_ir_tag_13;
                    struct mak__b *arena_obj_729 = (struct mak__b *)mak_ir_arena_alloc(sizeof(struct mak__b), _Alignof(struct mak__b));
                    if (!arena_obj_729) {
                        mak_ir_recover("ADT object allocation failed");
                        arena_obj_729 = &mak_ir_default_obj__b;
                     } else {
                        (arena_obj_729)->tag = mak_ir_tag_11;
                        (arena_obj_729)->v.v11.f0 = 99.95000000000000284;
                    }
                    (arena_obj_728)->v.v13.f0 = arena_obj_729;
                    (arena_obj_728)->v.v13.f1 = &mak_ir_ctor_obj_12;
                }
                (arena_obj_726)->v.v13.f1 = arena_obj_728;
            }
            (arena_obj_724)->v.v13.f1 = arena_obj_726;
        }
        (arena_obj_722)->v.v13.f1 = arena_obj_724;
    }
    mak_ir_fn_19("Recap: %s has %d items at %p, total %.2f", arena_obj_722);
    mak_ir_fn_19("", &mak_ir_ctor_obj_12);
    return mak_ir_fn_19("Done.", &mak_ir_ctor_obj_12);
}


int main(void) {
    mak_user_a = mak_ir_fn_0;
    mak_user_c = mak_ir_fn_1;
    mak_user_e = mak_ir_fn_2;
    mak_user_f = mak_ir_fn_3;
    mak_user_g = mak_ir_fn_4;
    mak_user_h = mak_ir_fn_5;
    mak_user_i = mak_ir_fn_6;
    mak_user_k = mak_ir_fn_7;
    mak_user_l = mak_ir_fn_8;
    mak_user_n = mak_ir_fn_9;
    mak_user_o = mak_ir_fn_10;
    mak_user_p = mak_ir_fn_11;
    mak_user_t = mak_ir_fn_12;
    mak_user_v = mak_ir_fn_13;
    mak_user_y = mak_ir_fn_14;
    mak_user_z = mak_ir_fn_15;
    mak_user_aa = mak_ir_fn_16;
    mak_user_ac = mak_ir_fn_17;
    mak_user_ag = mak_ir_fn_18;
    mak_user_ah = mak_ir_fn_19;
    mak_ir_fn_19("==================================================", &mak_ir_ctor_obj_12);
    mak_ir_fn_19("  pureprintf.mak \342\200\224 a printf in pure MAK", &mak_ir_ctor_obj_12);
    mak_ir_fn_19("==================================================", &mak_ir_ctor_obj_12);
    mak_ir_fn_19("", &mak_ir_ctor_obj_12);
    mak_ir_fn_19("Basic:", &mak_ir_ctor_obj_12);
    struct mak__c *arena_obj_730 = (struct mak__c *)mak_ir_arena_alloc(sizeof(struct mak__c), _Alignof(struct mak__c));
    if (!arena_obj_730) {
        mak_ir_recover("ADT object allocation failed");
        arena_obj_730 = &mak_ir_default_obj__c;
     } else {
        (arena_obj_730)->tag = mak_ir_tag_13;
        struct mak__b *arena_obj_731 = (struct mak__b *)mak_ir_arena_alloc(sizeof(struct mak__b), _Alignof(struct mak__b));
        if (!arena_obj_731) {
            mak_ir_recover("ADT object allocation failed");
            arena_obj_731 = &mak_ir_default_obj__b;
         } else {
            (arena_obj_731)->tag = mak_ir_tag_9;
            (arena_obj_731)->v.v9.f0 = "Alice";
        }
        (arena_obj_730)->v.v13.f0 = arena_obj_731;
        struct mak__c *arena_obj_732 = (struct mak__c *)mak_ir_arena_alloc(sizeof(struct mak__c), _Alignof(struct mak__c));
        if (!arena_obj_732) {
            mak_ir_recover("ADT object allocation failed");
            arena_obj_732 = &mak_ir_default_obj__c;
         } else {
            (arena_obj_732)->tag = mak_ir_tag_13;
            struct mak__b *arena_obj_733 = (struct mak__b *)mak_ir_arena_alloc(sizeof(struct mak__b), _Alignof(struct mak__b));
            if (!arena_obj_733) {
                mak_ir_recover("ADT object allocation failed");
                arena_obj_733 = &mak_ir_default_obj__b;
             } else {
                (arena_obj_733)->tag = mak_ir_tag_2;
                (arena_obj_733)->v.v2.f0 = INT64_C(30);
            }
            (arena_obj_732)->v.v13.f0 = arena_obj_733;
            struct mak__c *arena_obj_734 = (struct mak__c *)mak_ir_arena_alloc(sizeof(struct mak__c), _Alignof(struct mak__c));
            if (!arena_obj_734) {
                mak_ir_recover("ADT object allocation failed");
                arena_obj_734 = &mak_ir_default_obj__c;
             } else {
                (arena_obj_734)->tag = mak_ir_tag_13;
                struct mak__b *arena_obj_735 = (struct mak__b *)mak_ir_arena_alloc(sizeof(struct mak__b), _Alignof(struct mak__b));
                if (!arena_obj_735) {
                    mak_ir_recover("ADT object allocation failed");
                    arena_obj_735 = &mak_ir_default_obj__b;
                 } else {
                    (arena_obj_735)->tag = mak_ir_tag_8;
                    (arena_obj_735)->v.v8.f0 = (int32_t)(INT64_C(65));
                }
                (arena_obj_734)->v.v13.f0 = arena_obj_735;
                (arena_obj_734)->v.v13.f1 = &mak_ir_ctor_obj_12;
            }
            (arena_obj_732)->v.v13.f1 = arena_obj_734;
        }
        (arena_obj_730)->v.v13.f1 = arena_obj_732;
    }
    mak_ir_fn_19("  name=%s age=%d grade=%c", arena_obj_730);
    mak_ir_fn_19("", &mak_ir_ctor_obj_12);
    mak_ir_fn_19("Widths and flags:", &mak_ir_ctor_obj_12);
    struct mak__c *arena_obj_736 = (struct mak__c *)mak_ir_arena_alloc(sizeof(struct mak__c), _Alignof(struct mak__c));
    if (!arena_obj_736) {
        mak_ir_recover("ADT object allocation failed");
        arena_obj_736 = &mak_ir_default_obj__c;
     } else {
        (arena_obj_736)->tag = mak_ir_tag_13;
        struct mak__b *arena_obj_737 = (struct mak__b *)mak_ir_arena_alloc(sizeof(struct mak__b), _Alignof(struct mak__b));
        if (!arena_obj_737) {
            mak_ir_recover("ADT object allocation failed");
            arena_obj_737 = &mak_ir_default_obj__b;
         } else {
            (arena_obj_737)->tag = mak_ir_tag_2;
            (arena_obj_737)->v.v2.f0 = INT64_C(42);
        }
        (arena_obj_736)->v.v13.f0 = arena_obj_737;
        (arena_obj_736)->v.v13.f1 = &mak_ir_ctor_obj_12;
    }
    mak_ir_fn_19("  right: [%10d]", arena_obj_736);
    struct mak__c *arena_obj_738 = (struct mak__c *)mak_ir_arena_alloc(sizeof(struct mak__c), _Alignof(struct mak__c));
    if (!arena_obj_738) {
        mak_ir_recover("ADT object allocation failed");
        arena_obj_738 = &mak_ir_default_obj__c;
     } else {
        (arena_obj_738)->tag = mak_ir_tag_13;
        struct mak__b *arena_obj_739 = (struct mak__b *)mak_ir_arena_alloc(sizeof(struct mak__b), _Alignof(struct mak__b));
        if (!arena_obj_739) {
            mak_ir_recover("ADT object allocation failed");
            arena_obj_739 = &mak_ir_default_obj__b;
         } else {
            (arena_obj_739)->tag = mak_ir_tag_2;
            (arena_obj_739)->v.v2.f0 = INT64_C(42);
        }
        (arena_obj_738)->v.v13.f0 = arena_obj_739;
        (arena_obj_738)->v.v13.f1 = &mak_ir_ctor_obj_12;
    }
    mak_ir_fn_19("  left:  [%-10d]", arena_obj_738);
    struct mak__c *arena_obj_740 = (struct mak__c *)mak_ir_arena_alloc(sizeof(struct mak__c), _Alignof(struct mak__c));
    if (!arena_obj_740) {
        mak_ir_recover("ADT object allocation failed");
        arena_obj_740 = &mak_ir_default_obj__c;
     } else {
        (arena_obj_740)->tag = mak_ir_tag_13;
        struct mak__b *arena_obj_741 = (struct mak__b *)mak_ir_arena_alloc(sizeof(struct mak__b), _Alignof(struct mak__b));
        if (!arena_obj_741) {
            mak_ir_recover("ADT object allocation failed");
            arena_obj_741 = &mak_ir_default_obj__b;
         } else {
            (arena_obj_741)->tag = mak_ir_tag_2;
            (arena_obj_741)->v.v2.f0 = INT64_C(42);
        }
        (arena_obj_740)->v.v13.f0 = arena_obj_741;
        (arena_obj_740)->v.v13.f1 = &mak_ir_ctor_obj_12;
    }
    mak_ir_fn_19("  zero:  [%010d]", arena_obj_740);
    struct mak__c *arena_obj_742 = (struct mak__c *)mak_ir_arena_alloc(sizeof(struct mak__c), _Alignof(struct mak__c));
    if (!arena_obj_742) {
        mak_ir_recover("ADT object allocation failed");
        arena_obj_742 = &mak_ir_default_obj__c;
     } else {
        (arena_obj_742)->tag = mak_ir_tag_13;
        struct mak__b *arena_obj_743 = (struct mak__b *)mak_ir_arena_alloc(sizeof(struct mak__b), _Alignof(struct mak__b));
        if (!arena_obj_743) {
            mak_ir_recover("ADT object allocation failed");
            arena_obj_743 = &mak_ir_default_obj__b;
         } else {
            (arena_obj_743)->tag = mak_ir_tag_2;
            (arena_obj_743)->v.v2.f0 = INT64_C(42);
        }
        (arena_obj_742)->v.v13.f0 = arena_obj_743;
        (arena_obj_742)->v.v13.f1 = &mak_ir_ctor_obj_12;
    }
    mak_ir_fn_19("  plus:  [%+d]", arena_obj_742);
    struct mak__c *arena_obj_744 = (struct mak__c *)mak_ir_arena_alloc(sizeof(struct mak__c), _Alignof(struct mak__c));
    if (!arena_obj_744) {
        mak_ir_recover("ADT object allocation failed");
        arena_obj_744 = &mak_ir_default_obj__c;
     } else {
        (arena_obj_744)->tag = mak_ir_tag_13;
        struct mak__b *arena_obj_745 = (struct mak__b *)mak_ir_arena_alloc(sizeof(struct mak__b), _Alignof(struct mak__b));
        if (!arena_obj_745) {
            mak_ir_recover("ADT object allocation failed");
            arena_obj_745 = &mak_ir_default_obj__b;
         } else {
            (arena_obj_745)->tag = mak_ir_tag_2;
            (arena_obj_745)->v.v2.f0 = INT64_C(42);
        }
        (arena_obj_744)->v.v13.f0 = arena_obj_745;
        (arena_obj_744)->v.v13.f1 = &mak_ir_ctor_obj_12;
    }
    mak_ir_fn_19("  space: [% d]", arena_obj_744);
    struct mak__c *arena_obj_746 = (struct mak__c *)mak_ir_arena_alloc(sizeof(struct mak__c), _Alignof(struct mak__c));
    if (!arena_obj_746) {
        mak_ir_recover("ADT object allocation failed");
        arena_obj_746 = &mak_ir_default_obj__c;
     } else {
        (arena_obj_746)->tag = mak_ir_tag_13;
        struct mak__b *arena_obj_747 = (struct mak__b *)mak_ir_arena_alloc(sizeof(struct mak__b), _Alignof(struct mak__b));
        if (!arena_obj_747) {
            mak_ir_recover("ADT object allocation failed");
            arena_obj_747 = &mak_ir_default_obj__b;
         } else {
            (arena_obj_747)->tag = mak_ir_tag_2;
            (arena_obj_747)->v.v2.f0 = (int64_t)(((int64_t)UINT64_C(18446744073709551609)));
        }
        (arena_obj_746)->v.v13.f0 = arena_obj_747;
        (arena_obj_746)->v.v13.f1 = &mak_ir_ctor_obj_12;
    }
    mak_ir_fn_19("  neg:   [%d]", arena_obj_746);
    mak_ir_fn_19("", &mak_ir_ctor_obj_12);
    mak_ir_fn_19("Bases:", &mak_ir_ctor_obj_12);
    struct mak__c *arena_obj_748 = (struct mak__c *)mak_ir_arena_alloc(sizeof(struct mak__c), _Alignof(struct mak__c));
    if (!arena_obj_748) {
        mak_ir_recover("ADT object allocation failed");
        arena_obj_748 = &mak_ir_default_obj__c;
     } else {
        (arena_obj_748)->tag = mak_ir_tag_13;
        struct mak__b *arena_obj_749 = (struct mak__b *)mak_ir_arena_alloc(sizeof(struct mak__b), _Alignof(struct mak__b));
        if (!arena_obj_749) {
            mak_ir_recover("ADT object allocation failed");
            arena_obj_749 = &mak_ir_default_obj__b;
         } else {
            (arena_obj_749)->tag = mak_ir_tag_4;
            (arena_obj_749)->v.v4.f0 = (uint64_t)(INT64_C(3735928559));
        }
        (arena_obj_748)->v.v13.f0 = arena_obj_749;
        (arena_obj_748)->v.v13.f1 = &mak_ir_ctor_obj_12;
    }
    mak_ir_fn_19("  hex:  0x%x", arena_obj_748);
    struct mak__c *arena_obj_750 = (struct mak__c *)mak_ir_arena_alloc(sizeof(struct mak__c), _Alignof(struct mak__c));
    if (!arena_obj_750) {
        mak_ir_recover("ADT object allocation failed");
        arena_obj_750 = &mak_ir_default_obj__c;
     } else {
        (arena_obj_750)->tag = mak_ir_tag_13;
        struct mak__b *arena_obj_751 = (struct mak__b *)mak_ir_arena_alloc(sizeof(struct mak__b), _Alignof(struct mak__b));
        if (!arena_obj_751) {
            mak_ir_recover("ADT object allocation failed");
            arena_obj_751 = &mak_ir_default_obj__b;
         } else {
            (arena_obj_751)->tag = mak_ir_tag_5;
            (arena_obj_751)->v.v5.f0 = (uint64_t)(INT64_C(3735928559));
        }
        (arena_obj_750)->v.v13.f0 = arena_obj_751;
        (arena_obj_750)->v.v13.f1 = &mak_ir_ctor_obj_12;
    }
    mak_ir_fn_19("  HEX:  0x%X", arena_obj_750);
    struct mak__c *arena_obj_752 = (struct mak__c *)mak_ir_arena_alloc(sizeof(struct mak__c), _Alignof(struct mak__c));
    if (!arena_obj_752) {
        mak_ir_recover("ADT object allocation failed");
        arena_obj_752 = &mak_ir_default_obj__c;
     } else {
        (arena_obj_752)->tag = mak_ir_tag_13;
        struct mak__b *arena_obj_753 = (struct mak__b *)mak_ir_arena_alloc(sizeof(struct mak__b), _Alignof(struct mak__b));
        if (!arena_obj_753) {
            mak_ir_recover("ADT object allocation failed");
            arena_obj_753 = &mak_ir_default_obj__b;
         } else {
            (arena_obj_753)->tag = mak_ir_tag_6;
            (arena_obj_753)->v.v6.f0 = (uint64_t)(INT64_C(493));
        }
        (arena_obj_752)->v.v13.f0 = arena_obj_753;
        (arena_obj_752)->v.v13.f1 = &mak_ir_ctor_obj_12;
    }
    mak_ir_fn_19("  oct:  %o", arena_obj_752);
    struct mak__c *arena_obj_754 = (struct mak__c *)mak_ir_arena_alloc(sizeof(struct mak__c), _Alignof(struct mak__c));
    if (!arena_obj_754) {
        mak_ir_recover("ADT object allocation failed");
        arena_obj_754 = &mak_ir_default_obj__c;
     } else {
        (arena_obj_754)->tag = mak_ir_tag_13;
        struct mak__b *arena_obj_755 = (struct mak__b *)mak_ir_arena_alloc(sizeof(struct mak__b), _Alignof(struct mak__b));
        if (!arena_obj_755) {
            mak_ir_recover("ADT object allocation failed");
            arena_obj_755 = &mak_ir_default_obj__b;
         } else {
            (arena_obj_755)->tag = mak_ir_tag_7;
            (arena_obj_755)->v.v7.f0 = (uint64_t)(INT64_C(170));
        }
        (arena_obj_754)->v.v13.f0 = arena_obj_755;
        (arena_obj_754)->v.v13.f1 = &mak_ir_ctor_obj_12;
    }
    mak_ir_fn_19("  bin:  %b", arena_obj_754);
    struct mak__c *arena_obj_756 = (struct mak__c *)mak_ir_arena_alloc(sizeof(struct mak__c), _Alignof(struct mak__c));
    if (!arena_obj_756) {
        mak_ir_recover("ADT object allocation failed");
        arena_obj_756 = &mak_ir_default_obj__c;
     } else {
        (arena_obj_756)->tag = mak_ir_tag_13;
        struct mak__b *arena_obj_757 = (struct mak__b *)mak_ir_arena_alloc(sizeof(struct mak__b), _Alignof(struct mak__b));
        if (!arena_obj_757) {
            mak_ir_recover("ADT object allocation failed");
            arena_obj_757 = &mak_ir_default_obj__b;
         } else {
            (arena_obj_757)->tag = mak_ir_tag_10;
            (arena_obj_757)->v.v10.f0 = (const void *)((uintptr_t)(INT64_C(48879)));
        }
        (arena_obj_756)->v.v13.f0 = arena_obj_757;
        (arena_obj_756)->v.v13.f1 = &mak_ir_ctor_obj_12;
    }
    mak_ir_fn_19("  ptr:  %p", arena_obj_756);
    mak_ir_fn_19("", &mak_ir_ctor_obj_12);
    mak_ir_fn_19("Floats:", &mak_ir_ctor_obj_12);
    struct mak__c *arena_obj_758 = (struct mak__c *)mak_ir_arena_alloc(sizeof(struct mak__c), _Alignof(struct mak__c));
    if (!arena_obj_758) {
        mak_ir_recover("ADT object allocation failed");
        arena_obj_758 = &mak_ir_default_obj__c;
     } else {
        (arena_obj_758)->tag = mak_ir_tag_13;
        struct mak__b *arena_obj_759 = (struct mak__b *)mak_ir_arena_alloc(sizeof(struct mak__b), _Alignof(struct mak__b));
        if (!arena_obj_759) {
            mak_ir_recover("ADT object allocation failed");
            arena_obj_759 = &mak_ir_default_obj__b;
         } else {
            (arena_obj_759)->tag = mak_ir_tag_11;
            (arena_obj_759)->v.v11.f0 = 3.14159265358979001;
        }
        (arena_obj_758)->v.v13.f0 = arena_obj_759;
        (arena_obj_758)->v.v13.f1 = &mak_ir_ctor_obj_12;
    }
    mak_ir_fn_19("  pi        = %f", arena_obj_758);
    struct mak__c *arena_obj_760 = (struct mak__c *)mak_ir_arena_alloc(sizeof(struct mak__c), _Alignof(struct mak__c));
    if (!arena_obj_760) {
        mak_ir_recover("ADT object allocation failed");
        arena_obj_760 = &mak_ir_default_obj__c;
     } else {
        (arena_obj_760)->tag = mak_ir_tag_13;
        struct mak__b *arena_obj_761 = (struct mak__b *)mak_ir_arena_alloc(sizeof(struct mak__b), _Alignof(struct mak__b));
        if (!arena_obj_761) {
            mak_ir_recover("ADT object allocation failed");
            arena_obj_761 = &mak_ir_default_obj__b;
         } else {
            (arena_obj_761)->tag = mak_ir_tag_11;
            (arena_obj_761)->v.v11.f0 = 3.14159265358979001;
        }
        (arena_obj_760)->v.v13.f0 = arena_obj_761;
        (arena_obj_760)->v.v13.f1 = &mak_ir_ctor_obj_12;
    }
    mak_ir_fn_19("  pi(2)     = %.2f", arena_obj_760);
    struct mak__c *arena_obj_762 = (struct mak__c *)mak_ir_arena_alloc(sizeof(struct mak__c), _Alignof(struct mak__c));
    if (!arena_obj_762) {
        mak_ir_recover("ADT object allocation failed");
        arena_obj_762 = &mak_ir_default_obj__c;
     } else {
        (arena_obj_762)->tag = mak_ir_tag_13;
        struct mak__b *arena_obj_763 = (struct mak__b *)mak_ir_arena_alloc(sizeof(struct mak__b), _Alignof(struct mak__b));
        if (!arena_obj_763) {
            mak_ir_recover("ADT object allocation failed");
            arena_obj_763 = &mak_ir_default_obj__b;
         } else {
            (arena_obj_763)->tag = mak_ir_tag_11;
            (arena_obj_763)->v.v11.f0 = 3.14159265358979001;
        }
        (arena_obj_762)->v.v13.f0 = arena_obj_763;
        (arena_obj_762)->v.v13.f1 = &mak_ir_ctor_obj_12;
    }
    mak_ir_fn_19("  pi(0)     = %.0f", arena_obj_762);
    struct mak__c *arena_obj_764 = (struct mak__c *)mak_ir_arena_alloc(sizeof(struct mak__c), _Alignof(struct mak__c));
    if (!arena_obj_764) {
        mak_ir_recover("ADT object allocation failed");
        arena_obj_764 = &mak_ir_default_obj__c;
     } else {
        (arena_obj_764)->tag = mak_ir_tag_13;
        struct mak__b *arena_obj_765 = (struct mak__b *)mak_ir_arena_alloc(sizeof(struct mak__b), _Alignof(struct mak__b));
        if (!arena_obj_765) {
            mak_ir_recover("ADT object allocation failed");
            arena_obj_765 = &mak_ir_default_obj__b;
         } else {
            (arena_obj_765)->tag = mak_ir_tag_11;
            (arena_obj_765)->v.v11.f0 = ((double)(0.00000000000000000) - (double)(3.14159265000000021));
        }
        (arena_obj_764)->v.v13.f0 = arena_obj_765;
        (arena_obj_764)->v.v13.f1 = &mak_ir_ctor_obj_12;
    }
    mak_ir_fn_19("  -pi(3)    = %.3f", arena_obj_764);
    struct mak__c *arena_obj_766 = (struct mak__c *)mak_ir_arena_alloc(sizeof(struct mak__c), _Alignof(struct mak__c));
    if (!arena_obj_766) {
        mak_ir_recover("ADT object allocation failed");
        arena_obj_766 = &mak_ir_default_obj__c;
     } else {
        (arena_obj_766)->tag = mak_ir_tag_13;
        struct mak__b *arena_obj_767 = (struct mak__b *)mak_ir_arena_alloc(sizeof(struct mak__b), _Alignof(struct mak__b));
        if (!arena_obj_767) {
            mak_ir_recover("ADT object allocation failed");
            arena_obj_767 = &mak_ir_default_obj__b;
         } else {
            (arena_obj_767)->tag = mak_ir_tag_11;
            (arena_obj_767)->v.v11.f0 = 2.50000000000000000;
        }
        (arena_obj_766)->v.v13.f0 = arena_obj_767;
        (arena_obj_766)->v.v13.f1 = &mak_ir_ctor_obj_12;
    }
    mak_ir_fn_19("  width10   = [%10.4f]", arena_obj_766);
    mak_ir_fn_19("", &mak_ir_ctor_obj_12);
    mak_ir_fn_19("Strings with precision / width:", &mak_ir_ctor_obj_12);
    struct mak__c *arena_obj_768 = (struct mak__c *)mak_ir_arena_alloc(sizeof(struct mak__c), _Alignof(struct mak__c));
    if (!arena_obj_768) {
        mak_ir_recover("ADT object allocation failed");
        arena_obj_768 = &mak_ir_default_obj__c;
     } else {
        (arena_obj_768)->tag = mak_ir_tag_13;
        struct mak__b *arena_obj_769 = (struct mak__b *)mak_ir_arena_alloc(sizeof(struct mak__b), _Alignof(struct mak__b));
        if (!arena_obj_769) {
            mak_ir_recover("ADT object allocation failed");
            arena_obj_769 = &mak_ir_default_obj__b;
         } else {
            (arena_obj_769)->tag = mak_ir_tag_9;
            (arena_obj_769)->v.v9.f0 = "abcdefg";
        }
        (arena_obj_768)->v.v13.f0 = arena_obj_769;
        (arena_obj_768)->v.v13.f1 = &mak_ir_ctor_obj_12;
    }
    mak_ir_fn_19("  [%.3s]", arena_obj_768);
    struct mak__c *arena_obj_770 = (struct mak__c *)mak_ir_arena_alloc(sizeof(struct mak__c), _Alignof(struct mak__c));
    if (!arena_obj_770) {
        mak_ir_recover("ADT object allocation failed");
        arena_obj_770 = &mak_ir_default_obj__c;
     } else {
        (arena_obj_770)->tag = mak_ir_tag_13;
        struct mak__b *arena_obj_771 = (struct mak__b *)mak_ir_arena_alloc(sizeof(struct mak__b), _Alignof(struct mak__b));
        if (!arena_obj_771) {
            mak_ir_recover("ADT object allocation failed");
            arena_obj_771 = &mak_ir_default_obj__b;
         } else {
            (arena_obj_771)->tag = mak_ir_tag_9;
            (arena_obj_771)->v.v9.f0 = "hi";
        }
        (arena_obj_770)->v.v13.f0 = arena_obj_771;
        (arena_obj_770)->v.v13.f1 = &mak_ir_ctor_obj_12;
    }
    mak_ir_fn_19("  [%10s]", arena_obj_770);
    struct mak__c *arena_obj_772 = (struct mak__c *)mak_ir_arena_alloc(sizeof(struct mak__c), _Alignof(struct mak__c));
    if (!arena_obj_772) {
        mak_ir_recover("ADT object allocation failed");
        arena_obj_772 = &mak_ir_default_obj__c;
     } else {
        (arena_obj_772)->tag = mak_ir_tag_13;
        struct mak__b *arena_obj_773 = (struct mak__b *)mak_ir_arena_alloc(sizeof(struct mak__b), _Alignof(struct mak__b));
        if (!arena_obj_773) {
            mak_ir_recover("ADT object allocation failed");
            arena_obj_773 = &mak_ir_default_obj__b;
         } else {
            (arena_obj_773)->tag = mak_ir_tag_9;
            (arena_obj_773)->v.v9.f0 = "hi";
        }
        (arena_obj_772)->v.v13.f0 = arena_obj_773;
        (arena_obj_772)->v.v13.f1 = &mak_ir_ctor_obj_12;
    }
    mak_ir_fn_19("  [%-10s]", arena_obj_772);
    mak_ir_fn_19("", &mak_ir_ctor_obj_12);
    struct mak__c *arena_obj_774 = (struct mak__c *)mak_ir_arena_alloc(sizeof(struct mak__c), _Alignof(struct mak__c));
    if (!arena_obj_774) {
        mak_ir_recover("ADT object allocation failed");
        arena_obj_774 = &mak_ir_default_obj__c;
     } else {
        (arena_obj_774)->tag = mak_ir_tag_13;
        struct mak__b *arena_obj_775 = (struct mak__b *)mak_ir_arena_alloc(sizeof(struct mak__b), _Alignof(struct mak__b));
        if (!arena_obj_775) {
            mak_ir_recover("ADT object allocation failed");
            arena_obj_775 = &mak_ir_default_obj__b;
         } else {
            (arena_obj_775)->tag = mak_ir_tag_9;
            (arena_obj_775)->v.v9.f0 = "Order #1234";
        }
        (arena_obj_774)->v.v13.f0 = arena_obj_775;
        struct mak__c *arena_obj_776 = (struct mak__c *)mak_ir_arena_alloc(sizeof(struct mak__c), _Alignof(struct mak__c));
        if (!arena_obj_776) {
            mak_ir_recover("ADT object allocation failed");
            arena_obj_776 = &mak_ir_default_obj__c;
         } else {
            (arena_obj_776)->tag = mak_ir_tag_13;
            struct mak__b *arena_obj_777 = (struct mak__b *)mak_ir_arena_alloc(sizeof(struct mak__b), _Alignof(struct mak__b));
            if (!arena_obj_777) {
                mak_ir_recover("ADT object allocation failed");
                arena_obj_777 = &mak_ir_default_obj__b;
             } else {
                (arena_obj_777)->tag = mak_ir_tag_2;
                (arena_obj_777)->v.v2.f0 = INT64_C(5);
            }
            (arena_obj_776)->v.v13.f0 = arena_obj_777;
            struct mak__c *arena_obj_778 = (struct mak__c *)mak_ir_arena_alloc(sizeof(struct mak__c), _Alignof(struct mak__c));
            if (!arena_obj_778) {
                mak_ir_recover("ADT object allocation failed");
                arena_obj_778 = &mak_ir_default_obj__c;
             } else {
                (arena_obj_778)->tag = mak_ir_tag_13;
                struct mak__b *arena_obj_779 = (struct mak__b *)mak_ir_arena_alloc(sizeof(struct mak__b), _Alignof(struct mak__b));
                if (!arena_obj_779) {
                    mak_ir_recover("ADT object allocation failed");
                    arena_obj_779 = &mak_ir_default_obj__b;
                 } else {
                    (arena_obj_779)->tag = mak_ir_tag_10;
                    (arena_obj_779)->v.v10.f0 = (const void *)((uintptr_t)(INT64_C(305419896)));
                }
                (arena_obj_778)->v.v13.f0 = arena_obj_779;
                struct mak__c *arena_obj_780 = (struct mak__c *)mak_ir_arena_alloc(sizeof(struct mak__c), _Alignof(struct mak__c));
                if (!arena_obj_780) {
                    mak_ir_recover("ADT object allocation failed");
                    arena_obj_780 = &mak_ir_default_obj__c;
                 } else {
                    (arena_obj_780)->tag = mak_ir_tag_13;
                    struct mak__b *arena_obj_781 = (struct mak__b *)mak_ir_arena_alloc(sizeof(struct mak__b), _Alignof(struct mak__b));
                    if (!arena_obj_781) {
                        mak_ir_recover("ADT object allocation failed");
                        arena_obj_781 = &mak_ir_default_obj__b;
                     } else {
                        (arena_obj_781)->tag = mak_ir_tag_11;
                        (arena_obj_781)->v.v11.f0 = 99.95000000000000284;
                    }
                    (arena_obj_780)->v.v13.f0 = arena_obj_781;
                    (arena_obj_780)->v.v13.f1 = &mak_ir_ctor_obj_12;
                }
                (arena_obj_778)->v.v13.f1 = arena_obj_780;
            }
            (arena_obj_776)->v.v13.f1 = arena_obj_778;
        }
        (arena_obj_774)->v.v13.f1 = arena_obj_776;
    }
    mak_ir_fn_19("Recap: %s has %d items at %p, total %.2f", arena_obj_774);
    mak_ir_fn_19("", &mak_ir_ctor_obj_12);
    mak_ir_fn_19("Done.", &mak_ir_ctor_obj_12);
    mak_ir_arena_shutdown();
    return 0;
}
