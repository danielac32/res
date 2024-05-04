asm("li sp,0x000595ae   \n\t"
    "li gp,0x000595af   \n\t"
    "j main              \n\t"
);
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>

#include "lib/timer.h"
#include "lib/lib.h"
#include "lib/serial.h"
#include "lib/syscall.h"
#include "lib/task.h"
#include "lib/terminal.h"
#include "lib/cmd.h"


void main(){
      
	myprintf("init kernel 0.0.0.1\n");
    myprintf ("Build date: %s %s\n\n", __DATE__, __TIME__);
	int pid;

     
	ENABLE_TIMER=1;
     MODE_CPU=0;

     memset(mem, 0, sizeof(mem));
     chunklist_init((chunk_t*)mem, sizeof(mem));
     info((chunk_t*)mem);
     memset(__buffer__, 0, sizeof(__buffer__));
     chunklist_init((chunk_t*)__buffer__, sizeof(__buffer__));
     info((chunk_t*)__buffer__);
    
	 init_task();
     process_attach("terminal", 1, process0,&pid,0);
     process_run(pid);
     //process_attach("null", 100, process1,&pid,0);
     //process_run(pid);
     myprintf("terminal enable\n");
     scheduler();
}