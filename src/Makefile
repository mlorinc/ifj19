MAIN=main

PRJ-NAME=ifj19

SOURCES=ast_structures.c  deque.c  error.c  iterator.c  parser_ast.c  parser.c  parser_result.c  ptr_string.c  queue.c  scaner.c  semantic_analyser.c  stack.c  symtable.c
CC=gcc
CFLAGS=-Wall -Werror -Wextra -Wpedantic -std=c99 -pedantic -g -O0
LDFLAGS=-lm

all:
	$(CC) $(CFLAGS) -o $(PRJ-NAME) $(MAIN).c $(SOURCES) $(LDFLAGS)

.PHONY : all

clean:
	rm -f *.o $(PRJ-NAME)
