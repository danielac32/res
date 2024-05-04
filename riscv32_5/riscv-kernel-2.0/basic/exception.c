#include "exception.h"
 
#include <stdint.h>

// Declare a weak alias macro as described in the GCC manual[1][2]
#define WEAK_ALIAS(f) __attribute__ ((weak, alias (#f)));
#define SECTION(s) __attribute__ ((section(s)))

/******************************************************************************
 * Forward undefined IRQ handlers to an infinite loop function. The Handlers
 * are weakly aliased which means that (re)definitions will overide these.
 *****************************************************************************/
void irq_undefined() { 
    // myprintf("irq\n");
}

void fault_handler (void) __attribute__ ((weak));
//void svcall_handler (void) __attribute__ ((weak));
void svcall_handler ();
void pendsv_handler (void) __attribute__ ((weak));
void systick_handler (void) __attribute__ ((weak));

void tim0_handler(void) __attribute__ ((weak));

void tim0_handler(void){
     
}


void option(){
   // myprintf("fault_handler\n"); 
}


void fault_handler (void)   { 
 option();
}

typedef void (*svcall_t)(void);
 

 
struct extended_exception_frame
{
  uint32_t r8;
  uint32_t r9;
  uint32_t r10;
  uint32_t r11;
  uint32_t cause;
  uint32_t sp;
  uint32_t r4;
  uint32_t r5;
  uint32_t r6;
  uint32_t r7;
  uint32_t r0;
  uint32_t r1;
  uint32_t r2;
  uint32_t r3;
  uint32_t r12;
  uint32_t lr;
  uint32_t pc;
  uint32_t psr;
};


#define read(x) (*(volatile uint16_t*)x)
 


void fatal_exception_handler(struct extended_exception_frame* eh){
    /*myprintf("EXCEPTION %d\n", eh->cause);
    myprintf(" r0=%8x r1=%8x  r2=%8x  r3=%8x\n", eh->r0, eh->r1, eh->r2, eh->r3);
    myprintf(" r4=%8x r5=%8x  r6=%8x  r7=%8x\n", eh->r4, eh->r5, eh->r6, eh->r7);
    myprintf(" r8=%8x r9=%8x r10=%8x r11=%8x\n", eh->r8, eh->r9, eh->r10, eh->r11);
    myprintf("r12=%8x sp=%8x  lr=%8x  pc=%8x\n", eh->r12, eh->sp, eh->lr, eh->pc);*/

    //asm("bx %0"::"r"(0xFFFFFFFC));
   // while(1);
}


/*void arg_svc(uint8_t imm,uint8_t imm2){ 
     asm("nop");
     for (int i = 0; i < 4; ++i)
     {
          myprintf("%x",read(i));
     }
     asm("nop");
     myprintf("%x  %x\n",imm,imm2);
}*/

void svcall_handler ()  { 
  
  //myprintf("%8x\n",read(pc));
  
 
  ////myprintf("%4x  , %8x\n",read(pc-2),pc-2);
  //myprintf("%4x  , %8x\n",read(pc-4),pc-4);
  //myprintf("%4x  , %8x\n",read(0x2da),0x2da);

  /*int i=0;
  void *j;
  
  asm("mov %0,r0" : "=r"(i));
  asm("mov %0,r11" : "=r"(j));
  myprintf("svcall_handler");
  myprintf("%i  , %s\n",i,(char*)j);*/
  
}

 

void pendsv_handler (void)  { 
  //myprintf("pendsv_handler\n"); 
}

uint32_t get_tick(){
  return count;
}

void counter(){
count++;
}

void systick_handler (void) { 
  counter();
}

/******************************************************************************
 * Forward undefined IRQ handlers to an infinite loop function. The Handlers
 * are weakly aliased which means that (re)definitions will overide these.
 *****************************************************************************/

// Prototype the entry values, which are handled by the linker script
extern void* stack_entry;
extern void boot_entry(void);

// Defined irq vectors using simple c code following the description in a white 
// paper from ARM[3] and code example from Simonsson Fun Technologies[4].
// These vectors are placed at the memory location defined in the linker script
void *vectors[] SECTION(".irq_vectors") =
{
  // Stack and program reset entry point
  &stack_entry,          // The initial stack pointer
  boot_entry,            // The reset handler
  // Various fault handlers
  irq_undefined,         // The NMI handler
  fault_handler,         // The hard fault handler
  fault_handler,         // MemManage_Handler
  fault_handler,         // BusFault_Handler
  fault_handler,         // UsageFault_Handler
  0,                     // Reserved
  0,                     // Reserved
  0,                     // Reserved
  0,                     // Reserved
  svcall_handler,         // SVCall handler
  irq_undefined,         // DebugMon_Handler
  0,                     // Reserved
  pendsv_handler,         // The PendSV handler
  systick_handler,         // The SysTick handler
  
  // Wakeup I/O pins handlers
  irq_undefined,     // PIO0_0  Wakeup
  irq_undefined,     // PIO0_1  Wakeup
  irq_undefined,     // PIO0_2  Wakeup
  irq_undefined,     // PIO0_3  Wakeup
  irq_undefined,     // PIO0_4  Wakeup
  irq_undefined,     // PIO0_5  Wakeup
  irq_undefined,     // PIO0_6  Wakeup
  irq_undefined,     // PIO0_7  Wakeup
  irq_undefined,     // PIO0_8  Wakeup
  irq_undefined,     // PIO0_9  Wakeup
  irq_undefined,     // PIO0_10 Wakeup
  irq_undefined,     // PIO0_11 Wakeup
  irq_undefined,     // PIO1_0  Wakeup
  
  // Specific peripheral irq handlers
  irq_undefined,
  irq_undefined,
  irq_undefined,
  irq_undefined,
  irq_undefined,
  irq_undefined,
  irq_undefined,
  irq_undefined,
  irq_undefined,
  irq_undefined,
  irq_undefined,
  irq_undefined,
  irq_undefined,
  irq_undefined,
  irq_undefined,
  irq_undefined,
  irq_undefined,
  irq_undefined,
  irq_undefined,
};
