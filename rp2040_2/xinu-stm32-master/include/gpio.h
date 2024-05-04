/* gpio.h - definintions for the gpio device */

/*
 * Control and Status Register (CSR) definintions for the GPIO.
 * The code maps the structure structure directly onto the base address
 * CSR address for the device.
 */

/* Number of GPIO devices in the hardware */
#define NGPIO	4

#define GPIO_FUNC_XIP			0
#define GPIO_FUNC_SPI			1
#define GPIO_FUNC_UART			2
#define GPIO_FUNC_I2C			3
#define GPIO_FUNC_PWM			4
#define GPIO_FUNC_SIO			5
#define GPIO_FUNC_PIO0			6
#define GPIO_FUNC_PIO1			7
#define GPIO_FUNC_CLK			8
#define GPIO_FUNC_USB			9
#define GPIO_FUNC_NULL			0xF

#define GPIO_INT_LEVEL_LOW		1
#define GPIO_INT_LEVEL_HIGH		2
#define GPIO_INT_EDGE_FALL		4
#define GPIO_INT_EDGE_RISE		8


void gpio_init(uint32 gpio, uint32 func);
void gpio_dir(uint32 gpio, uint32 out);
void gpio_set(uint32 gpio, uint32 value);
uint32 gpio_get(uint32 gpio);
void gpio_deinit(uint32 gpio);

void gpio_pullup(uint32 gpio, uint32 enable);
void gpio_pulldown(uint32 gpio, uint32 enable);

void gpio_int_set(uint32 gpio, uint32 enable, uint32 event);
void gpio_int_ack(uint32 gpio);