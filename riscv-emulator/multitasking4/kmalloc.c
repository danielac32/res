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

 