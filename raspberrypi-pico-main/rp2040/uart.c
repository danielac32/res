#include "uart.h"
#include "gpio.h"
#include "resets.h"
#include <stdint.h>
struct uart_hw {
	uint dr;
	uint rsr;
	uint unused1[4];
	uint fr;
	uint unuded2;
	uint ilpr;
	uint ibrd;
	uint fbrd;
	uint lcr_h;
	uint cr;
	uint ifls;
	uint imsc;
	uint ris;
	uint mis;
	uint icr;
};

#define uart ((volatile struct uart_hw*)UART0_BASE)

#define FR_TXFE		(1 << 7)
#define FR_RXFF		(1 << 6)
#define FR_TXFF		(1 << 5)
#define FR_RXFE		(1 << 4)
#define FR_BUSY		(1 << 3)

#define LCR_WLEN_5	(0 << 5)
#define LCR_WLEN_6	(1 << 5)
#define LCR_WLEN_7	(2 << 5)
#define LCR_WLEN_8	(3 << 5)
#define LCR_FEN		(1 << 4)

#define CR_RXE		(1 << 9)
#define CR_TXE		(1 << 8)
#define CR_UARTEN	(1 << 0)


void uart_init() {
	IO_WR(CLOCKS_BASE + 0x48, 0x880); // clk_peri enable
	IO_WR(CLOCKS_BASE + 0x30, 0x2);
	reset_release_wait(RESET_IO_BANK0);
	reset_release_wait(RESET_PADS_BANK0);
	reset_release_wait(RESET_UART0);

	uart->cr = 0;
	uart->ibrd = 6; // 115200
	uart->fbrd = 33;
	uart->lcr_h = LCR_WLEN_8 | LCR_FEN;
	uart->cr = CR_TXE | CR_RXE | CR_UARTEN;

	gpio_init(0, GPIO_FUNC_UART);
	gpio_init(1, GPIO_FUNC_UART);
	gpio_dir(0, 1);
}

uint uart_read(char *buff, uint len) {
	uint i;
	for (i = 0; i < len; ++i) {
		while (uart->fr & FR_RXFE);
		buff[i] = uart->dr & 0xFF;
	}
	return i;
}

uint uart_write(char *buff, uint len) {
	uint i;
	for (i = 0; i < len; ++i) {
		while (uart->fr & FR_TXFF);

		uart->dr = buff[i];
	}
	return i;
}

static void hex2str(char *str, int n) {
	int i, hb;
	for (i = 0; i < 8; ++i) {
		hb = n >> (7 - i) * 4;
		hb &= 0x0F;

		if (hb > 9)
			str[i] = (char)(hb + 'A' - 10);
		else
			str[i] = (char)(hb + '0');
	}
	str[8] = 0;
}

void uart_print(char *s) {
	while (*s) {
		uart_write(s, 1);
		++s;
	}
}

void uart_hex(uint x) {
	char str[8 + 1];
	hex2str(str, x);
	uart_print(str);
}


char digits[] = "0123456789ABCDEF";



static void uart_sendbyte(unsigned char data){
	uart_write(&data,1);
}
void
printint(int xx, int base, int sgn)
{
  char buf[16];
  int i, neg;
  unsigned int x;

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
    uart_sendbyte( buf[i]);
}

/*void
printptr(uint64_t x) {
  int i;
  uart_sendbyte( '0');
  uart_sendbyte( 'x');
  for (i = 0; i < (sizeof(uint64_t) * 2); i++, x <<= 4)
    uart_sendbyte( digits[x >> (sizeof(uint64_t) * 8 - 4)]);
}*/

// Print to the given CONSOLE. Only understands %d, %x, %p, %s.
void
kvprintf(const char *fmt, va_list ap)
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
        uart_sendbyte( c);
      }
    } else if(state == '%'){
      if(c == 'd' || c == 'i'){
        printint( va_arg(ap, int), 10, 1);
      } else if(c == 'l') {
        printint(va_arg(ap, uint32_t), 10, 0);
      } else if(c == 'x') {
        printint( va_arg(ap, uint32_t), 16, 0);
      } else if(c == 'p') {
        //printptr( va_arg(ap, uint64_t));
      } else if(c == 's'){
        s = va_arg(ap, char*);
        if(s == 0)
          s = "(null)";
        while(*s != 0){
          uart_sendbyte( *s);
          s++;
        }
      } else if(c == 'c'){
        uart_sendbyte( va_arg(ap, int));
      } else if(c == '%'){
        uart_sendbyte( c);
      } else {
        // Unknown % sequence.  Print it to draw attention.
        uart_sendbyte( '%');
        uart_sendbyte( c);
      }
      state = 0;
    }
  }
}

void
kfprintf(const char *fmt, ...)
{
  va_list ap;

  va_start(ap, fmt);
  kvprintf( fmt, ap);
}

void
kprintf(const char *fmt, ...)
{
  va_list ap;

  va_start(ap, fmt);
  kvprintf( fmt, ap);
}