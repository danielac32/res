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

	/* After 1 sec, increment clktime */

	if(count1000 >= 1000) {
		clktime++;
		count1000 = 0;
	}
	 
  task_t *tmp = queue_peek(&sleep_queue);
  while (tmp) {
    if (count1000 >= tmp->param) {
      tmp = queue_pop(&sleep_queue);
      tmp->state = PR_CURR;
      queue_push(&ready_queue, tmp);
      tmp = queue_peek(&sleep_queue);
    } else {
      break;
    }
  }

  if((--preempt) == 0) {
      preempt = QUANTUM;
      PEND_SV();
  }
	
}
/*void clkhandler()
{
	
}*/
