/* meminit.c - meminit */

#include <xinu.h>

void	*minheap;	/* Start address of heap	*/
void	*maxheap;	/* End address of heap		*/

extern byte _kernel_end;  // linker-supplied address -- end of static kernel structures in RAM
extern byte _stack_limit; // linker-supplied address -- above there be dragons (eh, stack)


/*------------------------------------------------------------------------
 * meminit - Initialize the free memory list for BeagleBone Black
 *------------------------------------------------------------------------
 */
void	meminit(void)
{
	struct	memblk *memptr;	/* Memory block pointer	*/

	/* Initialize the minheap and maxheap variables */

	minheap = (void *)&_kernel_end;
	/* 1024 bytes is reserved for supervise mode handling */
	maxheap = (void *)MAXADDR - HANDLERSTACK;

	/* Initialize the memory list as one big block */

	memlist.mnext = (struct memblk *)minheap;
	memptr = memlist.mnext;

	memptr->mnext = (struct memblk *)NULL;
	memlist.mlength = memptr->mlength =
		(uint32)maxheap - (uint32)minheap;
}
