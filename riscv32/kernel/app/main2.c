
asm("j main");
#include "../lib/applib.h"





void main(int pid,uint32_t funcpointer){
int i;
char buffer[80];


  unsigned long si;
  char ch;
  unsigned long l;

  strcpy(buff2,"Makefile");
  i=openfile((uint32_t)buff2);
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
     while(l<si+2){
           ch=readfile();
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

kill_task(pid);
}