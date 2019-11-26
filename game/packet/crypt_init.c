#ifndef L2AUTH_LOGIN_GAME_PACKET_CRYPT_INIT
#define L2AUTH_LOGIN_GAME_PACKET_CRYPT_INIT

#include <core/l2_packet.c>

l2_packet* game_packet_crypt_init()
{
        l2_packet_type type = 0x00;
        unsigned char content[] = {
                0x01,
                // crypt key
                0x94,
                0x35,
                0x00,
                0x00,
                0xa1,
                0x6c,
                0x54,
                0x87,
        };

        return l2_packet_new(
                type,
                content,
                (unsigned short) sizeof(content)
        );
}

#endif
