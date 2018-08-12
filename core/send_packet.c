#ifndef L2AUTH_SEND_PACKET_C
#define L2AUTH_SEND_PACKET_C

#include <core/l2_socket.c>
#include <core/l2_packet.c>

void send_packet(struct l2_socket* client, struct l2_packet* packet)
{
    l2_socket_send(client, packet->buffer, packet->size);
}

#endif //L2AUTH_SEND_PACKET_C