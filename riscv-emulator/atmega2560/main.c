
asm("jmp main");
#include <avr/io.h>         //macros
#include <util/delay.h>     //_delay_ms function

#define DELAY_MS       100


void delay_ms(int t){
  for (int i = 0; i < t; ++i)
  {
  	asm("nop");
  }
}

int main()
{
	//register init - DDRB - PortB, DDB5 - Pin13
	DDRB |= _BV(DDB5);      //PB5 - pin 13 output
	

    while(1)
    {

        //Set Pins 0 to 13
        PORTB |= _BV(PORTB5);       //pin 13
      

        delay_ms(DELAY_MS);
        //Clear Pins 0 to 13
        PORTB &=~ _BV(PORTB5);       //pin 13
        
        delay_ms(DELAY_MS);
    }

	return 0;
}