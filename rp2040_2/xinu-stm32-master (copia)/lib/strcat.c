
#include <xinu.h>
char *strcat(char *dest, const char *src) {
	int i;
	for (i = 0; dest[i]; ++i);
	strcpy(&dest[i], src);
	return dest;
}