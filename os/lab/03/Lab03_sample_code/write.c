/* This program opens the file "value.txt" in the current directory 
   for writing, and writes the string "12345" to it. */

#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

int main()
{
  int fd, sz;

  fd = open("value.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
  if (fd < 0) { perror("value.txt"); exit(1); }

  sz = write(fd, "12345\n", strlen("12345\n"));
  
  printf("called write.  it returned the string length as %d\n", sz);

  close(fd);
  return 0;
}
