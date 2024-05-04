#include "resets.h"
#include "gpio.h"
#include "uart.h"
#include "xosc.h"
#include "nvic.h"
#include "systick.h"


extern int main(void);
 
extern void _stack_top(void);
extern unsigned int _text;
extern unsigned int _etext;
extern unsigned int _data;
extern unsigned int _edata;
extern unsigned int _bss;
extern unsigned int _ebss;
extern unsigned int _min_stack_sz;
void startup(void);


//-----------------------------------------------------------------------------
void startup(void)
{
  unsigned int *src, *dst;

  src = &_etext;
  dst = &_data;
  while (dst < &_edata)
    *dst++ = *src++;

  dst = &_bss;
  while (dst < &_ebss)
    *dst++ = 0;



 xosc_init();
 IO_WR(CLOCKS_BASE+0x48, 0x880); // clk_peri enble

 reset_release_wait(RESET_IO_BANK0);
 reset_release_wait(RESET_PADS_BANK0);
 reset_release_wait(RESET_UART0);
 

 uart_init(0, 115200);
 gpio_init(0, GPIO_FUNC_UART);
 gpio_init(1, GPIO_FUNC_UART);
 gpio_dir(0, 1);

 gpio_init(25, GPIO_FUNC_SIO);
 gpio_dir(25, 1);



 UART_TxHead = 0;
 UART_TxTail = 0;
 UART_RxHead = 0;
 UART_RxTail = 0;
 
  nvic_init();
 uart_intr_enable(0, 1, 0);
 nvic_register_irq(IRQ_UART0, irq_uart0);
 nvic_enable(IRQ_UART0);

 
 printf("bytes of code : %10d \n",(uint32_t)&_etext - (uint32_t)&_text);
 printf("              [0x%08X to 0x%08X]\n",(uint32_t)&_text, (uint32_t)&_etext);
 printf("bytes of data : %10d \n",(uint32_t)&_ebss - (uint32_t)&_data);
 printf("              [0x%08X to 0x%08X]\n\n",(uint32_t)&_data, (uint32_t)&_ebss);
 printf("bytes of stack : %10d \n",(uint32_t)&_min_stack_sz);
 printf("[0x%08X]\n\n",(uint32_t)&_stack_top);

      asm("CPSIE   I");
 main();

  while (1);
}

//-----------------------------------------------------------------------------
 
 
