CC=gcc
CFLAGS=-std=c99


all: adc_listener tank

adc_listener: adc_listener.c adc_listener.h
	$(CC) $(CFLAGS) -o adc_listener.exe adc_listener.c -lrt

tank: tank.c tank.h
	$(CC) $(CFLAGS) -o tank.exe tank.c -lrt

clean:
	rm -rf *.o *.exe
