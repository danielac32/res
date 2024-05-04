#ifndef __RISCV_H__
#define __RISCV_H__
#include <stdint.h>
#include "cpu.h"

void                reset(uint32_t start_addr);
uint32_t            get_opcode(uint32_t pc);
uint32_t            get_register(int r);
uint32_t            get_pc(void);
void                set_register(int r, uint32_t val);
void                set_pc(uint32_t val);
bool                 execute(void);
int                 load(uint32_t pc, uint32_t address, uint32_t *result, int width, bool signedLoad);
int                 store(uint32_t pc, uint32_t address, uint32_t data, int width);
bool                 access_csr(uint32_t address, uint32_t data, bool set, bool clr, uint32_t result);
uint32_t            access_csr2(uint32_t address, uint32_t data, bool set, bool clr);

void                exception(uint32_t cause, uint32_t pc, uint32_t badaddr);
void                rv32(uint32_t baseAddr);
void step();

void      write8(uint32_t addr, uint8_t data);
uint8_t   read8(uint32_t addr);
void      write16(uint32_t address, uint16_t val);
uint16_t  read16(uint32_t address);    
void      write32(uint32_t address, uint32_t val);
uint32_t  read32(uint32_t address);
uint32_t  ifetch32(uint32_t address);
uint16_t  ifetch16(uint32_t address);
uint8_t enable_timer;
uint32_t value_timer;
uint8_t flag;
uint8_t flag2;
#endif