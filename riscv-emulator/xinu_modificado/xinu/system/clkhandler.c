/* clkhandler.c - clkhandler */

/* avr specific */

#include <xinu.h>

//#include <avr/io.h>
//#include <avr/interrupt.h>

//volatile unsigned int avr_ticks=0;

/*-----------------------------------------------------------------------
 * clkhandler - high level clock interrupt handler
 *-----------------------------------------------------------------------
 */

void dlay(uint32 d){
   uint32 start=0;
   uint32 count_dlay;
   while(d>0){
   	     asm volatile ("csrr %0, cycle" : "=r" (count_dlay) : );
      if(count_dlay-start>=1){
      	 asm volatile ("csrr %0, cycle" : "=r" (count_dlay) : );
         start=count_dlay;
         d--;
      }
   }
}

void clkhandler(){
	count1000++;

	 

	if(count1000 >= 1000) {	 
		clktime++;
		count1000 = 0;
	}


    for(int i = 0; i < taskTop; ++i) {
        if(tasks[i].sleep > 0) {
           --tasks[i].sleep;
        }
    }

	/*if(!isempty(sleepq)) {
 

		if((--queuetab[firstid(sleepq)].qkey) == 0) {

			wakeup();
		}
	}

	 
	if((--preempt) == 0) {
		preempt = QUANTUM;
		resched();
	}*/

	
    //kprintf("clkhandler\n");
    //sys(XINU_YIELD,0,0,0);
}


/*
ISR(TIMER0_COMPA_vect)
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
	
}

*/