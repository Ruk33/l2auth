#include "../headers.h"
#include "create_char.h"

void server_packet_create_char(packet *dest)
{
        assert(dest);

        packet_type type = 0x19;

        byte buf[SERVER_PACKET_CREATE_CHAR_SIZE] = {0};
        byte *p = buf;

        struct ServerPacketCreateChar create_char_packet = {0};

        create_char_packet.response = 0x01;
        BYTE_WRITE_VAL(p, create_char_packet.response);

        packet_build(dest, type, buf, (size_t) (p - buf));
}
