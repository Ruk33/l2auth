#ifndef L2AUTH_LOGIN_PACKET_BLOWFISH_ENCRYPT_H
#define L2AUTH_LOGIN_PACKET_BLOWFISH_ENCRYPT_H

#include <core/l2_client.h>
#include <core/l2_raw_packet.h>

l2_raw_packet* login_packet_blowfish_encrypt
(
        struct L2Client* client,
        l2_raw_packet* to_encrypt
);

#endif
