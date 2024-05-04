 
#include <stdint.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include "../include/reg.h"
#include "../include/serial.h"
#include "../include/kmalloc.h"
#include "../include/diskio.h"
#include "../include/ff.h"
#include "../include/basic.h"
//#include <setjmp.h>

//jmp_buf main_context; 


void __attribute__ ((section(".interrupt"))) isr(){    
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    //int_isr();
    asm("uret");  
}


/*

*/


int main(void)
{
   MODE=0;
   cli();
   myprintf("Iniciando kernel\n");
   kmalloc_init(KMALLOC_START, KMALLOC_LENGTH);
   kmalloc_debug();
   FATFS FatFs;
   FRESULT res = f_mount(&FatFs, "", 1);
   if (res==FR_OK)
   {
        myprintf("OK f_mount\n");
   }else{
         myprintf("fail f_mount\n");
         while(1);
   }
   
  /*strcpy(p_buf,"");
  strcat(p_buf,"10 print a;");
  strcat(p_buf,"15 println;");
  strcat(p_buf,"20 a=a+1;");
  strcat(p_buf,"30 goto 10;");*/
  myprintf("Virtual Machine Basic language (Interpreter)\n\r");
  
  if(!load_program(p_buf,"basic/pg.bas"))while(1);

  prog = p_buf;
  scan_labels(); 
  ftos = 0; 
  gtos = 0;
            
      do {
              token_type = get_token();
              //printf("%d\n\r",token_type);
              if(token_type==VARIABLE) {
                 putback(); 
                 assignment();
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
              }
         } while (tok != FINISHED);
         
       while(1);
}



