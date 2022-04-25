#include <assert.h>
#include "include/util.h"

void cpy_str(i8 *dest, i8 *src, size_t n)
{
    assert(dest);
    assert(src);
    while (n && *src) {
        *dest = *src;
        dest += 1;
        src += 1;
        n -= 1;
    }
    // null terminator.
    *dest = 0;
}

void cpy_bytes(byte *dest, byte *src, size_t n)
{
    assert(dest);
    assert(src);
    while (n) {
        *dest = *src;
        dest += 1;
        src += 1;
        n -= 1;
    }
}

u32 be32tole(u32 src)
{
    return (
        ((src >> 24) & 0xff)      |
        ((src <<  8) & 0xff0000)  |
        ((src >>  8) & 0xff00)    |
        ((src << 24) & 0xff000000)
    );
}

u32 le32tobe(u32 src)
{
    return (
        ((src & 0x000000ff) << 24) |
        ((src & 0x0000ff00) <<  8) |
        ((src & 0x00ff0000) >>  8) |
        ((src & 0xff000000) >> 24)
    );
}