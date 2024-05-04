#include <stdint.h>
#include <string.h>
#include "serial.h"
#include "kmalloc.h"
//#include "task.h"
#include "context.h"

char stack0[4096];
char _gp[8];

typedef struct
{
	uint8_t EN;
	uint32_t VAL;
}IRQ;
#define TIMER    ( ( IRQ * )  0x4000200 )

typedef struct 
{
	uint8_t get;
	uint8_t put;
}console;
#define CONSOLE ( ( console * )  0x4000300 )



extern void exception_syscall();
char *cadena="daniel eduardo quintero villegas";
char cadena2[]={"ok ok\n"};
const char shell_init_msg[] = "\n\rfor a list of programs on this system type 'help'\nREADY\n\r";


/*void exception_dump_ctx(struct irq_context *ctx)
{
    printf("Exception: PC=%08x Cause=%x Status=%08x\n", ctx->pc, ctx->cause, ctx->status);
    for (int i=0;i<NUM_GP_REG;i++)
    {
        printf("REG %.2d: %08x\n", i, ctx->reg[i]);
    }
}*/


#define REG_RA                         1
#define REG_SP                         2
#define REG_ARG0                       10
#define REG_RET                        REG_ARG0
#define NUM_GP_REG                     32
#define NUM_CSR_REG                    3

#define CAUSE_MISALIGNED_FETCH         0
#define CAUSE_FAULT_FETCH              1
#define CAUSE_ILLEGAL_INSTRUCTION      2
#define CAUSE_BREAKPOINT               3
#define CAUSE_MISALIGNED_LOAD          4
#define CAUSE_FAULT_LOAD               5
#define CAUSE_MISALIGNED_STORE         6
#define CAUSE_FAULT_STORE              7
#define CAUSE_ECALL_U                  8
#define CAUSE_ECALL_S                  9
#define CAUSE_ECALL_M                  11
#define CAUSE_PAGE_FAULT_INST          12
#define CAUSE_PAGE_FAULT_LOAD          13
#define CAUSE_PAGE_FAULT_STORE         15
#define CAUSE_INTERRUPT                (1 << 31)
#define CAUSE_MAX_EXC      (CAUSE_PAGE_FAULT_STORE + 1)

struct irq_context
{
    uint32_t pc;
    uint32_t status;
    uint32_t cause;
    uint32_t reg[32];
};


void exception_dump_ctx(struct irq_context *ctx)
{
    myprintf("Exception: PC=%8x Cause=%x Status=%8x\n", ctx->pc, ctx->cause, ctx->status);
    for (int i=0;i<NUM_GP_REG;i++)
    {
         myprintf("REG %d: %8x\n", i, ctx->reg[i]);
    }
}



void *exception(struct irq_context *ctx){
    if (ctx->cause & CAUSE_INTERRUPT)
    {
        //myprintf("interrupt: %8x\n",ctx->cause);
       
       /* for(int i = 0; i < next_task; ++i) {
            if(tasks[i].sleep > 0) {
               --tasks[i].sleep;
            }
        }*/
       
    }else{
       switch (ctx->cause)
        {
            case CAUSE_ECALL_U:
            case CAUSE_ECALL_S:
            case CAUSE_ECALL_M:
                ctx->pc += 4;
                break;
        }
        if (ctx->cause < CAUSE_MAX_EXC){
            
            uint32_t a0 = ctx->reg[REG_ARG0 + 0];
    		uint32_t a1 = ctx->reg[REG_ARG0 + 1];
    		uint32_t a2 = ctx->reg[REG_ARG0 + 2];
    		uint32_t a3 = ctx->reg[REG_ARG0 + 3];
    		uint32_t a4 = ctx->reg[REG_ARG0 + 4];
    		uint32_t a5 = ctx->reg[REG_ARG0 + 5];
    		uint32_t n  = ctx->reg[REG_ARG0 + 7];
    		int      fd = (int)a0;

            myprintf("syscall: %8x\n",a0);

            //ctx = _exception_table[ctx->cause](ctx);
        }else if (ctx->cause == CAUSE_FAULT_LOAD || ctx->cause == CAUSE_FAULT_STORE ||
                 ctx->cause == CAUSE_MISALIGNED_LOAD || ctx->cause == CAUSE_MISALIGNED_STORE)
        {
            myprintf("other");
            exception_dump_ctx(ctx);
        }else
        {
            myprintf("Unhandled exception");
        }
    }

return ctx;
}

//extern int setjmp(void *env);
///extern void longjmp(void *env,int t); //

/*
 
void task3(void *ptr) {
    myprintf("arg 3: %s\n",ptr );
    //char *str=(int*)ptr;
    //myprintf("task 3: %s\n",ptr);
    while(1) {
       myprintf("\t\ttask3 %i\n", current_task->id);
       wait(100);
    }
}


void task1(void *ptr) {
 
   myprintf("arg 1: %s\n",ptr);
   //
    while(1) {
       myprintf("task1 %i\n", current_task->id);
       wait(30);
    }
}


void task2(void *ptr) {
    myprintf("arg 2: %s\n",ptr);
    create_task(task3, "ormary","core3");
    while(1) {
       myprintf("\ttask2 %i\n", current_task->id);
       wait(30);
    }
}
*/



long count;
int main()
{
   print("hola !!\n");
   print("123456789\n");
   print(shell_init_msg);
   print(cadena2);

   TIMER->VAL=10000;
   TIMER->EN=1;
    
   //myprintf(cadena2);
   exception_syscall();


    asm("li a0,77");
    asm("li a7,1025");
    asm("ecall"); 


    kmalloc_init(KMALLOC_START, KMALLOC_LENGTH);
    kmalloc_debug();

    
    /*char *arg1="veronica";
    char *arg2="daniel";

    create_task(task1, arg1);
    create_task(task2, arg2);
 
    //create_task(some_task, NULL);
    schedule();*/


   /* char *arg1="veronica";
    char *arg2="daniel";
    main_context.stack=(char*)kmalloc(512);
    create_task(task1, arg1,"core1");
    create_task(task2, arg2,"core2");
*/
 
    //schedule();
    
    while(1){
       /*  for(int i = 0; i < next_task; ++i) { // round-robin
            if(tasks[i].sleep == 0 && tasks[i].state==RUNNING) {
               proceed(&tasks[i]);
            }
        }*/
    }

    while(1){

       asm volatile ("csrr %0, cycle" : "=r" (count) : );
       myprintf("%i\n", count);

      // if(!(i % 100000*2))mem=88;
       
       /*for (int i = 0; i < 100000; ++i)
       {
         asm("nop");
         asm("nop");
       }*/
   }
   myprintf("end\n");
   return 0;
}