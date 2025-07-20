CC = gcc
CFLAGS = -Wall -Wextra -std=c11

TARGETS = test_rendezvous_hash demo experiment

all: $(TARGETS)

test_rendezvous_hash: rendezvous_hash.o test_rendezvous_hash.o
	$(CC) $(CFLAGS) $^ -o $@

demo: rendezvous_hash.o demo.o
	$(CC) $(CFLAGS) $^ -o $@

experiment: rendezvous_hash.o experiment.o
	$(CC) $(CFLAGS) $^ -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: all clean run_test run_demo run_experiment

run_test: test_rendezvous_hash
	./test_rendezvous_hash

run_demo: demo
	./demo

run_experiment: experiment
	./experiment

clean:
	rm -f *.o $(TARGETS) results.csv
