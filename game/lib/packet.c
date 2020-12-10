#include <assert.h>
#include <stddef.h>
#include "types.h"
#include "byte_read.h"
#include "byte_write.h"
#include "packet.h"

void packet_build(packet *dest, packet_type type, byte *content, size_t content_size)
{
        assert(dest);
        // Packet of type 0 is also valid
        // assert(type);
        assert(content);
        assert(content_size);

        packet_size size = (unsigned short) (
                sizeof(size) +
                sizeof(type) +
                content_size
        );

        BYTE_WRITE_VAL(dest, size);
        BYTE_WRITE_VAL(dest, type);
        BYTE_WRITE(dest, content, content_size);
}

packet_size packet_get_size(packet *src)
{
        assert(src);
        packet_size size = 0;
        BYTE_READ_VAL(size, src);
        return size;
}

packet_type packet_get_type(packet *src)
{
        assert(src);
        return src[2];
}

byte *packet_body(packet *src)
{
        assert(src);
        size_t size_header = sizeof(packet_size);
        size_t type_header = sizeof(packet_type);
        return src + size_header + type_header;
}
