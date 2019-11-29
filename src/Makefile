MAIN=main

PRJ-NAME=ifj19

SOURCES=array_nodes.c  error.c iterator.c  parser_ast.c ptr_string.c  scaner.c stack.c deque.c parser.c parser_result.c expression_parser.c queue.c
CC=gcc
CFLAGS=-Wall -Werror -Wextra -Wpedantic -std=c99 -pedantic -g -O0
LDFLAGS=-lm

all:
	$(CC) $(CFLAGS) -o $(PRJ-NAME) $(MAIN).c $(SOURCES) $(LDFLAGS)

.PHONY : all

clean:
	rm -f *.o $(PRJ-NAME)
