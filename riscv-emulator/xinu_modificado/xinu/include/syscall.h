/* Xinu system calls */

enum {
	XINU_NULLPROCESS = 0,
	XINU_UPTIME,
	XINU_PUTC,
	XINU_CLOSE, /* Devices */
	XINU_CONTROL,
	XINU_GETC,
	XINU_INIT,
	XINU_OPEN,
	XINU_READ,
	XINU_SEEK,
	XINU_WRITE,

	XINU_CREATE, /* Processes */ 
	XINU_CHPRIO,
	XINU_KILL,	/* Exit */
	XINU_GETPRIO,
	XINU_READY,
	XINU_RECEIVE,
	XINU_RECVCLR,
	XINU_RECVTIME,
	XINU_RESCHED_CNTL,
	XINU_RESUME,
	XINU_SEND,
	XINU_SIGNAL,
	XINU_SIGNALN,
	XINU_SLEEP,
	XINU_SLEEPMS,
	XINU_SUSPEND,
	XINU_UNSLEEP,
	XINU_WAKE,
	XINU_WAKEUP, 
	XINU_YIELD,
	XINU_RESCHED,
	
	XINU_MKBUFPOOL, /* Buffers */
	XINU_FREEBUF,
	XINU_FREEMEM,
	XINU_GETBUF,
	
	XINU_KPRINTF, /* Kernel */
	XINU_KPUTC,
	XINU_GETDEV,
	XINU_GETMEM,
	XINU_GETSTK,
	
	XINU_PTCOUNT, /* Ports */
	XINU_PTCREATE,
	XINU_PTDELETE,
	XINU_PTINIT,
	XINU_PTRCV,
	XINU_PTRESET,
	
	XINU_SEMCOUNT, /* Semaphores */
	XINU_SEMCREATE,
	XINU_SEMRESET,
	XINU_SEMDELET,

};

long sys(long num, long arg0, long arg1, long arg2,...);
/*
static long sys(long num, long arg0, long arg1, long arg2)
{
  register long a7 asm("a7") = num;
  register long a0 asm("a0") = arg0;
  register long a1 asm("a1") = arg1;
  register long a2 asm("a2") = arg2;
  asm volatile ("ecall" : "+r"(a0) : "r"(a1), "r"(a2), "r"(a7));
  return a0;
}
*/

//sys(XINU_CREATE,(long)"software Interrupts",(long)&coreA,NULL);
//pid32   create(char *name,void (*entry)(void*),int nargs,...);