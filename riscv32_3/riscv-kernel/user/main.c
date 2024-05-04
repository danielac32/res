
asm("j main");
#include <setjmp.h>
#include "../lib/applib.h"

#define NUM_LAB 100
#define LAB_LEN 10
#define FOR_NEST 2
#define SUB_NEST 2
#define PROG_SIZE 2048

#define DELIMITER  1
#define VARIABLE  2
#define NUMBER    3
#define COMMAND   4
#define STRING    5
#define QUOTE   6

#define PRINT 1
#define INPUT 2
#define IF    3
#define THEN  4
#define FOR   5
#define NEXT  6
#define TO    7
#define GOTO  8
#define EOL   9
#define FINISHED  10
#define GOSUB 11
#define RETURN 12
#define END 13
#define DELAY_MS 14
#define PINMODE 15
#define DWRITE 16
#define DREAD 17
#define AREAD 18
#define AWRITE 19
#define DELAY_US 20
#define PRINTLN 21

jmp_buf e_buf;
jmp_buf e_buf2;
static struct commands { /* keyword lookup table */
  char command[20];
  char tok;
} table[] = { /* Commands must be entered lowercase */
  "print", PRINT, /* in this table. */
  "input", INPUT,
  "if", IF,
  "then", THEN,
  "goto", GOTO,
  "for", FOR,
  "next", NEXT,
  "to", TO,
  "gosub", GOSUB,
  "return", RETURN,
  "end", END,
  "delay_ms",DELAY_MS,
  "delay_us",DELAY_US,
  "pinmode",PINMODE,
  "dwrite",DWRITE,
  "dread",DREAD,
  "aread",AREAD,
  "awrite",AWRITE,
  "println",PRINTLN,
  "", END 
};

static struct pt {
  char lc;
};

static struct pt tsk1;
static struct pt tsk2;
 
static char PT_YIELD_FLAG __attribute__ ((section(".data")))= 1;

#define LC_INIT(s) s = 0;

#define LC_RESUME(s) switch(s) { case 0:

#define LC_SET(s) s = __LINE__; case __LINE__:

#define LC_END(s) }

#define PT_WAIT_WHILE(pt,cond)  \
do{                             \
    LC_SET(pt.lc);              \
    if((cond)) {                \
        return;                 \
    }                           \
}while(0)


#define PT_YIELD(pt)         \
  do {                       \
    PT_YIELD_FLAG = 0;       \
    LC_SET(pt.lc);           \
    if(PT_YIELD_FLAG == 0) { \
      return;                \
    }                        \
  } while(0)


void printt(){
int res=0;
int i=0;
res=setjmp(e_buf);
myprintf("setjmp: %i\n",res);
for(i=0; *table[i].command; i++){
    myprintf("%s\n",table[i].command);
}
}
void printt2(){
 
myprintf("*********************************\n");

}


void task1(){ 
static int counter=0;
PT_YIELD_FLAG = 1;
LC_RESUME(tsk1.lc);
while(1){
      myprintf("task1: %i      ",counter++);
      PT_YIELD(tsk1);
}
LC_END(task1.lc);
PT_YIELD_FLAG = 0;
LC_INIT(tsk1.lc);
}

void task2(){
static int counter=0;
PT_YIELD_FLAG = 1;
LC_RESUME(tsk2.lc);
while(1){
      myprintf("task2: %i\n",counter++);
      PT_YIELD(tsk2);
}
LC_END(task2.lc);
PT_YIELD_FLAG = 0;
LC_INIT(tsk2.lc);
}



void main(int pid,uint32_t funcpointer){
//printt2();
//printt();
//longjmp(e_buf, 1);  
while(1){
    task1();
    task2();
}


kill_task(pid);
}