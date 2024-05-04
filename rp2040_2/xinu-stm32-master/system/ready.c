/* ready.c - ready */

#include <xinu.h>

qid16	readylist;			/* Index of ready list		*/

/*------------------------------------------------------------------------
 *  ready  -  Make a process eligible for CPU service
 *------------------------------------------------------------------------
 */
status	ready(
	  pid32		pid		/* ID of process to make ready	*/
	)
{
	/*struct procent *prptr;
    intmask		mask;
    mask = disable();
	if (isbadpid(pid)) {
		return SYSERR;
	}



	prptr = &proctab[pid];
	prptr->prstate = PR_READY;
	insert(pid, readylist, prptr->prprio);
	restore(mask);*/
	task_t *prptr;
prptr =& task[pid];
prptr->state = PR_CURR;
queue_push(&ready_queue, prptr); 

	return OK;
}
