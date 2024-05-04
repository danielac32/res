/* raminit.c  -  raminit */

#include <xinu.h>
//#include <avr_eeprom_disk.h>

/*------------------------------------------------------------------------
 *  eeprom_init  -  Initialize the eeprom disk
 *------------------------------------------------------------------------
 */
devcall	eeprom_init (
	  struct dentry	*devptr		/* Entry in device switch table	*/
	)
{
	eeprom_pos = 0;

	return OK;
}
