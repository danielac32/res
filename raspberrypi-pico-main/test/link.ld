

ENTRY(entry)

MEMORY
{
  FLASH (rx) : ORIGIN = 0x10000000, LENGTH = 100K
  RAM (rwx) :  ORIGIN = 0x20000000, LENGTH = 64K
}

SECTIONS
{
  .text :
  {
    *(.text)
    *(.text.*)
    *(.rodata)
    _sromdev = .;
    _eromdev = .; 
    _sidata = .;
  } >FLASH

  .data : AT(_sidata)
  {
    _sdata = .;
    *(.data)
    *(.data*)
    _edata = .;
  } >RAM

  .bss :
  {
    _sbss = .;
    *(.bss)
    _ebss = .;
  } >RAM

  _estack = ORIGIN(RAM) + LENGTH(RAM);
}
