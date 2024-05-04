#include <stdint.h>
#include <string.h>

#ifndef SHELL_H_
#define SHELL_H_

void removeWhiteSpace(char* buf);
void tokenize_buffer(char** param,int *nr,char *buf,const char *c);
void init_shell();
uint8_t get_cmd();

#endif