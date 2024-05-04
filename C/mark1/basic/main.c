/* A tiny BASIC interpreter */
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <setjmp.h>
//#include <math.h>
#include <ctype.h>
#include <stdlib.h>

#define NUM_LAB 100
#define LAB_LEN 10
#define FOR_NEST 25
#define SUB_NEST 25
#define PROG_SIZE 2048

#define DELIMITER  1
#define VARIABLE  2
#define NUMBER    3
#define COMMAND   4
#define STRING    5
#define QUOTE   6

#define PRINT 1
#define INPUT 2
#define IF    3
#define THEN  4
#define FOR   5
#define NEXT  6
#define TO    7
#define GOTO  8
#define EOL   9
#define FINISHED  10
#define GOSUB 11
#define RETURN 12
#define END 13
#define DELAY_MS 14
#define PINMODE 15
#define DWRITE 16
#define DREAD 17
#define AREAD 18
#define AWRITE 19
#define DELAY_US 20
#define PRINTLN 21
#define TOGGLE 22
#define ELSE 23


char *prog;  
char token[80];
char token_type, tok;



void find_eol()
{
  while(*prog!='\n'  && *prog!='\0') ++prog;
  if(*prog) prog++;
}

int isdelim(char c)
{
  if(strchr(" ;,+-<>/*%^=()", c) || c==9 || c=='\r' || c==0)
    return 1;
  return 0;
}

 
int iswhite(char c)
{
  if(c==' ' || c=='\t') return 1;
  else return 0;
}


int get_token(){
   char *temp;

  token_type=0; tok=0;
  temp=token;

  if(*prog=='\0') { /* end of file */
    *token=0;
    tok = FINISHED;
    return(token_type=DELIMITER);
  }

  while(iswhite(*prog)) ++prog;  /* skip over white space */

  if(*prog=='\r') { /* crlf */
    ++prog; ++prog;
    tok = EOL; *token='\r';
    token[1]='\n'; token[2]=0;
    return (token_type = DELIMITER);
  }

  if(strchr("+-*^/%=;(),><", *prog)){ /* delimiter */
    *temp=*prog;
    prog++; /* advance to next position */
    temp++;
    *temp=0;
    return (token_type=DELIMITER);
  }

  if(*prog=='"') { /* quoted string */
    prog++;
    while(*prog!='"'&& *prog!='\r') *temp++=*prog++;
    if(*prog=='\r') //serror(1);
    prog++;*temp=0;
    return(token_type=QUOTE);
  }

  if(isdigit(*prog)) { /* number */
    while(!isdelim(*prog)) *temp++=*prog++;
    *temp = '\0';
    return(token_type = NUMBER);
  }

  if(isalpha(*prog)) { /* var or command */
    while(!isdelim(*prog)) *temp++=*prog++;
    token_type=STRING;
  }
  *temp = '\0';
 
/*
  if(token_type==STRING) {
    tok=look_up(token); 
    if(!tok) token_type = VARIABLE;
    else token_type = COMMAND;  
  }*/

  return token_type;
}


 
void putback()
{

  char *t;

  t = token;
  for(; *t; t++) prog--;
}



 

int load_program(char *p, char *fname){
FILE *fp;
uint32_t lsize = 0;
fp = fopen(fname, "rb");
if(fp == NULL){
   printf("file not foud\n");
   exit(-1);
}
fseek(fp, 0, SEEK_END);
lsize = ftell(fp);

//p=(char *)calloc(lsize+1,sizeof(char));//buffer para guardar xml original

if(p==NULL){
  printf("error malloc\n");
  exit(0);
}
 
if(lsize<=999){
  printf("size file: %i Bytes\n", lsize);
}else if(lsize >=1000 && lsize<1000000){
  printf("size file: %i kb\n", lsize/1000);
}else if(lsize>=1000000){
  printf("size file: %i mb\n", lsize/1000000);
}
 
rewind(fp);
fread(p, sizeof(char), lsize, fp);
fclose(fp);
  return 1;
}

int main(int argc, char *argv[]){
  char p_buf[PROG_SIZE];
  
  memset(p_buf,0,sizeof(p_buf));
  printf("Virtual Machine Basic language (Interpreter)\n\r");
  
  load_program(p_buf,argv[1]);
  prog = p_buf;
  for (int i = 0; i < 20; ++i)
  {
     get_token();
  printf("%s\n",token );
  }
  
}





/**************************************************/






