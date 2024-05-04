/* kernel.h */

/* avr specific values. Original saved under orig/ folder */

/* General type declarations used throughout the kernel */

typedef	unsigned char	byte;
typedef	unsigned char	uint8;
typedef	long		int32;
typedef	int		int16;
typedef	unsigned long	uint32;
typedef	unsigned int	uint16;
typedef	unsigned long long uint64;

 

/* Xinu-specific types */

typedef	int16	pri16;		/* process priority			*/
typedef	uint32	umsg32;		/* message passed among processes	*/
typedef	int32	bpid32;		/* buffer pool ID			*/
typedef	byte	bool8;		/* Boolean type				*/

/* Xinu-specific types */

typedef int   sid32;          /* semaphore ID                         */
typedef int16   qid16;          /* queue ID                             */
typedef int32   pid32;          /* process ID                           */
typedef int32   did32;          /* device ID                            */
// typedef uint32  intmask;        /* saved interrupt mask                 */
typedef uint8  intmask;        /* saved interrupt mask                 */
typedef int32   ibid32;         /* index block ID (used in file system) */
typedef int32   dbid32;         /* data block ID (used in file system)  */
typedef int32   uid32;          /* ID for UDP table descriptor          */


//typedef	char	sid32;		/* semaphore ID				*/
//typedef	char	qid16;		/* queue ID				*/
//typedef	char	pid32;		/* process ID				*/
//typedef	char	did32;		/* device ID				*/
//typedef	char	intmask;	/* saved interrupt mask			*/
//typedef	char	ibid32;		/* index block ID (used in file system)	*/
//typedef	char	dbid32;		/* data block ID (used in file system)	*/
//typedef	char	uid32;		/* ID for UDP table descriptor		*/

/* Function declaration return types */

typedef int32	process;	/* top-level function of a process	*/
typedef	void	interrupt;	/* interrupt procedure			*/

typedef char	syscall;	/* system call declaration		*/
typedef char	devcall;	/* device call declaration		*/
typedef char	shellcmd;	/* shell command declaration		*/
typedef	char	status;		/* returned status value (OK/SYSERR)	*/

#define local	static		/* Local procedure or variable declar.	*/

/* Boolean constants */

#define FALSE	0		/* Boolean False			*/
#define TRUE	1		/* Boolean True				*/

/* Null pointer, character, and string definintions */

#define NULL	0		/* null pointer for linked lists	*/
#define NULLCH	'\0'		/* null character			*/
#define	NULLSTR	""		/* null string				*/

/* Universal return constants */

#define OK	( 1)		/* normal system call return		*/
#define	SYSERR	(-1)		/* system call failed			*/
#define	EOF	(-2)		/* End-of-file (usually from read)	*/
#define	TIMEOUT	(-3)		/* system call timed out		*/

extern	qid16	readylist;	/* global ID for list of ready processes*/

#define	MINSTK	64		/* minimum stack size in bytes		*/

#define	CONTEXT	64		/* bytes in a function call context on	*/
				/* the run-time stack			*/
#define	QUANTUM	1		/* time slice in milliseconds		*/

/* Size of the stack for the null process */

#define	NULLSTK		64	/* stack size for null process		*/

typedef struct
{
	uint8 EN;
	uint32 VAL;
}IRQ;
#define TIMER    ( ( IRQ * )  0x4000200 )

typedef struct 
{
	uint8 GET;
	uint8 PUT;
	uint8 ENA;
}console;
#define SERIAL ( ( console * )  0x4000300 )

#define REG_RA                         1
#define REG_SP                         2
#define REG_ARG0                       10
#define REG_RET                        REG_ARG0
#define NUM_GP_REG                     32
#define NUM_CSR_REG                    3

#define CAUSE_MISALIGNED_FETCH         0
#define CAUSE_FAULT_FETCH              1
#define CAUSE_ILLEGAL_INSTRUCTION      2
#define CAUSE_BREAKPOINT               3
#define CAUSE_MISALIGNED_LOAD          4
#define CAUSE_FAULT_LOAD               5
#define CAUSE_MISALIGNED_STORE         6
#define CAUSE_FAULT_STORE              7
#define CAUSE_ECALL_U                  8
#define CAUSE_ECALL_S                  9
#define CAUSE_ECALL_M                  11
#define CAUSE_PAGE_FAULT_INST          12
#define CAUSE_PAGE_FAULT_LOAD          13
#define CAUSE_PAGE_FAULT_STORE         15
#define CAUSE_INTERRUPT                (1 << 31)
#define CAUSE_MAX_EXC      (CAUSE_PAGE_FAULT_STORE + 1)

struct irq_context
{
    uint32 pc;
    uint32 status;
    uint32 cause;
    uint32 reg[32];
};

extern void sys_switch(void *ctx_old,void *ctx_new);
void exception_dump_ctx(struct irq_context *ctx);
void *exception(struct irq_context *ctx);
struct irq_context *syscall_handler(struct irq_context *ctx);


static int eeprom_pos;
#define EEPROM_SIZE 1000
void dlay(uint32 d);

#define min(a, b) ({ typeof(a) _a = (a); typeof(b) _b = (b); \
                            (_a < _b) ? _a : _b; })

/** Type-independent macro to calculate the maximum of 2 values.  */
#define max(a, b) ({ typeof(a) _a = (a); typeof(b) _b = (b); \
                            (_a > _b) ? _a : _b; })

/** Perform integer division, rounding up the quotient.  */
#define DIV_ROUND_UP(num, denom) (((num) + (denom) - 1) / (denom))

/** Get the number of elements in an array (not dynamically allocated)  */
#define ARRAY_LEN(array) (sizeof(array) / sizeof((array)[0]))
/* Prototypes of I/O functions used throughout the kernel */

