//#include "rp2040.h"
extern int main(void);
 
extern unsigned int _etext;
extern unsigned int _data;
extern unsigned int _edata;
extern unsigned int _bss;
extern unsigned int _ebss;
void startup(void);


//-----------------------------------------------------------------------------
void startup(void)
{
  unsigned int *src, *dst;

  src = &_etext;
  dst = &_data;
  while (dst < &_edata)
    *dst++ = *src++;

  dst = &_bss;
  while (dst < &_ebss)
    *dst++ = 0;

    main();

  while (1);
}

//-----------------------------------------------------------------------------
 
 
