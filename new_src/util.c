#include <assert.h>
#include "include/util.h"

void decode32le(u32 *dest, void *src_raw)
{
    byte *src = 0;

    assert(dest);
    assert(src_raw);

    src = src_raw;
    *dest = (u32) src[0] | ((u32) src[1] << 8) | ((u32) src[2] << 16) | ((u32) src[3] << 24);
}

void decode32be(u32 *dest, void *raw_src)
{
    byte *src = 0;

    assert(dest);
    assert(raw_src);

    src = raw_src;
    *dest = (u32) src[3] | ((u32) src[2] << 8) | ((u32) src[1] << 16) | ((u32) src[0] << 24);
}

void encode32le(void *raw_dest, u32 src)
{
    byte *dest = 0;
    assert(raw_dest);
    dest = raw_dest;
    dest[0] = (byte) src;
    dest[1] = (byte) (src >> 8);
    dest[2] = (byte) (src >> 16);
    dest[3] = (byte) (src >> 24);
}

void encode32be(void *raw_dest, u32 src)
{
    byte *dest = 0;
    assert(raw_dest);
    dest = raw_dest;
    dest[3] = (byte) src;
    dest[2] = (byte) (src >> 8);
    dest[1] = (byte) (src >> 16);
    dest[0] = (byte) (src >> 24);
}