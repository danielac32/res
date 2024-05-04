#include <xinu.h>

struct sio_hw {
	uint32 cpuid;
	uint32 gpio_in;
	uint32 gpio_hi_in;
	uint32 unused;
	uint32 gpio_out;
	uint32 gpio_out_set;
	uint32 gpio_out_clr;
	uint32 gpio_out_xor;
	uint32 gpio_oe;
	uint32 gpio_oe_set;
	uint32 gpio_oe_clr;
	uint32 gpio_oe_xor;
	uint32 gpio_hi_out;
	uint32 gpio_hi_out_set;
	uint32 gpio_hi_out_clr;
	uint32 gpio_hi_out_xor;
	uint32 gpio_hi_oe;
	uint32 gpio_hi_oe_set;
	uint32 gpio_hi_oe_clr;
	uint32 gpio_hi_oe_xor;
};

struct io_bank0_hw {
	struct gpio {
		uint32 status;
		uint32 ctrl;
	} gpio[30];
	uint32 intr[4];
	uint32 proc0_inte[4];
	uint32 proc0_intf[4];
	uint32 proc0_ints[4];
	uint32 proc1_inte[4];
	uint32 proc1_intf[4];
	uint32 proc1_ints[4];
	uint32 dormant_wake_inte[4];
	uint32 dormant_wake_intf[4];
	uint32 dormant_wake_ints[4];
};

struct pads_bank0_hw {
	uint32 voltage_select;
	uint32 gpio[30];
	uint32 swclk;
	uint32 swd;
};

#define sio		((volatile struct sio_hw*)SIO_BASE)
#define io		((volatile struct io_bank0_hw*)IO_BANK0_BASE)
#define pads 	((volatile struct pads_bank0_hw*)PADS_BANK0_BASE)

#define IOBANK_INT_EDGE_HIGH		4
#define IOBANK_INT_EDGE_LOW			3
#define IOBANK_INT_LEVEL_HIGH		2
#define IOBANK_INT_LEVEL_LOW		1
#define IOBANK_CTRL_IRQOVER_HIGH	(3 << 28)
#define IOBANK_CTRL_IRQOVER_LOW		(2 << 28)
#define IOBANK_CTRL_IRQOVER_INV		(1 << 28)
#define IOBANK_CTRL_IRQOVER_FSL		(0 << 28)
#define IOBANK_CTRL_INOVER_HIGH		(3 << 16)
#define IOBANK_CTRL_INOVER_LOW		(2 << 16)
#define IOBANK_CTRL_INOVER_INV		(1 << 16)
#define IOBANK_CTRL_INOVER_FSL		(0 << 16)
#define IOBANK_CTRL_OEOVER_ENO		(3 << 12)
#define IOBANK_CTRL_OEOVER_DSO		(2 << 12)
#define IOBANK_CTRL_OEOVER_INV		(1 << 12)
#define IOBANK_CTRL_OEOVER_FSL		(0 << 12)
#define IOBANK_CTRL_OUTOVER_HIGH	(3 << 8)
#define IOBANK_CTRL_OUTOVER_LOW		(2 << 8)
#define IOBANK_CTRL_OUTOVER_INV		(1 << 8)
#define IOBANK_CTRL_OUTOVER_FSL		(0 << 8)
#define IOBANK_CTRL_FUNCSEL_USB		9
#define IOBANK_CTRL_FUNCSEL_CLK		8
#define IOBANK_CTRL_FUNCSEL_PIO1	7
#define IOBANK_CTRL_FUNCSEL_PIO0	6
#define IOBANK_CTRL_FUNCSEL_SIO		5
#define IOBANK_CTRL_FUNCSEL_PWM		4
#define IOBANK_CTRL_FUNCSEL_I2C		3
#define IOBANK_CTRL_FUNCSEL_UART	2
#define IOBANK_CTRL_FUNCSEL_SPI		1
#define IOBANK_CTRL_FUNCSEL_XIP		0

#define PADS_VOLTAGE_SELECT_3V3		0
#define PADS_VOLTAGE_SELECT_1V8		1
#define PADS_GPIO_OD				(1 << 7)
#define PADS_GPIO_IE				(1 << 6)
#define PADS_GPIO_DRIVE_2mA			(0 << 4)
#define PADS_GPIO_DRIVE_4mA			(1 << 4)
#define PADS_GPIO_DRIVE_8mA			(2 << 4)
#define PADS_GPIO_DRIVE_12mA		(3 << 4)
#define PADS_GPIO_PUE				(1 << 3)
#define PADS_GPIO_PDE				(1 << 2)
#define PADS_GPIO_SCHMITT			(1 << 1)
#define PADS_GPIO_SLEWFAST			(1 << 0)


void gpio_init(uint32 gpio, uint32 func) {
	io->gpio[gpio].ctrl = func;
	pads->gpio[gpio] = PADS_GPIO_IE | PADS_GPIO_OD;
	sio->gpio_oe_clr = 1 << gpio;
}

void gpio_dir(uint32 gpio, uint32 out) {
	if (out) {
		sio->gpio_oe_set = 1 << gpio;
		pads->gpio[gpio] = 0;
	} else {
		sio->gpio_oe_clr = 1 << gpio;
		pads->gpio[gpio] = PADS_GPIO_IE | PADS_GPIO_OD;
	}
}

void gpio_set(uint32 gpio, uint32 value) {
	if (value)
		sio->gpio_out_set = 1 << gpio;
	else
		sio->gpio_out_clr = 1 << gpio;
}

uint32 gpio_get(uint32 gpio) {
	return sio->gpio_in & (1 << gpio);
}

void gpio_deinit(uint32 gpio) {
	pads->gpio[gpio] = PADS_GPIO_IE | PADS_GPIO_OD;
	sio->gpio_oe_clr = 1 << gpio;
}

void gpio_pullup(uint32 gpio, uint32 enable) {
	uint32 reg = pads->gpio[gpio];

	if (enable) {
		reg |= PADS_GPIO_PUE;
	} else {
		reg &= (~PADS_GPIO_PUE);
	}
	pads->gpio[gpio] = reg;
}

void gpio_pulldown(uint32 gpio, uint32 enable) {
	uint32 reg = pads->gpio[gpio];

	if (enable) {
		reg |= PADS_GPIO_PDE;
	} else {
		reg &= (~PADS_GPIO_PDE);
	}
	pads->gpio[gpio] = reg;
}

void gpio_int_set(uint32 gpio, uint32 enable, uint32 event) {
	gpio_int_ack(gpio);
	if (enable) {
		io->proc0_inte[gpio / 8] |= event << (4 *(gpio % 8));
	} else {
		io->proc0_inte[gpio / 8] &= ~(event << (4 *(gpio % 8)));
	}
}

void gpio_int_ack(uint32 gpio) {
	io->intr[gpio / 8] = 0xC << (4 * (gpio % 8));
}
