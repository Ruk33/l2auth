#include "../headers.h"
#include "d0.h"

void server_packet_d0(packet *dest)
{
        assert(dest);

        packet_type type = 0xFE;

        byte buf[SERVER_PACKET_D0_SIZE] = {0};
        byte *p = buf;

        struct ServerPacketD0 d0_packet = {0};

        d0_packet.unknown = 0x1B;
        d0_packet.manor_size = 0;

        BYTE_WRITE_VAL(p, d0_packet.unknown);
        BYTE_WRITE_VAL(p, d0_packet.manor_size);

        packet_build(dest, type, buf, (size_t) (p - buf));
}
