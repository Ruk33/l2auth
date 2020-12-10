#ifndef LIB_SERVER_PACKET_CHAR_INFO_H
#define LIB_SERVER_PACKET_CHAR_INFO_H

#include "../headers.h"

struct ServerPacketCharInfo {
        int x;
        int y;
        int z;
        int heading;
        int char_id;
        l2_string char_name[28];
        int race_id;
        int sex;
        int class_id;
        int empty[8];
        int paperdoll[10];
        int pvp_flag;
        int karma;
        int m_attack_speed;
        int p_attack_speed;
        int pvp_flag_copy;
        int karma_copy;
        int run_speed;
        int walk_speed;
        int swim_run_speed;
        int swim_walk_speed;
        int fly_run_speed;
        int fly_walk_speed;
        int fly_run_speed_copy;
        int fly_walk_speed_copy;
        double movement_multiplier;
        double attack_speed_multiplier;
        double collision_radius;
        double collision_height;
        int hair_style_id;
        int hair_color_id;
        int face;
        l2_string title[28];
        int clan_id;
        int clan_crest_id;
        int ally_id;
        int ally_crest_id;
        char standing;
        char running;
        char in_combat;
        char alike_dead;
        char invisible;
        char mount_type;
        char private_store_type;
        short cubics;
        char find_party_member;
        int abnormal_effect;
        char empty_char[2];
        short recommendations_left;
        char mounted;
        int clan_crest_large_id;
        char hero_symbol;
        char hero_aura;
        char fishing;
        int name_color;
};

#define SERVER_PACKET_CHAR_INFO_SIZE sizeof(struct ServerPacketCharInfo)
#define SERVER_PACKET_CHAR_INFO_FULL_SIZE sizeof(short) + sizeof(char) + SERVER_PACKET_CHAR_INFO_SIZE

void server_packet_char_info(packet *dest);

#endif
