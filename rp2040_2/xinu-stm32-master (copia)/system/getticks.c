/* getticks.c - getticks */

#include <xinu.h>
 

/*------------------------------------------------------------------------
 *  getticks  -  Retrieve the number of clock ticks since CPU reset
 *------------------------------------------------------------------------
 */
uint32  	getticks()
{
	return systick_get();
}
