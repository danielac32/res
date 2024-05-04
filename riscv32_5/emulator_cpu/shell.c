#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>

/*
Use these colors to print colored text on the console
*/
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_RESET   "\x1b[0m"

/*
use with redirection(< > >>) to indicate to the function in which mode to open the file
and to know that redirection of the input OR output has to be done
*/
#define INPUT 0
#define OUTPUT 1
#define APPEND 2

/*
removes the newline and space character from the end and start of a char*
*/
void removeWhiteSpace(char* buf){
	if(buf[strlen(buf)-1]==' ' || buf[strlen(buf)-1]=='\n')
	buf[strlen(buf)-1]='\0';
	if(buf[0]==' ' || buf[0]=='\n') memmove(buf, buf+1, strlen(buf));
}

/*
tokenizes char* buf using the delimiter c, and returns the array of strings in param
and the size of the array in pointer nr
*/
void tokenize_buffer(char** param,int *nr,char *buf,const char *c){
	char *token;
	token=strtok(buf,c);
	int pc=-1;
	while(token){
		param[++pc]=malloc(sizeof(token)+1);
		strcpy(param[pc],token);
		removeWhiteSpace(param[pc]);
		token=strtok(NULL,c);
	}
	param[++pc]=NULL;
	*nr=pc;
}

/*
used for debugging purposes to print all the strings in a string array
*/
void print_params(char ** param){
	while(*param){
		printf("param=%s..\n",*param++);
	}
}

/*
loads and executes a single external command
*/
void executeBasic(char** argv){
	if(fork()>0){
		//parent
		wait(NULL);
	}
	else{
		//child
		execvp(argv[0],argv);
		//in case exec is not successfull, exit
		perror(ANSI_COLOR_RED   "invalid input"   ANSI_COLOR_RESET "\n");
		exit(1);
	}
}

/*
loads and executes a series of external commands that are piped together
*/
void executePiped(char** buf,int nr){//can support up to 10 piped commands
	if(nr>10) return;
	
	int fd[10][2],i,pc;
	char *argv[100];

	for(i=0;i<nr;i++){
		tokenize_buffer(argv,&pc,buf[i]," ");
		if(i!=nr-1){
			if(pipe(fd[i])<0){
				perror("pipe creating was not successfull\n");
				return;
			}
		}
		if(fork()==0){//child1
			if(i!=nr-1){
				dup2(fd[i][1],1);
				close(fd[i][0]);
				close(fd[i][1]);
			}

			if(i!=0){
				dup2(fd[i-1][0],0);
				close(fd[i-1][1]);
				close(fd[i-1][0]);
			}
			execvp(argv[0],argv);
			perror("invalid input ");
			exit(1);//in case exec is not successfull, exit
		}
		//parent
		if(i!=0){//second process
			close(fd[i-1][0]);
			close(fd[i-1][1]);
		}
		wait(NULL);
	}
}

/*
loads and executes a series of external commands that have to be run asyncronously
*/
void executeAsync(char** buf,int nr){
	int i,pc;
	char *argv[100];
	for(i=0;i<nr;i++){
		tokenize_buffer(argv,&pc,buf[i]," ");
		if(fork()==0){
			execvp(argv[0],argv);
			perror("invalid input ");
			exit(1);//in case exec is not successfull, exit
		}
	}
	for(i=0;i<nr;i++){
		wait(NULL);
	}

}

/*
loads and executes a an external command that needs file redirection(input, output or append)
*/
void executeRedirect(char** buf,int nr,int mode){
	int pc,fd;
	char *argv[100];
	removeWhiteSpace(buf[1]);
	tokenize_buffer(argv,&pc,buf[0]," ");
	if(fork()==0){

		switch(mode){
		case INPUT:  fd=open(buf[1],O_RDONLY); break;
		case OUTPUT: fd=open(buf[1],O_WRONLY); break;
		case APPEND: fd=open(buf[1],O_WRONLY | O_APPEND); break;
		default: return;
		}

		if(fd<0){
			perror("cannot open file\n");
			return;
		}

		switch(mode){
		case INPUT:  		dup2(fd,0); break;
		case OUTPUT: 		dup2(fd,1); break;
		case APPEND: 		dup2(fd,1); break;
		default: return;
		}
		execvp(argv[0],argv);
		perror("invalid input ");
		exit(1);//in case exec is not successfull, exit
	}
	wait(NULL);
}

/*
shows the internal help
*/
 

int main(char** argv,int argc)
{
	char buf[500],*buffer[100],buf2[500],buf3[500], *params1[100],*params2[100],*token,cwd[1024];
	int nr=0;
	printf(   "*****************************************************************\n");
	printf(   "**************************CUSTOM SHELL***************************\n");

	while(1){
	 
		    fgets(buf, 500, stdin);//buffer overflow cannot happen
//single command including internal ones
			tokenize_buffer(params1,&nr,buf," ");
			 
			if(strstr(params1[0],"help")){//help builtin command
			   printf("%s %s\n",params1[1],params1[2] );
			}
			else if(strstr(params1[0],"exit")){//exit builtin command
				exit(0);
			}else printf("error\n");
			//else executeBasic(params1);
		}

	return 0;
}