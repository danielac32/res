OUTPUT_ARCH("riscv")

MEMORY{
    vector    : ORIGIN = 0x0000, LENGTH = 1k
    syscall   : ORIGIN = 0x0418, LENGTH = 1K
    mem (xrw) : ORIGIN = 0x0900 , LENGTH = 80K
}
ENTRY(start)


SECTIONS{

    .interrupt : {
      *(.interrupt)
    }>vector
     
    .svc : {
      *(.svc)
    }>syscall

    .text : {
       __text_start = .;
       *(.text)
       __data_start = .;

       *(.data .data.*)
       *(.sdata .sdata.*)
       *(.rodata .rodata.*)
       *(.bss .bss.*)
       *(.sbss .sbss.*) 

       __data_end = .;
    }>mem   
}
_text_start = 0x0900;
_start_data = __data_start ;
_end_data = __data_end ;
