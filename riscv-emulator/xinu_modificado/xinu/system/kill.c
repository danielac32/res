/* kill.c - kill */

#include <xinu.h>

/*------------------------------------------------------------------------
 *  kill  -  Kill a process and remove it from the system
 *------------------------------------------------------------------------
 */
syscall	kill(
	  pid32		pid		/* ID of process to kill	*/
	)
{
	intmask	mask;			/* Saved interrupt mask		*/
	struct	procent *prptr;		/* Ptr to process's table entry	*/
	int32	i;			/* Index into descriptors	*/
    
    prptr = &tasks[pid];
	//mask = disable();
	/*if (isbadpid(pid) || (pid == NULLPROC) || ((prptr = &tasks[pid])->prstate) == PR_FREE) {
		//restore(mask);
		kprintf("error kill\n");
		return SYSERR;
	}*/

	/*if (--prcount <= 1) {		 
		xdone();
	}*/

	/*send(prptr->prparent, pid);
	for (i=0; i<3; i++) {
		close(prptr->prdesc[i]);
	}*/
	//freestk(prptr->prstkbase, prptr->prstklen);
    
	switch (prptr->prstate) {
	case PR_CURR:

        {   
 
        	prptr->prstate = PR_FREE;
		    prptr->sleep = -1;
		    prptr->prname[0] = NULLCH;
		    prptr->prprio = 0;
 

	        kfree(prptr->prstkptr);
 

            sys(XINU_YIELD,0,0,0);
        }

 
		//resched();
    break;

	/*case PR_SLEEP:
	case PR_RECTIM:
		unsleep(pid);
		prptr->prstate = PR_FREE;
		break;

	case PR_WAIT:
		semtab[prptr->prsem].scount++;
		 
    break;
	case PR_READY:
		getitem(pid);		// Remove from queue 
	 
    break;*/
	default:
		prptr->prstate = PR_FREE;
	}

	//restore(mask);
	return OK;
}
