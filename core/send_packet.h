#ifndef L2AUTH_SEND_PACKET_H
#define L2AUTH_SEND_PACKET_H

#include <core/l2_socket.h>
#include <core/l2_raw_packet.h>

void send_packet(struct L2Socket* client, l2_raw_packet* packet);

#endif
