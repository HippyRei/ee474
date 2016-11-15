CC=gcc
CFLAGS=-std=c11


all: tank

tank: tank.c tank.h
	$(CC) $(CFLAGS) -o tank.exe tank.c -lrt

clean:
	rm -rf *.o *.exe
