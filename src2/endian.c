#include "include/l2auth.h"

u32 be32_to_le(u32 src)
{
    return (
        ((src >> 24) & 0xff)       |
        ((src <<  8) & 0xff0000)   |
        ((src >>  8) & 0xff00)     |
        ((src << 24) & 0xff000000)
    );
}

u32 le32_to_be(u32 src)
{
    return (
        ((src & 0x000000ff) << 24) |
        ((src & 0x0000ff00) <<  8) |
        ((src & 0x00ff0000) >>  8) |
        ((src & 0xff000000) >> 24)
    );
}
