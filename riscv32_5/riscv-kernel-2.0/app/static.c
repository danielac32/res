

asm("j main");

#include <stddef.h>
#include <stdint.h>
#include "../include/reg.h"
#include "../include/serial.h"
#include "../include/syscall.h"
 
int counter=0;
int store=99;
char *str="veronica";
char *str3="daniel";
char *ptr="global offset table";
uint8_t *ptr2="1234567890";


char *p;
char str2[]={"kjkjglkj"};

 char buffer[1000];

void main(){
    void *context;
    int pid=get_id();
    void *arg=get_args(pid);
   
    memset(buffer,0,1000);
    myprintf("%s\n",str);
    myprintf("%s\n",str3);
    myprintf("%s\n",ptr);
    myprintf("%s\n",ptr2);
     while(1){
        myprintf("%i,%i,%s,%s\n",counter,store,str,p);
        counter++;
        store++;

        context=get_context();
        k_yield(context);
     }
}