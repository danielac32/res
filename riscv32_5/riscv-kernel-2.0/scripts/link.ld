OUTPUT_ARCH("riscv")

MEMORY{
    vector    : ORIGIN = 0x0000, LENGTH = 1k
    syscall   : ORIGIN = 0x0418, LENGTH = 1K
     
    /*flash(rx)   : ORIGIN = 0x0900, LENGTH = 64K
    ram(xrw)  : ORIGIN = 0xfa00, LENGTH = 230K*/

    flash(rx)   : ORIGIN = 0x0900, LENGTH = 100K
    ram(xrw)  : ORIGIN = 0x186a0, LENGTH = 230K
}


SECTIONS{
  
    .interrupt : {
      KEEP(*(.interrupt))
    }>vector
     
    .svc : {
      *(.svc)
    }>syscall

    .text : {
        *(.text)           /* .text sections (code) */
        *(.text*)          /* .text* sections (code) */
        *(.rodata)         /* .rodata sections (constants, strings, etc.) */
        *(.rodata*)        /* .rodata* sections (constants, strings, etc.) */
        *(.srodata)        /* .rodata sections (constants, strings, etc.) */
        *(.srodata*)       /* .rodata* sections (constants, strings, etc.) */
        . = ALIGN(4);
    }>flash 

    .data :{
        *(.data)           /* .data sections */
        *(.data*)          /* .data* sections */
        *(.sdata)           /* .sdata sections */
        *(.sdata*)          /* .sdata* sections */
        *(.bss)
        *(.bss*)
        *(.sbss)
        *(.sbss*)
        *(COMMON)
        . = ALIGN(4);
    }>ram  
 
}

 