
#include <stdint.h>

#ifndef REG_H_
#define REG_H_

#define bool uint8_t
#define false 0
#define true (!false)
#define MAXTHREAD 10
#define STACK_SIZE 1024
//*******************************************
static char global_path[255];


//*******************************************

/*
void *pointer[MAXTHREAD];

#define MOS_MERGE_(a,b)       a##b
#define MOS_LABEL_(line)      MOS_MERGE_(LBL, line)

#define MOS_Break(tcb){                           \
        tcb = &&MOS_LABEL_(__LINE__);             \
        return 0;                                 \
}                                                 \
MOS_LABEL_(__LINE__):                             

#define MOS_Continue(tcb) {     \
  if(tcb != NULL) {             \
     goto *tcb;                 \
  }                             \
}*/

#define    TRAP_CALL 0x3127
#define    TRAP_RET 0x2731

#define reset(){                   \
asm volatile ("li   a7, 33");      \
asm("ecall");                      \
}
/*
#define trap_call2(type,arg1,arg2,arg3,arg4){          \
asm volatile ("mv a7, %0" : : "r" (0x1237));              \
asm volatile ("mv a0, %0" : : "r" (type));            \
asm volatile ("mv a1, %0" : : "r" (arg1));            \
asm volatile ("mv a2, %0" : : "r" (arg2));            \
asm volatile ("mv a3, %0" : : "r" (arg3));            \
asm volatile ("mv a4, %0" : : "r" (arg4));            \
asm("ecall");                                         \
}

#define trap_ret2(){                                   \
asm volatile ("mv a7, %0" : : "r" (7321));              \
asm("ecall");                                         \
}*/

#define trap_call(type,arg1,arg2,arg3,arg4){          \
asm volatile ("mv a7, %0" : : "r" (TRAP_CALL));              \
asm volatile ("mv a0, %0" : : "r" (type));            \
asm volatile ("mv a1, %0" : : "r" (arg1));            \
asm volatile ("mv a2, %0" : : "r" (arg2));            \
asm volatile ("mv a3, %0" : : "r" (arg3));            \
asm volatile ("mv a4, %0" : : "r" (arg4));            \
asm volatile ("ecall");                                         \
}

#define trap_ret(){                                   \
asm volatile ("mv a7, %0" : : "r" (TRAP_RET));              \
asm volatile ("ecall");                                         \
}


#define mem(x) (*(volatile uint8_t *)x)
#define mem32(x) (*(volatile uint32_t *)x)

#define MODE (*(volatile uint8_t *)0xFA029)
#define TX (*(volatile uint8_t *)0xFA030)
//***********************************************************
#define MS (*(volatile uint32_t *)0xFA001)///MILLI
#define US (*(volatile uint32_t *)0xFA002)///MICRO
//***********************************************************
#define ENT0 (*(volatile uint8_t *)0xFA031)
#define ENRX (*(volatile uint8_t *)0xFA032)
#define RX (*(volatile uint8_t *)0xFA033)
#define LED0 (*(volatile uint8_t *)0xFA034)
#define FLAG_T0 (*(volatile uint8_t *)0xFA035)

#define reg1 (*(volatile uint32_t *)0x800)//800-900
#define reg2 (*(volatile uint32_t *)0x810)

#define cli() ENT0=1
#define sei() ENT0=0

typedef struct tarea
{
    void (*TaskPtr)(void *args,void *ptr);
    char *stack;
    uint8_t TaskID;
    void* args;
    void* ptr;
    uint8_t name[20];
    char state;
}tarea_t;



#define SYSCALL_INTERRUPT_POINTER 0x428


enum syscall_code {
    //FILES

    SYSCALL_EXIST = 99,
    SYSCALL_READ = 1,
    SYSCALL_READBUFF = 2,
    SYSCALL_WRITE = 3,
    SYSCALL_WRITEBUFF = 4,
    SYSCALL_OPEN = 5,
    SYSCALL_CLOSE = 6,
    SYSCALL_SEEK = 7, 
    SYSCALL_FSIZE = 8, 
    SYSCALL_OPENDIR = 9,
    SYSCALL_READDIR = 10,
    SYSCALL_CLOSEDIR = 11,
    
    //MALLOC
    SYSCALL_MALLOC = 12,
    SYSCALL_FREE = 13,
    
    //TASK
    SYSCALL_CREATETASK = 14,
    SYSCALL_DELETETASK = 15,
    SYSCALL_CONTEXT = 16,
    SYSCALL_GETPID = 17,
    SYSCALL_YIELD = 18,
    SYSCALL_WAIT = 19,
    SYSCALL_SLEEP = 20,
    SYSCALL_EXIT = 21,
    SYSCALL_KILL = 22,
    SYSCALL_GETARG = 26,
    SYSCALL_N_TASK = 27,
    SYSCALL_PERFIL_TASK = 28,
    //SERIAL
    SYSCALL_PRINTSTR = 23,
    SYSCALL_PRINTCHAR = 24,

    SYSCALL_EXIT2 = 25
};
//static uint32_t stack[52];
//#define stack_t 0x1ae44


/*
#define save_context      \
asm("li a0,0");           \
asm("sw  ra,0(a0)");      \
asm("sw  s0,4(a0)");      \
asm("sw  s1,8(a0)");      \
asm("sw  s2,12(a0)");     \
asm("sw  s3,16(a0)");     \
asm("sw  s4,20(a0)");     \
asm("sw  s5,24(a0)");     \
asm("sw  s6,28(a0)");     \
asm("sw  s7,32(a0)");     \
asm("sw  s8,36(a0)");     \
asm("sw  s9,40(a0)");     \
asm("sw  s10,44(a0)");    \
asm("sw  s11,48(a0)");    \
asm("sw  sp,52(a0)");             



#define restore_context     \
asm("li a0,0");             \
asm("lw  ra,0(a0)");        \
asm("lw  s0,4(a0)");        \
asm("lw  s1,8(a0)");        \
asm("lw  s2,12(a0)");       \
asm("lw  s3,16(a0)");       \
asm("lw  s4,20(a0)");       \
asm("lw  s5,24(a0)");       \
asm("lw  s6,28(a0)");       \
asm("lw  s7,32(a0)");       \
asm("lw  s8,36(a0)");       \
asm("lw  s9,40(a0)");       \
asm("lw  s10,44(a0)");      \
asm("lw  s11,48(a0)");      \
asm("lw  sp,52(a0)");       
 
*/

/*
#define MOS_MERGE_(a,b)       a##b
#define MOS_LABEL_(line)      MOS_MERGE_(LBL, line)

#define MOS_Break(tcb){ \
        tcb = &&MOS_LABEL_(__LINE__);             \
        return 0;                                 \
}                                                 \
MOS_LABEL_(__LINE__):                             

#define MOS_Continue(tcb) {   \
  if(tcb != NULL) {           \
     goto *tcb;               \
  }                           \
}
*/

//asm("li a7, %[flag]" : : [flag] "I" (x));  \
//asm("li a0, 0");                           \
//asm("ecall"); 

/*
#define scv_init   \
asm("nop");        \
asm("nop");        \
asm("nop");        \
asm("nop");        \
asm("nop");        \
asm("nop");               

#define scv_return \
asm("nop");        \
asm("nop");        \
asm("nop");        \
asm("nop");        \
asm("uret");
*/

#endif