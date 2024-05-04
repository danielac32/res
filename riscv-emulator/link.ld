OUTPUT_ARCH("riscv")

ENTRY(entry)
MEMORY
{
  sram (xrw)  : ORIGIN = 0x0000, LENGTH = 250K
}

SECTIONS
{
   .text : { 
     *(.text) 
     *(.rodata)
     }>sram
     
   /* data */

   /* . += 0x1E848; */
   .data : { 
      *(.data) 
      *(.sdata)
      *(.bss)
    }>sram
}
