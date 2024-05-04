/* clkhandler.c - clkhandler */

/* avr specific */

#include <xinu.h>

#include <avr/io.h>
#include <avr/interrupt.h>

volatile unsigned int avr_ticks=0;

/*-----------------------------------------------------------------------
 * clkhandler - high level clock interrupt handler
 *-----------------------------------------------------------------------
 */

/* void clkhandler() */

ISR(TIMER1_COMPA_vect, ISR_NAKED)
{

 
    count1000++;
 

	if(count1000 >= 1000) { 
		clktime++;
		count1000 = 0;
	}
	if(!isempty(sleepq)) {
 

		if((--queuetab[firstid(sleepq)].qkey) == 0) {

			wakeup();
		}
	}
 
	if((--preempt) == 0) {
		preempt = QUANTUM;
		resched();
	}
	//asm volatile("reti");
}

