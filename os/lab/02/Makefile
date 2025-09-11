CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -pedantic -O2
TARGETS = show trim

.PHONY: all clean test

all: $(TARGETS)

show: show.c
	$(CC) $(CFLAGS) -o show show.c

trim: trim.c
	$(CC) $(CFLAGS) -o trim trim.c

clean:
	rm -f $(TARGETS) output*.txt

test: all
	@echo "=== Testing show program ==="
	@echo "Displaying contents of text.txt:"
	./show text.txt
	@echo "\n=== Testing trim program ==="
	@echo "Extracting lines 1-3 to output1.txt:"
	./trim 1 3 text.txt output1.txt
	@echo "Contents of output1.txt:"
	./show output1.txt
	@echo "\nExtracting lines 0-2 to output2.txt:"
	./trim 0 2 text.txt output2.txt
	@echo "Contents of output2.txt:"
	./show output2.txt
	@echo "\nExtracting lines 4-6 to output3.txt:"
	./trim 4 6 text.txt output3.txt
	@echo "Contents of output3.txt:"
	./show output3.txt

test-errors: all
	@echo "=== Testing error cases ==="
	@echo "Testing show with nonexistent file:"
	-./show nonexistent.txt
	@echo "\nTesting trim with invalid range:"
	-./trim 5 3 text.txt output_bad.txt
	@echo "\nTesting trim with nonexistent source:"
	-./trim 1 2 nonexistent.txt output_bad.txt

.SUFFIXES: