#ifndef L2AUTH_LOGIN_GAME_PACKET_CHAR_CREATE_OK
#define L2AUTH_LOGIN_GAME_PACKET_CHAR_CREATE_OK

#include <core/l2_packet.c>

l2_packet* game_packet_char_create_ok()
{
        l2_packet_type type = 0x19;
        unsigned int content = 0x01;

        return l2_packet_new(
                type,
                &content,
                (unsigned short) sizeof(content)
        );
}

#endif
