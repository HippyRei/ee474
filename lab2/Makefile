CC=gcc
CFLAGS=-std=c99


all: jitter getmoney lcdlistener input

jitter: jitter.c bbcommon.c bbcommon.h
	$(CC) $(CFLAGS) -o jitter.exe bbcommon.c jitter.c

getmoney: getmoney.c bbcommon.c bbcommon.h
	$(CC) $(CFLAGS) -o getmoney.exe bbcommon.c getmoney.c

lcdlistener: lcdlistener.c bbcommon.c bbcommon.h
	$(CC) $(CFLAGS) -o lcdlistener.exe bbcommon.c lcdlistener.c
input: input.c bbcommon.c bbcommon.h
	$(CC) $(CFLAGS) -o input.exe bbcommon.c input.c

clean:
	rm -rf *.o *.exe text_pipe command_pipe
