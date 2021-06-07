#include <assert.h>
#include "include/util.h"
#include "include/packet.h"

u16_t packet_size(packet_t *src)
{
        u16_t size = 0;
        assert(src);
        bytes_cpy((byte_t *) &size, src, sizeof(size));
        return size;
}

u8_t packet_type(packet_t *src)
{
        assert(src);
        return (u8_t) src[2];
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

        bytes_cpy(dest, (byte_t *) &new_size, sizeof(new_size));
        bytes_cpy(dest + size, src, n);
}
