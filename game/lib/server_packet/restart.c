#include <headers.h>
#include "restart.h"

void server_packet_restart(packet *dest)
{
        assert(dest);

        packet_type type = 0x5f;

        byte_t buf[sizeof(server_packet_restart_t)] = {0};
        byte_t *p = buf;

        server_packet_restart_t restart_packet = {0};

        restart_packet.response = 0x01;
        BYTE_WRITE_VAL(p, restart_packet.response);

        packet_build(dest, type, buf, (size_t) (p - buf));
}
