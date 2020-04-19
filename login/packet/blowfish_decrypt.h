#ifndef L2AUTH_LOGIN_PACKET_BLOWFISH_DECRYPT_H
#define L2AUTH_LOGIN_PACKET_BLOWFISH_DECRYPT_H

#include <core/l2_client.h>
#include <core/l2_raw_packet.h>

l2_raw_packet* login_packet_blowfish_decrypt
(
        struct L2Client* client,
        unsigned char* data,
        unsigned short data_size
);

#endif
