#include "../headers.h"
#include "restart.h"

void server_packet_restart(packet *dest)
{
        assert(dest);

        packet_type type = 0x5f;

        byte buf[SERVER_PACKET_RESTART_SIZE] = {0};
        byte *p = buf;

        struct ServerPacketRestart restart_packet = {0};

        restart_packet.response = 0x01;
        BYTE_WRITE_VAL(p, restart_packet.response);

        packet_build(dest, type, buf, (size_t) (p - buf));
}
