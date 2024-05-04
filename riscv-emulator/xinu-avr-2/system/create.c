/* create.c - create, newpid */

/* avr specific */

#include <stdarg.h>
#include <xinu.h>


local	pid32 newpid();

#define	roundew(x)	( (x+3)& ~0x3)

/* avr specific */
#define	MAGIC		0xaa	/* unusual value for top of stk	*/


/*------------------------------------------------------------------------
 *  create  -  create a process to start running a procedure
 *------------------------------------------------------------------------
 */
pid32	create(
	  int		(*procaddr)(),	/* procedure address		*/
	  int	ssize,		/* stack size in bytes		*/
	  int		priority,	/* process priority > 0		*/
	  char		*name,		/* name (for debugging)		*/
	  int	nargs,		/* number of args that follow	*/
	  ...
	)
{
	intmask 	mask;    	/* interrupt mask		*/
	pid32		pid;		/* stores new process id	*/
	struct	procent	*prptr;		/* pointer to proc. table entry */
	int i;
	unsigned char		*saddr;		/* stack address		*/
	va_list ap;
    


    if(ssize < MINSTK) ssize=MINSTK;

    saddr = (unsigned char *)getstk(ssize);
    pid = newpid();

    if(saddr == SYSERR || pid == SYSERR || priority < 1){
       freestk(prptr->prstkbase, prptr->prstklen);
       //avr_kprintf(m10);
       printf("error getstk: %s , %d\n",name,ssize);
       return SYSERR;
    }
 

	
	prcount++;
	prptr = &proctab[pid];

	/* initialize process table entry for new process */
	prptr->prstate = PR_SUSP;	/* initial state is suspended	*/
	prptr->prprio = priority;
	prptr->prstkbase = (char *)saddr;
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




	prptr->paddr = (int *)procaddr;
	prptr->pargs = nargs;

	int * a = (int *)(&nargs + 1);
	for (i = 0; i < nargs; i++) {
		prptr->parg[i] = (int) *a++;// argumentos 
	}
	prptr->parg[nargs] = 0;//


	prptr->pregs[24] = lobyte((unsigned)prptr->pargs);	
	prptr->pregs[25] = hibyte((unsigned)prptr->pargs);
	prptr->pregs[22] = lobyte((unsigned)&prptr->parg[0]);	
	prptr->pregs[23] = hibyte((unsigned)&prptr->parg[0]);

	/**(saddr--) = lo8(INITRET);//0x00; // clear r26
    *(saddr--) = hi8(INITRET);//0x00; // clear r27

    *(saddr--) = 0;
    *(saddr--) = 0;
    *(saddr--) = 0;
    *(saddr--) = 0;
    *(saddr--) = 0;
    *(saddr--) = 0;
    *(saddr--) = 0;
    *(saddr--) = 0;
    *(saddr--) = 0;
    *(saddr--) = 0;
    *(saddr--) = 0;
    *(saddr--) = 0;

    *(saddr--) = 0;
    *(saddr--) = 0;
    *(saddr--) = 0;
    *(saddr--) = 0;
    *(saddr--) = 0;
    *(saddr--) = 0;
    *(saddr--) = 0;
    *(saddr--) = 0;
    *(saddr--) = 0;
    *(saddr--) = 0;
      // load args in r22:23
    *(saddr--) = lo8((unsigned)&prptr->parg[0]);//0;//lo8(args);
    *(saddr--) = hi8((unsigned)&prptr->parg[0]);//0;//hi8(args);

    // load args in r24:25
    *(saddr--) = lo8(prptr->pargs);//0;//lo8(args);
    *(saddr--) = hi8(prptr->pargs);//0;//hi8(args);

    *(saddr--) = 0x00; // clear r26
    *(saddr--) = 0x00; // clear r27
    *(saddr--) = 0;//lo8(kernel.stackBottom); // r28- aka lo8(Y)
    *(saddr--) = 0;//hi8(kernel.stackBottom); // r29- aka hi8(Y)
    // load Z reg for ijmp
    *(saddr--) = lo8(procaddr); // r30- aka lo8(Z)
    *(saddr--) = hi8(procaddr); // r31- aka hi8(Z)
    *(saddr--) = 0x0; // __SREG__
    prptr->sp=saddr;*/
    
    
    *saddr-- = lobyte((unsigned)INITRET);	 
	*saddr-- = hibyte((unsigned)INITRET);
	*saddr-- = lobyte((unsigned)procaddr);	
	*saddr-- = hibyte((unsigned)procaddr);


    prptr->pregs[SSP_L] = lobyte((unsigned) saddr);
	prptr->pregs[SSP_H] = hibyte((unsigned) saddr);


    //prptr->sp=saddr;
	//restore(mask);
	return pid;
}

/*------------------------------------------------------------------------
 *  newpid  -  Obtain a new (free) process ID
 *------------------------------------------------------------------------
 */
local	pid32	newpid(void)
{
	uint32	i;			/* iterate through all processes*/
//	static	pid32 nextpid = 1;	/* position in table to try or	*/
	static	pid32 nextpid = 0;	/* position in table to try or	*/
					/*  one beyond end of table	*/

	/* check all NPROC slots */

	for (i = 0; i < NPROC; i++) {
		nextpid %= NPROC;	/* wrap around to beginning */
		if (proctab[nextpid].prstate == PR_FREE) {
			return nextpid++;
		} else {
			nextpid++;
		}
	}
	return (pid32) SYSERR;
}
