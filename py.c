#include <math.h>
#include <stdatomic.h>
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
        struct {
            struct mak__a * f0;
        } v3;
        struct {
            int64_t f0;
            struct mak__a * f1;
        } v5;
        struct {
            double f0;
            struct mak__a * f1;
        } v7;
        struct {
            int64_t f0;
            struct mak__a * f1;
        } v9;
        struct {
            double f0;
            struct mak__a * f1;
        } v10;
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
            struct mak__a * f0;
        } v3;
        struct {
            int64_t f0;
            struct mak__a * f1;
        } v5;
        struct {
            double f0;
            struct mak__a * f1;
        } v7;
        struct {
            int64_t f0;
            struct mak__a * f1;
        } v9;
        struct {
            double f0;
            struct mak__a * f1;
        } v10;
    } v;
};

static struct mak__b mak_ir_ctor_obj_2 = { mak_ir_tag_2 };
static struct mak__b mak_ir_default_obj__b = { mak_ir_tag_2 };
struct mak__c {
    uint32_t tag;
    union {
        struct {
            int32_t f0;
            struct mak__a * f1;
        } v1;
        struct {
            struct mak__a * f0;
        } v3;
        struct {
            int64_t f0;
            struct mak__a * f1;
        } v5;
        struct {
            double f0;
            struct mak__a * f1;
        } v7;
        struct {
            int64_t f0;
            struct mak__a * f1;
        } v9;
        struct {
            double f0;
            struct mak__a * f1;
        } v10;
    } v;
};

static struct mak__c mak_ir_ctor_obj_4 = { mak_ir_tag_4 };
static struct mak__c mak_ir_default_obj__c = { mak_ir_tag_4 };
struct mak__d {
    uint32_t tag;
    union {
        struct {
            int32_t f0;
            struct mak__a * f1;
        } v1;
        struct {
            struct mak__a * f0;
        } v3;
        struct {
            int64_t f0;
            struct mak__a * f1;
        } v5;
        struct {
            double f0;
            struct mak__a * f1;
        } v7;
        struct {
            int64_t f0;
            struct mak__a * f1;
        } v9;
        struct {
            double f0;
            struct mak__a * f1;
        } v10;
    } v;
};

static struct mak__d mak_ir_ctor_obj_6 = { mak_ir_tag_6 };
static struct mak__d mak_ir_default_obj__d = { mak_ir_tag_6 };
struct mak__e {
    uint32_t tag;
    union {
        struct {
            int32_t f0;
            struct mak__a * f1;
        } v1;
        struct {
            struct mak__a * f0;
        } v3;
        struct {
            int64_t f0;
            struct mak__a * f1;
        } v5;
        struct {
            double f0;
            struct mak__a * f1;
        } v7;
        struct {
            int64_t f0;
            struct mak__a * f1;
        } v9;
        struct {
            double f0;
            struct mak__a * f1;
        } v10;
    } v;
};

static struct mak__e mak_ir_ctor_obj_8 = { mak_ir_tag_8 };
static struct mak__e mak_ir_default_obj__e = { mak_ir_tag_8 };
struct mak__f {
    uint32_t tag;
    union {
        struct {
            int32_t f0;
            struct mak__a * f1;
        } v1;
        struct {
            struct mak__a * f0;
        } v3;
        struct {
            int64_t f0;
            struct mak__a * f1;
        } v5;
        struct {
            double f0;
            struct mak__a * f1;
        } v7;
        struct {
            int64_t f0;
            struct mak__a * f1;
        } v9;
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
static inline double mak_ir_fn_17(void);
static inline void mak_ir_fn_18(int64_t a0, int64_t a1, struct mak__f *out);
static inline void mak_ir_fn_19(int64_t a0, struct mak__a * a1, struct mak__f *out);
static inline double mak_ir_fn_20(int64_t a0, int64_t a1, int64_t a2, int64_t a3);
static inline void mak_ir_fn_21(int64_t a0, struct mak__a * a1, struct mak__f *out);
static inline void mak_ir_fn_22(int64_t a0, struct mak__a * a1, struct mak__f *out);
static inline double mak_ir_fn_23(int64_t a0, int64_t a1);

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
typedef uint64_t (*mak_rt_fn)(const uint64_t *, const uint8_t *, size_t);
typedef uintptr_t (*mak_rt_ctor)(const uint64_t *, const uint8_t *, size_t);
static uint64_t mak_rt_wrap_fn_1(const uint64_t *v,const uint8_t *k,size_t n) {
    (void)k; (void)n;
    struct mak__a * mak_rt_a_0 = (struct mak__a *)(uintptr_t)(v[0]);
    struct mak__b *out=( struct mak__b *)mak_ir_heap_alloc(sizeof(struct mak__b),_Alignof(struct mak__b));
    if(!out) { mak_ir_recover("runtime function result allocation failed"); return (uint64_t)0; }
    mak_ir_fn_1(mak_rt_a_0,out);
    return (uint64_t)(uintptr_t)out;
}
static uint64_t mak_rt_wrap_fn_2(const uint64_t *v,const uint8_t *k,size_t n) {
    (void)k; (void)n;
    struct mak__a * mak_rt_a_0 = (struct mak__a *)(uintptr_t)(v[0]);
    struct mak__a *out=( struct mak__a *)mak_ir_heap_alloc(sizeof(struct mak__a),_Alignof(struct mak__a));
    if(!out) { mak_ir_recover("runtime function result allocation failed"); return (uint64_t)0; }
    mak_ir_fn_2(mak_rt_a_0,out);
    return (uint64_t)(uintptr_t)out;
}
static uint64_t mak_rt_wrap_fn_4(const uint64_t *v,const uint8_t *k,size_t n) {
    (void)k; (void)n;
    int64_t mak_rt_a_0 = (int64_t)(v[0]);
    double rv=mak_ir_fn_4(mak_rt_a_0);
    return mak_rt_f64_bits(rv);
}
static uint64_t mak_rt_wrap_fn_11(const uint64_t *v,const uint8_t *k,size_t n) {
    (void)k; (void)n;
    struct mak__a * mak_rt_a_0 = (struct mak__a *)(uintptr_t)(v[0]);
    struct mak__f *out=( struct mak__f *)mak_ir_heap_alloc(sizeof(struct mak__f),_Alignof(struct mak__f));
    if(!out) { mak_ir_recover("runtime function result allocation failed"); return (uint64_t)0; }
    mak_ir_fn_11(mak_rt_a_0,out);
    return (uint64_t)(uintptr_t)out;
}
static uint64_t mak_rt_wrap_fn_12(const uint64_t *v,const uint8_t *k,size_t n) {
    (void)k; (void)n;
    struct mak__a * mak_rt_a_0 = (struct mak__a *)(uintptr_t)(v[0]);
    struct mak__f *out=( struct mak__f *)mak_ir_heap_alloc(sizeof(struct mak__f),_Alignof(struct mak__f));
    if(!out) { mak_ir_recover("runtime function result allocation failed"); return (uint64_t)0; }
    mak_ir_fn_12(mak_rt_a_0,out);
    return (uint64_t)(uintptr_t)out;
}
static uint64_t mak_rt_wrap_fn_14(const uint64_t *v,const uint8_t *k,size_t n) {
    (void)k; (void)n;
    struct mak__a * mak_rt_a_0 = (struct mak__a *)(uintptr_t)(v[0]);
    struct mak__f *out=( struct mak__f *)mak_ir_heap_alloc(sizeof(struct mak__f),_Alignof(struct mak__f));
    if(!out) { mak_ir_recover("runtime function result allocation failed"); return (uint64_t)0; }
    mak_ir_fn_14(mak_rt_a_0,out);
    return (uint64_t)(uintptr_t)out;
}
static uint64_t mak_rt_wrap_fn_16(const uint64_t *v,const uint8_t *k,size_t n) {
    (void)k; (void)n;
    struct mak__a * mak_rt_a_0 = (struct mak__a *)(uintptr_t)(v[0]);
    struct mak__f *out=( struct mak__f *)mak_ir_heap_alloc(sizeof(struct mak__f),_Alignof(struct mak__f));
    if(!out) { mak_ir_recover("runtime function result allocation failed"); return (uint64_t)0; }
    mak_ir_fn_16(mak_rt_a_0,out);
    return (uint64_t)(uintptr_t)out;
}
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
static const uint8_t mak_rt_fk_1[]={ 4 };
static const uint8_t mak_rt_fk_2[]={ 4 };
static const uint8_t mak_rt_fk_4[]={ 1 };
static const uint8_t mak_rt_fk_11[]={ 4 };
static const uint8_t mak_rt_fk_12[]={ 4 };
static const uint8_t mak_rt_fk_14[]={ 4 };
static const uint8_t mak_rt_fk_16[]={ 4 };
struct mak_rt_fn_desc { const char *name; const char *canon; size_t arity; uint8_t ret_kind; const uint8_t *kinds; mak_rt_fn call; };
static const struct mak_rt_fn_desc mak_rt_functions[]={ {"b","b",1, 4, mak_rt_fk_1, mak_rt_wrap_fn_1},{"e","e",1, 4, mak_rt_fk_2, mak_rt_wrap_fn_2},{"u","u",1, 2, mak_rt_fk_4, mak_rt_wrap_fn_4},{"an","an",1, 4, mak_rt_fk_11, mak_rt_wrap_fn_11},{"ao","ao",1, 4, mak_rt_fk_12, mak_rt_wrap_fn_12},{"aq","aq",1, 4, mak_rt_fk_14, mak_rt_wrap_fn_14},{"at","at",1, 4, mak_rt_fk_16, mak_rt_wrap_fn_16} };
static const size_t mak_rt_function_count=sizeof(mak_rt_functions)/sizeof(mak_rt_functions[0]);
static uint64_t mak_rt_call_function(const char *query, const uint64_t *args, const uint8_t *kinds, size_t n, uint8_t expected_ret) {
    static _Thread_local const struct mak_rt_fn_desc *cache[128]; static _Thread_local uint64_t keys[128]; static _Thread_local uint32_t sig[128];
    uint64_t h=mak_rt_hash(query)^((uint64_t)n*0x9e3779b97f4a7c15ULL)^((uint64_t)expected_ret<<56); for(size_t j=0;j<n;j++) h=(h*1099511628211ULL)^kinds[j]; size_t slot=(size_t)h&127u;
    const struct mak_rt_fn_desc *hit=cache[slot]; if(hit && keys[slot]==h && sig[slot]==(uint32_t)(n*257u+expected_ret) && hit->arity==n && (expected_ret==0 || hit->ret_kind==expected_ret)) { int ok=1; if(!(strcmp(query,hit->name)==0 || mak_rt_fold_eq(query,hit->name) || mak_rt_canon_eq(query,hit->name))) ok=0; for(size_t j=0;j<n && ok;j++) if(!mak_rt_kind_compatible(kinds[j],hit->kinds[j])) ok=0; if(ok) return hit->call(args,kinds,n); }
    const struct mak_rt_fn_desc *best=NULL; int best_score=-1, ties=0; for(size_t i=0;i<mak_rt_function_count;i++) { const struct mak_rt_fn_desc *d=&mak_rt_functions[i]; if(d->arity!=n) continue; int score=0; if(!strcmp(query,d->name)) score+=1000; else if(mak_rt_fold_eq(query,d->name)) score+=800; else if(mak_rt_canon_eq(query,d->name)) score+=650; for(size_t j=0;j<n;j++) if(mak_rt_kind_compatible(kinds[j],d->kinds[j])) score+=(kinds[j]==d->kinds[j]?20:3); if(expected_ret==0 || expected_ret==d->ret_kind) score+=25; else score-=100; if(score>best_score) {best=d;best_score=score;ties=0;} else if(score==best_score) ++ties; }
    if(!best) { mak_ir_recover("unresolved runtime function"); return 0; }
    if(ties>1) { mak_ir_recover("runtime function tie"); }
    cache[slot]=best; keys[slot]=h; sig[slot]=(uint32_t)(n*257u+expected_ret); return best->call(args,kinds,n);
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
    const struct mak_rt_ctor_desc *best=NULL; int best_score=-1,ties=0; for(size_t i=0;i<mak_rt_ctor_count;i++) { const struct mak_rt_ctor_desc *d=&mak_rt_ctors[i]; if(d->arity!=n) continue; int score=0; if(!strcmp(query,d->name))score+=1000; else if(mak_rt_fold_eq(query,d->name))score+=800; else if(mak_rt_canon_eq(query,d->name))score+=650; if(type_hint && !strcmp(type_hint,d->type)) score+=500; for(size_t j=0;j<n;j++) if(mak_rt_kind_compatible(kinds[j],d->kinds[j])) score+=(kinds[j]==d->kinds[j]?20:3); if(score>best_score) {best=d;best_score=score;ties=0;} else if(score==best_score) ++ties; }
    if(!best) { mak_ir_recover("unresolved runtime constructor"); return (uintptr_t)0; }
    if(ties>1) { mak_ir_recover("runtime constructor tie"); }
    cache[slot]=best; keys[slot]=h; sig[slot]=(uint32_t)n; return best->make(args,kinds,n); }
static void (*mak_user_a)(struct mak__a *, struct mak__a *, struct mak__a *);
static void (*mak_user_b)(struct mak__a *, struct mak__b *);
static void (*mak_user_e)(struct mak__a *, struct mak__a *);
static void (*mak_user_f)(int32_t);
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
static double (*mak_user_av)(void);

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
        int32_t mak_local___mak_opt_scrut_0_2 = mak_ir_ext_0();
        int32_t scrut_3 = mak_local___mak_opt_scrut_0_2 < (int32_t)(INT64_C(0));
        if ((scrut_3 == INT64_C(1))) {
            (out)->tag = mak_ir_tag_3;
            struct mak__a *arena_call_4 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
            if (!arena_call_4) {
                mak_ir_recover("ADT result allocation failed");
                arena_call_4 = &mak_ir_default_obj__a;
             } else {
                mak_ir_fn_0(a0, &mak_ir_ctor_obj_0, (struct mak__a *)(arena_call_4));
            }
            (out)->v.v3.f0 = arena_call_4;
        }
        else if (1) {
            int32_t scrut_5 = mak_local___mak_opt_scrut_0_2 == (int32_t)(INT64_C(10));
            if ((scrut_5 == INT64_C(1))) {
                (out)->tag = mak_ir_tag_3;
                struct mak__a *arena_call_6 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
                if (!arena_call_6) {
                    mak_ir_recover("ADT result allocation failed");
                    arena_call_6 = &mak_ir_default_obj__a;
                 } else {
                    mak_ir_fn_0(a0, &mak_ir_ctor_obj_0, (struct mak__a *)(arena_call_6));
                }
                (out)->v.v3.f0 = arena_call_6;
            }
            else if (1) {
                struct mak__a *arena_obj_8 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
                if (!arena_obj_8) {
                    mak_ir_recover("ADT object allocation failed");
                    arena_obj_8 = &mak_ir_default_obj__a;
                 } else {
                    (arena_obj_8)->tag = mak_ir_tag_1;
                    (arena_obj_8)->v.v1.f0 = mak_local___mak_opt_scrut_0_2;
                    (arena_obj_8)->v.v1.f1 = a0;
                }
                struct mak__a * tailarg_7 = arena_obj_8;
                a0 = tailarg_7;
                continue;
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
            switch (mak_local_bg_3) {
                case INT64_C(9): {
                    struct mak__a * tailarg_9 = mak_local_bh_4;
                    a0 = tailarg_9;
                    continue;
                }
                case INT64_C(10): {
                    struct mak__a * tailarg_10 = mak_local_bh_4;
                    a0 = tailarg_10;
                    continue;
                }
                case INT64_C(13): {
                    struct mak__a * tailarg_11 = mak_local_bh_4;
                    a0 = tailarg_11;
                    continue;
                }
                case INT64_C(32): {
                    struct mak__a * tailarg_12 = mak_local_bh_4;
                    a0 = tailarg_12;
                    continue;
                }
                default: {
                    *(out) = *(a0);
                    break;
                }
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
            int32_t scrut_13 = mak_local_cb_5 == (int32_t)(INT64_C(95));
            if ((scrut_13 == INT64_C(1))) {
                int32_t scrut_14 = mak_local_cb_5 >= (int32_t)(INT64_C(65));
                if ((scrut_14 == INT64_C(0))) {
                    struct mak__a * tailarg_15 = mak_local_cc_6;
                    int64_t eval_18 = (a1 * INT64_C(131));
                    int64_t eval_17 = (eval_18 + (int64_t)(mak_local_cb_5));
                    int64_t tailarg_16 = (eval_17 % INT64_C(1000000007));
                    a0 = tailarg_15;
                    a1 = tailarg_16;
                    continue;
                }
                else if (1) {
                    int32_t scrut_19 = mak_local_cb_5 <= (int32_t)(INT64_C(90));
                    if ((scrut_19 == INT64_C(1))) {
                        struct mak__a * tailarg_20 = mak_local_cc_6;
                        int64_t eval_24 = (a1 * INT64_C(131));
                        int64_t eval_23 = (eval_24 + (int64_t)(mak_local_cb_5));
                        int64_t eval_22 = (eval_23 + INT64_C(32));
                        int64_t tailarg_21 = (eval_22 % INT64_C(1000000007));
                        a0 = tailarg_20;
                        a1 = tailarg_21;
                        continue;
                    }
                    else if (1) {
                        struct mak__a * tailarg_25 = mak_local_cc_6;
                        int64_t eval_28 = (a1 * INT64_C(131));
                        int64_t eval_27 = (eval_28 + (int64_t)(mak_local_cb_5));
                        int64_t tailarg_26 = (eval_27 % INT64_C(1000000007));
                        a0 = tailarg_25;
                        a1 = tailarg_26;
                        continue;
                    }
                }
            }
            else if (1) {
                int32_t scrut_29 = mak_local_cb_5 >= (int32_t)(INT64_C(65));
                if ((scrut_29 == INT64_C(0))) {
                    int32_t scrut_30 = mak_local_cb_5 >= (int32_t)(INT64_C(97));
                    if ((scrut_30 == INT64_C(0))) {
                        int32_t scrut_31 = mak_local_cb_5 >= (int32_t)(INT64_C(48));
                        if ((scrut_31 == INT64_C(0))) {
                            (out)->tag = mak_ir_tag_5;
                            (out)->v.v5.f0 = a1;
                            (out)->v.v5.f1 = a0;
                        }
                        else if (1) {
                            int32_t scrut_32 = mak_local_cb_5 <= (int32_t)(INT64_C(57));
                            if ((scrut_32 == INT64_C(1))) {
                                int32_t scrut_33 = mak_local_cb_5 >= (int32_t)(INT64_C(65));
                                if ((scrut_33 == INT64_C(0))) {
                                    struct mak__a * tailarg_34 = mak_local_cc_6;
                                    int64_t eval_37 = (a1 * INT64_C(131));
                                    int64_t eval_36 = (eval_37 + (int64_t)(mak_local_cb_5));
                                    int64_t tailarg_35 = (eval_36 % INT64_C(1000000007));
                                    a0 = tailarg_34;
                                    a1 = tailarg_35;
                                    continue;
                                }
                                else if (1) {
                                    int32_t scrut_38 = mak_local_cb_5 <= (int32_t)(INT64_C(90));
                                    if ((scrut_38 == INT64_C(1))) {
                                        struct mak__a * tailarg_39 = mak_local_cc_6;
                                        int64_t eval_43 = (a1 * INT64_C(131));
                                        int64_t eval_42 = (eval_43 + (int64_t)(mak_local_cb_5));
                                        int64_t eval_41 = (eval_42 + INT64_C(32));
                                        int64_t tailarg_40 = (eval_41 % INT64_C(1000000007));
                                        a0 = tailarg_39;
                                        a1 = tailarg_40;
                                        continue;
                                    }
                                    else if (1) {
                                        struct mak__a * tailarg_44 = mak_local_cc_6;
                                        int64_t eval_47 = (a1 * INT64_C(131));
                                        int64_t eval_46 = (eval_47 + (int64_t)(mak_local_cb_5));
                                        int64_t tailarg_45 = (eval_46 % INT64_C(1000000007));
                                        a0 = tailarg_44;
                                        a1 = tailarg_45;
                                        continue;
                                    }
                                }
                            }
                            else if (1) {
                                (out)->tag = mak_ir_tag_5;
                                (out)->v.v5.f0 = a1;
                                (out)->v.v5.f1 = a0;
                            }
                        }
                    }
                    else if (1) {
                        int32_t scrut_48 = mak_local_cb_5 <= (int32_t)(INT64_C(122));
                        if ((scrut_48 == INT64_C(1))) {
                            int32_t scrut_49 = mak_local_cb_5 >= (int32_t)(INT64_C(65));
                            if ((scrut_49 == INT64_C(0))) {
                                struct mak__a * tailarg_50 = mak_local_cc_6;
                                int64_t eval_53 = (a1 * INT64_C(131));
                                int64_t eval_52 = (eval_53 + (int64_t)(mak_local_cb_5));
                                int64_t tailarg_51 = (eval_52 % INT64_C(1000000007));
                                a0 = tailarg_50;
                                a1 = tailarg_51;
                                continue;
                            }
                            else if (1) {
                                int32_t scrut_54 = mak_local_cb_5 <= (int32_t)(INT64_C(90));
                                if ((scrut_54 == INT64_C(1))) {
                                    struct mak__a * tailarg_55 = mak_local_cc_6;
                                    int64_t eval_59 = (a1 * INT64_C(131));
                                    int64_t eval_58 = (eval_59 + (int64_t)(mak_local_cb_5));
                                    int64_t eval_57 = (eval_58 + INT64_C(32));
                                    int64_t tailarg_56 = (eval_57 % INT64_C(1000000007));
                                    a0 = tailarg_55;
                                    a1 = tailarg_56;
                                    continue;
                                }
                                else if (1) {
                                    struct mak__a * tailarg_60 = mak_local_cc_6;
                                    int64_t eval_63 = (a1 * INT64_C(131));
                                    int64_t eval_62 = (eval_63 + (int64_t)(mak_local_cb_5));
                                    int64_t tailarg_61 = (eval_62 % INT64_C(1000000007));
                                    a0 = tailarg_60;
                                    a1 = tailarg_61;
                                    continue;
                                }
                            }
                        }
                        else if (1) {
                            int32_t scrut_64 = mak_local_cb_5 >= (int32_t)(INT64_C(48));
                            if ((scrut_64 == INT64_C(0))) {
                                (out)->tag = mak_ir_tag_5;
                                (out)->v.v5.f0 = a1;
                                (out)->v.v5.f1 = a0;
                            }
                            else if (1) {
                                int32_t scrut_65 = mak_local_cb_5 <= (int32_t)(INT64_C(57));
                                if ((scrut_65 == INT64_C(1))) {
                                    int32_t scrut_66 = mak_local_cb_5 >= (int32_t)(INT64_C(65));
                                    if ((scrut_66 == INT64_C(0))) {
                                        struct mak__a * tailarg_67 = mak_local_cc_6;
                                        int64_t eval_70 = (a1 * INT64_C(131));
                                        int64_t eval_69 = (eval_70 + (int64_t)(mak_local_cb_5));
                                        int64_t tailarg_68 = (eval_69 % INT64_C(1000000007));
                                        a0 = tailarg_67;
                                        a1 = tailarg_68;
                                        continue;
                                    }
                                    else if (1) {
                                        int32_t scrut_71 = mak_local_cb_5 <= (int32_t)(INT64_C(90));
                                        if ((scrut_71 == INT64_C(1))) {
                                            struct mak__a * tailarg_72 = mak_local_cc_6;
                                            int64_t eval_76 = (a1 * INT64_C(131));
                                            int64_t eval_75 = (eval_76 + (int64_t)(mak_local_cb_5));
                                            int64_t eval_74 = (eval_75 + INT64_C(32));
                                            int64_t tailarg_73 = (eval_74 % INT64_C(1000000007));
                                            a0 = tailarg_72;
                                            a1 = tailarg_73;
                                            continue;
                                        }
                                        else if (1) {
                                            struct mak__a * tailarg_77 = mak_local_cc_6;
                                            int64_t eval_80 = (a1 * INT64_C(131));
                                            int64_t eval_79 = (eval_80 + (int64_t)(mak_local_cb_5));
                                            int64_t tailarg_78 = (eval_79 % INT64_C(1000000007));
                                            a0 = tailarg_77;
                                            a1 = tailarg_78;
                                            continue;
                                        }
                                    }
                                }
                                else if (1) {
                                    (out)->tag = mak_ir_tag_5;
                                    (out)->v.v5.f0 = a1;
                                    (out)->v.v5.f1 = a0;
                                }
                            }
                        }
                    }
                }
                else if (1) {
                    int32_t scrut_81 = mak_local_cb_5 <= (int32_t)(INT64_C(90));
                    if ((scrut_81 == INT64_C(1))) {
                        int32_t scrut_82 = mak_local_cb_5 >= (int32_t)(INT64_C(65));
                        if ((scrut_82 == INT64_C(0))) {
                            struct mak__a * tailarg_83 = mak_local_cc_6;
                            int64_t eval_86 = (a1 * INT64_C(131));
                            int64_t eval_85 = (eval_86 + (int64_t)(mak_local_cb_5));
                            int64_t tailarg_84 = (eval_85 % INT64_C(1000000007));
                            a0 = tailarg_83;
                            a1 = tailarg_84;
                            continue;
                        }
                        else if (1) {
                            int32_t scrut_87 = mak_local_cb_5 <= (int32_t)(INT64_C(90));
                            if ((scrut_87 == INT64_C(1))) {
                                struct mak__a * tailarg_88 = mak_local_cc_6;
                                int64_t eval_92 = (a1 * INT64_C(131));
                                int64_t eval_91 = (eval_92 + (int64_t)(mak_local_cb_5));
                                int64_t eval_90 = (eval_91 + INT64_C(32));
                                int64_t tailarg_89 = (eval_90 % INT64_C(1000000007));
                                a0 = tailarg_88;
                                a1 = tailarg_89;
                                continue;
                            }
                            else if (1) {
                                struct mak__a * tailarg_93 = mak_local_cc_6;
                                int64_t eval_96 = (a1 * INT64_C(131));
                                int64_t eval_95 = (eval_96 + (int64_t)(mak_local_cb_5));
                                int64_t tailarg_94 = (eval_95 % INT64_C(1000000007));
                                a0 = tailarg_93;
                                a1 = tailarg_94;
                                continue;
                            }
                        }
                    }
                    else if (1) {
                        int32_t scrut_97 = mak_local_cb_5 >= (int32_t)(INT64_C(48));
                        if ((scrut_97 == INT64_C(0))) {
                            (out)->tag = mak_ir_tag_5;
                            (out)->v.v5.f0 = a1;
                            (out)->v.v5.f1 = a0;
                        }
                        else if (1) {
                            int32_t scrut_98 = mak_local_cb_5 <= (int32_t)(INT64_C(57));
                            if ((scrut_98 == INT64_C(1))) {
                                int32_t scrut_99 = mak_local_cb_5 >= (int32_t)(INT64_C(65));
                                if ((scrut_99 == INT64_C(0))) {
                                    struct mak__a * tailarg_100 = mak_local_cc_6;
                                    int64_t eval_103 = (a1 * INT64_C(131));
                                    int64_t eval_102 = (eval_103 + (int64_t)(mak_local_cb_5));
                                    int64_t tailarg_101 = (eval_102 % INT64_C(1000000007));
                                    a0 = tailarg_100;
                                    a1 = tailarg_101;
                                    continue;
                                }
                                else if (1) {
                                    int32_t scrut_104 = mak_local_cb_5 <= (int32_t)(INT64_C(90));
                                    if ((scrut_104 == INT64_C(1))) {
                                        struct mak__a * tailarg_105 = mak_local_cc_6;
                                        int64_t eval_109 = (a1 * INT64_C(131));
                                        int64_t eval_108 = (eval_109 + (int64_t)(mak_local_cb_5));
                                        int64_t eval_107 = (eval_108 + INT64_C(32));
                                        int64_t tailarg_106 = (eval_107 % INT64_C(1000000007));
                                        a0 = tailarg_105;
                                        a1 = tailarg_106;
                                        continue;
                                    }
                                    else if (1) {
                                        struct mak__a * tailarg_110 = mak_local_cc_6;
                                        int64_t eval_113 = (a1 * INT64_C(131));
                                        int64_t eval_112 = (eval_113 + (int64_t)(mak_local_cb_5));
                                        int64_t tailarg_111 = (eval_112 % INT64_C(1000000007));
                                        a0 = tailarg_110;
                                        a1 = tailarg_111;
                                        continue;
                                    }
                                }
                            }
                            else if (1) {
                                (out)->tag = mak_ir_tag_5;
                                (out)->v.v5.f0 = a1;
                                (out)->v.v5.f1 = a0;
                            }
                        }
                    }
                }
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
        int32_t scrut_114 = (int32_t)(a0) > (int32_t)(INT64_C(0));
        if ((scrut_114 == INT64_C(1))) {
            double eval_115 = mak_ir_fn_4((a0 - INT64_C(1)));
            return ((double)(10.00000000000000000) * (double)(eval_115));
        }
        else if (1) {
            double eval_116 = mak_ir_fn_4((a0 + INT64_C(1)));
            return ((double)(0.10000000000000001) * (double)(eval_116));
        }
    }
}

static inline void mak_ir_fn_5(struct mak__a * a0, int64_t a1, int64_t a2, struct mak__e *out) {
    for (;;) {
        if ((a0 != NULL && (a0)->tag == mak_ir_tag_1)) {
            int32_t mak_local_co_7 = (a0)->v.v1.f0;
            struct mak__a * mak_local_cp_8 = (a0)->v.v1.f1;
            int32_t scrut_117 = mak_local_co_7 >= (int32_t)(INT64_C(48));
            if ((scrut_117 == INT64_C(0))) {
                if ((a2 == INT64_C(1))) {
                    (out)->tag = mak_ir_tag_9;
                    (out)->v.v9.f0 = a1;
                    (out)->v.v9.f1 = a0;
                }
                else if (1) {
                    *(out) = mak_ir_ctor_obj_8;
                }
            }
            else if (1) {
                int32_t scrut_118 = mak_local_co_7 <= (int32_t)(INT64_C(57));
                if ((scrut_118 == INT64_C(1))) {
                    struct mak__a * tailarg_119 = mak_local_cp_8;
                    int64_t eval_122 = (a1 * INT64_C(10));
                    int64_t eval_121 = (eval_122 + (int64_t)(mak_local_co_7));
                    int64_t tailarg_120 = (eval_121 - INT64_C(48));
                    int64_t tailarg_123 = INT64_C(1);
                    a0 = tailarg_119;
                    a1 = tailarg_120;
                    a2 = tailarg_123;
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
    struct mak__a *arena_call_125 = (struct mak__a *)__builtin_alloca(sizeof(struct mak__a));
        mak_ir_fn_2(a0, (struct mak__a *)(arena_call_125));
    struct mak__a * scrut_124 = arena_call_125;
    if ((scrut_124 != NULL && (scrut_124)->tag == mak_ir_tag_1)) {
        int32_t mak_local_cs_9 = (scrut_124)->v.v1.f0;
        struct mak__a * mak_local_ct_10 = (scrut_124)->v.v1.f1;
        switch (mak_local_cs_9) {
            case INT64_C(43): {
                mak_ir_fn_5(mak_local_ct_10, INT64_C(0), INT64_C(0), (struct mak__e *)(out));
                break;
            }
            case INT64_C(45): {
                struct mak__e *arena_call_127 = (struct mak__e *)__builtin_alloca(sizeof(struct mak__e));
                    mak_ir_fn_5(mak_local_ct_10, INT64_C(0), INT64_C(0), (struct mak__e *)(arena_call_127));
                struct mak__e * scrut_126 = arena_call_127;
                if ((scrut_126 != NULL && (scrut_126)->tag == mak_ir_tag_8)) {
                    *(out) = mak_ir_ctor_obj_8;
                }
                else if ((scrut_126 != NULL && (scrut_126)->tag == mak_ir_tag_9)) {
                    int64_t mak_local_cu_11 = (scrut_126)->v.v9.f0;
                    struct mak__a * mak_local_cv_12 = (scrut_126)->v.v9.f1;
                    (out)->tag = mak_ir_tag_9;
                    (out)->v.v9.f0 = (-mak_local_cu_11);
                    (out)->v.v9.f1 = mak_local_cv_12;
                }
                else {
                    *(out) = *(&mak_ir_default_obj__e);
                }
                break;
            }
            default: {
                int32_t scrut_128 = mak_local_cs_9 >= (int32_t)(INT64_C(48));
                if ((scrut_128 == INT64_C(0))) {
                    *(out) = mak_ir_ctor_obj_8;
                }
                else if (1) {
                    int32_t scrut_129 = mak_local_cs_9 <= (int32_t)(INT64_C(57));
                    if ((scrut_129 == INT64_C(1))) {
                        struct mak__a *arena_obj_130 = (struct mak__a *)__builtin_alloca(sizeof(struct mak__a));
                            (arena_obj_130)->tag = mak_ir_tag_1;
                            (arena_obj_130)->v.v1.f0 = mak_local_cs_9;
                            (arena_obj_130)->v.v1.f1 = mak_local_ct_10;
                        mak_ir_fn_5(arena_obj_130, INT64_C(0), INT64_C(0), (struct mak__e *)(out));
                    }
                    else if (1) {
                        *(out) = mak_ir_ctor_obj_8;
                    }
                }
                break;
            }
        }
    }
    else if ((scrut_124 != NULL && (scrut_124)->tag == mak_ir_tag_0)) {
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
            int32_t scrut_131 = mak_local_cy_13 >= (int32_t)(INT64_C(48));
            if ((scrut_131 == INT64_C(0))) {
                (out)->tag = mak_ir_tag_7;
                (out)->v.v7.f0 = a1;
                (out)->v.v7.f1 = a0;
            }
            else if (1) {
                int32_t scrut_132 = mak_local_cy_13 <= (int32_t)(INT64_C(57));
                if ((scrut_132 == INT64_C(1))) {
                    switch (mak_local_cy_13) {
                        case INT64_C(48): {
                            struct mak__a * tailarg_133 = mak_local_cz_14;
                            double eval_135 = ((double)(a1) * (double)(10.00000000000000000));
                            double tailarg_134 = ((double)(eval_135) + (double)(0.00000000000000000));
                            a0 = tailarg_133;
                            a1 = tailarg_134;
                            continue;
                        }
                        case INT64_C(49): {
                            struct mak__a * tailarg_136 = mak_local_cz_14;
                            double eval_138 = ((double)(a1) * (double)(10.00000000000000000));
                            double tailarg_137 = ((double)(eval_138) + (double)(1.00000000000000000));
                            a0 = tailarg_136;
                            a1 = tailarg_137;
                            continue;
                        }
                        case INT64_C(50): {
                            struct mak__a * tailarg_139 = mak_local_cz_14;
                            double eval_141 = ((double)(a1) * (double)(10.00000000000000000));
                            double tailarg_140 = ((double)(eval_141) + (double)(2.00000000000000000));
                            a0 = tailarg_139;
                            a1 = tailarg_140;
                            continue;
                        }
                        case INT64_C(51): {
                            struct mak__a * tailarg_142 = mak_local_cz_14;
                            double eval_144 = ((double)(a1) * (double)(10.00000000000000000));
                            double tailarg_143 = ((double)(eval_144) + (double)(3.00000000000000000));
                            a0 = tailarg_142;
                            a1 = tailarg_143;
                            continue;
                        }
                        case INT64_C(52): {
                            struct mak__a * tailarg_145 = mak_local_cz_14;
                            double eval_147 = ((double)(a1) * (double)(10.00000000000000000));
                            double tailarg_146 = ((double)(eval_147) + (double)(4.00000000000000000));
                            a0 = tailarg_145;
                            a1 = tailarg_146;
                            continue;
                        }
                        case INT64_C(53): {
                            struct mak__a * tailarg_148 = mak_local_cz_14;
                            double eval_150 = ((double)(a1) * (double)(10.00000000000000000));
                            double tailarg_149 = ((double)(eval_150) + (double)(5.00000000000000000));
                            a0 = tailarg_148;
                            a1 = tailarg_149;
                            continue;
                        }
                        case INT64_C(54): {
                            struct mak__a * tailarg_151 = mak_local_cz_14;
                            double eval_153 = ((double)(a1) * (double)(10.00000000000000000));
                            double tailarg_152 = ((double)(eval_153) + (double)(6.00000000000000000));
                            a0 = tailarg_151;
                            a1 = tailarg_152;
                            continue;
                        }
                        case INT64_C(55): {
                            struct mak__a * tailarg_154 = mak_local_cz_14;
                            double eval_156 = ((double)(a1) * (double)(10.00000000000000000));
                            double tailarg_155 = ((double)(eval_156) + (double)(7.00000000000000000));
                            a0 = tailarg_154;
                            a1 = tailarg_155;
                            continue;
                        }
                        case INT64_C(56): {
                            struct mak__a * tailarg_157 = mak_local_cz_14;
                            double eval_159 = ((double)(a1) * (double)(10.00000000000000000));
                            double tailarg_158 = ((double)(eval_159) + (double)(8.00000000000000000));
                            a0 = tailarg_157;
                            a1 = tailarg_158;
                            continue;
                        }
                        case INT64_C(57): {
                            struct mak__a * tailarg_160 = mak_local_cz_14;
                            double eval_162 = ((double)(a1) * (double)(10.00000000000000000));
                            double tailarg_161 = ((double)(eval_162) + (double)(9.00000000000000000));
                            a0 = tailarg_160;
                            a1 = tailarg_161;
                            continue;
                        }
                        default: {
                            struct mak__a * tailarg_163 = mak_local_cz_14;
                            double eval_165 = ((double)(a1) * (double)(10.00000000000000000));
                            double tailarg_164 = ((double)(eval_165) + (double)(0.00000000000000000));
                            a0 = tailarg_163;
                            a1 = tailarg_164;
                            continue;
                        }
                    }
                }
                else if (1) {
                    (out)->tag = mak_ir_tag_7;
                    (out)->v.v7.f0 = a1;
                    (out)->v.v7.f1 = a0;
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

static inline void mak_ir_fn_8(struct mak__a * a0, double a1, double a2, struct mak__d *out) {
    if ((a0 != NULL && (a0)->tag == mak_ir_tag_1)) {
        int32_t mak_local_dd_15 = (a0)->v.v1.f0;
        struct mak__a * mak_local_de_16 = (a0)->v.v1.f1;
        int32_t scrut_166 = mak_local_dd_15 >= (int32_t)(INT64_C(48));
        if ((scrut_166 == INT64_C(0))) {
            switch (mak_local_dd_15) {
                case INT64_C(101): {
                    struct mak__e *arena_call_168 = (struct mak__e *)__builtin_alloca(sizeof(struct mak__e));
                        mak_ir_fn_6(mak_local_de_16, INT64_C(1), (struct mak__e *)(arena_call_168));
                    struct mak__e * scrut_167 = arena_call_168;
                    if ((scrut_167 != NULL && (scrut_167)->tag == mak_ir_tag_8)) {
                        *(out) = mak_ir_ctor_obj_6;
                    }
                    else if ((scrut_167 != NULL && (scrut_167)->tag == mak_ir_tag_9)) {
                        int64_t mak_local_df_17 = (scrut_167)->v.v9.f0;
                        struct mak__a * mak_local_dg_18 = (scrut_167)->v.v9.f1;
                        (out)->tag = mak_ir_tag_7;
                        double eval_169 = mak_ir_fn_4(mak_local_df_17);
                        (out)->v.v7.f0 = ((double)(a1) * (double)(eval_169));
                        (out)->v.v7.f1 = mak_local_dg_18;
                    }
                    else {
                        *(out) = *(&mak_ir_default_obj__d);
                    }
                    break;
                }
                case INT64_C(69): {
                    struct mak__e *arena_call_171 = (struct mak__e *)__builtin_alloca(sizeof(struct mak__e));
                        mak_ir_fn_6(mak_local_de_16, INT64_C(1), (struct mak__e *)(arena_call_171));
                    struct mak__e * scrut_170 = arena_call_171;
                    if ((scrut_170 != NULL && (scrut_170)->tag == mak_ir_tag_8)) {
                        *(out) = mak_ir_ctor_obj_6;
                    }
                    else if ((scrut_170 != NULL && (scrut_170)->tag == mak_ir_tag_9)) {
                        int64_t mak_local_dh_19 = (scrut_170)->v.v9.f0;
                        struct mak__a * mak_local_di_20 = (scrut_170)->v.v9.f1;
                        (out)->tag = mak_ir_tag_7;
                        double eval_172 = mak_ir_fn_4(mak_local_dh_19);
                        (out)->v.v7.f0 = ((double)(a1) * (double)(eval_172));
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
        else if (1) {
            int32_t scrut_173 = mak_local_dd_15 <= (int32_t)(INT64_C(57));
            if ((scrut_173 == INT64_C(1))) {
                double arg_174 = ((double)(a2) * (double)(0.10000000000000001));
                struct mak__d *rt_heap_obj_175 = (struct mak__d *)(uintptr_t)((struct mak__d *)(uintptr_t)(mak_rt_call_function("", (const uint64_t[]){mak_rt_f64_bits(arg_174)}, (const uint8_t[]){2}, 1, 4)));
                if (!mak_ir_adt_copy_checked((void *)(out), sizeof(struct mak__d), UINT32_C(1259039422), (const void *)(rt_heap_obj_175)) ) {
                    *(out) = *(&mak_ir_default_obj__d);
                }
                mak_ir_heap_free((void *)rt_heap_obj_175);
            }
            else if (1) {
                switch (mak_local_dd_15) {
                    case INT64_C(101): {
                        struct mak__e *arena_call_177 = (struct mak__e *)__builtin_alloca(sizeof(struct mak__e));
                            mak_ir_fn_6(mak_local_de_16, INT64_C(1), (struct mak__e *)(arena_call_177));
                        struct mak__e * scrut_176 = arena_call_177;
                        if ((scrut_176 != NULL && (scrut_176)->tag == mak_ir_tag_8)) {
                            *(out) = mak_ir_ctor_obj_6;
                        }
                        else if ((scrut_176 != NULL && (scrut_176)->tag == mak_ir_tag_9)) {
                            int64_t mak_local_df_21 = (scrut_176)->v.v9.f0;
                            struct mak__a * mak_local_dg_22 = (scrut_176)->v.v9.f1;
                            (out)->tag = mak_ir_tag_7;
                            double eval_178 = mak_ir_fn_4(mak_local_df_21);
                            (out)->v.v7.f0 = ((double)(a1) * (double)(eval_178));
                            (out)->v.v7.f1 = mak_local_dg_22;
                        }
                        else {
                            *(out) = *(&mak_ir_default_obj__d);
                        }
                        break;
                    }
                    case INT64_C(69): {
                        struct mak__e *arena_call_180 = (struct mak__e *)__builtin_alloca(sizeof(struct mak__e));
                            mak_ir_fn_6(mak_local_de_16, INT64_C(1), (struct mak__e *)(arena_call_180));
                        struct mak__e * scrut_179 = arena_call_180;
                        if ((scrut_179 != NULL && (scrut_179)->tag == mak_ir_tag_8)) {
                            *(out) = mak_ir_ctor_obj_6;
                        }
                        else if ((scrut_179 != NULL && (scrut_179)->tag == mak_ir_tag_9)) {
                            int64_t mak_local_dh_23 = (scrut_179)->v.v9.f0;
                            struct mak__a * mak_local_di_24 = (scrut_179)->v.v9.f1;
                            (out)->tag = mak_ir_tag_7;
                            double eval_181 = mak_ir_fn_4(mak_local_dh_23);
                            (out)->v.v7.f0 = ((double)(a1) * (double)(eval_181));
                            (out)->v.v7.f1 = mak_local_di_24;
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
                double eval_183 = mak_ir_ext_7(a2);
                int32_t scrut_182 = (int32_t)(eval_183) == (int32_t)(0.00000000000000000);
                if ((scrut_182 == INT64_C(1))) {
                    return mak_ir_ext_7(a1);
                }
                else if (1) {
                    int64_t tailarg_184 = INT64_C(1780652);
                    double tailarg_185 = mak_ir_ext_7(a2);
                    double tailarg_186 = mak_ir_ext_12(mak_ir_ext_7(a1), mak_ir_ext_7(a2));
                    a0 = tailarg_184;
                    a1 = tailarg_185;
                    a2 = tailarg_186;
                    continue;
                }
            }
            case INT64_C(1866466): {
                double eval_188 = mak_ir_fn_9(INT64_C(1780652), a1, a2);
                int32_t scrut_187 = (int32_t)(eval_188) == (int32_t)(0.00000000000000000);
                if ((scrut_187 == INT64_C(1))) {
                    return 0.00000000000000000;
                }
                else if (1) {
                    double eval_189 = mak_ir_ext_7(((double)(a1) * (double)(a2)));
                    double eval_190 = mak_ir_fn_9(INT64_C(1780652), a1, a2);
                    return ((double)(eval_189) / (double)(eval_190));
                }
            }
            case INT64_C(1900793): {
                double eval_192 = mak_ir_ext_11(a2);
                int32_t scrut_191 = (int32_t)(eval_192) < (int32_t)(0.00000000000000000);
                if ((scrut_191 == INT64_C(1))) {
                    return 0.00000000000000000;
                }
                else if (1) {
                    double eval_194 = mak_ir_ext_11(a2);
                    double eval_195 = mak_ir_ext_11(a1);
                    int32_t scrut_193 = (int32_t)(eval_194) > (int32_t)(eval_195);
                    if ((scrut_193 == INT64_C(1))) {
                        return 0.00000000000000000;
                    }
                    else if (1) {
                        double eval_198 = mak_ir_ext_11(a1);
                        double eval_197 = mak_ir_ext_36(((double)(eval_198) + (double)(1.00000000000000000)));
                        double eval_200 = mak_ir_ext_11(a2);
                        double eval_199 = mak_ir_ext_36(((double)(eval_200) + (double)(1.00000000000000000)));
                        double eval_196 = ((double)(eval_197) - (double)(eval_199));
                        double eval_203 = mak_ir_ext_11(a1);
                        double eval_204 = mak_ir_ext_11(a2);
                        double eval_202 = ((double)(eval_203) - (double)(eval_204));
                        double eval_201 = mak_ir_ext_36(((double)(eval_202) + (double)(1.00000000000000000)));
                        return mak_ir_ext_11(mak_ir_ext_15(((double)(eval_196) - (double)(eval_201))));
                    }
                }
            }
            case INT64_C(224480257): {
                int64_t tailarg_205 = INT64_C(1900793);
                double tailarg_206 = a1;
                double tailarg_207 = a2;
                a0 = tailarg_205;
                a1 = tailarg_206;
                a2 = tailarg_207;
                continue;
            }
            case INT64_C(1902496): {
                double eval_209 = mak_ir_ext_11(a2);
                int32_t scrut_208 = (int32_t)(eval_209) < (int32_t)(0.00000000000000000);
                if ((scrut_208 == INT64_C(1))) {
                    return 0.00000000000000000;
                }
                else if (1) {
                    double eval_211 = mak_ir_ext_11(a2);
                    double eval_212 = mak_ir_ext_11(a1);
                    int32_t scrut_210 = (int32_t)(eval_211) > (int32_t)(eval_212);
                    if ((scrut_210 == INT64_C(1))) {
                        return 0.00000000000000000;
                    }
                    else if (1) {
                        double eval_214 = mak_ir_ext_11(a1);
                        double eval_213 = mak_ir_ext_36(((double)(eval_214) + (double)(1.00000000000000000)));
                        double eval_217 = mak_ir_ext_11(a1);
                        double eval_218 = mak_ir_ext_11(a2);
                        double eval_216 = ((double)(eval_217) - (double)(eval_218));
                        double eval_215 = mak_ir_ext_36(((double)(eval_216) + (double)(1.00000000000000000)));
                        return mak_ir_ext_11(mak_ir_ext_15(((double)(eval_213) - (double)(eval_215))));
                    }
                }
            }
            case INT64_C(253534496): {
                int64_t tailarg_219 = INT64_C(1902496);
                double tailarg_220 = a1;
                double tailarg_221 = a2;
                a0 = tailarg_219;
                a1 = tailarg_220;
                a2 = tailarg_221;
                continue;
            }
            case INT64_C(1884414): {
                int32_t scrut_222 = (int32_t)(a1) < (int32_t)(a2);
                if ((scrut_222 == INT64_C(1))) {
                    return a1;
                }
                else if (1) {
                    return a2;
                }
            }
            case INT64_C(1883376): {
                int32_t scrut_223 = (int32_t)(a1) > (int32_t)(a2);
                if ((scrut_223 == INT64_C(1))) {
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
        struct mak__a *arena_call_225 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
        if (!arena_call_225) {
            mak_ir_recover("ADT result allocation failed");
            arena_call_225 = &mak_ir_default_obj__a;
         } else {
            mak_ir_fn_2(a1, (struct mak__a *)(arena_call_225));
        }
        struct mak__a * scrut_224 = arena_call_225;
        if ((scrut_224 != NULL && (scrut_224)->tag == mak_ir_tag_1) && (scrut_224->v.v1.f0 == INT64_C(33))) {
            struct mak__a * mak_local_ge_25 = (scrut_224)->v.v1.f1;
            double tailarg_226 = mak_ir_ext_35(((double)(a0) + (double)(1.00000000000000000)));
            struct mak__a * tailarg_227 = mak_local_ge_25;
            a0 = tailarg_226;
            a1 = tailarg_227;
            continue;
        }
        else if ((scrut_224 != NULL && (scrut_224)->tag == mak_ir_tag_1) && (scrut_224->v.v1.f0 == INT64_C(37))) {
            struct mak__a * mak_local_gf_26 = (scrut_224)->v.v1.f1;
            struct mak__a *arena_call_229 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
            if (!arena_call_229) {
                mak_ir_recover("ADT result allocation failed");
                arena_call_229 = &mak_ir_default_obj__a;
             } else {
                mak_ir_fn_2(mak_local_gf_26, (struct mak__a *)(arena_call_229));
            }
            struct mak__a * scrut_228 = arena_call_229;
            if ((scrut_228 != NULL && (scrut_228)->tag == mak_ir_tag_0)) {
                (out)->tag = mak_ir_tag_10;
                (out)->v.v10.f0 = ((double)(a0) / (double)(100.00000000000000000));
                (out)->v.v10.f1 = &mak_ir_ctor_obj_0;
            }
            else if ((scrut_228 != NULL && (scrut_228)->tag == mak_ir_tag_1)) {
                int32_t mak_local_gg_27 = (scrut_228)->v.v1.f0;
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
    struct mak__f *arena_case_231 = (struct mak__f *)__builtin_alloca(sizeof(struct mak__f));
        struct mak__f *arena_case_233 = (struct mak__f *)__builtin_alloca(sizeof(struct mak__f));
            struct mak__a *arena_call_235 = (struct mak__a *)__builtin_alloca(sizeof(struct mak__a));
                mak_ir_fn_2(a0, (struct mak__a *)(arena_call_235));
            struct mak__a * scrut_234 = arena_call_235;
            if ((scrut_234 != NULL && (scrut_234)->tag == mak_ir_tag_0)) {
                *(arena_case_233) = mak_ir_ctor_obj_11;
            }
            else if ((scrut_234 != NULL && (scrut_234)->tag == mak_ir_tag_1)) {
                int32_t mak_local_fs_28 = (scrut_234)->v.v1.f0;
                struct mak__a * mak_local_ft_29 = (scrut_234)->v.v1.f1;
                if ((mak_local_fs_28 == INT64_C(40))) {
                    struct mak__f *arena_call_237 = (struct mak__f *)__builtin_alloca(sizeof(struct mak__f));
                        mak_ir_fn_16(mak_local_ft_29, (struct mak__f *)(arena_call_237));
                    struct mak__f * scrut_236 = arena_call_237;
                    if ((scrut_236 != NULL && (scrut_236)->tag == mak_ir_tag_11)) {
                        *(arena_case_233) = mak_ir_ctor_obj_11;
                    }
                    else if ((scrut_236 != NULL && (scrut_236)->tag == mak_ir_tag_10)) {
                        double mak_local_fu_30 = (scrut_236)->v.v10.f0;
                        struct mak__a * mak_local_fv_31 = (scrut_236)->v.v10.f1;
                        struct mak__a *arena_call_239 = (struct mak__a *)__builtin_alloca(sizeof(struct mak__a));
                            mak_ir_fn_2(mak_local_fv_31, (struct mak__a *)(arena_call_239));
                        struct mak__a * scrut_238 = arena_call_239;
                        if ((scrut_238 != NULL && (scrut_238)->tag == mak_ir_tag_1) && (scrut_238->v.v1.f0 == INT64_C(41))) {
                            struct mak__a * mak_local_fw_32 = (scrut_238)->v.v1.f1;
                            (arena_case_233)->tag = mak_ir_tag_10;
                            (arena_case_233)->v.v10.f0 = mak_local_fu_30;
                            (arena_case_233)->v.v10.f1 = mak_local_fw_32;
                        }
                        else if (1) {
                            *(arena_case_233) = mak_ir_ctor_obj_11;
                        }
                    }
                    else {
                        *(arena_case_233) = *(&mak_ir_default_obj__f);
                    }
                }
                else if (1) {
                    int32_t scrut_240 = mak_local_fs_28 == (int32_t)(INT64_C(95));
                    if ((scrut_240 == INT64_C(1))) {
                        struct mak__f *arena_case_242 = (struct mak__f *)__builtin_alloca(sizeof(struct mak__f));
                            struct mak__a *arena_call_244 = (struct mak__a *)__builtin_alloca(sizeof(struct mak__a));
                                struct mak__a *arena_obj_245 = (struct mak__a *)__builtin_alloca(sizeof(struct mak__a));
                                    (arena_obj_245)->tag = mak_ir_tag_1;
                                    (arena_obj_245)->v.v1.f0 = mak_local_fs_28;
                                    (arena_obj_245)->v.v1.f1 = mak_local_ft_29;
                                mak_ir_fn_2(arena_obj_245, (struct mak__a *)(arena_call_244));
                            struct mak__a * scrut_243 = arena_call_244;
                            if ((scrut_243 != NULL && (scrut_243)->tag == mak_ir_tag_1)) {
                                int32_t mak_local_ce_33 = (scrut_243)->v.v1.f0;
                                struct mak__a * mak_local_cf_34 = (scrut_243)->v.v1.f1;
                                int32_t scrut_246 = mak_local_ce_33 == (int32_t)(INT64_C(95));
                                if ((scrut_246 == INT64_C(1))) {
                                    struct mak__c *arena_call_247 = (struct mak__c *)__builtin_alloca(sizeof(struct mak__c));
                                        struct mak__a *arena_obj_248 = (struct mak__a *)__builtin_alloca(sizeof(struct mak__a));
                                            (arena_obj_248)->tag = mak_ir_tag_1;
                                            (arena_obj_248)->v.v1.f0 = mak_local_ce_33;
                                            (arena_obj_248)->v.v1.f1 = mak_local_cf_34;
                                        mak_ir_fn_3(arena_obj_248, INT64_C(0), (struct mak__c *)(arena_call_247));
                                    if (!mak_ir_adt_copy_checked((void *)(arena_case_242), sizeof(struct mak__f), UINT32_C(1225484184), (const void *)(uintptr_t)(arena_call_247))) {
                                        *(arena_case_242) = *(&mak_ir_default_obj__f);
                                    }
                                }
                                else if (1) {
                                    int32_t scrut_249 = mak_local_ce_33 >= (int32_t)(INT64_C(65));
                                    if ((scrut_249 == INT64_C(0))) {
                                        int32_t scrut_250 = mak_local_ce_33 >= (int32_t)(INT64_C(97));
                                        if ((scrut_250 == INT64_C(0))) {
                                            struct mak__f *rt_heap_obj_251 = (struct mak__f *)(uintptr_t)((struct mak__f *)(uintptr_t)(mak_rt_call_ctor("NBad:_f", (const uint64_t[]){}, (const uint8_t[]){}, 0)));
                                            if (!mak_ir_adt_copy_checked((void *)(arena_case_242), sizeof(struct mak__f), UINT32_C(1225484184), (const void *)(rt_heap_obj_251)) ) {
                                                *(arena_case_242) = *(&mak_ir_default_obj__f);
                                            }
                                            mak_ir_heap_free((void *)rt_heap_obj_251);
                                        }
                                        else if (1) {
                                            int32_t scrut_252 = mak_local_ce_33 <= (int32_t)(INT64_C(122));
                                            if ((scrut_252 == INT64_C(1))) {
                                                struct mak__c *arena_call_253 = (struct mak__c *)__builtin_alloca(sizeof(struct mak__c));
                                                    struct mak__a *arena_obj_254 = (struct mak__a *)__builtin_alloca(sizeof(struct mak__a));
                                                        (arena_obj_254)->tag = mak_ir_tag_1;
                                                        (arena_obj_254)->v.v1.f0 = mak_local_ce_33;
                                                        (arena_obj_254)->v.v1.f1 = mak_local_cf_34;
                                                    mak_ir_fn_3(arena_obj_254, INT64_C(0), (struct mak__c *)(arena_call_253));
                                                if (!mak_ir_adt_copy_checked((void *)(arena_case_242), sizeof(struct mak__f), UINT32_C(1225484184), (const void *)(uintptr_t)(arena_call_253))) {
                                                    *(arena_case_242) = *(&mak_ir_default_obj__f);
                                                }
                                            }
                                            else if (1) {
                                                struct mak__f *rt_heap_obj_255 = (struct mak__f *)(uintptr_t)((struct mak__f *)(uintptr_t)(mak_rt_call_ctor("NBad:_f", (const uint64_t[]){}, (const uint8_t[]){}, 0)));
                                                if (!mak_ir_adt_copy_checked((void *)(arena_case_242), sizeof(struct mak__f), UINT32_C(1225484184), (const void *)(rt_heap_obj_255)) ) {
                                                    *(arena_case_242) = *(&mak_ir_default_obj__f);
                                                }
                                                mak_ir_heap_free((void *)rt_heap_obj_255);
                                            }
                                        }
                                    }
                                    else if (1) {
                                        int32_t scrut_256 = mak_local_ce_33 <= (int32_t)(INT64_C(90));
                                        if ((scrut_256 == INT64_C(1))) {
                                            struct mak__c *arena_call_257 = (struct mak__c *)__builtin_alloca(sizeof(struct mak__c));
                                                struct mak__a *arena_obj_258 = (struct mak__a *)__builtin_alloca(sizeof(struct mak__a));
                                                    (arena_obj_258)->tag = mak_ir_tag_1;
                                                    (arena_obj_258)->v.v1.f0 = mak_local_ce_33;
                                                    (arena_obj_258)->v.v1.f1 = mak_local_cf_34;
                                                mak_ir_fn_3(arena_obj_258, INT64_C(0), (struct mak__c *)(arena_call_257));
                                            if (!mak_ir_adt_copy_checked((void *)(arena_case_242), sizeof(struct mak__f), UINT32_C(1225484184), (const void *)(uintptr_t)(arena_call_257))) {
                                                *(arena_case_242) = *(&mak_ir_default_obj__f);
                                            }
                                        }
                                        else if (1) {
                                            struct mak__f *rt_heap_obj_259 = (struct mak__f *)(uintptr_t)((struct mak__f *)(uintptr_t)(mak_rt_call_ctor("NBad:_f", (const uint64_t[]){}, (const uint8_t[]){}, 0)));
                                            if (!mak_ir_adt_copy_checked((void *)(arena_case_242), sizeof(struct mak__f), UINT32_C(1225484184), (const void *)(rt_heap_obj_259)) ) {
                                                *(arena_case_242) = *(&mak_ir_default_obj__f);
                                            }
                                            mak_ir_heap_free((void *)rt_heap_obj_259);
                                        }
                                    }
                                }
                            }
                            else if ((scrut_243 != NULL && (scrut_243)->tag == mak_ir_tag_0)) {
                                struct mak__f *rt_heap_obj_260 = (struct mak__f *)(uintptr_t)((struct mak__f *)(uintptr_t)(mak_rt_call_ctor("NBad:_f", (const uint64_t[]){}, (const uint8_t[]){}, 0)));
                                if (!mak_ir_adt_copy_checked((void *)(arena_case_242), sizeof(struct mak__f), UINT32_C(1225484184), (const void *)(rt_heap_obj_260)) ) {
                                    *(arena_case_242) = *(&mak_ir_default_obj__f);
                                }
                                mak_ir_heap_free((void *)rt_heap_obj_260);
                            }
                            else {
                                *(arena_case_242) = *(&mak_ir_default_obj__f);
                            }
                        struct mak__f * scrut_241 = arena_case_242;
                        if ((scrut_241 != NULL && (scrut_241)->tag == mak_ir_tag_11)) {
                            *(arena_case_233) = mak_ir_ctor_obj_11;
                        }
                        else if ((scrut_241 != NULL && (scrut_241)->tag == mak_ir_tag_10)) {
                            double mak_local_fx_35 = (scrut_241)->v.v10.f0;
                            struct mak__a * mak_local_fy_36 = (scrut_241)->v.v10.f1;
                            struct mak__a *arena_call_262 = (struct mak__a *)__builtin_alloca(sizeof(struct mak__a));
                                mak_ir_fn_2(mak_local_fy_36, (struct mak__a *)(arena_call_262));
                            struct mak__a * scrut_261 = arena_call_262;
                            if ((scrut_261 != NULL && (scrut_261)->tag == mak_ir_tag_1) && (scrut_261->v.v1.f0 == INT64_C(40))) {
                                struct mak__a * mak_local_fz_37 = (scrut_261)->v.v1.f1;
                                *(arena_case_233) = mak_ir_ctor_obj_11;
                            }
                            else if (1) {
                                *(arena_case_233) = mak_ir_ctor_obj_11;
                            }
                        }
                        else {
                            *(arena_case_233) = *(&mak_ir_default_obj__f);
                        }
                    }
                    else if (1) {
                        int32_t scrut_263 = mak_local_fs_28 >= (int32_t)(INT64_C(65));
                        if ((scrut_263 == INT64_C(0))) {
                            int32_t scrut_264 = mak_local_fs_28 >= (int32_t)(INT64_C(97));
                            if ((scrut_264 == INT64_C(0))) {
                                struct mak__f *arena_case_266 = (struct mak__f *)__builtin_alloca(sizeof(struct mak__f));
                                    struct mak__a *arena_call_268 = (struct mak__a *)__builtin_alloca(sizeof(struct mak__a));
                                        mak_ir_fn_2(a0, (struct mak__a *)(arena_call_268));
                                    struct mak__a * scrut_267 = arena_call_268;
                                    if ((scrut_267 != NULL && (scrut_267)->tag == mak_ir_tag_0)) {
                                        struct mak__f *rt_heap_obj_269 = (struct mak__f *)(uintptr_t)((struct mak__f *)(uintptr_t)(mak_rt_call_ctor("NBad:_f", (const uint64_t[]){}, (const uint8_t[]){}, 0)));
                                        if (!mak_ir_adt_copy_checked((void *)(arena_case_266), sizeof(struct mak__f), UINT32_C(1225484184), (const void *)(rt_heap_obj_269)) ) {
                                            *(arena_case_266) = *(&mak_ir_default_obj__f);
                                        }
                                        mak_ir_heap_free((void *)rt_heap_obj_269);
                                    }
                                    else if ((scrut_267 != NULL && (scrut_267)->tag == mak_ir_tag_1)) {
                                        int32_t mak_local_dk_65 = (scrut_267)->v.v1.f0;
                                        struct mak__a * mak_local_dl_66 = (scrut_267)->v.v1.f1;
                                        int32_t scrut_270 = mak_local_dk_65 >= (int32_t)(INT64_C(48));
                                        if ((scrut_270 == INT64_C(0))) {
                                            if ((mak_local_dk_65 == INT64_C(46))) {
                                                if ((mak_local_dl_66 != NULL && (mak_local_dl_66)->tag == mak_ir_tag_1)) {
                                                    int32_t mak_local_dw_67 = (mak_local_dl_66)->v.v1.f0;
                                                    int32_t scrut_271 = mak_local_dw_67 >= (int32_t)(INT64_C(48));
                                                    if ((scrut_271 == INT64_C(0))) {
                                                        struct mak__f *rt_heap_obj_272 = (struct mak__f *)(uintptr_t)((struct mak__f *)(uintptr_t)(mak_rt_call_ctor("NBad:_f", (const uint64_t[]){}, (const uint8_t[]){}, 0)));
                                                        if (!mak_ir_adt_copy_checked((void *)(arena_case_266), sizeof(struct mak__f), UINT32_C(1225484184), (const void *)(rt_heap_obj_272)) ) {
                                                            *(arena_case_266) = *(&mak_ir_default_obj__f);
                                                        }
                                                        mak_ir_heap_free((void *)rt_heap_obj_272);
                                                    }
                                                    else if (1) {
                                                        int32_t scrut_273 = mak_local_dw_67 <= (int32_t)(INT64_C(57));
                                                        if ((scrut_273 == INT64_C(1))) {
                                                            struct mak__d *arena_call_274 = (struct mak__d *)__builtin_alloca(sizeof(struct mak__d));
                                                                mak_ir_fn_8(mak_local_dl_66, 0.00000000000000000, 0.10000000000000001, (struct mak__d *)(arena_call_274));
                                                            if (!mak_ir_adt_copy_checked((void *)(arena_case_266), sizeof(struct mak__f), UINT32_C(1225484184), (const void *)(uintptr_t)(arena_call_274))) {
                                                                *(arena_case_266) = *(&mak_ir_default_obj__f);
                                                            }
                                                        }
                                                        else if (1) {
                                                            struct mak__f *rt_heap_obj_275 = (struct mak__f *)(uintptr_t)((struct mak__f *)(uintptr_t)(mak_rt_call_ctor("NBad:_f", (const uint64_t[]){}, (const uint8_t[]){}, 0)));
                                                            if (!mak_ir_adt_copy_checked((void *)(arena_case_266), sizeof(struct mak__f), UINT32_C(1225484184), (const void *)(rt_heap_obj_275)) ) {
                                                                *(arena_case_266) = *(&mak_ir_default_obj__f);
                                                            }
                                                            mak_ir_heap_free((void *)rt_heap_obj_275);
                                                        }
                                                    }
                                                }
                                                else if (1) {
                                                    struct mak__f *rt_heap_obj_276 = (struct mak__f *)(uintptr_t)((struct mak__f *)(uintptr_t)(mak_rt_call_ctor("NBad:_f", (const uint64_t[]){}, (const uint8_t[]){}, 0)));
                                                    if (!mak_ir_adt_copy_checked((void *)(arena_case_266), sizeof(struct mak__f), UINT32_C(1225484184), (const void *)(rt_heap_obj_276)) ) {
                                                        *(arena_case_266) = *(&mak_ir_default_obj__f);
                                                    }
                                                    mak_ir_heap_free((void *)rt_heap_obj_276);
                                                }
                                            }
                                            else if (1) {
                                                struct mak__f *rt_heap_obj_277 = (struct mak__f *)(uintptr_t)((struct mak__f *)(uintptr_t)(mak_rt_call_ctor("NBad:_f", (const uint64_t[]){}, (const uint8_t[]){}, 0)));
                                                if (!mak_ir_adt_copy_checked((void *)(arena_case_266), sizeof(struct mak__f), UINT32_C(1225484184), (const void *)(rt_heap_obj_277)) ) {
                                                    *(arena_case_266) = *(&mak_ir_default_obj__f);
                                                }
                                                mak_ir_heap_free((void *)rt_heap_obj_277);
                                            }
                                        }
                                        else if (1) {
                                            int32_t scrut_278 = mak_local_dk_65 <= (int32_t)(INT64_C(57));
                                            if ((scrut_278 == INT64_C(1))) {
                                                struct mak__d *arena_call_280 = (struct mak__d *)__builtin_alloca(sizeof(struct mak__d));
                                                    struct mak__a *arena_obj_281 = (struct mak__a *)__builtin_alloca(sizeof(struct mak__a));
                                                        (arena_obj_281)->tag = mak_ir_tag_1;
                                                        (arena_obj_281)->v.v1.f0 = mak_local_dk_65;
                                                        (arena_obj_281)->v.v1.f1 = mak_local_dl_66;
                                                    mak_ir_fn_7(arena_obj_281, 0.00000000000000000, (struct mak__d *)(arena_call_280));
                                                struct mak__d * scrut_279 = arena_call_280;
                                                if ((scrut_279 != NULL && (scrut_279)->tag == mak_ir_tag_6)) {
                                                    struct mak__f *rt_heap_obj_282 = (struct mak__f *)(uintptr_t)((struct mak__f *)(uintptr_t)(mak_rt_call_ctor("NBad:_f", (const uint64_t[]){}, (const uint8_t[]){}, 0)));
                                                    if (!mak_ir_adt_copy_checked((void *)(arena_case_266), sizeof(struct mak__f), UINT32_C(1225484184), (const void *)(rt_heap_obj_282)) ) {
                                                        *(arena_case_266) = *(&mak_ir_default_obj__f);
                                                    }
                                                    mak_ir_heap_free((void *)rt_heap_obj_282);
                                                }
                                                else if ((scrut_279 != NULL && (scrut_279)->tag == mak_ir_tag_7)) {
                                                    double mak_local_dm_68 = (scrut_279)->v.v7.f0;
                                                    struct mak__a * mak_local_dn_69 = (scrut_279)->v.v7.f1;
                                                    if ((mak_local_dn_69 != NULL && (mak_local_dn_69)->tag == mak_ir_tag_1) && (mak_local_dn_69->v.v1.f0 == INT64_C(46))) {
                                                        struct mak__a * mak_local_dp_70 = (mak_local_dn_69)->v.v1.f1;
                                                        struct mak__d *arena_call_283 = (struct mak__d *)__builtin_alloca(sizeof(struct mak__d));
                                                            mak_ir_fn_8(mak_local_dp_70, mak_local_dm_68, 0.10000000000000001, (struct mak__d *)(arena_call_283));
                                                        if (!mak_ir_adt_copy_checked((void *)(arena_case_266), sizeof(struct mak__f), UINT32_C(1225484184), (const void *)(uintptr_t)(arena_call_283))) {
                                                            *(arena_case_266) = *(&mak_ir_default_obj__f);
                                                        }
                                                    }
                                                    else if ((mak_local_dn_69 != NULL && (mak_local_dn_69)->tag == mak_ir_tag_1)) {
                                                        int32_t mak_local_dq_71 = (mak_local_dn_69)->v.v1.f0;
                                                        struct mak__a * mak_local_dr_72 = (mak_local_dn_69)->v.v1.f1;
                                                        switch (mak_local_dq_71) {
                                                            case INT64_C(101): {
                                                                struct mak__e *arena_call_285 = (struct mak__e *)__builtin_alloca(sizeof(struct mak__e));
                                                                    mak_ir_fn_6(mak_local_dr_72, INT64_C(1), (struct mak__e *)(arena_call_285));
                                                                struct mak__e * scrut_284 = arena_call_285;
                                                                if ((scrut_284 != NULL && (scrut_284)->tag == mak_ir_tag_8)) {
                                                                    struct mak__f *rt_heap_obj_286 = (struct mak__f *)(uintptr_t)((struct mak__f *)(uintptr_t)(mak_rt_call_ctor("NBad:_f", (const uint64_t[]){}, (const uint8_t[]){}, 0)));
                                                                    if (!mak_ir_adt_copy_checked((void *)(arena_case_266), sizeof(struct mak__f), UINT32_C(1225484184), (const void *)(rt_heap_obj_286)) ) {
                                                                        *(arena_case_266) = *(&mak_ir_default_obj__f);
                                                                    }
                                                                    mak_ir_heap_free((void *)rt_heap_obj_286);
                                                                }
                                                                else if ((scrut_284 != NULL && (scrut_284)->tag == mak_ir_tag_9)) {
                                                                    int64_t mak_local_ds_73 = (scrut_284)->v.v9.f0;
                                                                    struct mak__a * mak_local_dt_74 = (scrut_284)->v.v9.f1;
                                                                    struct mak__d *arena_obj_287 = (struct mak__d *)__builtin_alloca(sizeof(struct mak__d));
                                                                        (arena_obj_287)->tag = mak_ir_tag_7;
                                                                        double eval_288 = mak_ir_fn_4(mak_local_ds_73);
                                                                        (arena_obj_287)->v.v7.f0 = ((double)(mak_local_dm_68) * (double)(eval_288));
                                                                        (arena_obj_287)->v.v7.f1 = mak_local_dt_74;
                                                                    if (!mak_ir_adt_copy_checked((void *)(arena_case_266), sizeof(struct mak__f), UINT32_C(1225484184), (const void *)(uintptr_t)(arena_obj_287))) {
                                                                        *(arena_case_266) = *(&mak_ir_default_obj__f);
                                                                    }
                                                                }
                                                                else {
                                                                    *(arena_case_266) = *(&mak_ir_default_obj__f);
                                                                }
                                                                break;
                                                            }
                                                            case INT64_C(69): {
                                                                struct mak__e *arena_call_290 = (struct mak__e *)__builtin_alloca(sizeof(struct mak__e));
                                                                    mak_ir_fn_6(mak_local_dr_72, INT64_C(1), (struct mak__e *)(arena_call_290));
                                                                struct mak__e * scrut_289 = arena_call_290;
                                                                if ((scrut_289 != NULL && (scrut_289)->tag == mak_ir_tag_8)) {
                                                                    struct mak__f *rt_heap_obj_291 = (struct mak__f *)(uintptr_t)((struct mak__f *)(uintptr_t)(mak_rt_call_ctor("NBad:_f", (const uint64_t[]){}, (const uint8_t[]){}, 0)));
                                                                    if (!mak_ir_adt_copy_checked((void *)(arena_case_266), sizeof(struct mak__f), UINT32_C(1225484184), (const void *)(rt_heap_obj_291)) ) {
                                                                        *(arena_case_266) = *(&mak_ir_default_obj__f);
                                                                    }
                                                                    mak_ir_heap_free((void *)rt_heap_obj_291);
                                                                }
                                                                else if ((scrut_289 != NULL && (scrut_289)->tag == mak_ir_tag_9)) {
                                                                    int64_t mak_local_du_75 = (scrut_289)->v.v9.f0;
                                                                    struct mak__a * mak_local_dv_76 = (scrut_289)->v.v9.f1;
                                                                    struct mak__d *arena_obj_292 = (struct mak__d *)__builtin_alloca(sizeof(struct mak__d));
                                                                        (arena_obj_292)->tag = mak_ir_tag_7;
                                                                        double eval_293 = mak_ir_fn_4(mak_local_du_75);
                                                                        (arena_obj_292)->v.v7.f0 = ((double)(mak_local_dm_68) * (double)(eval_293));
                                                                        (arena_obj_292)->v.v7.f1 = mak_local_dv_76;
                                                                    if (!mak_ir_adt_copy_checked((void *)(arena_case_266), sizeof(struct mak__f), UINT32_C(1225484184), (const void *)(uintptr_t)(arena_obj_292))) {
                                                                        *(arena_case_266) = *(&mak_ir_default_obj__f);
                                                                    }
                                                                }
                                                                else {
                                                                    *(arena_case_266) = *(&mak_ir_default_obj__f);
                                                                }
                                                                break;
                                                            }
                                                            default: {
                                                                struct mak__d *arena_obj_294 = (struct mak__d *)__builtin_alloca(sizeof(struct mak__d));
                                                                    (arena_obj_294)->tag = mak_ir_tag_7;
                                                                    (arena_obj_294)->v.v7.f0 = mak_local_dm_68;
                                                                    (arena_obj_294)->v.v7.f1 = mak_local_dn_69;
                                                                if (!mak_ir_adt_copy_checked((void *)(arena_case_266), sizeof(struct mak__f), UINT32_C(1225484184), (const void *)(uintptr_t)(arena_obj_294))) {
                                                                    *(arena_case_266) = *(&mak_ir_default_obj__f);
                                                                }
                                                                break;
                                                            }
                                                        }
                                                    }
                                                    else if (1) {
                                                        struct mak__d *arena_obj_295 = (struct mak__d *)__builtin_alloca(sizeof(struct mak__d));
                                                            (arena_obj_295)->tag = mak_ir_tag_7;
                                                            (arena_obj_295)->v.v7.f0 = mak_local_dm_68;
                                                            (arena_obj_295)->v.v7.f1 = mak_local_dn_69;
                                                        if (!mak_ir_adt_copy_checked((void *)(arena_case_266), sizeof(struct mak__f), UINT32_C(1225484184), (const void *)(uintptr_t)(arena_obj_295))) {
                                                            *(arena_case_266) = *(&mak_ir_default_obj__f);
                                                        }
                                                    }
                                                }
                                                else {
                                                    *(arena_case_266) = *(&mak_ir_default_obj__f);
                                                }
                                            }
                                            else if (1) {
                                                if ((mak_local_dk_65 == INT64_C(46))) {
                                                    if ((mak_local_dl_66 != NULL && (mak_local_dl_66)->tag == mak_ir_tag_1)) {
                                                        int32_t mak_local_dw_77 = (mak_local_dl_66)->v.v1.f0;
                                                        int32_t scrut_296 = mak_local_dw_77 >= (int32_t)(INT64_C(48));
                                                        if ((scrut_296 == INT64_C(0))) {
                                                            struct mak__f *rt_heap_obj_297 = (struct mak__f *)(uintptr_t)((struct mak__f *)(uintptr_t)(mak_rt_call_ctor("NBad:_f", (const uint64_t[]){}, (const uint8_t[]){}, 0)));
                                                            if (!mak_ir_adt_copy_checked((void *)(arena_case_266), sizeof(struct mak__f), UINT32_C(1225484184), (const void *)(rt_heap_obj_297)) ) {
                                                                *(arena_case_266) = *(&mak_ir_default_obj__f);
                                                            }
                                                            mak_ir_heap_free((void *)rt_heap_obj_297);
                                                        }
                                                        else if (1) {
                                                            int32_t scrut_298 = mak_local_dw_77 <= (int32_t)(INT64_C(57));
                                                            if ((scrut_298 == INT64_C(1))) {
                                                                struct mak__d *arena_call_299 = (struct mak__d *)__builtin_alloca(sizeof(struct mak__d));
                                                                    mak_ir_fn_8(mak_local_dl_66, 0.00000000000000000, 0.10000000000000001, (struct mak__d *)(arena_call_299));
                                                                if (!mak_ir_adt_copy_checked((void *)(arena_case_266), sizeof(struct mak__f), UINT32_C(1225484184), (const void *)(uintptr_t)(arena_call_299))) {
                                                                    *(arena_case_266) = *(&mak_ir_default_obj__f);
                                                                }
                                                            }
                                                            else if (1) {
                                                                struct mak__f *rt_heap_obj_300 = (struct mak__f *)(uintptr_t)((struct mak__f *)(uintptr_t)(mak_rt_call_ctor("NBad:_f", (const uint64_t[]){}, (const uint8_t[]){}, 0)));
                                                                if (!mak_ir_adt_copy_checked((void *)(arena_case_266), sizeof(struct mak__f), UINT32_C(1225484184), (const void *)(rt_heap_obj_300)) ) {
                                                                    *(arena_case_266) = *(&mak_ir_default_obj__f);
                                                                }
                                                                mak_ir_heap_free((void *)rt_heap_obj_300);
                                                            }
                                                        }
                                                    }
                                                    else if (1) {
                                                        struct mak__f *rt_heap_obj_301 = (struct mak__f *)(uintptr_t)((struct mak__f *)(uintptr_t)(mak_rt_call_ctor("NBad:_f", (const uint64_t[]){}, (const uint8_t[]){}, 0)));
                                                        if (!mak_ir_adt_copy_checked((void *)(arena_case_266), sizeof(struct mak__f), UINT32_C(1225484184), (const void *)(rt_heap_obj_301)) ) {
                                                            *(arena_case_266) = *(&mak_ir_default_obj__f);
                                                        }
                                                        mak_ir_heap_free((void *)rt_heap_obj_301);
                                                    }
                                                }
                                                else if (1) {
                                                    struct mak__f *rt_heap_obj_302 = (struct mak__f *)(uintptr_t)((struct mak__f *)(uintptr_t)(mak_rt_call_ctor("NBad:_f", (const uint64_t[]){}, (const uint8_t[]){}, 0)));
                                                    if (!mak_ir_adt_copy_checked((void *)(arena_case_266), sizeof(struct mak__f), UINT32_C(1225484184), (const void *)(rt_heap_obj_302)) ) {
                                                        *(arena_case_266) = *(&mak_ir_default_obj__f);
                                                    }
                                                    mak_ir_heap_free((void *)rt_heap_obj_302);
                                                }
                                            }
                                        }
                                    }
                                    else {
                                        *(arena_case_266) = *(&mak_ir_default_obj__f);
                                    }
                                struct mak__f * scrut_265 = arena_case_266;
                                if ((scrut_265 != NULL && (scrut_265)->tag == mak_ir_tag_11)) {
                                    *(arena_case_233) = mak_ir_ctor_obj_11;
                                }
                                else if ((scrut_265 != NULL && (scrut_265)->tag == mak_ir_tag_10)) {
                                    double mak_local_ga_78 = (scrut_265)->v.v10.f0;
                                    struct mak__a * mak_local_gb_79 = (scrut_265)->v.v10.f1;
                                    (arena_case_233)->tag = mak_ir_tag_10;
                                    (arena_case_233)->v.v10.f0 = mak_local_ga_78;
                                    (arena_case_233)->v.v10.f1 = mak_local_gb_79;
                                }
                                else {
                                    *(arena_case_233) = *(&mak_ir_default_obj__f);
                                }
                            }
                            else if (1) {
                                int32_t scrut_303 = mak_local_fs_28 <= (int32_t)(INT64_C(122));
                                if ((scrut_303 == INT64_C(1))) {
                                    struct mak__f *arena_case_305 = (struct mak__f *)__builtin_alloca(sizeof(struct mak__f));
                                        struct mak__a *arena_call_307 = (struct mak__a *)__builtin_alloca(sizeof(struct mak__a));
                                            struct mak__a *arena_obj_308 = (struct mak__a *)__builtin_alloca(sizeof(struct mak__a));
                                                (arena_obj_308)->tag = mak_ir_tag_1;
                                                (arena_obj_308)->v.v1.f0 = mak_local_fs_28;
                                                (arena_obj_308)->v.v1.f1 = mak_local_ft_29;
                                            mak_ir_fn_2(arena_obj_308, (struct mak__a *)(arena_call_307));
                                        struct mak__a * scrut_306 = arena_call_307;
                                        if ((scrut_306 != NULL && (scrut_306)->tag == mak_ir_tag_1)) {
                                            int32_t mak_local_ce_80 = (scrut_306)->v.v1.f0;
                                            struct mak__a * mak_local_cf_81 = (scrut_306)->v.v1.f1;
                                            int32_t scrut_309 = mak_local_ce_80 == (int32_t)(INT64_C(95));
                                            if ((scrut_309 == INT64_C(1))) {
                                                struct mak__c *arena_call_310 = (struct mak__c *)__builtin_alloca(sizeof(struct mak__c));
                                                    struct mak__a *arena_obj_311 = (struct mak__a *)__builtin_alloca(sizeof(struct mak__a));
                                                        (arena_obj_311)->tag = mak_ir_tag_1;
                                                        (arena_obj_311)->v.v1.f0 = mak_local_ce_80;
                                                        (arena_obj_311)->v.v1.f1 = mak_local_cf_81;
                                                    mak_ir_fn_3(arena_obj_311, INT64_C(0), (struct mak__c *)(arena_call_310));
                                                if (!mak_ir_adt_copy_checked((void *)(arena_case_305), sizeof(struct mak__f), UINT32_C(1225484184), (const void *)(uintptr_t)(arena_call_310))) {
                                                    *(arena_case_305) = *(&mak_ir_default_obj__f);
                                                }
                                            }
                                            else if (1) {
                                                int32_t scrut_312 = mak_local_ce_80 >= (int32_t)(INT64_C(65));
                                                if ((scrut_312 == INT64_C(0))) {
                                                    int32_t scrut_313 = mak_local_ce_80 >= (int32_t)(INT64_C(97));
                                                    if ((scrut_313 == INT64_C(0))) {
                                                        struct mak__f *rt_heap_obj_314 = (struct mak__f *)(uintptr_t)((struct mak__f *)(uintptr_t)(mak_rt_call_ctor("NBad:_f", (const uint64_t[]){}, (const uint8_t[]){}, 0)));
                                                        if (!mak_ir_adt_copy_checked((void *)(arena_case_305), sizeof(struct mak__f), UINT32_C(1225484184), (const void *)(rt_heap_obj_314)) ) {
                                                            *(arena_case_305) = *(&mak_ir_default_obj__f);
                                                        }
                                                        mak_ir_heap_free((void *)rt_heap_obj_314);
                                                    }
                                                    else if (1) {
                                                        int32_t scrut_315 = mak_local_ce_80 <= (int32_t)(INT64_C(122));
                                                        if ((scrut_315 == INT64_C(1))) {
                                                            struct mak__c *arena_call_316 = (struct mak__c *)__builtin_alloca(sizeof(struct mak__c));
                                                                struct mak__a *arena_obj_317 = (struct mak__a *)__builtin_alloca(sizeof(struct mak__a));
                                                                    (arena_obj_317)->tag = mak_ir_tag_1;
                                                                    (arena_obj_317)->v.v1.f0 = mak_local_ce_80;
                                                                    (arena_obj_317)->v.v1.f1 = mak_local_cf_81;
                                                                mak_ir_fn_3(arena_obj_317, INT64_C(0), (struct mak__c *)(arena_call_316));
                                                            if (!mak_ir_adt_copy_checked((void *)(arena_case_305), sizeof(struct mak__f), UINT32_C(1225484184), (const void *)(uintptr_t)(arena_call_316))) {
                                                                *(arena_case_305) = *(&mak_ir_default_obj__f);
                                                            }
                                                        }
                                                        else if (1) {
                                                            struct mak__f *rt_heap_obj_318 = (struct mak__f *)(uintptr_t)((struct mak__f *)(uintptr_t)(mak_rt_call_ctor("NBad:_f", (const uint64_t[]){}, (const uint8_t[]){}, 0)));
                                                            if (!mak_ir_adt_copy_checked((void *)(arena_case_305), sizeof(struct mak__f), UINT32_C(1225484184), (const void *)(rt_heap_obj_318)) ) {
                                                                *(arena_case_305) = *(&mak_ir_default_obj__f);
                                                            }
                                                            mak_ir_heap_free((void *)rt_heap_obj_318);
                                                        }
                                                    }
                                                }
                                                else if (1) {
                                                    int32_t scrut_319 = mak_local_ce_80 <= (int32_t)(INT64_C(90));
                                                    if ((scrut_319 == INT64_C(1))) {
                                                        struct mak__c *arena_call_320 = (struct mak__c *)__builtin_alloca(sizeof(struct mak__c));
                                                            struct mak__a *arena_obj_321 = (struct mak__a *)__builtin_alloca(sizeof(struct mak__a));
                                                                (arena_obj_321)->tag = mak_ir_tag_1;
                                                                (arena_obj_321)->v.v1.f0 = mak_local_ce_80;
                                                                (arena_obj_321)->v.v1.f1 = mak_local_cf_81;
                                                            mak_ir_fn_3(arena_obj_321, INT64_C(0), (struct mak__c *)(arena_call_320));
                                                        if (!mak_ir_adt_copy_checked((void *)(arena_case_305), sizeof(struct mak__f), UINT32_C(1225484184), (const void *)(uintptr_t)(arena_call_320))) {
                                                            *(arena_case_305) = *(&mak_ir_default_obj__f);
                                                        }
                                                    }
                                                    else if (1) {
                                                        struct mak__f *rt_heap_obj_322 = (struct mak__f *)(uintptr_t)((struct mak__f *)(uintptr_t)(mak_rt_call_ctor("NBad:_f", (const uint64_t[]){}, (const uint8_t[]){}, 0)));
                                                        if (!mak_ir_adt_copy_checked((void *)(arena_case_305), sizeof(struct mak__f), UINT32_C(1225484184), (const void *)(rt_heap_obj_322)) ) {
                                                            *(arena_case_305) = *(&mak_ir_default_obj__f);
                                                        }
                                                        mak_ir_heap_free((void *)rt_heap_obj_322);
                                                    }
                                                }
                                            }
                                        }
                                        else if ((scrut_306 != NULL && (scrut_306)->tag == mak_ir_tag_0)) {
                                            struct mak__f *rt_heap_obj_323 = (struct mak__f *)(uintptr_t)((struct mak__f *)(uintptr_t)(mak_rt_call_ctor("NBad:_f", (const uint64_t[]){}, (const uint8_t[]){}, 0)));
                                            if (!mak_ir_adt_copy_checked((void *)(arena_case_305), sizeof(struct mak__f), UINT32_C(1225484184), (const void *)(rt_heap_obj_323)) ) {
                                                *(arena_case_305) = *(&mak_ir_default_obj__f);
                                            }
                                            mak_ir_heap_free((void *)rt_heap_obj_323);
                                        }
                                        else {
                                            *(arena_case_305) = *(&mak_ir_default_obj__f);
                                        }
                                    struct mak__f * scrut_304 = arena_case_305;
                                    if ((scrut_304 != NULL && (scrut_304)->tag == mak_ir_tag_11)) {
                                        *(arena_case_233) = mak_ir_ctor_obj_11;
                                    }
                                    else if ((scrut_304 != NULL && (scrut_304)->tag == mak_ir_tag_10)) {
                                        double mak_local_fx_82 = (scrut_304)->v.v10.f0;
                                        struct mak__a * mak_local_fy_83 = (scrut_304)->v.v10.f1;
                                        struct mak__a *arena_call_325 = (struct mak__a *)__builtin_alloca(sizeof(struct mak__a));
                                            mak_ir_fn_2(mak_local_fy_83, (struct mak__a *)(arena_call_325));
                                        struct mak__a * scrut_324 = arena_call_325;
                                        if ((scrut_324 != NULL && (scrut_324)->tag == mak_ir_tag_1) && (scrut_324->v.v1.f0 == INT64_C(40))) {
                                            struct mak__a * mak_local_fz_84 = (scrut_324)->v.v1.f1;
                                            *(arena_case_233) = mak_ir_ctor_obj_11;
                                        }
                                        else if (1) {
                                            *(arena_case_233) = mak_ir_ctor_obj_11;
                                        }
                                    }
                                    else {
                                        *(arena_case_233) = *(&mak_ir_default_obj__f);
                                    }
                                }
                                else if (1) {
                                    struct mak__f *arena_case_327 = (struct mak__f *)__builtin_alloca(sizeof(struct mak__f));
                                        struct mak__a *arena_call_329 = (struct mak__a *)__builtin_alloca(sizeof(struct mak__a));
                                            mak_ir_fn_2(a0, (struct mak__a *)(arena_call_329));
                                        struct mak__a * scrut_328 = arena_call_329;
                                        if ((scrut_328 != NULL && (scrut_328)->tag == mak_ir_tag_0)) {
                                            struct mak__f *rt_heap_obj_330 = (struct mak__f *)(uintptr_t)((struct mak__f *)(uintptr_t)(mak_rt_call_ctor("NBad:_f", (const uint64_t[]){}, (const uint8_t[]){}, 0)));
                                            if (!mak_ir_adt_copy_checked((void *)(arena_case_327), sizeof(struct mak__f), UINT32_C(1225484184), (const void *)(rt_heap_obj_330)) ) {
                                                *(arena_case_327) = *(&mak_ir_default_obj__f);
                                            }
                                            mak_ir_heap_free((void *)rt_heap_obj_330);
                                        }
                                        else if ((scrut_328 != NULL && (scrut_328)->tag == mak_ir_tag_1)) {
                                            int32_t mak_local_dk_112 = (scrut_328)->v.v1.f0;
                                            struct mak__a * mak_local_dl_113 = (scrut_328)->v.v1.f1;
                                            int32_t scrut_331 = mak_local_dk_112 >= (int32_t)(INT64_C(48));
                                            if ((scrut_331 == INT64_C(0))) {
                                                if ((mak_local_dk_112 == INT64_C(46))) {
                                                    if ((mak_local_dl_113 != NULL && (mak_local_dl_113)->tag == mak_ir_tag_1)) {
                                                        int32_t mak_local_dw_114 = (mak_local_dl_113)->v.v1.f0;
                                                        int32_t scrut_332 = mak_local_dw_114 >= (int32_t)(INT64_C(48));
                                                        if ((scrut_332 == INT64_C(0))) {
                                                            struct mak__f *rt_heap_obj_333 = (struct mak__f *)(uintptr_t)((struct mak__f *)(uintptr_t)(mak_rt_call_ctor("NBad:_f", (const uint64_t[]){}, (const uint8_t[]){}, 0)));
                                                            if (!mak_ir_adt_copy_checked((void *)(arena_case_327), sizeof(struct mak__f), UINT32_C(1225484184), (const void *)(rt_heap_obj_333)) ) {
                                                                *(arena_case_327) = *(&mak_ir_default_obj__f);
                                                            }
                                                            mak_ir_heap_free((void *)rt_heap_obj_333);
                                                        }
                                                        else if (1) {
                                                            int32_t scrut_334 = mak_local_dw_114 <= (int32_t)(INT64_C(57));
                                                            if ((scrut_334 == INT64_C(1))) {
                                                                struct mak__d *arena_call_335 = (struct mak__d *)__builtin_alloca(sizeof(struct mak__d));
                                                                    mak_ir_fn_8(mak_local_dl_113, 0.00000000000000000, 0.10000000000000001, (struct mak__d *)(arena_call_335));
                                                                if (!mak_ir_adt_copy_checked((void *)(arena_case_327), sizeof(struct mak__f), UINT32_C(1225484184), (const void *)(uintptr_t)(arena_call_335))) {
                                                                    *(arena_case_327) = *(&mak_ir_default_obj__f);
                                                                }
                                                            }
                                                            else if (1) {
                                                                struct mak__f *rt_heap_obj_336 = (struct mak__f *)(uintptr_t)((struct mak__f *)(uintptr_t)(mak_rt_call_ctor("NBad:_f", (const uint64_t[]){}, (const uint8_t[]){}, 0)));
                                                                if (!mak_ir_adt_copy_checked((void *)(arena_case_327), sizeof(struct mak__f), UINT32_C(1225484184), (const void *)(rt_heap_obj_336)) ) {
                                                                    *(arena_case_327) = *(&mak_ir_default_obj__f);
                                                                }
                                                                mak_ir_heap_free((void *)rt_heap_obj_336);
                                                            }
                                                        }
                                                    }
                                                    else if (1) {
                                                        struct mak__f *rt_heap_obj_337 = (struct mak__f *)(uintptr_t)((struct mak__f *)(uintptr_t)(mak_rt_call_ctor("NBad:_f", (const uint64_t[]){}, (const uint8_t[]){}, 0)));
                                                        if (!mak_ir_adt_copy_checked((void *)(arena_case_327), sizeof(struct mak__f), UINT32_C(1225484184), (const void *)(rt_heap_obj_337)) ) {
                                                            *(arena_case_327) = *(&mak_ir_default_obj__f);
                                                        }
                                                        mak_ir_heap_free((void *)rt_heap_obj_337);
                                                    }
                                                }
                                                else if (1) {
                                                    struct mak__f *rt_heap_obj_338 = (struct mak__f *)(uintptr_t)((struct mak__f *)(uintptr_t)(mak_rt_call_ctor("NBad:_f", (const uint64_t[]){}, (const uint8_t[]){}, 0)));
                                                    if (!mak_ir_adt_copy_checked((void *)(arena_case_327), sizeof(struct mak__f), UINT32_C(1225484184), (const void *)(rt_heap_obj_338)) ) {
                                                        *(arena_case_327) = *(&mak_ir_default_obj__f);
                                                    }
                                                    mak_ir_heap_free((void *)rt_heap_obj_338);
                                                }
                                            }
                                            else if (1) {
                                                int32_t scrut_339 = mak_local_dk_112 <= (int32_t)(INT64_C(57));
                                                if ((scrut_339 == INT64_C(1))) {
                                                    struct mak__d *arena_call_341 = (struct mak__d *)__builtin_alloca(sizeof(struct mak__d));
                                                        struct mak__a *arena_obj_342 = (struct mak__a *)__builtin_alloca(sizeof(struct mak__a));
                                                            (arena_obj_342)->tag = mak_ir_tag_1;
                                                            (arena_obj_342)->v.v1.f0 = mak_local_dk_112;
                                                            (arena_obj_342)->v.v1.f1 = mak_local_dl_113;
                                                        mak_ir_fn_7(arena_obj_342, 0.00000000000000000, (struct mak__d *)(arena_call_341));
                                                    struct mak__d * scrut_340 = arena_call_341;
                                                    if ((scrut_340 != NULL && (scrut_340)->tag == mak_ir_tag_6)) {
                                                        struct mak__f *rt_heap_obj_343 = (struct mak__f *)(uintptr_t)((struct mak__f *)(uintptr_t)(mak_rt_call_ctor("NBad:_f", (const uint64_t[]){}, (const uint8_t[]){}, 0)));
                                                        if (!mak_ir_adt_copy_checked((void *)(arena_case_327), sizeof(struct mak__f), UINT32_C(1225484184), (const void *)(rt_heap_obj_343)) ) {
                                                            *(arena_case_327) = *(&mak_ir_default_obj__f);
                                                        }
                                                        mak_ir_heap_free((void *)rt_heap_obj_343);
                                                    }
                                                    else if ((scrut_340 != NULL && (scrut_340)->tag == mak_ir_tag_7)) {
                                                        double mak_local_dm_115 = (scrut_340)->v.v7.f0;
                                                        struct mak__a * mak_local_dn_116 = (scrut_340)->v.v7.f1;
                                                        if ((mak_local_dn_116 != NULL && (mak_local_dn_116)->tag == mak_ir_tag_1) && (mak_local_dn_116->v.v1.f0 == INT64_C(46))) {
                                                            struct mak__a * mak_local_dp_117 = (mak_local_dn_116)->v.v1.f1;
                                                            struct mak__d *arena_call_344 = (struct mak__d *)__builtin_alloca(sizeof(struct mak__d));
                                                                mak_ir_fn_8(mak_local_dp_117, mak_local_dm_115, 0.10000000000000001, (struct mak__d *)(arena_call_344));
                                                            if (!mak_ir_adt_copy_checked((void *)(arena_case_327), sizeof(struct mak__f), UINT32_C(1225484184), (const void *)(uintptr_t)(arena_call_344))) {
                                                                *(arena_case_327) = *(&mak_ir_default_obj__f);
                                                            }
                                                        }
                                                        else if ((mak_local_dn_116 != NULL && (mak_local_dn_116)->tag == mak_ir_tag_1)) {
                                                            int32_t mak_local_dq_118 = (mak_local_dn_116)->v.v1.f0;
                                                            struct mak__a * mak_local_dr_119 = (mak_local_dn_116)->v.v1.f1;
                                                            switch (mak_local_dq_118) {
                                                                case INT64_C(101): {
                                                                    struct mak__e *arena_call_346 = (struct mak__e *)__builtin_alloca(sizeof(struct mak__e));
                                                                        mak_ir_fn_6(mak_local_dr_119, INT64_C(1), (struct mak__e *)(arena_call_346));
                                                                    struct mak__e * scrut_345 = arena_call_346;
                                                                    if ((scrut_345 != NULL && (scrut_345)->tag == mak_ir_tag_8)) {
                                                                        struct mak__f *rt_heap_obj_347 = (struct mak__f *)(uintptr_t)((struct mak__f *)(uintptr_t)(mak_rt_call_ctor("NBad:_f", (const uint64_t[]){}, (const uint8_t[]){}, 0)));
                                                                        if (!mak_ir_adt_copy_checked((void *)(arena_case_327), sizeof(struct mak__f), UINT32_C(1225484184), (const void *)(rt_heap_obj_347)) ) {
                                                                            *(arena_case_327) = *(&mak_ir_default_obj__f);
                                                                        }
                                                                        mak_ir_heap_free((void *)rt_heap_obj_347);
                                                                    }
                                                                    else if ((scrut_345 != NULL && (scrut_345)->tag == mak_ir_tag_9)) {
                                                                        int64_t mak_local_ds_120 = (scrut_345)->v.v9.f0;
                                                                        struct mak__a * mak_local_dt_121 = (scrut_345)->v.v9.f1;
                                                                        struct mak__d *arena_obj_348 = (struct mak__d *)__builtin_alloca(sizeof(struct mak__d));
                                                                            (arena_obj_348)->tag = mak_ir_tag_7;
                                                                            double eval_349 = mak_ir_fn_4(mak_local_ds_120);
                                                                            (arena_obj_348)->v.v7.f0 = ((double)(mak_local_dm_115) * (double)(eval_349));
                                                                            (arena_obj_348)->v.v7.f1 = mak_local_dt_121;
                                                                        if (!mak_ir_adt_copy_checked((void *)(arena_case_327), sizeof(struct mak__f), UINT32_C(1225484184), (const void *)(uintptr_t)(arena_obj_348))) {
                                                                            *(arena_case_327) = *(&mak_ir_default_obj__f);
                                                                        }
                                                                    }
                                                                    else {
                                                                        *(arena_case_327) = *(&mak_ir_default_obj__f);
                                                                    }
                                                                    break;
                                                                }
                                                                case INT64_C(69): {
                                                                    struct mak__e *arena_call_351 = (struct mak__e *)__builtin_alloca(sizeof(struct mak__e));
                                                                        mak_ir_fn_6(mak_local_dr_119, INT64_C(1), (struct mak__e *)(arena_call_351));
                                                                    struct mak__e * scrut_350 = arena_call_351;
                                                                    if ((scrut_350 != NULL && (scrut_350)->tag == mak_ir_tag_8)) {
                                                                        struct mak__f *rt_heap_obj_352 = (struct mak__f *)(uintptr_t)((struct mak__f *)(uintptr_t)(mak_rt_call_ctor("NBad:_f", (const uint64_t[]){}, (const uint8_t[]){}, 0)));
                                                                        if (!mak_ir_adt_copy_checked((void *)(arena_case_327), sizeof(struct mak__f), UINT32_C(1225484184), (const void *)(rt_heap_obj_352)) ) {
                                                                            *(arena_case_327) = *(&mak_ir_default_obj__f);
                                                                        }
                                                                        mak_ir_heap_free((void *)rt_heap_obj_352);
                                                                    }
                                                                    else if ((scrut_350 != NULL && (scrut_350)->tag == mak_ir_tag_9)) {
                                                                        int64_t mak_local_du_122 = (scrut_350)->v.v9.f0;
                                                                        struct mak__a * mak_local_dv_123 = (scrut_350)->v.v9.f1;
                                                                        struct mak__d *arena_obj_353 = (struct mak__d *)__builtin_alloca(sizeof(struct mak__d));
                                                                            (arena_obj_353)->tag = mak_ir_tag_7;
                                                                            double eval_354 = mak_ir_fn_4(mak_local_du_122);
                                                                            (arena_obj_353)->v.v7.f0 = ((double)(mak_local_dm_115) * (double)(eval_354));
                                                                            (arena_obj_353)->v.v7.f1 = mak_local_dv_123;
                                                                        if (!mak_ir_adt_copy_checked((void *)(arena_case_327), sizeof(struct mak__f), UINT32_C(1225484184), (const void *)(uintptr_t)(arena_obj_353))) {
                                                                            *(arena_case_327) = *(&mak_ir_default_obj__f);
                                                                        }
                                                                    }
                                                                    else {
                                                                        *(arena_case_327) = *(&mak_ir_default_obj__f);
                                                                    }
                                                                    break;
                                                                }
                                                                default: {
                                                                    struct mak__d *arena_obj_355 = (struct mak__d *)__builtin_alloca(sizeof(struct mak__d));
                                                                        (arena_obj_355)->tag = mak_ir_tag_7;
                                                                        (arena_obj_355)->v.v7.f0 = mak_local_dm_115;
                                                                        (arena_obj_355)->v.v7.f1 = mak_local_dn_116;
                                                                    if (!mak_ir_adt_copy_checked((void *)(arena_case_327), sizeof(struct mak__f), UINT32_C(1225484184), (const void *)(uintptr_t)(arena_obj_355))) {
                                                                        *(arena_case_327) = *(&mak_ir_default_obj__f);
                                                                    }
                                                                    break;
                                                                }
                                                            }
                                                        }
                                                        else if (1) {
                                                            struct mak__d *arena_obj_356 = (struct mak__d *)__builtin_alloca(sizeof(struct mak__d));
                                                                (arena_obj_356)->tag = mak_ir_tag_7;
                                                                (arena_obj_356)->v.v7.f0 = mak_local_dm_115;
                                                                (arena_obj_356)->v.v7.f1 = mak_local_dn_116;
                                                            if (!mak_ir_adt_copy_checked((void *)(arena_case_327), sizeof(struct mak__f), UINT32_C(1225484184), (const void *)(uintptr_t)(arena_obj_356))) {
                                                                *(arena_case_327) = *(&mak_ir_default_obj__f);
                                                            }
                                                        }
                                                    }
                                                    else {
                                                        *(arena_case_327) = *(&mak_ir_default_obj__f);
                                                    }
                                                }
                                                else if (1) {
                                                    if ((mak_local_dk_112 == INT64_C(46))) {
                                                        if ((mak_local_dl_113 != NULL && (mak_local_dl_113)->tag == mak_ir_tag_1)) {
                                                            int32_t mak_local_dw_124 = (mak_local_dl_113)->v.v1.f0;
                                                            int32_t scrut_357 = mak_local_dw_124 >= (int32_t)(INT64_C(48));
                                                            if ((scrut_357 == INT64_C(0))) {
                                                                struct mak__f *rt_heap_obj_358 = (struct mak__f *)(uintptr_t)((struct mak__f *)(uintptr_t)(mak_rt_call_ctor("NBad:_f", (const uint64_t[]){}, (const uint8_t[]){}, 0)));
                                                                if (!mak_ir_adt_copy_checked((void *)(arena_case_327), sizeof(struct mak__f), UINT32_C(1225484184), (const void *)(rt_heap_obj_358)) ) {
                                                                    *(arena_case_327) = *(&mak_ir_default_obj__f);
                                                                }
                                                                mak_ir_heap_free((void *)rt_heap_obj_358);
                                                            }
                                                            else if (1) {
                                                                int32_t scrut_359 = mak_local_dw_124 <= (int32_t)(INT64_C(57));
                                                                if ((scrut_359 == INT64_C(1))) {
                                                                    struct mak__d *arena_call_360 = (struct mak__d *)__builtin_alloca(sizeof(struct mak__d));
                                                                        mak_ir_fn_8(mak_local_dl_113, 0.00000000000000000, 0.10000000000000001, (struct mak__d *)(arena_call_360));
                                                                    if (!mak_ir_adt_copy_checked((void *)(arena_case_327), sizeof(struct mak__f), UINT32_C(1225484184), (const void *)(uintptr_t)(arena_call_360))) {
                                                                        *(arena_case_327) = *(&mak_ir_default_obj__f);
                                                                    }
                                                                }
                                                                else if (1) {
                                                                    struct mak__f *rt_heap_obj_361 = (struct mak__f *)(uintptr_t)((struct mak__f *)(uintptr_t)(mak_rt_call_ctor("NBad:_f", (const uint64_t[]){}, (const uint8_t[]){}, 0)));
                                                                    if (!mak_ir_adt_copy_checked((void *)(arena_case_327), sizeof(struct mak__f), UINT32_C(1225484184), (const void *)(rt_heap_obj_361)) ) {
                                                                        *(arena_case_327) = *(&mak_ir_default_obj__f);
                                                                    }
                                                                    mak_ir_heap_free((void *)rt_heap_obj_361);
                                                                }
                                                            }
                                                        }
                                                        else if (1) {
                                                            struct mak__f *rt_heap_obj_362 = (struct mak__f *)(uintptr_t)((struct mak__f *)(uintptr_t)(mak_rt_call_ctor("NBad:_f", (const uint64_t[]){}, (const uint8_t[]){}, 0)));
                                                            if (!mak_ir_adt_copy_checked((void *)(arena_case_327), sizeof(struct mak__f), UINT32_C(1225484184), (const void *)(rt_heap_obj_362)) ) {
                                                                *(arena_case_327) = *(&mak_ir_default_obj__f);
                                                            }
                                                            mak_ir_heap_free((void *)rt_heap_obj_362);
                                                        }
                                                    }
                                                    else if (1) {
                                                        struct mak__f *rt_heap_obj_363 = (struct mak__f *)(uintptr_t)((struct mak__f *)(uintptr_t)(mak_rt_call_ctor("NBad:_f", (const uint64_t[]){}, (const uint8_t[]){}, 0)));
                                                        if (!mak_ir_adt_copy_checked((void *)(arena_case_327), sizeof(struct mak__f), UINT32_C(1225484184), (const void *)(rt_heap_obj_363)) ) {
                                                            *(arena_case_327) = *(&mak_ir_default_obj__f);
                                                        }
                                                        mak_ir_heap_free((void *)rt_heap_obj_363);
                                                    }
                                                }
                                            }
                                        }
                                        else {
                                            *(arena_case_327) = *(&mak_ir_default_obj__f);
                                        }
                                    struct mak__f * scrut_326 = arena_case_327;
                                    if ((scrut_326 != NULL && (scrut_326)->tag == mak_ir_tag_11)) {
                                        *(arena_case_233) = mak_ir_ctor_obj_11;
                                    }
                                    else if ((scrut_326 != NULL && (scrut_326)->tag == mak_ir_tag_10)) {
                                        double mak_local_ga_125 = (scrut_326)->v.v10.f0;
                                        struct mak__a * mak_local_gb_126 = (scrut_326)->v.v10.f1;
                                        (arena_case_233)->tag = mak_ir_tag_10;
                                        (arena_case_233)->v.v10.f0 = mak_local_ga_125;
                                        (arena_case_233)->v.v10.f1 = mak_local_gb_126;
                                    }
                                    else {
                                        *(arena_case_233) = *(&mak_ir_default_obj__f);
                                    }
                                }
                            }
                        }
                        else if (1) {
                            int32_t scrut_364 = mak_local_fs_28 <= (int32_t)(INT64_C(90));
                            if ((scrut_364 == INT64_C(1))) {
                                struct mak__f *arena_case_366 = (struct mak__f *)__builtin_alloca(sizeof(struct mak__f));
                                    struct mak__a *arena_call_368 = (struct mak__a *)__builtin_alloca(sizeof(struct mak__a));
                                        struct mak__a *arena_obj_369 = (struct mak__a *)__builtin_alloca(sizeof(struct mak__a));
                                            (arena_obj_369)->tag = mak_ir_tag_1;
                                            (arena_obj_369)->v.v1.f0 = mak_local_fs_28;
                                            (arena_obj_369)->v.v1.f1 = mak_local_ft_29;
                                        mak_ir_fn_2(arena_obj_369, (struct mak__a *)(arena_call_368));
                                    struct mak__a * scrut_367 = arena_call_368;
                                    if ((scrut_367 != NULL && (scrut_367)->tag == mak_ir_tag_1)) {
                                        int32_t mak_local_ce_127 = (scrut_367)->v.v1.f0;
                                        struct mak__a * mak_local_cf_128 = (scrut_367)->v.v1.f1;
                                        int32_t scrut_370 = mak_local_ce_127 == (int32_t)(INT64_C(95));
                                        if ((scrut_370 == INT64_C(1))) {
                                            struct mak__c *arena_call_371 = (struct mak__c *)__builtin_alloca(sizeof(struct mak__c));
                                                struct mak__a *arena_obj_372 = (struct mak__a *)__builtin_alloca(sizeof(struct mak__a));
                                                    (arena_obj_372)->tag = mak_ir_tag_1;
                                                    (arena_obj_372)->v.v1.f0 = mak_local_ce_127;
                                                    (arena_obj_372)->v.v1.f1 = mak_local_cf_128;
                                                mak_ir_fn_3(arena_obj_372, INT64_C(0), (struct mak__c *)(arena_call_371));
                                            if (!mak_ir_adt_copy_checked((void *)(arena_case_366), sizeof(struct mak__f), UINT32_C(1225484184), (const void *)(uintptr_t)(arena_call_371))) {
                                                *(arena_case_366) = *(&mak_ir_default_obj__f);
                                            }
                                        }
                                        else if (1) {
                                            int32_t scrut_373 = mak_local_ce_127 >= (int32_t)(INT64_C(65));
                                            if ((scrut_373 == INT64_C(0))) {
                                                int32_t scrut_374 = mak_local_ce_127 >= (int32_t)(INT64_C(97));
                                                if ((scrut_374 == INT64_C(0))) {
                                                    struct mak__f *rt_heap_obj_375 = (struct mak__f *)(uintptr_t)((struct mak__f *)(uintptr_t)(mak_rt_call_ctor("NBad:_f", (const uint64_t[]){}, (const uint8_t[]){}, 0)));
                                                    if (!mak_ir_adt_copy_checked((void *)(arena_case_366), sizeof(struct mak__f), UINT32_C(1225484184), (const void *)(rt_heap_obj_375)) ) {
                                                        *(arena_case_366) = *(&mak_ir_default_obj__f);
                                                    }
                                                    mak_ir_heap_free((void *)rt_heap_obj_375);
                                                }
                                                else if (1) {
                                                    int32_t scrut_376 = mak_local_ce_127 <= (int32_t)(INT64_C(122));
                                                    if ((scrut_376 == INT64_C(1))) {
                                                        struct mak__c *arena_call_377 = (struct mak__c *)__builtin_alloca(sizeof(struct mak__c));
                                                            struct mak__a *arena_obj_378 = (struct mak__a *)__builtin_alloca(sizeof(struct mak__a));
                                                                (arena_obj_378)->tag = mak_ir_tag_1;
                                                                (arena_obj_378)->v.v1.f0 = mak_local_ce_127;
                                                                (arena_obj_378)->v.v1.f1 = mak_local_cf_128;
                                                            mak_ir_fn_3(arena_obj_378, INT64_C(0), (struct mak__c *)(arena_call_377));
                                                        if (!mak_ir_adt_copy_checked((void *)(arena_case_366), sizeof(struct mak__f), UINT32_C(1225484184), (const void *)(uintptr_t)(arena_call_377))) {
                                                            *(arena_case_366) = *(&mak_ir_default_obj__f);
                                                        }
                                                    }
                                                    else if (1) {
                                                        struct mak__f *rt_heap_obj_379 = (struct mak__f *)(uintptr_t)((struct mak__f *)(uintptr_t)(mak_rt_call_ctor("NBad:_f", (const uint64_t[]){}, (const uint8_t[]){}, 0)));
                                                        if (!mak_ir_adt_copy_checked((void *)(arena_case_366), sizeof(struct mak__f), UINT32_C(1225484184), (const void *)(rt_heap_obj_379)) ) {
                                                            *(arena_case_366) = *(&mak_ir_default_obj__f);
                                                        }
                                                        mak_ir_heap_free((void *)rt_heap_obj_379);
                                                    }
                                                }
                                            }
                                            else if (1) {
                                                int32_t scrut_380 = mak_local_ce_127 <= (int32_t)(INT64_C(90));
                                                if ((scrut_380 == INT64_C(1))) {
                                                    struct mak__c *arena_call_381 = (struct mak__c *)__builtin_alloca(sizeof(struct mak__c));
                                                        struct mak__a *arena_obj_382 = (struct mak__a *)__builtin_alloca(sizeof(struct mak__a));
                                                            (arena_obj_382)->tag = mak_ir_tag_1;
                                                            (arena_obj_382)->v.v1.f0 = mak_local_ce_127;
                                                            (arena_obj_382)->v.v1.f1 = mak_local_cf_128;
                                                        mak_ir_fn_3(arena_obj_382, INT64_C(0), (struct mak__c *)(arena_call_381));
                                                    if (!mak_ir_adt_copy_checked((void *)(arena_case_366), sizeof(struct mak__f), UINT32_C(1225484184), (const void *)(uintptr_t)(arena_call_381))) {
                                                        *(arena_case_366) = *(&mak_ir_default_obj__f);
                                                    }
                                                }
                                                else if (1) {
                                                    struct mak__f *rt_heap_obj_383 = (struct mak__f *)(uintptr_t)((struct mak__f *)(uintptr_t)(mak_rt_call_ctor("NBad:_f", (const uint64_t[]){}, (const uint8_t[]){}, 0)));
                                                    if (!mak_ir_adt_copy_checked((void *)(arena_case_366), sizeof(struct mak__f), UINT32_C(1225484184), (const void *)(rt_heap_obj_383)) ) {
                                                        *(arena_case_366) = *(&mak_ir_default_obj__f);
                                                    }
                                                    mak_ir_heap_free((void *)rt_heap_obj_383);
                                                }
                                            }
                                        }
                                    }
                                    else if ((scrut_367 != NULL && (scrut_367)->tag == mak_ir_tag_0)) {
                                        struct mak__f *rt_heap_obj_384 = (struct mak__f *)(uintptr_t)((struct mak__f *)(uintptr_t)(mak_rt_call_ctor("NBad:_f", (const uint64_t[]){}, (const uint8_t[]){}, 0)));
                                        if (!mak_ir_adt_copy_checked((void *)(arena_case_366), sizeof(struct mak__f), UINT32_C(1225484184), (const void *)(rt_heap_obj_384)) ) {
                                            *(arena_case_366) = *(&mak_ir_default_obj__f);
                                        }
                                        mak_ir_heap_free((void *)rt_heap_obj_384);
                                    }
                                    else {
                                        *(arena_case_366) = *(&mak_ir_default_obj__f);
                                    }
                                struct mak__f * scrut_365 = arena_case_366;
                                if ((scrut_365 != NULL && (scrut_365)->tag == mak_ir_tag_11)) {
                                    *(arena_case_233) = mak_ir_ctor_obj_11;
                                }
                                else if ((scrut_365 != NULL && (scrut_365)->tag == mak_ir_tag_10)) {
                                    double mak_local_fx_129 = (scrut_365)->v.v10.f0;
                                    struct mak__a * mak_local_fy_130 = (scrut_365)->v.v10.f1;
                                    struct mak__a *arena_call_386 = (struct mak__a *)__builtin_alloca(sizeof(struct mak__a));
                                        mak_ir_fn_2(mak_local_fy_130, (struct mak__a *)(arena_call_386));
                                    struct mak__a * scrut_385 = arena_call_386;
                                    if ((scrut_385 != NULL && (scrut_385)->tag == mak_ir_tag_1) && (scrut_385->v.v1.f0 == INT64_C(40))) {
                                        struct mak__a * mak_local_fz_131 = (scrut_385)->v.v1.f1;
                                        *(arena_case_233) = mak_ir_ctor_obj_11;
                                    }
                                    else if (1) {
                                        *(arena_case_233) = mak_ir_ctor_obj_11;
                                    }
                                }
                                else {
                                    *(arena_case_233) = *(&mak_ir_default_obj__f);
                                }
                            }
                            else if (1) {
                                struct mak__f *arena_case_388 = (struct mak__f *)__builtin_alloca(sizeof(struct mak__f));
                                    struct mak__a *arena_call_390 = (struct mak__a *)__builtin_alloca(sizeof(struct mak__a));
                                        mak_ir_fn_2(a0, (struct mak__a *)(arena_call_390));
                                    struct mak__a * scrut_389 = arena_call_390;
                                    if ((scrut_389 != NULL && (scrut_389)->tag == mak_ir_tag_0)) {
                                        struct mak__f *rt_heap_obj_391 = (struct mak__f *)(uintptr_t)((struct mak__f *)(uintptr_t)(mak_rt_call_ctor("NBad:_f", (const uint64_t[]){}, (const uint8_t[]){}, 0)));
                                        if (!mak_ir_adt_copy_checked((void *)(arena_case_388), sizeof(struct mak__f), UINT32_C(1225484184), (const void *)(rt_heap_obj_391)) ) {
                                            *(arena_case_388) = *(&mak_ir_default_obj__f);
                                        }
                                        mak_ir_heap_free((void *)rt_heap_obj_391);
                                    }
                                    else if ((scrut_389 != NULL && (scrut_389)->tag == mak_ir_tag_1)) {
                                        int32_t mak_local_dk_159 = (scrut_389)->v.v1.f0;
                                        struct mak__a * mak_local_dl_160 = (scrut_389)->v.v1.f1;
                                        int32_t scrut_392 = mak_local_dk_159 >= (int32_t)(INT64_C(48));
                                        if ((scrut_392 == INT64_C(0))) {
                                            if ((mak_local_dk_159 == INT64_C(46))) {
                                                if ((mak_local_dl_160 != NULL && (mak_local_dl_160)->tag == mak_ir_tag_1)) {
                                                    int32_t mak_local_dw_161 = (mak_local_dl_160)->v.v1.f0;
                                                    int32_t scrut_393 = mak_local_dw_161 >= (int32_t)(INT64_C(48));
                                                    if ((scrut_393 == INT64_C(0))) {
                                                        struct mak__f *rt_heap_obj_394 = (struct mak__f *)(uintptr_t)((struct mak__f *)(uintptr_t)(mak_rt_call_ctor("NBad:_f", (const uint64_t[]){}, (const uint8_t[]){}, 0)));
                                                        if (!mak_ir_adt_copy_checked((void *)(arena_case_388), sizeof(struct mak__f), UINT32_C(1225484184), (const void *)(rt_heap_obj_394)) ) {
                                                            *(arena_case_388) = *(&mak_ir_default_obj__f);
                                                        }
                                                        mak_ir_heap_free((void *)rt_heap_obj_394);
                                                    }
                                                    else if (1) {
                                                        int32_t scrut_395 = mak_local_dw_161 <= (int32_t)(INT64_C(57));
                                                        if ((scrut_395 == INT64_C(1))) {
                                                            struct mak__d *arena_call_396 = (struct mak__d *)__builtin_alloca(sizeof(struct mak__d));
                                                                mak_ir_fn_8(mak_local_dl_160, 0.00000000000000000, 0.10000000000000001, (struct mak__d *)(arena_call_396));
                                                            if (!mak_ir_adt_copy_checked((void *)(arena_case_388), sizeof(struct mak__f), UINT32_C(1225484184), (const void *)(uintptr_t)(arena_call_396))) {
                                                                *(arena_case_388) = *(&mak_ir_default_obj__f);
                                                            }
                                                        }
                                                        else if (1) {
                                                            struct mak__f *rt_heap_obj_397 = (struct mak__f *)(uintptr_t)((struct mak__f *)(uintptr_t)(mak_rt_call_ctor("NBad:_f", (const uint64_t[]){}, (const uint8_t[]){}, 0)));
                                                            if (!mak_ir_adt_copy_checked((void *)(arena_case_388), sizeof(struct mak__f), UINT32_C(1225484184), (const void *)(rt_heap_obj_397)) ) {
                                                                *(arena_case_388) = *(&mak_ir_default_obj__f);
                                                            }
                                                            mak_ir_heap_free((void *)rt_heap_obj_397);
                                                        }
                                                    }
                                                }
                                                else if (1) {
                                                    struct mak__f *rt_heap_obj_398 = (struct mak__f *)(uintptr_t)((struct mak__f *)(uintptr_t)(mak_rt_call_ctor("NBad:_f", (const uint64_t[]){}, (const uint8_t[]){}, 0)));
                                                    if (!mak_ir_adt_copy_checked((void *)(arena_case_388), sizeof(struct mak__f), UINT32_C(1225484184), (const void *)(rt_heap_obj_398)) ) {
                                                        *(arena_case_388) = *(&mak_ir_default_obj__f);
                                                    }
                                                    mak_ir_heap_free((void *)rt_heap_obj_398);
                                                }
                                            }
                                            else if (1) {
                                                struct mak__f *rt_heap_obj_399 = (struct mak__f *)(uintptr_t)((struct mak__f *)(uintptr_t)(mak_rt_call_ctor("NBad:_f", (const uint64_t[]){}, (const uint8_t[]){}, 0)));
                                                if (!mak_ir_adt_copy_checked((void *)(arena_case_388), sizeof(struct mak__f), UINT32_C(1225484184), (const void *)(rt_heap_obj_399)) ) {
                                                    *(arena_case_388) = *(&mak_ir_default_obj__f);
                                                }
                                                mak_ir_heap_free((void *)rt_heap_obj_399);
                                            }
                                        }
                                        else if (1) {
                                            int32_t scrut_400 = mak_local_dk_159 <= (int32_t)(INT64_C(57));
                                            if ((scrut_400 == INT64_C(1))) {
                                                struct mak__d *arena_call_402 = (struct mak__d *)__builtin_alloca(sizeof(struct mak__d));
                                                    struct mak__a *arena_obj_403 = (struct mak__a *)__builtin_alloca(sizeof(struct mak__a));
                                                        (arena_obj_403)->tag = mak_ir_tag_1;
                                                        (arena_obj_403)->v.v1.f0 = mak_local_dk_159;
                                                        (arena_obj_403)->v.v1.f1 = mak_local_dl_160;
                                                    mak_ir_fn_7(arena_obj_403, 0.00000000000000000, (struct mak__d *)(arena_call_402));
                                                struct mak__d * scrut_401 = arena_call_402;
                                                if ((scrut_401 != NULL && (scrut_401)->tag == mak_ir_tag_6)) {
                                                    struct mak__f *rt_heap_obj_404 = (struct mak__f *)(uintptr_t)((struct mak__f *)(uintptr_t)(mak_rt_call_ctor("NBad:_f", (const uint64_t[]){}, (const uint8_t[]){}, 0)));
                                                    if (!mak_ir_adt_copy_checked((void *)(arena_case_388), sizeof(struct mak__f), UINT32_C(1225484184), (const void *)(rt_heap_obj_404)) ) {
                                                        *(arena_case_388) = *(&mak_ir_default_obj__f);
                                                    }
                                                    mak_ir_heap_free((void *)rt_heap_obj_404);
                                                }
                                                else if ((scrut_401 != NULL && (scrut_401)->tag == mak_ir_tag_7)) {
                                                    double mak_local_dm_162 = (scrut_401)->v.v7.f0;
                                                    struct mak__a * mak_local_dn_163 = (scrut_401)->v.v7.f1;
                                                    if ((mak_local_dn_163 != NULL && (mak_local_dn_163)->tag == mak_ir_tag_1) && (mak_local_dn_163->v.v1.f0 == INT64_C(46))) {
                                                        struct mak__a * mak_local_dp_164 = (mak_local_dn_163)->v.v1.f1;
                                                        struct mak__d *arena_call_405 = (struct mak__d *)__builtin_alloca(sizeof(struct mak__d));
                                                            mak_ir_fn_8(mak_local_dp_164, mak_local_dm_162, 0.10000000000000001, (struct mak__d *)(arena_call_405));
                                                        if (!mak_ir_adt_copy_checked((void *)(arena_case_388), sizeof(struct mak__f), UINT32_C(1225484184), (const void *)(uintptr_t)(arena_call_405))) {
                                                            *(arena_case_388) = *(&mak_ir_default_obj__f);
                                                        }
                                                    }
                                                    else if ((mak_local_dn_163 != NULL && (mak_local_dn_163)->tag == mak_ir_tag_1)) {
                                                        int32_t mak_local_dq_165 = (mak_local_dn_163)->v.v1.f0;
                                                        struct mak__a * mak_local_dr_166 = (mak_local_dn_163)->v.v1.f1;
                                                        switch (mak_local_dq_165) {
                                                            case INT64_C(101): {
                                                                struct mak__e *arena_call_407 = (struct mak__e *)__builtin_alloca(sizeof(struct mak__e));
                                                                    mak_ir_fn_6(mak_local_dr_166, INT64_C(1), (struct mak__e *)(arena_call_407));
                                                                struct mak__e * scrut_406 = arena_call_407;
                                                                if ((scrut_406 != NULL && (scrut_406)->tag == mak_ir_tag_8)) {
                                                                    struct mak__f *rt_heap_obj_408 = (struct mak__f *)(uintptr_t)((struct mak__f *)(uintptr_t)(mak_rt_call_ctor("NBad:_f", (const uint64_t[]){}, (const uint8_t[]){}, 0)));
                                                                    if (!mak_ir_adt_copy_checked((void *)(arena_case_388), sizeof(struct mak__f), UINT32_C(1225484184), (const void *)(rt_heap_obj_408)) ) {
                                                                        *(arena_case_388) = *(&mak_ir_default_obj__f);
                                                                    }
                                                                    mak_ir_heap_free((void *)rt_heap_obj_408);
                                                                }
                                                                else if ((scrut_406 != NULL && (scrut_406)->tag == mak_ir_tag_9)) {
                                                                    int64_t mak_local_ds_167 = (scrut_406)->v.v9.f0;
                                                                    struct mak__a * mak_local_dt_168 = (scrut_406)->v.v9.f1;
                                                                    struct mak__d *arena_obj_409 = (struct mak__d *)__builtin_alloca(sizeof(struct mak__d));
                                                                        (arena_obj_409)->tag = mak_ir_tag_7;
                                                                        double eval_410 = mak_ir_fn_4(mak_local_ds_167);
                                                                        (arena_obj_409)->v.v7.f0 = ((double)(mak_local_dm_162) * (double)(eval_410));
                                                                        (arena_obj_409)->v.v7.f1 = mak_local_dt_168;
                                                                    if (!mak_ir_adt_copy_checked((void *)(arena_case_388), sizeof(struct mak__f), UINT32_C(1225484184), (const void *)(uintptr_t)(arena_obj_409))) {
                                                                        *(arena_case_388) = *(&mak_ir_default_obj__f);
                                                                    }
                                                                }
                                                                else {
                                                                    *(arena_case_388) = *(&mak_ir_default_obj__f);
                                                                }
                                                                break;
                                                            }
                                                            case INT64_C(69): {
                                                                struct mak__e *arena_call_412 = (struct mak__e *)__builtin_alloca(sizeof(struct mak__e));
                                                                    mak_ir_fn_6(mak_local_dr_166, INT64_C(1), (struct mak__e *)(arena_call_412));
                                                                struct mak__e * scrut_411 = arena_call_412;
                                                                if ((scrut_411 != NULL && (scrut_411)->tag == mak_ir_tag_8)) {
                                                                    struct mak__f *rt_heap_obj_413 = (struct mak__f *)(uintptr_t)((struct mak__f *)(uintptr_t)(mak_rt_call_ctor("NBad:_f", (const uint64_t[]){}, (const uint8_t[]){}, 0)));
                                                                    if (!mak_ir_adt_copy_checked((void *)(arena_case_388), sizeof(struct mak__f), UINT32_C(1225484184), (const void *)(rt_heap_obj_413)) ) {
                                                                        *(arena_case_388) = *(&mak_ir_default_obj__f);
                                                                    }
                                                                    mak_ir_heap_free((void *)rt_heap_obj_413);
                                                                }
                                                                else if ((scrut_411 != NULL && (scrut_411)->tag == mak_ir_tag_9)) {
                                                                    int64_t mak_local_du_169 = (scrut_411)->v.v9.f0;
                                                                    struct mak__a * mak_local_dv_170 = (scrut_411)->v.v9.f1;
                                                                    struct mak__d *arena_obj_414 = (struct mak__d *)__builtin_alloca(sizeof(struct mak__d));
                                                                        (arena_obj_414)->tag = mak_ir_tag_7;
                                                                        double eval_415 = mak_ir_fn_4(mak_local_du_169);
                                                                        (arena_obj_414)->v.v7.f0 = ((double)(mak_local_dm_162) * (double)(eval_415));
                                                                        (arena_obj_414)->v.v7.f1 = mak_local_dv_170;
                                                                    if (!mak_ir_adt_copy_checked((void *)(arena_case_388), sizeof(struct mak__f), UINT32_C(1225484184), (const void *)(uintptr_t)(arena_obj_414))) {
                                                                        *(arena_case_388) = *(&mak_ir_default_obj__f);
                                                                    }
                                                                }
                                                                else {
                                                                    *(arena_case_388) = *(&mak_ir_default_obj__f);
                                                                }
                                                                break;
                                                            }
                                                            default: {
                                                                struct mak__d *arena_obj_416 = (struct mak__d *)__builtin_alloca(sizeof(struct mak__d));
                                                                    (arena_obj_416)->tag = mak_ir_tag_7;
                                                                    (arena_obj_416)->v.v7.f0 = mak_local_dm_162;
                                                                    (arena_obj_416)->v.v7.f1 = mak_local_dn_163;
                                                                if (!mak_ir_adt_copy_checked((void *)(arena_case_388), sizeof(struct mak__f), UINT32_C(1225484184), (const void *)(uintptr_t)(arena_obj_416))) {
                                                                    *(arena_case_388) = *(&mak_ir_default_obj__f);
                                                                }
                                                                break;
                                                            }
                                                        }
                                                    }
                                                    else if (1) {
                                                        struct mak__d *arena_obj_417 = (struct mak__d *)__builtin_alloca(sizeof(struct mak__d));
                                                            (arena_obj_417)->tag = mak_ir_tag_7;
                                                            (arena_obj_417)->v.v7.f0 = mak_local_dm_162;
                                                            (arena_obj_417)->v.v7.f1 = mak_local_dn_163;
                                                        if (!mak_ir_adt_copy_checked((void *)(arena_case_388), sizeof(struct mak__f), UINT32_C(1225484184), (const void *)(uintptr_t)(arena_obj_417))) {
                                                            *(arena_case_388) = *(&mak_ir_default_obj__f);
                                                        }
                                                    }
                                                }
                                                else {
                                                    *(arena_case_388) = *(&mak_ir_default_obj__f);
                                                }
                                            }
                                            else if (1) {
                                                if ((mak_local_dk_159 == INT64_C(46))) {
                                                    if ((mak_local_dl_160 != NULL && (mak_local_dl_160)->tag == mak_ir_tag_1)) {
                                                        int32_t mak_local_dw_171 = (mak_local_dl_160)->v.v1.f0;
                                                        int32_t scrut_418 = mak_local_dw_171 >= (int32_t)(INT64_C(48));
                                                        if ((scrut_418 == INT64_C(0))) {
                                                            struct mak__f *rt_heap_obj_419 = (struct mak__f *)(uintptr_t)((struct mak__f *)(uintptr_t)(mak_rt_call_ctor("NBad:_f", (const uint64_t[]){}, (const uint8_t[]){}, 0)));
                                                            if (!mak_ir_adt_copy_checked((void *)(arena_case_388), sizeof(struct mak__f), UINT32_C(1225484184), (const void *)(rt_heap_obj_419)) ) {
                                                                *(arena_case_388) = *(&mak_ir_default_obj__f);
                                                            }
                                                            mak_ir_heap_free((void *)rt_heap_obj_419);
                                                        }
                                                        else if (1) {
                                                            int32_t scrut_420 = mak_local_dw_171 <= (int32_t)(INT64_C(57));
                                                            if ((scrut_420 == INT64_C(1))) {
                                                                struct mak__d *arena_call_421 = (struct mak__d *)__builtin_alloca(sizeof(struct mak__d));
                                                                    mak_ir_fn_8(mak_local_dl_160, 0.00000000000000000, 0.10000000000000001, (struct mak__d *)(arena_call_421));
                                                                if (!mak_ir_adt_copy_checked((void *)(arena_case_388), sizeof(struct mak__f), UINT32_C(1225484184), (const void *)(uintptr_t)(arena_call_421))) {
                                                                    *(arena_case_388) = *(&mak_ir_default_obj__f);
                                                                }
                                                            }
                                                            else if (1) {
                                                                struct mak__f *rt_heap_obj_422 = (struct mak__f *)(uintptr_t)((struct mak__f *)(uintptr_t)(mak_rt_call_ctor("NBad:_f", (const uint64_t[]){}, (const uint8_t[]){}, 0)));
                                                                if (!mak_ir_adt_copy_checked((void *)(arena_case_388), sizeof(struct mak__f), UINT32_C(1225484184), (const void *)(rt_heap_obj_422)) ) {
                                                                    *(arena_case_388) = *(&mak_ir_default_obj__f);
                                                                }
                                                                mak_ir_heap_free((void *)rt_heap_obj_422);
                                                            }
                                                        }
                                                    }
                                                    else if (1) {
                                                        struct mak__f *rt_heap_obj_423 = (struct mak__f *)(uintptr_t)((struct mak__f *)(uintptr_t)(mak_rt_call_ctor("NBad:_f", (const uint64_t[]){}, (const uint8_t[]){}, 0)));
                                                        if (!mak_ir_adt_copy_checked((void *)(arena_case_388), sizeof(struct mak__f), UINT32_C(1225484184), (const void *)(rt_heap_obj_423)) ) {
                                                            *(arena_case_388) = *(&mak_ir_default_obj__f);
                                                        }
                                                        mak_ir_heap_free((void *)rt_heap_obj_423);
                                                    }
                                                }
                                                else if (1) {
                                                    struct mak__f *rt_heap_obj_424 = (struct mak__f *)(uintptr_t)((struct mak__f *)(uintptr_t)(mak_rt_call_ctor("NBad:_f", (const uint64_t[]){}, (const uint8_t[]){}, 0)));
                                                    if (!mak_ir_adt_copy_checked((void *)(arena_case_388), sizeof(struct mak__f), UINT32_C(1225484184), (const void *)(rt_heap_obj_424)) ) {
                                                        *(arena_case_388) = *(&mak_ir_default_obj__f);
                                                    }
                                                    mak_ir_heap_free((void *)rt_heap_obj_424);
                                                }
                                            }
                                        }
                                    }
                                    else {
                                        *(arena_case_388) = *(&mak_ir_default_obj__f);
                                    }
                                struct mak__f * scrut_387 = arena_case_388;
                                if ((scrut_387 != NULL && (scrut_387)->tag == mak_ir_tag_11)) {
                                    *(arena_case_233) = mak_ir_ctor_obj_11;
                                }
                                else if ((scrut_387 != NULL && (scrut_387)->tag == mak_ir_tag_10)) {
                                    double mak_local_ga_172 = (scrut_387)->v.v10.f0;
                                    struct mak__a * mak_local_gb_173 = (scrut_387)->v.v10.f1;
                                    (arena_case_233)->tag = mak_ir_tag_10;
                                    (arena_case_233)->v.v10.f0 = mak_local_ga_172;
                                    (arena_case_233)->v.v10.f1 = mak_local_gb_173;
                                }
                                else {
                                    *(arena_case_233) = *(&mak_ir_default_obj__f);
                                }
                            }
                        }
                    }
                }
            }
            else {
                *(arena_case_233) = *(&mak_ir_default_obj__f);
            }
        struct mak__f * scrut_232 = arena_case_233;
        if ((scrut_232 != NULL && (scrut_232)->tag == mak_ir_tag_11)) {
            *(arena_case_231) = mak_ir_ctor_obj_11;
        }
        else if ((scrut_232 != NULL && (scrut_232)->tag == mak_ir_tag_10)) {
            double mak_local_gi_174 = (scrut_232)->v.v10.f0;
            struct mak__a * mak_local_gj_175 = (scrut_232)->v.v10.f1;
            mak_ir_fn_10(mak_local_gi_174, mak_local_gj_175, (struct mak__f *)(arena_case_231));
        }
        else {
            *(arena_case_231) = *(&mak_ir_default_obj__f);
        }
    struct mak__f * scrut_230 = arena_case_231;
    if ((scrut_230 != NULL && (scrut_230)->tag == mak_ir_tag_11)) {
        *(out) = mak_ir_ctor_obj_11;
    }
    else if ((scrut_230 != NULL && (scrut_230)->tag == mak_ir_tag_10)) {
        double mak_local_gl_176 = (scrut_230)->v.v10.f0;
        struct mak__a * mak_local_gm_177 = (scrut_230)->v.v10.f1;
        struct mak__a *arena_call_426 = (struct mak__a *)__builtin_alloca(sizeof(struct mak__a));
            mak_ir_fn_2(mak_local_gm_177, (struct mak__a *)(arena_call_426));
        struct mak__a * scrut_425 = arena_call_426;
        if ((scrut_425 != NULL && (scrut_425)->tag == mak_ir_tag_1) && (scrut_425->v.v1.f0 == INT64_C(94))) {
            struct mak__a * mak_local_gn_178 = (scrut_425)->v.v1.f1;
            struct mak__f *arena_call_428 = (struct mak__f *)__builtin_alloca(sizeof(struct mak__f));
                mak_ir_fn_12(mak_local_gn_178, (struct mak__f *)(arena_call_428));
            struct mak__f * scrut_427 = arena_call_428;
            if ((scrut_427 != NULL && (scrut_427)->tag == mak_ir_tag_11)) {
                *(out) = mak_ir_ctor_obj_11;
            }
            else if ((scrut_427 != NULL && (scrut_427)->tag == mak_ir_tag_10)) {
                double mak_local_go_179 = (scrut_427)->v.v10.f0;
                struct mak__a * mak_local_gp_180 = (scrut_427)->v.v10.f1;
                (out)->tag = mak_ir_tag_10;
                (out)->v.v10.f0 = mak_ir_ext_14(mak_local_gl_176, mak_local_go_179);
                (out)->v.v10.f1 = mak_local_gp_180;
            }
            else {
                *(out) = *(&mak_ir_default_obj__f);
            }
        }
        else if (1) {
            (out)->tag = mak_ir_tag_10;
            (out)->v.v10.f0 = mak_local_gl_176;
            (out)->v.v10.f1 = mak_local_gm_177;
        }
    }
    else {
        *(out) = *(&mak_ir_default_obj__f);
    }
    return;
}

static inline void mak_ir_fn_12(struct mak__a * a0, struct mak__f *out) {
    for (;;) {
        struct mak__a *arena_call_430 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
        if (!arena_call_430) {
            mak_ir_recover("ADT result allocation failed");
            arena_call_430 = &mak_ir_default_obj__a;
         } else {
            mak_ir_fn_2(a0, (struct mak__a *)(arena_call_430));
        }
        struct mak__a * scrut_429 = arena_call_430;
        if ((scrut_429 != NULL && (scrut_429)->tag == mak_ir_tag_0)) {
            *(out) = mak_ir_ctor_obj_11;
        }
        else if ((scrut_429 != NULL && (scrut_429)->tag == mak_ir_tag_1)) {
            int32_t mak_local_gr_181 = (scrut_429)->v.v1.f0;
            struct mak__a * mak_local_gs_182 = (scrut_429)->v.v1.f1;
            switch (mak_local_gr_181) {
                case INT64_C(43): {
                    struct mak__a * tailarg_431 = mak_local_gs_182;
                    a0 = tailarg_431;
                    continue;
                }
                case INT64_C(45): {
                    struct mak__f *arena_call_433 = (struct mak__f *)mak_ir_arena_alloc(sizeof(struct mak__f), _Alignof(struct mak__f));
                    if (!arena_call_433) {
                        mak_ir_recover("ADT result allocation failed");
                        arena_call_433 = &mak_ir_default_obj__f;
                     } else {
                        mak_ir_fn_12(mak_local_gs_182, (struct mak__f *)(arena_call_433));
                    }
                    struct mak__f * scrut_432 = arena_call_433;
                    if ((scrut_432 != NULL && (scrut_432)->tag == mak_ir_tag_11)) {
                        *(out) = mak_ir_ctor_obj_11;
                    }
                    else if ((scrut_432 != NULL && (scrut_432)->tag == mak_ir_tag_10)) {
                        double mak_local_gt_183 = (scrut_432)->v.v10.f0;
                        struct mak__a * mak_local_gu_184 = (scrut_432)->v.v10.f1;
                        (out)->tag = mak_ir_tag_10;
                        (out)->v.v10.f0 = ((double)(0.00000000000000000) - (double)(mak_local_gt_183));
                        (out)->v.v10.f1 = mak_local_gu_184;
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
        struct mak__a *arena_call_435 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
        if (!arena_call_435) {
            mak_ir_recover("ADT result allocation failed");
            arena_call_435 = &mak_ir_default_obj__a;
         } else {
            mak_ir_fn_2(a1, (struct mak__a *)(arena_call_435));
        }
        struct mak__a * scrut_434 = arena_call_435;
        if ((scrut_434 != NULL && (scrut_434)->tag == mak_ir_tag_0)) {
            (out)->tag = mak_ir_tag_10;
            (out)->v.v10.f0 = a0;
            (out)->v.v10.f1 = &mak_ir_ctor_obj_0;
        }
        else if ((scrut_434 != NULL && (scrut_434)->tag == mak_ir_tag_1)) {
            int32_t mak_local_gx_185 = (scrut_434)->v.v1.f0;
            struct mak__a * mak_local_gy_186 = (scrut_434)->v.v1.f1;
            switch (mak_local_gx_185) {
                case INT64_C(42): {
                    struct mak__f *arena_call_437 = (struct mak__f *)mak_ir_arena_alloc(sizeof(struct mak__f), _Alignof(struct mak__f));
                    if (!arena_call_437) {
                        mak_ir_recover("ADT result allocation failed");
                        arena_call_437 = &mak_ir_default_obj__f;
                     } else {
                        mak_ir_fn_11(mak_local_gy_186, (struct mak__f *)(arena_call_437));
                    }
                    struct mak__f * scrut_436 = arena_call_437;
                    if ((scrut_436 != NULL && (scrut_436)->tag == mak_ir_tag_11)) {
                        *(out) = mak_ir_ctor_obj_11;
                    }
                    else if ((scrut_436 != NULL && (scrut_436)->tag == mak_ir_tag_10)) {
                        double mak_local_gz_187 = (scrut_436)->v.v10.f0;
                        struct mak__a * mak_local_ha_188 = (scrut_436)->v.v10.f1;
                        double tailarg_438 = ((double)(a0) * (double)(mak_local_gz_187));
                        struct mak__a * tailarg_439 = mak_local_ha_188;
                        a0 = tailarg_438;
                        a1 = tailarg_439;
                        continue;
                    }
                    else {
                        *(out) = *(&mak_ir_default_obj__f);
                    }
                    break;
                }
                case INT64_C(47): {
                    if ((mak_local_gy_186 != NULL && (mak_local_gy_186)->tag == mak_ir_tag_1) && (mak_local_gy_186->v.v1.f0 == INT64_C(47))) {
                        struct mak__a * mak_local_hb_189 = (mak_local_gy_186)->v.v1.f1;
                        struct mak__f *arena_call_441 = (struct mak__f *)mak_ir_arena_alloc(sizeof(struct mak__f), _Alignof(struct mak__f));
                        if (!arena_call_441) {
                            mak_ir_recover("ADT result allocation failed");
                            arena_call_441 = &mak_ir_default_obj__f;
                         } else {
                            mak_ir_fn_11(mak_local_hb_189, (struct mak__f *)(arena_call_441));
                        }
                        struct mak__f * scrut_440 = arena_call_441;
                        if ((scrut_440 != NULL && (scrut_440)->tag == mak_ir_tag_11)) {
                            *(out) = mak_ir_ctor_obj_11;
                        }
                        else if ((scrut_440 != NULL && (scrut_440)->tag == mak_ir_tag_10)) {
                            double mak_local_hc_190 = (scrut_440)->v.v10.f0;
                            struct mak__a * mak_local_hd_191 = (scrut_440)->v.v10.f1;
                            int32_t scrut_442 = (int32_t)(mak_local_hc_190) == (int32_t)(0.00000000000000000);
                            if ((scrut_442 == INT64_C(1))) {
                                *(out) = mak_ir_ctor_obj_11;
                            }
                            else if (1) {
                                double tailarg_443 = mak_ir_ext_8(((double)(a0) / (double)(mak_local_hc_190)));
                                struct mak__a * tailarg_444 = mak_local_hd_191;
                                a0 = tailarg_443;
                                a1 = tailarg_444;
                                continue;
                            }
                        }
                        else {
                            *(out) = *(&mak_ir_default_obj__f);
                        }
                    }
                    else if (1) {
                        struct mak__f *arena_call_446 = (struct mak__f *)mak_ir_arena_alloc(sizeof(struct mak__f), _Alignof(struct mak__f));
                        if (!arena_call_446) {
                            mak_ir_recover("ADT result allocation failed");
                            arena_call_446 = &mak_ir_default_obj__f;
                         } else {
                            mak_ir_fn_11(mak_local_gy_186, (struct mak__f *)(arena_call_446));
                        }
                        struct mak__f * scrut_445 = arena_call_446;
                        if ((scrut_445 != NULL && (scrut_445)->tag == mak_ir_tag_11)) {
                            *(out) = mak_ir_ctor_obj_11;
                        }
                        else if ((scrut_445 != NULL && (scrut_445)->tag == mak_ir_tag_10)) {
                            double mak_local_he_192 = (scrut_445)->v.v10.f0;
                            struct mak__a * mak_local_hf_193 = (scrut_445)->v.v10.f1;
                            int32_t scrut_447 = (int32_t)(mak_local_he_192) == (int32_t)(0.00000000000000000);
                            if ((scrut_447 == INT64_C(1))) {
                                *(out) = mak_ir_ctor_obj_11;
                            }
                            else if (1) {
                                double tailarg_448 = ((double)(a0) / (double)(mak_local_he_192));
                                struct mak__a * tailarg_449 = mak_local_hf_193;
                                a0 = tailarg_448;
                                a1 = tailarg_449;
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
                    struct mak__f *arena_call_451 = (struct mak__f *)mak_ir_arena_alloc(sizeof(struct mak__f), _Alignof(struct mak__f));
                    if (!arena_call_451) {
                        mak_ir_recover("ADT result allocation failed");
                        arena_call_451 = &mak_ir_default_obj__f;
                     } else {
                        mak_ir_fn_11(mak_local_gy_186, (struct mak__f *)(arena_call_451));
                    }
                    struct mak__f * scrut_450 = arena_call_451;
                    if ((scrut_450 != NULL && (scrut_450)->tag == mak_ir_tag_11)) {
                        *(out) = mak_ir_ctor_obj_11;
                    }
                    else if ((scrut_450 != NULL && (scrut_450)->tag == mak_ir_tag_10)) {
                        double mak_local_hg_194 = (scrut_450)->v.v10.f0;
                        struct mak__a * mak_local_hh_195 = (scrut_450)->v.v10.f1;
                        int32_t scrut_452 = (int32_t)(mak_local_hg_194) == (int32_t)(0.00000000000000000);
                        if ((scrut_452 == INT64_C(1))) {
                            *(out) = mak_ir_ctor_obj_11;
                        }
                        else if (1) {
                            double tailarg_453 = mak_ir_ext_12(a0, mak_local_hg_194);
                            struct mak__a * tailarg_454 = mak_local_hh_195;
                            a0 = tailarg_453;
                            a1 = tailarg_454;
                            continue;
                        }
                    }
                    else {
                        *(out) = *(&mak_ir_default_obj__f);
                    }
                    break;
                }
                default: {
                    int32_t scrut_455 = mak_local_gx_185 == (int32_t)(INT64_C(95));
                    if ((scrut_455 == INT64_C(1))) {
                        struct mak__f *arena_call_457 = (struct mak__f *)mak_ir_arena_alloc(sizeof(struct mak__f), _Alignof(struct mak__f));
                        if (!arena_call_457) {
                            mak_ir_recover("ADT result allocation failed");
                            arena_call_457 = &mak_ir_default_obj__f;
                         } else {
                            mak_ir_fn_11(a1, (struct mak__f *)(arena_call_457));
                        }
                        struct mak__f * scrut_456 = arena_call_457;
                        if ((scrut_456 != NULL && (scrut_456)->tag == mak_ir_tag_11)) {
                            (out)->tag = mak_ir_tag_10;
                            (out)->v.v10.f0 = a0;
                            (out)->v.v10.f1 = a1;
                        }
                        else if ((scrut_456 != NULL && (scrut_456)->tag == mak_ir_tag_10)) {
                            double mak_local_hi_196 = (scrut_456)->v.v10.f0;
                            struct mak__a * mak_local_hj_197 = (scrut_456)->v.v10.f1;
                            double tailarg_458 = ((double)(a0) * (double)(mak_local_hi_196));
                            struct mak__a * tailarg_459 = mak_local_hj_197;
                            a0 = tailarg_458;
                            a1 = tailarg_459;
                            continue;
                        }
                        else {
                            *(out) = *(&mak_ir_default_obj__f);
                        }
                    }
                    else if (1) {
                        int32_t scrut_460 = mak_local_gx_185 >= (int32_t)(INT64_C(65));
                        if ((scrut_460 == INT64_C(0))) {
                            int32_t scrut_461 = mak_local_gx_185 >= (int32_t)(INT64_C(97));
                            if ((scrut_461 == INT64_C(0))) {
                                int32_t scrut_462 = mak_local_gx_185 >= (int32_t)(INT64_C(48));
                                if ((scrut_462 == INT64_C(0))) {
                                    switch (mak_local_gx_185) {
                                        case INT64_C(46): {
                                            struct mak__f *arena_call_464 = (struct mak__f *)mak_ir_arena_alloc(sizeof(struct mak__f), _Alignof(struct mak__f));
                                            if (!arena_call_464) {
                                                mak_ir_recover("ADT result allocation failed");
                                                arena_call_464 = &mak_ir_default_obj__f;
                                             } else {
                                                mak_ir_fn_11(a1, (struct mak__f *)(arena_call_464));
                                            }
                                            struct mak__f * scrut_463 = arena_call_464;
                                            if ((scrut_463 != NULL && (scrut_463)->tag == mak_ir_tag_11)) {
                                                (out)->tag = mak_ir_tag_10;
                                                (out)->v.v10.f0 = a0;
                                                (out)->v.v10.f1 = a1;
                                            }
                                            else if ((scrut_463 != NULL && (scrut_463)->tag == mak_ir_tag_10)) {
                                                double mak_local_hm_198 = (scrut_463)->v.v10.f0;
                                                struct mak__a * mak_local_hn_199 = (scrut_463)->v.v10.f1;
                                                double tailarg_465 = ((double)(a0) * (double)(mak_local_hm_198));
                                                struct mak__a * tailarg_466 = mak_local_hn_199;
                                                a0 = tailarg_465;
                                                a1 = tailarg_466;
                                                continue;
                                            }
                                            else {
                                                *(out) = *(&mak_ir_default_obj__f);
                                            }
                                            break;
                                        }
                                        case INT64_C(40): {
                                            struct mak__f *arena_call_468 = (struct mak__f *)mak_ir_arena_alloc(sizeof(struct mak__f), _Alignof(struct mak__f));
                                            if (!arena_call_468) {
                                                mak_ir_recover("ADT result allocation failed");
                                                arena_call_468 = &mak_ir_default_obj__f;
                                             } else {
                                                mak_ir_fn_11(a1, (struct mak__f *)(arena_call_468));
                                            }
                                            struct mak__f * scrut_467 = arena_call_468;
                                            if ((scrut_467 != NULL && (scrut_467)->tag == mak_ir_tag_11)) {
                                                (out)->tag = mak_ir_tag_10;
                                                (out)->v.v10.f0 = a0;
                                                (out)->v.v10.f1 = a1;
                                            }
                                            else if ((scrut_467 != NULL && (scrut_467)->tag == mak_ir_tag_10)) {
                                                double mak_local_ho_200 = (scrut_467)->v.v10.f0;
                                                struct mak__a * mak_local_hp_201 = (scrut_467)->v.v10.f1;
                                                double tailarg_469 = ((double)(a0) * (double)(mak_local_ho_200));
                                                struct mak__a * tailarg_470 = mak_local_hp_201;
                                                a0 = tailarg_469;
                                                a1 = tailarg_470;
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
                                else if (1) {
                                    int32_t scrut_471 = mak_local_gx_185 <= (int32_t)(INT64_C(57));
                                    if ((scrut_471 == INT64_C(1))) {
                                        struct mak__f *arena_call_473 = (struct mak__f *)mak_ir_arena_alloc(sizeof(struct mak__f), _Alignof(struct mak__f));
                                        if (!arena_call_473) {
                                            mak_ir_recover("ADT result allocation failed");
                                            arena_call_473 = &mak_ir_default_obj__f;
                                         } else {
                                            mak_ir_fn_11(a1, (struct mak__f *)(arena_call_473));
                                        }
                                        struct mak__f * scrut_472 = arena_call_473;
                                        if ((scrut_472 != NULL && (scrut_472)->tag == mak_ir_tag_11)) {
                                            (out)->tag = mak_ir_tag_10;
                                            (out)->v.v10.f0 = a0;
                                            (out)->v.v10.f1 = a1;
                                        }
                                        else if ((scrut_472 != NULL && (scrut_472)->tag == mak_ir_tag_10)) {
                                            double mak_local_hk_202 = (scrut_472)->v.v10.f0;
                                            struct mak__a * mak_local_hl_203 = (scrut_472)->v.v10.f1;
                                            double tailarg_474 = ((double)(a0) * (double)(mak_local_hk_202));
                                            struct mak__a * tailarg_475 = mak_local_hl_203;
                                            a0 = tailarg_474;
                                            a1 = tailarg_475;
                                            continue;
                                        }
                                        else {
                                            *(out) = *(&mak_ir_default_obj__f);
                                        }
                                    }
                                    else if (1) {
                                        switch (mak_local_gx_185) {
                                            case INT64_C(46): {
                                                struct mak__f *arena_call_477 = (struct mak__f *)mak_ir_arena_alloc(sizeof(struct mak__f), _Alignof(struct mak__f));
                                                if (!arena_call_477) {
                                                    mak_ir_recover("ADT result allocation failed");
                                                    arena_call_477 = &mak_ir_default_obj__f;
                                                 } else {
                                                    mak_ir_fn_11(a1, (struct mak__f *)(arena_call_477));
                                                }
                                                struct mak__f * scrut_476 = arena_call_477;
                                                if ((scrut_476 != NULL && (scrut_476)->tag == mak_ir_tag_11)) {
                                                    (out)->tag = mak_ir_tag_10;
                                                    (out)->v.v10.f0 = a0;
                                                    (out)->v.v10.f1 = a1;
                                                }
                                                else if ((scrut_476 != NULL && (scrut_476)->tag == mak_ir_tag_10)) {
                                                    double mak_local_hm_204 = (scrut_476)->v.v10.f0;
                                                    struct mak__a * mak_local_hn_205 = (scrut_476)->v.v10.f1;
                                                    double tailarg_478 = ((double)(a0) * (double)(mak_local_hm_204));
                                                    struct mak__a * tailarg_479 = mak_local_hn_205;
                                                    a0 = tailarg_478;
                                                    a1 = tailarg_479;
                                                    continue;
                                                }
                                                else {
                                                    *(out) = *(&mak_ir_default_obj__f);
                                                }
                                                break;
                                            }
                                            case INT64_C(40): {
                                                struct mak__f *arena_call_481 = (struct mak__f *)mak_ir_arena_alloc(sizeof(struct mak__f), _Alignof(struct mak__f));
                                                if (!arena_call_481) {
                                                    mak_ir_recover("ADT result allocation failed");
                                                    arena_call_481 = &mak_ir_default_obj__f;
                                                 } else {
                                                    mak_ir_fn_11(a1, (struct mak__f *)(arena_call_481));
                                                }
                                                struct mak__f * scrut_480 = arena_call_481;
                                                if ((scrut_480 != NULL && (scrut_480)->tag == mak_ir_tag_11)) {
                                                    (out)->tag = mak_ir_tag_10;
                                                    (out)->v.v10.f0 = a0;
                                                    (out)->v.v10.f1 = a1;
                                                }
                                                else if ((scrut_480 != NULL && (scrut_480)->tag == mak_ir_tag_10)) {
                                                    double mak_local_ho_206 = (scrut_480)->v.v10.f0;
                                                    struct mak__a * mak_local_hp_207 = (scrut_480)->v.v10.f1;
                                                    double tailarg_482 = ((double)(a0) * (double)(mak_local_ho_206));
                                                    struct mak__a * tailarg_483 = mak_local_hp_207;
                                                    a0 = tailarg_482;
                                                    a1 = tailarg_483;
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
                            }
                            else if (1) {
                                int32_t scrut_484 = mak_local_gx_185 <= (int32_t)(INT64_C(122));
                                if ((scrut_484 == INT64_C(1))) {
                                    struct mak__f *arena_call_486 = (struct mak__f *)mak_ir_arena_alloc(sizeof(struct mak__f), _Alignof(struct mak__f));
                                    if (!arena_call_486) {
                                        mak_ir_recover("ADT result allocation failed");
                                        arena_call_486 = &mak_ir_default_obj__f;
                                     } else {
                                        mak_ir_fn_11(a1, (struct mak__f *)(arena_call_486));
                                    }
                                    struct mak__f * scrut_485 = arena_call_486;
                                    if ((scrut_485 != NULL && (scrut_485)->tag == mak_ir_tag_11)) {
                                        (out)->tag = mak_ir_tag_10;
                                        (out)->v.v10.f0 = a0;
                                        (out)->v.v10.f1 = a1;
                                    }
                                    else if ((scrut_485 != NULL && (scrut_485)->tag == mak_ir_tag_10)) {
                                        double mak_local_hi_208 = (scrut_485)->v.v10.f0;
                                        struct mak__a * mak_local_hj_209 = (scrut_485)->v.v10.f1;
                                        double tailarg_487 = ((double)(a0) * (double)(mak_local_hi_208));
                                        struct mak__a * tailarg_488 = mak_local_hj_209;
                                        a0 = tailarg_487;
                                        a1 = tailarg_488;
                                        continue;
                                    }
                                    else {
                                        *(out) = *(&mak_ir_default_obj__f);
                                    }
                                }
                                else if (1) {
                                    int32_t scrut_489 = mak_local_gx_185 >= (int32_t)(INT64_C(48));
                                    if ((scrut_489 == INT64_C(0))) {
                                        switch (mak_local_gx_185) {
                                            case INT64_C(46): {
                                                struct mak__f *arena_call_491 = (struct mak__f *)mak_ir_arena_alloc(sizeof(struct mak__f), _Alignof(struct mak__f));
                                                if (!arena_call_491) {
                                                    mak_ir_recover("ADT result allocation failed");
                                                    arena_call_491 = &mak_ir_default_obj__f;
                                                 } else {
                                                    mak_ir_fn_11(a1, (struct mak__f *)(arena_call_491));
                                                }
                                                struct mak__f * scrut_490 = arena_call_491;
                                                if ((scrut_490 != NULL && (scrut_490)->tag == mak_ir_tag_11)) {
                                                    (out)->tag = mak_ir_tag_10;
                                                    (out)->v.v10.f0 = a0;
                                                    (out)->v.v10.f1 = a1;
                                                }
                                                else if ((scrut_490 != NULL && (scrut_490)->tag == mak_ir_tag_10)) {
                                                    double mak_local_hm_210 = (scrut_490)->v.v10.f0;
                                                    struct mak__a * mak_local_hn_211 = (scrut_490)->v.v10.f1;
                                                    double tailarg_492 = ((double)(a0) * (double)(mak_local_hm_210));
                                                    struct mak__a * tailarg_493 = mak_local_hn_211;
                                                    a0 = tailarg_492;
                                                    a1 = tailarg_493;
                                                    continue;
                                                }
                                                else {
                                                    *(out) = *(&mak_ir_default_obj__f);
                                                }
                                                break;
                                            }
                                            case INT64_C(40): {
                                                struct mak__f *arena_call_495 = (struct mak__f *)mak_ir_arena_alloc(sizeof(struct mak__f), _Alignof(struct mak__f));
                                                if (!arena_call_495) {
                                                    mak_ir_recover("ADT result allocation failed");
                                                    arena_call_495 = &mak_ir_default_obj__f;
                                                 } else {
                                                    mak_ir_fn_11(a1, (struct mak__f *)(arena_call_495));
                                                }
                                                struct mak__f * scrut_494 = arena_call_495;
                                                if ((scrut_494 != NULL && (scrut_494)->tag == mak_ir_tag_11)) {
                                                    (out)->tag = mak_ir_tag_10;
                                                    (out)->v.v10.f0 = a0;
                                                    (out)->v.v10.f1 = a1;
                                                }
                                                else if ((scrut_494 != NULL && (scrut_494)->tag == mak_ir_tag_10)) {
                                                    double mak_local_ho_212 = (scrut_494)->v.v10.f0;
                                                    struct mak__a * mak_local_hp_213 = (scrut_494)->v.v10.f1;
                                                    double tailarg_496 = ((double)(a0) * (double)(mak_local_ho_212));
                                                    struct mak__a * tailarg_497 = mak_local_hp_213;
                                                    a0 = tailarg_496;
                                                    a1 = tailarg_497;
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
                                    else if (1) {
                                        int32_t scrut_498 = mak_local_gx_185 <= (int32_t)(INT64_C(57));
                                        if ((scrut_498 == INT64_C(1))) {
                                            struct mak__f *arena_call_500 = (struct mak__f *)mak_ir_arena_alloc(sizeof(struct mak__f), _Alignof(struct mak__f));
                                            if (!arena_call_500) {
                                                mak_ir_recover("ADT result allocation failed");
                                                arena_call_500 = &mak_ir_default_obj__f;
                                             } else {
                                                mak_ir_fn_11(a1, (struct mak__f *)(arena_call_500));
                                            }
                                            struct mak__f * scrut_499 = arena_call_500;
                                            if ((scrut_499 != NULL && (scrut_499)->tag == mak_ir_tag_11)) {
                                                (out)->tag = mak_ir_tag_10;
                                                (out)->v.v10.f0 = a0;
                                                (out)->v.v10.f1 = a1;
                                            }
                                            else if ((scrut_499 != NULL && (scrut_499)->tag == mak_ir_tag_10)) {
                                                double mak_local_hk_214 = (scrut_499)->v.v10.f0;
                                                struct mak__a * mak_local_hl_215 = (scrut_499)->v.v10.f1;
                                                double tailarg_501 = ((double)(a0) * (double)(mak_local_hk_214));
                                                struct mak__a * tailarg_502 = mak_local_hl_215;
                                                a0 = tailarg_501;
                                                a1 = tailarg_502;
                                                continue;
                                            }
                                            else {
                                                *(out) = *(&mak_ir_default_obj__f);
                                            }
                                        }
                                        else if (1) {
                                            switch (mak_local_gx_185) {
                                                case INT64_C(46): {
                                                    struct mak__f *arena_call_504 = (struct mak__f *)mak_ir_arena_alloc(sizeof(struct mak__f), _Alignof(struct mak__f));
                                                    if (!arena_call_504) {
                                                        mak_ir_recover("ADT result allocation failed");
                                                        arena_call_504 = &mak_ir_default_obj__f;
                                                     } else {
                                                        mak_ir_fn_11(a1, (struct mak__f *)(arena_call_504));
                                                    }
                                                    struct mak__f * scrut_503 = arena_call_504;
                                                    if ((scrut_503 != NULL && (scrut_503)->tag == mak_ir_tag_11)) {
                                                        (out)->tag = mak_ir_tag_10;
                                                        (out)->v.v10.f0 = a0;
                                                        (out)->v.v10.f1 = a1;
                                                    }
                                                    else if ((scrut_503 != NULL && (scrut_503)->tag == mak_ir_tag_10)) {
                                                        double mak_local_hm_216 = (scrut_503)->v.v10.f0;
                                                        struct mak__a * mak_local_hn_217 = (scrut_503)->v.v10.f1;
                                                        double tailarg_505 = ((double)(a0) * (double)(mak_local_hm_216));
                                                        struct mak__a * tailarg_506 = mak_local_hn_217;
                                                        a0 = tailarg_505;
                                                        a1 = tailarg_506;
                                                        continue;
                                                    }
                                                    else {
                                                        *(out) = *(&mak_ir_default_obj__f);
                                                    }
                                                    break;
                                                }
                                                case INT64_C(40): {
                                                    struct mak__f *arena_call_508 = (struct mak__f *)mak_ir_arena_alloc(sizeof(struct mak__f), _Alignof(struct mak__f));
                                                    if (!arena_call_508) {
                                                        mak_ir_recover("ADT result allocation failed");
                                                        arena_call_508 = &mak_ir_default_obj__f;
                                                     } else {
                                                        mak_ir_fn_11(a1, (struct mak__f *)(arena_call_508));
                                                    }
                                                    struct mak__f * scrut_507 = arena_call_508;
                                                    if ((scrut_507 != NULL && (scrut_507)->tag == mak_ir_tag_11)) {
                                                        (out)->tag = mak_ir_tag_10;
                                                        (out)->v.v10.f0 = a0;
                                                        (out)->v.v10.f1 = a1;
                                                    }
                                                    else if ((scrut_507 != NULL && (scrut_507)->tag == mak_ir_tag_10)) {
                                                        double mak_local_ho_218 = (scrut_507)->v.v10.f0;
                                                        struct mak__a * mak_local_hp_219 = (scrut_507)->v.v10.f1;
                                                        double tailarg_509 = ((double)(a0) * (double)(mak_local_ho_218));
                                                        struct mak__a * tailarg_510 = mak_local_hp_219;
                                                        a0 = tailarg_509;
                                                        a1 = tailarg_510;
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
                                }
                            }
                        }
                        else if (1) {
                            int32_t scrut_511 = mak_local_gx_185 <= (int32_t)(INT64_C(90));
                            if ((scrut_511 == INT64_C(1))) {
                                struct mak__f *arena_call_513 = (struct mak__f *)mak_ir_arena_alloc(sizeof(struct mak__f), _Alignof(struct mak__f));
                                if (!arena_call_513) {
                                    mak_ir_recover("ADT result allocation failed");
                                    arena_call_513 = &mak_ir_default_obj__f;
                                 } else {
                                    mak_ir_fn_11(a1, (struct mak__f *)(arena_call_513));
                                }
                                struct mak__f * scrut_512 = arena_call_513;
                                if ((scrut_512 != NULL && (scrut_512)->tag == mak_ir_tag_11)) {
                                    (out)->tag = mak_ir_tag_10;
                                    (out)->v.v10.f0 = a0;
                                    (out)->v.v10.f1 = a1;
                                }
                                else if ((scrut_512 != NULL && (scrut_512)->tag == mak_ir_tag_10)) {
                                    double mak_local_hi_220 = (scrut_512)->v.v10.f0;
                                    struct mak__a * mak_local_hj_221 = (scrut_512)->v.v10.f1;
                                    double tailarg_514 = ((double)(a0) * (double)(mak_local_hi_220));
                                    struct mak__a * tailarg_515 = mak_local_hj_221;
                                    a0 = tailarg_514;
                                    a1 = tailarg_515;
                                    continue;
                                }
                                else {
                                    *(out) = *(&mak_ir_default_obj__f);
                                }
                            }
                            else if (1) {
                                int32_t scrut_516 = mak_local_gx_185 >= (int32_t)(INT64_C(48));
                                if ((scrut_516 == INT64_C(0))) {
                                    switch (mak_local_gx_185) {
                                        case INT64_C(46): {
                                            struct mak__f *arena_call_518 = (struct mak__f *)mak_ir_arena_alloc(sizeof(struct mak__f), _Alignof(struct mak__f));
                                            if (!arena_call_518) {
                                                mak_ir_recover("ADT result allocation failed");
                                                arena_call_518 = &mak_ir_default_obj__f;
                                             } else {
                                                mak_ir_fn_11(a1, (struct mak__f *)(arena_call_518));
                                            }
                                            struct mak__f * scrut_517 = arena_call_518;
                                            if ((scrut_517 != NULL && (scrut_517)->tag == mak_ir_tag_11)) {
                                                (out)->tag = mak_ir_tag_10;
                                                (out)->v.v10.f0 = a0;
                                                (out)->v.v10.f1 = a1;
                                            }
                                            else if ((scrut_517 != NULL && (scrut_517)->tag == mak_ir_tag_10)) {
                                                double mak_local_hm_222 = (scrut_517)->v.v10.f0;
                                                struct mak__a * mak_local_hn_223 = (scrut_517)->v.v10.f1;
                                                double tailarg_519 = ((double)(a0) * (double)(mak_local_hm_222));
                                                struct mak__a * tailarg_520 = mak_local_hn_223;
                                                a0 = tailarg_519;
                                                a1 = tailarg_520;
                                                continue;
                                            }
                                            else {
                                                *(out) = *(&mak_ir_default_obj__f);
                                            }
                                            break;
                                        }
                                        case INT64_C(40): {
                                            struct mak__f *arena_call_522 = (struct mak__f *)mak_ir_arena_alloc(sizeof(struct mak__f), _Alignof(struct mak__f));
                                            if (!arena_call_522) {
                                                mak_ir_recover("ADT result allocation failed");
                                                arena_call_522 = &mak_ir_default_obj__f;
                                             } else {
                                                mak_ir_fn_11(a1, (struct mak__f *)(arena_call_522));
                                            }
                                            struct mak__f * scrut_521 = arena_call_522;
                                            if ((scrut_521 != NULL && (scrut_521)->tag == mak_ir_tag_11)) {
                                                (out)->tag = mak_ir_tag_10;
                                                (out)->v.v10.f0 = a0;
                                                (out)->v.v10.f1 = a1;
                                            }
                                            else if ((scrut_521 != NULL && (scrut_521)->tag == mak_ir_tag_10)) {
                                                double mak_local_ho_224 = (scrut_521)->v.v10.f0;
                                                struct mak__a * mak_local_hp_225 = (scrut_521)->v.v10.f1;
                                                double tailarg_523 = ((double)(a0) * (double)(mak_local_ho_224));
                                                struct mak__a * tailarg_524 = mak_local_hp_225;
                                                a0 = tailarg_523;
                                                a1 = tailarg_524;
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
                                else if (1) {
                                    int32_t scrut_525 = mak_local_gx_185 <= (int32_t)(INT64_C(57));
                                    if ((scrut_525 == INT64_C(1))) {
                                        struct mak__f *arena_call_527 = (struct mak__f *)mak_ir_arena_alloc(sizeof(struct mak__f), _Alignof(struct mak__f));
                                        if (!arena_call_527) {
                                            mak_ir_recover("ADT result allocation failed");
                                            arena_call_527 = &mak_ir_default_obj__f;
                                         } else {
                                            mak_ir_fn_11(a1, (struct mak__f *)(arena_call_527));
                                        }
                                        struct mak__f * scrut_526 = arena_call_527;
                                        if ((scrut_526 != NULL && (scrut_526)->tag == mak_ir_tag_11)) {
                                            (out)->tag = mak_ir_tag_10;
                                            (out)->v.v10.f0 = a0;
                                            (out)->v.v10.f1 = a1;
                                        }
                                        else if ((scrut_526 != NULL && (scrut_526)->tag == mak_ir_tag_10)) {
                                            double mak_local_hk_226 = (scrut_526)->v.v10.f0;
                                            struct mak__a * mak_local_hl_227 = (scrut_526)->v.v10.f1;
                                            double tailarg_528 = ((double)(a0) * (double)(mak_local_hk_226));
                                            struct mak__a * tailarg_529 = mak_local_hl_227;
                                            a0 = tailarg_528;
                                            a1 = tailarg_529;
                                            continue;
                                        }
                                        else {
                                            *(out) = *(&mak_ir_default_obj__f);
                                        }
                                    }
                                    else if (1) {
                                        switch (mak_local_gx_185) {
                                            case INT64_C(46): {
                                                struct mak__f *arena_call_531 = (struct mak__f *)mak_ir_arena_alloc(sizeof(struct mak__f), _Alignof(struct mak__f));
                                                if (!arena_call_531) {
                                                    mak_ir_recover("ADT result allocation failed");
                                                    arena_call_531 = &mak_ir_default_obj__f;
                                                 } else {
                                                    mak_ir_fn_11(a1, (struct mak__f *)(arena_call_531));
                                                }
                                                struct mak__f * scrut_530 = arena_call_531;
                                                if ((scrut_530 != NULL && (scrut_530)->tag == mak_ir_tag_11)) {
                                                    (out)->tag = mak_ir_tag_10;
                                                    (out)->v.v10.f0 = a0;
                                                    (out)->v.v10.f1 = a1;
                                                }
                                                else if ((scrut_530 != NULL && (scrut_530)->tag == mak_ir_tag_10)) {
                                                    double mak_local_hm_228 = (scrut_530)->v.v10.f0;
                                                    struct mak__a * mak_local_hn_229 = (scrut_530)->v.v10.f1;
                                                    double tailarg_532 = ((double)(a0) * (double)(mak_local_hm_228));
                                                    struct mak__a * tailarg_533 = mak_local_hn_229;
                                                    a0 = tailarg_532;
                                                    a1 = tailarg_533;
                                                    continue;
                                                }
                                                else {
                                                    *(out) = *(&mak_ir_default_obj__f);
                                                }
                                                break;
                                            }
                                            case INT64_C(40): {
                                                struct mak__f *arena_call_535 = (struct mak__f *)mak_ir_arena_alloc(sizeof(struct mak__f), _Alignof(struct mak__f));
                                                if (!arena_call_535) {
                                                    mak_ir_recover("ADT result allocation failed");
                                                    arena_call_535 = &mak_ir_default_obj__f;
                                                 } else {
                                                    mak_ir_fn_11(a1, (struct mak__f *)(arena_call_535));
                                                }
                                                struct mak__f * scrut_534 = arena_call_535;
                                                if ((scrut_534 != NULL && (scrut_534)->tag == mak_ir_tag_11)) {
                                                    (out)->tag = mak_ir_tag_10;
                                                    (out)->v.v10.f0 = a0;
                                                    (out)->v.v10.f1 = a1;
                                                }
                                                else if ((scrut_534 != NULL && (scrut_534)->tag == mak_ir_tag_10)) {
                                                    double mak_local_ho_230 = (scrut_534)->v.v10.f0;
                                                    struct mak__a * mak_local_hp_231 = (scrut_534)->v.v10.f1;
                                                    double tailarg_536 = ((double)(a0) * (double)(mak_local_ho_230));
                                                    struct mak__a * tailarg_537 = mak_local_hp_231;
                                                    a0 = tailarg_536;
                                                    a1 = tailarg_537;
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
    struct mak__f *arena_call_539 = (struct mak__f *)__builtin_alloca(sizeof(struct mak__f));
        mak_ir_fn_11(a0, (struct mak__f *)(arena_call_539));
    struct mak__f * scrut_538 = arena_call_539;
    if ((scrut_538 != NULL && (scrut_538)->tag == mak_ir_tag_11)) {
        *(out) = mak_ir_ctor_obj_11;
    }
    else if ((scrut_538 != NULL && (scrut_538)->tag == mak_ir_tag_10)) {
        double mak_local_hr_232 = (scrut_538)->v.v10.f0;
        struct mak__a * mak_local_hs_233 = (scrut_538)->v.v10.f1;
        mak_ir_fn_13(mak_local_hr_232, mak_local_hs_233, (struct mak__f *)(out));
    }
    else {
        *(out) = *(&mak_ir_default_obj__f);
    }
    return;
}

static inline void mak_ir_fn_15(double a0, struct mak__a * a1, struct mak__f *out) {
    for (;;) {
        struct mak__a *arena_call_541 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
        if (!arena_call_541) {
            mak_ir_recover("ADT result allocation failed");
            arena_call_541 = &mak_ir_default_obj__a;
         } else {
            mak_ir_fn_2(a1, (struct mak__a *)(arena_call_541));
        }
        struct mak__a * scrut_540 = arena_call_541;
        if ((scrut_540 != NULL && (scrut_540)->tag == mak_ir_tag_0)) {
            (out)->tag = mak_ir_tag_10;
            (out)->v.v10.f0 = a0;
            (out)->v.v10.f1 = &mak_ir_ctor_obj_0;
        }
        else if ((scrut_540 != NULL && (scrut_540)->tag == mak_ir_tag_1)) {
            int32_t mak_local_hv_234 = (scrut_540)->v.v1.f0;
            struct mak__a * mak_local_hw_235 = (scrut_540)->v.v1.f1;
            switch (mak_local_hv_234) {
                case INT64_C(43): {
                    struct mak__f *arena_call_543 = (struct mak__f *)mak_ir_arena_alloc(sizeof(struct mak__f), _Alignof(struct mak__f));
                    if (!arena_call_543) {
                        mak_ir_recover("ADT result allocation failed");
                        arena_call_543 = &mak_ir_default_obj__f;
                     } else {
                        mak_ir_fn_14(mak_local_hw_235, (struct mak__f *)(arena_call_543));
                    }
                    struct mak__f * scrut_542 = arena_call_543;
                    if ((scrut_542 != NULL && (scrut_542)->tag == mak_ir_tag_11)) {
                        *(out) = mak_ir_ctor_obj_11;
                    }
                    else if ((scrut_542 != NULL && (scrut_542)->tag == mak_ir_tag_10)) {
                        double mak_local_hx_236 = (scrut_542)->v.v10.f0;
                        struct mak__a * mak_local_hy_237 = (scrut_542)->v.v10.f1;
                        double tailarg_544 = ((double)(a0) + (double)(mak_local_hx_236));
                        struct mak__a * tailarg_545 = mak_local_hy_237;
                        a0 = tailarg_544;
                        a1 = tailarg_545;
                        continue;
                    }
                    else {
                        *(out) = *(&mak_ir_default_obj__f);
                    }
                    break;
                }
                case INT64_C(45): {
                    struct mak__f *arena_call_547 = (struct mak__f *)mak_ir_arena_alloc(sizeof(struct mak__f), _Alignof(struct mak__f));
                    if (!arena_call_547) {
                        mak_ir_recover("ADT result allocation failed");
                        arena_call_547 = &mak_ir_default_obj__f;
                     } else {
                        mak_ir_fn_14(mak_local_hw_235, (struct mak__f *)(arena_call_547));
                    }
                    struct mak__f * scrut_546 = arena_call_547;
                    if ((scrut_546 != NULL && (scrut_546)->tag == mak_ir_tag_11)) {
                        *(out) = mak_ir_ctor_obj_11;
                    }
                    else if ((scrut_546 != NULL && (scrut_546)->tag == mak_ir_tag_10)) {
                        double mak_local_hz_238 = (scrut_546)->v.v10.f0;
                        struct mak__a * mak_local_ia_239 = (scrut_546)->v.v10.f1;
                        double tailarg_548 = ((double)(a0) - (double)(mak_local_hz_238));
                        struct mak__a * tailarg_549 = mak_local_ia_239;
                        a0 = tailarg_548;
                        a1 = tailarg_549;
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
    struct mak__f *arena_call_551 = (struct mak__f *)__builtin_alloca(sizeof(struct mak__f));
        mak_ir_fn_14(a0, (struct mak__f *)(arena_call_551));
    struct mak__f * scrut_550 = arena_call_551;
    if ((scrut_550 != NULL && (scrut_550)->tag == mak_ir_tag_11)) {
        *(out) = mak_ir_ctor_obj_11;
    }
    else if ((scrut_550 != NULL && (scrut_550)->tag == mak_ir_tag_10)) {
        double mak_local_ic_240 = (scrut_550)->v.v10.f0;
        struct mak__a * mak_local_id_241 = (scrut_550)->v.v10.f1;
        mak_ir_fn_15(mak_local_ic_240, mak_local_id_241, (struct mak__f *)(out));
    }
    else {
        *(out) = *(&mak_ir_default_obj__f);
    }
    return;
}

static inline double mak_ir_fn_17(void) {
    for (;;) {
        mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)(">>> "), mak_ir_ext_3((const void *)(">>> ")));
        int32_t mak_local___mak_opt_scrut_1_242 = mak_ir_ext_0();
        struct mak__b *arena_case_553 = (struct mak__b *)mak_ir_arena_alloc(sizeof(struct mak__b), _Alignof(struct mak__b));
        if (!arena_case_553) {
            mak_ir_recover("ADT case-result allocation failed");
            arena_case_553 = &mak_ir_default_obj__b;
         } else {
            int32_t scrut_554 = mak_local___mak_opt_scrut_1_242 < (int32_t)(INT64_C(0));
            if ((scrut_554 == INT64_C(1))) {
                *(arena_case_553) = mak_ir_ctor_obj_2;
            }
            else if (1) {
                int32_t scrut_555 = mak_local___mak_opt_scrut_1_242 == (int32_t)(INT64_C(10));
                if ((scrut_555 == INT64_C(1))) {
                    (arena_case_553)->tag = mak_ir_tag_3;
                    (arena_case_553)->v.v3.f0 = &mak_ir_ctor_obj_0;
                }
                else if (1) {
                    struct mak__a *arena_obj_556 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
                    if (!arena_obj_556) {
                        mak_ir_recover("ADT object allocation failed");
                        arena_obj_556 = &mak_ir_default_obj__a;
                     } else {
                        (arena_obj_556)->tag = mak_ir_tag_1;
                        (arena_obj_556)->v.v1.f0 = mak_local___mak_opt_scrut_1_242;
                        (arena_obj_556)->v.v1.f1 = &mak_ir_ctor_obj_0;
                    }
                    mak_ir_fn_1(arena_obj_556, (struct mak__b *)(arena_case_553));
                }
            }
        }
        struct mak__b * scrut_552 = arena_case_553;
        if ((scrut_552 != NULL && (scrut_552)->tag == mak_ir_tag_2)) {
            struct mak__b *arena_obj_557 = (struct mak__b *)mak_ir_arena_alloc(sizeof(struct mak__b), _Alignof(struct mak__b));
            if (!arena_obj_557) {
                mak_ir_recover("ADT object allocation failed");
                arena_obj_557 = &mak_ir_default_obj__b;
             } else {
                (arena_obj_557)->tag = mak_ir_tag_3;
                (arena_obj_557)->v.v3.f0 = (struct mak__a *)mak_ir_adt_recover_ptr((const void *)&mak_ir_default_obj__a, "integer used as ADT");
            }
            return ((double)(uintptr_t)(arena_obj_557));
        }
        else if ((scrut_552 != NULL && (scrut_552)->tag == mak_ir_tag_3)) {
            struct mak__a * mak_local_ii_243 = (scrut_552)->v.v3.f0;
            struct mak__a *arena_call_559 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
            if (!arena_call_559) {
                mak_ir_recover("ADT result allocation failed");
                arena_call_559 = &mak_ir_default_obj__a;
             } else {
                mak_ir_fn_2(mak_local_ii_243, (struct mak__a *)(arena_call_559));
            }
            struct mak__a * scrut_558 = arena_call_559;
            if ((scrut_558 != NULL && (scrut_558)->tag == mak_ir_tag_0)) {
                struct mak__a *arena_call_561 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
                if (!arena_call_561) {
                    mak_ir_recover("ADT result allocation failed");
                    arena_call_561 = &mak_ir_default_obj__a;
                 } else {
                    mak_ir_fn_2(mak_local_ii_243, (struct mak__a *)(arena_call_561));
                }
                struct mak__a * scrut_560 = arena_call_561;
                if ((scrut_560 != NULL && (scrut_560)->tag == mak_ir_tag_1) && (scrut_560->v.v1.f0 == INT64_C(63))) {
                    struct mak__a * mak_local_br_244 = (scrut_560)->v.v1.f1;
                    struct mak__a *arena_call_563 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
                    if (!arena_call_563) {
                        mak_ir_recover("ADT result allocation failed");
                        arena_call_563 = &mak_ir_default_obj__a;
                     } else {
                        mak_ir_fn_2(mak_local_br_244, (struct mak__a *)(arena_call_563));
                    }
                    struct mak__a * scrut_562 = arena_call_563;
                    if ((scrut_562 != NULL && (scrut_562)->tag == mak_ir_tag_0)) {
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
                        int32_t discard_564 = 0;
                        struct mak__f *arena_call_566 = (struct mak__f *)mak_ir_arena_alloc(sizeof(struct mak__f), _Alignof(struct mak__f));
                        if (!arena_call_566) {
                            mak_ir_recover("ADT result allocation failed");
                            arena_call_566 = &mak_ir_default_obj__f;
                         } else {
                            mak_ir_fn_16(mak_local_ii_243, (struct mak__f *)(arena_call_566));
                        }
                        struct mak__f * scrut_565 = arena_call_566;
                        if ((scrut_565 != NULL && (scrut_565)->tag == mak_ir_tag_11)) {
                            mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)("Error: invalid expression\n"), mak_ir_ext_3((const void *)("Error: invalid expression\n")));
                        }
                        else if ((scrut_565 != NULL && (scrut_565)->tag == mak_ir_tag_10)) {
                            double mak_local_ig_245 = (scrut_565)->v.v10.f0;
                            struct mak__a * mak_local_ih_246 = (scrut_565)->v.v10.f1;
                            int32_t case_567 = 0;
                            struct mak__a *arena_call_569 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
                            if (!arena_call_569) {
                                mak_ir_recover("ADT result allocation failed");
                                arena_call_569 = &mak_ir_default_obj__a;
                             } else {
                                mak_ir_fn_2(mak_local_ih_246, (struct mak__a *)(arena_call_569));
                            }
                            struct mak__a * scrut_568 = arena_call_569;
                            if ((scrut_568 != NULL && (scrut_568)->tag == mak_ir_tag_0)) {
                                case_567 = mak_ir_ext_2((const void *)("%.17g\n"), mak_local_ig_245);
                            }
                            else if (1) {
                                mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)("Error: unexpected characters\n"), mak_ir_ext_3((const void *)("Error: unexpected characters\n")));
                            }
                            discard_564 = case_567;
                        }
                        else {
                            discard_564 = 0;
                        }
                        continue;
                    }
                }
                else if ((scrut_560 != NULL && (scrut_560)->tag == mak_ir_tag_1) && (scrut_560->v.v1.f0 == INT64_C(104))) {
                    struct mak__a * mak_local_bs_247 = (scrut_560)->v.v1.f1;
                    if ((mak_local_bs_247 != NULL && (mak_local_bs_247)->tag == mak_ir_tag_1) && (mak_local_bs_247->v.v1.f0 == INT64_C(101))) {
                        struct mak__a * mak_local_bt_248 = (mak_local_bs_247)->v.v1.f1;
                        if ((mak_local_bt_248 != NULL && (mak_local_bt_248)->tag == mak_ir_tag_1) && (mak_local_bt_248->v.v1.f0 == INT64_C(108))) {
                            struct mak__a * mak_local_bu_249 = (mak_local_bt_248)->v.v1.f1;
                            if ((mak_local_bu_249 != NULL && (mak_local_bu_249)->tag == mak_ir_tag_1) && (mak_local_bu_249->v.v1.f0 == INT64_C(112))) {
                                struct mak__a * mak_local_bv_250 = (mak_local_bu_249)->v.v1.f1;
                                struct mak__a *arena_call_571 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
                                if (!arena_call_571) {
                                    mak_ir_recover("ADT result allocation failed");
                                    arena_call_571 = &mak_ir_default_obj__a;
                                 } else {
                                    mak_ir_fn_2(mak_local_bv_250, (struct mak__a *)(arena_call_571));
                                }
                                struct mak__a * scrut_570 = arena_call_571;
                                if ((scrut_570 != NULL && (scrut_570)->tag == mak_ir_tag_0)) {
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
                                    int32_t discard_572 = 0;
                                    struct mak__f *arena_call_574 = (struct mak__f *)mak_ir_arena_alloc(sizeof(struct mak__f), _Alignof(struct mak__f));
                                    if (!arena_call_574) {
                                        mak_ir_recover("ADT result allocation failed");
                                        arena_call_574 = &mak_ir_default_obj__f;
                                     } else {
                                        mak_ir_fn_16(mak_local_ii_243, (struct mak__f *)(arena_call_574));
                                    }
                                    struct mak__f * scrut_573 = arena_call_574;
                                    if ((scrut_573 != NULL && (scrut_573)->tag == mak_ir_tag_11)) {
                                        mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)("Error: invalid expression\n"), mak_ir_ext_3((const void *)("Error: invalid expression\n")));
                                    }
                                    else if ((scrut_573 != NULL && (scrut_573)->tag == mak_ir_tag_10)) {
                                        double mak_local_ig_251 = (scrut_573)->v.v10.f0;
                                        struct mak__a * mak_local_ih_252 = (scrut_573)->v.v10.f1;
                                        int32_t case_575 = 0;
                                        struct mak__a *arena_call_577 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
                                        if (!arena_call_577) {
                                            mak_ir_recover("ADT result allocation failed");
                                            arena_call_577 = &mak_ir_default_obj__a;
                                         } else {
                                            mak_ir_fn_2(mak_local_ih_252, (struct mak__a *)(arena_call_577));
                                        }
                                        struct mak__a * scrut_576 = arena_call_577;
                                        if ((scrut_576 != NULL && (scrut_576)->tag == mak_ir_tag_0)) {
                                            case_575 = mak_ir_ext_2((const void *)("%.17g\n"), mak_local_ig_251);
                                        }
                                        else if (1) {
                                            mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)("Error: unexpected characters\n"), mak_ir_ext_3((const void *)("Error: unexpected characters\n")));
                                        }
                                        discard_572 = case_575;
                                    }
                                    else {
                                        discard_572 = 0;
                                    }
                                    continue;
                                }
                            }
                            else if (1) {
                                int32_t discard_578 = 0;
                                struct mak__f *arena_call_580 = (struct mak__f *)mak_ir_arena_alloc(sizeof(struct mak__f), _Alignof(struct mak__f));
                                if (!arena_call_580) {
                                    mak_ir_recover("ADT result allocation failed");
                                    arena_call_580 = &mak_ir_default_obj__f;
                                 } else {
                                    mak_ir_fn_16(mak_local_ii_243, (struct mak__f *)(arena_call_580));
                                }
                                struct mak__f * scrut_579 = arena_call_580;
                                if ((scrut_579 != NULL && (scrut_579)->tag == mak_ir_tag_11)) {
                                    mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)("Error: invalid expression\n"), mak_ir_ext_3((const void *)("Error: invalid expression\n")));
                                }
                                else if ((scrut_579 != NULL && (scrut_579)->tag == mak_ir_tag_10)) {
                                    double mak_local_ig_253 = (scrut_579)->v.v10.f0;
                                    struct mak__a * mak_local_ih_254 = (scrut_579)->v.v10.f1;
                                    int32_t case_581 = 0;
                                    struct mak__a *arena_call_583 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
                                    if (!arena_call_583) {
                                        mak_ir_recover("ADT result allocation failed");
                                        arena_call_583 = &mak_ir_default_obj__a;
                                     } else {
                                        mak_ir_fn_2(mak_local_ih_254, (struct mak__a *)(arena_call_583));
                                    }
                                    struct mak__a * scrut_582 = arena_call_583;
                                    if ((scrut_582 != NULL && (scrut_582)->tag == mak_ir_tag_0)) {
                                        case_581 = mak_ir_ext_2((const void *)("%.17g\n"), mak_local_ig_253);
                                    }
                                    else if (1) {
                                        mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)("Error: unexpected characters\n"), mak_ir_ext_3((const void *)("Error: unexpected characters\n")));
                                    }
                                    discard_578 = case_581;
                                }
                                else {
                                    discard_578 = 0;
                                }
                                continue;
                            }
                        }
                        else if (1) {
                            int32_t discard_584 = 0;
                            struct mak__f *arena_call_586 = (struct mak__f *)mak_ir_arena_alloc(sizeof(struct mak__f), _Alignof(struct mak__f));
                            if (!arena_call_586) {
                                mak_ir_recover("ADT result allocation failed");
                                arena_call_586 = &mak_ir_default_obj__f;
                             } else {
                                mak_ir_fn_16(mak_local_ii_243, (struct mak__f *)(arena_call_586));
                            }
                            struct mak__f * scrut_585 = arena_call_586;
                            if ((scrut_585 != NULL && (scrut_585)->tag == mak_ir_tag_11)) {
                                mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)("Error: invalid expression\n"), mak_ir_ext_3((const void *)("Error: invalid expression\n")));
                            }
                            else if ((scrut_585 != NULL && (scrut_585)->tag == mak_ir_tag_10)) {
                                double mak_local_ig_255 = (scrut_585)->v.v10.f0;
                                struct mak__a * mak_local_ih_256 = (scrut_585)->v.v10.f1;
                                int32_t case_587 = 0;
                                struct mak__a *arena_call_589 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
                                if (!arena_call_589) {
                                    mak_ir_recover("ADT result allocation failed");
                                    arena_call_589 = &mak_ir_default_obj__a;
                                 } else {
                                    mak_ir_fn_2(mak_local_ih_256, (struct mak__a *)(arena_call_589));
                                }
                                struct mak__a * scrut_588 = arena_call_589;
                                if ((scrut_588 != NULL && (scrut_588)->tag == mak_ir_tag_0)) {
                                    case_587 = mak_ir_ext_2((const void *)("%.17g\n"), mak_local_ig_255);
                                }
                                else if (1) {
                                    mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)("Error: unexpected characters\n"), mak_ir_ext_3((const void *)("Error: unexpected characters\n")));
                                }
                                discard_584 = case_587;
                            }
                            else {
                                discard_584 = 0;
                            }
                            continue;
                        }
                    }
                    else if (1) {
                        int32_t discard_590 = 0;
                        struct mak__f *arena_call_592 = (struct mak__f *)mak_ir_arena_alloc(sizeof(struct mak__f), _Alignof(struct mak__f));
                        if (!arena_call_592) {
                            mak_ir_recover("ADT result allocation failed");
                            arena_call_592 = &mak_ir_default_obj__f;
                         } else {
                            mak_ir_fn_16(mak_local_ii_243, (struct mak__f *)(arena_call_592));
                        }
                        struct mak__f * scrut_591 = arena_call_592;
                        if ((scrut_591 != NULL && (scrut_591)->tag == mak_ir_tag_11)) {
                            mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)("Error: invalid expression\n"), mak_ir_ext_3((const void *)("Error: invalid expression\n")));
                        }
                        else if ((scrut_591 != NULL && (scrut_591)->tag == mak_ir_tag_10)) {
                            double mak_local_ig_257 = (scrut_591)->v.v10.f0;
                            struct mak__a * mak_local_ih_258 = (scrut_591)->v.v10.f1;
                            int32_t case_593 = 0;
                            struct mak__a *arena_call_595 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
                            if (!arena_call_595) {
                                mak_ir_recover("ADT result allocation failed");
                                arena_call_595 = &mak_ir_default_obj__a;
                             } else {
                                mak_ir_fn_2(mak_local_ih_258, (struct mak__a *)(arena_call_595));
                            }
                            struct mak__a * scrut_594 = arena_call_595;
                            if ((scrut_594 != NULL && (scrut_594)->tag == mak_ir_tag_0)) {
                                case_593 = mak_ir_ext_2((const void *)("%.17g\n"), mak_local_ig_257);
                            }
                            else if (1) {
                                mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)("Error: unexpected characters\n"), mak_ir_ext_3((const void *)("Error: unexpected characters\n")));
                            }
                            discard_590 = case_593;
                        }
                        else {
                            discard_590 = 0;
                        }
                        continue;
                    }
                }
                else if (1) {
                    int32_t discard_596 = 0;
                    struct mak__f *arena_call_598 = (struct mak__f *)mak_ir_arena_alloc(sizeof(struct mak__f), _Alignof(struct mak__f));
                    if (!arena_call_598) {
                        mak_ir_recover("ADT result allocation failed");
                        arena_call_598 = &mak_ir_default_obj__f;
                     } else {
                        mak_ir_fn_16(mak_local_ii_243, (struct mak__f *)(arena_call_598));
                    }
                    struct mak__f * scrut_597 = arena_call_598;
                    if ((scrut_597 != NULL && (scrut_597)->tag == mak_ir_tag_11)) {
                        mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)("Error: invalid expression\n"), mak_ir_ext_3((const void *)("Error: invalid expression\n")));
                    }
                    else if ((scrut_597 != NULL && (scrut_597)->tag == mak_ir_tag_10)) {
                        double mak_local_ig_259 = (scrut_597)->v.v10.f0;
                        struct mak__a * mak_local_ih_260 = (scrut_597)->v.v10.f1;
                        int32_t case_599 = 0;
                        struct mak__a *arena_call_601 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
                        if (!arena_call_601) {
                            mak_ir_recover("ADT result allocation failed");
                            arena_call_601 = &mak_ir_default_obj__a;
                         } else {
                            mak_ir_fn_2(mak_local_ih_260, (struct mak__a *)(arena_call_601));
                        }
                        struct mak__a * scrut_600 = arena_call_601;
                        if ((scrut_600 != NULL && (scrut_600)->tag == mak_ir_tag_0)) {
                            case_599 = mak_ir_ext_2((const void *)("%.17g\n"), mak_local_ig_259);
                        }
                        else if (1) {
                            mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)("Error: unexpected characters\n"), mak_ir_ext_3((const void *)("Error: unexpected characters\n")));
                        }
                        discard_596 = case_599;
                    }
                    else {
                        discard_596 = 0;
                    }
                    continue;
                }
            }
            else if ((scrut_558 != NULL && (scrut_558)->tag == mak_ir_tag_1) && (scrut_558->v.v1.f0 == INT64_C(101))) {
                struct mak__a * mak_local_bm_261 = (scrut_558)->v.v1.f1;
                if ((mak_local_bm_261 != NULL && (mak_local_bm_261)->tag == mak_ir_tag_1) && (mak_local_bm_261->v.v1.f0 == INT64_C(120))) {
                    struct mak__a * mak_local_bn_262 = (mak_local_bm_261)->v.v1.f1;
                    if ((mak_local_bn_262 != NULL && (mak_local_bn_262)->tag == mak_ir_tag_1) && (mak_local_bn_262->v.v1.f0 == INT64_C(105))) {
                        struct mak__a * mak_local_bo_263 = (mak_local_bn_262)->v.v1.f1;
                        if ((mak_local_bo_263 != NULL && (mak_local_bo_263)->tag == mak_ir_tag_1) && (mak_local_bo_263->v.v1.f0 == INT64_C(116))) {
                            struct mak__a * mak_local_bp_264 = (mak_local_bo_263)->v.v1.f1;
                            struct mak__a *arena_call_603 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
                            if (!arena_call_603) {
                                mak_ir_recover("ADT result allocation failed");
                                arena_call_603 = &mak_ir_default_obj__a;
                             } else {
                                mak_ir_fn_2(mak_local_bp_264, (struct mak__a *)(arena_call_603));
                            }
                            struct mak__a * scrut_602 = arena_call_603;
                            if ((scrut_602 != NULL && (scrut_602)->tag == mak_ir_tag_0)) {
                                struct mak__b *arena_obj_604 = (struct mak__b *)mak_ir_arena_alloc(sizeof(struct mak__b), _Alignof(struct mak__b));
                                if (!arena_obj_604) {
                                    mak_ir_recover("ADT object allocation failed");
                                    arena_obj_604 = &mak_ir_default_obj__b;
                                 } else {
                                    (arena_obj_604)->tag = mak_ir_tag_3;
                                    (arena_obj_604)->v.v3.f0 = (struct mak__a *)mak_ir_adt_recover_ptr((const void *)&mak_ir_default_obj__a, "integer used as ADT");
                                }
                                return ((double)(uintptr_t)(arena_obj_604));
                            }
                            else if (1) {
                                struct mak__a *arena_call_606 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
                                if (!arena_call_606) {
                                    mak_ir_recover("ADT result allocation failed");
                                    arena_call_606 = &mak_ir_default_obj__a;
                                 } else {
                                    mak_ir_fn_2(mak_local_ii_243, (struct mak__a *)(arena_call_606));
                                }
                                struct mak__a * scrut_605 = arena_call_606;
                                if ((scrut_605 != NULL && (scrut_605)->tag == mak_ir_tag_1) && (scrut_605->v.v1.f0 == INT64_C(63))) {
                                    struct mak__a * mak_local_br_265 = (scrut_605)->v.v1.f1;
                                    struct mak__a *arena_call_608 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
                                    if (!arena_call_608) {
                                        mak_ir_recover("ADT result allocation failed");
                                        arena_call_608 = &mak_ir_default_obj__a;
                                     } else {
                                        mak_ir_fn_2(mak_local_br_265, (struct mak__a *)(arena_call_608));
                                    }
                                    struct mak__a * scrut_607 = arena_call_608;
                                    if ((scrut_607 != NULL && (scrut_607)->tag == mak_ir_tag_0)) {
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
                                        int32_t discard_609 = 0;
                                        struct mak__f *arena_call_611 = (struct mak__f *)mak_ir_arena_alloc(sizeof(struct mak__f), _Alignof(struct mak__f));
                                        if (!arena_call_611) {
                                            mak_ir_recover("ADT result allocation failed");
                                            arena_call_611 = &mak_ir_default_obj__f;
                                         } else {
                                            mak_ir_fn_16(mak_local_ii_243, (struct mak__f *)(arena_call_611));
                                        }
                                        struct mak__f * scrut_610 = arena_call_611;
                                        if ((scrut_610 != NULL && (scrut_610)->tag == mak_ir_tag_11)) {
                                            mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)("Error: invalid expression\n"), mak_ir_ext_3((const void *)("Error: invalid expression\n")));
                                        }
                                        else if ((scrut_610 != NULL && (scrut_610)->tag == mak_ir_tag_10)) {
                                            double mak_local_ig_266 = (scrut_610)->v.v10.f0;
                                            struct mak__a * mak_local_ih_267 = (scrut_610)->v.v10.f1;
                                            int32_t case_612 = 0;
                                            struct mak__a *arena_call_614 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
                                            if (!arena_call_614) {
                                                mak_ir_recover("ADT result allocation failed");
                                                arena_call_614 = &mak_ir_default_obj__a;
                                             } else {
                                                mak_ir_fn_2(mak_local_ih_267, (struct mak__a *)(arena_call_614));
                                            }
                                            struct mak__a * scrut_613 = arena_call_614;
                                            if ((scrut_613 != NULL && (scrut_613)->tag == mak_ir_tag_0)) {
                                                case_612 = mak_ir_ext_2((const void *)("%.17g\n"), mak_local_ig_266);
                                            }
                                            else if (1) {
                                                mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)("Error: unexpected characters\n"), mak_ir_ext_3((const void *)("Error: unexpected characters\n")));
                                            }
                                            discard_609 = case_612;
                                        }
                                        else {
                                            discard_609 = 0;
                                        }
                                        continue;
                                    }
                                }
                                else if ((scrut_605 != NULL && (scrut_605)->tag == mak_ir_tag_1) && (scrut_605->v.v1.f0 == INT64_C(104))) {
                                    struct mak__a * mak_local_bs_268 = (scrut_605)->v.v1.f1;
                                    if ((mak_local_bs_268 != NULL && (mak_local_bs_268)->tag == mak_ir_tag_1) && (mak_local_bs_268->v.v1.f0 == INT64_C(101))) {
                                        struct mak__a * mak_local_bt_269 = (mak_local_bs_268)->v.v1.f1;
                                        if ((mak_local_bt_269 != NULL && (mak_local_bt_269)->tag == mak_ir_tag_1) && (mak_local_bt_269->v.v1.f0 == INT64_C(108))) {
                                            struct mak__a * mak_local_bu_270 = (mak_local_bt_269)->v.v1.f1;
                                            if ((mak_local_bu_270 != NULL && (mak_local_bu_270)->tag == mak_ir_tag_1) && (mak_local_bu_270->v.v1.f0 == INT64_C(112))) {
                                                struct mak__a * mak_local_bv_271 = (mak_local_bu_270)->v.v1.f1;
                                                struct mak__a *arena_call_616 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
                                                if (!arena_call_616) {
                                                    mak_ir_recover("ADT result allocation failed");
                                                    arena_call_616 = &mak_ir_default_obj__a;
                                                 } else {
                                                    mak_ir_fn_2(mak_local_bv_271, (struct mak__a *)(arena_call_616));
                                                }
                                                struct mak__a * scrut_615 = arena_call_616;
                                                if ((scrut_615 != NULL && (scrut_615)->tag == mak_ir_tag_0)) {
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
                                                    int32_t discard_617 = 0;
                                                    struct mak__f *arena_call_619 = (struct mak__f *)mak_ir_arena_alloc(sizeof(struct mak__f), _Alignof(struct mak__f));
                                                    if (!arena_call_619) {
                                                        mak_ir_recover("ADT result allocation failed");
                                                        arena_call_619 = &mak_ir_default_obj__f;
                                                     } else {
                                                        mak_ir_fn_16(mak_local_ii_243, (struct mak__f *)(arena_call_619));
                                                    }
                                                    struct mak__f * scrut_618 = arena_call_619;
                                                    if ((scrut_618 != NULL && (scrut_618)->tag == mak_ir_tag_11)) {
                                                        mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)("Error: invalid expression\n"), mak_ir_ext_3((const void *)("Error: invalid expression\n")));
                                                    }
                                                    else if ((scrut_618 != NULL && (scrut_618)->tag == mak_ir_tag_10)) {
                                                        double mak_local_ig_272 = (scrut_618)->v.v10.f0;
                                                        struct mak__a * mak_local_ih_273 = (scrut_618)->v.v10.f1;
                                                        int32_t case_620 = 0;
                                                        struct mak__a *arena_call_622 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
                                                        if (!arena_call_622) {
                                                            mak_ir_recover("ADT result allocation failed");
                                                            arena_call_622 = &mak_ir_default_obj__a;
                                                         } else {
                                                            mak_ir_fn_2(mak_local_ih_273, (struct mak__a *)(arena_call_622));
                                                        }
                                                        struct mak__a * scrut_621 = arena_call_622;
                                                        if ((scrut_621 != NULL && (scrut_621)->tag == mak_ir_tag_0)) {
                                                            case_620 = mak_ir_ext_2((const void *)("%.17g\n"), mak_local_ig_272);
                                                        }
                                                        else if (1) {
                                                            mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)("Error: unexpected characters\n"), mak_ir_ext_3((const void *)("Error: unexpected characters\n")));
                                                        }
                                                        discard_617 = case_620;
                                                    }
                                                    else {
                                                        discard_617 = 0;
                                                    }
                                                    continue;
                                                }
                                            }
                                            else if (1) {
                                                int32_t discard_623 = 0;
                                                struct mak__f *arena_call_625 = (struct mak__f *)mak_ir_arena_alloc(sizeof(struct mak__f), _Alignof(struct mak__f));
                                                if (!arena_call_625) {
                                                    mak_ir_recover("ADT result allocation failed");
                                                    arena_call_625 = &mak_ir_default_obj__f;
                                                 } else {
                                                    mak_ir_fn_16(mak_local_ii_243, (struct mak__f *)(arena_call_625));
                                                }
                                                struct mak__f * scrut_624 = arena_call_625;
                                                if ((scrut_624 != NULL && (scrut_624)->tag == mak_ir_tag_11)) {
                                                    mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)("Error: invalid expression\n"), mak_ir_ext_3((const void *)("Error: invalid expression\n")));
                                                }
                                                else if ((scrut_624 != NULL && (scrut_624)->tag == mak_ir_tag_10)) {
                                                    double mak_local_ig_274 = (scrut_624)->v.v10.f0;
                                                    struct mak__a * mak_local_ih_275 = (scrut_624)->v.v10.f1;
                                                    int32_t case_626 = 0;
                                                    struct mak__a *arena_call_628 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
                                                    if (!arena_call_628) {
                                                        mak_ir_recover("ADT result allocation failed");
                                                        arena_call_628 = &mak_ir_default_obj__a;
                                                     } else {
                                                        mak_ir_fn_2(mak_local_ih_275, (struct mak__a *)(arena_call_628));
                                                    }
                                                    struct mak__a * scrut_627 = arena_call_628;
                                                    if ((scrut_627 != NULL && (scrut_627)->tag == mak_ir_tag_0)) {
                                                        case_626 = mak_ir_ext_2((const void *)("%.17g\n"), mak_local_ig_274);
                                                    }
                                                    else if (1) {
                                                        mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)("Error: unexpected characters\n"), mak_ir_ext_3((const void *)("Error: unexpected characters\n")));
                                                    }
                                                    discard_623 = case_626;
                                                }
                                                else {
                                                    discard_623 = 0;
                                                }
                                                continue;
                                            }
                                        }
                                        else if (1) {
                                            int32_t discard_629 = 0;
                                            struct mak__f *arena_call_631 = (struct mak__f *)mak_ir_arena_alloc(sizeof(struct mak__f), _Alignof(struct mak__f));
                                            if (!arena_call_631) {
                                                mak_ir_recover("ADT result allocation failed");
                                                arena_call_631 = &mak_ir_default_obj__f;
                                             } else {
                                                mak_ir_fn_16(mak_local_ii_243, (struct mak__f *)(arena_call_631));
                                            }
                                            struct mak__f * scrut_630 = arena_call_631;
                                            if ((scrut_630 != NULL && (scrut_630)->tag == mak_ir_tag_11)) {
                                                mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)("Error: invalid expression\n"), mak_ir_ext_3((const void *)("Error: invalid expression\n")));
                                            }
                                            else if ((scrut_630 != NULL && (scrut_630)->tag == mak_ir_tag_10)) {
                                                double mak_local_ig_276 = (scrut_630)->v.v10.f0;
                                                struct mak__a * mak_local_ih_277 = (scrut_630)->v.v10.f1;
                                                int32_t case_632 = 0;
                                                struct mak__a *arena_call_634 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
                                                if (!arena_call_634) {
                                                    mak_ir_recover("ADT result allocation failed");
                                                    arena_call_634 = &mak_ir_default_obj__a;
                                                 } else {
                                                    mak_ir_fn_2(mak_local_ih_277, (struct mak__a *)(arena_call_634));
                                                }
                                                struct mak__a * scrut_633 = arena_call_634;
                                                if ((scrut_633 != NULL && (scrut_633)->tag == mak_ir_tag_0)) {
                                                    case_632 = mak_ir_ext_2((const void *)("%.17g\n"), mak_local_ig_276);
                                                }
                                                else if (1) {
                                                    mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)("Error: unexpected characters\n"), mak_ir_ext_3((const void *)("Error: unexpected characters\n")));
                                                }
                                                discard_629 = case_632;
                                            }
                                            else {
                                                discard_629 = 0;
                                            }
                                            continue;
                                        }
                                    }
                                    else if (1) {
                                        int32_t discard_635 = 0;
                                        struct mak__f *arena_call_637 = (struct mak__f *)mak_ir_arena_alloc(sizeof(struct mak__f), _Alignof(struct mak__f));
                                        if (!arena_call_637) {
                                            mak_ir_recover("ADT result allocation failed");
                                            arena_call_637 = &mak_ir_default_obj__f;
                                         } else {
                                            mak_ir_fn_16(mak_local_ii_243, (struct mak__f *)(arena_call_637));
                                        }
                                        struct mak__f * scrut_636 = arena_call_637;
                                        if ((scrut_636 != NULL && (scrut_636)->tag == mak_ir_tag_11)) {
                                            mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)("Error: invalid expression\n"), mak_ir_ext_3((const void *)("Error: invalid expression\n")));
                                        }
                                        else if ((scrut_636 != NULL && (scrut_636)->tag == mak_ir_tag_10)) {
                                            double mak_local_ig_278 = (scrut_636)->v.v10.f0;
                                            struct mak__a * mak_local_ih_279 = (scrut_636)->v.v10.f1;
                                            int32_t case_638 = 0;
                                            struct mak__a *arena_call_640 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
                                            if (!arena_call_640) {
                                                mak_ir_recover("ADT result allocation failed");
                                                arena_call_640 = &mak_ir_default_obj__a;
                                             } else {
                                                mak_ir_fn_2(mak_local_ih_279, (struct mak__a *)(arena_call_640));
                                            }
                                            struct mak__a * scrut_639 = arena_call_640;
                                            if ((scrut_639 != NULL && (scrut_639)->tag == mak_ir_tag_0)) {
                                                case_638 = mak_ir_ext_2((const void *)("%.17g\n"), mak_local_ig_278);
                                            }
                                            else if (1) {
                                                mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)("Error: unexpected characters\n"), mak_ir_ext_3((const void *)("Error: unexpected characters\n")));
                                            }
                                            discard_635 = case_638;
                                        }
                                        else {
                                            discard_635 = 0;
                                        }
                                        continue;
                                    }
                                }
                                else if (1) {
                                    int32_t discard_641 = 0;
                                    struct mak__f *arena_call_643 = (struct mak__f *)mak_ir_arena_alloc(sizeof(struct mak__f), _Alignof(struct mak__f));
                                    if (!arena_call_643) {
                                        mak_ir_recover("ADT result allocation failed");
                                        arena_call_643 = &mak_ir_default_obj__f;
                                     } else {
                                        mak_ir_fn_16(mak_local_ii_243, (struct mak__f *)(arena_call_643));
                                    }
                                    struct mak__f * scrut_642 = arena_call_643;
                                    if ((scrut_642 != NULL && (scrut_642)->tag == mak_ir_tag_11)) {
                                        mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)("Error: invalid expression\n"), mak_ir_ext_3((const void *)("Error: invalid expression\n")));
                                    }
                                    else if ((scrut_642 != NULL && (scrut_642)->tag == mak_ir_tag_10)) {
                                        double mak_local_ig_280 = (scrut_642)->v.v10.f0;
                                        struct mak__a * mak_local_ih_281 = (scrut_642)->v.v10.f1;
                                        int32_t case_644 = 0;
                                        struct mak__a *arena_call_646 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
                                        if (!arena_call_646) {
                                            mak_ir_recover("ADT result allocation failed");
                                            arena_call_646 = &mak_ir_default_obj__a;
                                         } else {
                                            mak_ir_fn_2(mak_local_ih_281, (struct mak__a *)(arena_call_646));
                                        }
                                        struct mak__a * scrut_645 = arena_call_646;
                                        if ((scrut_645 != NULL && (scrut_645)->tag == mak_ir_tag_0)) {
                                            case_644 = mak_ir_ext_2((const void *)("%.17g\n"), mak_local_ig_280);
                                        }
                                        else if (1) {
                                            mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)("Error: unexpected characters\n"), mak_ir_ext_3((const void *)("Error: unexpected characters\n")));
                                        }
                                        discard_641 = case_644;
                                    }
                                    else {
                                        discard_641 = 0;
                                    }
                                    continue;
                                }
                            }
                        }
                        else if (1) {
                            struct mak__a *arena_call_648 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
                            if (!arena_call_648) {
                                mak_ir_recover("ADT result allocation failed");
                                arena_call_648 = &mak_ir_default_obj__a;
                             } else {
                                mak_ir_fn_2(mak_local_ii_243, (struct mak__a *)(arena_call_648));
                            }
                            struct mak__a * scrut_647 = arena_call_648;
                            if ((scrut_647 != NULL && (scrut_647)->tag == mak_ir_tag_1) && (scrut_647->v.v1.f0 == INT64_C(63))) {
                                struct mak__a * mak_local_br_282 = (scrut_647)->v.v1.f1;
                                struct mak__a *arena_call_650 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
                                if (!arena_call_650) {
                                    mak_ir_recover("ADT result allocation failed");
                                    arena_call_650 = &mak_ir_default_obj__a;
                                 } else {
                                    mak_ir_fn_2(mak_local_br_282, (struct mak__a *)(arena_call_650));
                                }
                                struct mak__a * scrut_649 = arena_call_650;
                                if ((scrut_649 != NULL && (scrut_649)->tag == mak_ir_tag_0)) {
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
                                    int32_t discard_651 = 0;
                                    struct mak__f *arena_call_653 = (struct mak__f *)mak_ir_arena_alloc(sizeof(struct mak__f), _Alignof(struct mak__f));
                                    if (!arena_call_653) {
                                        mak_ir_recover("ADT result allocation failed");
                                        arena_call_653 = &mak_ir_default_obj__f;
                                     } else {
                                        mak_ir_fn_16(mak_local_ii_243, (struct mak__f *)(arena_call_653));
                                    }
                                    struct mak__f * scrut_652 = arena_call_653;
                                    if ((scrut_652 != NULL && (scrut_652)->tag == mak_ir_tag_11)) {
                                        mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)("Error: invalid expression\n"), mak_ir_ext_3((const void *)("Error: invalid expression\n")));
                                    }
                                    else if ((scrut_652 != NULL && (scrut_652)->tag == mak_ir_tag_10)) {
                                        double mak_local_ig_283 = (scrut_652)->v.v10.f0;
                                        struct mak__a * mak_local_ih_284 = (scrut_652)->v.v10.f1;
                                        int32_t case_654 = 0;
                                        struct mak__a *arena_call_656 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
                                        if (!arena_call_656) {
                                            mak_ir_recover("ADT result allocation failed");
                                            arena_call_656 = &mak_ir_default_obj__a;
                                         } else {
                                            mak_ir_fn_2(mak_local_ih_284, (struct mak__a *)(arena_call_656));
                                        }
                                        struct mak__a * scrut_655 = arena_call_656;
                                        if ((scrut_655 != NULL && (scrut_655)->tag == mak_ir_tag_0)) {
                                            case_654 = mak_ir_ext_2((const void *)("%.17g\n"), mak_local_ig_283);
                                        }
                                        else if (1) {
                                            mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)("Error: unexpected characters\n"), mak_ir_ext_3((const void *)("Error: unexpected characters\n")));
                                        }
                                        discard_651 = case_654;
                                    }
                                    else {
                                        discard_651 = 0;
                                    }
                                    continue;
                                }
                            }
                            else if ((scrut_647 != NULL && (scrut_647)->tag == mak_ir_tag_1) && (scrut_647->v.v1.f0 == INT64_C(104))) {
                                struct mak__a * mak_local_bs_285 = (scrut_647)->v.v1.f1;
                                if ((mak_local_bs_285 != NULL && (mak_local_bs_285)->tag == mak_ir_tag_1) && (mak_local_bs_285->v.v1.f0 == INT64_C(101))) {
                                    struct mak__a * mak_local_bt_286 = (mak_local_bs_285)->v.v1.f1;
                                    if ((mak_local_bt_286 != NULL && (mak_local_bt_286)->tag == mak_ir_tag_1) && (mak_local_bt_286->v.v1.f0 == INT64_C(108))) {
                                        struct mak__a * mak_local_bu_287 = (mak_local_bt_286)->v.v1.f1;
                                        if ((mak_local_bu_287 != NULL && (mak_local_bu_287)->tag == mak_ir_tag_1) && (mak_local_bu_287->v.v1.f0 == INT64_C(112))) {
                                            struct mak__a * mak_local_bv_288 = (mak_local_bu_287)->v.v1.f1;
                                            struct mak__a *arena_call_658 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
                                            if (!arena_call_658) {
                                                mak_ir_recover("ADT result allocation failed");
                                                arena_call_658 = &mak_ir_default_obj__a;
                                             } else {
                                                mak_ir_fn_2(mak_local_bv_288, (struct mak__a *)(arena_call_658));
                                            }
                                            struct mak__a * scrut_657 = arena_call_658;
                                            if ((scrut_657 != NULL && (scrut_657)->tag == mak_ir_tag_0)) {
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
                                                int32_t discard_659 = 0;
                                                struct mak__f *arena_call_661 = (struct mak__f *)mak_ir_arena_alloc(sizeof(struct mak__f), _Alignof(struct mak__f));
                                                if (!arena_call_661) {
                                                    mak_ir_recover("ADT result allocation failed");
                                                    arena_call_661 = &mak_ir_default_obj__f;
                                                 } else {
                                                    mak_ir_fn_16(mak_local_ii_243, (struct mak__f *)(arena_call_661));
                                                }
                                                struct mak__f * scrut_660 = arena_call_661;
                                                if ((scrut_660 != NULL && (scrut_660)->tag == mak_ir_tag_11)) {
                                                    mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)("Error: invalid expression\n"), mak_ir_ext_3((const void *)("Error: invalid expression\n")));
                                                }
                                                else if ((scrut_660 != NULL && (scrut_660)->tag == mak_ir_tag_10)) {
                                                    double mak_local_ig_289 = (scrut_660)->v.v10.f0;
                                                    struct mak__a * mak_local_ih_290 = (scrut_660)->v.v10.f1;
                                                    int32_t case_662 = 0;
                                                    struct mak__a *arena_call_664 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
                                                    if (!arena_call_664) {
                                                        mak_ir_recover("ADT result allocation failed");
                                                        arena_call_664 = &mak_ir_default_obj__a;
                                                     } else {
                                                        mak_ir_fn_2(mak_local_ih_290, (struct mak__a *)(arena_call_664));
                                                    }
                                                    struct mak__a * scrut_663 = arena_call_664;
                                                    if ((scrut_663 != NULL && (scrut_663)->tag == mak_ir_tag_0)) {
                                                        case_662 = mak_ir_ext_2((const void *)("%.17g\n"), mak_local_ig_289);
                                                    }
                                                    else if (1) {
                                                        mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)("Error: unexpected characters\n"), mak_ir_ext_3((const void *)("Error: unexpected characters\n")));
                                                    }
                                                    discard_659 = case_662;
                                                }
                                                else {
                                                    discard_659 = 0;
                                                }
                                                continue;
                                            }
                                        }
                                        else if (1) {
                                            int32_t discard_665 = 0;
                                            struct mak__f *arena_call_667 = (struct mak__f *)mak_ir_arena_alloc(sizeof(struct mak__f), _Alignof(struct mak__f));
                                            if (!arena_call_667) {
                                                mak_ir_recover("ADT result allocation failed");
                                                arena_call_667 = &mak_ir_default_obj__f;
                                             } else {
                                                mak_ir_fn_16(mak_local_ii_243, (struct mak__f *)(arena_call_667));
                                            }
                                            struct mak__f * scrut_666 = arena_call_667;
                                            if ((scrut_666 != NULL && (scrut_666)->tag == mak_ir_tag_11)) {
                                                mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)("Error: invalid expression\n"), mak_ir_ext_3((const void *)("Error: invalid expression\n")));
                                            }
                                            else if ((scrut_666 != NULL && (scrut_666)->tag == mak_ir_tag_10)) {
                                                double mak_local_ig_291 = (scrut_666)->v.v10.f0;
                                                struct mak__a * mak_local_ih_292 = (scrut_666)->v.v10.f1;
                                                int32_t case_668 = 0;
                                                struct mak__a *arena_call_670 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
                                                if (!arena_call_670) {
                                                    mak_ir_recover("ADT result allocation failed");
                                                    arena_call_670 = &mak_ir_default_obj__a;
                                                 } else {
                                                    mak_ir_fn_2(mak_local_ih_292, (struct mak__a *)(arena_call_670));
                                                }
                                                struct mak__a * scrut_669 = arena_call_670;
                                                if ((scrut_669 != NULL && (scrut_669)->tag == mak_ir_tag_0)) {
                                                    case_668 = mak_ir_ext_2((const void *)("%.17g\n"), mak_local_ig_291);
                                                }
                                                else if (1) {
                                                    mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)("Error: unexpected characters\n"), mak_ir_ext_3((const void *)("Error: unexpected characters\n")));
                                                }
                                                discard_665 = case_668;
                                            }
                                            else {
                                                discard_665 = 0;
                                            }
                                            continue;
                                        }
                                    }
                                    else if (1) {
                                        int32_t discard_671 = 0;
                                        struct mak__f *arena_call_673 = (struct mak__f *)mak_ir_arena_alloc(sizeof(struct mak__f), _Alignof(struct mak__f));
                                        if (!arena_call_673) {
                                            mak_ir_recover("ADT result allocation failed");
                                            arena_call_673 = &mak_ir_default_obj__f;
                                         } else {
                                            mak_ir_fn_16(mak_local_ii_243, (struct mak__f *)(arena_call_673));
                                        }
                                        struct mak__f * scrut_672 = arena_call_673;
                                        if ((scrut_672 != NULL && (scrut_672)->tag == mak_ir_tag_11)) {
                                            mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)("Error: invalid expression\n"), mak_ir_ext_3((const void *)("Error: invalid expression\n")));
                                        }
                                        else if ((scrut_672 != NULL && (scrut_672)->tag == mak_ir_tag_10)) {
                                            double mak_local_ig_293 = (scrut_672)->v.v10.f0;
                                            struct mak__a * mak_local_ih_294 = (scrut_672)->v.v10.f1;
                                            int32_t case_674 = 0;
                                            struct mak__a *arena_call_676 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
                                            if (!arena_call_676) {
                                                mak_ir_recover("ADT result allocation failed");
                                                arena_call_676 = &mak_ir_default_obj__a;
                                             } else {
                                                mak_ir_fn_2(mak_local_ih_294, (struct mak__a *)(arena_call_676));
                                            }
                                            struct mak__a * scrut_675 = arena_call_676;
                                            if ((scrut_675 != NULL && (scrut_675)->tag == mak_ir_tag_0)) {
                                                case_674 = mak_ir_ext_2((const void *)("%.17g\n"), mak_local_ig_293);
                                            }
                                            else if (1) {
                                                mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)("Error: unexpected characters\n"), mak_ir_ext_3((const void *)("Error: unexpected characters\n")));
                                            }
                                            discard_671 = case_674;
                                        }
                                        else {
                                            discard_671 = 0;
                                        }
                                        continue;
                                    }
                                }
                                else if (1) {
                                    int32_t discard_677 = 0;
                                    struct mak__f *arena_call_679 = (struct mak__f *)mak_ir_arena_alloc(sizeof(struct mak__f), _Alignof(struct mak__f));
                                    if (!arena_call_679) {
                                        mak_ir_recover("ADT result allocation failed");
                                        arena_call_679 = &mak_ir_default_obj__f;
                                     } else {
                                        mak_ir_fn_16(mak_local_ii_243, (struct mak__f *)(arena_call_679));
                                    }
                                    struct mak__f * scrut_678 = arena_call_679;
                                    if ((scrut_678 != NULL && (scrut_678)->tag == mak_ir_tag_11)) {
                                        mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)("Error: invalid expression\n"), mak_ir_ext_3((const void *)("Error: invalid expression\n")));
                                    }
                                    else if ((scrut_678 != NULL && (scrut_678)->tag == mak_ir_tag_10)) {
                                        double mak_local_ig_295 = (scrut_678)->v.v10.f0;
                                        struct mak__a * mak_local_ih_296 = (scrut_678)->v.v10.f1;
                                        int32_t case_680 = 0;
                                        struct mak__a *arena_call_682 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
                                        if (!arena_call_682) {
                                            mak_ir_recover("ADT result allocation failed");
                                            arena_call_682 = &mak_ir_default_obj__a;
                                         } else {
                                            mak_ir_fn_2(mak_local_ih_296, (struct mak__a *)(arena_call_682));
                                        }
                                        struct mak__a * scrut_681 = arena_call_682;
                                        if ((scrut_681 != NULL && (scrut_681)->tag == mak_ir_tag_0)) {
                                            case_680 = mak_ir_ext_2((const void *)("%.17g\n"), mak_local_ig_295);
                                        }
                                        else if (1) {
                                            mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)("Error: unexpected characters\n"), mak_ir_ext_3((const void *)("Error: unexpected characters\n")));
                                        }
                                        discard_677 = case_680;
                                    }
                                    else {
                                        discard_677 = 0;
                                    }
                                    continue;
                                }
                            }
                            else if (1) {
                                int32_t discard_683 = 0;
                                struct mak__f *arena_call_685 = (struct mak__f *)mak_ir_arena_alloc(sizeof(struct mak__f), _Alignof(struct mak__f));
                                if (!arena_call_685) {
                                    mak_ir_recover("ADT result allocation failed");
                                    arena_call_685 = &mak_ir_default_obj__f;
                                 } else {
                                    mak_ir_fn_16(mak_local_ii_243, (struct mak__f *)(arena_call_685));
                                }
                                struct mak__f * scrut_684 = arena_call_685;
                                if ((scrut_684 != NULL && (scrut_684)->tag == mak_ir_tag_11)) {
                                    mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)("Error: invalid expression\n"), mak_ir_ext_3((const void *)("Error: invalid expression\n")));
                                }
                                else if ((scrut_684 != NULL && (scrut_684)->tag == mak_ir_tag_10)) {
                                    double mak_local_ig_297 = (scrut_684)->v.v10.f0;
                                    struct mak__a * mak_local_ih_298 = (scrut_684)->v.v10.f1;
                                    int32_t case_686 = 0;
                                    struct mak__a *arena_call_688 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
                                    if (!arena_call_688) {
                                        mak_ir_recover("ADT result allocation failed");
                                        arena_call_688 = &mak_ir_default_obj__a;
                                     } else {
                                        mak_ir_fn_2(mak_local_ih_298, (struct mak__a *)(arena_call_688));
                                    }
                                    struct mak__a * scrut_687 = arena_call_688;
                                    if ((scrut_687 != NULL && (scrut_687)->tag == mak_ir_tag_0)) {
                                        case_686 = mak_ir_ext_2((const void *)("%.17g\n"), mak_local_ig_297);
                                    }
                                    else if (1) {
                                        mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)("Error: unexpected characters\n"), mak_ir_ext_3((const void *)("Error: unexpected characters\n")));
                                    }
                                    discard_683 = case_686;
                                }
                                else {
                                    discard_683 = 0;
                                }
                                continue;
                            }
                        }
                    }
                    else if (1) {
                        struct mak__a *arena_call_690 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
                        if (!arena_call_690) {
                            mak_ir_recover("ADT result allocation failed");
                            arena_call_690 = &mak_ir_default_obj__a;
                         } else {
                            mak_ir_fn_2(mak_local_ii_243, (struct mak__a *)(arena_call_690));
                        }
                        struct mak__a * scrut_689 = arena_call_690;
                        if ((scrut_689 != NULL && (scrut_689)->tag == mak_ir_tag_1) && (scrut_689->v.v1.f0 == INT64_C(63))) {
                            struct mak__a * mak_local_br_299 = (scrut_689)->v.v1.f1;
                            struct mak__a *arena_call_692 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
                            if (!arena_call_692) {
                                mak_ir_recover("ADT result allocation failed");
                                arena_call_692 = &mak_ir_default_obj__a;
                             } else {
                                mak_ir_fn_2(mak_local_br_299, (struct mak__a *)(arena_call_692));
                            }
                            struct mak__a * scrut_691 = arena_call_692;
                            if ((scrut_691 != NULL && (scrut_691)->tag == mak_ir_tag_0)) {
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
                                int32_t discard_693 = 0;
                                struct mak__f *arena_call_695 = (struct mak__f *)mak_ir_arena_alloc(sizeof(struct mak__f), _Alignof(struct mak__f));
                                if (!arena_call_695) {
                                    mak_ir_recover("ADT result allocation failed");
                                    arena_call_695 = &mak_ir_default_obj__f;
                                 } else {
                                    mak_ir_fn_16(mak_local_ii_243, (struct mak__f *)(arena_call_695));
                                }
                                struct mak__f * scrut_694 = arena_call_695;
                                if ((scrut_694 != NULL && (scrut_694)->tag == mak_ir_tag_11)) {
                                    mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)("Error: invalid expression\n"), mak_ir_ext_3((const void *)("Error: invalid expression\n")));
                                }
                                else if ((scrut_694 != NULL && (scrut_694)->tag == mak_ir_tag_10)) {
                                    double mak_local_ig_300 = (scrut_694)->v.v10.f0;
                                    struct mak__a * mak_local_ih_301 = (scrut_694)->v.v10.f1;
                                    int32_t case_696 = 0;
                                    struct mak__a *arena_call_698 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
                                    if (!arena_call_698) {
                                        mak_ir_recover("ADT result allocation failed");
                                        arena_call_698 = &mak_ir_default_obj__a;
                                     } else {
                                        mak_ir_fn_2(mak_local_ih_301, (struct mak__a *)(arena_call_698));
                                    }
                                    struct mak__a * scrut_697 = arena_call_698;
                                    if ((scrut_697 != NULL && (scrut_697)->tag == mak_ir_tag_0)) {
                                        case_696 = mak_ir_ext_2((const void *)("%.17g\n"), mak_local_ig_300);
                                    }
                                    else if (1) {
                                        mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)("Error: unexpected characters\n"), mak_ir_ext_3((const void *)("Error: unexpected characters\n")));
                                    }
                                    discard_693 = case_696;
                                }
                                else {
                                    discard_693 = 0;
                                }
                                continue;
                            }
                        }
                        else if ((scrut_689 != NULL && (scrut_689)->tag == mak_ir_tag_1) && (scrut_689->v.v1.f0 == INT64_C(104))) {
                            struct mak__a * mak_local_bs_302 = (scrut_689)->v.v1.f1;
                            if ((mak_local_bs_302 != NULL && (mak_local_bs_302)->tag == mak_ir_tag_1) && (mak_local_bs_302->v.v1.f0 == INT64_C(101))) {
                                struct mak__a * mak_local_bt_303 = (mak_local_bs_302)->v.v1.f1;
                                if ((mak_local_bt_303 != NULL && (mak_local_bt_303)->tag == mak_ir_tag_1) && (mak_local_bt_303->v.v1.f0 == INT64_C(108))) {
                                    struct mak__a * mak_local_bu_304 = (mak_local_bt_303)->v.v1.f1;
                                    if ((mak_local_bu_304 != NULL && (mak_local_bu_304)->tag == mak_ir_tag_1) && (mak_local_bu_304->v.v1.f0 == INT64_C(112))) {
                                        struct mak__a * mak_local_bv_305 = (mak_local_bu_304)->v.v1.f1;
                                        struct mak__a *arena_call_700 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
                                        if (!arena_call_700) {
                                            mak_ir_recover("ADT result allocation failed");
                                            arena_call_700 = &mak_ir_default_obj__a;
                                         } else {
                                            mak_ir_fn_2(mak_local_bv_305, (struct mak__a *)(arena_call_700));
                                        }
                                        struct mak__a * scrut_699 = arena_call_700;
                                        if ((scrut_699 != NULL && (scrut_699)->tag == mak_ir_tag_0)) {
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
                                            int32_t discard_701 = 0;
                                            struct mak__f *arena_call_703 = (struct mak__f *)mak_ir_arena_alloc(sizeof(struct mak__f), _Alignof(struct mak__f));
                                            if (!arena_call_703) {
                                                mak_ir_recover("ADT result allocation failed");
                                                arena_call_703 = &mak_ir_default_obj__f;
                                             } else {
                                                mak_ir_fn_16(mak_local_ii_243, (struct mak__f *)(arena_call_703));
                                            }
                                            struct mak__f * scrut_702 = arena_call_703;
                                            if ((scrut_702 != NULL && (scrut_702)->tag == mak_ir_tag_11)) {
                                                mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)("Error: invalid expression\n"), mak_ir_ext_3((const void *)("Error: invalid expression\n")));
                                            }
                                            else if ((scrut_702 != NULL && (scrut_702)->tag == mak_ir_tag_10)) {
                                                double mak_local_ig_306 = (scrut_702)->v.v10.f0;
                                                struct mak__a * mak_local_ih_307 = (scrut_702)->v.v10.f1;
                                                int32_t case_704 = 0;
                                                struct mak__a *arena_call_706 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
                                                if (!arena_call_706) {
                                                    mak_ir_recover("ADT result allocation failed");
                                                    arena_call_706 = &mak_ir_default_obj__a;
                                                 } else {
                                                    mak_ir_fn_2(mak_local_ih_307, (struct mak__a *)(arena_call_706));
                                                }
                                                struct mak__a * scrut_705 = arena_call_706;
                                                if ((scrut_705 != NULL && (scrut_705)->tag == mak_ir_tag_0)) {
                                                    case_704 = mak_ir_ext_2((const void *)("%.17g\n"), mak_local_ig_306);
                                                }
                                                else if (1) {
                                                    mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)("Error: unexpected characters\n"), mak_ir_ext_3((const void *)("Error: unexpected characters\n")));
                                                }
                                                discard_701 = case_704;
                                            }
                                            else {
                                                discard_701 = 0;
                                            }
                                            continue;
                                        }
                                    }
                                    else if (1) {
                                        int32_t discard_707 = 0;
                                        struct mak__f *arena_call_709 = (struct mak__f *)mak_ir_arena_alloc(sizeof(struct mak__f), _Alignof(struct mak__f));
                                        if (!arena_call_709) {
                                            mak_ir_recover("ADT result allocation failed");
                                            arena_call_709 = &mak_ir_default_obj__f;
                                         } else {
                                            mak_ir_fn_16(mak_local_ii_243, (struct mak__f *)(arena_call_709));
                                        }
                                        struct mak__f * scrut_708 = arena_call_709;
                                        if ((scrut_708 != NULL && (scrut_708)->tag == mak_ir_tag_11)) {
                                            mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)("Error: invalid expression\n"), mak_ir_ext_3((const void *)("Error: invalid expression\n")));
                                        }
                                        else if ((scrut_708 != NULL && (scrut_708)->tag == mak_ir_tag_10)) {
                                            double mak_local_ig_308 = (scrut_708)->v.v10.f0;
                                            struct mak__a * mak_local_ih_309 = (scrut_708)->v.v10.f1;
                                            int32_t case_710 = 0;
                                            struct mak__a *arena_call_712 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
                                            if (!arena_call_712) {
                                                mak_ir_recover("ADT result allocation failed");
                                                arena_call_712 = &mak_ir_default_obj__a;
                                             } else {
                                                mak_ir_fn_2(mak_local_ih_309, (struct mak__a *)(arena_call_712));
                                            }
                                            struct mak__a * scrut_711 = arena_call_712;
                                            if ((scrut_711 != NULL && (scrut_711)->tag == mak_ir_tag_0)) {
                                                case_710 = mak_ir_ext_2((const void *)("%.17g\n"), mak_local_ig_308);
                                            }
                                            else if (1) {
                                                mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)("Error: unexpected characters\n"), mak_ir_ext_3((const void *)("Error: unexpected characters\n")));
                                            }
                                            discard_707 = case_710;
                                        }
                                        else {
                                            discard_707 = 0;
                                        }
                                        continue;
                                    }
                                }
                                else if (1) {
                                    int32_t discard_713 = 0;
                                    struct mak__f *arena_call_715 = (struct mak__f *)mak_ir_arena_alloc(sizeof(struct mak__f), _Alignof(struct mak__f));
                                    if (!arena_call_715) {
                                        mak_ir_recover("ADT result allocation failed");
                                        arena_call_715 = &mak_ir_default_obj__f;
                                     } else {
                                        mak_ir_fn_16(mak_local_ii_243, (struct mak__f *)(arena_call_715));
                                    }
                                    struct mak__f * scrut_714 = arena_call_715;
                                    if ((scrut_714 != NULL && (scrut_714)->tag == mak_ir_tag_11)) {
                                        mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)("Error: invalid expression\n"), mak_ir_ext_3((const void *)("Error: invalid expression\n")));
                                    }
                                    else if ((scrut_714 != NULL && (scrut_714)->tag == mak_ir_tag_10)) {
                                        double mak_local_ig_310 = (scrut_714)->v.v10.f0;
                                        struct mak__a * mak_local_ih_311 = (scrut_714)->v.v10.f1;
                                        int32_t case_716 = 0;
                                        struct mak__a *arena_call_718 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
                                        if (!arena_call_718) {
                                            mak_ir_recover("ADT result allocation failed");
                                            arena_call_718 = &mak_ir_default_obj__a;
                                         } else {
                                            mak_ir_fn_2(mak_local_ih_311, (struct mak__a *)(arena_call_718));
                                        }
                                        struct mak__a * scrut_717 = arena_call_718;
                                        if ((scrut_717 != NULL && (scrut_717)->tag == mak_ir_tag_0)) {
                                            case_716 = mak_ir_ext_2((const void *)("%.17g\n"), mak_local_ig_310);
                                        }
                                        else if (1) {
                                            mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)("Error: unexpected characters\n"), mak_ir_ext_3((const void *)("Error: unexpected characters\n")));
                                        }
                                        discard_713 = case_716;
                                    }
                                    else {
                                        discard_713 = 0;
                                    }
                                    continue;
                                }
                            }
                            else if (1) {
                                int32_t discard_719 = 0;
                                struct mak__f *arena_call_721 = (struct mak__f *)mak_ir_arena_alloc(sizeof(struct mak__f), _Alignof(struct mak__f));
                                if (!arena_call_721) {
                                    mak_ir_recover("ADT result allocation failed");
                                    arena_call_721 = &mak_ir_default_obj__f;
                                 } else {
                                    mak_ir_fn_16(mak_local_ii_243, (struct mak__f *)(arena_call_721));
                                }
                                struct mak__f * scrut_720 = arena_call_721;
                                if ((scrut_720 != NULL && (scrut_720)->tag == mak_ir_tag_11)) {
                                    mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)("Error: invalid expression\n"), mak_ir_ext_3((const void *)("Error: invalid expression\n")));
                                }
                                else if ((scrut_720 != NULL && (scrut_720)->tag == mak_ir_tag_10)) {
                                    double mak_local_ig_312 = (scrut_720)->v.v10.f0;
                                    struct mak__a * mak_local_ih_313 = (scrut_720)->v.v10.f1;
                                    int32_t case_722 = 0;
                                    struct mak__a *arena_call_724 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
                                    if (!arena_call_724) {
                                        mak_ir_recover("ADT result allocation failed");
                                        arena_call_724 = &mak_ir_default_obj__a;
                                     } else {
                                        mak_ir_fn_2(mak_local_ih_313, (struct mak__a *)(arena_call_724));
                                    }
                                    struct mak__a * scrut_723 = arena_call_724;
                                    if ((scrut_723 != NULL && (scrut_723)->tag == mak_ir_tag_0)) {
                                        case_722 = mak_ir_ext_2((const void *)("%.17g\n"), mak_local_ig_312);
                                    }
                                    else if (1) {
                                        mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)("Error: unexpected characters\n"), mak_ir_ext_3((const void *)("Error: unexpected characters\n")));
                                    }
                                    discard_719 = case_722;
                                }
                                else {
                                    discard_719 = 0;
                                }
                                continue;
                            }
                        }
                        else if (1) {
                            int32_t discard_725 = 0;
                            struct mak__f *arena_call_727 = (struct mak__f *)mak_ir_arena_alloc(sizeof(struct mak__f), _Alignof(struct mak__f));
                            if (!arena_call_727) {
                                mak_ir_recover("ADT result allocation failed");
                                arena_call_727 = &mak_ir_default_obj__f;
                             } else {
                                mak_ir_fn_16(mak_local_ii_243, (struct mak__f *)(arena_call_727));
                            }
                            struct mak__f * scrut_726 = arena_call_727;
                            if ((scrut_726 != NULL && (scrut_726)->tag == mak_ir_tag_11)) {
                                mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)("Error: invalid expression\n"), mak_ir_ext_3((const void *)("Error: invalid expression\n")));
                            }
                            else if ((scrut_726 != NULL && (scrut_726)->tag == mak_ir_tag_10)) {
                                double mak_local_ig_314 = (scrut_726)->v.v10.f0;
                                struct mak__a * mak_local_ih_315 = (scrut_726)->v.v10.f1;
                                int32_t case_728 = 0;
                                struct mak__a *arena_call_730 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
                                if (!arena_call_730) {
                                    mak_ir_recover("ADT result allocation failed");
                                    arena_call_730 = &mak_ir_default_obj__a;
                                 } else {
                                    mak_ir_fn_2(mak_local_ih_315, (struct mak__a *)(arena_call_730));
                                }
                                struct mak__a * scrut_729 = arena_call_730;
                                if ((scrut_729 != NULL && (scrut_729)->tag == mak_ir_tag_0)) {
                                    case_728 = mak_ir_ext_2((const void *)("%.17g\n"), mak_local_ig_314);
                                }
                                else if (1) {
                                    mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)("Error: unexpected characters\n"), mak_ir_ext_3((const void *)("Error: unexpected characters\n")));
                                }
                                discard_725 = case_728;
                            }
                            else {
                                discard_725 = 0;
                            }
                            continue;
                        }
                    }
                }
                else if (1) {
                    struct mak__a *arena_call_732 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
                    if (!arena_call_732) {
                        mak_ir_recover("ADT result allocation failed");
                        arena_call_732 = &mak_ir_default_obj__a;
                     } else {
                        mak_ir_fn_2(mak_local_ii_243, (struct mak__a *)(arena_call_732));
                    }
                    struct mak__a * scrut_731 = arena_call_732;
                    if ((scrut_731 != NULL && (scrut_731)->tag == mak_ir_tag_1) && (scrut_731->v.v1.f0 == INT64_C(63))) {
                        struct mak__a * mak_local_br_316 = (scrut_731)->v.v1.f1;
                        struct mak__a *arena_call_734 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
                        if (!arena_call_734) {
                            mak_ir_recover("ADT result allocation failed");
                            arena_call_734 = &mak_ir_default_obj__a;
                         } else {
                            mak_ir_fn_2(mak_local_br_316, (struct mak__a *)(arena_call_734));
                        }
                        struct mak__a * scrut_733 = arena_call_734;
                        if ((scrut_733 != NULL && (scrut_733)->tag == mak_ir_tag_0)) {
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
                            int32_t discard_735 = 0;
                            struct mak__f *arena_call_737 = (struct mak__f *)mak_ir_arena_alloc(sizeof(struct mak__f), _Alignof(struct mak__f));
                            if (!arena_call_737) {
                                mak_ir_recover("ADT result allocation failed");
                                arena_call_737 = &mak_ir_default_obj__f;
                             } else {
                                mak_ir_fn_16(mak_local_ii_243, (struct mak__f *)(arena_call_737));
                            }
                            struct mak__f * scrut_736 = arena_call_737;
                            if ((scrut_736 != NULL && (scrut_736)->tag == mak_ir_tag_11)) {
                                mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)("Error: invalid expression\n"), mak_ir_ext_3((const void *)("Error: invalid expression\n")));
                            }
                            else if ((scrut_736 != NULL && (scrut_736)->tag == mak_ir_tag_10)) {
                                double mak_local_ig_317 = (scrut_736)->v.v10.f0;
                                struct mak__a * mak_local_ih_318 = (scrut_736)->v.v10.f1;
                                int32_t case_738 = 0;
                                struct mak__a *arena_call_740 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
                                if (!arena_call_740) {
                                    mak_ir_recover("ADT result allocation failed");
                                    arena_call_740 = &mak_ir_default_obj__a;
                                 } else {
                                    mak_ir_fn_2(mak_local_ih_318, (struct mak__a *)(arena_call_740));
                                }
                                struct mak__a * scrut_739 = arena_call_740;
                                if ((scrut_739 != NULL && (scrut_739)->tag == mak_ir_tag_0)) {
                                    case_738 = mak_ir_ext_2((const void *)("%.17g\n"), mak_local_ig_317);
                                }
                                else if (1) {
                                    mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)("Error: unexpected characters\n"), mak_ir_ext_3((const void *)("Error: unexpected characters\n")));
                                }
                                discard_735 = case_738;
                            }
                            else {
                                discard_735 = 0;
                            }
                            continue;
                        }
                    }
                    else if ((scrut_731 != NULL && (scrut_731)->tag == mak_ir_tag_1) && (scrut_731->v.v1.f0 == INT64_C(104))) {
                        struct mak__a * mak_local_bs_319 = (scrut_731)->v.v1.f1;
                        if ((mak_local_bs_319 != NULL && (mak_local_bs_319)->tag == mak_ir_tag_1) && (mak_local_bs_319->v.v1.f0 == INT64_C(101))) {
                            struct mak__a * mak_local_bt_320 = (mak_local_bs_319)->v.v1.f1;
                            if ((mak_local_bt_320 != NULL && (mak_local_bt_320)->tag == mak_ir_tag_1) && (mak_local_bt_320->v.v1.f0 == INT64_C(108))) {
                                struct mak__a * mak_local_bu_321 = (mak_local_bt_320)->v.v1.f1;
                                if ((mak_local_bu_321 != NULL && (mak_local_bu_321)->tag == mak_ir_tag_1) && (mak_local_bu_321->v.v1.f0 == INT64_C(112))) {
                                    struct mak__a * mak_local_bv_322 = (mak_local_bu_321)->v.v1.f1;
                                    struct mak__a *arena_call_742 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
                                    if (!arena_call_742) {
                                        mak_ir_recover("ADT result allocation failed");
                                        arena_call_742 = &mak_ir_default_obj__a;
                                     } else {
                                        mak_ir_fn_2(mak_local_bv_322, (struct mak__a *)(arena_call_742));
                                    }
                                    struct mak__a * scrut_741 = arena_call_742;
                                    if ((scrut_741 != NULL && (scrut_741)->tag == mak_ir_tag_0)) {
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
                                        int32_t discard_743 = 0;
                                        struct mak__f *arena_call_745 = (struct mak__f *)mak_ir_arena_alloc(sizeof(struct mak__f), _Alignof(struct mak__f));
                                        if (!arena_call_745) {
                                            mak_ir_recover("ADT result allocation failed");
                                            arena_call_745 = &mak_ir_default_obj__f;
                                         } else {
                                            mak_ir_fn_16(mak_local_ii_243, (struct mak__f *)(arena_call_745));
                                        }
                                        struct mak__f * scrut_744 = arena_call_745;
                                        if ((scrut_744 != NULL && (scrut_744)->tag == mak_ir_tag_11)) {
                                            mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)("Error: invalid expression\n"), mak_ir_ext_3((const void *)("Error: invalid expression\n")));
                                        }
                                        else if ((scrut_744 != NULL && (scrut_744)->tag == mak_ir_tag_10)) {
                                            double mak_local_ig_323 = (scrut_744)->v.v10.f0;
                                            struct mak__a * mak_local_ih_324 = (scrut_744)->v.v10.f1;
                                            int32_t case_746 = 0;
                                            struct mak__a *arena_call_748 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
                                            if (!arena_call_748) {
                                                mak_ir_recover("ADT result allocation failed");
                                                arena_call_748 = &mak_ir_default_obj__a;
                                             } else {
                                                mak_ir_fn_2(mak_local_ih_324, (struct mak__a *)(arena_call_748));
                                            }
                                            struct mak__a * scrut_747 = arena_call_748;
                                            if ((scrut_747 != NULL && (scrut_747)->tag == mak_ir_tag_0)) {
                                                case_746 = mak_ir_ext_2((const void *)("%.17g\n"), mak_local_ig_323);
                                            }
                                            else if (1) {
                                                mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)("Error: unexpected characters\n"), mak_ir_ext_3((const void *)("Error: unexpected characters\n")));
                                            }
                                            discard_743 = case_746;
                                        }
                                        else {
                                            discard_743 = 0;
                                        }
                                        continue;
                                    }
                                }
                                else if (1) {
                                    int32_t discard_749 = 0;
                                    struct mak__f *arena_call_751 = (struct mak__f *)mak_ir_arena_alloc(sizeof(struct mak__f), _Alignof(struct mak__f));
                                    if (!arena_call_751) {
                                        mak_ir_recover("ADT result allocation failed");
                                        arena_call_751 = &mak_ir_default_obj__f;
                                     } else {
                                        mak_ir_fn_16(mak_local_ii_243, (struct mak__f *)(arena_call_751));
                                    }
                                    struct mak__f * scrut_750 = arena_call_751;
                                    if ((scrut_750 != NULL && (scrut_750)->tag == mak_ir_tag_11)) {
                                        mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)("Error: invalid expression\n"), mak_ir_ext_3((const void *)("Error: invalid expression\n")));
                                    }
                                    else if ((scrut_750 != NULL && (scrut_750)->tag == mak_ir_tag_10)) {
                                        double mak_local_ig_325 = (scrut_750)->v.v10.f0;
                                        struct mak__a * mak_local_ih_326 = (scrut_750)->v.v10.f1;
                                        int32_t case_752 = 0;
                                        struct mak__a *arena_call_754 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
                                        if (!arena_call_754) {
                                            mak_ir_recover("ADT result allocation failed");
                                            arena_call_754 = &mak_ir_default_obj__a;
                                         } else {
                                            mak_ir_fn_2(mak_local_ih_326, (struct mak__a *)(arena_call_754));
                                        }
                                        struct mak__a * scrut_753 = arena_call_754;
                                        if ((scrut_753 != NULL && (scrut_753)->tag == mak_ir_tag_0)) {
                                            case_752 = mak_ir_ext_2((const void *)("%.17g\n"), mak_local_ig_325);
                                        }
                                        else if (1) {
                                            mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)("Error: unexpected characters\n"), mak_ir_ext_3((const void *)("Error: unexpected characters\n")));
                                        }
                                        discard_749 = case_752;
                                    }
                                    else {
                                        discard_749 = 0;
                                    }
                                    continue;
                                }
                            }
                            else if (1) {
                                int32_t discard_755 = 0;
                                struct mak__f *arena_call_757 = (struct mak__f *)mak_ir_arena_alloc(sizeof(struct mak__f), _Alignof(struct mak__f));
                                if (!arena_call_757) {
                                    mak_ir_recover("ADT result allocation failed");
                                    arena_call_757 = &mak_ir_default_obj__f;
                                 } else {
                                    mak_ir_fn_16(mak_local_ii_243, (struct mak__f *)(arena_call_757));
                                }
                                struct mak__f * scrut_756 = arena_call_757;
                                if ((scrut_756 != NULL && (scrut_756)->tag == mak_ir_tag_11)) {
                                    mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)("Error: invalid expression\n"), mak_ir_ext_3((const void *)("Error: invalid expression\n")));
                                }
                                else if ((scrut_756 != NULL && (scrut_756)->tag == mak_ir_tag_10)) {
                                    double mak_local_ig_327 = (scrut_756)->v.v10.f0;
                                    struct mak__a * mak_local_ih_328 = (scrut_756)->v.v10.f1;
                                    int32_t case_758 = 0;
                                    struct mak__a *arena_call_760 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
                                    if (!arena_call_760) {
                                        mak_ir_recover("ADT result allocation failed");
                                        arena_call_760 = &mak_ir_default_obj__a;
                                     } else {
                                        mak_ir_fn_2(mak_local_ih_328, (struct mak__a *)(arena_call_760));
                                    }
                                    struct mak__a * scrut_759 = arena_call_760;
                                    if ((scrut_759 != NULL && (scrut_759)->tag == mak_ir_tag_0)) {
                                        case_758 = mak_ir_ext_2((const void *)("%.17g\n"), mak_local_ig_327);
                                    }
                                    else if (1) {
                                        mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)("Error: unexpected characters\n"), mak_ir_ext_3((const void *)("Error: unexpected characters\n")));
                                    }
                                    discard_755 = case_758;
                                }
                                else {
                                    discard_755 = 0;
                                }
                                continue;
                            }
                        }
                        else if (1) {
                            int32_t discard_761 = 0;
                            struct mak__f *arena_call_763 = (struct mak__f *)mak_ir_arena_alloc(sizeof(struct mak__f), _Alignof(struct mak__f));
                            if (!arena_call_763) {
                                mak_ir_recover("ADT result allocation failed");
                                arena_call_763 = &mak_ir_default_obj__f;
                             } else {
                                mak_ir_fn_16(mak_local_ii_243, (struct mak__f *)(arena_call_763));
                            }
                            struct mak__f * scrut_762 = arena_call_763;
                            if ((scrut_762 != NULL && (scrut_762)->tag == mak_ir_tag_11)) {
                                mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)("Error: invalid expression\n"), mak_ir_ext_3((const void *)("Error: invalid expression\n")));
                            }
                            else if ((scrut_762 != NULL && (scrut_762)->tag == mak_ir_tag_10)) {
                                double mak_local_ig_329 = (scrut_762)->v.v10.f0;
                                struct mak__a * mak_local_ih_330 = (scrut_762)->v.v10.f1;
                                int32_t case_764 = 0;
                                struct mak__a *arena_call_766 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
                                if (!arena_call_766) {
                                    mak_ir_recover("ADT result allocation failed");
                                    arena_call_766 = &mak_ir_default_obj__a;
                                 } else {
                                    mak_ir_fn_2(mak_local_ih_330, (struct mak__a *)(arena_call_766));
                                }
                                struct mak__a * scrut_765 = arena_call_766;
                                if ((scrut_765 != NULL && (scrut_765)->tag == mak_ir_tag_0)) {
                                    case_764 = mak_ir_ext_2((const void *)("%.17g\n"), mak_local_ig_329);
                                }
                                else if (1) {
                                    mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)("Error: unexpected characters\n"), mak_ir_ext_3((const void *)("Error: unexpected characters\n")));
                                }
                                discard_761 = case_764;
                            }
                            else {
                                discard_761 = 0;
                            }
                            continue;
                        }
                    }
                    else if (1) {
                        int32_t discard_767 = 0;
                        struct mak__f *arena_call_769 = (struct mak__f *)mak_ir_arena_alloc(sizeof(struct mak__f), _Alignof(struct mak__f));
                        if (!arena_call_769) {
                            mak_ir_recover("ADT result allocation failed");
                            arena_call_769 = &mak_ir_default_obj__f;
                         } else {
                            mak_ir_fn_16(mak_local_ii_243, (struct mak__f *)(arena_call_769));
                        }
                        struct mak__f * scrut_768 = arena_call_769;
                        if ((scrut_768 != NULL && (scrut_768)->tag == mak_ir_tag_11)) {
                            mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)("Error: invalid expression\n"), mak_ir_ext_3((const void *)("Error: invalid expression\n")));
                        }
                        else if ((scrut_768 != NULL && (scrut_768)->tag == mak_ir_tag_10)) {
                            double mak_local_ig_331 = (scrut_768)->v.v10.f0;
                            struct mak__a * mak_local_ih_332 = (scrut_768)->v.v10.f1;
                            int32_t case_770 = 0;
                            struct mak__a *arena_call_772 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
                            if (!arena_call_772) {
                                mak_ir_recover("ADT result allocation failed");
                                arena_call_772 = &mak_ir_default_obj__a;
                             } else {
                                mak_ir_fn_2(mak_local_ih_332, (struct mak__a *)(arena_call_772));
                            }
                            struct mak__a * scrut_771 = arena_call_772;
                            if ((scrut_771 != NULL && (scrut_771)->tag == mak_ir_tag_0)) {
                                case_770 = mak_ir_ext_2((const void *)("%.17g\n"), mak_local_ig_331);
                            }
                            else if (1) {
                                mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)("Error: unexpected characters\n"), mak_ir_ext_3((const void *)("Error: unexpected characters\n")));
                            }
                            discard_767 = case_770;
                        }
                        else {
                            discard_767 = 0;
                        }
                        continue;
                    }
                }
            }
            else if (1) {
                struct mak__a *arena_call_774 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
                if (!arena_call_774) {
                    mak_ir_recover("ADT result allocation failed");
                    arena_call_774 = &mak_ir_default_obj__a;
                 } else {
                    mak_ir_fn_2(mak_local_ii_243, (struct mak__a *)(arena_call_774));
                }
                struct mak__a * scrut_773 = arena_call_774;
                if ((scrut_773 != NULL && (scrut_773)->tag == mak_ir_tag_1) && (scrut_773->v.v1.f0 == INT64_C(63))) {
                    struct mak__a * mak_local_br_333 = (scrut_773)->v.v1.f1;
                    struct mak__a *arena_call_776 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
                    if (!arena_call_776) {
                        mak_ir_recover("ADT result allocation failed");
                        arena_call_776 = &mak_ir_default_obj__a;
                     } else {
                        mak_ir_fn_2(mak_local_br_333, (struct mak__a *)(arena_call_776));
                    }
                    struct mak__a * scrut_775 = arena_call_776;
                    if ((scrut_775 != NULL && (scrut_775)->tag == mak_ir_tag_0)) {
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
                        int32_t discard_777 = 0;
                        struct mak__f *arena_call_779 = (struct mak__f *)mak_ir_arena_alloc(sizeof(struct mak__f), _Alignof(struct mak__f));
                        if (!arena_call_779) {
                            mak_ir_recover("ADT result allocation failed");
                            arena_call_779 = &mak_ir_default_obj__f;
                         } else {
                            mak_ir_fn_16(mak_local_ii_243, (struct mak__f *)(arena_call_779));
                        }
                        struct mak__f * scrut_778 = arena_call_779;
                        if ((scrut_778 != NULL && (scrut_778)->tag == mak_ir_tag_11)) {
                            mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)("Error: invalid expression\n"), mak_ir_ext_3((const void *)("Error: invalid expression\n")));
                        }
                        else if ((scrut_778 != NULL && (scrut_778)->tag == mak_ir_tag_10)) {
                            double mak_local_ig_334 = (scrut_778)->v.v10.f0;
                            struct mak__a * mak_local_ih_335 = (scrut_778)->v.v10.f1;
                            int32_t case_780 = 0;
                            struct mak__a *arena_call_782 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
                            if (!arena_call_782) {
                                mak_ir_recover("ADT result allocation failed");
                                arena_call_782 = &mak_ir_default_obj__a;
                             } else {
                                mak_ir_fn_2(mak_local_ih_335, (struct mak__a *)(arena_call_782));
                            }
                            struct mak__a * scrut_781 = arena_call_782;
                            if ((scrut_781 != NULL && (scrut_781)->tag == mak_ir_tag_0)) {
                                case_780 = mak_ir_ext_2((const void *)("%.17g\n"), mak_local_ig_334);
                            }
                            else if (1) {
                                mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)("Error: unexpected characters\n"), mak_ir_ext_3((const void *)("Error: unexpected characters\n")));
                            }
                            discard_777 = case_780;
                        }
                        else {
                            discard_777 = 0;
                        }
                        continue;
                    }
                }
                else if ((scrut_773 != NULL && (scrut_773)->tag == mak_ir_tag_1) && (scrut_773->v.v1.f0 == INT64_C(104))) {
                    struct mak__a * mak_local_bs_336 = (scrut_773)->v.v1.f1;
                    if ((mak_local_bs_336 != NULL && (mak_local_bs_336)->tag == mak_ir_tag_1) && (mak_local_bs_336->v.v1.f0 == INT64_C(101))) {
                        struct mak__a * mak_local_bt_337 = (mak_local_bs_336)->v.v1.f1;
                        if ((mak_local_bt_337 != NULL && (mak_local_bt_337)->tag == mak_ir_tag_1) && (mak_local_bt_337->v.v1.f0 == INT64_C(108))) {
                            struct mak__a * mak_local_bu_338 = (mak_local_bt_337)->v.v1.f1;
                            if ((mak_local_bu_338 != NULL && (mak_local_bu_338)->tag == mak_ir_tag_1) && (mak_local_bu_338->v.v1.f0 == INT64_C(112))) {
                                struct mak__a * mak_local_bv_339 = (mak_local_bu_338)->v.v1.f1;
                                struct mak__a *arena_call_784 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
                                if (!arena_call_784) {
                                    mak_ir_recover("ADT result allocation failed");
                                    arena_call_784 = &mak_ir_default_obj__a;
                                 } else {
                                    mak_ir_fn_2(mak_local_bv_339, (struct mak__a *)(arena_call_784));
                                }
                                struct mak__a * scrut_783 = arena_call_784;
                                if ((scrut_783 != NULL && (scrut_783)->tag == mak_ir_tag_0)) {
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
                                    int32_t discard_785 = 0;
                                    struct mak__f *arena_call_787 = (struct mak__f *)mak_ir_arena_alloc(sizeof(struct mak__f), _Alignof(struct mak__f));
                                    if (!arena_call_787) {
                                        mak_ir_recover("ADT result allocation failed");
                                        arena_call_787 = &mak_ir_default_obj__f;
                                     } else {
                                        mak_ir_fn_16(mak_local_ii_243, (struct mak__f *)(arena_call_787));
                                    }
                                    struct mak__f * scrut_786 = arena_call_787;
                                    if ((scrut_786 != NULL && (scrut_786)->tag == mak_ir_tag_11)) {
                                        mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)("Error: invalid expression\n"), mak_ir_ext_3((const void *)("Error: invalid expression\n")));
                                    }
                                    else if ((scrut_786 != NULL && (scrut_786)->tag == mak_ir_tag_10)) {
                                        double mak_local_ig_340 = (scrut_786)->v.v10.f0;
                                        struct mak__a * mak_local_ih_341 = (scrut_786)->v.v10.f1;
                                        int32_t case_788 = 0;
                                        struct mak__a *arena_call_790 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
                                        if (!arena_call_790) {
                                            mak_ir_recover("ADT result allocation failed");
                                            arena_call_790 = &mak_ir_default_obj__a;
                                         } else {
                                            mak_ir_fn_2(mak_local_ih_341, (struct mak__a *)(arena_call_790));
                                        }
                                        struct mak__a * scrut_789 = arena_call_790;
                                        if ((scrut_789 != NULL && (scrut_789)->tag == mak_ir_tag_0)) {
                                            case_788 = mak_ir_ext_2((const void *)("%.17g\n"), mak_local_ig_340);
                                        }
                                        else if (1) {
                                            mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)("Error: unexpected characters\n"), mak_ir_ext_3((const void *)("Error: unexpected characters\n")));
                                        }
                                        discard_785 = case_788;
                                    }
                                    else {
                                        discard_785 = 0;
                                    }
                                    continue;
                                }
                            }
                            else if (1) {
                                int32_t discard_791 = 0;
                                struct mak__f *arena_call_793 = (struct mak__f *)mak_ir_arena_alloc(sizeof(struct mak__f), _Alignof(struct mak__f));
                                if (!arena_call_793) {
                                    mak_ir_recover("ADT result allocation failed");
                                    arena_call_793 = &mak_ir_default_obj__f;
                                 } else {
                                    mak_ir_fn_16(mak_local_ii_243, (struct mak__f *)(arena_call_793));
                                }
                                struct mak__f * scrut_792 = arena_call_793;
                                if ((scrut_792 != NULL && (scrut_792)->tag == mak_ir_tag_11)) {
                                    mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)("Error: invalid expression\n"), mak_ir_ext_3((const void *)("Error: invalid expression\n")));
                                }
                                else if ((scrut_792 != NULL && (scrut_792)->tag == mak_ir_tag_10)) {
                                    double mak_local_ig_342 = (scrut_792)->v.v10.f0;
                                    struct mak__a * mak_local_ih_343 = (scrut_792)->v.v10.f1;
                                    int32_t case_794 = 0;
                                    struct mak__a *arena_call_796 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
                                    if (!arena_call_796) {
                                        mak_ir_recover("ADT result allocation failed");
                                        arena_call_796 = &mak_ir_default_obj__a;
                                     } else {
                                        mak_ir_fn_2(mak_local_ih_343, (struct mak__a *)(arena_call_796));
                                    }
                                    struct mak__a * scrut_795 = arena_call_796;
                                    if ((scrut_795 != NULL && (scrut_795)->tag == mak_ir_tag_0)) {
                                        case_794 = mak_ir_ext_2((const void *)("%.17g\n"), mak_local_ig_342);
                                    }
                                    else if (1) {
                                        mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)("Error: unexpected characters\n"), mak_ir_ext_3((const void *)("Error: unexpected characters\n")));
                                    }
                                    discard_791 = case_794;
                                }
                                else {
                                    discard_791 = 0;
                                }
                                continue;
                            }
                        }
                        else if (1) {
                            int32_t discard_797 = 0;
                            struct mak__f *arena_call_799 = (struct mak__f *)mak_ir_arena_alloc(sizeof(struct mak__f), _Alignof(struct mak__f));
                            if (!arena_call_799) {
                                mak_ir_recover("ADT result allocation failed");
                                arena_call_799 = &mak_ir_default_obj__f;
                             } else {
                                mak_ir_fn_16(mak_local_ii_243, (struct mak__f *)(arena_call_799));
                            }
                            struct mak__f * scrut_798 = arena_call_799;
                            if ((scrut_798 != NULL && (scrut_798)->tag == mak_ir_tag_11)) {
                                mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)("Error: invalid expression\n"), mak_ir_ext_3((const void *)("Error: invalid expression\n")));
                            }
                            else if ((scrut_798 != NULL && (scrut_798)->tag == mak_ir_tag_10)) {
                                double mak_local_ig_344 = (scrut_798)->v.v10.f0;
                                struct mak__a * mak_local_ih_345 = (scrut_798)->v.v10.f1;
                                int32_t case_800 = 0;
                                struct mak__a *arena_call_802 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
                                if (!arena_call_802) {
                                    mak_ir_recover("ADT result allocation failed");
                                    arena_call_802 = &mak_ir_default_obj__a;
                                 } else {
                                    mak_ir_fn_2(mak_local_ih_345, (struct mak__a *)(arena_call_802));
                                }
                                struct mak__a * scrut_801 = arena_call_802;
                                if ((scrut_801 != NULL && (scrut_801)->tag == mak_ir_tag_0)) {
                                    case_800 = mak_ir_ext_2((const void *)("%.17g\n"), mak_local_ig_344);
                                }
                                else if (1) {
                                    mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)("Error: unexpected characters\n"), mak_ir_ext_3((const void *)("Error: unexpected characters\n")));
                                }
                                discard_797 = case_800;
                            }
                            else {
                                discard_797 = 0;
                            }
                            continue;
                        }
                    }
                    else if (1) {
                        int32_t discard_803 = 0;
                        struct mak__f *arena_call_805 = (struct mak__f *)mak_ir_arena_alloc(sizeof(struct mak__f), _Alignof(struct mak__f));
                        if (!arena_call_805) {
                            mak_ir_recover("ADT result allocation failed");
                            arena_call_805 = &mak_ir_default_obj__f;
                         } else {
                            mak_ir_fn_16(mak_local_ii_243, (struct mak__f *)(arena_call_805));
                        }
                        struct mak__f * scrut_804 = arena_call_805;
                        if ((scrut_804 != NULL && (scrut_804)->tag == mak_ir_tag_11)) {
                            mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)("Error: invalid expression\n"), mak_ir_ext_3((const void *)("Error: invalid expression\n")));
                        }
                        else if ((scrut_804 != NULL && (scrut_804)->tag == mak_ir_tag_10)) {
                            double mak_local_ig_346 = (scrut_804)->v.v10.f0;
                            struct mak__a * mak_local_ih_347 = (scrut_804)->v.v10.f1;
                            int32_t case_806 = 0;
                            struct mak__a *arena_call_808 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
                            if (!arena_call_808) {
                                mak_ir_recover("ADT result allocation failed");
                                arena_call_808 = &mak_ir_default_obj__a;
                             } else {
                                mak_ir_fn_2(mak_local_ih_347, (struct mak__a *)(arena_call_808));
                            }
                            struct mak__a * scrut_807 = arena_call_808;
                            if ((scrut_807 != NULL && (scrut_807)->tag == mak_ir_tag_0)) {
                                case_806 = mak_ir_ext_2((const void *)("%.17g\n"), mak_local_ig_346);
                            }
                            else if (1) {
                                mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)("Error: unexpected characters\n"), mak_ir_ext_3((const void *)("Error: unexpected characters\n")));
                            }
                            discard_803 = case_806;
                        }
                        else {
                            discard_803 = 0;
                        }
                        continue;
                    }
                }
                else if (1) {
                    int32_t discard_809 = 0;
                    struct mak__f *arena_call_811 = (struct mak__f *)mak_ir_arena_alloc(sizeof(struct mak__f), _Alignof(struct mak__f));
                    if (!arena_call_811) {
                        mak_ir_recover("ADT result allocation failed");
                        arena_call_811 = &mak_ir_default_obj__f;
                     } else {
                        mak_ir_fn_16(mak_local_ii_243, (struct mak__f *)(arena_call_811));
                    }
                    struct mak__f * scrut_810 = arena_call_811;
                    if ((scrut_810 != NULL && (scrut_810)->tag == mak_ir_tag_11)) {
                        mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)("Error: invalid expression\n"), mak_ir_ext_3((const void *)("Error: invalid expression\n")));
                    }
                    else if ((scrut_810 != NULL && (scrut_810)->tag == mak_ir_tag_10)) {
                        double mak_local_ig_348 = (scrut_810)->v.v10.f0;
                        struct mak__a * mak_local_ih_349 = (scrut_810)->v.v10.f1;
                        int32_t case_812 = 0;
                        struct mak__a *arena_call_814 = (struct mak__a *)mak_ir_arena_alloc(sizeof(struct mak__a), _Alignof(struct mak__a));
                        if (!arena_call_814) {
                            mak_ir_recover("ADT result allocation failed");
                            arena_call_814 = &mak_ir_default_obj__a;
                         } else {
                            mak_ir_fn_2(mak_local_ih_349, (struct mak__a *)(arena_call_814));
                        }
                        struct mak__a * scrut_813 = arena_call_814;
                        if ((scrut_813 != NULL && (scrut_813)->tag == mak_ir_tag_0)) {
                            case_812 = mak_ir_ext_2((const void *)("%.17g\n"), mak_local_ig_348);
                        }
                        else if (1) {
                            mak_ir_ext_4((int32_t)(INT64_C(1)), (const void *)("Error: unexpected characters\n"), mak_ir_ext_3((const void *)("Error: unexpected characters\n")));
                        }
                        discard_809 = case_812;
                    }
                    else {
                        discard_809 = 0;
                    }
                    continue;
                }
            }
        }
        else {
            mak_ir_recover("case had no matching arm");
            return 0.0;
        }
    }
}

static inline void mak_ir_fn_18(int64_t a0, int64_t a1, struct mak__f *out) {
    switch (a0) {
        case INT64_C(399964812): {
            struct mak__f *arena_call_816 = (struct mak__f *)__builtin_alloca(sizeof(struct mak__f));
                mak_ir_fn_16((struct mak__a *)mak_ir_adt_recover_ptr((const void *)&mak_ir_default_obj__a, "integer used as ADT"), (struct mak__f *)(arena_call_816));
            struct mak__f * scrut_815 = arena_call_816;
            if ((scrut_815 != NULL && (scrut_815)->tag == mak_ir_tag_11)) {
                *(out) = mak_ir_ctor_obj_11;
            }
            else if ((scrut_815 != NULL && (scrut_815)->tag == mak_ir_tag_10)) {
                double mak_local_ff_350 = (scrut_815)->v.v10.f0;
                struct mak__a * mak_local_fg_351 = (scrut_815)->v.v10.f1;
                struct mak__a *arena_call_818 = (struct mak__a *)__builtin_alloca(sizeof(struct mak__a));
                    mak_ir_fn_2(mak_local_fg_351, (struct mak__a *)(arena_call_818));
                struct mak__a * scrut_817 = arena_call_818;
                if ((scrut_817 != NULL && (scrut_817)->tag == mak_ir_tag_1) && (scrut_817->v.v1.f0 == INT64_C(44))) {
                    struct mak__a * mak_local_fh_352 = (scrut_817)->v.v1.f1;
                    struct mak__f *arena_call_820 = (struct mak__f *)__builtin_alloca(sizeof(struct mak__f));
                        mak_ir_fn_16(mak_local_fh_352, (struct mak__f *)(arena_call_820));
                    struct mak__f * scrut_819 = arena_call_820;
                    if ((scrut_819 != NULL && (scrut_819)->tag == mak_ir_tag_11)) {
                        *(out) = mak_ir_ctor_obj_11;
                    }
                    else if ((scrut_819 != NULL && (scrut_819)->tag == mak_ir_tag_10)) {
                        double mak_local_fi_353 = (scrut_819)->v.v10.f0;
                        struct mak__a * mak_local_fj_354 = (scrut_819)->v.v10.f1;
                        struct mak__a *arena_call_822 = (struct mak__a *)__builtin_alloca(sizeof(struct mak__a));
                            mak_ir_fn_2(mak_local_fj_354, (struct mak__a *)(arena_call_822));
                        struct mak__a * scrut_821 = arena_call_822;
                        if ((scrut_821 != NULL && (scrut_821)->tag == mak_ir_tag_1) && (scrut_821->v.v1.f0 == INT64_C(44))) {
                            struct mak__a * mak_local_fk_355 = (scrut_821)->v.v1.f1;
                            struct mak__f *arena_call_824 = (struct mak__f *)__builtin_alloca(sizeof(struct mak__f));
                                mak_ir_fn_16(mak_local_fk_355, (struct mak__f *)(arena_call_824));
                            struct mak__f * scrut_823 = arena_call_824;
                            if ((scrut_823 != NULL && (scrut_823)->tag == mak_ir_tag_11)) {
                                *(out) = mak_ir_ctor_obj_11;
                            }
                            else if ((scrut_823 != NULL && (scrut_823)->tag == mak_ir_tag_10)) {
                                double mak_local_fl_356 = (scrut_823)->v.v10.f0;
                                struct mak__a * mak_local_fm_357 = (scrut_823)->v.v10.f1;
                                struct mak__a *arena_call_826 = (struct mak__a *)__builtin_alloca(sizeof(struct mak__a));
                                    mak_ir_fn_2(mak_local_fm_357, (struct mak__a *)(arena_call_826));
                                struct mak__a * scrut_825 = arena_call_826;
                                if ((scrut_825 != NULL && (scrut_825)->tag == mak_ir_tag_1) && (scrut_825->v.v1.f0 == INT64_C(41))) {
                                    struct mak__a * mak_local_fo_358 = (scrut_825)->v.v1.f1;
                                    switch (a0) {
                                        case INT64_C(399964812): {
                                            (out)->tag = mak_ir_tag_10;
                                            double case_827 = 0.0;
                                            switch (a0) {
                                                case INT64_C(399964812): {
                                                    double case_828 = 0.0;
                                                    int32_t scrut_829 = (int32_t)(mak_local_ff_350) < (int32_t)(mak_local_fi_353);
                                                    if ((scrut_829 == INT64_C(1))) {
                                                        case_828 = mak_local_fi_353;
                                                    }
                                                    else if (1) {
                                                        double case_830 = 0.0;
                                                        int32_t scrut_831 = (int32_t)(mak_local_ff_350) > (int32_t)(mak_local_fl_356);
                                                        if ((scrut_831 == INT64_C(1))) {
                                                            case_830 = mak_local_fl_356;
                                                        }
                                                        else if (1) {
                                                            case_830 = mak_local_ff_350;
                                                        }
                                                        case_828 = case_830;
                                                    }
                                                    case_827 = case_828;
                                                    break;
                                                }
                                                case INT64_C(1764798): {
                                                    case_827 = mak_ir_ext_45(mak_local_ff_350, mak_local_fi_353, mak_local_fl_356);
                                                    break;
                                                }
                                                default: {
                                                    case_827 = mak_local_ff_350;
                                                    break;
                                                }
                                            }
                                            (out)->v.v10.f0 = case_827;
                                            (out)->v.v10.f1 = mak_local_fo_358;
                                            break;
                                        }
                                        case INT64_C(1764798): {
                                            (out)->tag = mak_ir_tag_10;
                                            double case_832 = 0.0;
                                            switch (a0) {
                                                case INT64_C(399964812): {
                                                    double case_833 = 0.0;
                                                    int32_t scrut_834 = (int32_t)(mak_local_ff_350) < (int32_t)(mak_local_fi_353);
                                                    if ((scrut_834 == INT64_C(1))) {
                                                        case_833 = mak_local_fi_353;
                                                    }
                                                    else if (1) {
                                                        double case_835 = 0.0;
                                                        int32_t scrut_836 = (int32_t)(mak_local_ff_350) > (int32_t)(mak_local_fl_356);
                                                        if ((scrut_836 == INT64_C(1))) {
                                                            case_835 = mak_local_fl_356;
                                                        }
                                                        else if (1) {
                                                            case_835 = mak_local_ff_350;
                                                        }
                                                        case_833 = case_835;
                                                    }
                                                    case_832 = case_833;
                                                    break;
                                                }
                                                case INT64_C(1764798): {
                                                    case_832 = mak_ir_ext_45(mak_local_ff_350, mak_local_fi_353, mak_local_fl_356);
                                                    break;
                                                }
                                                default: {
                                                    case_832 = mak_local_ff_350;
                                                    break;
                                                }
                                            }
                                            (out)->v.v10.f0 = case_832;
                                            (out)->v.v10.f1 = mak_local_fo_358;
                                            break;
                                        }
                                        default: {
                                            *(out) = mak_ir_ctor_obj_11;
                                            break;
                                        }
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
            break;
        }
        case INT64_C(1764798): {
            struct mak__f *arena_call_838 = (struct mak__f *)__builtin_alloca(sizeof(struct mak__f));
                mak_ir_fn_16((struct mak__a *)mak_ir_adt_recover_ptr((const void *)&mak_ir_default_obj__a, "integer used as ADT"), (struct mak__f *)(arena_call_838));
            struct mak__f * scrut_837 = arena_call_838;
            if ((scrut_837 != NULL && (scrut_837)->tag == mak_ir_tag_11)) {
                *(out) = mak_ir_ctor_obj_11;
            }
            else if ((scrut_837 != NULL && (scrut_837)->tag == mak_ir_tag_10)) {
                double mak_local_ff_359 = (scrut_837)->v.v10.f0;
                struct mak__a * mak_local_fg_360 = (scrut_837)->v.v10.f1;
                struct mak__a *arena_call_840 = (struct mak__a *)__builtin_alloca(sizeof(struct mak__a));
                    mak_ir_fn_2(mak_local_fg_360, (struct mak__a *)(arena_call_840));
                struct mak__a * scrut_839 = arena_call_840;
                if ((scrut_839 != NULL && (scrut_839)->tag == mak_ir_tag_1) && (scrut_839->v.v1.f0 == INT64_C(44))) {
                    struct mak__a * mak_local_fh_361 = (scrut_839)->v.v1.f1;
                    struct mak__f *arena_call_842 = (struct mak__f *)__builtin_alloca(sizeof(struct mak__f));
                        mak_ir_fn_16(mak_local_fh_361, (struct mak__f *)(arena_call_842));
                    struct mak__f * scrut_841 = arena_call_842;
                    if ((scrut_841 != NULL && (scrut_841)->tag == mak_ir_tag_11)) {
                        *(out) = mak_ir_ctor_obj_11;
                    }
                    else if ((scrut_841 != NULL && (scrut_841)->tag == mak_ir_tag_10)) {
                        double mak_local_fi_362 = (scrut_841)->v.v10.f0;
                        struct mak__a * mak_local_fj_363 = (scrut_841)->v.v10.f1;
                        struct mak__a *arena_call_844 = (struct mak__a *)__builtin_alloca(sizeof(struct mak__a));
                            mak_ir_fn_2(mak_local_fj_363, (struct mak__a *)(arena_call_844));
                        struct mak__a * scrut_843 = arena_call_844;
                        if ((scrut_843 != NULL && (scrut_843)->tag == mak_ir_tag_1) && (scrut_843->v.v1.f0 == INT64_C(44))) {
                            struct mak__a * mak_local_fk_364 = (scrut_843)->v.v1.f1;
                            struct mak__f *arena_call_846 = (struct mak__f *)__builtin_alloca(sizeof(struct mak__f));
                                mak_ir_fn_16(mak_local_fk_364, (struct mak__f *)(arena_call_846));
                            struct mak__f * scrut_845 = arena_call_846;
                            if ((scrut_845 != NULL && (scrut_845)->tag == mak_ir_tag_11)) {
                                *(out) = mak_ir_ctor_obj_11;
                            }
                            else if ((scrut_845 != NULL && (scrut_845)->tag == mak_ir_tag_10)) {
                                double mak_local_fl_365 = (scrut_845)->v.v10.f0;
                                struct mak__a * mak_local_fm_366 = (scrut_845)->v.v10.f1;
                                struct mak__a *arena_call_848 = (struct mak__a *)__builtin_alloca(sizeof(struct mak__a));
                                    mak_ir_fn_2(mak_local_fm_366, (struct mak__a *)(arena_call_848));
                                struct mak__a * scrut_847 = arena_call_848;
                                if ((scrut_847 != NULL && (scrut_847)->tag == mak_ir_tag_1) && (scrut_847->v.v1.f0 == INT64_C(41))) {
                                    struct mak__a * mak_local_fo_367 = (scrut_847)->v.v1.f1;
                                    switch (a0) {
                                        case INT64_C(399964812): {
                                            (out)->tag = mak_ir_tag_10;
                                            double case_849 = 0.0;
                                            switch (a0) {
                                                case INT64_C(399964812): {
                                                    double case_850 = 0.0;
                                                    int32_t scrut_851 = (int32_t)(mak_local_ff_359) < (int32_t)(mak_local_fi_362);
                                                    if ((scrut_851 == INT64_C(1))) {
                                                        case_850 = mak_local_fi_362;
                                                    }
                                                    else if (1) {
                                                        double case_852 = 0.0;
                                                        int32_t scrut_853 = (int32_t)(mak_local_ff_359) > (int32_t)(mak_local_fl_365);
                                                        if ((scrut_853 == INT64_C(1))) {
                                                            case_852 = mak_local_fl_365;
                                                        }
                                                        else if (1) {
                                                            case_852 = mak_local_ff_359;
                                                        }
                                                        case_850 = case_852;
                                                    }
                                                    case_849 = case_850;
                                                    break;
                                                }
                                                case INT64_C(1764798): {
                                                    case_849 = mak_ir_ext_45(mak_local_ff_359, mak_local_fi_362, mak_local_fl_365);
                                                    break;
                                                }
                                                default: {
                                                    case_849 = mak_local_ff_359;
                                                    break;
                                                }
                                            }
                                            (out)->v.v10.f0 = case_849;
                                            (out)->v.v10.f1 = mak_local_fo_367;
                                            break;
                                        }
                                        case INT64_C(1764798): {
                                            (out)->tag = mak_ir_tag_10;
                                            double case_854 = 0.0;
                                            switch (a0) {
                                                case INT64_C(399964812): {
                                                    double case_855 = 0.0;
                                                    int32_t scrut_856 = (int32_t)(mak_local_ff_359) < (int32_t)(mak_local_fi_362);
                                                    if ((scrut_856 == INT64_C(1))) {
                                                        case_855 = mak_local_fi_362;
                                                    }
                                                    else if (1) {
                                                        double case_857 = 0.0;
                                                        int32_t scrut_858 = (int32_t)(mak_local_ff_359) > (int32_t)(mak_local_fl_365);
                                                        if ((scrut_858 == INT64_C(1))) {
                                                            case_857 = mak_local_fl_365;
                                                        }
                                                        else if (1) {
                                                            case_857 = mak_local_ff_359;
                                                        }
                                                        case_855 = case_857;
                                                    }
                                                    case_854 = case_855;
                                                    break;
                                                }
                                                case INT64_C(1764798): {
                                                    case_854 = mak_ir_ext_45(mak_local_ff_359, mak_local_fi_362, mak_local_fl_365);
                                                    break;
                                                }
                                                default: {
                                                    case_854 = mak_local_ff_359;
                                                    break;
                                                }
                                            }
                                            (out)->v.v10.f0 = case_854;
                                            (out)->v.v10.f1 = mak_local_fo_367;
                                            break;
                                        }
                                        default: {
                                            *(out) = mak_ir_ctor_obj_11;
                                            break;
                                        }
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
            break;
        }
        default: {
            int64_t case_860 = 0;
            switch (a0) {
                case INT64_C(828949774): {
                    case_860 = INT64_C(1);
                    break;
                }
                case INT64_C(231190472): {
                    case_860 = INT64_C(1);
                    break;
                }
                case INT64_C(1885190): {
                    case_860 = INT64_C(1);
                    break;
                }
                case INT64_C(1969694): {
                    case_860 = INT64_C(1);
                    break;
                }
                case INT64_C(510764621): {
                    case_860 = INT64_C(1);
                    break;
                }
                case INT64_C(1936692): {
                    case_860 = INT64_C(1);
                    break;
                }
                case INT64_C(258201902): {
                    case_860 = INT64_C(1);
                    break;
                }
                case INT64_C(548941060): {
                    case_860 = INT64_C(1);
                    break;
                }
                case INT64_C(901947274): {
                    case_860 = INT64_C(1);
                    break;
                }
                case INT64_C(1780652): {
                    case_860 = INT64_C(1);
                    break;
                }
                case INT64_C(1866466): {
                    case_860 = INT64_C(1);
                    break;
                }
                case INT64_C(1900793): {
                    case_860 = INT64_C(1);
                    break;
                }
                case INT64_C(224480257): {
                    case_860 = INT64_C(1);
                    break;
                }
                case INT64_C(1902496): {
                    case_860 = INT64_C(1);
                    break;
                }
                case INT64_C(253534496): {
                    case_860 = INT64_C(1);
                    break;
                }
                case INT64_C(1884414): {
                    case_860 = INT64_C(1);
                    break;
                }
                case INT64_C(1883376): {
                    case_860 = INT64_C(1);
                    break;
                }
                case INT64_C(262106026): {
                    case_860 = INT64_C(1);
                    break;
                }
                case INT64_C(231035246): {
                    case_860 = INT64_C(1);
                    break;
                }
                case INT64_C(231188658): {
                    case_860 = INT64_C(1);
                    break;
                }
                case INT64_C(231189696): {
                    case_860 = INT64_C(1);
                    break;
                }
                case INT64_C(743972013): {
                    case_860 = INT64_C(1);
                    break;
                }
                default: {
                    case_860 = INT64_C(0);
                    break;
                }
            }
            int64_t scrut_859 = case_860;
            if ((scrut_859 == INT64_C(1))) {
                struct mak__f *arena_call_862 = (struct mak__f *)__builtin_alloca(sizeof(struct mak__f));
                    mak_ir_fn_16((struct mak__a *)mak_ir_adt_recover_ptr((const void *)&mak_ir_default_obj__a, "integer used as ADT"), (struct mak__f *)(arena_call_862));
                struct mak__f * scrut_861 = arena_call_862;
                if ((scrut_861 != NULL && (scrut_861)->tag == mak_ir_tag_11)) {
                    *(out) = mak_ir_ctor_obj_11;
                }
                else if ((scrut_861 != NULL && (scrut_861)->tag == mak_ir_tag_10)) {
                    double mak_local_ex_368 = (scrut_861)->v.v10.f0;
                    struct mak__a * mak_local_ey_369 = (scrut_861)->v.v10.f1;
                    struct mak__a *arena_call_864 = (struct mak__a *)__builtin_alloca(sizeof(struct mak__a));
                        mak_ir_fn_2(mak_local_ey_369, (struct mak__a *)(arena_call_864));
                    struct mak__a * scrut_863 = arena_call_864;
                    if ((scrut_863 != NULL && (scrut_863)->tag == mak_ir_tag_1) && (scrut_863->v.v1.f0 == INT64_C(44))) {
                        struct mak__a * mak_local_ez_370 = (scrut_863)->v.v1.f1;
                        struct mak__f *arena_call_866 = (struct mak__f *)__builtin_alloca(sizeof(struct mak__f));
                            mak_ir_fn_16(mak_local_ez_370, (struct mak__f *)(arena_call_866));
                        struct mak__f * scrut_865 = arena_call_866;
                        if ((scrut_865 != NULL && (scrut_865)->tag == mak_ir_tag_11)) {
                            *(out) = mak_ir_ctor_obj_11;
                        }
                        else if ((scrut_865 != NULL && (scrut_865)->tag == mak_ir_tag_10)) {
                            double mak_local_fa_371 = (scrut_865)->v.v10.f0;
                            struct mak__a * mak_local_fb_372 = (scrut_865)->v.v10.f1;
                            struct mak__a *arena_call_868 = (struct mak__a *)__builtin_alloca(sizeof(struct mak__a));
                                mak_ir_fn_2(mak_local_fb_372, (struct mak__a *)(arena_call_868));
                            struct mak__a * scrut_867 = arena_call_868;
                            if ((scrut_867 != NULL && (scrut_867)->tag == mak_ir_tag_1) && (scrut_867->v.v1.f0 == INT64_C(41))) {
                                struct mak__a * mak_local_fc_373 = (scrut_867)->v.v1.f1;
                                int64_t case_870 = 0;
                                switch (a0) {
                                    case INT64_C(828949774): {
                                        case_870 = INT64_C(1);
                                        break;
                                    }
                                    case INT64_C(231190472): {
                                        case_870 = INT64_C(1);
                                        break;
                                    }
                                    case INT64_C(1885190): {
                                        case_870 = INT64_C(1);
                                        break;
                                    }
                                    case INT64_C(1969694): {
                                        case_870 = INT64_C(1);
                                        break;
                                    }
                                    case INT64_C(510764621): {
                                        case_870 = INT64_C(1);
                                        break;
                                    }
                                    case INT64_C(1936692): {
                                        case_870 = INT64_C(1);
                                        break;
                                    }
                                    case INT64_C(258201902): {
                                        case_870 = INT64_C(1);
                                        break;
                                    }
                                    case INT64_C(548941060): {
                                        case_870 = INT64_C(1);
                                        break;
                                    }
                                    case INT64_C(901947274): {
                                        case_870 = INT64_C(1);
                                        break;
                                    }
                                    case INT64_C(1780652): {
                                        case_870 = INT64_C(1);
                                        break;
                                    }
                                    case INT64_C(1866466): {
                                        case_870 = INT64_C(1);
                                        break;
                                    }
                                    case INT64_C(1900793): {
                                        case_870 = INT64_C(1);
                                        break;
                                    }
                                    case INT64_C(224480257): {
                                        case_870 = INT64_C(1);
                                        break;
                                    }
                                    case INT64_C(1902496): {
                                        case_870 = INT64_C(1);
                                        break;
                                    }
                                    case INT64_C(253534496): {
                                        case_870 = INT64_C(1);
                                        break;
                                    }
                                    case INT64_C(1884414): {
                                        case_870 = INT64_C(1);
                                        break;
                                    }
                                    case INT64_C(1883376): {
                                        case_870 = INT64_C(1);
                                        break;
                                    }
                                    case INT64_C(262106026): {
                                        case_870 = INT64_C(1);
                                        break;
                                    }
                                    case INT64_C(231035246): {
                                        case_870 = INT64_C(1);
                                        break;
                                    }
                                    case INT64_C(231188658): {
                                        case_870 = INT64_C(1);
                                        break;
                                    }
                                    case INT64_C(231189696): {
                                        case_870 = INT64_C(1);
                                        break;
                                    }
                                    case INT64_C(743972013): {
                                        case_870 = INT64_C(1);
                                        break;
                                    }
                                    default: {
                                        case_870 = INT64_C(0);
                                        break;
                                    }
                                }
                                int64_t scrut_869 = case_870;
                                if ((scrut_869 == INT64_C(1))) {
                                    (out)->tag = mak_ir_tag_10;
                                    (out)->v.v10.f0 = mak_ir_fn_9(a0, mak_local_ex_368, mak_local_fa_371);
                                    (out)->v.v10.f1 = mak_local_fc_373;
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
                int64_t case_872 = 0;
                switch (a0) {
                    case INT64_C(1677570): {
                        case_872 = INT64_C(1);
                        break;
                    }
                    case INT64_C(230982852): {
                        case_872 = INT64_C(1);
                        break;
                    }
                    case INT64_C(260484708): {
                        case_872 = INT64_C(1);
                        break;
                    }
                    case INT64_C(224257837): {
                        case_872 = INT64_C(1);
                        break;
                    }
                    case INT64_C(1749093): {
                        case_872 = INT64_C(1);
                        break;
                    }
                    case INT64_C(229131233): {
                        case_872 = INT64_C(1);
                        break;
                    }
                    case INT64_C(16199091): {
                        case_872 = INT64_C(1);
                        break;
                    }
                    case INT64_C(1868032): {
                        case_872 = INT64_C(1);
                        break;
                    }
                    case INT64_C(14258): {
                        case_872 = INT64_C(1);
                        break;
                    }
                    case INT64_C(244712242): {
                        case_872 = INT64_C(1);
                        break;
                    }
                    case INT64_C(57303395): {
                        case_872 = INT64_C(1);
                        break;
                    }
                    case INT64_C(57303459): {
                        case_872 = INT64_C(1);
                        break;
                    }
                    case INT64_C(1987380): {
                        case_872 = INT64_C(1);
                        break;
                    }
                    case INT64_C(1713595): {
                        case_872 = INT64_C(1);
                        break;
                    }
                    case INT64_C(2003493): {
                        case_872 = INT64_C(1);
                        break;
                    }
                    case INT64_C(220052207): {
                        case_872 = INT64_C(1);
                        break;
                    }
                    case INT64_C(219778422): {
                        case_872 = INT64_C(1);
                        break;
                    }
                    case INT64_C(220068320): {
                        case_872 = INT64_C(1);
                        break;
                    }
                    case INT64_C(260346884): {
                        case_872 = INT64_C(1);
                        break;
                    }
                    case INT64_C(224481049): {
                        case_872 = INT64_C(1);
                        break;
                    }
                    case INT64_C(262457687): {
                        case_872 = INT64_C(1);
                        break;
                    }
                    case INT64_C(224547597): {
                        case_872 = INT64_C(1);
                        break;
                    }
                    case INT64_C(260276799): {
                        case_872 = INT64_C(1);
                        break;
                    }
                    case INT64_C(224481180): {
                        case_872 = INT64_C(1);
                        break;
                    }
                    case INT64_C(826839025): {
                        case_872 = INT64_C(1);
                        break;
                    }
                    case INT64_C(790973190): {
                        case_872 = INT64_C(1);
                        break;
                    }
                    case INT64_C(828949828): {
                        case_872 = INT64_C(1);
                        break;
                    }
                    case INT64_C(1713596): {
                        case_872 = INT64_C(1);
                        break;
                    }
                    case INT64_C(1986845): {
                        case_872 = INT64_C(1);
                        break;
                    }
                    case INT64_C(1714103): {
                        case_872 = INT64_C(1);
                        break;
                    }
                    case INT64_C(219778423): {
                        case_872 = INT64_C(1);
                        break;
                    }
                    case INT64_C(220051672): {
                        case_872 = INT64_C(1);
                        break;
                    }
                    case INT64_C(219778930): {
                        case_872 = INT64_C(1);
                        break;
                    }
                    case INT64_C(283705086): {
                        case_872 = INT64_C(1);
                        break;
                    }
                    case INT64_C(224308133): {
                        case_872 = INT64_C(1);
                        break;
                    }
                    case INT64_C(420295318): {
                        case_872 = INT64_C(1);
                        break;
                    }
                    case INT64_C(824551211): {
                        case_872 = INT64_C(1);
                        break;
                    }
                    case INT64_C(354247632): {
                        case_872 = INT64_C(1);
                        break;
                    }
                    case INT64_C(230982984): {
                        case_872 = INT64_C(1);
                        break;
                    }
                    case INT64_C(553441405): {
                        case_872 = INT64_C(1);
                        break;
                    }
                    case INT64_C(138294544): {
                        case_872 = INT64_C(1);
                        break;
                    }
                    case INT64_C(1748297): {
                        case_872 = INT64_C(1);
                        break;
                    }
                    case INT64_C(229027006): {
                        case_872 = INT64_C(1);
                        break;
                    }
                    case INT64_C(244712290): {
                        case_872 = INT64_C(1);
                        break;
                    }
                    case INT64_C(260345973): {
                        case_872 = INT64_C(1);
                        break;
                    }
                    case INT64_C(231274442): {
                        case_872 = INT64_C(1);
                        break;
                    }
                    case INT64_C(1729434): {
                        case_872 = INT64_C(1);
                        break;
                    }
                    case INT64_C(1969161): {
                        case_872 = INT64_C(1);
                        break;
                    }
                    case INT64_C(992605559): {
                        case_872 = INT64_C(1);
                        break;
                    }
                    case INT64_C(430838469): {
                        case_872 = INT64_C(1);
                        break;
                    }
                    case INT64_C(184475947): {
                        case_872 = INT64_C(1);
                        break;
                    }
                    case INT64_C(224581785): {
                        case_872 = INT64_C(1);
                        break;
                    }
                    default: {
                        case_872 = INT64_C(0);
                        break;
                    }
                }
                int64_t scrut_871 = case_872;
                if ((scrut_871 == INT64_C(1))) {
                    struct mak__f *arena_call_874 = (struct mak__f *)__builtin_alloca(sizeof(struct mak__f));
                        mak_ir_fn_16((struct mak__a *)mak_ir_adt_recover_ptr((const void *)&mak_ir_default_obj__a, "integer used as ADT"), (struct mak__f *)(arena_call_874));
                    struct mak__f * scrut_873 = arena_call_874;
                    if ((scrut_873 != NULL && (scrut_873)->tag == mak_ir_tag_11)) {
                        *(out) = mak_ir_ctor_obj_11;
                    }
                    else if ((scrut_873 != NULL && (scrut_873)->tag == mak_ir_tag_10)) {
                        double mak_local_es_374 = (scrut_873)->v.v10.f0;
                        struct mak__a * mak_local_et_375 = (scrut_873)->v.v10.f1;
                        struct mak__a *arena_call_876 = (struct mak__a *)__builtin_alloca(sizeof(struct mak__a));
                            mak_ir_fn_2(mak_local_et_375, (struct mak__a *)(arena_call_876));
                        struct mak__a * scrut_875 = arena_call_876;
                        if ((scrut_875 != NULL && (scrut_875)->tag == mak_ir_tag_1) && (scrut_875->v.v1.f0 == INT64_C(41))) {
                            struct mak__a * mak_local_eu_376 = (scrut_875)->v.v1.f1;
                            int64_t case_878 = 0;
                            switch (a0) {
                                case INT64_C(1677570): {
                                    case_878 = INT64_C(1);
                                    break;
                                }
                                case INT64_C(230982852): {
                                    case_878 = INT64_C(1);
                                    break;
                                }
                                case INT64_C(260484708): {
                                    case_878 = INT64_C(1);
                                    break;
                                }
                                case INT64_C(224257837): {
                                    case_878 = INT64_C(1);
                                    break;
                                }
                                case INT64_C(1749093): {
                                    case_878 = INT64_C(1);
                                    break;
                                }
                                case INT64_C(229131233): {
                                    case_878 = INT64_C(1);
                                    break;
                                }
                                case INT64_C(16199091): {
                                    case_878 = INT64_C(1);
                                    break;
                                }
                                case INT64_C(1868032): {
                                    case_878 = INT64_C(1);
                                    break;
                                }
                                case INT64_C(14258): {
                                    case_878 = INT64_C(1);
                                    break;
                                }
                                case INT64_C(244712242): {
                                    case_878 = INT64_C(1);
                                    break;
                                }
                                case INT64_C(57303395): {
                                    case_878 = INT64_C(1);
                                    break;
                                }
                                case INT64_C(57303459): {
                                    case_878 = INT64_C(1);
                                    break;
                                }
                                case INT64_C(1987380): {
                                    case_878 = INT64_C(1);
                                    break;
                                }
                                case INT64_C(1713595): {
                                    case_878 = INT64_C(1);
                                    break;
                                }
                                case INT64_C(2003493): {
                                    case_878 = INT64_C(1);
                                    break;
                                }
                                case INT64_C(220052207): {
                                    case_878 = INT64_C(1);
                                    break;
                                }
                                case INT64_C(219778422): {
                                    case_878 = INT64_C(1);
                                    break;
                                }
                                case INT64_C(220068320): {
                                    case_878 = INT64_C(1);
                                    break;
                                }
                                case INT64_C(260346884): {
                                    case_878 = INT64_C(1);
                                    break;
                                }
                                case INT64_C(224481049): {
                                    case_878 = INT64_C(1);
                                    break;
                                }
                                case INT64_C(262457687): {
                                    case_878 = INT64_C(1);
                                    break;
                                }
                                case INT64_C(224547597): {
                                    case_878 = INT64_C(1);
                                    break;
                                }
                                case INT64_C(260276799): {
                                    case_878 = INT64_C(1);
                                    break;
                                }
                                case INT64_C(224481180): {
                                    case_878 = INT64_C(1);
                                    break;
                                }
                                case INT64_C(826839025): {
                                    case_878 = INT64_C(1);
                                    break;
                                }
                                case INT64_C(790973190): {
                                    case_878 = INT64_C(1);
                                    break;
                                }
                                case INT64_C(828949828): {
                                    case_878 = INT64_C(1);
                                    break;
                                }
                                case INT64_C(1713596): {
                                    case_878 = INT64_C(1);
                                    break;
                                }
                                case INT64_C(1986845): {
                                    case_878 = INT64_C(1);
                                    break;
                                }
                                case INT64_C(1714103): {
                                    case_878 = INT64_C(1);
                                    break;
                                }
                                case INT64_C(219778423): {
                                    case_878 = INT64_C(1);
                                    break;
                                }
                                case INT64_C(220051672): {
                                    case_878 = INT64_C(1);
                                    break;
                                }
                                case INT64_C(219778930): {
                                    case_878 = INT64_C(1);
                                    break;
                                }
                                case INT64_C(283705086): {
                                    case_878 = INT64_C(1);
                                    break;
                                }
                                case INT64_C(224308133): {
                                    case_878 = INT64_C(1);
                                    break;
                                }
                                case INT64_C(420295318): {
                                    case_878 = INT64_C(1);
                                    break;
                                }
                                case INT64_C(824551211): {
                                    case_878 = INT64_C(1);
                                    break;
                                }
                                case INT64_C(354247632): {
                                    case_878 = INT64_C(1);
                                    break;
                                }
                                case INT64_C(230982984): {
                                    case_878 = INT64_C(1);
                                    break;
                                }
                                case INT64_C(553441405): {
                                    case_878 = INT64_C(1);
                                    break;
                                }
                                case INT64_C(138294544): {
                                    case_878 = INT64_C(1);
                                    break;
                                }
                                case INT64_C(1748297): {
                                    case_878 = INT64_C(1);
                                    break;
                                }
                                case INT64_C(229027006): {
                                    case_878 = INT64_C(1);
                                    break;
                                }
                                case INT64_C(244712290): {
                                    case_878 = INT64_C(1);
                                    break;
                                }
                                case INT64_C(260345973): {
                                    case_878 = INT64_C(1);
                                    break;
                                }
                                case INT64_C(231274442): {
                                    case_878 = INT64_C(1);
                                    break;
                                }
                                case INT64_C(1729434): {
                                    case_878 = INT64_C(1);
                                    break;
                                }
                                case INT64_C(1969161): {
                                    case_878 = INT64_C(1);
                                    break;
                                }
                                case INT64_C(992605559): {
                                    case_878 = INT64_C(1);
                                    break;
                                }
                                case INT64_C(430838469): {
                                    case_878 = INT64_C(1);
                                    break;
                                }
                                case INT64_C(184475947): {
                                    case_878 = INT64_C(1);
                                    break;
                                }
                                case INT64_C(224581785): {
                                    case_878 = INT64_C(1);
                                    break;
                                }
                                default: {
                                    case_878 = INT64_C(0);
                                    break;
                                }
                            }
                            int64_t scrut_877 = case_878;
                            if ((scrut_877 == INT64_C(1))) {
                                (out)->tag = mak_ir_tag_10;
                                double case_879 = 0.0;
                                switch (a0) {
                                    case INT64_C(1677570): {
                                        case_879 = mak_ir_ext_7(mak_local_es_374);
                                        break;
                                    }
                                    case INT64_C(230982852): {
                                        case_879 = mak_ir_ext_7(mak_local_es_374);
                                        break;
                                    }
                                    case INT64_C(260484708): {
                                        case_879 = mak_ir_ext_5(mak_local_es_374);
                                        break;
                                    }
                                    case INT64_C(224257837): {
                                        case_879 = mak_ir_ext_6(mak_local_es_374);
                                        break;
                                    }
                                    case INT64_C(1749093): {
                                        case_879 = mak_ir_ext_15(mak_local_es_374);
                                        break;
                                    }
                                    case INT64_C(229131233): {
                                        case_879 = mak_ir_ext_16(mak_local_es_374);
                                        break;
                                    }
                                    case INT64_C(16199091): {
                                        case_879 = mak_ir_ext_17(mak_local_es_374);
                                        break;
                                    }
                                    case INT64_C(1868032): {
                                        case_879 = mak_ir_ext_18(mak_local_es_374);
                                        break;
                                    }
                                    case INT64_C(14258): {
                                        case_879 = mak_ir_ext_18(mak_local_es_374);
                                        break;
                                    }
                                    case INT64_C(244712242): {
                                        case_879 = mak_ir_ext_19(mak_local_es_374);
                                        break;
                                    }
                                    case INT64_C(57303395): {
                                        case_879 = mak_ir_ext_20(mak_local_es_374);
                                        break;
                                    }
                                    case INT64_C(57303459): {
                                        case_879 = mak_ir_ext_21(mak_local_es_374);
                                        break;
                                    }
                                    case INT64_C(1987380): {
                                        case_879 = mak_ir_ext_22(mak_local_es_374);
                                        break;
                                    }
                                    case INT64_C(1713595): {
                                        case_879 = mak_ir_ext_23(mak_local_es_374);
                                        break;
                                    }
                                    case INT64_C(2003493): {
                                        case_879 = mak_ir_ext_24(mak_local_es_374);
                                        break;
                                    }
                                    case INT64_C(220052207): {
                                        case_879 = mak_ir_ext_25(mak_local_es_374);
                                        break;
                                    }
                                    case INT64_C(219778422): {
                                        case_879 = mak_ir_ext_26(mak_local_es_374);
                                        break;
                                    }
                                    case INT64_C(220068320): {
                                        case_879 = mak_ir_ext_27(mak_local_es_374);
                                        break;
                                    }
                                    case INT64_C(260346884): {
                                        case_879 = mak_ir_ext_29(mak_local_es_374);
                                        break;
                                    }
                                    case INT64_C(224481049): {
                                        case_879 = mak_ir_ext_30(mak_local_es_374);
                                        break;
                                    }
                                    case INT64_C(262457687): {
                                        case_879 = mak_ir_ext_31(mak_local_es_374);
                                        break;
                                    }
                                    case INT64_C(224547597): {
                                        double eval_880 = mak_ir_ext_29(mak_local_es_374);
                                        case_879 = ((double)(1.00000000000000000) / (double)(eval_880));
                                        break;
                                    }
                                    case INT64_C(260276799): {
                                        double eval_881 = mak_ir_ext_30(mak_local_es_374);
                                        case_879 = ((double)(1.00000000000000000) / (double)(eval_881));
                                        break;
                                    }
                                    case INT64_C(224481180): {
                                        double eval_882 = mak_ir_ext_30(mak_local_es_374);
                                        double eval_883 = mak_ir_ext_29(mak_local_es_374);
                                        case_879 = ((double)(eval_882) / (double)(eval_883));
                                        break;
                                    }
                                    case INT64_C(826839025): {
                                        case_879 = mak_ir_ext_32(mak_local_es_374);
                                        break;
                                    }
                                    case INT64_C(790973190): {
                                        case_879 = mak_ir_ext_33(mak_local_es_374);
                                        break;
                                    }
                                    case INT64_C(828949828): {
                                        case_879 = mak_ir_ext_34(mak_local_es_374);
                                        break;
                                    }
                                    case INT64_C(1713596): {
                                        double eval_884 = mak_ir_ext_23(mak_local_es_374);
                                        double eval_885 = mak_ir_ext_22(mak_local_es_374);
                                        case_879 = ((double)(eval_884) / (double)(eval_885));
                                        break;
                                    }
                                    case INT64_C(1986845): {
                                        double eval_886 = mak_ir_ext_23(mak_local_es_374);
                                        case_879 = ((double)(1.00000000000000000) / (double)(eval_886));
                                        break;
                                    }
                                    case INT64_C(1714103): {
                                        double eval_887 = mak_ir_ext_22(mak_local_es_374);
                                        case_879 = ((double)(1.00000000000000000) / (double)(eval_887));
                                        break;
                                    }
                                    case INT64_C(219778423): {
                                        double eval_888 = mak_ir_ext_27(mak_local_es_374);
                                        case_879 = ((double)(1.57079632679489656) - (double)(eval_888));
                                        break;
                                    }
                                    case INT64_C(220051672): {
                                        case_879 = mak_ir_ext_26(((double)(1.00000000000000000) / (double)(mak_local_es_374)));
                                        break;
                                    }
                                    case INT64_C(219778930): {
                                        case_879 = mak_ir_ext_25(((double)(1.00000000000000000) / (double)(mak_local_es_374)));
                                        break;
                                    }
                                    case INT64_C(283705086): {
                                        case_879 = mak_ir_ext_8(mak_local_es_374);
                                        break;
                                    }
                                    case INT64_C(224308133): {
                                        case_879 = mak_ir_ext_9(mak_local_es_374);
                                        break;
                                    }
                                    case INT64_C(420295318): {
                                        case_879 = mak_ir_ext_10(mak_local_es_374);
                                        break;
                                    }
                                    case INT64_C(824551211): {
                                        case_879 = mak_ir_ext_11(mak_local_es_374);
                                        break;
                                    }
                                    case INT64_C(354247632): {
                                        case_879 = mak_ir_ext_35(((double)(mak_local_es_374) + (double)(1.00000000000000000)));
                                        break;
                                    }
                                    case INT64_C(230982984): {
                                        case_879 = mak_ir_ext_35(((double)(mak_local_es_374) + (double)(1.00000000000000000)));
                                        break;
                                    }
                                    case INT64_C(553441405): {
                                        case_879 = mak_ir_ext_35(mak_local_es_374);
                                        break;
                                    }
                                    case INT64_C(138294544): {
                                        case_879 = mak_ir_ext_36(mak_local_es_374);
                                        break;
                                    }
                                    case INT64_C(1748297): {
                                        case_879 = mak_ir_ext_38(mak_local_es_374);
                                        break;
                                    }
                                    case INT64_C(229027006): {
                                        case_879 = mak_ir_ext_39(mak_local_es_374);
                                        break;
                                    }
                                    case INT64_C(244712290): {
                                        case_879 = mak_ir_ext_40(mak_local_es_374);
                                        break;
                                    }
                                    case INT64_C(260345973): {
                                        double case_889 = 0.0;
                                        int32_t scrut_890 = (int32_t)(mak_local_es_374) > (int32_t)(0.00000000000000000);
                                        if ((scrut_890 == INT64_C(1))) {
                                            case_889 = 1.00000000000000000;
                                        }
                                        else if (1) {
                                            double case_891 = 0.0;
                                            int32_t scrut_892 = (int32_t)(mak_local_es_374) < (int32_t)(0.00000000000000000);
                                            if ((scrut_892 == INT64_C(1))) {
                                                case_891 = -1.00000000000000000;
                                            }
                                            else if (1) {
                                                case_891 = 0.00000000000000000;
                                            }
                                            case_889 = case_891;
                                        }
                                        case_879 = case_889;
                                        break;
                                    }
                                    case INT64_C(231274442): {
                                        double eval_893 = mak_ir_ext_10(mak_local_es_374);
                                        case_879 = ((double)(mak_local_es_374) - (double)(eval_893));
                                        break;
                                    }
                                    case INT64_C(1729434): {
                                        case_879 = ((double)(mak_local_es_374) * (double)(57.29577951308232286));
                                        break;
                                    }
                                    case INT64_C(1969161): {
                                        case_879 = ((double)(mak_local_es_374) * (double)(0.01745329251994330));
                                        break;
                                    }
                                    case INT64_C(992605559): {
                                        case_879 = ((double)(mak_local_es_374) * (double)(0.01745329251994330));
                                        break;
                                    }
                                    case INT64_C(430838469): {
                                        case_879 = ((double)(mak_local_es_374) * (double)(57.29577951308232286));
                                        break;
                                    }
                                    case INT64_C(184475947): {
                                        case_879 = ((double)(mak_local_es_374) * (double)(mak_local_es_374));
                                        break;
                                    }
                                    case INT64_C(224581785): {
                                        double eval_894 = ((double)(mak_local_es_374) * (double)(mak_local_es_374));
                                        case_879 = ((double)(eval_894) * (double)(mak_local_es_374));
                                        break;
                                    }
                                    default: {
                                        case_879 = 0.00000000000000000;
                                        break;
                                    }
                                }
                                (out)->v.v10.f0 = case_879;
                                (out)->v.v10.f1 = mak_local_eu_376;
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
            break;
        }
    }
    return;
}

static inline void mak_ir_fn_19(int64_t a0, struct mak__a * a1, struct mak__f *out) {
    struct mak__f *arena_call_896 = (struct mak__f *)__builtin_alloca(sizeof(struct mak__f));
        mak_ir_fn_16(a1, (struct mak__f *)(arena_call_896));
    struct mak__f * scrut_895 = arena_call_896;
    if ((scrut_895 != NULL && (scrut_895)->tag == mak_ir_tag_11)) {
        *(out) = mak_ir_ctor_obj_11;
    }
    else if ((scrut_895 != NULL && (scrut_895)->tag == mak_ir_tag_10)) {
        double mak_local_ff_377 = (scrut_895)->v.v10.f0;
        struct mak__a * mak_local_fg_378 = (scrut_895)->v.v10.f1;
        struct mak__a *arena_call_898 = (struct mak__a *)__builtin_alloca(sizeof(struct mak__a));
            mak_ir_fn_2(mak_local_fg_378, (struct mak__a *)(arena_call_898));
        struct mak__a * scrut_897 = arena_call_898;
        if ((scrut_897 != NULL && (scrut_897)->tag == mak_ir_tag_1) && (scrut_897->v.v1.f0 == INT64_C(44))) {
            struct mak__a * mak_local_fh_379 = (scrut_897)->v.v1.f1;
            struct mak__f *arena_call_900 = (struct mak__f *)__builtin_alloca(sizeof(struct mak__f));
                mak_ir_fn_16(mak_local_fh_379, (struct mak__f *)(arena_call_900));
            struct mak__f * scrut_899 = arena_call_900;
            if ((scrut_899 != NULL && (scrut_899)->tag == mak_ir_tag_11)) {
                *(out) = mak_ir_ctor_obj_11;
            }
            else if ((scrut_899 != NULL && (scrut_899)->tag == mak_ir_tag_10)) {
                double mak_local_fi_380 = (scrut_899)->v.v10.f0;
                struct mak__a * mak_local_fj_381 = (scrut_899)->v.v10.f1;
                struct mak__a *arena_call_902 = (struct mak__a *)__builtin_alloca(sizeof(struct mak__a));
                    mak_ir_fn_2(mak_local_fj_381, (struct mak__a *)(arena_call_902));
                struct mak__a * scrut_901 = arena_call_902;
                if ((scrut_901 != NULL && (scrut_901)->tag == mak_ir_tag_1) && (scrut_901->v.v1.f0 == INT64_C(44))) {
                    struct mak__a * mak_local_fk_382 = (scrut_901)->v.v1.f1;
                    struct mak__f *arena_call_904 = (struct mak__f *)__builtin_alloca(sizeof(struct mak__f));
                        mak_ir_fn_16(mak_local_fk_382, (struct mak__f *)(arena_call_904));
                    struct mak__f * scrut_903 = arena_call_904;
                    if ((scrut_903 != NULL && (scrut_903)->tag == mak_ir_tag_11)) {
                        *(out) = mak_ir_ctor_obj_11;
                    }
                    else if ((scrut_903 != NULL && (scrut_903)->tag == mak_ir_tag_10)) {
                        double mak_local_fl_383 = (scrut_903)->v.v10.f0;
                        struct mak__a * mak_local_fm_384 = (scrut_903)->v.v10.f1;
                        struct mak__a *arena_call_906 = (struct mak__a *)__builtin_alloca(sizeof(struct mak__a));
                            mak_ir_fn_2(mak_local_fm_384, (struct mak__a *)(arena_call_906));
                        struct mak__a * scrut_905 = arena_call_906;
                        if ((scrut_905 != NULL && (scrut_905)->tag == mak_ir_tag_1) && (scrut_905->v.v1.f0 == INT64_C(41))) {
                            struct mak__a * mak_local_fo_385 = (scrut_905)->v.v1.f1;
                            switch (a0) {
                                case INT64_C(399964812): {
                                    (out)->tag = mak_ir_tag_10;
                                    double case_907 = 0.0;
                                    switch (a0) {
                                        case INT64_C(399964812): {
                                            double case_908 = 0.0;
                                            int32_t scrut_909 = (int32_t)(mak_local_ff_377) < (int32_t)(mak_local_fi_380);
                                            if ((scrut_909 == INT64_C(1))) {
                                                case_908 = mak_local_fi_380;
                                            }
                                            else if (1) {
                                                double case_910 = 0.0;
                                                int32_t scrut_911 = (int32_t)(mak_local_ff_377) > (int32_t)(mak_local_fl_383);
                                                if ((scrut_911 == INT64_C(1))) {
                                                    case_910 = mak_local_fl_383;
                                                }
                                                else if (1) {
                                                    case_910 = mak_local_ff_377;
                                                }
                                                case_908 = case_910;
                                            }
                                            case_907 = case_908;
                                            break;
                                        }
                                        case INT64_C(1764798): {
                                            case_907 = mak_ir_ext_45(mak_local_ff_377, mak_local_fi_380, mak_local_fl_383);
                                            break;
                                        }
                                        default: {
                                            case_907 = mak_local_ff_377;
                                            break;
                                        }
                                    }
                                    (out)->v.v10.f0 = case_907;
                                    (out)->v.v10.f1 = mak_local_fo_385;
                                    break;
                                }
                                case INT64_C(1764798): {
                                    (out)->tag = mak_ir_tag_10;
                                    double case_912 = 0.0;
                                    switch (a0) {
                                        case INT64_C(399964812): {
                                            double case_913 = 0.0;
                                            int32_t scrut_914 = (int32_t)(mak_local_ff_377) < (int32_t)(mak_local_fi_380);
                                            if ((scrut_914 == INT64_C(1))) {
                                                case_913 = mak_local_fi_380;
                                            }
                                            else if (1) {
                                                double case_915 = 0.0;
                                                int32_t scrut_916 = (int32_t)(mak_local_ff_377) > (int32_t)(mak_local_fl_383);
                                                if ((scrut_916 == INT64_C(1))) {
                                                    case_915 = mak_local_fl_383;
                                                }
                                                else if (1) {
                                                    case_915 = mak_local_ff_377;
                                                }
                                                case_913 = case_915;
                                            }
                                            case_912 = case_913;
                                            break;
                                        }
                                        case INT64_C(1764798): {
                                            case_912 = mak_ir_ext_45(mak_local_ff_377, mak_local_fi_380, mak_local_fl_383);
                                            break;
                                        }
                                        default: {
                                            case_912 = mak_local_ff_377;
                                            break;
                                        }
                                    }
                                    (out)->v.v10.f0 = case_912;
                                    (out)->v.v10.f1 = mak_local_fo_385;
                                    break;
                                }
                                default: {
                                    *(out) = mak_ir_ctor_obj_11;
                                    break;
                                }
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

static inline double mak_ir_fn_20(int64_t a0, int64_t a1, int64_t a2, int64_t a3) {
    switch (a0) {
        case INT64_C(399964812): {
            int32_t scrut_917 = (int32_t)(a1) < (int32_t)(a2);
            if ((scrut_917 == INT64_C(1))) {
                return a2;
            }
            else if (1) {
                int32_t scrut_918 = (int32_t)(a1) > (int32_t)(a3);
                if ((scrut_918 == INT64_C(1))) {
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

static inline void mak_ir_fn_21(int64_t a0, struct mak__a * a1, struct mak__f *out) {
    struct mak__f *arena_call_920 = (struct mak__f *)__builtin_alloca(sizeof(struct mak__f));
        mak_ir_fn_16(a1, (struct mak__f *)(arena_call_920));
    struct mak__f * scrut_919 = arena_call_920;
    if ((scrut_919 != NULL && (scrut_919)->tag == mak_ir_tag_11)) {
        *(out) = mak_ir_ctor_obj_11;
    }
    else if ((scrut_919 != NULL && (scrut_919)->tag == mak_ir_tag_10)) {
        double mak_local_ex_386 = (scrut_919)->v.v10.f0;
        struct mak__a * mak_local_ey_387 = (scrut_919)->v.v10.f1;
        struct mak__a *arena_call_922 = (struct mak__a *)__builtin_alloca(sizeof(struct mak__a));
            mak_ir_fn_2(mak_local_ey_387, (struct mak__a *)(arena_call_922));
        struct mak__a * scrut_921 = arena_call_922;
        if ((scrut_921 != NULL && (scrut_921)->tag == mak_ir_tag_1) && (scrut_921->v.v1.f0 == INT64_C(44))) {
            struct mak__a * mak_local_ez_388 = (scrut_921)->v.v1.f1;
            struct mak__f *arena_call_924 = (struct mak__f *)__builtin_alloca(sizeof(struct mak__f));
                mak_ir_fn_16(mak_local_ez_388, (struct mak__f *)(arena_call_924));
            struct mak__f * scrut_923 = arena_call_924;
            if ((scrut_923 != NULL && (scrut_923)->tag == mak_ir_tag_11)) {
                *(out) = mak_ir_ctor_obj_11;
            }
            else if ((scrut_923 != NULL && (scrut_923)->tag == mak_ir_tag_10)) {
                double mak_local_fa_389 = (scrut_923)->v.v10.f0;
                struct mak__a * mak_local_fb_390 = (scrut_923)->v.v10.f1;
                struct mak__a *arena_call_926 = (struct mak__a *)__builtin_alloca(sizeof(struct mak__a));
                    mak_ir_fn_2(mak_local_fb_390, (struct mak__a *)(arena_call_926));
                struct mak__a * scrut_925 = arena_call_926;
                if ((scrut_925 != NULL && (scrut_925)->tag == mak_ir_tag_1) && (scrut_925->v.v1.f0 == INT64_C(41))) {
                    struct mak__a * mak_local_fc_391 = (scrut_925)->v.v1.f1;
                    int64_t case_928 = 0;
                    switch (a0) {
                        case INT64_C(828949774): {
                            case_928 = INT64_C(1);
                            break;
                        }
                        case INT64_C(231190472): {
                            case_928 = INT64_C(1);
                            break;
                        }
                        case INT64_C(1885190): {
                            case_928 = INT64_C(1);
                            break;
                        }
                        case INT64_C(1969694): {
                            case_928 = INT64_C(1);
                            break;
                        }
                        case INT64_C(510764621): {
                            case_928 = INT64_C(1);
                            break;
                        }
                        case INT64_C(1936692): {
                            case_928 = INT64_C(1);
                            break;
                        }
                        case INT64_C(258201902): {
                            case_928 = INT64_C(1);
                            break;
                        }
                        case INT64_C(548941060): {
                            case_928 = INT64_C(1);
                            break;
                        }
                        case INT64_C(901947274): {
                            case_928 = INT64_C(1);
                            break;
                        }
                        case INT64_C(1780652): {
                            case_928 = INT64_C(1);
                            break;
                        }
                        case INT64_C(1866466): {
                            case_928 = INT64_C(1);
                            break;
                        }
                        case INT64_C(1900793): {
                            case_928 = INT64_C(1);
                            break;
                        }
                        case INT64_C(224480257): {
                            case_928 = INT64_C(1);
                            break;
                        }
                        case INT64_C(1902496): {
                            case_928 = INT64_C(1);
                            break;
                        }
                        case INT64_C(253534496): {
                            case_928 = INT64_C(1);
                            break;
                        }
                        case INT64_C(1884414): {
                            case_928 = INT64_C(1);
                            break;
                        }
                        case INT64_C(1883376): {
                            case_928 = INT64_C(1);
                            break;
                        }
                        case INT64_C(262106026): {
                            case_928 = INT64_C(1);
                            break;
                        }
                        case INT64_C(231035246): {
                            case_928 = INT64_C(1);
                            break;
                        }
                        case INT64_C(231188658): {
                            case_928 = INT64_C(1);
                            break;
                        }
                        case INT64_C(231189696): {
                            case_928 = INT64_C(1);
                            break;
                        }
                        case INT64_C(743972013): {
                            case_928 = INT64_C(1);
                            break;
                        }
                        default: {
                            case_928 = INT64_C(0);
                            break;
                        }
                    }
                    int64_t scrut_927 = case_928;
                    if ((scrut_927 == INT64_C(1))) {
                        (out)->tag = mak_ir_tag_10;
                        (out)->v.v10.f0 = mak_ir_fn_9(a0, mak_local_ex_386, mak_local_fa_389);
                        (out)->v.v10.f1 = mak_local_fc_391;
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

static inline void mak_ir_fn_22(int64_t a0, struct mak__a * a1, struct mak__f *out) {
    struct mak__f *arena_call_930 = (struct mak__f *)__builtin_alloca(sizeof(struct mak__f));
        mak_ir_fn_16(a1, (struct mak__f *)(arena_call_930));
    struct mak__f * scrut_929 = arena_call_930;
    if ((scrut_929 != NULL && (scrut_929)->tag == mak_ir_tag_11)) {
        *(out) = mak_ir_ctor_obj_11;
    }
    else if ((scrut_929 != NULL && (scrut_929)->tag == mak_ir_tag_10)) {
        double mak_local_es_392 = (scrut_929)->v.v10.f0;
        struct mak__a * mak_local_et_393 = (scrut_929)->v.v10.f1;
        struct mak__a *arena_call_932 = (struct mak__a *)__builtin_alloca(sizeof(struct mak__a));
            mak_ir_fn_2(mak_local_et_393, (struct mak__a *)(arena_call_932));
        struct mak__a * scrut_931 = arena_call_932;
        if ((scrut_931 != NULL && (scrut_931)->tag == mak_ir_tag_1) && (scrut_931->v.v1.f0 == INT64_C(41))) {
            struct mak__a * mak_local_eu_394 = (scrut_931)->v.v1.f1;
            int64_t case_934 = 0;
            switch (a0) {
                case INT64_C(1677570): {
                    case_934 = INT64_C(1);
                    break;
                }
                case INT64_C(230982852): {
                    case_934 = INT64_C(1);
                    break;
                }
                case INT64_C(260484708): {
                    case_934 = INT64_C(1);
                    break;
                }
                case INT64_C(224257837): {
                    case_934 = INT64_C(1);
                    break;
                }
                case INT64_C(1749093): {
                    case_934 = INT64_C(1);
                    break;
                }
                case INT64_C(229131233): {
                    case_934 = INT64_C(1);
                    break;
                }
                case INT64_C(16199091): {
                    case_934 = INT64_C(1);
                    break;
                }
                case INT64_C(1868032): {
                    case_934 = INT64_C(1);
                    break;
                }
                case INT64_C(14258): {
                    case_934 = INT64_C(1);
                    break;
                }
                case INT64_C(244712242): {
                    case_934 = INT64_C(1);
                    break;
                }
                case INT64_C(57303395): {
                    case_934 = INT64_C(1);
                    break;
                }
                case INT64_C(57303459): {
                    case_934 = INT64_C(1);
                    break;
                }
                case INT64_C(1987380): {
                    case_934 = INT64_C(1);
                    break;
                }
                case INT64_C(1713595): {
                    case_934 = INT64_C(1);
                    break;
                }
                case INT64_C(2003493): {
                    case_934 = INT64_C(1);
                    break;
                }
                case INT64_C(220052207): {
                    case_934 = INT64_C(1);
                    break;
                }
                case INT64_C(219778422): {
                    case_934 = INT64_C(1);
                    break;
                }
                case INT64_C(220068320): {
                    case_934 = INT64_C(1);
                    break;
                }
                case INT64_C(260346884): {
                    case_934 = INT64_C(1);
                    break;
                }
                case INT64_C(224481049): {
                    case_934 = INT64_C(1);
                    break;
                }
                case INT64_C(262457687): {
                    case_934 = INT64_C(1);
                    break;
                }
                case INT64_C(224547597): {
                    case_934 = INT64_C(1);
                    break;
                }
                case INT64_C(260276799): {
                    case_934 = INT64_C(1);
                    break;
                }
                case INT64_C(224481180): {
                    case_934 = INT64_C(1);
                    break;
                }
                case INT64_C(826839025): {
                    case_934 = INT64_C(1);
                    break;
                }
                case INT64_C(790973190): {
                    case_934 = INT64_C(1);
                    break;
                }
                case INT64_C(828949828): {
                    case_934 = INT64_C(1);
                    break;
                }
                case INT64_C(1713596): {
                    case_934 = INT64_C(1);
                    break;
                }
                case INT64_C(1986845): {
                    case_934 = INT64_C(1);
                    break;
                }
                case INT64_C(1714103): {
                    case_934 = INT64_C(1);
                    break;
                }
                case INT64_C(219778423): {
                    case_934 = INT64_C(1);
                    break;
                }
                case INT64_C(220051672): {
                    case_934 = INT64_C(1);
                    break;
                }
                case INT64_C(219778930): {
                    case_934 = INT64_C(1);
                    break;
                }
                case INT64_C(283705086): {
                    case_934 = INT64_C(1);
                    break;
                }
                case INT64_C(224308133): {
                    case_934 = INT64_C(1);
                    break;
                }
                case INT64_C(420295318): {
                    case_934 = INT64_C(1);
                    break;
                }
                case INT64_C(824551211): {
                    case_934 = INT64_C(1);
                    break;
                }
                case INT64_C(354247632): {
                    case_934 = INT64_C(1);
                    break;
                }
                case INT64_C(230982984): {
                    case_934 = INT64_C(1);
                    break;
                }
                case INT64_C(553441405): {
                    case_934 = INT64_C(1);
                    break;
                }
                case INT64_C(138294544): {
                    case_934 = INT64_C(1);
                    break;
                }
                case INT64_C(1748297): {
                    case_934 = INT64_C(1);
                    break;
                }
                case INT64_C(229027006): {
                    case_934 = INT64_C(1);
                    break;
                }
                case INT64_C(244712290): {
                    case_934 = INT64_C(1);
                    break;
                }
                case INT64_C(260345973): {
                    case_934 = INT64_C(1);
                    break;
                }
                case INT64_C(231274442): {
                    case_934 = INT64_C(1);
                    break;
                }
                case INT64_C(1729434): {
                    case_934 = INT64_C(1);
                    break;
                }
                case INT64_C(1969161): {
                    case_934 = INT64_C(1);
                    break;
                }
                case INT64_C(992605559): {
                    case_934 = INT64_C(1);
                    break;
                }
                case INT64_C(430838469): {
                    case_934 = INT64_C(1);
                    break;
                }
                case INT64_C(184475947): {
                    case_934 = INT64_C(1);
                    break;
                }
                case INT64_C(224581785): {
                    case_934 = INT64_C(1);
                    break;
                }
                default: {
                    case_934 = INT64_C(0);
                    break;
                }
            }
            int64_t scrut_933 = case_934;
            if ((scrut_933 == INT64_C(1))) {
                (out)->tag = mak_ir_tag_10;
                double case_935 = 0.0;
                switch (a0) {
                    case INT64_C(1677570): {
                        case_935 = mak_ir_ext_7(mak_local_es_392);
                        break;
                    }
                    case INT64_C(230982852): {
                        case_935 = mak_ir_ext_7(mak_local_es_392);
                        break;
                    }
                    case INT64_C(260484708): {
                        case_935 = mak_ir_ext_5(mak_local_es_392);
                        break;
                    }
                    case INT64_C(224257837): {
                        case_935 = mak_ir_ext_6(mak_local_es_392);
                        break;
                    }
                    case INT64_C(1749093): {
                        case_935 = mak_ir_ext_15(mak_local_es_392);
                        break;
                    }
                    case INT64_C(229131233): {
                        case_935 = mak_ir_ext_16(mak_local_es_392);
                        break;
                    }
                    case INT64_C(16199091): {
                        case_935 = mak_ir_ext_17(mak_local_es_392);
                        break;
                    }
                    case INT64_C(1868032): {
                        case_935 = mak_ir_ext_18(mak_local_es_392);
                        break;
                    }
                    case INT64_C(14258): {
                        case_935 = mak_ir_ext_18(mak_local_es_392);
                        break;
                    }
                    case INT64_C(244712242): {
                        case_935 = mak_ir_ext_19(mak_local_es_392);
                        break;
                    }
                    case INT64_C(57303395): {
                        case_935 = mak_ir_ext_20(mak_local_es_392);
                        break;
                    }
                    case INT64_C(57303459): {
                        case_935 = mak_ir_ext_21(mak_local_es_392);
                        break;
                    }
                    case INT64_C(1987380): {
                        case_935 = mak_ir_ext_22(mak_local_es_392);
                        break;
                    }
                    case INT64_C(1713595): {
                        case_935 = mak_ir_ext_23(mak_local_es_392);
                        break;
                    }
                    case INT64_C(2003493): {
                        case_935 = mak_ir_ext_24(mak_local_es_392);
                        break;
                    }
                    case INT64_C(220052207): {
                        case_935 = mak_ir_ext_25(mak_local_es_392);
                        break;
                    }
                    case INT64_C(219778422): {
                        case_935 = mak_ir_ext_26(mak_local_es_392);
                        break;
                    }
                    case INT64_C(220068320): {
                        case_935 = mak_ir_ext_27(mak_local_es_392);
                        break;
                    }
                    case INT64_C(260346884): {
                        case_935 = mak_ir_ext_29(mak_local_es_392);
                        break;
                    }
                    case INT64_C(224481049): {
                        case_935 = mak_ir_ext_30(mak_local_es_392);
                        break;
                    }
                    case INT64_C(262457687): {
                        case_935 = mak_ir_ext_31(mak_local_es_392);
                        break;
                    }
                    case INT64_C(224547597): {
                        double eval_936 = mak_ir_ext_29(mak_local_es_392);
                        case_935 = ((double)(1.00000000000000000) / (double)(eval_936));
                        break;
                    }
                    case INT64_C(260276799): {
                        double eval_937 = mak_ir_ext_30(mak_local_es_392);
                        case_935 = ((double)(1.00000000000000000) / (double)(eval_937));
                        break;
                    }
                    case INT64_C(224481180): {
                        double eval_938 = mak_ir_ext_30(mak_local_es_392);
                        double eval_939 = mak_ir_ext_29(mak_local_es_392);
                        case_935 = ((double)(eval_938) / (double)(eval_939));
                        break;
                    }
                    case INT64_C(826839025): {
                        case_935 = mak_ir_ext_32(mak_local_es_392);
                        break;
                    }
                    case INT64_C(790973190): {
                        case_935 = mak_ir_ext_33(mak_local_es_392);
                        break;
                    }
                    case INT64_C(828949828): {
                        case_935 = mak_ir_ext_34(mak_local_es_392);
                        break;
                    }
                    case INT64_C(1713596): {
                        double eval_940 = mak_ir_ext_23(mak_local_es_392);
                        double eval_941 = mak_ir_ext_22(mak_local_es_392);
                        case_935 = ((double)(eval_940) / (double)(eval_941));
                        break;
                    }
                    case INT64_C(1986845): {
                        double eval_942 = mak_ir_ext_23(mak_local_es_392);
                        case_935 = ((double)(1.00000000000000000) / (double)(eval_942));
                        break;
                    }
                    case INT64_C(1714103): {
                        double eval_943 = mak_ir_ext_22(mak_local_es_392);
                        case_935 = ((double)(1.00000000000000000) / (double)(eval_943));
                        break;
                    }
                    case INT64_C(219778423): {
                        double eval_944 = mak_ir_ext_27(mak_local_es_392);
                        case_935 = ((double)(1.57079632679489656) - (double)(eval_944));
                        break;
                    }
                    case INT64_C(220051672): {
                        case_935 = mak_ir_ext_26(((double)(1.00000000000000000) / (double)(mak_local_es_392)));
                        break;
                    }
                    case INT64_C(219778930): {
                        case_935 = mak_ir_ext_25(((double)(1.00000000000000000) / (double)(mak_local_es_392)));
                        break;
                    }
                    case INT64_C(283705086): {
                        case_935 = mak_ir_ext_8(mak_local_es_392);
                        break;
                    }
                    case INT64_C(224308133): {
                        case_935 = mak_ir_ext_9(mak_local_es_392);
                        break;
                    }
                    case INT64_C(420295318): {
                        case_935 = mak_ir_ext_10(mak_local_es_392);
                        break;
                    }
                    case INT64_C(824551211): {
                        case_935 = mak_ir_ext_11(mak_local_es_392);
                        break;
                    }
                    case INT64_C(354247632): {
                        case_935 = mak_ir_ext_35(((double)(mak_local_es_392) + (double)(1.00000000000000000)));
                        break;
                    }
                    case INT64_C(230982984): {
                        case_935 = mak_ir_ext_35(((double)(mak_local_es_392) + (double)(1.00000000000000000)));
                        break;
                    }
                    case INT64_C(553441405): {
                        case_935 = mak_ir_ext_35(mak_local_es_392);
                        break;
                    }
                    case INT64_C(138294544): {
                        case_935 = mak_ir_ext_36(mak_local_es_392);
                        break;
                    }
                    case INT64_C(1748297): {
                        case_935 = mak_ir_ext_38(mak_local_es_392);
                        break;
                    }
                    case INT64_C(229027006): {
                        case_935 = mak_ir_ext_39(mak_local_es_392);
                        break;
                    }
                    case INT64_C(244712290): {
                        case_935 = mak_ir_ext_40(mak_local_es_392);
                        break;
                    }
                    case INT64_C(260345973): {
                        double case_945 = 0.0;
                        int32_t scrut_946 = (int32_t)(mak_local_es_392) > (int32_t)(0.00000000000000000);
                        if ((scrut_946 == INT64_C(1))) {
                            case_945 = 1.00000000000000000;
                        }
                        else if (1) {
                            double case_947 = 0.0;
                            int32_t scrut_948 = (int32_t)(mak_local_es_392) < (int32_t)(0.00000000000000000);
                            if ((scrut_948 == INT64_C(1))) {
                                case_947 = -1.00000000000000000;
                            }
                            else if (1) {
                                case_947 = 0.00000000000000000;
                            }
                            case_945 = case_947;
                        }
                        case_935 = case_945;
                        break;
                    }
                    case INT64_C(231274442): {
                        double eval_949 = mak_ir_ext_10(mak_local_es_392);
                        case_935 = ((double)(mak_local_es_392) - (double)(eval_949));
                        break;
                    }
                    case INT64_C(1729434): {
                        case_935 = ((double)(mak_local_es_392) * (double)(57.29577951308232286));
                        break;
                    }
                    case INT64_C(1969161): {
                        case_935 = ((double)(mak_local_es_392) * (double)(0.01745329251994330));
                        break;
                    }
                    case INT64_C(992605559): {
                        case_935 = ((double)(mak_local_es_392) * (double)(0.01745329251994330));
                        break;
                    }
                    case INT64_C(430838469): {
                        case_935 = ((double)(mak_local_es_392) * (double)(57.29577951308232286));
                        break;
                    }
                    case INT64_C(184475947): {
                        case_935 = ((double)(mak_local_es_392) * (double)(mak_local_es_392));
                        break;
                    }
                    case INT64_C(224581785): {
                        double eval_950 = ((double)(mak_local_es_392) * (double)(mak_local_es_392));
                        case_935 = ((double)(eval_950) * (double)(mak_local_es_392));
                        break;
                    }
                    default: {
                        case_935 = 0.00000000000000000;
                        break;
                    }
                }
                (out)->v.v10.f0 = case_935;
                (out)->v.v10.f1 = mak_local_eu_394;
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

static inline double mak_ir_fn_23(int64_t a0, int64_t a1) {
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
            double eval_951 = mak_ir_ext_29(a1);
            return ((double)(1.00000000000000000) / (double)(eval_951));
        }
        case INT64_C(260276799): {
            double eval_952 = mak_ir_ext_30(a1);
            return ((double)(1.00000000000000000) / (double)(eval_952));
        }
        case INT64_C(224481180): {
            double eval_953 = mak_ir_ext_30(a1);
            double eval_954 = mak_ir_ext_29(a1);
            return ((double)(eval_953) / (double)(eval_954));
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
            double eval_955 = mak_ir_ext_23(a1);
            double eval_956 = mak_ir_ext_22(a1);
            return ((double)(eval_955) / (double)(eval_956));
        }
        case INT64_C(1986845): {
            double eval_957 = mak_ir_ext_23(a1);
            return ((double)(1.00000000000000000) / (double)(eval_957));
        }
        case INT64_C(1714103): {
            double eval_958 = mak_ir_ext_22(a1);
            return ((double)(1.00000000000000000) / (double)(eval_958));
        }
        case INT64_C(219778423): {
            double eval_959 = mak_ir_ext_27(a1);
            return ((double)(1.57079632679489656) - (double)(eval_959));
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
            int32_t scrut_960 = (int32_t)(a1) > (int32_t)(0.00000000000000000);
            if ((scrut_960 == INT64_C(1))) {
                return 1.00000000000000000;
            }
            else if (1) {
                int32_t scrut_961 = (int32_t)(a1) < (int32_t)(0.00000000000000000);
                if ((scrut_961 == INT64_C(1))) {
                    return -1.00000000000000000;
                }
                else if (1) {
                    return 0.00000000000000000;
                }
            }
        }
        case INT64_C(231274442): {
            double eval_962 = mak_ir_ext_10(a1);
            return ((double)(a1) - (double)(eval_962));
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
            return (a1 * a1);
        }
        case INT64_C(224581785): {
            int64_t eval_963 = (a1 * a1);
            return (eval_963 * a1);
        }
        default: {
            return 0.00000000000000000;
        }
    }
}


int main(void) {
    mak_user_a = mak_ir_fn_0;
    mak_user_b = mak_ir_fn_1;
    mak_user_e = mak_ir_fn_2;
    mak_user_f = mak_ir_ext_1;
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
