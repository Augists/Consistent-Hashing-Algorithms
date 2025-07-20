CC = gcc
CFLAGS = -Wall -Wextra -std=c11

TARGETS = test_consistent_hashing demo experiment

all: $(TARGETS)

test_consistent_hashing: consistent_hashing.o test_consistent_hashing.o
	$(CC) $(CFLAGS) $^ -o $@

demo: consistent_hashing.o demo.o
	$(CC) $(CFLAGS) $^ -o $@

experiment: consistent_hashing.o experiment.o
	$(CC) $(CFLAGS) $^ -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: all clean run_test run_demo run_experiment

run_test: test_consistent_hashing
	./test_consistent_hashing

run_demo: demo
	./demo

run_experiment: experiment
	./experiment

clean:
	rm -f *.o $(TARGETS) results.csv
