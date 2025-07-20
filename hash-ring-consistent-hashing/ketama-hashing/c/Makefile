CC = gcc
CFLAGS = -Wall -Wextra -std=c11

TARGETS = test_ketama demo experiment

all: $(TARGETS)

test_ketama: ketama.o test_ketama.o
	$(CC) $(CFLAGS) $^ -o $@

demo: ketama.o demo.o
	$(CC) $(CFLAGS) $^ -o $@

experiment: ketama.o experiment.o
	$(CC) $(CFLAGS) $^ -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: all clean run_test run_demo run_experiment

run_test: test_ketama
	./test_ketama

run_demo: demo
	./demo

run_experiment: experiment
	./experiment

clean:
	rm -f *.o $(TARGETS) results.csv
