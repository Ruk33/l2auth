#ifndef L2AUTH_SEND_PACKET_C
#define L2AUTH_SEND_PACKET_C

#include <core/l2_socket.c>
#include <core/l2_packet.c>

void send_packet(struct l2_socket* client, l2_raw_packet* packet)
{
        l2_socket_send(
                client,
                packet,
                l2_raw_packet_get_size(packet)
        );
}

#endif //L2AUTH_SEND_PACKET_C
