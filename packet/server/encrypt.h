#ifndef L2AUTH_PACKET_SERVER_ENCRYPT_H
#define L2AUTH_PACKET_SERVER_ENCRYPT_H

#include <core/l2_client.h>
#include <core/l2_raw_packet.h>

l2_raw_packet* packet_server_encrypt
(
        struct L2Client* client,
        l2_raw_packet* to_encrypt
);

#endif
