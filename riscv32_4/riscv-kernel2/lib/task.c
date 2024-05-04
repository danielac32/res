#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stddef.h>
#include <string.h>

#include "task.h"
#include "lib.h"
#include "serial.h"
#include "kmalloc.h"
 
void init_task(){

}
void createtask(int prio, void (*function)(int,uint32_t),int *pid,size_t len){

}
int killtask(int pid){
  
}
void list_process(){
  
}
void runtask(int pid){
  
}
void modifytask(int prio, void (*function)(int,uint32_t),int pid){
  
} 
int scheduler(){
  
}

int lauch_exec(int case_,char *url,char *value){
  
}

 