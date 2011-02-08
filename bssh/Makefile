CC=gcc
CFLAGS= -std=c99 -Wall -g $(shell /usr/pkg/bin/pthread-config --cflags)
LDFLAGS=$(shell /usr/pkg/bin/pthread-config --ldflags)
LIBS=$(shell /usr/pkg/bin/pthread-config --libs)

DEFAULT: all

.PHONY: all, clean, test

all: bssh

bssh: src/main.o src/cmd.o src/env.o
	$(CC) $(LDFLAGS) $(LIBS) src/main.o src/cmd.o src/env.o -o bssh	

src/main.o: src/main.c
	$(CC) -c $(CFLAGS) src/main.c -o src/main.o

src/cmd.o: src/cmd.c
	$(CC) -c $(CFLAGS) src/cmd.c -o src/cmd.o

src/env.o: src/env.c
	$(CC) -c $(CFLAGS) src/env.c -o src/env.o
	
clean:
	rm -f bssh src/*.o

test: 
	./bssh > tst/case1.log
	./bssh > tst/case2.log
	./bssh > tst/case3.log