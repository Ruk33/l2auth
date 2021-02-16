#include <headers.h>
#include "logout.h"

void server_packet_logout(packet *dest)
{
        assert(dest);

        packet_type type = 0x7e;

        unsigned char buf[sizeof(server_packet_logout_t)] = {0};
        unsigned char *p = buf;

        server_packet_logout_t logout_packet = {0};

        BYTE_WRITE(p, logout_packet.empty, sizeof(logout_packet.empty));

        packet_build(dest, type, buf, (size_t) (p - buf));
}
