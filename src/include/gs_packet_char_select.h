#ifndef INCLUDE_GS_PACKET_CHAR_SELECT_H
#define INCLUDE_GS_PACKET_CHAR_SELECT_H

#include "packet.h"
#include "l2_string.h"
#include "gs_character.h"

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
} gs_packet_char_select_t;

void gs_packet_char_select_set_playok(
        gs_packet_char_select_t *dest,
        int playOK1);

void gs_packet_char_select_set_char(
        gs_packet_char_select_t *dest,
        gs_character_t *src);

void gs_packet_char_select_pack(packet_t *dest, gs_packet_char_select_t *src);

#endif
