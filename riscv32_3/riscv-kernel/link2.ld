OUTPUT_ARCH("riscv")

MEMORY{
    vector : ORIGIN = 0x0000,LENGTH = 1k
    mem (xrw) : ORIGIN = 0x420 , LENGTH = 40K
}
ENTRY(start)


SECTIONS{

    .interrupt : {
      *(.interrupt)
    }>vector 

    .text : {
     
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

PROVIDE ( _start_data = __data_start );
PROVIDE ( _end_data = __data_end );
