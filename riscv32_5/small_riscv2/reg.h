
#define reset(){                   \
asm volatile ("li   a7, 33");      \
asm("ecall");                      \
}
#define MODE (*(volatile uint8_t *)0xFA029)
#define TX (*(volatile uint8_t *)0xFA030)
//***********************************************************
#define MS (*(volatile uint32_t *)0xFA001)///MILLI
#define US (*(volatile uint32_t *)0xFA002)///MICRO
//***********************************************************
#define ENT0 (*(volatile uint8_t *)0xFA031)
#define ENRX (*(volatile uint8_t *)0xFA032)
#define RX (*(volatile uint8_t *)0xFA033)
#define LED0 (*(volatile uint8_t *)0xFA034)
