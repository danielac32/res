/*
 riscv64-unknown-elf-gcc -march=rv32i -mabi=ilp32 -O3 -nostdlib test1.c -o test1
*/
#include <stdint.h>
#define uart *(volatile char*) 0x40002000


void printstr(char *str){
    while(*str){
        (uart) = *str++;
    }
}


#define MTIM 7
#define MSIT 3
#define MEIT 11

extern void enable_interrupt_status();
extern void enable_interrupt(unsigned int num);
extern void disable_interrupt_status();
extern void disable_interrupt(unsigned int num);
extern void clear_interrupts();
extern void soft_trap();
#define timel *(uint32_t*)0x11004000
#define timeh *(uint32_t*)0x11004004

extern void trap_vector();

// These are going to be bound to memory addresses in the linker script.
extern uint32_t SYSCON;
extern uint32_t TIMERL;












uint32_t exceptionHandler(int epc,int cause){

  uint32_t return_pc = epc;
  uint32_t cause_code = cause & 0xfff;
  
  if (cause & 0x80000000){

        switch (cause_code)
        {
        case 3:
          printstr("software interruption!\n");
          break;
        case 7:
           
          break;
        case 11:
          printstr("external interruption!\n");
          break;
        default:
          printstr("unknown async exception!\n");
          break;
        }

  }else{
   
    printstr("Sync exceptions!\n");
    while(1);
   
  }
  return return_pc;


//pprint(a);
//pprint(b);
// clear_interrupts();
}


void softTrapHandler(int a) {
    printstr("soft trap\n");
    clear_interrupts();
}


int main()
{
    
     
enable_interrupt_status();
enable_interrupt(MSIT);
//enable_interrupt(MTIM);



printstr("hola\n");

asm("ecall");

return 0;
}
