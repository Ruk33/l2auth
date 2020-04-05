#ifndef L2AUTH_LOGIN_GAME_PACKET_RESTART
#define L2AUTH_LOGIN_GAME_PACKET_RESTART

#include <core/l2_packet.c>

l2_packet* game_packet_restart()
{
        l2_packet_type type = 0x5f;
        unsigned int content = 0x01;

        return l2_packet_new(
                type,
                &content,
                (unsigned short) sizeof(content)
        );
}

#endif
