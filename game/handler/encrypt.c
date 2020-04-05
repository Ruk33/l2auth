#ifndef L2AUTH_GAME_HANDLER_ENCRYPT_C
#define L2AUTH_GAME_HANDLER_ENCRYPT_C

#include <stdlib.h>
#include <string.h>
#include <log/log.h>
#include <core/l2_raw_packet.c>
#include <core/l2_packet.c>
#include <game/crypt.c>
#include <core/byte_buffer.c>

l2_packet* game_handler_encrypt
(
        l2_raw_packet* request,
        unsigned char* encrypt_key
)
{
        l2_raw_packet_size size;
        unsigned char* content;
        struct ByteBuffer* buffer;
        unsigned int checksum_space;
        unsigned int padding;
        l2_raw_packet* request_with_checksum;
        unsigned int sz = 0;

        if (1) {
                sz = l2_raw_packet_get_size(request);
                if (sz > 1) sz -= 2;
                game_crypt_encrypt(
                        request + 2,
                        sz,
                        encrypt_key
                );
                return request;
        }

        size = l2_raw_packet_get_size(request);
        content = calloc(size, sizeof(unsigned char));
        buffer = byte_buffer_create();
        checksum_space = 0x00;
        padding = (size + sizeof(checksum_space)) % 8;

        l2_raw_packet_content(request, content, 0, size);
        byte_buffer_append(buffer, content, size);
        byte_buffer_append(buffer, &checksum_space, sizeof(checksum_space));

        if (padding != 0) {
                // 8 extra bytes
                byte_buffer_append(buffer, &checksum_space, sizeof(checksum_space));
                byte_buffer_append(buffer, &checksum_space, sizeof(checksum_space));
        }

        request_with_checksum = l2_raw_packet_new(
                byte_buffer_content(buffer),
                byte_buffer_size(buffer)
        );

        game_crypt_checksum(
                request_with_checksum + 2,
                l2_raw_packet_get_size(request_with_checksum)
        );
        game_crypt_encrypt(
                request_with_checksum + 2,
                l2_raw_packet_get_size(request_with_checksum),
                encrypt_key
        );

        byte_buffer_free(buffer);

        return request_with_checksum;
}

#endif
