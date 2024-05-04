
asm("j main");
#include "../lib/nwlib.h"



void main(int pid)
{
	kill_task(pid);
    myprintf("killed\n");
}