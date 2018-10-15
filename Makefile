CC = gcc
CFLAGS = -std=c99 -Wall -Werror -pedantic -g -D_XOPEN_SOURCE
INCS = -I/usr/include
LIBS = -L/usr/lib -lc -lreadline

bsql: bsql.c
	$(CC) $(CFLAGS) $^ $(INCS) $(LIBS) -o $@

t: t.c
	$(CC) $(CFLAGS) $^ $(INCS) $(LIBS) -o $@

buf: buf.c
	$(CC) $(CFLAGS) -c $^ $(INCS)

clean:
	rm bsql

.PHONY: clean
