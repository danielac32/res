OUTPUT_ARCH("riscv")

MEMORY
{

 vector : ORIGIN = 0x0000,LENGTH = 1k
 rom  :  ORIGIN = 0x0000420 , LENGTH = 30K
 ram  (xrw): ORIGIN = 0x7530 , LENGTH = 80K
 /*stack (xrw): ORIGIN = 0x1D4C0 , LENGTH = 10K*/

}
ENTRY(start)


SECTIONS{
    .interrupt : {
      *(.interrupt)
    }>vector 

    .text : {
       *(.text)
       *(.text*)
    }>rom

    .data : {
       *(.data .data.*)
       *(.sdata .sdata.*)
       *(.rodata .rodata.*)
       *(.bss .bss.*)
       *(.sbss .sbss.*)

    } > ram

    /*.stack_point : {
      *(.stack_point)
    }>stack*/
    
}