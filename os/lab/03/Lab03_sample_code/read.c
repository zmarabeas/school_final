/* Showing what happens when you don't NULL terminate. */

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

int main()
{
  char c[10];
  char sub_result[10];
  int fd, sz;

//  strcpy(c, "ABCDEFGHIJKLMNOPQRSTUVWXYZ");
  fd = open("value.txt", O_RDONLY);
  if (fd < 0) { perror("r1"); exit(1); }

  sz = read(fd, c, 10);         
  printf("%s\n", c);          
  sprintf(sub_result, "%d", atoi(c) - 5);
  printf("Substracting 5 from the value we get,  %s \n", sub_result);
  
  return 0;
}
