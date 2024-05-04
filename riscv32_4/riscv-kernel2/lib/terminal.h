#include <stdint.h>

#ifndef TERMINAL_H
#define TERMINAL_H
struct serial{
   char indata[50];
   int index;
   char inChar;
   char *pch;
   int pid;
};

struct serial key;

char token[100];
char *prog;
char token_type, tok;
#define DELIMITER  1
#define VARIABLE  2
#define NUMBER    3
#define COMMAND   4
#define STRING    5
#define QUOTE   6
#define EOL   9
#define FINISHED  10
int isdelim(char c);
int iswhite(char c);
int get_token();


#endif