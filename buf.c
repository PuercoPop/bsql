#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>


#include "buf.h"

void die(char *msg, ...);

void
die(char *msg, ...)
{
	va_list ap;

	va_start(ap, msg);
	vfprintf(stderr, msg, ap);
	va_end(ap);
	exit(EXIT_FAILURE);
}

void
init_buffer(Buffer *buf)
{
        buf = calloc(1, sizeof(Buffer));
        if (buf)
                die("Could not allocate buffer.\n");

        buf->contents = malloc(BUFSIZ);
        if (buf->contents == NULL)
                die("Could not allocate buffer contents.\n");
        buf->cur_size = 0;
        buf->max_size = BUFSIZ;
        buf->eof = false;
}
