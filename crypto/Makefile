CC=gcc
CFLAGS=-I.
LD=gcc
LDFLAGS=-static -lsodium

DEBUG=-g

OBJS=crypto.o

all: main.out

main.out: $(OBJS)
	$(LD) $(DEBUG) -o $@ $(OBJS) $(LDFLAGS)

#main.o: main.c 
#	$(CC) $(CFLAGS) $(DEBUG) -c main.c 

crypto.o: crypto.c
	$(CC) $(DEBUG) -c crypto.c $(CFLAGS)

.PHONY: clean
clean:
	rm -f *.o *.out main

rebuild:
	make clean
	make
