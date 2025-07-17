CC = gcc
CFLAGS = -Wall -Wextra -std=c11

TARGET = test_consistent_hashing

SRCS = consistent_hashing.c test_consistent_hashing.c
OBJS = $(SRCS:.c=.o)

.PHONY: all clean run

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

run:
	./$(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)
