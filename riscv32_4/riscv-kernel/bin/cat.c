
asm("j main");
//#include "../lib/applib.h"
#include "../lib/nwlib.h"


void main(char *url){
int i;
 
unsigned long si;
char ch;
unsigned long l,k=0;
 
myprintf("file: %s\n",url);

  i=openfile(url);
  if(!i){
     myprintf("ok\n");
     si=filesize();
     if(si<=999){
        myprintf("size file: %i Bytes\n", si);
     }else if(si >=1000 && si<1000000){
        myprintf("size file: %i kb\n", si/1000);
     }else if(si>=1000000){
        myprintf("size file: %l mb\n", si/1000000);
    }
     myprintf("\n*******************************************************\n");
     l=0;
     char *in=kmalloc(si);
     readbuff((void*)in,si);
     for (uint32_t i = 0; i < si; ++i){
       myprintf("%c",in[i]);
     }
     mfree(in);
     l=0;
     myprintf("\n*******************************************************\n");
     myprintf("\n end\n");
     closefile();
  }else{
     myprintf("fail\n");
  }
}
