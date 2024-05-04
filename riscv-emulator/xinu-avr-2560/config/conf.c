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
	  (void *)0x40013800, (void *)ttyhandler, 53 },

/* GPIO0 is gpio */
	{ 1, 0, "GPIO0",
	  (void *)gpioinit, (void *)ionull, (void *)ionull,
	  (void *)gpioread, (void *)gpiowrite, (void *)ioerr,
	  (void *)ionull, (void *)ionull, (void *)gpiocontrol,
	  (void *)0x40010800, (void *)gpiohandler, 99 },

/* GPIO1 is gpio */
	{ 2, 1, "GPIO1",
	  (void *)gpioinit, (void *)ionull, (void *)ionull,
	  (void *)gpioread, (void *)gpiowrite, (void *)ioerr,
	  (void *)ionull, (void *)ionull, (void *)gpiocontrol,
	  (void *)0x40010801, (void *)gpiohandler, 100 },

/* GPIO2 is gpio */
	{ 3, 2, "GPIO2",
	  (void *)gpioinit, (void *)ionull, (void *)ionull,
	  (void *)gpioread, (void *)gpiowrite, (void *)ioerr,
	  (void *)ionull, (void *)ionull, (void *)gpiocontrol,
	  (void *)0x40010802, (void *)gpiohandler, 110 },

/* GPIO3 is gpio */
	{ 4, 3, "GPIO3",
	  (void *)gpioinit, (void *)ionull, (void *)ionull,
	  (void *)gpioread, (void *)gpiowrite, (void *)ioerr,
	  (void *)ionull, (void *)ionull, (void *)gpiocontrol,
	  (void *)0x40010802, (void *)gpiohandler, 110 },

/* GPIO4 is gpio */
	{ 5, 4, "GPIO4",
	  (void *)gpioinit, (void *)ionull, (void *)ionull,
	  (void *)gpioread, (void *)gpiowrite, (void *)ioerr,
	  (void *)ionull, (void *)ionull, (void *)gpiocontrol,
	  (void *)0x40010802, (void *)gpiohandler, 110 },

/* GPIO5 is gpio */
	{ 6, 5, "GPIO5",
	  (void *)gpioinit, (void *)ionull, (void *)ionull,
	  (void *)gpioread, (void *)gpiowrite, (void *)ioerr,
	  (void *)ionull, (void *)ionull, (void *)gpiocontrol,
	  (void *)0x40010802, (void *)gpiohandler, 110 },

/* GPIO6 is gpio */
	{ 7, 6, "GPIO6",
	  (void *)gpioinit, (void *)ionull, (void *)ionull,
	  (void *)gpioread, (void *)gpiowrite, (void *)ioerr,
	  (void *)ionull, (void *)ionull, (void *)gpiocontrol,
	  (void *)0x40010802, (void *)gpiohandler, 110 },

/* NULLDEV is null */
	{ 8, 0, "NULLDEV",
	  (void *)ionull, (void *)ionull, (void *)ionull,
	  (void *)ionull, (void *)ionull, (void *)ioerr,
	  (void *)ionull, (void *)ionull, (void *)ioerr,
	  (void *)0x0, (void *)ioerr, 0 },

/* NAMESPACE is nam */
	{ 9, 0, "NAMESPACE",
	  (void *)naminit, (void *)namopen, (void *)ioerr,
	  (void *)ioerr, (void *)ioerr, (void *)ioerr,
	  (void *)ioerr, (void *)ioerr, (void *)ioerr,
	  (void *)0x0, (void *)ioerr, 0 },

/* EEPROM0 is eeprom */
	{ 10, 0, "EEPROM0",
	  (void *)eeprom_init, (void *)eeprom_open, (void *)eeprom_close,
	  (void *)eeprom_read, (void *)eeprom_write, (void *)ioerr,
	  (void *)ioerr, (void *)ioerr, (void *)ioerr,
	  (void *)0x0, (void *)ionull, 0 },

/* VFS is eeprom */
	{ 11, 1, "VFS",
	  (void *)eeprom_init, (void *)eeprom_open, (void *)eeprom_close,
	  (void *)eeprom_read, (void *)eeprom_write, (void *)ioerr,
	  (void *)ioerr, (void *)ioerr, (void *)ioerr,
	  (void *)0x0, (void *)ionull, 0 },

/* SDCARD is eeprom */
	{ 12, 2, "SDCARD",
	  (void *)eeprom_init, (void *)eeprom_open, (void *)eeprom_close,
	  (void *)eeprom_read, (void *)eeprom_write, (void *)ioerr,
	  (void *)ioerr, (void *)ioerr, (void *)ioerr,
	  (void *)0x0, (void *)ionull, 0 },

/* SRAM is eeprom */
	{ 13, 3, "SRAM",
	  (void *)eeprom_init, (void *)eeprom_open, (void *)eeprom_close,
	  (void *)eeprom_read, (void *)eeprom_write, (void *)ioerr,
	  (void *)ioerr, (void *)ioerr, (void *)ioerr,
	  (void *)0x0, (void *)ionull, 0 }
};
