#include <assert.h>
#include <log/log.h>
#include <core/l2_packet.h>
#include <core/byte_builder.h>
#include <core/session_key.h>
#include <login/client.h>
#include <login/packet/ok.h>

l2_packet* login_packet_ok(struct LoginClient* client)
{
        assert(client);

        l2_packet_type type = 0x03;
        struct L2SessionKey* session_key = login_client_session(client);
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
        byte_builder* buffer = login_client_byte_builder(
                client,
                sizeof(session_key->loginOK1) +
                sizeof(session_key->loginOK2) +
                sizeof(content_after_keys)
        );

        log_info(
                "Using keys: '%d' - '%d'",
                session_key->loginOK1,
                session_key->loginOK2
        );

        byte_builder_append(
                buffer,
                (unsigned char *) &session_key->loginOK1,
                sizeof(session_key->loginOK1)
        );
        byte_builder_append(
                buffer,
                (unsigned char *) &session_key->loginOK2,
                sizeof(session_key->loginOK2)
        );
        byte_builder_append(
                buffer,
                content_after_keys,
                sizeof(content_after_keys)
        );

        return login_client_create_packet(
                client,
                type,
                buffer,
                (unsigned short) byte_builder_length(buffer)
        );
}
