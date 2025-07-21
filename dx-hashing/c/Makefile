CC=gcc
CFLAGS=-O2 -Wall

all: test_dx_hash experiment

test_dx_hash: dx_hash.o test_dx_hash.o
	$(CC) $(CFLAGS) -o $@ $^

dx_hash.o: dx_hash.c dx_hash.h
	$(CC) $(CFLAGS) -c dx_hash.c

test_dx_hash.o: test_dx_hash.c dx_hash.h
	$(CC) $(CFLAGS) -c test_dx_hash.c

experiment: dx_hash.o experiment.o
	$(CC) $(CFLAGS) -o $@ $^

experiment.o: experiment.c dx_hash.h
	$(CC) $(CFLAGS) -c experiment.c

clean:
	rm -f *.o test_dx_hash experiment

.PHONY: all clean test experiment

test: test_dx_hash
	./test_dx_hash

experiment: experiment
	./experiment
