OUTPUT_ARCH("riscv")
 
MEMORY
{
 
}
ENTRY(start)


SECTIONS{

    .text 0x010074: {
       *(.text*)
       *(.rodata*)
       *(.data*)
       *(.got*)
    }

    .bss : {
       *(.bss*)
    }
}