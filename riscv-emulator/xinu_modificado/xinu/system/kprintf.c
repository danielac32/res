/* kprintf.c -  kputc, kgetc, kprintf */
/* Kprintf uses a direct method to write to serial
 * Used for debugging purposes. Should in normal cases
 * use printf that does a syscall
 * */

#include <xinu.h>
#include <stdarg.h>

 
char digits[] = "0123456789ABCDEF";
 
void
printint(int xx, int base, int sgn)
{
  char buf[16];
  int i, neg;
  uint x;

  neg = 0;
  if(sgn && xx < 0){
    neg = 1;
    x = -xx;
  } else {
    x = xx;
  }

  i = 0;
  do{
    buf[i++] = digits[x % base];
  }while((x /= base) != 0);
  if(neg)
    buf[i++] = '-';

  while(--i >= 0)
    putc(CONSOLE, buf[i]);
}

void
printptr(uint64 x) {
  int i;
  putc(CONSOLE, '0');
  putc(CONSOLE, 'x');
  for (i = 0; i < (sizeof(uint64) * 2); i++, x <<= 4)
    putc(CONSOLE, digits[x >> (sizeof(uint64) * 8 - 4)]);
}

// Print to the given CONSOLE. Only understands %d, %x, %p, %s.
void
vprintf(const char *fmt, va_list ap)
{
  char *s;
  int c, i, state;

  state = 0;
  for(i = 0; fmt[i]; i++){
    c = fmt[i] & 0xff;
    if(state == 0){
      if(c == '%'){
        state = '%';
      } else {
        putc(CONSOLE, c);
      }
    } else if(state == '%'){
      if(c == 'd' || c == 'i'){
        printint( va_arg(ap, int), 10, 1);
      } else if(c == 'l') {
        printint(va_arg(ap, uint64), 10, 0);
      } else if(c == 'x') {
        printint( va_arg(ap, uint32), 16, 0);
      } else if(c == 'p') {
        printptr( va_arg(ap, uint64));
      } else if(c == 's'){
        s = va_arg(ap, char*);
        if(s == 0)
          s = "(null)";
        while(*s != 0){
          putc(CONSOLE, *s);
          s++;
        }
      } else if(c == 'c'){
        putc(CONSOLE, va_arg(ap, uint));
      } else if(c == '%'){
        putc(CONSOLE, c);
      } else {
        // Unknown % sequence.  Print it to draw attention.
        putc(CONSOLE, '%');
        putc(CONSOLE, c);
      }
      state = 0;
    }
  }
}

void
fprintf(const char *fmt, ...)
{
  va_list ap;

  va_start(ap, fmt);
  vprintf( fmt, ap);
}

void
kprintf(const char *fmt, ...)
{
  va_list ap;

  va_start(ap, fmt);
  vprintf( fmt, ap);
}
