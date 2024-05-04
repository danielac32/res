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

/* clkhandler.c - clkhandler */

#include <xinu.h>



void systick_handler(){
     //*((uint32*)0xE000ED04) = (1 << 28);
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
		// PendSV call
		PEND_SV();//*SCB_ICSR |= (1 << PENDSV_INTR);
	}
}
/*void clkhandler()
{
	
}*/
