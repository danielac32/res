#include <stdint.h>
#include <string.h>
#include "kmalloc.h"
#include "serial.h"
#include "riscv.h"

void kmalloc_init(void *start, int length)
{
  head = (struct kmalloc_chunk *) start;
  head->state = KMALLOC_STATE_FREE;
  head->length = length;
  head->next = 0;
  head->prev = 0;
}

/*
Split a large chunk into two, such that the current chunk
has the desired length, and the next chunk has the remainder.
*/

static void ksplit(struct kmalloc_chunk *c, int length)
{
  struct kmalloc_chunk *n = (struct kmalloc_chunk *) ((char *) c + length);

  n->state = KMALLOC_STATE_FREE;
  n->length = c->length - length;
  n->prev = c;
  n->next = c->next;

  if(c->next)
    c->next->prev = n;

  c->next = n;
  c->length = length;
}

/*
Allocate a chunk of memory of the given length.
To avoid fragmentation, round up the length to
a multiple of the chunk size.  Then, search fo
a chunk of the desired size, and split it if necessary.
*/

void *kmalloc(int length)
{
  // round up length to a multiple of KUNIT
  int extra = length % KUNIT;
  if(extra)
    length += (KUNIT - extra);

  // then add one more unit to accommodate the chunk header
  length += KUNIT;

  struct kmalloc_chunk *c = head;

  while(1) {
    if(!c) {
      kprintf("kmalloc: out of memory!\n");
      return NULL;
    }
    if(c->state == KMALLOC_STATE_FREE && c->length >= length)
      break;
    c = c->next;
  }

  // split the chunk if the remainder is greater than two units
  if(length - c->length > 2 * KUNIT) {
    ksplit(c, length);
  }

  c->state = KMALLOC_STATE_USED;

  // return a pointer to the memory following the chunk header
  return (c + 1);
}

/*
Attempt to merge a chunk with its successor,
if it exists and both are in the free state.
*/

static void kmerge(struct kmalloc_chunk *c)
{
  if(!c)
    return;

  if(c->state != KMALLOC_STATE_FREE)
    return;

  if(c->next && c->next->state == KMALLOC_STATE_FREE) {
    c->length += c->next->length;
    if(c->next->next) {
      c->next->next->prev = c;
    }
    c->next = c->next->next;
  }

}

/*
Free memory by marking the chunk as de-allocated,
then attempting to merge it with the predecessor and successor.
*/

void kfree(void *ptr)
{
  struct kmalloc_chunk *c = (struct kmalloc_chunk *) ptr;
  c--;

  if(c->state != KMALLOC_STATE_USED) {
    kprintf("invalid kfree(%x)\n", ptr);
    return;
  }

  c->state = KMALLOC_STATE_FREE;

  kmerge(c);
  kmerge(c->prev);
}

void kmalloc_debug()
{
  struct kmalloc_chunk *c;

  kprintf("state ptr      prev     next     length  ptr(%8x)\n",KMALLOC_START);

  for(c = head; c; c = c->next) {
    if(c->state == KMALLOC_STATE_FREE) {
      kprintf("Free");
    } else if(c->state == KMALLOC_STATE_USED) {
      kprintf("Used");
    } else {
      kprintf("kmalloc list corrupted at %x!\n", c);
      return;
    }
    kprintf("  %x,%i       %x       %x       %l\n", c,c, c->prev, c->next, c->length);
  }
}

 
void    *minheap;   /* Start address of heap    */
void    *maxheap;   /* End address of heap      */

struct  memblk  memlist;    /* List of free memory blocks       */


 void    meminit(void)
{
    struct  memblk *memptr; /* Memory block pointer */

    /* avr specific */
    

    /* Initialize the minheap and maxheap variables */

    minheap = KMALLOC_START;//ptr_bss_end+1;
    maxheap = KMALLOC_LENGTH;//0x008008FF;   /* AVR atmega328p stack pointer when booting */

    /* Initialize the memory list as one big block */

    memlist.mnext = (struct memblk *)minheap;
    memptr = memlist.mnext;

    memptr->mnext = (struct memblk *)NULL;
    memlist.mlength = memptr->mlength = (uint32_t)maxheap - (uint32_t)minheap;
}

 
char    freemem(
      char      *blkaddr,   /* Pointer to memory block  */
      uint32_t  nbytes      /* Size of block in bytes   */
    )
{
    struct  memblk  *next, *prev, *block;
    uint32_t    top;
    uint8_t mask=disable();

    if ((nbytes == 0) || ((uint32_t) blkaddr < (uint32_t) minheap)
              || ((uint32_t) blkaddr > (uint32_t) maxheap)) {
         //kprintf("error 1\n");
        restore(mask);
        return SYSERR;
    }

    nbytes = (uint32_t) roundmb(nbytes);    /* Use memblk multiples */
    block = (struct memblk *)blkaddr;

    prev = &memlist;            /* Walk along free list */
    next = memlist.mnext;
    while ((next != NULL) && (next < block)) {
        prev = next;
        next = next->mnext;
    }

    if (prev == &memlist) {     /* Compute top of previous block*/
        top = (uint32_t) NULL;
    } else {
        top = (uint32_t) prev + prev->mlength;
    }

    /* Ensure new block does not overlap previous or next blocks    */

    if (((prev != &memlist) && (uint32_t) block < top)
        || ((next != NULL)  && (uint32_t) block+nbytes>(uint32_t)next)) {
        //kprintf("error 2\n");
        restore(mask);
        return SYSERR;
    }

    memlist.mlength += nbytes;

    /* Either coalesce with previous block or add to free list */

    if (top == (uint32_t) block) {  /* Coalesce with previous block */
        prev->mlength += nbytes;
        block = prev;
    } else {            /* Link into list as new node   */
        block->mnext = next;
        block->mlength = nbytes;
        prev->mnext = block;
    }

    /* Coalesce with next block if adjacent */

    if (((uint32_t) block + block->mlength) == (uint32_t) next) {
        block->mlength += next->mlength;
        block->mnext = next->mnext;
    }
    restore(mask);
    return OK;
}


char    *getmem(
      uint32_t  nbytes      /* Size of memory requested */
    )
{
    uint8_t   mask;           /* Saved interrupt mask     */
    struct  memblk  *prev, *curr, *leftover;

    mask = disable();
    if (nbytes == 0) {
        restore(mask);
        return (char *)SYSERR;
    }

    nbytes = (uint32_t) roundmb(nbytes);    /* Use memblk multiples */

    prev = &memlist;
    curr = memlist.mnext;
    while (curr != NULL) {          /* Search free list */

        if (curr->mlength == nbytes) {  /* Block is exact match */
            prev->mnext = curr->mnext;
            memlist.mlength -= nbytes;
            restore(mask);
            return (char *)(curr);

        } else if (curr->mlength > nbytes) { /* Split big block */
            leftover = (struct memblk *)((uint32_t) curr +
                    nbytes);
            prev->mnext = leftover;
            leftover->mnext = curr->mnext;
            leftover->mlength = curr->mlength - nbytes;
            memlist.mlength -= nbytes;
            restore(mask);
            return (char *)(curr);
        } else {            /* Move to next block   */
            prev = curr;
            curr = curr->mnext;
        }
    }
    restore(mask);
    return (char *)SYSERR;
}

void list(){
    struct  memblk  *memptr;    /* Ptr to memory block      */
    uint32_t    free_mem;       /* Total amount of free memory  */

    /* Initialize the system */
 
    free_mem = 0;
    for (memptr = memlist.mnext; memptr != NULL;
                        memptr = memptr->mnext) {
        free_mem += memptr->mlength;
    }
    kprintf("\nFreeMEM:%d (bytes)\n\n", free_mem);
}