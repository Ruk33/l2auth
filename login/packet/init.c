#ifndef L2AUTH_LOGIN_PACKET_INIT_C
#define L2AUTH_LOGIN_PACKET_INIT_C

#include <assert.h>
#include <stdlib.h>
#include <log/log.h>
#include <core/l2_client.h>
#include <core/l2_rsa_key.h>
#include <core/l2_packet.h>
#include <core/byte_builder.h>
#include <login/packet/init.h>

l2_packet* login_packet_init(struct L2Client* client)
{
        assert(client);

        l2_packet_type type = 0x00;

        struct L2RSAKey *rsa_key = l2_client_rsa_key(client);

        unsigned char session_id[] = {
                0xfd,
                0x8a,
                0x22,
                0x00,
        };

        unsigned char protocol[] = {
                0x5a,
                0x78,
                0x00,
                0x00,
        };

        int rsa_size = l2_rsa_key_size(rsa_key);

        unsigned short content_size = (unsigned short) (
                sizeof(session_id) +
                sizeof(protocol) +
                (size_t) (rsa_size)
        );

        unsigned char* rsa_modulus = l2_client_alloc_temp_mem(
                client,
                (size_t) (rsa_size)
        );

        byte_builder* buffer = l2_client_byte_builder(
                client,
                content_size * sizeof(char)
        );

        byte_builder_append(buffer, session_id, sizeof(session_id));
        byte_builder_append(buffer, protocol, sizeof(protocol));
        l2_rsa_key_modulus(rsa_key, rsa_modulus);
        byte_builder_append(buffer, rsa_modulus, (size_t) (rsa_size));

        return l2_client_create_packet(
                client,
                type,
                buffer,
                (unsigned short) byte_builder_length(buffer)
        );
}

#endif
