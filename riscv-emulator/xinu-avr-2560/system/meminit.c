/* meminit.c - meminit */

#include <xinu.h>

void	*minheap;	/* Start address of heap	*/
void	*maxheap;	/* End address of heap		*/

/*------------------------------------------------------------------------
 * meminit - Initialize the free memory list for BeagleBone Black
 *------------------------------------------------------------------------
 */
extern uint32 __bss_end;
extern uint32 __data_end;
extern uint32 __heap_start;
extern uint32 __heap_end;

 
 
local uint8 buffer[1500];

void	meminit(void)
{
	struct	memblk *memptr;	/* Memory block pointer	*/

	 

	/* Initialize the minheap and maxheap variables */

	 

	/* Initialize the minheap and maxheap variables */
 
   
	minheap = &buffer[1];
	maxheap = &buffer[1500];	/* AVR atmega328p stack pointer when booting */

	/* Initialize the memory list as one big block */

	memlist.mnext = (struct memblk *)minheap;
	memptr = memlist.mnext;

	memptr->mnext = (struct memblk *)NULL;

	memlist.mlength = memptr->mlength =(uint32)maxheap - (uint32)minheap;
 

 
}
