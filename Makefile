#!/bin/make

ifeq ($(shell uname), Darwin)
	CC = clang
	CFLAGS += -DDARWIN
	LIBS = -framework Accelerate
else
	CC = gcc
	CFLAGS += -llapack
endif


ifdef debug
	CFLAGS += -g -Wall -DDEBUG=1
else
	CFLAGS += -O3 -Wall
endif

CFLAGS += -std=c99

eigvals: eig.c
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean
clean:
	rm -fr eigvals *.o eigvals.dSYM
