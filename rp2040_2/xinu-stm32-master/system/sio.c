#include "xinu.h"

#define CPUID 			((volatile uint32*)(SIO_BASE + 0x000))

#define GPIO_IN 		((volatile uint32*)(SIO_BASE + 0x004))
#define GPIO_HI_IN 		((volatile uint32*)(SIO_BASE + 0x008))
#define GPIO_OUT 		((volatile uint32*)(SIO_BASE + 0x010))
#define GPIO_OUT_SET 	((volatile uint32*)(SIO_BASE + 0x014))
#define GPIO_OUT_CLR 	((volatile uint32*)(SIO_BASE + 0x018))
#define GPIO_OUT_XOR 	((volatile uint32*)(SIO_BASE + 0x01C))
#define GPIO_OE 		((volatile uint32*)(SIO_BASE + 0x020))
#define GPIO_OE_SET 	((volatile uint32*)(SIO_BASE + 0x024))
#define GPIO_OE_CLR 	((volatile uint32*)(SIO_BASE + 0x028))
#define GPIO_OE_XOR 	((volatile uint32*)(SIO_BASE + 0x02C))
#define GPIO_HI_OUT 	((volatile uint32*)(SIO_BASE + 0x030))
#define GPIO_HI_OUT_SET ((volatile uint32*)(SIO_BASE + 0x034))
#define GPIO_HI_OUT_CLR ((volatile uint32*)(SIO_BASE + 0x038))
#define GPIO_HI_OUT_XOR ((volatile uint32*)(SIO_BASE + 0x03C))
#define GPIO_HI_OE 		((volatile uint32*)(SIO_BASE + 0x040))
#define GPIO_HI_OE_SET 	((volatile uint32*)(SIO_BASE + 0x044))
#define GPIO_HI_OE_CLR 	((volatile uint32*)(SIO_BASE + 0x048))
#define GPIO_HI_OE_XOR 	((volatile uint32*)(SIO_BASE + 0x04C))

#define FIFO_ST 		((volatile uint32*)(SIO_BASE + 0x050))
#define FIFO_WR 		((volatile uint32*)(SIO_BASE + 0x054))
#define FIFO_RD 		((volatile uint32*)(SIO_BASE + 0x058))

#define SPINLOCK_ST 	((volatile uint32*)(SIO_BASE + 0x05C)) // 1 = locked

#define DIV_UDIVIDEND	((volatile uint32*)(SIO_BASE + 0x060))
#define DIV_UDIVISOR	((volatile uint32*)(SIO_BASE + 0x064))
#define DIV_SDIVIDEND	((volatile uint32*)(SIO_BASE + 0x068))
#define DIV_SDIVISOR	((volatile uint32*)(SIO_BASE + 0x06C))
#define DIV_QUOTIENT	((volatile uint32*)(SIO_BASE + 0x070))
#define DIV_REMAINDER	((volatile uint32*)(SIO_BASE + 0x074))
#define DIV_CSR			((volatile uint32*)(SIO_BASE + 0x078))

#define SPINLOCK_N		((volatile uint32*)(SIO_BASE + 0x100)) // 0-31


#define FIFO_ST_ROE		(1 << 3) // RX FIFO was read when empty
#define FIFO_ST_WOF		(1 << 2) // TX FIFO was written when full
#define FIFO_ST_RDY		(1 << 1) // 1 when TX FIFO is not full
#define FIFO_ST_VLD		(1 << 0) // 1 when RX FIFO is not empty

#define DIV_CSR_DIRTY	(1 << 1)
#define DIV_CSR_READY	(1 << 0)


uint32 cpuid() {
	return *CPUID;
}

uint32 gpio_in(uint32 gpio) {
	return *GPIO_IN & (1 << gpio);
}

uint32 gpio_hi_in(uint32 gpio) {
	return *GPIO_HI_IN & (1 << gpio);
}

void gpio_out(uint32 gpio, uint32 value) {
	if (value)
		*GPIO_OUT_SET = 1 << gpio;
	else
		*GPIO_OUT_CLR = 1 << gpio;
}

void gpio_oe(uint32 gpio, uint32 oe) {
	if (oe)
		*GPIO_OE_SET = 1 << gpio;
	else
	 	*GPIO_OE_CLR = 1 << gpio;
}

void gpio_hi_out(uint32 gpio, uint32 value) {
	if (value)
		*GPIO_HI_OUT_SET = 1 << gpio;
	else
		*GPIO_HI_OUT_CLR = 1 << gpio;
}

void gpio_hi_oe(uint32 gpio, uint32 oe) {
	if (oe)
		*GPIO_HI_OE_SET = 1 << gpio;
	else
	 	*GPIO_HI_OE_CLR = 1 << gpio;
}

int fifo_write(uint32 value) {
	if (!(*FIFO_ST & FIFO_ST_RDY))
		return -1;

	*FIFO_WR = value;
	return 0;
}

int fifo_read(uint32 *value) {
	if (!(*FIFO_ST & FIFO_ST_VLD))
		return -1;

	*value = *FIFO_RD;
	return 0;
}

uint32 spinlock_get(uint32 n) {
	return *SPINLOCK_ST & (1 << n);
}

uint32 spinlock_lock(uint32 n) {
	uint32 notlocked = *(SPINLOCK_N + 4 * n);
	if (notlocked)
		return 0;
	else
		return -1;
}

void spinlock_unlock(uint32 n) {
	*(SPINLOCK_N + 4 * n) = 1;
}
