
#include <os.h>

void panic(char * msg) {
     
     while(1){
     	printf("%s\n",msg);
     	gpio_set(25, 0);
        delay(10000);
        gpio_set(25, 1);
        delay(10000);
     }
}
