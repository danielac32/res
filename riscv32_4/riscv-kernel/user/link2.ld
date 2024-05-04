OUTPUT_ARCH("riscv")

MEMORY
{
 
}
ENTRY(start)


SECTIONS{

    .text : {
      /* *(.text)
       *(.data)
       *(.sdata) 
       *(.rodata)
       *(.bss)
       *(.sbss)*/
       
       *(.text)
       *(.text*)
       *(.data .data.*)
       *(.sdata .sdata.*)
       *(.rodata .rodata.*)
       *(.bss .bss.*)
       *(.sbss .sbss.*)
    }
}