#include <xinu.h>

#include <avr/interrupt.h>

#define BAUD 9600
#define BAUD_PRESCALE (((F_CPU / (BAUD * 16UL))) - 1)


void serial_init() 
{
  UCSR0B |= (1 << RXEN0) | (1 << TXEN0); //transmit side of hardware
  UCSR0C |= (1 << UCSZ00) | (1 << UCSZ01); //receive side of hardware

  UBRR0L = BAUD_PRESCALE; //set the baud to 9600, have to split it into the two registers
  UBRR0H = (BAUD_PRESCALE >> 8); //high end of baud register

  UCSR0B |= (1 << RXCIE0); //recieve data interrupt, makes sure we don't loose data

}


void serial_put_char (char outputChar)
{

 while(!(UCSR0A & (1u << 5))){};   //wait for empty
    UDR0 = outputChar;                      //write data
    while(!(UCSR0A & (1u << 6))){};   //wait for complete

}

char value;

/*
 * RX interrupt service rutine
 */
ISR(USART0_RX_vect){
	value = UDR0;             /* read UART register into value */
	ttyhandler (1, value, 0);
}

char serial_get_char(void)
{
	UCSR0B |= (1<<RXCIE0);
	return UDR0;
}

void serial_put_str (char * msg)
{
	while (*msg) {
		serial_put_char(*msg);
		msg++;
	};
}

