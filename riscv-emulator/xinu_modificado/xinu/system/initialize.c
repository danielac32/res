 
#include <xinu.h>
#include <string.h>
 
#include <avr_serial.h>
#include <prototypes.h>
 

 
/* Function prototypes */
static void	stop();

extern  void main(void);	/* Main is the first process created	*/
static	void sysinit(); 	/* Internal system initialization	*/
extern	void meminit(void);	/* Initializes the free memory list	*/
extern void core1(int nargs, char *args[]);
extern void blink(int nargs, char *args[]);

 
/* Declarations of major kernel variables */

struct	procent	proctab[NPROC];	/* Process table			*/
struct	sentry	semtab[NSEM];	/* Semaphore table			*/
 

/* Active system status */

int32	prcount;		/* Total number of live processes	*/
pid32	currpid;		/* ID of currently executing process	*/


struct procent tasks[NPROC];
struct procent main_context;
struct procent *current_task;
int taskTop=0;  // total number of task
//int counter_task=0;

/* Control sequence to reset the console colors and cusor positiion	*/

#define	CONSOLE_RESET	" \033[0m\033[2J\033[;H"

 
 
static void coreA(int nargs, char *args[]){
    kprintf("software Interrupts\n");
     for (int i = 0; i < nargs; ++i)
	{
		kprintf("%s\n",args[i]);

	}

	while(1){
		 clkhandler();
         sys(XINU_YIELD,0,0,0);
   
	}
}

 
static void coreB(){
    kprintf("core 2\n");
    int count=0;
    
	while(1){
         
        for (int i = 0; i < 100; ++i)
        {
              //kprintf("coreB %d\n",i);
              sys(XINU_YIELD,0,0,0);
        }
     
	}
} 
 




int	nulluser()
{	
	
	//kprintf("\nInitialize the system\n");

	 sysinit();



     sys(XINU_CREATE,(long)"SosfIRQ",(long)&coreA,5,"daniel","veronica","ormary","1234","kernel");// int pid1=create("core0",&coreA,NULL);
     sys(XINU_CREATE,(long)"core1",(long)&coreB,NULL);//int pid2=create("core1",&coreB,NULL);
     sys(XINU_CREATE,(long)"shell",(long)&main,NULL);
 
     kprintf("sistema iniciado\n");
  
    
	//enable();
    while(1){
    	if(tasks[currpid].sleep == 0 && tasks[currpid].prstate==PR_CURR){
           sys(XINU_RESCHED,currpid,0,0);
        }
  
        currpid = (currpid + 1) % NPROC; // Round Robin Scheduling

    }
	/* nullprocess continues here */
	 
    return 0;
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
    

    //tasks init
    taskTop =0;
    //counter_task =0;
    currpid=0;
	/* Platform Specific Initialization */
    
    //kprintf("->Initialize uart\n");
	platinit();

    kprintf("\n\n%s%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n",
		SHELL_BAN0,SHELL_BAN1,SHELL_BAN2,SHELL_BAN3,SHELL_BAN4,
		SHELL_BAN5,SHELL_BAN6,SHELL_BAN7,SHELL_BAN8,SHELL_BAN9,SHELL_BAN10);

    /*kprintf("%s\n",SHELL_BANNER_DEFAULT);

    kprintf("%s\n",SHELL_BANNER_DEFAULT_NONVT100);
    kprintf("%s\n",SHELL_BANNER_PI);
    kprintf("%s\n",SHELL_BANNER_PI_NONVT100);    */
	//kprintf(CONSOLE_RESET);
	//const  char sysinit_m0[] = "\nWelcome to Xinu!\n\n\r\tXinu OS Copyright (c) 2012, 2015\n\r\tDouglas E. Comer and CRC Press, Inc.\n\n\r\tXinu OS for AVR atmega328p v0.1 (c) 2020\n\r\tRafael Ignacio Zurita <rafa@fi.uncoma.edu.ar>\n\n\r";
	kprintf("Welcome to Xinu!\n");
	kprintf("Xinu OS Copyright (c) 2012, 2015\n");
	kprintf("Daniel Eduardo Quintero Villegas\n");


	/* Initialize free memory list */
    kprintf("->Initialize memory\n");
	meminit();

	//kprintf("->Initialize system variables\n");
    
	/* Count the Null process as the first process in the system */
	prcount = 0;
	// prcount = 1;

	/* Scheduling is not currently blocked */

	Defer.ndefers = 0;

	kprintf("->Initialize process table entries free\n");

	for (i = 0; i < NPROC; i++) {
		prptr = &tasks[i];
		prptr->prstate = PR_FREE;
		prptr->sleep = 0;
		prptr->prname[0] = NULLCH;
		prptr->prprio = 0;
	}


	kprintf("->Initialize semaphores\n");

	for (i = 0; i < NSEM; i++) {
		semptr = &semtab[i];
		semptr->sstate = S_FREE;
		semptr->scount = 0;
		semptr->squeue = newqueue();
	}


	kprintf("->Create a ready list for processes\n");

	readylist = newqueue();
    kprintf("->Initialize devs\n");
	for (i = 0; i < NDEVS; i++) {
		init(i);
	}


	kprintf("->Initialize the real time clock\n");

	clkinit();
    
   

	return;
}

void	stop()
{
	kprintf("stop\n");
	while(1)
		/* Empty */;
}

int32	delay(int n)
{
	sys(XINU_SLEEP,n,0,0);//DELAY(n);
	return OK;
}
