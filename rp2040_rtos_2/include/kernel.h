#include <rp2040.h>
#ifndef KERNEL_H
#define KERNEL_H
#define MAX_NUMBER_OF_TASKS 12


int create(
      int       (*procaddr)(),  /* procedure address        */
      int   ssize,      /* stack size in bytes      */
      int       priority,   /* process priority > 0     */
      char      *name,      /* name (for debugging)     */
      int   nargs,      /* number of args that follow   */
      ...
    );

void init();

void panic();
int   newpid(void);
void delay(long);


#define MAX_ARG 4
#define	PR_FREE		0	/* Process table entry is unused	*/
#define	PR_CURR		1	/* Process is currently running		*/
#define	PR_READY	2	/* Process is on ready queue		*/
#define	PR_RECV		3	/* Process waiting for message		*/
#define	PR_SLEEP	4	/* Process is sleeping			*/
#define	PR_SUSP		5	/* Process is suspended			*/
#define	PR_WAIT		6	/* Process is on semaphore queue	*/
#define	PR_RECTIM	7	/* Process is receiving with timeout	*/
#define	PR_KILL     8

extern uint32_t *swch(uint32_t *stack);
extern	uint32_t	disable(void);
extern	void	enable(void);
extern	void	restore(uint32_t);
extern void init_stack(uint32_t *);
struct procent
{
	  uint32_t	*prstkptr;	/* Saved stack pointer			*/
	  uint32_t	*prstkbase;	/* Base of run time stack		*/
    uint32_t    *sp;
	  uint32_t	prstklen;	/* Stack length in bytes		*/
    int pargs;              
    void *parg[MAX_ARG]; 
    int *paddr;
    int pid;
    uint8_t prio;
    uint16_t state;
    uint32_t wait_ticks;
    uint8_t name[16];
    struct procent *next;
};





uint32_t dummy[32];
void kernel_init();

extern struct procent proctab[];
uint8_t currpid;
uint8_t preempt;
uint8_t ntask;
//void insert(int pid);
uint8_t extract();
uint8_t getpid();
void resched(void);
void kill();
void unlock(int);
#endif /* KERNEL_H */
