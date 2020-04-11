#ifndef L2AUTH_LOGIN_GAME_PACKET_RESTART_C
#define L2AUTH_LOGIN_GAME_PACKET_RESTART_C

#include <core/l2_packet.h>
#include <game/packet/restart.h>

l2_packet* game_packet_restart()
{
        l2_packet_type type = 0x5f;
        unsigned int content = 0x01;

        return l2_packet_new(
                type,
                (unsigned char *) &content,
                (unsigned short) sizeof(content)
        );
}

#endif
