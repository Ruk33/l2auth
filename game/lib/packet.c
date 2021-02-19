#include <assert.h>
#include <stddef.h>
#include "types.h"
#include "byte_read.h"
#include "byte_write.h"
#include "packet.h"

void packet_build(packet *dest, packet_type type, byte_t *content, size_t content_size)
{
        packet_size size = 0;

        assert(dest);

        size = (packet_size) (
                sizeof(size) +
                PACKET_PADDED_SIZE(sizeof(type) + content_size)
        );

        BYTE_WRITE_VAL(dest, size);
        BYTE_WRITE_VAL(dest, type);

        if (!content || !content_size) {
                return;
        }

        BYTE_WRITE(dest, content, content_size);
}

packet_size packet_get_size(packet *src)
{
        packet_size size = 0;

        assert(src);
        BYTE_READ_VAL(size, src);

        return (packet_size) (size & 0xFFFF);
}

packet_type packet_get_type(packet *src)
{
        assert(src);
        return (packet_type) (src[2] & 0xff);
}

byte_t *packet_body(packet *src)
{
        size_t size_header = sizeof(packet_size);
        size_t type_header = sizeof(packet_type);
        assert(src);
        return src + size_header + type_header;
}
