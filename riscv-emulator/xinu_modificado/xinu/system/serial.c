#include <xinu.h>

//#define reg (*(volatile char *)0x4000100)
void serial_init() {}

void serial_put_char (char outputChar)
{
	 SERIAL->PUT=outputChar;
}

char value;

 
char serial_get_char(void)
{
     return SERIAL->GET;
}

void serial_put_str (char * msg)
{
	while (*msg) {
		serial_put_char(*msg);
		msg++;
	};
}

