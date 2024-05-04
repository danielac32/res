#include <os.h>
#ifndef MALLOC_H_
#define MALLOC_H_
 
 

/**
 * initializes the memory management subsystem
 */
void mm_init(void);
 
void* malloc(size_t size);
 
void free(void *ptr);
#endif