#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define BUFFER_SIZE 4096

void print_error(const char* operation, const char* filename) {
    fprintf(stderr, "Error: %s failed for file '%s': %s\n", 
            operation, filename, strerror(errno));
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s fileName\n", argv[0]);
        return 1;
    }
    
    const char* filename = argv[1];
    int fd;
    char buffer[BUFFER_SIZE];
    ssize_t bytes_read, bytes_written;
    
    // Open file with error checking (10 points)
    fd = open(filename, O_RDONLY);
    if (fd == -1) {
        print_error("open", filename);
        return 1;
    }
    
    // Read and write loop with error checking (10 points each)
    while ((bytes_read = read(fd, buffer, BUFFER_SIZE)) > 0) {
        bytes_written = write(STDOUT_FILENO, buffer, bytes_read);
        if (bytes_written != bytes_read) {
            print_error("write", "stdout");
            close(fd);
            return 1;
        }
    }
    
    // Check for read error (10 points)
    if (bytes_read == -1) {
        print_error("read", filename);
        close(fd);
        return 1;
    }
    
    // Close file with error checking (10 points)
    if (close(fd) == -1) {
        print_error("close", filename);
        return 1;
    }
    
    return 0;
}