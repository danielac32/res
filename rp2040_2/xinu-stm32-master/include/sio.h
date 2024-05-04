 

uint32 cpuid();

uint32 gpio_in(uint32 gpio);
uint32 gpio_hi_in(uint32 gpio);
void gpio_out(uint32 gpio, uint32 value);
void gpio_oe(uint32 gpio, uint32 oe);
void gpio_hi_out(uint32 gpio, uint32 value);
void gpio_hi_oe(uint32 gpio, uint32 oe);

int fifo_write(uint32 value);
int fifo_read(uint32 *value);

uint32 spinlock_get(uint32 n);
uint32 spinlock_lock(uint32 n);
void spinlock_unlock(uint32 n);
 