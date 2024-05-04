OUTPUT_ARCH("riscv")

ENTRY(entry)
MEMORY
{
  sram (xrw)  : ORIGIN = 0x0000, LENGTH = 500k
}
_system_stack_size = 4096;
SECTIONS
{
   .text : { 
     *(.text) 
     *(.rodata)
     }>sram
     
   /* data */

    /* . += 100K; */
   .data : { 
      *(.data) 
      *(.sdata)
      *(.bss)
    }>sram
     
    _gp = .;
    .stack : 
    {
        . = . + _system_stack_size;
        . = ALIGN(4);
        _estack = .;
    } >sram
}

