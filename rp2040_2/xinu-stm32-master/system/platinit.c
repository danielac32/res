/* Xinu for STM32
 *
 * Original license applies
 * Modifications for STM32 by Robin Krens
 * Please see LICENSE and AUTHORS 
 * 
 * $LOG$
 * 2019/11/11 - ROBIN KRENS
 * Initial version 
 * 
 * $DESCRIPTION$
 *
 * */

/* platinit.c - platinit */

#include <xinu.h>
 

void uartinit();

/*------------------------------------------------------------------------
 * platinit - platform specific initialization
 *------------------------------------------------------------------------
 */
void platinit(void)
{
	/* Initialize UART Serial communication */
	uartinit();
	//nvic_init();
}

/* Simple UART initialization, not using a TTY device
Used by kprintf and early system debugging */

void uartinit()
{
    uart_init(0, 115200);
    gpio_init(0, GPIO_FUNC_UART);
    gpio_init(1, GPIO_FUNC_UART);
    gpio_dir(0, 1);
    gpio_init(25, GPIO_FUNC_SIO);
    gpio_dir(25, 1);
    uart_intr_enable(0, 1, 0);
    nvic_register_irq(IRQ_UART0, irq_uart0);
    nvic_enable(IRQ_UART0);
}
