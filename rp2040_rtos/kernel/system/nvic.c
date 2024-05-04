

#include "nvic.h"

#define NVIC_ISER   (PPB_BASE + 0xE100)
#define NVIC_ICER   (PPB_BASE + 0xE180)
#define NVIC_ISPR   (PPB_BASE + 0xE200)
#define NVIC_ICPR   (PPB_BASE + 0xE280)
#define NVIC_IPR0	(PPB_BASE + 0xE400)
#define NVIC_IPR1	(PPB_BASE + 0xE404)
#define NVIC_IPR2	(PPB_BASE + 0xE408)
#define NVIC_IPR3	(PPB_BASE + 0xE40C)
#define NVIC_IPR4	(PPB_BASE + 0xE410)
#define NVIC_IPR5	(PPB_BASE + 0xE414)
#define NVIC_IPR6	(PPB_BASE + 0xE418)
#define NVIC_IPR7	(PPB_BASE + 0xE41C)
#define CPUID 		(PPB_BASE + 0xED00)
#define ICSR 		(PPB_BASE + 0xED04)
#define VTOR 		(PPB_BASE + 0xED08)
#define AICR 		(PPB_BASE + 0xED0C)

//extern unsigned int _reset_handler;
static unsigned int __attribute__((aligned(256))) vector_table[16 + IRQ_COUNT];





void reset_handler(){}
void nmi_handler(){}
void hardfault_handler(){}

/*
void isr_svcall(){
	  __asm(
	    ".global svccall_handler_c\n"
	    "MRS r0, MSP\n"
	    "B svccall_handler_c\n"
	  );
}*/

/*void pendsv_handler(){
  printf("pendsv_handler\n");
}*/

extern void svccall_handler_c();
extern void pendsv_handler();
extern void systick_handler();
extern void isr_svcall();

void nvic_init() {
	int i;
	for (i = 0; i < (16 + IRQ_COUNT); ++i)
		vector_table[i] = 0;

	vector_table[1] = (unsigned int)reset_handler;
	vector_table[2] = (unsigned int)nmi_handler;
	vector_table[3] = (unsigned int)hardfault_handler;
	vector_table[11] = (unsigned int)isr_svcall;
	vector_table[14] = (unsigned int)pendsv_handler;
	vector_table[15] = (unsigned int)systick_handler;
	*((volatile unsigned int*)VTOR) = (unsigned int)vector_table;
}

void nvic_register_irq(unsigned int irq, void (*handler)()) {
	vector_table[16 + irq] = (unsigned int)handler;
}

void nvic_enable(unsigned int irq) {
	*((volatile unsigned int*)(NVIC_ISER)) = 1 << irq;
}

void nvic_disable(unsigned int irq) {
	*((volatile unsigned int*)(NVIC_ICER)) = 1 << irq;
}

void nvic_setpending(unsigned int irq) {
	*((volatile unsigned int*)(NVIC_ISPR)) = 1 << irq;
}

void nvic_clearpending(unsigned int irq) {
	*((volatile unsigned int*)(NVIC_ICPR)) = 1 << irq;
}
