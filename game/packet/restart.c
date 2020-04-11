#ifndef L2AUTH_LOGIN_GAME_PACKET_RESTART_C
#define L2AUTH_LOGIN_GAME_PACKET_RESTART_C

#include <core/l2_packet.h>
#include <game/packet/restart.h>

l2_packet* game_packet_restart(struct L2Client* client)
{
        l2_packet_type type = 0x5f;
        unsigned int content = 0x01;

        return l2_client_create_packet(
                client,
                type,
                (unsigned char *) &content,
                (unsigned short) sizeof(content)
        );
}

#endif
