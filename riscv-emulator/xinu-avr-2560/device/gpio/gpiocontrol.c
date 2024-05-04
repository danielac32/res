/* gpiocontrol.c - gpiocontrol */

#include <xinu.h>

#define OFFSETMODE(x)	(x * 4)
#define OFFSETCFG(x) 	((x * 4) + 2)
#define OFFSETMODE2(x)	((x - 8) * 4) 
#define OFFSETCFG2(x) 	(((x - 8) * 4) + 2)

static int setmode(struct dentry * devptr, uint32 mode, uint32 pin) 
{	
 
	return 0;
}

static int setcnf(struct dentry * devptr, uint32 mode, uint32 pin) 
{
	 
	return 0;
}

/*------------------------------------------------------------------------
 *  gpiocontrol  -  Control a gpio device pin settings
 *------------------------------------------------------------------------
 */
devcall gpiocontrol(struct dentry * devptr, int32 func, int32 mode, int32 pin) {

 
 
	return OK;

}
