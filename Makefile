CC = gcc
CFLAGS = -std=c99 -Wall -Werror -pedantic -g -D_XOPEN_SOURCE
INCS = -I/usr/include
LIBS = -L/usr/lib -lc -lreadline

bsql: bsql.c
	$(CC) $(CFLAGS) $^ $(INCS) $(LIBS) -o $@

clean:
	rm bsql

.PHONY: clean
