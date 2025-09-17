#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#define BUFFER_SIZE 4096

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s fileName\n", argv[0]);
        return 1;
    }

    int fd;
    char buffer[BUFFER_SIZE];
    ssize_t bytes_read, bytes_written;

    // open file
    fd = open(argv[1], O_RDONLY);
    if (fd == -1) {
        perror("open");
        return 1;
    }

    // read and write loop
    while ((bytes_read = read(fd, buffer, BUFFER_SIZE)) > 0) {
        bytes_written = write(STDOUT_FILENO, buffer, bytes_read);
        if (bytes_written != bytes_read) {
            perror("write");
            close(fd);
            return 1;
        }
    }

    // check for read errors
    if (bytes_read == -1) {
        perror("read");
        close(fd);
        return 1;
    }

    // close file
    if (close(fd) == -1) {
        perror("close");
        return 1;
    }

    return 0;
}
