OUTPUT_ARCH("riscv")

MEMORY
{
 rom  :  ORIGIN = 0x00000000 , LENGTH = 512K
 vector : ORIGIN = 0x7a200,LENGTH = 1k
 ram  (rwx): ORIGIN = 0x0007d000 , LENGTH = 512K
 
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
       /*__global_pointer$ = . + 0x000fc710;*/
       *(.data)
       *(.sdata) 
       *(.rodata)
       *(.bss)
       *(.sbss)
       
    }>ram
    
    /*.data :{
      *(.data)
      *(.sdata)
      *(.sbss)
    }>ram
    .rodata :{
       *(.rodata)
    }>ram
    .bss :{
      *(.bss)
    }>ram*/
  
}