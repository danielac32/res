#include <stdint.h>

#define ioaddr(x) 0xFA000 + x

#define MODE_CPU        (*(volatile uint8_t *)0xFA001)
#define UART_TX_ADDR    (*(volatile uint8_t *)0xFA002)
#define CPUTIMER           (*(volatile uint32_t *)0xFA003)
#define ENABLE_TIMER    (*(volatile uint8_t *)0xFA004)
#define keyAvailable (*(volatile uint8_t *)0xFA005)
#define keyRead      (*(volatile uint8_t *)0xFA006)
#define led             (*(volatile uint8_t *)0xFA007)

#define workregister1   (*(volatile uint32_t *)0xFA008)
#define workregister2   (*(volatile uint32_t *)0xFA009)
#define workregister3   (*(volatile uint32_t *)0xFA00A)
#define workregister4   (*(volatile uint32_t *)0xFA00B)
#define workregister5   (*(volatile uint32_t *)0xFA00C)
#define workregister6   (*(volatile uint32_t *)0xFA00D)
#define workregister7   (*(volatile uint32_t *)0xFA00E)
char buff[100];