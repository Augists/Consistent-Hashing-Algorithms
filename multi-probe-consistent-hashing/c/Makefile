CC = gcc
CFLAGS = -I. -g -Wall -Wextra -std=c11

all: test_multi_probe_hashing demo experiment

test_multi_probe_hashing: multi_probe_hashing.o test_multi_probe_hashing.o
	$(CC) $(CFLAGS) -o test_multi_probe_hashing multi_probe_hashing.o test_multi_probe_hashing.o

demo: multi_probe_hashing.o demo.o
	$(CC) $(CFLAGS) -o demo multi_probe_hashing.o demo.o

experiment: multi_probe_hashing.o experiment.o
	$(CC) $(CFLAGS) -o experiment multi_probe_hashing.o experiment.o

multi_probe_hashing.o: multi_probe_hashing.c multi_probe_hashing.h
	$(CC) $(CFLAGS) -c multi_probe_hashing.c

test_multi_probe_hashing.o: test_multi_probe_hashing.c multi_probe_hashing.h
	$(CC) $(CFLAGS) -c test_multi_probe_hashing.c

demo.o: demo.c multi_probe_hashing.h
	$(CC) $(CFLAGS) -c demo.c

experiment.o: experiment.c multi_probe_hashing.h
	$(CC) $(CFLAGS) -c experiment.c

run_tests: test_multi_probe_hashing
	./test_multi_probe_hashing

clean:
	rm -f *.o test_multi_probe_hashing demo experiment results.csv
