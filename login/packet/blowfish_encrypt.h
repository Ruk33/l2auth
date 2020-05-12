#ifndef L2AUTH_LOGIN_PACKET_BLOWFISH_ENCRYPT_H
#define L2AUTH_LOGIN_PACKET_BLOWFISH_ENCRYPT_H

#include <core/l2_raw_packet.h>
#include <login/client.h>

l2_raw_packet* login_packet_blowfish_encrypt
(
        struct LoginClient* client,
        l2_raw_packet* to_encrypt
);

#endif
