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
       *(.rodata .rodata.*)
       *(.bss .bss.*)
    }
}