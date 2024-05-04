/* ttywrite.c - ttywrite */

#include <xinu.h>

/*------------------------------------------------------------------------
 *  ttywrite  -  Write character(s) to a tty device (interrupts disabled)
 *------------------------------------------------------------------------
 */
devcall	ttywrite(
	  struct dentry	*devptr,	/* Entry in device switch table	*/
	  char	*buff,			/* Buffer of characters		*/
	  int32	count 			/* Count of character to write	*/
	)
{
	 
	return OK;
}
