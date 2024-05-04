/* yield.c - yield */

#include <xinu.h>

/*------------------------------------------------------------------------
 *  yield  -  Voluntarily relinquish the CPU (end a timeslice)
 *------------------------------------------------------------------------
 */
syscall	yield(void)
{
	intmask	mask;			/* Saved interrupt mask		*/

	//mask = disable();
	//resched();
	//restore(mask);
	//sys(XINU_YIELD,0,0,0);
	struct procent *ctx = current_task;
    current_task = &main_context;
    sys_switch(ctx->reg, &main_context.reg);
	return OK;
}
