/* clkinit.c */

/* avr specific */

#include <xinu.h>

#include <avr/io.h>
#include <avr/interrupt.h>

uint32	clktime;		/* Seconds since boot			*/
unsigned long  count1000;	/* ms since last clock tick             */
qid16	sleepq;			/* Queue of sleeping processes		*/
unsigned long preempt;		/* Preemption counter			*/

/*------------------------------------------------------------------------
 * clkinit  -  Initialize the clock and sleep queue at startup
 *------------------------------------------------------------------------
 */
void clkinit(void)
{
	
	struct clock_csreg * clockptr;

	sleepq = newqueue();	/* Allocate a queue to hold the delta	*/
				/*   list of sleeping processes		*/
	preempt = QUANTUM;	/* Set the preemption time		*/
	clktime = 0;		/* Start counting seconds		*/
    count1000 = 0;


	/* 
         * AVR atmega328p timer/clock init: interrupt every 1ms 
	 * The AVR TIMER interrupt rutine is in clkhandler.c
         */
   TCCR1A = 0;// set entire TCCR1A register to 0
    TCCR1B = 0;// same for TCCR1B
    TCNT1  = 0;//initialize counter value to 0
  // set compare match register for 1hz increments
    OCR1A = 600;// = (16*10^6) / (1*1024) - 1 (must be <65536)
  // turn on CTC mode
    TCCR1B |= (1 << WGM12);
  // Set CS12 and CS10 bits for 1024 prescaler
    TCCR1B |= (1 << CS12) | (1 << CS10);  
  // enable timer compare interrupt
    TIMSK1 |= (1 << OCIE1A);
  
	return;
}

