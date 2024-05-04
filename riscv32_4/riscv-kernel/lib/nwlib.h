
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdarg.h>
#include <stdio.h>
#include <ctype.h>


#ifndef NWLIB_H
#define NWLIB_H
//char b[100];
//char mem3[30];
char buff[100];

/*******************************************************/
#define base_io 0xFA000

#define mode (*(volatile char *)0xFA001)
//#define reg (*(volatile char *)0xFA002)
//#define timer (*(volatile uint32_t *)0xFA003)
//#define enable_timer (*(volatile char *)0xFA004)

//#define keyAvailable (*(volatile uint8_t *)0xFA005)
//#define keyRead (*(volatile uint8_t *)0xFA006)
//#define led (*(volatile uint8_t *)0xFA007)

#define workregister1 (*(volatile uint32_t *)0xFA008)
#define workregister2 (*(volatile uint32_t *)0xFA009)
#define workregister3 (*(volatile uint32_t *)0xFA00a)
#define workregister4 (*(volatile uint32_t *)0xFA00b)
#define workregister5 (*(volatile uint32_t *)0xFA00c)
#define workregister6 (*(volatile uint32_t *)0xFA00d)
#define workregister7 (*(volatile uint32_t *)0xFA00e)


#define SPACE 0x20
#define ENTER 0xd
#define UP 0x48

#define PRINTSTR 4
#define PRINTCHAR 11
#define SERIALAVAILABLE 12
#define SERIALREAD 13
#define YIELD 5
#define EXITTASK 200
#define EXITCPU 10
#define OPENFILE 14
#define READFILE 15
#define FILESIZE 16
#define CLOSEFILE 17
#define TIMER 18
#define FPRINT 19
#define ALLOC 21
#define FREE 22
#define READFILEBUFF 24


void* kmalloc(uint32_t size);
void mfree(void *p);
void print(char *str);
void fprint(float f);
void syscall(uint32_t num,uint32_t arg0,uint32_t arg1,uint32_t arg2);
int SerialAvailable();
void exit_cpu();
uint8_t SerialRead();
void putc_(char ch);
void yield(uint32_t point);
void kill_task(int pid);
void exit_();
void closefile();
int openfile(char *str);
char readfile();
void readbuff(void *p,uint32_t si);
uint32_t filesize();
uint32_t get_time();
void reverse(char *str,int len);
int intToStr(int x,char *str,int d);
 char hexdigit(long value, int digit, bool uppercase);
 void itoh(char * b, unsigned v);
 void itoa(char * b, unsigned n);
/*******************************************************/
static  unsigned long dv[]  = {
//  4294967296      // 32 bit unsigned max
    1000000000,     // +0
     100000000,     // +1
      10000000,     // +2
       1000000,     // +3
        100000,     // +4
//       65535      // 16 bit unsigned max     
         10000,     // +5
          1000,     // +6
           100,     // +7
            10,     // +8
             1,     // +9
};
#define PUTC(c) putc_(c)
void xtoa(unsigned long x,  unsigned long *dp);
void puth(unsigned n);
int myprintf(const char *format, ...);
int mysprintf( char *buff,const char *format, ...);
void print_hex(unsigned int val,int digits);

#endif