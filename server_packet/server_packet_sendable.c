#ifndef L2AUTH_SERVER_PACKET_SENDABLE_C
#define L2AUTH_SERVER_PACKET_SENDABLE_C

#include <core/l2_packet.c>

void server_packet_sendable(struct l2_packet *packet, struct l2_packet *to_wrap)
{
    unsigned short total_packet_size = sizeof(total_packet_size) + to_wrap->size;

    packet_reset(packet);
    packet_write(packet, (char*) &total_packet_size, sizeof(total_packet_size));
    packet_write(packet, to_wrap->buffer, to_wrap->size);
}

#endif //L2AUTH_SERVER_PACKET_SENDABLE_C