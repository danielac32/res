
#include <stdint.h>

#ifndef REG_H_
#define REG_H_

#define bool uint8_t
#define false 0
#define true (!false)
#define MAXTHREAD 4
#define STACK_SIZE 274


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
}

#define base_io 0xFA000
#define MODE (*(volatile uint8_t *)0xFA001)
#define TX (*(volatile uint8_t *)0xFA002)

#define TIMER0 (*(volatile uint32_t *)0xFA003)
#define ENT0 (*(volatile uint8_t *)0xFA004)

#define ENRX (*(volatile uint8_t *)0xFA005)
#define RX (*(volatile uint8_t *)0xFA006)
#define LED0 (*(volatile uint8_t *)0xFA007)

#define WRG1 (*(volatile uint32_t *)0xFA008)//syscall argument return 
#define WRG2 (*(volatile uint32_t *)0xFA009)//
#define WRG3 (*(volatile uint32_t *)0xFA00a)
#define WRG4 (*(volatile uint32_t *)0xFA00b)
#define WRG5 (*(volatile uint32_t *)0xFA00c)
#define WRG6 (*(volatile uint32_t *)0xFA00d)
#define WRG7 (*(volatile uint32_t *)0xFA00e)
#define SYSCALL_INTERRUPT_POINTER 0x428


enum syscall_code {
    //FILES
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
    SYSCALL_PID = 16,
    SYSCALL_GETPID = 17,
    SYSCALL_YIELD = 18,
    SYSCALL_WAIT = 19,
    SYSCALL_SLEEP = 20,
    SYSCALL_EXIT = 21,
    SYSCALL_KILL = 22,
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