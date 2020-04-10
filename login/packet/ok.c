#ifndef L2AUTH_LOGIN_PACKET_OK_C
#define L2AUTH_LOGIN_PACKET_OK_C

#include <assert.h>
#include <log/log.h>
#include <core/l2_client.h>
#include <core/l2_packet.h>
#include <core/byte_buffer.h>
#include <login/dto/session_key.h>
#include <login/packet/ok.h>

l2_packet* login_packet_ok(struct L2Client* client)
{
        assert(client);

        l2_packet_type type = 0x03;
        struct LoginDtoSessionKey* session_key = l2_client_session(client);
        struct ByteBuffer* buffer = byte_buffer_create();
        l2_packet* packet;
        unsigned char content_after_keys[] = {
                0x00,
                0x00,
                0x00,
                0x00,

                0x00,
                0x00,
                0x00,
                0x00,

                0xea,
                0x03,
                0x00,
                0x00,

                0x00,
                0x00,
                0x00,
                0x00,

                0x00,
                0x00,
                0x00,
                0x00,

                0x02,
                0x00,
                0x00,
                0x00,

                0x00,
                0x00,
                0x00,
                0x00,

                0x00,
                0x00,
                0x00,
                0x00,

                0x60,
                0x62,
                0xe0,
                0x00,

                0x00,
                0x00,
                0x00,
        };

        assert(session_key);

        log_info(
                "Using keys: '%d' - '%d'",
                session_key->loginOK1,
                session_key->loginOK2
        );

        byte_buffer_append(
                buffer,
                &session_key->loginOK1,
                sizeof(session_key->loginOK1)
        );
        byte_buffer_append(
                buffer,
                &session_key->loginOK2,
                sizeof(session_key->loginOK2)
        );
        byte_buffer_append(
                buffer,
                &content_after_keys,
                sizeof(content_after_keys)
        );

        packet = l2_client_create_packet(
                client,
                type,
                byte_buffer_content(buffer),
                (unsigned short) byte_buffer_size(buffer)
        );

        byte_buffer_free(buffer);

        return packet;
}

#endif
