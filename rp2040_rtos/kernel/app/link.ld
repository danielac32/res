

ENTRY(main); 

SECTIONS {
     
    .text : {
        . = ALIGN(4);

        _stext = .;

        *(.text)
        *(.text.*)
        *(.rodata)
        *(.rodata.*)

        _etext = .;
        _sidata = _etext;
    }

    .data : AT(_sidata) {
        . = ALIGN(4);

        _sdata = .;

        *(.data)
        *(.data.*)
        *(.RAMtext)

        _edata = .;
    }

    .bss : {
        . = ALIGN(4);

        _sbss = .;

        *(.bss)
        *(.bss.*)
        *(COMMON)

        _ebss = .;
    }
 
}
