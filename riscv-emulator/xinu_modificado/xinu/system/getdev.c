/* getdev.c - getdev */

#include <xinu.h>
#include <string.h>

/*------------------------------------------------------------------------
 *  getdev  -  Convert a device name to a device ID
 *------------------------------------------------------------------------
 */
syscall	getdev(
	  char		*devname	/* Name of the device		*/
	)
{
	 for (did32 id = 0; id < NDEVS; id++) {
         if(!strcmp(devtab[id].dvname, devname)){
            return id;
         }
    }
    return -1;
}
