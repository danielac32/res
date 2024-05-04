


#ifndef KMALLOC_H
#define KMALLOC_H

 char mem[150000];
#define KMALLOC_START  (void*)mem
#define KMALLOC_LENGTH sizeof(mem)
void *kmalloc(int length);
void kfree(void *ptr);

void kmalloc_init(void *start, int length);
void kmalloc_debug();
int kmalloc_test();


#define KUNIT sizeof(struct kmalloc_chunk)

#define KMALLOC_STATE_FREE 0xa1a1a1a1
#define KMALLOC_STATE_USED 0xbfbfbfbf

struct kmalloc_chunk {
	int state;
	int length;
	struct kmalloc_chunk *next;
	struct kmalloc_chunk *prev;
};

static struct kmalloc_chunk *head = 0;


#endif