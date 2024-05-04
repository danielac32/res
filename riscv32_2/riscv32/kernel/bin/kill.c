
asm("j main");
#include "../lib/applib.h"



void main(int pid)
{
	kill_task(pid);
    myprintf("killed\n");
}