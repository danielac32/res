/* meminit.c - meminit */

#include <xinu.h>

//void	*minheap;	/* Start address of heap	*/
//void	*maxheap;	/* End address of heap		*/
 
//extern uint32 end;
//char buffer[40000];

void	meminit(void)
{
	/*struct	memblk *memptr;	 

	minheap = (void *)buffer;
 
  maxheap = (void *)buffer + sizeof(buffer);
 
	memlist.mnext = (struct memblk *)minheap;
	memptr = memlist.mnext;

	memptr->mnext = (struct memblk *)NULL;
	memlist.mlength = memptr->mlength =
		(uint32)maxheap - (uint32)minheap;*/
	kmalloc_init(KMALLOC_START, KMALLOC_LENGTH);
    kmalloc_debug();

}
