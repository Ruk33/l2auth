#ifndef L2AUTH_GAME_HANDLER_ENCRYPT_C
#define L2AUTH_GAME_HANDLER_ENCRYPT_C

#include <core/l2_raw_packet.h>
#include <core/l2_packet.h>
#include <game/crypt.h>
#include <game/handler/encrypt.h>

l2_packet* game_handler_encrypt
(
        l2_raw_packet* request,
        unsigned char* encrypt_key
)
{
        unsigned int packet_size = l2_raw_packet_get_size(request);
        if (packet_size > 1) packet_size -= 2;
        game_crypt_encrypt(
                request + 2,
                packet_size,
                encrypt_key
        );
        return request;
}

#endif
