#ifndef L2AUTH_GET_CLIENT_PACKET_TYPE_C
#define L2AUTH_GET_CLIENT_PACKET_TYPE_C

#include <core/l2_packet.c>

l2_packet_type get_client_packet_type(l2_packet *packet)
{
        return l2_packet_get_type(packet);
}

#endif //L2AUTH_GET_CLIENT_PACKET_TYPE_C
