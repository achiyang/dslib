CC = gcc
CFLAGS = -Wall -Wextra -Iinclude -Isrc/internal

SRC = \
	src/core/ds_types.c \
	src/containers/ds_vector.c \
	src/containers/ds_deque.c

VECTOR_TEST = tests/test_vector.c
DEQUE_TEST = tests/test_deque.c
STACK_TEST = tests/test_stack.c
QUEUE_TEST = tests/test_queue.c

all: test_vector test_deque

test_vector: $(VECTOR_TEST) $(SRC)
	$(CC) $(CFLAGS) $(VECTOR_TEST) $(SRC) -o test_vector

test_deque: $(DEQUE_TEST) $(SRC)
	$(CC) $(CFLAGS) $(DEQUE_TEST) $(SRC) -o test_deque

test_stack: $(STACK_TEST) $(SRC)
	$(CC) $(CFLAGS) $(STACK_TEST) $(SRC) -o test_stack

test_queue: $(QUEUE_TEST) $(SRC)
	$(CC) $(CFLAGS) $(QUEUE_TEST) $(SRC) -o test_queue

run_vector: test_vector
	./test_vector

run_deque: test_deque
	./test_deque

run_stack: test_stack
	./test_stack

run_queue: test_queue
	./test_queue

clean:
	rm -f test_vector test_deque test_stack test_queue