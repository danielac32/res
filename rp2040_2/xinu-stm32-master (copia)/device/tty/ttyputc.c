/* ttyputc.c - ttyputc */

#include <xinu.h>

/*------------------------------------------------------------------------
 *  ttyputc  -  Write one character to a tty device (interrupts disabled)
 *------------------------------------------------------------------------
 */
devcall	ttyputc(
	struct	dentry	*devptr,	/* Entry in device switch table	*/
	char	ch			/* Character to write		*/
	)
{
	//struct uart_csreg * uptr;
	struct	ttycblk	*typtr;		/* Pointer to tty control block	*/

	 

	return OK;
}
