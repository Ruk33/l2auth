#ifndef L2AUTH_GAME_SERVICE_CRYPT_PACKET_ENCRYPT_H
#define L2AUTH_GAME_SERVICE_CRYPT_PACKET_ENCRYPT_H

#include <core/l2_raw_packet.h>
#include <core/l2_packet.h>

l2_packet* game_service_crypt_packet_encrypt
(
        l2_raw_packet* request,
        unsigned char* encrypt_key
);

#endif
