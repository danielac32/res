#include "kmalloc.h"
#include "uart.h"
#include "rp2040.h"
#include "kernel.h"

struct  memblk  memlist;    /* List of free memory blocks       */

/*------------------------------------------------------------------------
 * meminit - Initialize the free memory list for BeagleBone Black
 *------------------------------------------------------------------------
 */

#define MAXADDR   0x20040000    /* 64kB SRAM */
#define HANDLERSTACK  1024      /* Size reserved for stack in Handler mode */


 
void  *minheap; /* Start address of heap  */
void  *maxheap; /* End address of heap    */


 void    meminit(void)
{
    struct  memblk *memptr; /* Memory block pointer */
 

    /* Initialize the minheap and maxheap variables */

    minheap = (void *)&_kernel_end;
  /* 1024 bytes is reserved for supervise mode handling */
    maxheap = (void *)MAXADDR - HANDLERSTACK;


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
    uint32_t mask=disable();
 
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
    uint32_t   mask;           /* Saved interrupt mask     */
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
    printf("\n%d bytes of free memory.  Free list:\n", free_mem);
    for (memptr=memlist.mnext; memptr!=NULL;memptr = memptr->mnext) {
        printf("           [0x%08X to 0x%08X]\n",(uint32_t)memptr, ((uint32_t)memptr) + memptr->mlength - 1);
    }
    
}