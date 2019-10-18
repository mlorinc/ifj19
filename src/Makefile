PRJ=ptr_string
#
PROGS=$(PRJ)-test
CC=gcc
CFLAGS=-Wall -std=c99 -pedantic -lm

all: $(PROGS)

$(PRJ)-test: $(PRJ).c error.c main.c $(PRJ).h error.h
	$(CC) $(CFLAGS) -o $@ $(PRJ).c error.c main.c

clean:
	rm -f *.o  $(PROGS)