
 
#include <stdint.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include "../include/reg.h"
#include "../include/serial.h"
#include "../include/kmalloc.h"
#include "../include/diskio.h"
#include "../include/ff.h"
#include "../include/timer.h"
#include "../include/shell.h"
#include "../include/syscall.h"
#include "../include/task.h"
#include "../include/basic.h"
#include "data.h"

FATFS FatFs;    
FILINFO fno;

FRESULT scan_files(char *path){
  FRESULT res;
  DIR dir;
  UINT i;
  static FILINFO fno;
  res = f_opendir(&dir,path);
  if(res==FR_OK){
     for (;;)
     {
       res = f_readdir(&dir,&fno);
       if(res != FR_OK || fno.fname[0]==0)break;
        
        
       myprintf("%s",fno.fname);

       if(fno.fattrib & AM_DIR){
          i = strlen(path);
          myprintf("/\n");
          memcpy(&path[i],fno.fname,13);
          res = scan_files(path);
          if(res != FR_OK)break;
          path[i]=0;
       }else{
         // myprintf("%s,%s\n",path,fno.fname);
        myprintf("\t\t");
        myprintf("%l\n",fno.fsize);
       } 

       
     }
     f_closedir(&dir);
  }

  return res;
}


void basic_interpreter(){
  static int pid;
  pid=get_id();
  void *arg=get_args(pid);
  strcpy(p_buf,"");
  /*strcat(p_buf,"10 print a;");
  strcat(p_buf,"15 println;");
  strcat(p_buf,"20 a=a+1;");
  strcat(p_buf,"30 goto 10;");*/
  myprintf("Virtual Machine Basic language (Interpreter)\n\r");
  
  if(!load_program(p_buf,(char*)arg)){
     while(1){
          yield();
     }
  }

  prog = p_buf;
  scan_labels(); 
  ftos = 0; 
  gtos = 0;
  myprintf("%s\n",prog);
      do {
              token_type = get_token();
              //printf("%d\n\r",token_type);
              if(token_type==VARIABLE) {
                 putback(); 
                 assignment();
                 yield();
              }else{
                  switch(tok) {
                        case PRINT:
                          _print();
                        break;
                        case GOTO:
                          exec_goto();
                        break;
                        case IF:
                          exec_if();
                        break;
                        case ELSE:
                          exec_else();
                        break;
                        case FOR:
                         exec_for();
                        break;
                        case NEXT:
                         next();
                        break;
                        case INPUT:
                          input();//printf("input\n\r");
                        break;
                        case GOSUB:
                          gosub(); 
                        break;
                        case RETURN:
                          greturn();
                        break;
                        case DELAY_MS:
                          delay_ms();//printf("delay_ms\n\r"); 
                        break;
                        case DELAY_US:
                          delay_us();//printf("delay_us\n\r");
                        break;
                        case PINMODE:
                          pinmode();//printf("pinmode\n\r"); 
                        break;   
                        case DWRITE:
                          dwrite();//printf("dwrite\n\r");
                        break;
                        case DREAD:
                          dread();//printf("dread\n\r");
                        break;
                        case AREAD:
                          aread();//printf("aread\n\r");
                        break;
                        case AWRITE:
                          awrite();///printf("awrite\n\r");
                        break;
                        case PRINTLN:
                          println();
                        break;
                        case TOGGLE:
                          myprintf("toggle\n\r");
                        break;
                        case END:
                          tok=FINISHED;
                        break;
                  }
                  yield();
              }
              yield();
         } while (tok != FINISHED);
         myprintf("exit(basic RUNNING)\n");
         while(1){
             yield();
         }
}
//***************************************************************




void task1() {
    static int count=0;
    int argc=0;
    char *argv[10]; 
    FIL file; 
    init_shell();
    myprintf("shell ok :%i\n",get_id());
    while(1) {

       //myprintf("task %i ,%i\t\t", current_task->id,count++);
       int c =get_cmd(&argc,argv);  
       if(c){ 
           if(!strcmp(argv[0],"pwd")){
                myprintf("%s\n",global_path);
           }else
           if(!strcmp(argv[0],"cd")){
            {  
               if(!strcmp(argv[1],"..")){
                   {
                    char *pch;
                    char temp[20][20];
                    int count=0;
                    
                    //myprintf("%s\n",global_path);
                    pch = strtok (global_path,"/");
                    strcpy(temp[count],pch);
                    count++;
                    while(pch){
                        pch = strtok (0,"/");
                        strcpy(temp[count],pch);
                        count++;
                    }
                    count-=2;
                    strcpy(global_path,"");
                    //myprintf("%i\n",count);
                    for (int i = 0; i < count; ++i)
                    {
                         //myprintf("%i) %s\n",i,temp[i]);
                         strcat(global_path,"/");
                         strcat(global_path,temp[i]);
                    }
                   }
               }else if(!strcmp(argv[1],"clear")){
                        strcpy(global_path,"");
               }else if(!strcmp(global_path,"/")){
                        strcat(global_path,argv[1]);
               }else{
                  strcat(global_path,"/");
                  strcat(global_path,argv[1]);
               }
               //myprintf(">> %s\n",global_path );
            }
           }else
           if(!strcmp(argv[0],"ls")){
            {
                char *buff=(char*)kmalloc(256*sizeof(char));
                strcpy(buff,global_path);
                scan_files(buff);
                kfree(buff);
            }
           }else
           if(!strcmp(argv[0],"run")){
             //kmalloc_debug();
            {
              myprintf("load array\n");
              void (*dynamic)(void);
              dynamic=(void*)raw;
              (*dynamic)();
              myprintf("\nend\n");
            }
         }else

          if(!strcmp(argv[0],"basic")){
             //kmalloc_debug();
            {
               create_task(basic_interpreter,argv[1],"Interpreter");
            }
         }else
         if(!strcmp(argv[0],"ok")){
            myprintf("ok\n");
         }else
         if(!strcmp(argv[0],"kill")){
             {
              if(!strcmp(argv[1],"all")){
                 for (int i = 2; i < next_task; ++i)
                 {
                      kill(i);
                 }
              }else{
                 uint8_t pid=atoi(argv[1]);
                 kill(pid);
              }
             }
         }else
          if(!strcmp(argv[0],"cat")){
             {
                loadDynamically("bin/cat.bin",argv[1]);
             }
          }else
          if(!strcmp(argv[0],"dump")){
            {
               loadDynamically("bin/dump.bin",argv[1]);
            }
         }else
         if(!strcmp(argv[0],"list")){
            //myprintf("counter: %i\n",task_cnt);
            kmalloc_debug();
            for (int i = 0; i < next_task; ++i){
                 if (tasks[i].state==RUNNING)
                 {
                     myprintf("id: %i, entry: %8x , stack pointer: %8x , name: %s , arg: %s\n",tasks[i].id,tasks[i].entry,tasks[i].stack,tasks[i].name,tasks[i].arg);
                 }
            }
         }else
         if(!strcmp(argv[0],"echo")){
            {
                char *buff=(char*)kmalloc(100);
                strcpy(buff,"");
                for (int i = 1; i < argc; ++i)
                {
                   strcat(buff,argv[i]); 
                   strcat(buff," ");   
                }
               
                loadDynamically("bin/echo.bin",buff);
                kfree(buff);
            }
        }else
         if(!strcmp(argv[0],"load")){ 
             {     
                   myprintf("%s %s\n",argv[1],argv[2]);
                   loadDynamically(argv[1],argv[2]);
             }
         }else
         if(!strcmp(argv[0],"reset")){
           reset();
         }else
          if(!strcmp(argv[0],"root")){
              //char buff[256];
              char *buff=(char*)kmalloc(256*sizeof(char));
              strcpy(buff,"/");
              scan_files(buff);
              kfree(buff);
          }
          for (int i = 0; i < argc; ++i){
               kfree(argv[i]);
          }
       }
       yield();
    }
}
 

void task2() {
     static int count=0;
    void *context;
    
   myprintf("core 2:%i\n",get_id());
    while(1) {
       //myprintf("task %i ,%i\n", current_task->id,count+=2);

        context=get_context();
        k_yield(context);

      wait(2000);
    }
}



int main(void)
{
   MODE=0;
   cli();
  
   myprintf("Iniciando kernel\n");
   kmalloc_init(KMALLOC_START, KMALLOC_LENGTH);
   kmalloc_debug();
   strcpy(global_path,"");
   strcpy(global_path,"/");

   FRESULT res = f_mount(&FatFs, "", 1);
   if (res==FR_OK)
   {
       myprintf("OK f_mount\n");
       char buff[256];
       strcpy(buff,"/");
       f_mount(&FatFs, "", 1);
       scan_files(buff);
       myprintf("type: %i\n",FatFs.fs_type);
   }else{
       myprintf("fail f_mount %i\n",res);
      //while(1);
   }

   uint8_t result;
     FIL *file,*otro; 
     
   file=(FIL*)kmalloc(sizeof(FIL));
   otro=(FIL*)kmalloc(sizeof(FIL));

    result=exist("app.bin");
    myprintf("exist: %i\n",result);
 
    result=openFile("bin/app.bin",file,FA_OPEN_EXISTING | FA_READ);
    myprintf("openFile: %i,%i\n",result,file->fsize); 
    char data;
    char buff[64];
    UINT bw;
    //buff=readbuff(file,64);
    seek(file,7);

    readbyte(file,&data);
    myprintf("%x\n",data);

    seek(file,0);
    readbuff(file,buff,32);
    for (int i = 0; i < 32; ++i)
    {
     //readbyte(file,&data);
     myprintf("%x ",buff[i]);
    }
 
    myprintf("\n");
    close(file);

    kfree(file);
    kfree(otro); 
  

    init_thread();
    create_task(task1, "shell","core1");
    create_task(task2, "core2","core2");
   /// create_task(basic_interpreter,"core","core");
    schedule();


   /******************************************/
    

return 0;
 }
 
