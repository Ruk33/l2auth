#ifndef L2AUTH_LOGIN_PACKET_PLAY_OK_C
#define L2AUTH_LOGIN_PACKET_PLAY_OK_C

#include <log/log.h>
#include <core/l2_packet.c>
#include <core/byte_buffer.c>
#include <login/dto/session_key.c>

l2_packet* login_packet_play_ok
(
        struct LoginDtoSessionKey *session_key
)
{
        l2_packet_type type = 0x07;
        struct ByteBuffer* buffer = byte_buffer_create();
        l2_packet* packet;

        if (session_key) {
                log_info("Using play OK ids %d - %d", session_key->playOK1, session_key->playOK2);
                byte_buffer_append(buffer, &session_key->playOK1, sizeof(int));
                byte_buffer_append(buffer, &session_key->playOK2, sizeof(int));
        } else {
                log_fatal("No session key was provided in login play ok");
        }

        log_info("buffer content, %s", byte_buffer_content(buffer));

        packet = l2_packet_new(
                type,
                byte_buffer_content(buffer),
                (unsigned short) byte_buffer_size(buffer)
        );

        byte_buffer_free(buffer);

        return packet;
}

#endif
