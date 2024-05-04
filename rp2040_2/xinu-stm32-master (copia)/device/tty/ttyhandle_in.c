/* ttyhandle_in.c - ttyhandle_in, erase1, eputc, echoch */

#include <xinu.h>

 
/*------------------------------------------------------------------------
 *  ttyhandle_in  -  Handle one arriving char (interrupts disabled)
 *------------------------------------------------------------------------
 */
void	ttyhandle_in (
	  struct ttycblk *typtr,	/* Pointer to ttytab entry	*/
	  struct uart_csreg *csrptr	/* Address of UART's CSR	*/
	)
{
	char	ch;			/* Next char from device	*/
	int32	avail;			/* Chars available in buffer	*/

 
}

 