#include "applib.h"


void syscall(uint32_t num,uint32_t arg0,uint32_t arg1,uint32_t arg2){
register uint32_t a7 asm("a7")=num;
register uint32_t a0 asm("a0")=arg0;
register uint32_t a1 asm("a1")=arg1;
register uint32_t a2 asm("a2")=arg2;
asm("ecall");
}


void* kmalloc(long size){
workregister1=size;
workregister3=0;
workregister2=ALLOC;
mode=1;
while(mode){
  asm("nop");
  asm("nop");
}
return (void*)workregister3;
}

void mfree(void *p){
workregister1=(uint32_t)p;
workregister2=FREE;
mode=1;
while(mode){
  asm("nop");
  asm("nop");
}
}

uint8_t SerialRead(){
workregister1=0;
workregister2=SERIALREAD;
mode=1;
while(mode){
  asm("nop");
  asm("nop");
}
return workregister1;
}

int SerialAvailable(){
workregister1=0;
workregister2=SERIALAVAILABLE;
mode=1;
while(mode){
  asm("nop");
  asm("nop");
}
return workregister1;
}

void exit_cpu(){
workregister1=0;
workregister2=EXITCPU;
mode=1;
while(mode){
  asm("nop");
  asm("nop");
}
}

void putc_(char ch){
workregister1=(uint32_t)ch;
workregister2=PRINTCHAR;
mode=1;
while(mode){
  asm("nop");
  asm("nop");
}
workregister1=0;
}


void print(char *str){
workregister1=(uint32_t)str;
workregister2=PRINTSTR;
mode=1;
while(mode){
  asm("nop");
  asm("nop");
}
workregister1=0;
}

void fprint(float f){
workregister1=*(uint32_t *)&f;     
workregister2=FPRINT; 
mode=1;                 
while(mode){            
  asm("nop");           
  asm("nop");           
}
}

void yield(uint32_t point){
workregister1=point;
workregister2=YIELD;
mode=1;
while(mode){
  asm("nop");
  asm("nop");
}
}

void kill_task(int pid){
workregister1=pid;
workregister2=EXITTASK;
mode=1;
while(mode){
  asm("nop");
  asm("nop");
}
}

void exit_(){
workregister1=0;
workregister2=EXITTASK;
mode=1;
while(mode){
  asm("nop");
  asm("nop");
}
}

void closefile(){
workregister1=0;
workregister2=CLOSEFILE;
mode=1;
while(mode){
  asm("nop");
  asm("nop");
}
workregister1=0;
}

int openfile(uint32_t str){
workregister1=str;
workregister2=OPENFILE;
mode=1;
while(mode){
  asm("nop");
  asm("nop");
}
return workregister2;
}
char readfile(){
workregister1=0;
workregister3=0;
workregister2=READFILE;
mode=1;
while(mode){
  asm("nop");
  asm("nop");
}
return (char)workregister3;
}
uint32_t filesize(){
workregister1=0;
workregister3=0;
workregister2=FILESIZE;
mode=1;
while(mode){
  asm("nop");
  asm("nop");
}
return workregister3;
}

uint32_t get_time(){
workregister3=0;
workregister2=TIMER;
mode=1;
while(mode){
  asm("nop");
  asm("nop");
}
return workregister3;
}

/***************************************************/
void reverse(char *str,int len){
  int i=0,j=len-1,temp;
  while(i<j){
    temp = str[i];
    str[i]=str[j];
    str[j]=temp;
    i++;
    j--;
  }
}
int intToStr(int x,char *str,int d){
  int i=0;
  while(x){
    str[i++]=(x%10)+'0';
    x=x/10;
  }
  while(i<d){
    str[i++]='0';
  }
  reverse(str,i);
  str[i]='\0';
  return i;
}

 char hexdigit(long value, int digit, bool uppercase)
{
  const char * lowercase_digits = "0123456789abcdef";
  const char * uppercase_digits = "0123456789ABCDEF";

  if(uppercase)
    return uppercase_digits[value >> (digit << 2) & 0xf];
  else
    return lowercase_digits[value >> (digit << 2) & 0xf];
}

 void itoh(char * b, unsigned v)
{
  int count=0;
  for(int i = 7, out = 0; i >= 0; --i){
    b[out++] = hexdigit(v, i, false);
    count++;
  }
   b[count++]='\0';
}

 void itoa(char * b, unsigned n)
{
  int count=0;
  for(int i = 0; i < 10; ++i)
    b[i] = '0';

  for(int i = 9; i >= 0; --i)
  {
    b[i] = '0' + (n % 10);
    n /= 10;
    count++;
  }
  b[count++]='\0';
}

#define PUTC(c) putc_(c)
void xtoa(unsigned long x,  unsigned long *dp)
{
    char c;
    unsigned long d;
    if(x) {
        while(x < *dp) ++dp;
        do {
            d = *dp++;
            c = '0';
            while(x >= d) ++c, x -= d;
            PUTC(c);
        } while(!(d & 1));
    } else
        PUTC('0');
}

 void puth(unsigned n)
{
    static const char hex[16] = { '0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
    PUTC(hex[n & 15]);
}

void print_hex(unsigned int val,int digits){
for (int i = (4*digits)-4; i >=0; i-=4)
{
    PUTC("0123456789ABCDEF"[(val >> i) % 16]);
}
}

int myprintf(const char *format, ...)
{
    char c;
    int i,count;
    long n;
    int fill_zeros;
    unsigned d;
    double val;
    ///char buff[80];
    int hex;
    va_list a;
    int nu=2;
    
    va_start(a, format);
    while((c = *format++)) {
        if(c == '%') {
            fill_zeros = 0;
            parse_fmt_char:
            switch(c = *format++) {
                case 'f':
                    val =va_arg(a, double);
                    fprint(val);
                    break;
                case 's':                       // String
                    //io_puts_no_newline(va_arg(a, char*));
                    print(va_arg(a, char*));
                    break;
                case 'c':                       // Char
                    PUTC(va_arg(a, int)); // TODO: 'char' generated a warning
                    break;
                case 'd':
                case 'i':                       // 16 bit Integer
                case 'u':                       // 16 bit Unsigned
                    i = va_arg(a, int);
                    if(c == 'i' && i < 0) i = -i, PUTC('-');
                    xtoa((unsigned)i, dv + 5);
                    break;
                case 'l':                       // 32 bit Long
                case 'n':                       // 32 bit uNsigned loNg
                    n = va_arg(a, long);
                    if(c == 'l' &&  n < 0) n = -n, PUTC('-');
                    xtoa((unsigned long)n, dv);
                    break;
                /*case 'x':                       // 16 bit heXadecimal
                    i = va_arg(a, int);
                    d = i >> 12;
                    if (d > 0 || fill_zeros >= 4)
                        puth(d);
                    d = i >> 8;
                    if (d > 0 || fill_zeros >= 3)
                        puth(d);
                    d = i >> 4;
                    if (d > 0 || fill_zeros >= 2)
                        puth(d);
                    puth(i);
                    break;*/
                case 'x':    
                     {
                     // if(c!='x')nu=(int)c-'0';
                     //nu=2;
                      hex = va_arg(a, int);
                      
                      print_hex(hex,nu);
                     }                   // 16 bit heXadecimal
                    
                    break;
                case '2':
                case '4':
                case '8':
                   nu=(int)c-'0';
                   goto parse_fmt_char;
                break;
                case '0':
                    c = *format++;
                    fill_zeros = c - '0';
                    goto parse_fmt_char;
                case 0: return 0;
                default: goto bad_fmt;
            }
        } else
bad_fmt:    PUTC(c);
    }
    va_end(a);
    return 0; // TODO: return number of chars printed
}


int mysprintf( char *buff,const char *format, ...){

    char c;
    int i,count;
    long n;
    int fill_zeros;
    unsigned d;
    float val;

    //
     char *str;
     char ch;
     int dec; 
     long ln;
     int hex;
     double temp;
     char b[10];
    //
    int nu=2;
    memset(buff,0,(sizeof(buff)));

    va_list a;
    va_start(a, format);
    while((c = *format++)) {
        if(c == '%') {
            parse_fmt_char:
            switch(c = *format++) {
                case 'f':                       // String
                    {
                      //print("str ");
                      temp=va_arg(a, double);
                      //ftoa((float)temp,b,2);
                      //myprintf("%s",b);
                    }
                    break; 
                case 's':                       // String
                    {
                      //print("str ");
                      str=va_arg(a, char*);
                      while(*str){
                        *buff++=*str++;
                      }
                      *buff++='\0';
                    }
                    break;
                case 'c':                       // Char
                    {
                      //print("char ");
                      ch=va_arg(a, int);
                      *buff++=ch;
                      *buff++='\0';
                    }
                    break;
                case 'i':                       // 16 bit Integer
                case 'u':                       // 16 bit Unsigned
                    {
                      char n[20];
                      char *p;
                      dec = va_arg(a, int);
                      intToStr(dec,n,10);
                      p=n;
                      while(*p)*buff++=*p++;
                    }
                    //if(c == 'i' && i < 0) i = -i, PUTC('-');
                    //xtoa((unsigned)i, dv + 5);
                    break;
                case 'l':                       // 32 bit Long
                case 'n':                       // 32 bit uNsigned loNg
                    {
                      char n[20];
                      char *p;
                      ln = va_arg(a, long);
                      intToStr(ln,n,10);
                      p=n;
                      while(*p)*buff++=*p++;
                    }
                    //if(c == 'l' &&  n < 0) n = -n, PUTC('-');
                    //xtoa((unsigned long)n, dv);
                    break;
                case 'x':    
                     {
                     // if(c!='x')nu=(int)c-'0';
                     //nu=2;
                      hex = va_arg(a, int);
                      
                      for (int i = (4*nu)-4; i >=0; i-=4){
                           *buff++= "0123456789ABCDEF"[(hex >> i) % 16];
                      }
                      //*buff++='\0';
                     }                   // 16 bit heXadecimal
                    
                    break;
                case '2':
                case '4':
                case '8':
                   nu=(int)c-'0';
                   goto parse_fmt_char;
                break;
                default:// PUTC(c);
                   // nu=(int)c-'0';
                    //goto _else;
                break;
            }
        } else{
            //PUTC(c);
          _else:
          *buff++=c;
        }
    }
    *buff++='\0';
    va_end(a);
    return 0; // TODO: return number of chars printed
}