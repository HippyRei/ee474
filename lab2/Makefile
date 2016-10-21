CC=gcc
CFLAGS=-std=c99


all: jitter test lcdlistener input

jitter: jitter.c bbcommon.c bbcommon.h
	$(CC) $(CFLAGS) -o jitter bbcommon.c jitter.c

test: test.c bbcommon.c bbcommon.h
	$(CC) $(CFLAGS) -o test bbcommon.c test.c

lcdlistener: lcdlistener.c bbcommon.c bbcommon.h
	$(CC) $(CFLAGS) -o lcdlistener bbcommon.c lcdlistener.c
input: input.c bbcommon.c bbcommon.h
	$(CC) $(CFLAGS) -o input bbcommon.c input.c

clean:
	rm -rf *.o jitter test lcdlistener input
