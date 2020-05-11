#include <log/log.h>
#include <core/l2_raw_packet.h>
#include <core/l2_packet.h>
#include "../encrypt.h"
#include "encrypt.h"

l2_packet* game_service_crypt_packet_encrypt
(
        l2_raw_packet* request,
        unsigned char* encrypt_key
)
{
        unsigned int packet_size = l2_raw_packet_get_size(request);
        if (packet_size > 1) packet_size -= 2;
        game_service_crypt_encrypt(
                request + 2,
                packet_size,
                encrypt_key
        );
        log_info("Packet encrypted");
        return request;
}
