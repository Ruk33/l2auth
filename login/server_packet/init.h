#ifndef SERVER_PACKET_INIT_H
#define SERVER_PACKET_INIT_H

#include "../rsa.h"

struct ServerPacketInit {
        unsigned char session_id[4];
        unsigned char protocol[4];
        unsigned char modulus[1024];
};

#define SERVER_PACKET_INIT_SIZE sizeof(struct ServerPacketInit)
#define SERVER_PACKET_INIT_FULL_SIZE SERVER_PACKET_INIT_SIZE + sizeof(short)

void server_packet_init(unsigned char *dest, struct Rsa *rsa);

#endif
