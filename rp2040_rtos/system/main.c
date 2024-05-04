#include "resets.h"
#include "gpio.h"
#include "uart.h"
#include "xosc.h"
#include "nvic.h"
#include "systick.h"
#include <queue.h>
#include <string.h>
#include "kmalloc.h"
#include <rp2040.h>
#include "kernel.h"
#include "shell.h"
#define LED   25
sem_t sem1;
 
 



/////////////////////////////////////////////////


 




 void delay2(long t) {
  while (t--)
    asm volatile ("nop");
}


void t3(int nargs, char *args[]) {
  for (int i = 0; i < nargs; ++i)
  {
       printf("%s ", args[i]);
  }
  printf("\n");
  while (1) {
    //sem_wait(&sem1);
    printf("\t\t\tT  3 :%d\r\n",getpid());
    //thread_sleep(0);
    delay2(100000);
    //sem_signal(&sem1);
  }
}


int atoi(char *p)
{
    int n = 0, f = 0;

    for (;; p++)
    {
        switch (*p)
        {
        case ' ':
        case '\t':
            continue;
        case '-':
            f++;
        case '+':
            p++;
        }
        break;
    }

    while (*p >= '0' && *p <= '9')
    {
        n = n * 10 + *p++ - '0';
    }

    return (f ? -n : n);
}


void t1() {
  while (1) {
    printf("%s",SHELL_PROMPT);
         while(!uart0_available());
          while(uart0_available()){
              len = readBytes(buf,sizeof(buf));
        }
        if (len == EOF) {
           break;
        }
        if (len <= 1) {
            continue;
        }

        buf[len] = SH_NEWLINE;  /* terminate line */

  

    ntok = lexan(buf, len, tokbuf, &tlen, tok, toktyp);

    /* Handle parsing error */

    if (ntok == SYSERR) {
      printf("%s\n",SHELL_SYNERRMSG);
      continue;
    }

    /* If line is empty, go to next input line */

    if (ntok == 0) {
      printf( "\n");
      continue;
    }
    
    for (j = 0; j < ncmd; j++) {
      src = cmdtab[j].cname;
      cmp = tokbuf;
      diff = false;
      while (*src != NULLCH) {
        if (*cmp != *src) {
          diff = true;
          break;
        }
        src++;
        cmp++;
      }
      if (diff || (*cmp != NULLCH)) {
        continue;
      } else {
        break;
      }
    }

    /* Handle command not found */

    if (j >= ncmd) {
        printf("command %s not found %d\n",tokbuf,len);
        continue;
    }else{
        printf("ok");
        printf(" %d ",j);
      for (int i=0; i<ntok; i++) {
          args[i] = &tokbuf[tok[i]];
          printf("%s",args[i]);
          printf(" ");
      }
      if(!strcmp(args[0],"create")){
          ready(create((void *)t3,4096,1,"task2",2,"hola","daniel"));
      }else if(!strcmp(args[0],"kill")){
          kill(atoi(args[1]));
      }else if(!strcmp(args[0],"mem")){
          list();
      }
      printf("\n");   
    }
  }
}



void t2() {
   //thread_start(t3, 0);
  //int pid=create((void *)t3,4096,1,"blink",0);//pid=kernel_taskCreate("blink",&core1, 128, (void*)0);
  //queue_push(&ready_queue, &proctab[pid]);//task_enqueue2(&proctab[pid]);//insert(pid);
  while (1) {
    gpio_set(25, 1);
    thread_sleep(50);
    gpio_set(25, 0);
    thread_sleep(100);
  }
}




void test_threads() {
  //sem_init(&sem1, 1);
  //thread_start(t1, 0);
  //thread_start(t2, 0);
 int pid=create((void *)t1,4096,1,"task1",0); 
 ready(pid);
 pid=create((void *)t2,4096,1,"shell",0); 
 ready(pid);


}


int  main() {
disable();
meminit();
list();
 

systick_init();
rtos_init();

test_threads();


systick_set(12000);
enable();
PEND_SV();//rtos_start();
return 0;
}
