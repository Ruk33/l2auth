#ifndef LIB_SERVER_PACKET_ENTER_WORLD_H
#define LIB_SERVER_PACKET_ENTER_WORLD_H

#include "../headers.h"
#include "../character.h"

struct ServerPacketEnterWorld {
        int x;
        int y;
        int z;
        int heading;
        int id;
        l2_string name[28];
        int race_id;
        int sex;
        int class_id;
        int level;
        int exp;
        int str;
        int dex;
        int con;
        int _int;
        int wit;
        int men;
        int max_hp;
        int hp;
        int max_mp;
        int mp;
        int sp;
        int current_load;
        int max_load;
        int unknown;
        int paperdoll_under;
        int paperdoll_r_ear;
        int paperdoll_l_ear;
        int paperdoll_neck;
        int paperdoll_r_finger;
        int paperdoll_l_finger;
        int paperdoll_head;
        int paperdoll_r_hand;
        int paperdoll_l_hand;
        int paperdoll_gloves;
        int paperdoll_chest;
        int paperdoll_legs;
        int paperdoll_feet;
        int paperdoll_back;
        int paperdoll_lr_hand;
        int paperdoll_hair;
        int empty[16];
        int p_attack;
        int p_attack_speed;
        int p_def;
        int evasion_rate;
        int accuracy;
        int critical_hit;
        int m_attack;
        int m_attack_speed;
        int p_attack_speed_copy;
        int m_def;
        int pvp;
        int karma;
        int run_speed;
        int walk_speed;
        int swim_run_speed;
        int swim_walk_speed;
        int fly_run_speed;
        int fly_walk_speed;
        int fly_run_speed_copy;
        int fly_walk_speed_copy;
        double movement_speed_multiplier;
        double attack_speed_multiplier;
        double collision_radius;
        double collision_height;
        int hair_style_id;
        int hair_color_id;
        int face;
        int access_level;
        l2_string title[28];
        int clan_id;
        int clan_crest_id;
        int ally_id;
        int ally_crest_id;
        int clan_leader;
        char mount_type;
        char private_store_type;
        char dwarven_craft;
        int pk_kills;
        int pvp_kills;
        short cubics;
        char party_members;
        int abnormal_effect;
        char char_empty[3];
        int clan_privileges;
        int int_empty[9];
        short recommendation_left;
        short recommendation_have;
        short inventory_limit;
        int class_id_copy;
        int max_cp;
        int cp;
        char mounted;
        int clan_crest_large_id;
        char hero_symbol;
        char hero;
        int fish_x;
        int fish_y;
        int fish_z;
        int name_color;
};

#define SERVER_PACKET_ENTER_WORLD_SIZE sizeof(struct ServerPacketEnterWorld)
#define SERVER_PACKET_ENTER_WORLD_FULL_SIZE sizeof(short) + sizeof(char) + SERVER_PACKET_ENTER_WORLD_SIZE

void server_packet_enter_world(packet *dest, character_t *character);

#endif
