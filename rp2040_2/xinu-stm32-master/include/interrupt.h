/* interrupt.h */

struct scb_csreg {
	volatile uint32 cpuid;  
	volatile uint32 icsr;  
	volatile uint32 vtor;  
	volatile uint32 aircr;  
	volatile uint32 scr;  
	volatile uint32 ccr;  
	volatile uint32 shpr1;  
	volatile uint32 shpr2;  
	volatile uint32 shpr3;  
	volatile uint32 shcsr;  
	volatile uint32 cfcsr;  
	volatile uint32 hfsr;  
	volatile uint32 mmar;  
	volatile uint32 bfar;  
};

/* System control block base */
#define SCB_BASE	0xE000ED00

/* Faults enable */
#define	MPUFAULT_EN	16
#define	BUSFAULT_EN	17
#define USAGEFAULT_EN	18

/* Exception vector */
#define MEMFAULT_ENTRY		4
#define BUSFAULT_ENTRY		5
#define USAGEFAULT_ENTRY	6
#define SVC_ENTRY		11
#define PENDSV_ENTRY		14
#define SYSTICK_ENTRY		15

#define IRQ_TIMER0      0
#define IRQ_TIMER1      1
#define IRQ_TIMER2      2
#define IRQ_TIMER3      3
#define IRQ_PWM_WRAP    4
#define IRQ_USBCTRL     5
#define IRQ_XIP         6
#define IRQ_0_PIO0      7
#define IRQ_1_PIO0      8
#define IRQ_0_PIO1      9
#define IRQ_1_PIO1      10
#define IRQ_DMA_0       11
#define IRQ_DMA_1       12
#define IRQ_IO_BANK0    13
#define IRQ_IO_QSPI     14
#define IRQ_SIO_PROC0   15
#define IRQ_SIO_PROC1   16
#define IRQ_CLOCKS      17
#define IRQ_SPI0        18
#define IRQ_SPI1        19
#define IRQ_UART0       20
#define IRQ_UART1       21
#define IRQ_ADC_FIFO    22
#define IRQ_I2C0        23
#define IRQ_I2C1        24
#define IRQ_RTC         25
#define IRQ_COUNT		26

#define PEND_SV()		*((uint32 *)0xE000ED04) = (1 << 28);
void nvic_init();
void nvic_register_irq(uint32 irq, void (*handler)());
void nvic_enable(uint32 irq);
void nvic_disable(uint32 irq);
void nvic_setpending(uint32 irq);
void nvic_clearpending(uint32 irq);



void systick_init();
void systick_deinit();
void systick_set(uint32 value);

uint32 systick_get();
void systick_enable();
void systick_disable();
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