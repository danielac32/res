
#include <xinu.h>
 

extern int nulluser(void);
 
extern unsigned int _etext;
extern unsigned int _data;
extern unsigned int _edata;
extern unsigned int _bss;
extern unsigned int _ebss;
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
  
  nvic_init();
    
  reset_release_wait(RESET_IO_BANK0);
  reset_release_wait(RESET_PADS_BANK0);
  reset_release_wait(RESET_UART0);
  
  uart_init(0, 115200);
  gpio_init(0, GPIO_FUNC_UART);
  gpio_init(1, GPIO_FUNC_UART);
  gpio_dir(0, 1);
  gpio_init(25, GPIO_FUNC_SIO);
  gpio_dir(25, 1);
  uart_intr_enable(0, 1, 0);
  nvic_register_irq(IRQ_UART0, irq_uart0);
  nvic_enable(IRQ_UART0);
  nulluser();

  while (1);
}

//-----------------------------------------------------------------------------
 
 
