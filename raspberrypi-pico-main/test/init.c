
#include <stdint.h>
// Prototype the required startup functions
extern int main(void);

static inline void _exit(int exit_code)
{
    if (exit_code == 0)
        asm volatile("bkpt 0");
    else
        asm volatile("bkpt 1");
}

extern uint32_t _sidata;
/* start address for the .data section. defined in linker script */
extern uint32_t _sdata;
/* end address for the .data section. defined in linker script */
extern uint32_t _edata;
/* start address for the .bss section. defined in linker script */
extern uint32_t _sbss;
/* end address for the .bss section. defined in linker script */
extern uint32_t _ebss;
/* end address for the stack. defined in linker script */
extern uint32_t _estack;

// The entry point of the application, prepare segments,
// initialize the cpu and execute main()
void boot_entry(void)
{
 
  uint32_t *idata_begin = &_sidata;
  uint32_t *data_begin = &_sdata;
  uint32_t *data_end = &_edata;
  while (data_begin < data_end) *data_begin++ = *idata_begin++;

  /* Zero fill the bss segment. */
  uint32_t *bss_begin = &_sbss;
  uint32_t *bss_end = &_ebss;
  while (bss_begin < bss_end) *bss_begin++ = 0;

  // Execute the code at the program entry point
  int res = main();
  _exit(res);
}
