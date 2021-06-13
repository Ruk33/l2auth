#ifndef INCLUDE_GS_PACKET_CHAR_INFO_H
#define INCLUDE_GS_PACKET_CHAR_INFO_H

#include "util.h"
#include "packet.h"
#include "l2_string.h"
#include "gs_character.h"

typedef struct {
        i32_t x;
        i32_t y;
        i32_t z;
        i32_t heading;
        u32_t id;
        l2_string_t name[28];
        u32_t race_id;
        u32_t sex;
        u32_t class_id;
        // int empty[8]
        // int paperdoll[10]
        u32_t pvp_flag;
        u32_t karma;
        u32_t m_attack_speed;
        u32_t p_attack_speed;
        u32_t run_speed;
        u32_t walk_speed;
        u32_t swim_run_speed;
        u32_t swim_walk_speed;
        u32_t fly_run_speed;
        u32_t fly_walk_speed;
        double movement_multiplier;
        double attack_speed_multiplier;
        double collision_radius;
        double collision_height;
        u32_t hair_style;
        u32_t hair_color;
        u32_t face;
        l2_string_t title[28];
        u32_t clan_id;
        u32_t clan_crest_id;
        u32_t ally_id;
        u32_t ally_crest_id;
        u8_t standing;
        u8_t running;
        u8_t in_combat;
        u8_t alike_dead;
        u8_t invisible;
        u8_t mount_type;
        u8_t private_store_type;
        u16_t cubics;
        u8_t find_party_member;
        u32_t abnormal_effect;
        // char empty_char[2]
        u16_t recommendations_left;
        u8_t mounted;
        u32_t clan_crest_large_id;
        u8_t hero_symbol;
        u8_t hero_aura;
        u8_t fishing;
        u32_t name_color;
} gs_packet_char_info_t;

void gs_packet_char_info(gs_packet_char_info_t *dest, gs_character_t *src);

void gs_packet_char_info_pack(packet_t *dest, gs_packet_char_info_t *src);

#endif
