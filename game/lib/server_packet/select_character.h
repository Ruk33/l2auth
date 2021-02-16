#ifndef LIB_SERVER_PACKET_SELECT_CHARACTER_H
#define LIB_SERVER_PACKET_SELECT_CHARACTER_H

#include <headers.h>
#include <character.h>

struct ServerPacketSelectCharacter {
        int empty[54];
        l2_string_t name[28];
        int id;
        l2_string_t title[28];
        int playOK1;
        int clan_id;
        int sex;
        int race_id;
        int class_id;
        int active;
        int x;
        int y;
        int z;
        double hp;
        double mp;
        int sp;
        int exp;
        int level;
        int _int;
        int str;
        int con;
        int men;
        int dex;
        int wit;
        int game_time;
};

typedef struct ServerPacketSelectCharacter server_packet_select_character_t;

void server_packet_select_character(packet *dest, character_t *character, int playOK1);

#endif
