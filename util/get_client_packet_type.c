#ifndef L2AUTH_GET_CLIENT_PACKET_TYPE_C
#define L2AUTH_GET_CLIENT_PACKET_TYPE_C

#include <core/l2_client_packet_type.c>
#include <core/l2_packet.c>

enum client_packet_type get_client_packet_type(struct l2_packet *packet)
{
    return packet->buffer[0] & 0xff;
}

#endif //L2AUTH_GET_CLIENT_PACKET_TYPE_C