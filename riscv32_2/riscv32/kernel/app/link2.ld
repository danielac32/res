OUTPUT_ARCH("riscv")

MEMORY
{
/* rom  (rx):  ORIGIN = 0x765608 , LENGTH = 20K
 ram  (rwx): ORIGIN = 0x76a428 , LENGTH = 5K*/
}
ENTRY(start)


SECTIONS{

    .text : {
       *(.text)
       *(.data)
       *(.sdata) 
       *(.rodata)
       *(.bss)
       *(.sbss)
    }
}