#ifndef L2AUTH_LOGIN_GAME_PACKET_CHAR_LIST
#define L2AUTH_LOGIN_GAME_PACKET_CHAR_LIST

#include <core/l2_packet.c>
#include <core/byte_buffer.c>

l2_packet* game_packet_char_list()
{
        l2_packet_type type = 0x13;
        unsigned char content[] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
        return l2_packet_new(
                type,
                &content,
                sizeof(content)
        );
}

#endif
