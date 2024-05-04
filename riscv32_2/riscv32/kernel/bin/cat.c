
asm("j main");
#include "../lib/applib.h"



void main(char *url){
int i;
char buffer[80];
unsigned long si;
char ch;
unsigned long l,k=0;
strcpy(buffer,url);
myprintf("file: %s\n",url);

  i=openfile((uint32_t)buffer);
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
     while(l<si+2){
           ch=readfile();
           if(ch=='\n'){
           	//myprintf(" (%l)",k++);
           }
           myprintf("%c",ch);
           l++;
     }
     l=0;
     myprintf("\n*******************************************************\n");
     myprintf("\n end\n");
     closefile();
  }else{
     myprintf("fail\n");
  }
}
