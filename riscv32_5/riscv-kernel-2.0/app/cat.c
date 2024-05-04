

asm("j main");

#include <stddef.h>
#include <stdint.h>
#include "../include/reg.h"
#include "../include/serial.h"
#include "../include/syscall.h"
#include "../include/ff.h"

 
 

 void main(){
   char p[2];
   UINT bw; 
   char c;
   void *context;
   FIL *file;
   FRESULT res;

   uint32_t size=0;
   file=(FIL*)k_malloc(sizeof(FIL));
   int pid=get_id();
   void *arg=get_args(pid);
   res=exist((char*)arg);
   if (res == FR_OK)
   {
       openFile((char*)arg,file,FA_OPEN_EXISTING | FA_READ);
       size=file->fsize;
       myprintf("cat %s , %i\n",(char*)arg,size);
       char *buff=(char *)k_malloc(64*sizeof(char));
       do{
          bw=readbuff(file,buff,64);
          
          
          for (int i = 0; i < 64; ++i)
          {
               myprintf("%c",buff[i]);
          }
          //readbyte(file,&c);
          //myprintf("%c",c);
          //context=get_context();
          //k_yield(context);
          if(bw!=64){
             break;
          } 
       }while(1/*size--*/);
       close(file);
       k_free(buff);
   }else{
      myprintf("file not found\n");
   }

   k_free(file);
   //context=get_context();
   //k_yield(context);
   exit_task(pid);
   /*
 
   char *p;
    static int counter=0;
  // myprintf("%i,%s\n",get_id(),current_task->args);
   myprintf("%i\n",get_id());
   void *context;
  
  while(1){
      int pid=get_id();
      void *arg=get_args(pid);
     

      myprintf("%s , pid:(%i) , %l\n",(char*)arg,pid,counter+=2);
     //wait(1);
      context=get_context();
      k_yield(context);
      if(!(counter % 10)){
        myprintf("exit task\n");
        exit_task(pid);
      }
  }*/
}
