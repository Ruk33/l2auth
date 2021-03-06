#include <assert.h>
#include "include/util.h"
#include "include/packet.h"
#include "include/gs_packet_protocol_version.h"

void gs_packet_protocol_version(gs_packet_protocol_version_t *src)
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

        assert(src);

        bytes_cpy(src->content, content, sizeof(src->content));
}

void gs_packet_protocol_version_pack(
        packet_t *dest,
        gs_packet_protocol_version_t *src)
{
        byte_t type = 0;

        assert(dest);
        assert(src);

        type = 0x00;

        packet_append_val(dest, type);
        packet_append(dest, src->content);
}
