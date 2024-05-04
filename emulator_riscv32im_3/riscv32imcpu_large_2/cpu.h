#include <stdint.h>

#ifndef CPU_H_
#define CPU_H_

unsigned long counter_timer;
struct rv32{
uint32_t temp2;
uint8_t mode;
uint32_t timer;
uint16_t cycles;
uint8_t _enable_timer_;
uint32_t wreg1;
uint32_t wreg2;
uint32_t wreg3;
uint32_t wreg4;
uint32_t wreg5;
uint32_t wreg6;
uint32_t wreg7;
uint32_t UART_TX_ADDR;
uint32_t PROGRAM_COUNTER_START_VAL;
uint32_t RAM_SIZE;
uint32_t base_text;
uint32_t base_text_limit;
uint32_t base_ram;
uint32_t base_ram_limit;
uint32_t base_sp;
uint32_t base_gp;
uint8_t next;
uint8_t debug_inst;
uint8_t memory[0x000fc711];
uint8_t __running;
uint8_t pending_exception;
uint32_t pending_tval;
uint32_t pc;
uint32_t insn;
uint32_t reg[32];
char name[30];
uint32_t values[12];
};
struct rv32 riscv;
#define init_variable(){   \
int i=0;                   \
riscv.__running=1;         \
riscv.temp2=0;             \
riscv.mode=0;              \
riscv.timer=0;             \
riscv.cycles=0;            \
riscv._enable_timer_=0;    \
riscv.wreg1=0;             \
riscv.wreg2=0;             \
riscv.wreg3=0;             \
riscv.wreg4=0;             \
riscv.wreg5=0;             \
riscv.wreg6=0;             \
riscv.wreg7=0;             \
riscv.pc=0;                \
 for (i = 0; i < 32; ++i)  \
 {                         \
  riscv.reg[i]=0;        \
 }                         \
 for (i = 0; i < 30; ++i)  \
 {                         \
    riscv.name[i]=0;       \
 }                         \
 for (i = 0; i < 12; ++i)  \
 {                         \
    riscv.values[i]=0;     \
 }                         \
}

void openconf(char *);
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
#define vector_interrupt 0x7a200

#define MODE_CPU     0x300001
#define TIMER        0x300002 
#define ENABLE_TIMER 0x300003
#define workregister1 0x300004
#define workregister2 0x300005
#define workregister3 0x300006
#define workregister4 0x300007
#define workregister5 0x30000a
#define workregister6 0x30000b
#define workregister7 0x30000c


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

