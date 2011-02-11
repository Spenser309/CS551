CC=gcc
CFLAGS=-std=c99 -Wall -g `/usr/pkg/bin/pthread-config --cflags`
LDFLAGS=`/usr/pkg/bin/pthread-config --ldflags`
LIBS=`/usr/pkg/bin/pthread-config --libs`

DEFAULT: all

.PHONY: all, clean, test

all: bssh tst/pvst

bssh: src/main.o src/cmd.o src/env.o
	$(CC) $(LDFLAGS) src/main.o src/cmd.o src/env.o -o bssh $(LIBS)

src/main.o: src/main.c
	$(CC) -c $(CFLAGS) src/main.c -o src/main.o

src/cmd.o: src/cmd.c
	$(CC) -c $(CFLAGS) src/cmd.c -o src/cmd.o

src/env.o: src/env.c
	$(CC) -c $(CFLAGS) src/env.c -o src/env.o
	
clean:
	rm -f bssh tst/pvst src/*.o tst/*.o tst/case*.log tst/case3.dat

tst/pvst: tst/threadsvsprocess.o
	$(CC) $(LDFLAGS) tst/threadsvsprocess.o -o tst/pvst $(LIBS)
	
tst/threadsvsprocess.o: tst/threadsvsprocess.c
	$(CC) -c $(CFLAGS) tst/threadsvsprocess.c -o tst/threadsvsprocess.o

test: 
	@echo "### TEST CASE 1 ###"
	@echo "-- Commands --"
	@cat tst/case1.tst
	@echo "-- Response --"
	@./bssh < tst/case1.tst | tee tst/case1.log
	@echo
	@echo "### TEST CASE 2 ###"
	@echo "-- Commands --"
	@cat tst/case2.tst
	@echo "-- Response --"
	@./bssh < tst/case2.tst | tee tst/case2.log
	@echo
	@echo "### TEST CASE 3 ###"
	@echo "-- Commands --"
	@cat tst/case3.tst
	@echo "-- Response --"
	@./bssh < tst/case3.tst | tee tst/case3.log
	@echo
	@echo "### TEST CASE 4 ###"
	@echo "-- Commands --"
	@cat tst/case4.tst
	@echo "-- Response --"
	@./bssh < tst/case4.tst | tee tst/case4.log
	@echo
	@echo "### TEST CASE 5 ###"
	@echo "-- Commands --"
	@cat tst/case5.tst
	@echo "-- Response --"
	@./bssh < tst/case5.tst | tee tst/case5.log
	@echo
	@echo "### TEST CASE 6 ###"
	@echo "-- Commands --"
	@cat tst/case6.tst
	@echo "-- Response --"
	@./bssh < tst/case6.tst | tee tst/case6.log
	@echo
	@echo "### TEST CASE 7 ###"
	@echo "-- Commands --"
	@cat tst/case7.tst
	@echo "-- Response --"
	@./bssh < tst/case7.tst | tee tst/case7.log
	@echo
	@echo "### TEST CASE 8 ###"
	@echo "-- Commands --"
	@cat tst/case8.tst
	@echo "-- Response --"
	@./bssh < tst/case8.tst | tee tst/case8.log
	@echo
	@echo "### TEST CASE 9 ###"
	@echo "-- Commands --"
	@cat tst/case9.tst
	@echo "-- Response --"
	@./bssh < tst/case9.tst | tee tst/case9.log
	@echo
	@echo "### TEST CASE 10 ###"
	@echo "-- Commands --"
	@cat tst/case10.tst
	@echo "-- Response --"
	@./bssh < tst/case10.tst | tee tst/case10.log	
