#include "resets.h"
#include "gpio.h"

#define LED   25


void delay(int t) {
  while (t--)
    asm volatile ("nop");
}


void main() {
  reset_release_wait(RESET_IO_BANK0);
  reset_release_wait(RESET_PADS_BANK0);

  gpio_init(LED, GPIO_FUNC_SIO);
  gpio_dir(LED, 1);

  while(1){
    gpio_set(LED, 0);
    delay(100000);
    gpio_set(LED, 1);
    delay(10000);
  }
}
