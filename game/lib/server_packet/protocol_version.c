#include "../headers.h"
#include "protocol_version.h"

void server_packet_protocol_version(packet *dest)
{
        assert(dest);

        packet_type type = 0x00;

        byte content[] = {
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

        packet_build(dest, type, content, sizeof(content));
}
