# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -g -std=c11

# Source files
SRCS = maglev.c
OBJS = $(SRCS:.c=.o)

# Default target
all: $(OBJS)

# Test specific
TEST_SRCS = test_maglev.c
TEST_OBJS = $(TEST_SRCS:.c=.o)
TEST_EXEC = test_runner

# Demo specific
DEMO_SRCS = demo.c
DEMO_OBJS = $(DEMO_SRCS:.c=.o)
DEMO_EXEC = demo

# Experiment specific
EXPERIMENT_SRCS = experiment.c
EXPERIMENT_OBJS = $(EXPERIMENT_SRCS:.c=.o)
EXPERIMENT_EXEC = experiment

all: $(OBJS)

test: $(TEST_EXEC)
	@./$(TEST_EXEC)

$(TEST_EXEC): $(OBJS) $(TEST_OBJS)
	$(CC) $(CFLAGS) $^ -o $@

demo: $(DEMO_EXEC)

run-demo: demo
	@./$(DEMO_EXEC)

$(DEMO_EXEC): $(OBJS) $(DEMO_OBJS)
	$(CC) $(CFLAGS) $^ -o $@

experiment: $(EXPERIMENT_EXEC)

run-experiment: experiment
	@echo "Running C k-failure experiment..."
	@./$(EXPERIMENT_EXEC) > results.csv
	@echo "C experiment finished. Results saved to c/results.csv"

$(EXPERIMENT_EXEC): $(OBJS) $(EXPERIMENT_OBJS)
	$(CC) $(CFLAGS) $^ -o $@

# Rule to compile .c to .o
%.o: %.c maglev.h
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up build artifacts
clean:
	rm -f $(OBJS) $(TEST_OBJS) $(DEMO_OBJS) $(EXPERIMENT_OBJS)
	rm -f *.out
	rm -f $(DEMO_EXEC)
	rm -f $(EXPERIMENT_EXEC)
	rm -f $(TEST_EXEC)
	rm -f results.csv
