CC=gcc
CFLAGS=-std=c99


all: adc_listener tank tank_entry

adc_listener: adc_listener.c adc_listener.h
	$(CC) $(CFLAGS) -o adc_listener.exe adc_listener.c -lrt

tank: tank.c tank.h
	$(CC) $(CFLAGS) -o tank.exe tank.c -lrt

tank_entry: tank_entry.c tank_entry.h
	$(CC) $(CFLAGS) -o tank_entry.exe tank_entry.c -lrt

clean:
	rm -rf *.o *.exe
