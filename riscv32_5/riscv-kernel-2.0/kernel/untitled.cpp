//Simple C program to encrypt and decrypt a string

#include <stdio.h>


char str[100];
int main()
{
   strcpy(str,"daniel");
   int i=0;

   
      for(i = 0; (i < 100 && str[i] != '\0'); i++)
        str[i] = str[i] + 3; //the key for encryption is 3 that is added to ASCII value

      printf("\nEncrypted string: %s\n", str);


 
      for(i = 0; (i < 100 && str[i] != '\0'); i++)
        str[i] = str[i] - 3; //the key for encryption is 3 that is subtracted to ASCII value

      printf("\nDecrypted string: %s\n", str);
      
   
   return 0;
}
