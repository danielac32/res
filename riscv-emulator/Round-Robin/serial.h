


#ifndef SERIAL_H
#define SERIAL_H

 

char spbuffer[50];
char buff[50];

 void putc_( char ch);
 void print( char *str);
 void reverse(char *str,int len);
 int intToStr(int x,char str[],int d);
 float _pow_(float base,float ex);
 void ftoa(float n,char *res,int afterpoint);
 void print_dec(unsigned long val);
 
 
static void xtoa(unsigned long x, const unsigned long *dp);
static void puth(unsigned n);
void print_hex(unsigned int val,int digits);
int myprintf(const char *format, ...);
int mysprintf(char *buff,const char *format, ...);
void floatprint(float f);
#endif