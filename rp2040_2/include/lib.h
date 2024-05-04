#include "rp2040.h"
#ifndef LIB_H_
#define LIB_H_

#define bool   _Bool
#define true   1
#define false  0
typedef int           intptr_t;
typedef unsigned int  uintptr_t;
typedef int           ssize_t;
typedef unsigned int  size_t;

#define NULL  ((void*)0)


typedef signed char   int8_t;
typedef short int     int16_t;
typedef int           int32_t;
typedef long long int int64_t;

/* --- unsigned --------------------------------------------------------------------------------- */
typedef unsigned char          uint8_t;
typedef unsigned short int     uint16_t;
typedef unsigned int           uint32_t;
typedef unsigned long long int uint64_t;

#define intern    static // for use in file scope
#define common    static // for use in function scope
#define readonly  const  // it's not constant, it's just read-only!

#define byte  uint8_t // to be used with signed/unsigned (that's why it isn't a typedef)

static inline int min(const int a, const int b) { return (a < b) ? a : b; }
static inline int max(const int a, const int b) { return (a < b) ? b : a; }


#define BIT_VAL(n) 			(1 << (n))
#define BIT_GET(x, n) 		(((x) >> (n)) & 1)
#define BIT_SET(x, n)		((x) | (1 << (n)))
#define BIT_CLR(x, n)		((x) & ( ~(1 << (n))))
#define BIT_INV(x, n)		((x) ^ (1 << (n)))
#define BIT_WR(x, n, v)		(((x) & ( ~(1 << (n)))) | (((v) & 1) << (n)))

#define DEC2ASCII(dec)		((dec) + 0x30)
#define ASCII2DEC(asc)		((asc) - 0x30)
#define HEX2ASCII(hex)  	(((hex) > 0x09) ? ((hex) + 0x37): ((hex) + 0x30))
#define ASCII2HEX(asc)  	(((asc) > 0x39) ? ((asc) - 0x37): ((asc) - 0x30))

#define GET_BYTE0(b)		((b >> 0) & 0xFF)
#define GET_BYTE1(b)		((b >> 8) & 0xFF)
#define GET_BYTE2(b)		((b >> 16) & 0xFF)
#define GET_BYTE3(b)		((b >> 24) & 0xFF)

#define GET_MAX(a, b)		(((a) > (b))? (a): (b))
#define GET_MIN(a, b)		(((a) < (b))? (a): (b))
#define GET_ABS(a)			(((a) < 0)? -(a): (a))

typedef unsigned int uint;

void *memset(void *s, int c, int n);
void *memcpy(void *dest, void *src, int n);
void *memchr(const void *str, int c, uint n);
int memcmp(const void *str1, const void *str2, uint n);
int strlen(const char *s);
int strcmp(const char *s1, const char *s2);
int strncmp(const char *s1, const char *s2, uint n);
char *strcpy(char *dest, const char *src);
char *strncpy(char *dest, const char *src, int n);
char *strcat(char *dest, const char *src);
char *strncat(char *dest, const char *src, uint n);
char *strchr(char *str, int c);
char *strstr(char *str, const char *find);

/*
int vssprintf(char *str, char **arg);
int sprintf(char *buff, char *format, ...);
int printf(char *format, ...);
*/

//#define XPRINTF_USE_FLOAT      1   /* 1: Use floating point */
#define XPRINTF_USE_XFUNC_OUT      1   /* 1: Use floating point */
#define	_CR_CRLF		1	    /* 1: Convert \n ==> \r\n in the output char */
#define	_USE_LONGLONG	1	    /* 1: Enable long long integer in type "ll". */
#define	_LONGLONG_t		long long	/* Platform dependent long long integer type */
#define XPRINTF_USE_XFUNC_IN	1
#define	_LINE_ECHO		1

//#define xdev_out(func) xfunc_out = (void(*)(unsigned char))(func)
//extern void (*xfunc_out)(unsigned char);


void xputc (char c);
void xputs (const char* str);
void printf (const char* fmt, ...);
void sprintf (char* buff, const char* fmt, ...);
void put_dump (const void* buff, unsigned long addr, int len, int width);
#define DW_CHAR		sizeof(char)
#define DW_SHORT	sizeof(short)
#define DW_LONG		sizeof(long)


int atoi(char *str);
void hex2str(char *str, int n);
void bin2str(char *str, int n, int b);
void dec2str(char *str, int n);

int isalnum(int c);
int isalpha(int c);
int iscntrl(int c);
int isdigit(int c);
int isgraph(int c);
int islower(int c);
int isprint(int c);
int ispunct(int c);
int isspace(int c);
int isupper(int c);
int isxdigit(int c);
int tolower(int c);
int toupper(int c);


void mm_init(void);
void* malloc(size_t size);
void free(void *ptr);

#define RAND_MAX 2147483646

int abs(int);
long labs(long);
int atoi(char *);
long atol(char *);
void bzero(void *, int);
void qsort(char *, unsigned int, int, int (*fc)(char *, char *));
int rand(void);
void srand(unsigned int);

int __aeabi_idiv(int a, int b);
int __aeabi_idivmod(int a, int b);
extern uint32_t disable();
extern void restore(uint32_t mask);
extern void delay(uint32_t);

#endif