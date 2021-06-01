#include "include/util.h"
#include "include/packet.h"
#include "include/packet_protocol_version.h"

void packet_protocol_version(packet_protocol_version_t *src)
{
        byte_t content[] = {
                0x01,
                // crypt key
                0x94,
                0x35,
                0x00,
                0x00,
                0xa1,
                0x6c,
                0x54,
                0x87,
        };

        bytes_cpy(src->content, content, sizeof(src->content));
}

void packet_protocol_version_pack(packet_t *d, packet_protocol_version_t *s)
{
        byte_t type = 0;

        type = 0x00;

        packet_append_val(d, type);
        packet_append(d, s->content);
}
