CC = gcc
CFLAGS = -I. -g -Wall -Wextra -std=c11

all: test demo experiment

test: jump_hash.o test.o
	$(CC) $(CFLAGS) -o test jump_hash.o test.o

demo: jump_hash.o demo.o
	$(CC) $(CFLAGS) -o demo jump_hash.o demo.o

experiment: jump_hash.o experiment.o
	$(CC) $(CFLAGS) -o experiment jump_hash.o experiment.o

jump_hash.o: jump_hash.c jump_hash.h
	$(CC) $(CFLAGS) -c jump_hash.c

test.o: test.c jump_hash.h
	$(CC) $(CFLAGS) -c test.c

demo.o: demo.c jump_hash.h
	$(CC) $(CFLAGS) -c demo.c

experiment.o: experiment.c jump_hash.h
	$(CC) $(CFLAGS) -c experiment.c

run_tests: test
	./test

clean:
	rm -f *.o test demo experiment results.csv
