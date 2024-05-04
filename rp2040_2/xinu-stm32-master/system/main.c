/*  main.c  - main */
// This is user land

#include <xinu.h>

process	main(void)
{
	while(1){
    	 printf("\t\t\t\t\t\t\thola como estas\n");
    	 sleepms(4);
    }
}

/*
	recvclr();
	//resume(create(shell, 4096, 50, "shell", 1, 0));
	
	 

	while (TRUE) {
		receive();
		//kprintf("Entering sleep");
		sleepms(200);
		printf("\n\nMain process recreating shell\n\n");
		///resume(create(shell, 4096, 20, "shell", 1, CONSOLE));
	}
	return OK;*/
