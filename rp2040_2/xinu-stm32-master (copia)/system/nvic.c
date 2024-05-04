#include <xinu.h>

 
//extern uint32 _reset_handler;
static uint32 __attribute__((aligned(256))) vector_table[16 + IRQ_COUNT];



static void reset_handler() {

}


static void nmi_handler() {

}

static void hardfault_handler() {

}

/*
void svccall_handler_c(uint32 *sp) {
	uint32 svc_number = ((char *)sp[6])[-2];
	uint32 svc_arg1 = sp[0];
	uint32 svc_arg2 = sp[1];
	uint32 result = svc_arg1 + svc_arg2;
    struct procent * p;

	printf("SVC num = %x\n",svc_number);

	printf("arg1: 0x%08x arg2: 0x%08x\n",svc_arg1,svc_arg2);
    switch(svc_number){
    	case 1:
            p = (struct procent *) svc_arg2; 
            //set_psp(p->prstkptr);
     
    	break;
    }
 
}


void svccall_handler() {
   __asm(
	    ".global svccall_handler_c\n"
	    "MRS r0, MSP\n"
	    "B svccall_handler_c\n"
	  );
}
*/

//static void pendsv_handler() {
  //printf("pendsv\r\n");
//}

//static void systick_handler() {
//	printf("systick\r\n");
//}



extern void systick_handler();
extern void pendsv_handler();
extern void svccall_handler();
extern void isr_svcall();

void nvic_init() {
	int i;
	for (i = 0; i < (16 + IRQ_COUNT); ++i)
		vector_table[i] = 0;

	vector_table[1] = (uint32)reset_handler;
	vector_table[2] = (uint32)nmi_handler;
	vector_table[3] = (uint32)hardfault_handler;
	vector_table[11] = (uint32)isr_svcall;
	vector_table[14] = (uint32)pendsv_handler;
	vector_table[15] = (uint32)systick_handler;
	*((volatile uint32*)VTOR) = (uint32)vector_table;
}

void nvic_register_irq(uint32 irq, void (*handler)()) {
	vector_table[16 + irq] = (uint32)handler;
}

void nvic_enable(uint32 irq) {
	*((volatile uint32*)(NVIC_ISER)) = 1 << irq;
}

void nvic_disable(uint32 irq) {
	*((volatile uint32*)(NVIC_ICER)) = 1 << irq;
}

void nvic_setpending(uint32 irq) {
	*((volatile uint32*)(NVIC_ISPR)) = 1 << irq;
}

void nvic_clearpending(uint32 irq) {
	*((volatile uint32*)(NVIC_ICPR)) = 1 << irq;
}
