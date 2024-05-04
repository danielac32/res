/* create.c - create, newpid */

#include <stdarg.h>
#include <xinu.h>

 local pid32	newpid(void);

#define	roundew(x)	( (x+3)& ~0x3)

/*------------------------------------------------------------------------
 *  create  -  create a process to start running a procedure
 *------------------------------------------------------------------------
 */
 
 

uint32 *prepare_task_stack(uint32 *task_stack,uint32 size,void *entry, uint32 nargs, void *parg){
  task_stack += size - sizeof(context_t); /* End of task_stack, minus what we are about to push */
  context_t *ctx = (context_t*)task_stack;

  ctx->r0 = (uint32) nargs;
  ctx->r1 = (uint32) parg;
  ctx->lr = (uint32) INITRET;
  ctx->pc = (uint32) entry;
  ctx->psr = (uint32) 0x01000000; /* PSR Thumb bit */
  return task_stack;
}


pid32	create(
	  void		*procaddr,	/* procedure address		*/
	  uint32	ssize,		/* stack size in bytes		*/
	  pri16		priority,	/* process priority > 0		*/
	  char		*name,		/* name (for debugging)		*/
	  uint32	nargs,		/* number of args that follow	*/
	  ...
	)
{
	intmask 	mask;    	/* interrupt mask		*/
	pid32		pid;		/* stores new process id	*/
	struct	procent	*prptr;		/* pointer to proc. table entry */
	int32		i;
	//uint32		*a;		/* points to list of args	*/
	uint32		*saddr;		/* stack address		*/
 
   va_list ap;
	mask = disable();
	if (ssize < MINSTK)
		ssize = MINSTK;
	ssize = (uint32) roundew(ssize);
	if (((saddr = (uint32 *)getmem(ssize)) ==
	    (uint32 *)SYSERR ) ||
	    (pid=newpid()) == SYSERR || priority < 1 ) {
		restore(mask);
		return SYSERR;
	}

	
	prcount++;
	prptr = &proctab[pid];

	/* initialize process table entry for new process */
	prptr->prstate = PR_SUSP;	/* initial state is suspended	*/
	prptr->prprio = priority;
	prptr->prstkbase = saddr;
	prptr->prstklen = ssize;
	prptr->prname[PNMLEN-1] = NULLCH;
	for (i=0 ; i<PNMLEN-1 && (prptr->prname[i]=name[i])!=NULLCH; i++)
		;
	prptr->prsem = -1;
	prptr->prparent = (pid32)getpid();
	prptr->prhasmsg = FALSE;

	/* set up initial device descriptors for the shell		*/
	prptr->prdesc[0] = CONSOLE;	/* stdin  is CONSOLE device	*/
	prptr->prdesc[1] = CONSOLE;	/* stdout is CONSOLE device	*/
	prptr->prdesc[2] = CONSOLE;	/* stderr is CONSOLE device	*/
   
    va_start(ap, nargs);
    for (int j=0; j < nargs; j++){
        prptr->parg[j] = (void *)va_arg(ap, long);
    }
    va_end(ap);
    prptr->sp =  prepare_task_stack(saddr,ssize,procaddr,nargs,&prptr->parg[0]);

    printf("created %d\n",pid );
	restore(mask);
	return pid;
}

/*------------------------------------------------------------------------
 *  newpid  -  Obtain a new (free) process ID
 *------------------------------------------------------------------------
 */
local pid32	newpid(void)
{
	uint32	i;			/* iterate through all processes*/
//	static	pid32 nextpid = 1;	/* position in table to try or	*/
	static	pid32 nextpid = 0;	/* position in table to try or	*/
					/*  one beyond end of table	*/

	/* check all NPROC slots */

	for (i = 0; i < NPROC; i++) {
 
		if (proctab[nextpid].prstate == PR_FREE) {
			return nextpid++;
		} else {
			nextpid++;
		}
	}
	return (pid32) SYSERR;
}
