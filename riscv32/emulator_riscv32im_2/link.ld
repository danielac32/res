OUTPUT_ARCH("riscv")

MEMORY
{
 rom  (rwx):  ORIGIN = 0x0000 , LENGTH = 50K
 vector : ORIGIN = 0xBF68     ,LENGTH = 1k
 ram  (rwx): ORIGIN = 0xC350  , LENGTH = 12K
}
ENTRY(start)


SECTIONS{

    .text : {
       *(.text)
    }>rom
    
    .interrupt : {
      *(.interrupt)
    }>vector

    .data :{
       *(.data)
       *(.sdata) 
       *(.rodata)
       *(.bss)
       *(.sbss)
    }>ram
}