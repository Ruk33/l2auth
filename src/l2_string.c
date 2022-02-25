#include <assert.h>
#include <stddef.h>
#include "include/util.h"
#include "include/l2_string.h"

void l2_string_cpy(struct buffer *dest, struct buffer *src, size_t n)
{
    size_t src_i = 0;

    assert(dest);
    assert(src);
    assert(dest->buf);
    assert(src->buf);

    while (dest->used + 2 <= dest->size && src_i + 2 <= src->size && 2 <= n && ((char *) src->buf)[src_i]) {
        ((char *) dest->buf)[dest->used++] = ((char *) src->buf)[src_i++];
        ((char *) dest->buf)[dest->used++] = ((char *) src->buf)[src_i++];
        n -= 2;
    }

    ((char *) dest->buf)[dest->used] = 0;

    // Try to add the secondary byte to the l2 string.
    // Todo: make this code better, maybe we can find a cleaner way?
    if (dest->used) {
        dest->used += 1;
        ((char *) dest->buf)[dest->used] = 0;
    }
}

void l2_string_to_char(struct buffer *dest, struct buffer *src, size_t n)
{
    size_t src_i = 0;

    assert(dest);
    assert(src);
    assert(dest->buf);
    assert(src->buf);

    while (dest->used <= dest->size && src_i + 2 <= src->size && n && ((char *) src->buf)[src_i]) {
        ((char *) dest->buf)[dest->used++] = ((char *) src->buf)[src_i++];
        // Ignore the second byte of each l2 string character.
        src_i += 1;
        n -= 1;
    }

    ((char *) dest->buf)[dest->used] = 0;
}

void l2_string_from_char(struct buffer *dest, struct buffer *src, size_t n)
{
    size_t src_i = 0;

    assert(dest);
    assert(src);
    assert(dest->buf);
    assert(src->buf);

    while (dest->used <= dest->size && src_i <= src->size && n && ((char *) src->buf)[src_i]) {
        ((char *) dest->buf)[dest->used++] = ((char *) src->buf)[src_i++];
        ((char *) dest->buf)[dest->used++] = 0;
        n -= 1;
    }

    ((char *) dest->buf)[dest->used] = 0;

    // Try to add the secondary byte to the l2 string.
    // Todo: make this code better, maybe we can find a cleaner way?
    if (dest->used) {
        dest->used += 1;
        ((char *) dest->buf)[dest->used] = 0;
    }
}

size_t l2_string_len(struct buffer *src)
{
    size_t src_i = 0;
    size_t len = 0;

    assert(src);
    assert(src->buf);

    while (src_i + 2 <= src->size && ((char *) src->buf)[src_i]) {
        len += 1;
        src_i += 2;
    }

    return len;
}

size_t l2_string_bytes_from_buf(struct buffer *src)
{
    assert(src);
    assert(src->buf);
    return (l2_string_len(src) + 1) * 2;
}

size_t l2_string_bytes(l2_string_t *src, size_t n)
{
    struct buffer buffer = { 0 };
    buffer.buf = src;
    buffer.size = n;
    return l2_string_bytes_from_buf(&buffer);
}