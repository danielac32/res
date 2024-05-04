/* ttyhandler.c - ttyhandler */

#include <xinu.h>

/*------------------------------------------------------------------------
 *  ttyhandler  -  Handle an interrupt for a tty (serial) device
 *------------------------------------------------------------------------
 */
void ttyhandler(uint32 xnum) {
	struct	dentry	*devptr;	/* Address of device control blk*/
	struct	ttycblk	*typtr;		/* Pointer to ttytab entry	*/	
	struct	uart_csreg *csrptr;	/* Address of UART's CSR	*/
//	uint32	iir = 0;		/* Interrupt identification	*/
//	uint32	lsr = 0;		/* Line status			*/
//
//
	/* Get CSR address of the device (assume console for now) */

 
}
