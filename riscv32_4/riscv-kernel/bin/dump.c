
asm("j main");
#include "../lib/nwlib.h"

void hexdump( unsigned char *buf )  {
  int i;
  char s[16];
  
  // A.  hex bytes
  for( i=0 ; i<16 ; i++ ) {
      mysprintf( s, "%2x ", buf[i] & 0x0ff );
      myprintf("%s",s);
     if( i==7 )myprintf(" "); 
   }
   
  myprintf(" "); 
   
   s[1] = '\0';
   
   for( i=0 ; i<16 ; i++ ) {
     if( buf[i] >=0x20 && buf[i] <= 0x7e ) {
       s[0] = buf[i];
 
       myprintf("%s",s);
     } else {
       
       myprintf(".");
     }
     if( i==7 ) myprintf(" "); 
   }
}


void dump(char *point,uint32_t size){
  long offset = 0;

  unsigned char buf[32];
  
  myprintf("       0  1  2  3  4  5  6  7   8  9  A  B  C  D  E  F\n");

  for(offset = 0 ; offset < size ; offset += 16 ){
      myprintf( "%4x   ", offset );
      //myprintf("%s",buff);

      for( int b=0 ; b<16 ; b++ ){
          
            buf[b] = point[offset + b];
      }
      hexdump( buf );
      myprintf("\n");
  }
}

//char *string __attribute__((naked))="jbdjfblkrfkr\n";



void main(char *url){
int i;
 
unsigned long si;
char ch;
unsigned long l;
 
myprintf("file: %s\n",url);
char *pointer;

  i=openfile(url);
  if(!i){
     myprintf("ok\n");
     si=filesize();
     pointer=kmalloc(si);

     if(si<=999){
        myprintf("size file: %i Bytes\n", si);
     }else if(si >=1000 && si<1000000){
        myprintf("size file: %i kb\n", si/1000);
     }else if(si>=1000000){
        myprintf("size file: %l mb\n", si/1000000);
    }
    
     myprintf("\n*******************************************************\n");
     readbuff((void*)pointer,si);
     dump(pointer,si+2);
     myprintf("\n*******************************************************\n");
     myprintf("\n end\n");
     closefile();
  }else{
     myprintf("fail\n");
  }

  mfree(pointer);
}