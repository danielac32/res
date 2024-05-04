#include <stddef.h>
#include <stdint.h>
#include "reg.h"
#include "kmalloc.h"
#include "shell.h"
#include "serial.h"


typedef struct CMD{
   char buff[50];
   char get;
   uint8_t count;
   int argc;
   char *params1[10];
}cmd_t;
cmd_t *shell;


void init_shell(){
	shell=(cmd_t *)kmalloc(sizeof(cmd_t));
  if(shell==NULL)myprintf("error malloc\n");
}
void removeWhiteSpace(char* buf){
	if(buf[strlen(buf)-1]==' ' || buf[strlen(buf)-1]=='\n')
	buf[strlen(buf)-1]='\0';
	if(buf[0]==' ' || buf[0]=='\n') memmove(buf, buf+1, strlen(buf));
}

void tokenize_buffer(char** param,int *nr,char *buf,const char *c){
  char *token;
	token=strtok(buf,c);
	int pc=-1;
	while(token){
		param[++pc]=kmalloc(sizeof(token)+1);
		strcpy(param[pc],token);
		removeWhiteSpace(param[pc]);
		token=strtok(NULL,c);
	}
	param[++pc]=NULL;
	*nr=pc;
}

uint8_t get_cmd(int *argc, char *argv[]){
	if(ENRX){
       shell->get=RX;//SerialRead();
       if(shell->count<45){
          shell->buff[shell->count]=shell->get;
          shell->count++;
          shell->buff[shell->count]='\0';
       }else if(shell->buff[shell->count]==0x08){
       	  shell->count--;
       	  shell->count--;
       	  shell->buff[shell->count]='\0';
       }  
       if(shell->buff[shell->count]=='\0' && shell->buff[shell->count-1]=='\n'){
           shell->count=0;
           tokenize_buffer(shell->params1,&shell->argc,shell->buff," ");
		       for (int i = 0; i < shell->argc; ++i)
		       {
		   	        argv[i]=shell->params1[i];
		       }
		       *argc=shell->argc;
          /* if(strstr(shell->params1[0],"help")){//help builtin command
           	  for (int i = 1; i < shell->argc; ++i)
           	  {
           	  	myprintf("%s ",shell->params1[i]);
           	  }
	          myprintf("\n");
           }*/
           memset(shell->buff,0,sizeof(shell->buff));
          /* for (int i = 0; i < shell->argc; ++i)
           {
                kfree(shell->params1[i]);
           }*/
           return 1;
        }
        return 0;
    }
}
void get_param(int argc, char *argv[]){

}
 
/*
if(ENRX){
       shell->get=RX;//SerialRead();
       if(shell->count<45){
          shell->buff[shell->count]=shell->get;
          shell->count++;
          shell->buff[shell->count]='\0';
       }else if(shell->buff[shell->count]==0x08){
       	  shell->count--;
       	  shell->count--;
       	  shell->buff[shell->count]='\0';
       }  
        if(shell->buff[shell->count]=='\0' && shell->buff[shell->count-1]=='\n'){
           shell->count=0;

           tokenize_buffer(shell->params1,&shell->argc,shell->buff," ");
			 
           if(strstr(shell->params1[0],"help")){//help builtin command
           	  for (int i = 1; i < shell->argc; ++i)
           	  {
           	  	myprintf("%s ",shell->params1[i]);
           	  }
	          myprintf("\n");
           }

           //myprintf("%s\n",shell->buff);
           memset(shell->buff,0,sizeof(shell->buff));

        }
    }
*/