#ifndef CONTEXT_H_
#define CONTEXT_H_

#include <stdint.h>

 
#define SECTION(s) __attribute__ ((section(s)))
void reset();
void irq();
void irq2();
void isr_default();


#endif /* ARCH_RISCV_CONTEXT_H_ */
