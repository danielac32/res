#include <xinu.h>
 
int	newpid2(void)
{
	int	i;			/* iterate through all processes*/
//	static	pid32 nextpid = 1;	/* position in table to try or	*/
	int nextpid = 0;	/* position in table to try or	*/
					/*  one beyond end of table	*/

	/* check all NPROC slots */

	for (i = 0; i < NPROC; i++) {
		//nextpid %= NPROC;	/* wrap around to beginning */
		if (proctab[nextpid].prstate == PR_FREE) {
			return nextpid++;
		} else {
			nextpid++;
		}
	}
	return (int) SYSERR;
}


int piccolo_create_task(int  (*procaddr)()) {
  struct procent *tsk;
  uint32 mask=disable();
  int pid=ntask;
  tsk=&proctab[pid];
  tsk->prstkbase=(unsigned int *)getmem(INITSTK);
  tsk->sp=prepare_task_stack(tsk->prstkbase,INITSTK,procaddr);//__piccolo_os_create_task(tsk->stkaddr,pointer_to_task_function);
  tsk->prstate = PR_SUSP;
  ntask++;
  printf("pid : %d\n",pid );
  restore(mask);
  return pid;
}


void piccolo_start() {
  
  uint8 curr=0;
 
  while(1){
    proctab[curr].sp=swch(proctab[curr].sp);
    curr++;
    if(curr >= prcount){
       curr = 0;
    }
  }

}