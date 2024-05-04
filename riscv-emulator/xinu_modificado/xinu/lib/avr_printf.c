#include <xinu.h>

extern serial_put_char(char);

void avr_kprintf(const char m[])
{
	int i;
	for (i=0; i<strlen(m); i++)
		serial_put_char(m[i]);
}

