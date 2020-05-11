#include <assert.h>
#include <log/log.h>
#include <core/l2_packet.h>
#include <core/byte_builder.h>
#include <core/session_key.h>
#include <login/packet/play_ok.h>

l2_packet* login_packet_play_ok(struct L2Client* client)
{
        assert(client);
        l2_packet_type type = 0x07;
        struct L2DtoSessionKey* session = l2_client_session(client);
        assert(session);
        byte_builder* buffer = l2_client_byte_builder(
                client,
                sizeof(session->playOK1) + sizeof(session->playOK2)
        );

        log_info(
                "Using play OK ids %d - %d",
                session->playOK1,
                session->playOK2
        );
        byte_builder_append(
                buffer,
                (unsigned char *) &session->playOK1,
                sizeof(session->playOK1)
        );
        byte_builder_append(
                buffer,
                (unsigned char *) &session->playOK2,
                sizeof(session->playOK2)
        );

        return l2_client_create_packet(
                client,
                type,
                buffer,
                (unsigned short) byte_builder_length(buffer)
        );
}
