/* gpioinit.c - gpioinit */

#include <xinu.h>

struct	gpiocblk	gpiotab[NGPIO];

/*------------------------------------------------------------------------
 *  gpioinit  -  Initialize settings for a gpio device
 *------------------------------------------------------------------------
 */
devcall	gpioinit(
	  struct dentry	*devptr		/* Entry in device switch table	*/
	)
{
	 
	return OK;
}
