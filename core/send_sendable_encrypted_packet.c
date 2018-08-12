#ifndef L2AUTH_SEND_ENCRYPTED_PACKET_C
#define L2AUTH_SEND_ENCRYPTED_PACKET_C

#include <core/l2_socket.c>
#include <core/l2_blowfish_key.c>
#include <core/send_sendable_packet.c>
#include <core/l2_packet.c>
#include <server_packet/server_packet_encrypted.c>

void send_sendable_encrypted_packet(struct l2_socket* client, struct l2_packet* packet, struct l2_blowfish_key* key)
{
    struct l2_packet encrypted_packet;
    server_packet_encrypted(&encrypted_packet, packet, key);
    send_sendable_packet(client, &encrypted_packet);
}

#endif //L2AUTH_SEND_ENCRYPTED_PACKET_C