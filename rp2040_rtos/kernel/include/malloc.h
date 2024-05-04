

#include <kernel.h>

#ifndef MALLOC_H
#define MALLOC_H

extern  char _kernel_end;  // linker-supplied address -- end of static kernel structures in RAM
extern  char _stack_limit;
 
/*----------------------------------------------------------------------
 * roundmb, truncmb - Round or truncate address to memory block size
 *----------------------------------------------------------------------
 */
#define	roundmb(x)	(char *)( (7 + (uint32_t)(x)) & (~7) )
#define	truncmb(x)	(char *)( ((uint32_t)(x)) & (~7) )

/*----------------------------------------------------------------------
 *  freestk  --  Free stack memory allocated by getstk
 *----------------------------------------------------------------------
 */
#define	freestk(p,len)	freemem((char *)((uint32_t)(p)		\
				- ((uint32_t)roundmb(len))	\
				+ (uint32_t)sizeof(uint32_t)),	\
				(uint32_t)roundmb(len) )

struct	memblk	{			/* See roundmb & truncmb	*/
	struct	memblk	*mnext;		/* Ptr to next free memory blk	*/
	uint32_t	mlength;		/* Size of blk (includes memblk)*/
	};
extern	struct	memblk	memlist;	/* Head of free memory list	*/
extern	void	*minheap;		/* Start of heap		*/
extern	void	*maxheap;		/* Highest valid heap address	*/


void	meminit(void);
char	freemem(
	  char		*blkaddr,	/* Pointer to memory block	*/
	  uint32_t	nbytes		/* Size of block in bytes	*/
	);

void list();
char  	*getmem(
	  uint32_t	nbytes		/* Size of memory requested	*/
	);

#endif