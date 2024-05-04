


#ifndef KMALLOC_H
#define KMALLOC_H

char mem[80000];
#define KMALLOC_START  (void*)mem
#define KMALLOC_LENGTH sizeof(mem)
void *kmalloc(int length);
void kfree(void *ptr);

void kmalloc_init(void *start, int length);
void kmalloc_debug();
int kmalloc_test();


#define KUNIT sizeof(struct kmalloc_chunk)

#define KMALLOC_STATE_FREE 0xa1a1a1a1
#define KMALLOC_STATE_USED 0xbfbfbfbf

struct kmalloc_chunk {
	int state;
	int length;
	struct kmalloc_chunk *next;
	struct kmalloc_chunk *prev;
};

static struct kmalloc_chunk *head = 0;



//#define NULL	0		/* null pointer for linked lists	*/
#define OK	( 1)		/* normal system call return		*/
#define	SYSERR	(-1)		/* system call failed			*/
#define	PAGE_SIZE	1024 // TODO: unused?

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