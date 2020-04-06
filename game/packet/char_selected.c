#ifndef L2AUTH_LOGIN_GAME_PACKET_CHAR_SELECTED_C
#define L2AUTH_LOGIN_GAME_PACKET_CHAR_SELECTED_C

#include <stdlib.h>
#include <log/log.h>
#include <core/l2_packet.h>
#include <core/byte_buffer.h>
#include <login/dto/session_key.h>
#include <game/packet/char_selected.h>

l2_packet* game_packet_char_selected(int playOK1)
{
        l2_packet_type type = 0x15;
        struct ByteBuffer* buffer = byte_buffer_create();
        l2_packet* packet;

        unsigned char name[] = { 'r', 0, 'u', 0, 'k', 0, 'e', 0, 0, 0 };
        unsigned int char_id = 0x00;
        unsigned char title[] = { 0, 0 };
        unsigned int clan_id = 0;
        unsigned int empty = 0;
        unsigned int sex = 0x00;
        unsigned int race = 0x00;
        unsigned int class = 0x00;
        unsigned int active = 0x01;
        int x = -71338;
        int y = 258271;
        int z = -3104;
        double current_hp = 200;
        double current_mp = 100;
        unsigned int sp = 20;
        unsigned int exp = 10;
        unsigned int level = 1;
        unsigned int _int = 10;
        unsigned int str = 10;
        unsigned int con = 10;
        unsigned int men = 10;
        unsigned int dex = 10;
        unsigned int wit = 10;
        unsigned char* thirty_empty = calloc(30, sizeof(unsigned char));
        unsigned int game_time = 10;

        byte_buffer_append(buffer, name, sizeof(name));
        byte_buffer_append(buffer, &char_id, sizeof(char_id));
        byte_buffer_append(buffer, title, sizeof(title));
        byte_buffer_append(buffer, &playOK1, sizeof(playOK1));
        byte_buffer_append(buffer, &clan_id, sizeof(clan_id));
        byte_buffer_append(buffer, &empty, sizeof(empty));
        byte_buffer_append(buffer, &sex, sizeof(sex));
        byte_buffer_append(buffer, &race, sizeof(race));
        byte_buffer_append(buffer, &class, sizeof(class));
        byte_buffer_append(buffer, &active, sizeof(active));
        byte_buffer_append(buffer, &x, sizeof(x));
        byte_buffer_append(buffer, &y, sizeof(y));
        byte_buffer_append(buffer, &z, sizeof(z));
        byte_buffer_append(buffer, &current_hp, sizeof(current_hp));
        byte_buffer_append(buffer, &current_mp, sizeof(current_mp));
        byte_buffer_append(buffer, &sp, sizeof(sp));
        byte_buffer_append(buffer, &exp, sizeof(exp));
        byte_buffer_append(buffer, &level, sizeof(level));
        byte_buffer_append(buffer, &empty, sizeof(empty));
        byte_buffer_append(buffer, &empty, sizeof(empty));
        byte_buffer_append(buffer, &_int, sizeof(_int));
        byte_buffer_append(buffer, &str, sizeof(str));
        byte_buffer_append(buffer, &con, sizeof(con));
        byte_buffer_append(buffer, &men, sizeof(men));
        byte_buffer_append(buffer, &dex, sizeof(dex));
        byte_buffer_append(buffer, &wit, sizeof(wit));
        byte_buffer_append(buffer, thirty_empty, 30);
        byte_buffer_append(buffer, &empty, sizeof(empty));
        byte_buffer_append(buffer, &empty, sizeof(empty));
        byte_buffer_append(buffer, &empty, sizeof(empty));
        byte_buffer_append(buffer, &empty, sizeof(empty));
        byte_buffer_append(buffer, &empty, sizeof(empty));
        byte_buffer_append(buffer, &empty, sizeof(empty));
        byte_buffer_append(buffer, &game_time, sizeof(game_time));
        byte_buffer_append(buffer, &empty, sizeof(empty));
        byte_buffer_append(buffer, &empty, sizeof(empty));
        byte_buffer_append(buffer, &empty, sizeof(empty));
        byte_buffer_append(buffer, &empty, sizeof(empty));
        byte_buffer_append(buffer, &empty, sizeof(empty));
        byte_buffer_append(buffer, &empty, sizeof(empty));
        byte_buffer_append(buffer, &empty, sizeof(empty));
        byte_buffer_append(buffer, &empty, sizeof(empty));
        byte_buffer_append(buffer, &empty, sizeof(empty));
        byte_buffer_append(buffer, &empty, sizeof(empty));
        byte_buffer_append(buffer, &empty, sizeof(empty));
        byte_buffer_append(buffer, &empty, sizeof(empty));
        byte_buffer_append(buffer, &empty, sizeof(empty));
        byte_buffer_append(buffer, &empty, sizeof(empty));
        byte_buffer_append(buffer, &empty, sizeof(empty));

        packet = l2_packet_new(
                type,
                byte_buffer_content(buffer),
                (unsigned short) byte_buffer_size(buffer)
        );

        byte_buffer_free(buffer);

        return packet;
}

#endif
