#include "../headers.h"
#include "show_map.h"

void server_packet_show_map(packet *dest, int map_id)
{
        assert(dest);

        packet_type type = 0x9d;

        unsigned char buf[SERVER_PACKET_SHOW_MAP_SIZE] = {0};
        unsigned char *p = buf;

        struct ServerPacketShowMap show_map_packet = {0};

        show_map_packet.map_id = map_id;
        BYTE_WRITE_VAL(p, show_map_packet.map_id);

        packet_build(dest, type, buf, (size_t) (p - buf));
}
