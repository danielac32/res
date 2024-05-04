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

/* initialize.c - nulluser, sysinit */

/* Handle system initialization and become the null process */

#include <xinu.h>
#include <string.h>

extern	void	start(void);	/* Start of Xinu code			*/
//extern	void	*_end;		/* End of Xinu code			*/

/* Function prototypes */
extern unsigned int _text;
extern unsigned int _etext;
extern unsigned int _data;
extern unsigned int _edata;
extern unsigned int _bss;
extern unsigned int _ebss;

extern	void main(void);	/* Main is the first process created	*/
void sysinit(); 	/* Internal system initialization	*/
extern	void meminit(void);	/* Initializes the free memory list	*/
extern 	int32 initintc(void);
uint32 dummy[32];
extern  void init_stack(uint32 *stack);

int currpid=0;
int ntask=0;
int lastTask;
//typedef uint32_t piccolo_sleep_t;

 

 





/* Declarations of major kernel variables */

struct	procent	proctab[NPROC];	/* Process table			*/
struct	sentry	semtab[NSEM];	/* Semaphore table			*/
struct	memblk	memlist;	/* List of free memory blocks		*/

/* Active system status */

int	prcount;		/* Total number of live processes	*/
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
 * such as printf.
 *------------------------------------------------------------------------
 */
 
void blink(){
    int b=0;
	for(;;){
		gpio_set(25, 1);
		delay(100000);
		gpio_set(25, 0);
		delay(100000);
	}
}

int32	n = 0;			/* n assigned an initial value of zero	*/


void	prod2(int nargs, char *args[]);
void	cons2(int nargs, char *args[]);

void	prod2(int nargs, char *args[])
{
	sid32 consumed = args[0];
	sid32 produced = args[1];
	int32	i;

	for( i=1 ; i<=2000 ; i++ ) {
		wait(consumed);
		n++;
		signal(produced);
	}
}

/*------------------------------------------------------------------------
 * cons2  --  print n 2000 times, waiting for it to be produced
 *------------------------------------------------------------------------
 */
void	cons2(int nargs, char *args[])
{
	sid32 consumed = args[0];
	sid32 produced = args[1];
	int32	i;

	for( i=1 ; i<=2000 ; i++ ) {
		wait(produced);
		printf("n is %d \n", n);
		signal(consumed);
	}
}


void nullprocess(void) {
	sid32	produced, consumed;

	consumed = semcreate(0);
	produced = semcreate(1);
	resume( create(cons2, 256, 20, "cons", 2, consumed, produced) );
	resume( create(prod2, 256, 20, "prod", 2, consumed, produced) );

	
}




 



void	nulluser()
{	
	struct	memblk	*memptr;	/* Ptr to memory block		*/
	uint32	free_mem;		/* Total amount of free memory	*/
    disable();
	/* Initialize the system */

	sysinit();
    

	/* Output Xinu memory layout */

	free_mem = 0;
	for (memptr = memlist.mnext; memptr != NULL;
						memptr = memptr->mnext) {
		free_mem += memptr->mlength;
	}
	printf("\n%d bytes of free memory.  Free list:\n", free_mem);
	for (memptr=memlist.mnext; memptr!=NULL;memptr = memptr->mnext) {
	    printf("           [0x%08X to 0x%08X]\n",(uint32)memptr, ((uint32)memptr) + memptr->mlength - 1);
	}
    
	printf("%d bytes of Xinu code.\n",(uint32)&_etext - (uint32)&_text);
	printf("           [0x%08X to 0x%08X]\n",(uint32)&_text, (uint32)&_etext - 1);
	printf("%d bytes of data.\n",(uint32)&_ebss - (uint32)&_data);
	printf("           [0x%08X to 0x%08X]\n\n",(uint32)&_data, (uint32)&_ebss - 1);
    printf("number process: %d\n",NPROC);
    
    

    struct	dentry	*devptr;	 
	int32	i;		 

	printf("Device     Name     Minor\n");
	printf("------ ------------ -----\n");

	for (i = 0; i < NDEVS; i++) {
		devptr = &devtab[i];
		printf("%4d   %-12s %3d\n", i, devptr->dvname,devptr->dvminor);
	}

 
   //piccolo_create_task((void *)blink);
   //piccolo_create_task((void *)nullprocess); 
   ///piccolo_create_task((void *)main); 
 
	/* Initialize the Null process entry */	
	int pid = create((void *)blink, INITSTK, 11, "Null process", 0, NULL);
	resume(pid);
	
    //resume(create((void *)main, INITSTK, 11, "Null process", 0, NULL));

	//struct procent * prptr = &proctab[pid];
	//prptr->prstate = PR_CURR;
 
    //proctab[pid].prstkptr=swch(proctab[pid].prstkptr);
   

	init_stack(dummy + 32);
    systick_init();
    systick_set(12000);
    enable();

    resume(create((void *)nullprocess, INITSTK, 11, "Null process", 0, NULL));
    //piccolo_start();
     printf("running..\n");
    resched();
  while(1){
    
         
    }

	

}




/*------------------------------------------------------------------------
 *
 * sysinit  -  Initialize all Xinu data structures and devices
 *
 *------------------------------------------------------------------------
 */
void	sysinit()
{
	int32	i;
	struct	procent	*prptr;		/* Ptr to process table entry	*/
	struct	sentry	*semptr;	/* Ptr to semaphore table entry	*/
 
   

	/* Platform Specific Initialization */
    currpid=0;



	printf(CONSOLE_RESET);
	printf("\n%s\n\n", VERSION);
    printf("\nWelcome to Xinu!\n");
	printf("Xinu OS Copyright (c) 2012, 2015\n");
	printf("Daniel Eduardo Quintero Villegas\n");
	/* Initialize free memory list */
	
	meminit();

	/* Initialize system variables */

	/* Count the Null process as the first process in the system */
	prcount = 0;
//	prcount = 1;

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
	clkinit();
}

int32	stop(char *s)
{
	printf("%s\n", s);
	printf("looping... press reset\n");
	while(1)
		/* Empty */;
}

int32	delay(int n)
{
	DELAY(n);
	return OK;
}
