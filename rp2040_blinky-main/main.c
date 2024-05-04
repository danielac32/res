#include "timer.h"
#include "gpio.h"
//#include "uart.h"

#define PICO_DEFAULT_LED_PIN 25
#define UART_ID uart0
#define BAUD_RATE 115200

// We are using pins 0 and 1, but see the GPIO function select table in the
// datasheet for information on which other pins can be used.
#define UART_TX_PIN 0
#define UART_RX_PIN 1

//-----------------------------------------------------------------
// Main: Main loop
//-----------------------------------------------------------------
int main(void)
{
    const uint LED_PIN = PICO_DEFAULT_LED_PIN;
    
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);

  /*  uart_init(UART_ID, BAUD_RATE);

    gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART);
    gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART);

  
    uart_puts(UART_ID, " Hello, UART!\n");*/

    while (true)
    {
        gpio_put(LED_PIN, 1);
        timer_sleep_ms(100);
        gpio_put(LED_PIN, 0);
        timer_sleep_ms(1000);
    }

    return 0;
}