#ifndef L2AUTH_LOGIN_GAME_PACKET_QUEST_LIST
#define L2AUTH_LOGIN_GAME_PACKET_QUEST_LIST

#include <core/l2_packet.c>

l2_packet* game_packet_quest_list()
{
        l2_packet_type type = 0x80;
        unsigned char content[] = {
                0,
                0,
                0,
                0
        };

        return l2_packet_new(
                type,
                content,
                (unsigned short) sizeof(content)
        );
}

#endif
