
#include <lib.h>
#include <uart.h>
#include <stdarg.h>


 

/*
int vssprintf(char *str, char **arg) {
	char *format, *p;
	char buffer[20];
	int v, t, i = 0;

	if (*arg)
		format = *arg++;
	else
		return 0;

	while (*format) {
		v = *((int*)arg);

		if (*format == '%') {
			++format;

			switch (*format) {
			case 'd':
				dec2str(buffer, v);
				t = strlen(buffer);
				memcpy(str + i, buffer, t);
				i += t;
				break;
			case 'u':
				if (v < 0)
					v = -v;

				dec2str(buffer, v);
				t = strlen(buffer);
				memcpy(str + i, buffer, t);
				i += t;
				break;
			case 'x':
				hex2str(buffer, v);
				t = strlen(buffer);
				memcpy(str + i, buffer, t);
				i += t;
				break;
			case 'c':
				str[i++] = v;
				break;
			case 's':
				p = *arg;
				if (!p) {
					str[i] = 0;
					strcat(str, "null");
					i += strlen("null");
				} else {
					t = strlen(p);
					memcpy(str + i, p, t);
					i += t;
				}
			default:
				break;
			}
			(int*)arg++;
		} else {
			if (*format == '\n') {
				str[i++] = '\r';
			}
			str[i++] = *format;
		}
		++format;
	}

	str[i++] = 0;
	return i;
}

int sprintf(char *buff, char *format, ...) {
	return vssprintf(buff, &format);
}

int printf(char *format, ...) {
	int len;
	char buff[128];
	len = vssprintf(buff, &format);
	uart_write(0, buff, len);
	return len;
}
*/
 

/*----------------------------------------------*/
/* Formatted string output                      */
/*----------------------------------------------*/
/*  xprintf("%d", 1234);			"1234"
    xprintf("%6d,%3d%%", -200, 5);	"  -200,  5%"
    xprintf("%-6u", 100);			"100   "
    xprintf("%ld", 12345678);		"12345678"
    xprintf("%llu", 0x100000000);	"4294967296"	<_USE_LONGLONG>
    xprintf("%04x", 0xA3);			"00a3"
    xprintf("%08lX", 0x123ABC);		"00123ABC"
    xprintf("%016b", 0x550F);		"0101010100001111"
    xprintf("%*d", 6, 100);			"   100"
    xprintf("%s", "String");		"String"
    xprintf("%-5s", "abc");			"abc  "
    xprintf("%5s", "abc");			"  abc"
    xprintf("%c", 'a');				"a"
    xprintf("%f", 10.0);            <xprintf lacks floating point support. Use regular printf.>
*/

static char *outptr;


void xputc (char c)
{
	//if (_CR_CRLF && c == '\n') xputc('\r');		/* CR -> CRLF */

 
	uart_write(0, &c, 1);
	
}
 


void xputs (					/* Put a string to the default device */
	const char* str				/* Pointer to the string */
)
{
	while (*str) {
		xputc(*str++);
	}
}

static
void xvprintf (
	const char*	fmt,	/* Pointer to the format string */
	va_list arp			/* Pointer to arguments */
)
{
	unsigned int r, i, j, w, f;
	int n;
	char s[32], c, d, *p;
#if _USE_LONGLONG
	_LONGLONG_t v;
	unsigned _LONGLONG_t vs;
#else
	long v;
	unsigned long vs;
#endif


	for (;;) {
		c = *fmt++;					/* Get a format character */
		if (!c) break;				/* End of format? */
		if (c != '%') {				/* Pass it through if not a % sequense */
			xputc(c); continue;
		}
		f = w = 0;					/* Clear parms */
		c = *fmt++;					/* Get first char of the sequense */
		if (c == '0') {				/* Flag: left '0' padded */
			f = 1; c = *fmt++;
		} else {
			if (c == '-') {			/* Flag: left justified */
				f = 2; c = *fmt++;
			}
		}
		if (c == '*') {				/* Minimum width from an argument */
			n = va_arg(arp, int);
			if (n < 0) {			/* Flag: left justified */
				n = 0 - n;
				f = 2;
			}
			w = n; c = *fmt++;
		} else {
			while (c >= '0' && c <= '9') {	/* Minimum width */
				w = w * 10 + c - '0';
				c = *fmt++;
			}
		}
		if (c == 'l' || c == 'L') {	/* Prefix: Size is long */
			f |= 4; c = *fmt++;
#if _USE_LONGLONG
			if (c == 'l' || c == 'L') {	/* Prefix: Size is long long */
				f |= 8; c = *fmt++;
			}
#endif
		}
		if (!c) break;				/* End of format? */
		d = c;
		if (d >= 'a') d -= 0x20;
		switch (d) {				/* Type is... */
		case 'S' :					/* String */
			p = va_arg(arp, char*);
			for (j = 0; p[j]; j++) ;
			while (!(f & 2) && j++ < w) xputc(' ');
			xputs(p);
			while (j++ < w) xputc(' ');
			continue;
#if XPRINTF_USE_FLOAT
		case 'F' :					/* Float */
			{
				char a[48];
				//ftoa(a,48,precision?(precision-'0'):2,(double)va_arg( args, double ));
				ftoa((double)va_arg( arp, double ),a,48,3);
				p = a;
				while(*p)
					xputc(*p++);
				continue;
			}
#endif
		case 'C' :					/* Character */
			xputc((char)va_arg(arp, int)); continue;
		case 'B' :					/* Binary */
			r = 2; break;
		case 'O' :					/* Octal */
			r = 8; break;
		case 'D' :					/* Signed decimal */
		case 'U' :					/* Unsigned decimal */
			r = 10; break;
		case 'X' :					/* Hexdecimal */
			r = 16; break;
		default:					/* Unknown type (passthrough) */
			xputc(c); continue;
		}

#if XPRINTF_USE_FLOAT
#endif

		/* Get an argument and put it in numeral */
#if _USE_LONGLONG
		if (f & 8) {	/* long long argument? */
			v = va_arg(arp, _LONGLONG_t);
		} else {
			if (f & 4) {	/* long argument? */
				v = (d == 'D') ? (long)va_arg(arp, long) : (long)va_arg(arp, unsigned long);
			} else {		/* int/short/char argument */
				v = (d == 'D') ? (long)va_arg(arp, int) : (long)va_arg(arp, unsigned int);
			}
		}
#else
		if (f & 4) {	/* long argument? */
			v = va_arg(arp, long);
		} else {		/* int/short/char argument */
			v = (d == 'D') ? (long)va_arg(arp, int) : (long)va_arg(arp, unsigned int);
		}
#endif
		if (d == 'D' && v < 0) {	/* Negative value? */
			v = 0 - v; f |= 16;
		}
		i = 0; vs = v;
		do {
			d = (char)(vs % r); vs /= r;
			if (d > 9) d += (c == 'x') ? 0x27 : 0x07;
			s[i++] = d + '0';
		} while (vs != 0 && i < sizeof s);
		if (f & 16) s[i++] = '-';
		j = i; d = (f & 1) ? '0' : ' ';
		while (!(f & 2) && j++ < w) xputc(d);
		do xputc(s[--i]); while (i != 0);
		while (j++ < w) xputc(' ');
	}
}

void printf (			/* Put a formatted string to the default device */
	const char*	fmt,	/* Pointer to the format string */
	...					/* Optional arguments */
)
{
	va_list arp;


	va_start(arp, fmt);
	xvprintf(fmt, arp);
	va_end(arp);
}

void sprintf (			/* Put a formatted string to the memory */
	char* buff,			/* Pointer to the output buffer */
	const char*	fmt,	/* Pointer to the format string */
	...					/* Optional arguments */
)
{
	va_list arp;


	outptr = buff;		/* Switch destination for memory */

	va_start(arp, fmt);
	xvprintf(fmt, arp);
	va_end(arp);

	*outptr = 0;		/* Terminate output string with a \0 */
	outptr = 0;			/* Switch destination for device */
}

/*----------------------------------------------*/
/* Dump a line of binary dump                   */
/*----------------------------------------------*/

void put_dump (
	const void* buff,		/* Pointer to the array to be dumped */
	unsigned long addr,		/* Heading address value */
	int len,				/* Number of items to be dumped */
	int width				/* Size of the items (DF_CHAR, DF_SHORT, DF_LONG) */
)
{
	int i;
	const unsigned char *bp;
	const unsigned short *sp;
	const unsigned long *lp;


	printf("%08lX ", addr);		/* address */

	switch (width) {
	case DW_CHAR:
		bp = buff;
		for (i = 0; i < len; i++)		/* Hexdecimal dump */
			printf(" %02X", bp[i]);
		xputc(' ');
		for (i = 0; i < len; i++)		/* ASCII dump */
			xputc((unsigned char)((bp[i] >= ' ' && bp[i] <= '~') ? bp[i] : '.'));
		break;
	case DW_SHORT:
		sp = buff;
		do								/* Hexdecimal dump */
			printf(" %04X", *sp++);
		while (--len);
		break;
	case DW_LONG:
		lp = buff;
		do								/* Hexdecimal dump */
			printf(" %08LX", *lp++);
		while (--len);
		break;
	}

	xputc('\n');
}