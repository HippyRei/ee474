# compile jitter.c
jitter: jitter.c jitter.h bbcommon.c bbcommon.h
	gcc -std=c99 -o jitter bbcommon.c jitter.c

test: test.c bbcommon.c bbcommon.h
	gcc -std=c99 -o test bbcommon.c test.c

clean:
	rm -rf *.o jitter test
