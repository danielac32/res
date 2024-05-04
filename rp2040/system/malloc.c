#include <lib.h>
#include <panic.h>
#include <bit_manip.h>

#define UNIT      4u // smallest allocation unit (4 bytes)

typedef uint32_t moff_t;
#define MHDR_SZ   ((moff_t)(sizeof (mhdr_t)))
#define FULLBIT   ((moff_t)(1u<<31))
#define SIZEBITS  ((moff_t)(FULLBIT-1))

extern const byte _kernel_end;  // linker-supplied address -- end of static kernel structures in RAM
extern const byte _stack_limit; // linker-supplied address -- above there be dragons (eh, stack)
#define mem_start  ((uintptr_t)(ALIGN(UNIT, (size_t)(&_kernel_end)))) // start of managed memory
#define mem_size   ((moff_t)(ALIGN(UNIT, (size_t)(&_stack_limit) - (size_t)(&_kernel_end)))) // size of managed memory

typedef uint32_t* mhdr_t;
 mhdr_t hdr_at(const moff_t addr) { return (uint32_t*)(mem_start + addr); }

 void set_full(mhdr_t h) { *h |= FULLBIT; }
 void set_free(mhdr_t h) { *h &= ~FULLBIT; }
 void set_size(mhdr_t h, moff_t sz) { kassert(sz <= SIZEBITS); *h &= ~SIZEBITS; *h |= sz; }
 bool   is_full (const mhdr_t h) { return *h & FULLBIT;  }
 moff_t get_size(const mhdr_t h) { return *h & SIZEBITS; }

 void mk_free_block(const moff_t start, const moff_t end) {
    mhdr_t h = hdr_at(start);
    set_free(h); set_size(h, end - start - MHDR_SZ);
}

 bool is_fragmented_at(moff_t addr) {
    const mhdr_t h = hdr_at(addr);
    return !is_full(h) &&
           addr + MHDR_SZ + get_size(h) < mem_size &&
           !is_full(hdr_at(addr + MHDR_SZ + get_size(h)));
}

// actually O(1) if everything works, as no more than 3 consecutive free blocks should occur
 void defrag_at(const moff_t start) {
    moff_t curr = start;
    while (curr < mem_size && !is_full(hdr_at(curr))) {
        curr += MHDR_SZ + get_size(hdr_at(curr));
    }
    mk_free_block(start, curr);
}

void *malloc(size_t sz) {
    if (sz == 0) return NULL;
    moff_t size = ALIGN(UNIT, sz);

    // find the best fitting free block
    moff_t best_addr;
    moff_t best_size = 0;
    moff_t curr = 0;
    while (curr < mem_size) {
        mhdr_t h = hdr_at(curr);

        if (is_fragmented_at(curr)) defrag_at(curr);

        if (!is_full(h) && get_size(h) >= size) { // we only want free blocks that are big enough
            if (!best_size || get_size(h) < best_size) {
                best_size = get_size(h);
                best_addr = curr;
            }
        }
        curr += MHDR_SZ + get_size(h);
    }
    if (!best_size) return NULL; // no free block was big enough

    if (best_size - size <= MHDR_SZ) {
        // then we need to allocate more, because a block does not fit between this and the next one
        size = best_size;
    }

    mhdr_t h = hdr_at(best_addr); set_size(h, size); set_full(h);
    if (size != best_size) { // then there is a new free block
        mk_free_block(best_addr + MHDR_SZ + size, best_addr + MHDR_SZ + best_size);
    }

    return (void*)(mem_start + best_addr + MHDR_SZ);
}

void free(void *ptr) {
    if (ptr == NULL) return; // just ignore NULL

    const moff_t blockbeg = (size_t)ptr - mem_start - MHDR_SZ;
    mhdr_t h = hdr_at(blockbeg);
    kassert(is_full(h));
    set_free(h);
}

/* === mm_init -- memory management subsystem initialization routine ============================ */

void mm_init(void) {
    // needed by kmalloc/kfree
    printf("%x,%d\n",mem_start,mem_size);
    mk_free_block(0, mem_size); // create a single free block spanning all memory
}
