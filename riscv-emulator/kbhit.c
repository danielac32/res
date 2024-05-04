#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>

int _kbhit() {
struct timeval tv;
fd_set rdfs;
tv.tv_sec=0;
tv.tv_usec=0;
FD_ZERO(&rdfs);
FD_SET(STDIN_FILENO,&rdfs);
select(STDIN_FILENO+1,&rdfs,NULL,NULL,&tv);
return FD_ISSET(STDIN_FILENO,&rdfs);
}

//////////////////////////////////////////////
//    Simple demo of _kbhit()

#include <unistd.h>

int main(int argc, char** argv) {
    printf("Press any key");
    while (! _kbhit()) {
        printf(".");
       
    }
    printf("\nDone.\n");

    return 0;
} 