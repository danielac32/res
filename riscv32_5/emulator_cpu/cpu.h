#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#ifndef CPU_H_
#define CPU_H_

 
#define vector_interrupt 0x0010
#define vector_interrupt_syscall 0x0428
#define TIME_INTERRUPT 1000
#define PROGRAM_COUNTER_START_VAL 0x900
#define SYSCALL_INTERRUPT_POINTER 0x428

uint32_t space_reserved;
uint8_t __debug__;
uint32_t temp_reg[32];
uint32_t temp_reg2[32];

unsigned long counter_timer;
struct rv32{
uint32_t temp1;
uint32_t temp2;
uint32_t temp3;
uint8_t flag;
uint8_t trap;
uint8_t syscall_event;
uint8_t mode;
uint32_t timer;
uint16_t cycles;
uint8_t enable_timer;
uint32_t wreg1;
uint32_t wreg2;
uint32_t wreg3;
uint32_t wreg4;
uint32_t wreg5;
uint32_t wreg6;
uint32_t wreg7;
uint8_t next;
uint8_t debug_inst;
uint8_t memory[512000];
uint8_t running;
uint8_t pending_exception;
uint32_t pending_tval;
uint32_t pc;
uint32_t insn;
uint32_t reg[32];
char name[30];
};
struct rv32 riscv;
#define init_variable(){   \
int i=0;                   \
__debug__=0;\
riscv.running=1;         \
riscv.temp2=0;             \
riscv.temp1=0;             \
riscv.trap=0;             \
riscv.flag=1;             \
riscv.syscall_event=0;             \
riscv.mode=0;              \
riscv.timer=0;             \
riscv.cycles=0;            \
riscv.enable_timer=0;    \
riscv.wreg1=0;             \
riscv.wreg2=0;             \
riscv.wreg3=0;             \
riscv.wreg4=0;             \
riscv.wreg5=0;             \
riscv.wreg6=0;             \
riscv.wreg7=0;             \
riscv.pc=PROGRAM_COUNTER_START_VAL;                \
 for (i = 0; i < 32; ++i)  \
 {                         \
 	riscv.reg[i]=0;        \
 }                         \
}



void openconf(char *);
void openrom(char *);

int32_t div32(int32_t a, int32_t b);
uint32_t divu32(uint32_t a, uint32_t b);
int32_t rem32(int32_t a, int32_t b);
uint32_t remu32(uint32_t a, uint32_t b);
uint32_t mulh32(int32_t a, int32_t b);
uint32_t mulhsu32(int32_t a, uint32_t b);
uint32_t mulhu32(uint32_t a, uint32_t b);

void relative_jump(int32_t j,uint32_t *current); 
void jump(int32_t j,uint32_t *current);
void event_cycles();
void call_isr();
void ret_isr();
void raise_exception(uint32_t cause, uint32_t instruction);
void execute_instruction();
void init_soc();

void call_trap();
void ret_trap();

#define XLEN 32
#define BASE_CPU 0xFA028
//#define BC(x) BASE_CPU + x
#define MODE_CPU        0xFA029//BC(1) 
#define UART_TX_ADDR         0xFA030//BC(2) 
#define ENABLE_TIMER    0xFA031//BC(3) 
#define KEYPRESSED      0xFA032//BC(4) 
#define KEYREAD         0xFA033//BC(5) 
#define LED             0xFA034//BC(6)
#define FLAG_T0         0xFA035
/*
#define base_io 0xFA000
#define ioaddr(x) base_io + x

#define MODE_CPU        ioaddr(1) 
#define UART_TX_ADDR    ioaddr(2) 
#define TIMER           ioaddr(3) 
#define ENABLE_TIMER    ioaddr(4) 

#define KEYPRESSED      ioaddr(5) 
#define KEYREAD         ioaddr(6) 
#define LED             ioaddr(7) 
#define workregister1   ioaddr(8) 
#define workregister2   ioaddr(9)
#define workregister3   ioaddr(0xA) 
#define workregister4   ioaddr(0xB) 
#define workregister5   ioaddr(0xC) 
#define workregister6   ioaddr(0xD) 
#define workregister7   ioaddr(0xE) 
*/

// exception causes
#define CAUSE_MISALIGNED_FETCH    0x0
#define CAUSE_FAULT_FETCH         0x1
#define CAUSE_ILLEGAL_INSTRUCTION 0x2
#define CAUSE_BREAKPOINT          0x3
#define CAUSE_MISALIGNED_LOAD     0x4
#define CAUSE_FAULT_LOAD          0x5
#define CAUSE_MISALIGNED_STORE    0x6
#define CAUSE_FAULT_STORE         0x7
#define CAUSE_USER_ECALL          0x8
#define CAUSE_SUPERVISOR_ECALL    0x9
#define CAUSE_HYPERVISOR_ECALL    0xa
#define CAUSE_MACHINE_ECALL       0xb
#define CAUSE_FETCH_PAGE_FAULT    0xc
#define CAUSE_LOAD_PAGE_FAULT     0xd
#define CAUSE_STORE_PAGE_FAULT    0xf
#endif

