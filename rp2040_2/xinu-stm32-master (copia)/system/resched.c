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

 

//__attribute__ ((naked))
void resched(void)
{


    //proctab[currpid].prstkptr=swch(proctab[currpid].prstkptr);


    while(1){
         struct procent *ptold;	 
         struct procent *ptnew;	
         ptold = &proctab[currpid];
        
    	 if (Defer.ndefers > 0) {
		     Defer.attempt = TRUE;
		     ptold->sp=swch(ptold->sp);
	     }
         
         ptold = &proctab[currpid];

	     if (ptold->prstate == PR_CURR) {  /* Process remains eligible */
		    // if (ptold->prprio > firstkey(readylist)) {
			    // ptold->sp=swch(ptold->sp);
		     //}

		/* Old process will no longer remain current */

		     ptold->prstate = PR_READY;
		     insert(currpid, readylist, ptold->prprio);
	     }
         currpid = dequeue(readylist);
	     ptnew = &proctab[currpid];
	     ptnew->prstate = PR_CURR;
	     preempt = QUANTUM;		/* Reset time slice for process	*/
	     ptnew->sp=swch(ptnew->sp);


         /*switch(ptold->prstate){
                
                case PR_CURR:
                     if (ptold->prprio > firstkey(readylist)) {
                         ptold->sp=swch(ptold->sp);
		             }
		             ptold->prstate = PR_READY;
		             insert(currpid, readylist, ptold->prprio);
		             currpid = dequeue(readylist);
	                 ptnew = &proctab[currpid];
	                 ptnew->prstate = PR_CURR;
	                 preempt = QUANTUM;	 
                     ptnew->sp=swch(ptnew->sp);
                break;

         }*/

    }

	/*
	 
	struct procent *ptold;	 
	struct procent *ptnew;	 

	 
	

	 

	ptold = &proctab[currpid];

	if (ptold->prstate == PR_CURR) {  
		if (ptold->prprio > firstkey(readylist)) {
			 set_psp(restorestk);
             
		 
		}

	 

		ptold->prstate = PR_READY;
		insert(currpid, readylist, ptold->prprio);
	}

 
	
	currpid = dequeue(readylist);
	ptnew = &proctab[currpid];
	ptnew->prstate = PR_CURR;
	preempt = QUANTUM;	 


	ptold->prstkptr = restorestk;
	*/
 
  
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
