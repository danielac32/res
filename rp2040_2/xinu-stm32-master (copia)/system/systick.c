#include <xinu.h>

#define bits_on(x, m)   ((x) |= (m))
#define bits_off(x, m)  ((x) &= ~(m))

struct systick_hw {
	uint32 csr;
	uint32 rvr;
	uint32 cvr;
	uint32 calib;
};

#define systick 	((volatile struct systick_hw*)(PPB_BASE + 0xE010))
static uint32 csr_local;

#define CSR_COUNTFLAG 	(1 << 16)
#define CSR_CLKSRC 		(1 << 2)
#define CSR_TICKINT 	(1 << 1)
#define CSR_ENABLE 		(1 << 0)


void systick_init() {
	systick->csr = CSR_ENABLE | CSR_CLKSRC | CSR_TICKINT;
}

void systick_deinit() {
	systick->csr = 0;
}

void systick_set(uint32 value) {
	systick->rvr = value;
	systick->cvr = value;
}

uint32 systick_get() {
	return systick->cvr;
}
void systick_enable() {
    bits_on(csr_local, (1 << CSR_ENABLE));
    systick->csr = csr_local;
}

void systick_disable() {
    bits_off(csr_local, (1 << CSR_ENABLE));
    systick->csr = csr_local;
}
