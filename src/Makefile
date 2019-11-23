MAIN=main

PRJ-NAME=ifj19

SOURCES=deque.c  error.c iterator.c ptr_string.c  queue.c  scaner.c  stack.c
CC=gcc
CFLAGS=-Wall -Werror -Wextra -Wpedantic -std=c99 -pedantic -g -O0
LDFLAGS=-lm

all:
	$(CC) $(CFLAGS) -o $(PRJ-NAME) $(MAIN).c $(SOURCES) $(LDFLAGS)

.PHONY : all

clean:
	rm -f *.o $(PRJ-NAME)
