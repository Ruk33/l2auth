#ifndef L2AUTH_LOGIN_GAME_PACKET_PLAYER_AUTH_REQUEST
#define L2AUTH_LOGIN_GAME_PACKET_PLAYER_AUTH_REQUEST

#include <log/log.h>
#include <core/l2_packet.c>
#include <core/byte_buffer.c>
#include <login/dto/session_key.c>

l2_packet* game_packet_player_auth_request
(
        char* account,
        size_t account_length,
        struct LoginDtoSessionKey* key
)
{
        l2_packet_type type = 0x05;
        struct ByteBuffer* buffer = byte_buffer_create();
        l2_packet* packet;
        char ch[] = {0x00, 0x00};

        for (size_t i = 0; i < account_length; i++) {
                ch[0] = account[i];
                ch[1] = 0x00;
                byte_buffer_append(buffer, ch, 2);
        }

        byte_buffer_append(buffer, &key->playOK1, sizeof(key->playOK1));
        byte_buffer_append(buffer, &key->playOK2, sizeof(key->playOK2));
        byte_buffer_append(buffer, &key->loginOK1, sizeof(key->loginOK1));
        byte_buffer_append(buffer, &key->loginOK2, sizeof(key->loginOK2));

        packet = l2_packet_new(
                type,
                byte_buffer_content(buffer),
                (unsigned short) byte_buffer_size(buffer)
        );

        byte_buffer_free(buffer);

        return packet;
}

#endif
