/* This program opens the file "value.txt" in the current directory, and prints out the
   return value of the open() system call.  If "value.txt" exists, open() will return a
   non-negative integer (three).  If "value.txt" does not exist, then it will return -1. */

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>

int main()
{
  int fd;

 // O_RDONLY ||  O_WRONLY | O_CREAT | O_TRUNC, 0644
  

  if ((fd = open("value.txt", O_RDONLY)) > 0)
	printf("Success \n");
  else
	printf("Failed \n");

  printf("%d\n", fd);

  return 0;
}
