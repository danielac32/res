

#include <stdio.h>
#include <stdlib.h>
void hexDump(char *desc, void *addr, int len) 
{
    int i;
    unsigned char buff[17];
    unsigned char *pc = addr;

    for (i = 0; i < len; i++) {
        // Multiple of 16 means new line (with line offset).

        if ((i % 16) == 0) {
            // Just don't print ASCII for the zeroth line.
            if (i != 0)
                printf ("  %s\n", buff);

            // Output the offset.
            printf ("  %04x ", i);
        }

        // Now the hex code for the specific character.
        printf (" %02x", pc[i]);

        // And store a printable ASCII character for later.
        if ((pc[i] < 0x20) || (pc[i] > 0x7e))
            buff[i % 16] = '.';
        else
            buff[i % 16] = pc[i];
        buff[(i % 16) + 1] = '\0';
    }

    // Pad out last line if not exactly 16 characters.
    while ((i % 16) != 0) {
        printf ("   ");
        i++;
    }

    // And print the final ASCII bit.
    printf ("  %s\n", buff);
}


 

int main (int argc, char *argv[]) 
{
    char my_str[] = "a char string greater than 16 chars";
    printf("%s\n", my_str);
    char *p;

    FILE * file = NULL;
    unsigned long lsize = 0;
 
    file = fopen(argv[1], "rb");
    if(file == NULL){
       printf("Could not open file!");
       exit(-1);
    }
    fseek(file, 0, SEEK_END);
    lsize = ftell(file);
    rewind(file);

    p=(char*)malloc(lsize*sizeof(char));

    fread(p, sizeof(char), lsize, file);
    fclose(file);

    hexDump ("my_str", p, lsize);
    return 0;
}