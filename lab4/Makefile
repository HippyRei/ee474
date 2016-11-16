CC=gcc
CFLAGS=-std=c99


all: acd_listener tank

acd_listener: acd_listener.c acd_listener.h
	$(CC) $(CFLAGS) -o acd_listener.exe acd_listener.c -lrt

tank: tank.c tank.h
	$(CC) $(CFLAGS) -o tank.exe tank.c -lrt

clean:
	rm -rf *.o *.exe
