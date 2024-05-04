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

/* resched.c - resched, resched_cntl */

#include <xinu.h>

struct	defer	Defer;


local void load_context(void* psp) {
  asm volatile(
  "mov sp, %0   \n"
  "pop  {r4-r7} \n"
  "mov  r8, r4  \n"
  "mov  r9, r5  \n"
  "mov  r10, r6 \n"
  "mov  r11, r7 \n"
  "pop  {r4-r7} \n"
  "bx %1      \n" : : "r" (psp), "r" (0xFFFFFFF9)
  );
}

local void* save_context() {
  void *reg;
  asm volatile(
  "push {r4-r7} \n"
  "mov  r4, r8 \n"
  "mov  r5, r9 \n"
  "mov  r6, r10 \n"
  "mov  r7, r11 \n"
  "push {r4-r7} \n"
  "mov %0, sp \n" : "=r" (reg)
  );
  return reg;
}
 

__attribute__ ((naked))
void resched(void)
{
	void* reg;

  //task_t* tmp;

  // save context of current thread
  reg = save_context();
  active_task->sp = reg;



  if (active_task != 0 && active_task->state == PR_CURR) {
      queue_push(&ready_queue, active_task);
  }

  

  // load new context of next thread
  active_task = queue_pop(&ready_queue);
  preempt = QUANTUM;
  load_context(active_task->sp);
}

/*------------------------------------------------------------------------
 *  resched_cntl  -  Control whether rescheduling is deferred or allowed
 *------------------------------------------------------------------------
 */
status	resched_cntl(		/* Assumes interrupts are disabled	*/
	  int32	defer		/* Either DEFER_START or DEFER_STOP	*/
	)
{
	switch (defer) {

	    case DEFER_START:	/* Handle a deferral request */

		if (Defer.ndefers++ == 0) {
			Defer.attempt = FALSE;
		}
		return OK;

	    case DEFER_STOP:	/* Handle end of deferral */
		if (Defer.ndefers <= 0) {
			return SYSERR;
		}
		if ( (--Defer.ndefers == 0) && Defer.attempt ) {
			PEND_SV();
		}
		return OK;

	    default:
		return SYSERR;
	}
}
