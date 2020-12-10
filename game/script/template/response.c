#include "../headers.h"
#include "REQUEST_LOWERCASE_NAME.h"

void server_packet_REQUEST_LOWERCASE_NAME(packet *dest)
{
        assert(dest);

        packet_type type = 0x00;

        unsigned char buf[SERVER_PACKET_REQUEST_UPPERCASE_NAME_SIZE] = {0};
        unsigned char *p = buf;

        struct ServerPacketREQUEST_UPPERCAMELCASE_NAME REQUEST_LOWERCASE_NAME_packet = {0};

        REQUEST_LOWERCASE_NAME_packet.some_content = 42;
        BYTE_WRITE_VAL(p, REQUEST_LOWERCASE_NAME_packet.some_content);

        packet_build(dest, type, buf, (size_t) (p - buf));
}
