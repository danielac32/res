/* Xinu for STM32
 *
 * Original license applies
 * Modifications for STM32 by Robin Krens
 * Please see LICENSE and AUTHORS 
 * 
 * $LOG$
 * 2019/11/11 - ROBIN KRENS
 * Initial version 
 * 
 * $DESCRIPTION$
 *
 * */

/* clkinit.c - clkinit STM32 */

#include <xinu.h>

uint32	clktime;		/* Seconds since boot			*/
uint32  count1000;              /* ms since last clock tick             */
qid16	sleepq;			/* Queue of sleeping processes		*/
uint32	preempt;		/* Preemption counter			*/

/*------------------------------------------------------------------------
 * clkinit  -  Initialize the clock and sleep queue at startup
 *------------------------------------------------------------------------
 */
void clkinit(void)
{
	
	/*struct clock_csreg * clockptr;
	struct timer_csreg * tptr;
	clockptr = (struct clock_csreg *)CLOCK_BASE;
	tptr = (struct timer_csreg *)TIM2_BASE;*/

	sleepq = newqueue();	 
				 
	preempt = QUANTUM;	 
	clktime = 0;		 
    count1000 = 0;
    
    
}
