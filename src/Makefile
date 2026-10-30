CC=clang
CFLAGS=-Wall -Wextra -Wpedantic -std=iso9899:1999 -O2

../bin/ping: $(wildcard *.[c|h])
	mkdir -p ../bin
	$(CC) $(CFLAGS) -o ../bin/ping $(wildcard *.c)

run: ../bin/ping
	./../bin/ping

clean:
	rm -f ../bin/ping
