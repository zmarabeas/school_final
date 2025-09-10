CC = gcc
CFLAGS = -Wall -Wextra -std=c99
TARGET = csc4420_shell
SOURCE = csc4420_shell.c

all: $(TARGET)

$(TARGET): $(SOURCE)
	$(CC) $(CFLAGS) -o $(TARGET) $(SOURCE)

clean:
	rm -f $(TARGET)

run: $(TARGET)
	./$(TARGET)

.PHONY: all clean run