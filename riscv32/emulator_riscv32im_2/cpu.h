#include <stdint.h>

#ifndef CPU_H_
#define CPU_H_

struct rv32{
uint8_t memory[64001];
uint32_t wreg1;
uint32_t wreg2;
uint32_t PROGRAM_COUNTER_START_VAL;
uint8_t running;
uint8_t pending_exception;
uint32_t pending_tval;
uint32_t pc;
uint32_t insn;
uint32_t reg[32];
uint32_t temp2;
uint32_t cycles;

uint32_t timer;
uint8_t enable_timer;

uint8_t next;
uint8_t debug_inst;
};

struct rv32 riscv;

#define init_variable(){   \
int i=0;                   \
riscv.PROGRAM_COUNTER_START_VAL=0;\
riscv.enable_timer=0;      \
riscv.cycles=0;            \
riscv.running=1;           \
riscv.wreg1=0;             \
riscv.wreg2=0;             \
riscv.pc=0;                \
 for (i = 0; i < 32; ++i)  \
 {                         \
 	riscv.reg[i]=0;        \
 }                         \
 }                     

void openrom(char *);
int kbhit(void);
void raise_exception(uint32_t cause, uint32_t tval);
uint8_t target_read_u8(uint8_t *pval, uint32_t addr);
uint8_t target_read_u16(uint16_t *pval, uint32_t addr);
uint8_t target_read_u32(uint32_t *pval, uint32_t addr);
uint8_t target_write_u8(uint32_t addr, uint8_t val);
uint8_t target_write_u16(uint32_t addr, uint16_t val);
uint8_t target_write_u32(uint32_t addr, uint32_t val);
int32_t div32(int32_t a, int32_t b);
uint32_t divu32(uint32_t a, uint32_t b);
int32_t rem32(int32_t a, int32_t b);
uint32_t remu32(uint32_t a, uint32_t b);
uint32_t mulh32(int32_t a, int32_t b);
uint32_t mulhsu32(int32_t a, uint32_t b);
uint32_t mulhu32(uint32_t a, uint32_t b);
void dump_regs();
void execute_instruction();
void init_soc();
 

#define XLEN 32
/*
#define MODE_CPU     0x300001*/
#define UART_TX_ADDR 0xFA01
#define workregister1 0xfa02
#define workregister2 0xfa03
#define TIMER         0xfa04
#define ENABLE_TIMER  0xfa05
#define vector_interrupt 0xBF68

#define KEYPRESSED 0x300008
#define KEYREAD 0x300009
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

