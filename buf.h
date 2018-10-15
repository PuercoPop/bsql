#include <stddef.h>
#include <stdbool.h>

typedef struct Buffer {
        char *contents;
        size_t cur_size;
        size_t max_size;
        bool eof;
} Buffer;

void init_buffer(Buffer *);
void grow_buffer(Buffer *);
int read_line(int, Buffer *);
