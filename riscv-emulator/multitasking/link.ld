OUTPUT_ARCH("riscv")

ENTRY(reset)

MEMORY
{
  vector (rx) : ORIGIN = 0x0000, LENGTH = 1K 
  sram (xrw)  : ORIGIN = 0x0400, LENGTH = 250K
}
_system_stack_size = 4096;
SECTIONS
{   
   
   .interrupt : {
      *(.interrupt)
    }>vector 

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

