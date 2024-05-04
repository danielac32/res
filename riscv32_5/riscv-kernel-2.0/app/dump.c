asm("j main");

#include <stddef.h>
#include <stdint.h>
#include "../include/reg.h"
#include "../include/serial.h"
#include "../include/syscall.h"
#include "../include/ff.h"

 
 
void hexdump( unsigned char *buf )  {
  int i;
  char s[16];
  
  // A.  hex bytes
  for(int i=0 ; i<16 ; i++ ) {
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

 void main(){
   UINT bw; 
   void *context;
   FIL *file;
   FRESULT res;
   long offset = 0;
   long offset2 = 0;
   unsigned char buf[32];

   uint32_t size=0;
   file=(FIL*)k_malloc(sizeof(FIL));
   int pid=get_id();
   void *arg=get_args(pid);
   res=exist((char*)arg);
   if (res == FR_OK)
   {
       openFile((char*)arg,file,FA_OPEN_EXISTING | FA_READ);
       size=file->fsize;
       myprintf("dump %s , %i\n",(char*)arg,size);
       char *block=(char *)k_malloc(64*sizeof(char));
       do{
          bw=readbuff(file,block,64);
          for(offset = 0 ; offset < 64 ; offset += 16 ){
              myprintf( "%4x   ", offset2+=16 );

              for( int b=0 ; b<16 ; b++ ){
                   buf[b] = block[offset + b];
              }
              hexdump( buf );
              myprintf("\n");
          }
          if(bw!=64){
             break;
          } 
       }while(1/*size--*/);
       close(file);
       k_free(block);
   }else{
      myprintf("file not found\n");
   }

   k_free(file);
   //context=get_context();
   //k_yield(context);
   exit_task(pid);
   
}
