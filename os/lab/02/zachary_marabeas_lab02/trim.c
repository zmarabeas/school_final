#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#define BUFFER_SIZE 4096

int main(int argc, char *argv[]) {
    if (argc != 5) {
        fprintf(stderr, "Usage: %s m n source.txt destination.txt\n", argv[0]);
        return 1;
    }

    int start_line = atoi(argv[1]);
    int end_line = atoi(argv[2]);

    if (start_line < 0 || end_line < start_line) {
        fprintf(stderr, "Error: Invalid line range. m=%d, n=%d\n", start_line, end_line);
        return 1;
    }

    int source_fd, dest_fd;
    char buffer[BUFFER_SIZE];
    ssize_t bytes_read;
    int current_line = 0;

    // open source file
    source_fd = open(argv[3], O_RDONLY);
    if (source_fd == -1) {
        perror("open source");
        return 1;
    }

    // open destination file
    dest_fd = open(argv[4], O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (dest_fd == -1) {
        perror("open dest");
        close(source_fd);
        return 1;
    }

    // read and process line by line
    while ((bytes_read = read(source_fd, buffer, BUFFER_SIZE)) > 0) {
        for (int i = 0; i < bytes_read; i++) {
            // write character if in target line range
            if (current_line >= start_line && current_line <= end_line) {
                if (write(dest_fd, &buffer[i], 1) != 1) {
                    perror("write");
                    close(source_fd);
                    close(dest_fd);
                    return 1;
                }
            }

            // count lines
            if (buffer[i] == '\n') {
                current_line++;
                if (current_line > end_line) {
                    goto done;
                }
            }
        }
    }

done:
    // check for read errors
    if (bytes_read == -1) {
        perror("read");
        close(source_fd);
        close(dest_fd);
        return 1;
    }

    // close files
    if (close(source_fd) == -1) {
        perror("close source");
        close(dest_fd);
        return 1;
    }

    if (close(dest_fd) == -1) {
        perror("close dest");
        return 1;
    }

    return 0;
}