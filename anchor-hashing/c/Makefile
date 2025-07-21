CC=gcc
CFLAGS=-O2 -Wall

all: test_anchor_hash experiment

test_anchor_hash: anchor_hash.o test_anchor_hash.o
	$(CC) $(CFLAGS) -o $@ $^

anchor_hash.o: anchor_hash.c anchor_hash.h
	$(CC) $(CFLAGS) -c anchor_hash.c

test_anchor_hash.o: test_anchor_hash.c anchor_hash.h
	$(CC) $(CFLAGS) -c test_anchor_hash.c

experiment: anchor_hash.o experiment.o
	$(CC) $(CFLAGS) -o $@ $^

experiment.o: experiment.c anchor_hash.h
	$(CC) $(CFLAGS) -c experiment.c

clean:
	rm -f *.o test_anchor_hash experiment

.PHONY: all clean test experiment

test: test_anchor_hash
	./test_anchor_hash

experiment: experiment
	./experiment
