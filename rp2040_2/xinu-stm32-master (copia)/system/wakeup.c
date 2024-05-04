/* wakeup.c - wakeup */

#include <xinu.h>

/*------------------------------------------------------------------------
 *  wakeup  -  Called by clock interrupt handler to awaken processes
 *------------------------------------------------------------------------
 */
void	wakeup(void)
{

	int pid=0;
	int sleep=0;
	struct procent *prptr;	
    
	/* Awaken all processes that have no more time to sleep */
	resched_cntl(DEFER_START);

	while (nonempty(sleepq) && (firstkey(sleepq) <= 0)) {
		   pid = dequeue(sleepq);
		   ready(pid);
	}
   
    resched_cntl(DEFER_STOP);
 
	return;
}
