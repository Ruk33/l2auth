#ifndef L2AUTH_SEND_SENDABLE_PACKET_C
#define L2AUTH_SEND_SENDABLE_PACKET_C

#include <core/l2_socket.c>
#include <core/l2_packet.c>
#include <core/send_packet.c>
#include <server_packet/server_packet_sendable.c>

void send_sendable_packet(struct l2_socket* client, struct l2_packet* packet)
{
    struct l2_packet sendable;
    server_packet_sendable(&sendable, packet);
    send_packet(client, &sendable);
}

#endif //L2AUTH_SEND_SENDABLE_PACKET_C