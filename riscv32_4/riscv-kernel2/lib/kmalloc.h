


#ifndef KMALLOC_H
#define KMALLOC_H

#define MAX_HEAPS 20000
typedef unsigned long Align;

union header {
  struct {
    union header *ptr;
    unsigned long size;
    unsigned long store;
  } s;
  Align x;
};

typedef union header Header;

static unsigned char heaps[MAX_HEAPS];
static unsigned char *program_break = heaps;

static Header base; /* empty list to get started */
static Header *freep = NULL; /* start of free list */

void *kmalloc(unsigned long nbytes);
void kfree(void *ap);


#endif