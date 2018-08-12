#ifndef L2AUTH_PACKET_C
#define L2AUTH_PACKET_C

#include <string.h>

struct l2_packet {
  char buffer[65535];
  unsigned short size;
};

void packet_init(struct l2_packet *packet, char packet_type)
{
    packet->buffer[0] = packet_type;
    packet->size = 1;
}

void packet_reset(struct l2_packet *packet)
{
    packet->size = 0;
}

void packet_write(struct l2_packet* packet, const void* data, unsigned short data_size)
{
    memcpy(packet->buffer+packet->size, data, data_size);
    packet->size += data_size;
}

#endif //L2AUTH_PACKET_C