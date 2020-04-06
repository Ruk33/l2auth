#ifndef L2AUTH_PACKET_CLIENT_DECRYPT_H
#define L2AUTH_PACKET_CLIENT_DECRYPT_H

#include <core/l2_raw_packet.h>
#include <core/l2_blowfish_key.h>

l2_raw_packet* packet_client_decrypt
(
        struct L2BlowfishKey* blowfish_key,
        unsigned char* data,
        unsigned short data_size
);

#endif
