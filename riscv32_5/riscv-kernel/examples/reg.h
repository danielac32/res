
#include <stdint.h>

#ifndef REG_H_
#define REG_H_


#define base_io 0xFA000
#define MODE (*(volatile uint8_t *)0xFA001)
#define TX (*(volatile uint8_t *)0xFA002)
#define TIMER0 (*(volatile uint32_t *)0xFA003)
#define ENT0 (*(volatile uint8_t *)0xFA004)

#define ENRX (*(volatile uint8_t *)0xFA005)
#define RX (*(volatile uint8_t *)0xFA006)
#define LED0 (*(volatile uint8_t *)0xFA007)

#define WRG1 (*(volatile uint32_t *)0xFA008)
#define WRG2 (*(volatile uint32_t *)0xFA009)
#define WRG3 (*(volatile uint32_t *)0xFA00a)
#define WRG4 (*(volatile uint32_t *)0xFA00b)
#define WRG5 (*(volatile uint32_t *)0xFA00c)
#define WRG6 (*(volatile uint32_t *)0xFA00d)
#define WRG7 (*(volatile uint32_t *)0xFA00e)


#define save_context      \
asm("nop");               \
asm("nop");               \
asm("addi  sp,sp,-120 "); \
asm("sw    ra,116(sp) "); \
asm("sw    t6,112(sp) "); \
asm("sw    t5,108(sp) "); \
asm("sw    t4,104(sp) "); \
asm("sw    t3,100(sp) "); \
asm("sw    s11,96(sp) "); \
asm("sw    s10,92(sp) "); \
asm("sw    s9,88(sp)  "); \
asm("sw    s8,84(sp)  "); \
asm("sw    s7,80(sp)  "); \
asm("sw    s6,76(sp)  "); \
asm("sw    s5,72(sp)  "); \
asm("sw    s4,68(sp)  "); \
asm("sw    s3,64(sp)  "); \
asm("sw    s2,60(sp)  "); \
asm("sw    a7,56(sp)  "); \
asm("sw    a6,52(sp)  "); \
asm("sw    a5,48(sp)  "); \
asm("sw    a4,44(sp)  "); \
asm("sw    a3,40(sp)  "); \
asm("sw    a2,36(sp)  "); \
asm("sw    a1,32(sp)  "); \
asm("sw    a0,28(sp)  "); \
asm("sw    s1,24(sp)  "); \
asm("sw    s0,20(sp)  "); \
asm("sw    t2,16(sp)  "); \
asm("sw    t1,12(sp)  "); \
asm("sw    t0,8(sp)   "); \
asm("sw    tp,4(sp)   "); \
asm("sw    gp,0(sp)   "); 

#define restore_context     \
asm("nop");                 \
asm("lw    ra,116(sp) ");   \
asm("lw    t6,112(sp) ");   \
asm("lw    t5,108(sp) ");   \
asm("lw    t4,104(sp) ");   \
asm("lw    t3,100(sp) ");   \
asm("lw    s11,96(sp) ");   \
asm("lw    s10,92(sp) ");   \
asm("lw    s9,88(sp)  ");   \
asm("lw    s8,84(sp)  ");   \
asm("lw    s7,80(sp)  ");   \
asm("lw    s6,76(sp)  ");   \
asm("lw    s5,72(sp)  ");   \
asm("lw    s4,68(sp)  ");   \
asm("lw    s3,64(sp)  ");   \
asm("lw    s2,60(sp)  ");   \
asm("lw    a7,56(sp)  ");   \
asm("lw    a6,52(sp)  ");   \
asm("lw    a5,48(sp)  ");   \
asm("lw    a4,44(sp)  ");   \
asm("lw    a3,40(sp)  ");   \
asm("lw    a2,36(sp)  ");   \
asm("lw    a1,32(sp)  ");   \
asm("lw    a0,28(sp)  ");   \
asm("lw    s1,24(sp)  ");   \
asm("lw    s0,20(sp)  ");   \
asm("lw    t2,16(sp)  ");   \
asm("lw    t1,12(sp)  ");   \
asm("lw    t0,8(sp)   ");   \
asm("lw    tp,4(sp)   ");   \
asm("lw    gp,0(sp)   ");   \
asm("addi  sp,sp,120  ");   \
asm("uret");   


//SCV
#define SYSCALL(x)                         \
asm("li a7, %[flag]" : : [flag] "I" (x));  \
asm("li a0, 0");                           \
asm("ecall"); 

#define scv_init   \
asm("nop");        \
asm("nop");        \
asm("nop"); 

#define scv_return \
asm("nop");        \
asm("nop");        \
asm("uret");


#endif