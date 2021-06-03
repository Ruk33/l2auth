#ifndef INCLUDE_PACKET_CHAR_SELECT_H
#define INCLUDE_PACKET_CHAR_SELECT_H

#include "packet.h"
#include "l2_string.h"
#include "character.h"

typedef struct {
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
} packet_char_select_t;

void packet_char_select_set_playok(packet_char_select_t *dest, int playOK1);

void packet_char_select_set_char(packet_char_select_t *dest, character_t *src);

void packet_char_select_pack(packet_t *dest, packet_char_select_t *src);

#endif
