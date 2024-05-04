/* process.h - isbadpid */

/* Maximum number of processes in the system */

#ifndef NPROC
#define	NPROC		4
#endif		
/* Process state constants */

#define	PR_FREE		0	/* Process table entry is unused	*/
#define	PR_CURR		1	/* Process is currently running		*/
#define	PR_READY	2	/* Process is on ready queue		*/
#define	PR_RECV		3	/* Process waiting for message		*/
#define	PR_SLEEP	4	/* Process is sleeping			*/
#define	PR_SUSP		5	/* Process is suspended			*/
#define	PR_WAIT		6	/* Process is on semaphore queue	*/
#define	PR_RECTIM	7	/* Process is receiving with timeout	*/

/* Miscellaneous process definitions */

#define	PNMLEN		8	/* Length of process "name"		*/
#define	NULLPROC	0	/* ID of the null process		*/

/* avr specific */

#define PNREGS          38              /* size of saved register area  */
#define SSP_L           32              /* saved SP (low)               */
#define SSP_H           33              /* saved SP (high)                              */
#define SPC_L           34              /* saved PC (low)               */
#define SPC_H           35              /* saved PC (high)              */
#define SSREG           36              /* saved Status Register        */

#define	INITPS		0x80	/* initial process SREG (interrupts enabled)	*/
#define MAXARG		4
#define	INITREG		0


/* Process initialization constants */

#define	INITSTK		64	/* Initial process stack size		*/
#define	INITPRIO	20	/* Initial process priority		*/
#define	INITRET		userret	/* Address to which process returns	*/

/* Inline code to check process ID (assumes interrupts are disabled)	*/

#define	isbadpid(x)	( ((pid32)(x) < 0) || \
			  ((pid32)(x) >= NPROC) || \
			  (proctab[(x)].prstate == PR_FREE))

/* Number of device descriptors a process can have open */

#define NDESC		5	/* must be odd to make procent 4N bytes	*/

/* Definition of the process table (multiple of 32 bits) */

struct procent {		/* Entry in the process table		*/
	uint16	prstate;	/* Process state: PR_CURR, etc.		*/
	pri16	prprio;		/* Process priority			*/
	unsigned char pregs[PNREGS];/* saved context (see ctxsw)	*/
    uint16_t sp;
	int pargs;				/* initial number of arguments	*/
	void * parg[MAXARG];	/* arguments					*/
	int *paddr;			/* initial code address			*/

	char	*prstkptr;	/* Saved stack pointer			*/
	unsigned char	*prstkbase;	/* Base of run time stack		*/
	uint32	prstklen;	/* Stack length in bytes		*/
	char	prname[PNMLEN];	/* Process name				*/
	sid32	prsem;		/* Semaphore on which process waits	*/
	pid32	prparent;	/* ID of the creating process		*/
	umsg32	prmsg;		/* Message sent to this process		*/
	bool8	prhasmsg;	/* Nonzero iff msg is valid		*/
	int16	prdesc[NDESC];	/* Device descriptors for process	*/
};

/* Marker for the top of a process stack (used to help detect overflow)	*/
#define	STACKMAGIC	0x0A0AAAA9

extern	struct	procent proctab[];
extern	int32	prcount;	/* Currently active processes		*/
extern	pid32	currpid;	/* Currently executing process		*/


#define lo8(X) (((uint16_t)X)&0x00FF)
#define hi8(X) ((((uint16_t)X)&0xFF00)>>8)
#define contextPush() \
    asm volatile(\
        "push __tmp_reg__\n\t"\
        "push __zero_reg__\n\t"\
        "push r2\n\t"\
        "push r3\n\t"\
        "push r4\n\t"\
        "push r5\n\t"\
        "push r6\n\t"\
        "push r7\n\t"\
        "push r8\n\t"\
        "push r9\n\t"\
        "push r10\n\t"\
        "push r11\n\t"\
        "push r12\n\t"\
        "push r13\n\t"\
        "push r14\n\t"\
        "push r15\n\t"\
        "push r16\n\t"\
        "push r17\n\t"\
        "push r18\n\t"\
        "push r19\n\t"\
        "push r20\n\t"\
        "push r21\n\t"\
        "push r22\n\t"\
        "push r23\n\t"\
        "push r24\n\t"\
        "push r25\n\t"\
        "push r26\n\t"\
        "push r27\n\t"\
        "push r28\n\t"\
        "push r29\n\t"\
        "push r30\n\t"\
        "push r31\n\t"\
        "in __tmp_reg__,  __SREG__\n\t"\
        "push __tmp_reg__\n\t"\
        "in r28,__SP_L__\n\t"\
        "in r29,__SP_H__\n\t"\
        "clr __zero_reg__")

#define contextPop() \
    asm volatile(\
        "pop __tmp_reg__\n\t"\
        "out __SREG__, __tmp_reg__\n\t"\
        "pop r31\n\t"\
        "pop r30\n\t"\
        "pop r29\n\t"\
        "pop r28\n\t"\
        "pop r27\n\t"\
        "pop r26\n\t"\
        "pop r25\n\t"\
        "pop r24\n\t"\
        "pop r23\n\t"\
        "pop r22\n\t"\
        "pop r21\n\t"\
        "pop r20\n\t"\
        "pop r19\n\t"\
        "pop r18\n\t"\
        "pop r17\n\t"\
        "pop r16\n\t"\
        "pop r15\n\t"\
        "pop r14\n\t"\
        "pop r13\n\t"\
        "pop r12\n\t"\
        "pop r11\n\t"\
        "pop r10\n\t"\
        "pop r9\n\t"\
        "pop r8\n\t"\
        "pop r7\n\t"\
        "pop r6\n\t"\
        "pop r5\n\t"\
        "pop r4\n\t"\
        "pop r3\n\t"\
        "pop r2\n\t"\
        "pop __zero_reg__\n\t"\
        "pop __tmp_reg__")