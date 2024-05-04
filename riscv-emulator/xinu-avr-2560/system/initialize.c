/* initialize.c - nulluser, sysinit */

/* avr specific */

/* Handle system initialization and become the null process */

#include <xinu.h>
#include <string.h>
//#include <avr/interrupt.h>
/* avr specific */
#include <avr_serial.h>


/* Function prototypes */
int32	stop(char *s);

int32	delay(int n);
extern	void main(void);	/* Main is the first process created	*/
static	void sysinit(); 	/* Internal system initialization	*/
extern	void meminit(void);	/* Initializes the free memory list	*/
extern void core1(int nargs, char *args[]);
extern void blink(int nargs, char *args[]);
/* Declarations of major kernel variables */

struct	procent	proctab[NPROC];	/* Process table			*/
struct	sentry	semtab[NSEM];	/* Semaphore table			*/
struct	memblk	memlist;	/* List of free memory blocks		*/

/* Active system status */

int32	prcount;		/* Total number of live processes	*/
pid32	currpid;		/* ID of currently executing process	*/

/* Control sequence to reset the console colors and cusor positiion	*/

#define	CONSOLE_RESET	" \033[0m\033[2J\033[;H"

/*------------------------------------------------------------------------
 * nulluser - initialize the system and become the null process
 *
 * Note: execution begins here after the C run-time environment has been
 * established.  Interrupts are initially DISABLED, and must eventually
 * be enabled explicitly.  The code turns itself into the null process
 * after initialization.  Because it must always remain ready to execute,
 * the null process cannot execute code that might cause it to be
 * suspended, wait for a semaphore, put to sleep, or exit.  In
 * particular, the code must not perform I/O except for polled versions
 * such as kprintf.
 *------------------------------------------------------------------------
 */
 

void	nulluser()
{	
	struct	memblk	*memptr;	/* Ptr to memory block		*/
	uint32	free_mem;		/* Total amount of free memory	*/

	/* Initialize the system */

	sysinit();

	/* Output Xinu memory layout */

	free_mem = 0;
	for (memptr = memlist.mnext; memptr != NULL;
						memptr = memptr->mnext) {
		free_mem += memptr->mlength;
	}
	kprintf("\n\n\nFreeMEM:%d (bytes)\n\n", free_mem);



	/* Initialize the Null process entry */	

	int pid = create(NULL, INITSTK, 10, "nullp", 0, NULL);
	struct procent * prptr = &proctab[pid];
	prptr->prstate = PR_CURR;
	

   // enable interrupts, library wouldn't work without this
 

	/* Enable interrupts */

	enable();
	 
	/* main */
 
	//resume(create((void *)main, 440, INITPRIO, "main", 0, NULL));
    resume(create((void *)blink, 200, INITPRIO, "blink",0,NULL));
     

    kprintf("ready\n");
	/* nullprocess continues here */
	for(;;){
        kprintf("nullprocess\n");
    }
}

/*------------------------------------------------------------------------
 *
 * sysinit  -  Initialize all Xinu data structures and devices
 *
 *------------------------------------------------------------------------
 */
static	void	sysinit()
{
	int32	i;
	struct	procent	*prptr;		/* Ptr to process table entry	*/
	struct	sentry	*semptr;	/* Ptr to semaphore table entry	*/

	/* Platform Specific Initialization */

	platinit();

	//kprintf(CONSOLE_RESET);
	//const  char sysinit_m0[] = "\nWelcome to Xinu!\n\n\r\tXinu OS Copyright (c) 2012, 2015\n\r\tDouglas E. Comer and CRC Press, Inc.\n\n\r\tXinu OS for AVR atmega328p v0.1 (c) 2020\n\r\tRafael Ignacio Zurita <rafa@fi.uncoma.edu.ar>\n\n\r";
	kprintf("\nWelcome to Xinu!\n");
	kprintf("Xinu OS Copyright (c) 2012, 2015\n");
	kprintf("Daniel Eduardo Quintero Villegas\n");


	/* Initialize free memory list */

	meminit();

	/* Initialize system variables */

	/* Count the Null process as the first process in the system */
	prcount = 0;
	// prcount = 1;

	/* Scheduling is not currently blocked */

	Defer.ndefers = 0;

	/* Initialize process table entries free */

	for (i = 0; i < NPROC; i++) {
		prptr = &proctab[i];
		prptr->prstate = PR_FREE;
		prptr->prname[0] = NULLCH;
		prptr->prstkbase = NULL;
		prptr->prprio = 0;
	}


	/* Initialize semaphores */

	for (i = 0; i < NSEM; i++) {
		semptr = &semtab[i];
		semptr->sstate = S_FREE;
		semptr->scount = 0;
		semptr->squeue = newqueue();
	}


	/* Initialize buffer pools */

	bufinit();

	/* Create a ready list for processes */

	readylist = newqueue();

	for (i = 0; i < NDEVS; i++) {
		init(i);
	}


	/* Initialize the real time clock */

	clkinit();

	return;
}

int32	stop(char *s)
{
	kprintf("stop\n");
	while(1)
		/* Empty */;
}

int32	delay(int n)
{
	DELAY(n);
	return OK;
}