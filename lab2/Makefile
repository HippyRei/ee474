CC=gcc
CFLAGS=-std=c99


all: jitter getmoney lcdlistener input

jitter: jitter.c bbcommon.c bbcommon.h
	$(CC) $(CFLAGS) -o jitter bbcommon.c jitter.c

getmoney: getmoney.c bbcommon.c bbcommon.h
	$(CC) $(CFLAGS) -o getmoney bbcommon.c getmoney.c

lcdlistener: lcdlistener.c bbcommon.c bbcommon.h
	$(CC) $(CFLAGS) -o lcdlistener bbcommon.c lcdlistener.c
input: input.c bbcommon.c bbcommon.h
	$(CC) $(CFLAGS) -o input bbcommon.c input.c

clean:
	rm -rf *.o jitter getmoney lcdlistener input
