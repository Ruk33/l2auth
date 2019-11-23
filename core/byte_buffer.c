#ifndef L2AUTH_BYTE_BUFFER_C
#define L2AUTH_BYTE_BUFFER_C

#include <stdlib.h>

struct ByteBuffer
{
        unsigned char* content;
        size_t size;
        size_t used_space;
};

struct ByteBuffer* byte_buffer_create()
{
        struct ByteBuffer* buffer = malloc(sizeof(struct ByteBuffer));

        buffer->size = 0;
        buffer->used_space = 0;
        buffer->content = NULL;

        return buffer;
}

void byte_buffer_free(struct ByteBuffer* buffer)
{
        if (!buffer) return;
        if (buffer->content) free(buffer->content);
        free(buffer);
}

void byte_buffer_realloc_if_required
(
        struct ByteBuffer* buffer,
        size_t required
)
{
        if (!buffer) return;
        if (buffer->size >= buffer->used_space + required) return;

        buffer->size = buffer->size > 0 ? buffer->size : 16;

        while (buffer->size < buffer->used_space + required) {
                buffer->size *= 2;
        }

        buffer->content = realloc(
                buffer->content,
                sizeof(char) * buffer->size
        );
}

void byte_buffer_append(struct ByteBuffer* buffer, const void* content, size_t content_size)
{
        if (!buffer || !content || content_size == 0) return;
        byte_buffer_realloc_if_required(buffer, content_size);
        memcpy(buffer->content + buffer->used_space, content, content_size);
        buffer->used_space += content_size;
}

unsigned char* byte_buffer_content(struct ByteBuffer* buffer)
{
        return buffer ? buffer->content : NULL;
}

size_t byte_buffer_size(struct ByteBuffer* buffer)
{
        return buffer ? buffer->used_space : 0;
}

#endif
