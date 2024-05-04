MEMORY
{
  FLASH (rx) : ORIGIN = 0x00000000, LENGTH = 100k
  SRAM(xrw): ORIGIN = 0x20000000, LENGTH = 200k
}

sram_top = ORIGIN(SRAM) + LENGTH(SRAM);
ENTRY(boot_entry)

SECTIONS
{
  .text :
  {
    KEEP(*(.irq_vectors))
    *(.text*)
    *(.rodata*)
  } > FLASH

  
   
  _etext = .;
    
  .data :
  {
    _data = .;
    *(vtable)
    *(.data*)
    _edata = .;
  } > SRAM AT> FLASH

  /* zero initialized data */
  .bss :
  {
    _bss = .;
    *(.bss*)
    *(COMMON)
    _ebss = .;
  } > SRAM
  
  end = .;

  /* For GDB compatibility we decrease the top with 16 bytes */
  stack_entry = sram_top - 16;
}
