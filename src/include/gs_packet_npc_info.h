#ifndef INCLUDE_GS_PACKET_NPC_INFO_T
#define INCLUDE_GS_PACKET_NPC_INFO_T

#include "util.h"
#include "gs_types.h"
#include "packet.h"
#include "l2_string.h"

typedef struct {
        u32_t id;
        u32_t template_id;
        u32_t attackable;
        i32_t x;
        i32_t y;
        i32_t z;
        i32_t heading;
        u32_t m_attack_speed;
        u32_t p_attack_speed;
        u32_t run_speed;
        u32_t walk_speed;
        u32_t swim_run_speed;
        u32_t swim_walk_speed;
        u32_t fly_run_speed;
        u32_t fly_walk_speed;
        double magic_multiplier;
        double movement_multiplier;
        double collision_radius;
        double collision_height;
        u32_t r_hand;
        u32_t l_hand;
        u8_t name_above_char;
        u8_t running;
        u8_t in_combat;
        u8_t alike_dead;
        u8_t summoned;
        l2_string_t name[28];
        l2_string_t title[28];
        u32_t karma; // ?
        u32_t abnormal_effect;
        u8_t team_circle;
} gs_packet_npc_info_t;

void gs_packet_npc_info(gs_packet_npc_info_t *dest, struct gs_character *src);

void gs_packet_npc_info_pack(packet_t *dest, gs_packet_npc_info_t *src);

#endif
