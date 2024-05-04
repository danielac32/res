/* conf.c (GENERATED FILE; DO NOT EDIT) */

#include <xinu.h>


extern	devcall	ioerr(void);
extern	devcall	ionull(void);

/* Device independent I/O switch */

 struct	dentry	devtab[] =
{
/**
 * Format of entries is:
 * dev-number, minor-number, dev-name,
 * init, open, close,
 * read, write, seek,
 * getc, putc, control,
 * dev-csr-address, intr-handler, irq
 */

/* CONSOLE is tty */
	{ 0, 0, "CONSOLE",
	  (void *)ttyinit, (void *)ionull, (void *)ionull,
	  (void *)ttyread, (void *)ttywrite, (void *)ioerr,
	  (void *)ttygetc, (void *)ttyputc, (void *)ttycontrol,
	  (void *)0x4000301, (void *)ioerr, 53 },

/* GPIO0 is gpio */
	{ 1, 0, "GPIO0",
	  (void *)gpioinit, (void *)ionull, (void *)ionull,
	  (void *)gpioread, (void *)gpiowrite, (void *)ioerr,
	  (void *)ionull, (void *)ionull, (void *)gpiocontrol,
	  (void *)0x40010800, (void *)gpiohandler, 99 },

/* NULLDEV is null */
	{ 2, 0, "NULLDEV",
	  (void *)ionull, (void *)ionull, (void *)ionull,
	  (void *)ionull, (void *)ionull, (void *)ioerr,
	  (void *)ionull, (void *)ionull, (void *)ioerr,
	  (void *)0x0, (void *)ioerr, 0 },

/* NAMESPACE is nam */
	{ 3, 0, "NAMESPACE",
	  (void *)naminit, (void *)namopen, (void *)ioerr,
	  (void *)ioerr, (void *)ioerr, (void *)ioerr,
	  (void *)ioerr, (void *)ioerr, (void *)ioerr,
	  (void *)0x0, (void *)ioerr, 0 },

/* EEPROM0 is eeprom */
	{ 4, 0, "EEPROM0",
	  (void *)eeprom_init, (void *)eeprom_open, (void *)eeprom_close,
	  (void *)eeprom_read, (void *)eeprom_write, (void *)ioerr,
	  (void *)ioerr, (void *)ioerr, (void *)ioerr,
	  (void *)0x0, (void *)ionull, 0 }
};
