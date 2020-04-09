#ifndef L2AUTH_BYTE_BUFFER_H
#define L2AUTH_BYTE_BUFFER_H

#include <stdlib.h>

struct ByteBuffer;

struct ByteBuffer* byte_buffer_create();
void byte_buffer_free(struct ByteBuffer* buffer);
void byte_buffer_append
(
        struct ByteBuffer* buffer,
        const void* content,
        size_t content_size
);
unsigned char* byte_buffer_content(struct ByteBuffer* buffer);
size_t byte_buffer_size(struct ByteBuffer* buffer);

#endif
