CC = gcc
CFLAGS = -I. -g -Wall

all: test_dx_hash demo experiment

test_dx_hash: dx_hash.o test_dx_hash.o
	$(CC) $(CFLAGS) -o test_dx_hash dx_hash.o test_dx_hash.o

demo: dx_hash.o demo.o
	$(CC) $(CFLAGS) -o demo dx_hash.o demo.o

experiment: dx_hash.o experiment.o
	$(CC) $(CFLAGS) -o experiment dx_hash.o experiment.o

dx_hash.o: dx_hash.c dx_hash.h
	$(CC) $(CFLAGS) -c dx_hash.c

test_dx_hash.o: test_dx_hash.c dx_hash.h
	$(CC) $(CFLAGS) -c test_dx_hash.c

demo.o: demo.c dx_hash.h
	$(CC) $(CFLAGS) -c demo.c

experiment.o: experiment.c dx_hash.h
	$(CC) $(CFLAGS) -c experiment.c

run_tests: test_dx_hash
	./test_dx_hash

clean:
	rm -f *.o test_dx_hash demo experiment results.csv
