STRING=ptr_string
ERR=error
MAIN=main

# Define what project you want to compile
PRJ-NAME=ifj19

TARGET=$(PRJ-NAME)
CC=gcc
CFLAGS=-Wall -Werror -Wextra -Wpedantic -std=c99 -pedantic -lm

.PHONY : all

all: $(TARGET)

$(TARGET): $(STRING).o $(ERR).o $(MAIN).o
	$(CC) $(CFLAGS) $^ -o $@

$(STRING).o: $(STRING).c $(ERR).h $(STRING).h
	$(CC) $(CFLAGS) -c $<

$(ERR).o: $(ERR).c $(ERR).h
	$(CC) $(CFLAGS) -c $<

$(MAIN).o: $(MAIN).c $(ERR).h $(STRING).h
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f *.o $(TARGET)
