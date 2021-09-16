#include <assert.h>
#include "include/util.h"
#include "include/packet.h"

u16_t packet_size(packet_t *src)
{
        u16_t size = 0;
        assert(src);
        size = *((u16_t *) src);
        return size & 0xFFFF;
}

u8_t packet_type(packet_t *src)
{
        assert(src);
        return (u8_t) src[2] & 0xff;
}

packet_t *packet_body(packet_t *src)
{
        assert(src);
        return src + 2;
}

void packet_append_n(packet_t *dest, byte_t *src, size_t n)
{
        u16_t size     = 0;
        u16_t new_size = 0;

        assert(dest);
        assert(src);

        size     = packet_size(dest);
        size     = size == 0 ? 2 : size; // Leave space for packet size header.
        new_size = size + n;

        *((u16_t *) dest) = new_size;

        // (franco.montenegro) Make SURE we don't overflow!
        util_cpy_bytes(dest + size, src, n, n, n);
}
