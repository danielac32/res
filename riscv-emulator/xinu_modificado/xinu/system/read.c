/* read.c - read */

#include <xinu.h>

/*------------------------------------------------------------------------
 *  read  -  Read one or more bytes from a device
 *------------------------------------------------------------------------
 */
syscall	read(
	  did32		descrp,		/* Descriptor for device	*/
	  char		*buffer,	/* Address of buffer		*/
	  uint32	count		/* Length of buffer		*/
	)
{
	intmask		mask;		/* Saved interrupt mask		*/
	 struct dentry *devptr; /* Entry in device switch table */
	int32		retval;		/* Value to return to caller	*/

	//mask = disable();
	if (isbaddev(descrp)) {
		//restore(mask);
		return SYSERR;
	}
	devptr = (struct dentry *) &devtab[descrp];
	retval = (*devptr->dvread) (devptr, buffer, count);
	//sys(XINU_YIELD,0,0,0);
	//restore(mask);
	return retval;
}
