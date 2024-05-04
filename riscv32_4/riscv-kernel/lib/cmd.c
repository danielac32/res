#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "lib.h"
#include "serial.h"
#include "syscall.h"
#include "task.h"
#include "terminal.h"
#include "cmd.h"
#include "klib.h"

char ch;
int i=0;
uint32_t si=0;

void put(char c){
     myprintf("%c",c);
}

void process0(int pid,uint32_t funcpointer)
{
 
       if(k_SerialAvailable()){
          char c=k_SerialRead();
          key.inChar=c;//SerialRead();
          
       if(key.index<45){
          key.indata[key.index]=key.inChar;
          key.index++;
          key.indata[key.index]='\0';
       }else if(key.indata[key.index]==0x08){
       	  key.index--;
       	  key.index--;
       	  key.indata[key.index]='\0';
       }  
       #if mcu == 1
             //myprintf("%c",c);
              k_putc(c);
       #endif
       if(key.indata[key.index]=='\0' && key.indata[key.index-1]=='\n'){
       	  key.indata[key.index-1]='\0';
       	  key.index=0;
       	  if(key.indata[0]==0x0a){//si tine salto de linea
             for(int i=0;i<strlen(key.indata)-1;i++){
                 key.indata[i]=key.indata[i+1];
             }
             key.indata[strlen(key.indata)-1]='\0';
          }
 
           prog=key.indata;
           
           get_token();
            
           
           if(!strcmp(token,"process")){
               myprintf("ok\n");
               list_process();
           }  
           if(!strcmp(token,"kill")){
              get_token();
              strcpy(buff,"bin/kill.bin");
              if(lauch_cmd(0,buff,token)==-1)myprintf("error kill\n");
           }
           if(!strcmp(token,"cat")){ 
               get_token();
               strcpy(buff,"bin/cat.bin");
               if(lauch_cmd(1,buff,token)==-1)myprintf("error cat\n");
           }
           if(!strcmp(token,"echo")){ 
               get_token();
               strcpy(buff,"bin/echo.bin");
               if(lauch_cmd(1,buff,token)==-1)myprintf("error echo\n");
           }
           if(!strcmp(token,"dump")){
               get_token();
               strcpy(buff,"bin/dump.bin");
               if(lauch_cmd(1,buff,token)==-1)myprintf("error dump\n");
           }

           if(!strcmp(token,"list")){
               myprintf("ok\n");
               for (int i = 0; i < MAX_PROCESSES; ++i){
                    myprintf("pid: (%i) name: %s , run: %l \n",i,processlist[i].name,processlist[i].attached);
               }
               info((chunk_t*)mem);
           }

           if(!strcmp(token,"run")){
              get_token();
              myprintf("-> : %s\n",token);
              if(isdigit(*token)) {
                 run_task(atoi(token));
                 myprintf("ok\n");
              }else{
                 myprintf("fail\n");
              }
           }

           if(!strcmp(token,"load")){
               {
                get_token();
                myprintf("-> : %s\n",token);

                strcpy(buff,"user/");
                strcat(buff,token);
                strcat(buff,".bin");

                i=k_openfile(buff);

                if(!i){
                   myprintf("ok\n");
                   si=k_filesize();
                   if(si<=999){
                      myprintf("size file: %i Bytes\n", si);
                   }else if(si >=1000 && si<1000000){
                      myprintf("size file: %i kb\n", si/1000);
                   }else if(si>=1000000){
                      myprintf("size file: %l mb\n", si/1000000);
                   }
                   process_attach("task", 1,0,&core.i,si);
                   k_readbuff((void*)processlist[core.i].p,si); 
                 
                   k_closefile();
                   process_modify(nametsk[core.i], 1, (void*)processlist[core.i].p,core.i);
                   //
                   /*void (*jump)(void);
                   jump=(void*)processlist[core.i].p;
                   (*jump)();
                   do_free((chunk_t*)mem,processlist[core.i].p); */
                   //
                   myprintf("point: %x , pid: %i\n",processlist[core.i].p,core.i);
                  // process_run(core.i);
                }else{
                   myprintf("fail\n");
                }
               }
           }
           if(!strcmp(token,"exit")){
               myprintf("ok\n");
               myprintf("->exit cpu riscv\n");
               exit_cpu();
           }

           myprintf("\n");
       	   strcpy(key.indata,"");
       	   *prog='\0';
           strcpy(token,"");
      }
    }
}
uint32_t counter=0;
void process1(int pid,uint32_t funcpointer)
{
	 asm("nop");
	 asm("nop");
	 asm("nop");
	 asm("nop");
   if(k_get_time()-counter>= 5000){
      counter=k_get_time();
      myprintf(" task 2 \n");
   }
   
}