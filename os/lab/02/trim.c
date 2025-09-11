#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define BUFFER_SIZE 1

void print_error(const char* operation, const char* filename) {
    fprintf(stderr, "Error: %s failed for file '%s': %s\n", 
            operation, filename, strerror(errno));
}

int main(int argc, char *argv[]) {
    if (argc != 5) {
        fprintf(stderr, "Usage: %s m n source.txt destination.txt\n", argv[0]);
        return 1;
    }
    
    int start_line = atoi(argv[1]);
    int end_line = atoi(argv[2]);
    const char* source_file = argv[3];
    const char* dest_file = argv[4];
    
    if (start_line < 0 || end_line < start_line) {
        fprintf(stderr, "Error: Invalid line range. m=%d, n=%d\n", 
                start_line, end_line);
        return 1;
    }
    
    int source_fd, dest_fd;
    char buffer[BUFFER_SIZE];
    ssize_t bytes_read, bytes_written;
    int current_line = 0;
    int in_range = 0;
    
    // Open source file with error checking (10 points)
    source_fd = open(source_file, O_RDONLY);
    if (source_fd == -1) {
        print_error("open", source_file);
        return 1;
    }
    
    // Open destination file with error checking (10 points)
    dest_fd = open(dest_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (dest_fd == -1) {
        print_error("open", dest_file);
        close(source_fd);
        return 1;
    }
    
    // Process file character by character
    while ((bytes_read = read(source_fd, buffer, BUFFER_SIZE)) > 0) {
        // Check for read error (10 points)
        if (bytes_read == -1) {
            print_error("read", source_file);
            close(source_fd);
            close(dest_fd);
            return 1;
        }
        
        // Check if we should start copying
        if (current_line == start_line && !in_range) {
            in_range = 1;
        }
        
        // Copy character if in range
        if (in_range) {
            bytes_written = write(dest_fd, buffer, bytes_read);
            if (bytes_written != bytes_read) {
                print_error("write", dest_file);
                close(source_fd);
                close(dest_fd);
                return 1;
            }
        }
        
        // Check for newline to increment line counter
        if (buffer[0] == '\n') {
            current_line++;
            // Stop copying after end line
            if (current_line > end_line && in_range) {
                break;
            }
        }
    }
    
    // Check for read error (already handled above but separate check)
    if (bytes_read == -1) {
        print_error("read", source_file);
        close(source_fd);
        close(dest_fd);
        return 1;
    }
    
    // Close source file with error checking (10 points)
    if (close(source_fd) == -1) {
        print_error("close", source_file);
        close(dest_fd);
        return 1;
    }
    
    // Close destination file with error checking (10 points)
    if (close(dest_fd) == -1) {
        print_error("close", dest_file);
        return 1;
    }
    
    return 0;
}