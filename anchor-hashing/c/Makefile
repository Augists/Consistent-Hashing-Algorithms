CC = gcc
CFLAGS = -I. -g -Wall

all: test_anchor_hash demo experiment

test_anchor_hash: anchor_hash.o test_anchor_hash.o
	$(CC) $(CFLAGS) -o test_anchor_hash anchor_hash.o test_anchor_hash.o

demo: anchor_hash.o demo.o
	$(CC) $(CFLAGS) -o demo anchor_hash.o demo.o

experiment: anchor_hash.o experiment.o
	$(CC) $(CFLAGS) -o experiment anchor_hash.o experiment.o

anchor_hash.o: anchor_hash.c anchor_hash.h
	$(CC) $(CFLAGS) -c anchor_hash.c

test_anchor_hash.o: test_anchor_hash.c anchor_hash.h
	$(CC) $(CFLAGS) -c test_anchor_hash.c

demo.o: demo.c anchor_hash.h
	$(CC) $(CFLAGS) -c demo.c

experiment.o: experiment.c anchor_hash.h
	$(CC) $(CFLAGS) -c experiment.c

run_tests: test_anchor_hash
	./test_anchor_hash

clean:
	rm -f *.o test_anchor_hash demo experiment results.csv
