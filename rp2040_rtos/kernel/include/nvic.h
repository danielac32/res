#include "rp2040.h"
#ifndef NVIC_H_
#define NVIC_H_


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

#define PEND_SV()		*((unsigned int *)0xE000ED04) = (1 << 28);
void nvic_init();
void nvic_register_irq(unsigned int irq, void (*handler)());
void nvic_enable(unsigned int irq);
void nvic_disable(unsigned int irq);
void nvic_setpending(unsigned int irq);
void nvic_clearpending(unsigned int irq);


/*void reset_handler();
void nmi_handler();
void hardfault_handler();
void svccall_handler();
void pendsv_handler();
void systick_handler();*/

#endif