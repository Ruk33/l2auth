#ifndef L2AUTH_PACKET_CLIENT_DECRYPT_H
#define L2AUTH_PACKET_CLIENT_DECRYPT_H

#include <core/l2_client.h>
#include <core/l2_raw_packet.h>

l2_raw_packet* packet_client_decrypt
(
        struct L2Client* client,
        unsigned char* data,
        unsigned short data_size
);

#endif
