#ifndef L2AUTH_BYTE_BUFFER_C
#define L2AUTH_BYTE_BUFFER_C

#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <log/log.h>
#include <core/byte_buffer.h>

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
        assert(buffer);
        if (buffer->content) free(buffer->content);
        free(buffer);
}

void byte_buffer_realloc_if_required
(
        struct ByteBuffer* buffer,
        size_t required
)
{
        assert(buffer);
        assert(required);

        size_t new_required_mem = 0;

        if (buffer->size >= buffer->used_space + required) return;

        buffer->size = buffer->size > 0 ? buffer->size : 16;

        new_required_mem = buffer->used_space + required;

        while (buffer->size < new_required_mem) {
                buffer->size *= 2;
        }

        buffer->content = realloc(
                buffer->content,
                sizeof(char) * buffer->size
        );
}

void byte_buffer_append
(
        struct ByteBuffer* buffer,
        const void* content,
        size_t content_size
)
{
        assert(buffer);
        assert(content);
        assert(content_size);

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
