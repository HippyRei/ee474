CC=gcc
CFLAGS=-std=c99


all: adc_listener tank tank_entry

adc_listener: adc_listener.c adc_listener.h lab4_common.c lab4_common.h
	$(CC) $(CFLAGS) -o adc_listener.exe adc_listener.c lab4_common.c -lrt

tank: tank.c tank.h lab4_common.c lab4_common.h
	$(CC) $(CFLAGS) -o tank.exe tank.c lab4_common.c

tank_entry: tank_entry.c tank_entry.h lab4_common.c lab4_common.h
	$(CC) $(CFLAGS) -o tank_entry.exe tank_entry.c lab4_common.c

clean:
	rm -rf *.exe
