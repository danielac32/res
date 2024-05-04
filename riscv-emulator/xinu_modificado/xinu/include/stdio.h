/* stdio.h - definintions and constants for standard I/O functions */

/* avr specific values. Original saved under orig/ folder */
 
#ifndef _STDIO_H_
#define _STDIO_H_
#include <stdarg.h>
/* Prototypes for formatted input functions */

/*	long	_doscan(char *,long *, long (*)(void),
			long (*)(char), long, long);
	long	sscanf(char *, char *, long);
	long	fscanf(long, char *, long);
#define	scanf(fmt, args)	fscanf(CONSOLE, fmt, args)
*/

/* Definintion of standard input/ouput/error used with shell commands */

#define	stdin	((proctab[currpid]).prdesc[0])
#define	stdout	((proctab[currpid]).prdesc[1])
#define	stderr	((proctab[currpid]).prdesc[2])


/* Prototypes for formatted output functions */

/*	int	fprintf(int, char *, ...);
	int	printf(const char *, ...);
	long	sprintf(char *, char *, ...);
*/

/* Prototypes for character input and output functions */

/*long	fgetc(int);
char	*fgets(char *, long, long);
long	fputc(long, long);
long	fputs(char *, long);
int	putchar(int c);
int	getchar(void);*/

 
void
printint(int xx, int base, int sgn);

void
printptr(uint64 x);

// Print to the given CONSOLE. Only understands %d, %x, %p, %s.
void
vprintf(const char *fmt, va_list ap);
void
fprintf(const char *fmt, ...);

void
kprintf(const char *fmt, ...);


/*
syscall	kprintf(char *fmt, ...);
syscall	kputc(byte);
syscall	kgetc(void);

*/
#endif