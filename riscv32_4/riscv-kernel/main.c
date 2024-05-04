
asm("li sp,89997  \n\t"
    "li gp,89998   \n\t"
    "j main         \n\t"
);

#include <stdint.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>


#include "lib/timer.h"
#include "lib/lib.h"
#include "lib/serial.h"
#include "lib/syscall.h"
#include "lib/terminal.h"
#include "lib/klib.h"
#include "lib/task.h"
#include "lib/cmd.h"


/*
static unsigned char rawData[246] = {
    0x6F, 0x00, 0xC0, 0x08, 0x13, 0x01, 0x01, 0xFE, 0x23, 0x2E, 0x81, 0x00,
    0x13, 0x04, 0x01, 0x02, 0x93, 0x07, 0x05, 0x00, 0xA3, 0x07, 0xF4, 0xFE,
    0xB7, 0xA7, 0x0F, 0x00, 0x93, 0x87, 0x27, 0x00, 0x03, 0x47, 0xF4, 0xFE,
    0x23, 0x80, 0xE7, 0x00, 0x13, 0x00, 0x00, 0x00, 0x03, 0x24, 0xC1, 0x01,
    0x13, 0x01, 0x01, 0x02, 0x67, 0x80, 0x00, 0x00, 0x13, 0x01, 0x01, 0xFE,
    0x23, 0x2E, 0x11, 0x00, 0x23, 0x2C, 0x81, 0x00, 0x13, 0x04, 0x01, 0x02,
    0x23, 0x26, 0xA4, 0xFE, 0x6F, 0x00, 0x00, 0x02, 0x83, 0x27, 0xC4, 0xFE,
    0x13, 0x87, 0x17, 0x00, 0x23, 0x26, 0xE4, 0xFE, 0x83, 0xC7, 0x07, 0x00,
    0x13, 0x85, 0x07, 0x00, 0x97, 0x00, 0x00, 0x00, 0xE7, 0x80, 0x00, 0xFA,
    0x83, 0x27, 0xC4, 0xFE, 0x83, 0xC7, 0x07, 0x00, 0xE3, 0x9E, 0x07, 0xFC,
    0x13, 0x00, 0x00, 0x00, 0x83, 0x20, 0xC1, 0x01, 0x03, 0x24, 0x81, 0x01,
    0x13, 0x01, 0x01, 0x02, 0x67, 0x80, 0x00, 0x00, 0x13, 0x01, 0x01, 0xFE,
    0x23, 0x2E, 0x11, 0x00, 0x23, 0x2C, 0x81, 0x00, 0x13, 0x04, 0x01, 0x02,
    0x97, 0x07, 0x00, 0x00, 0x93, 0x87, 0x07, 0x05, 0x23, 0x24, 0xF4, 0xFE,
    0x23, 0x26, 0x04, 0xFE, 0x6F, 0x00, 0xC0, 0x01, 0x03, 0x25, 0x84, 0xFE,
    0x97, 0x00, 0x00, 0x00, 0xE7, 0x80, 0x40, 0xF8, 0x83, 0x27, 0xC4, 0xFE,
    0x93, 0x87, 0x17, 0x00, 0x23, 0x26, 0xF4, 0xFE, 0x03, 0x27, 0xC4, 0xFE,
    0x93, 0x07, 0x90, 0x00, 0xE3, 0xD0, 0xE7, 0xFE, 0x13, 0x00, 0x00, 0x00,
    0x83, 0x20, 0xC1, 0x01, 0x03, 0x24, 0x81, 0x01, 0x13, 0x01, 0x01, 0x02,
    0x67, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x63, 0x70, 0x75, 0x20,
    0x74, 0x65, 0x73, 0x74, 0x0A, 0x00
};
*/

//extern uint32_t _start_data,_end_data,_text_start;

void main()
{ 
  
    /*void (*p)(void);
    p=(void*)rawData;
    (*p)();*/
 
    
	myprintf("init kernel 0.0.0.1\n");
    myprintf ("Build date: %s %s  (%i)\n\n", __DATE__, __TIME__,mcu);
	int pid;

    //myprintf("text start: %8x, data start: %8x, data end: %8x\n",(uint32_t)&_text_start,(uint32_t)&_start_data,(uint32_t)&_end_data);


    mode=0;
    enable_timer=1;

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

    while(1);

}

