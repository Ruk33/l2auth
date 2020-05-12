#ifndef L2AUTH_LOGIN_PACKET_BLOWFISH_DECRYPT_H
#define L2AUTH_LOGIN_PACKET_BLOWFISH_DECRYPT_H

#include <login/client.h>
#include <core/l2_raw_packet.h>

l2_raw_packet* login_packet_blowfish_decrypt
(
        struct LoginClient* client,
        unsigned char* data,
        unsigned short data_size
);

#endif
