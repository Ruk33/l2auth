#ifndef LIB_SERVER_PACKET_NPC_INFO_H
#define LIB_SERVER_PACKET_NPC_INFO_H

#include <headers.h>
#include <character.h>

typedef struct {
        int obj_id;
        int template_id;
        int attackable;
        int x;
        int y;
        int z;
        int heading;
        int after_heading_empty;
        int m_atk_speed;
        int p_atk_speed;
        int run_speed;
        int walk_speed;
        int swim_run_speed;
        int swim_walk_speed;
        int fly_run_speed;
        int fly_walk_speed;
        int fly_run_speed_2; // Ignore
        int fly_walk_speed_2; // Ignore
        double multiplier;
        double p_atk_speed_d;
        double collision_radius;
        double collision_height;
        int r_hand;
        int after_r_hand_empty;
        int l_hand;
        char name_above_char;
        char running;
        char in_combat;
        char alike_dead;
        char summoned;
        l2_string_t name[18]; // Hardcoded, not sure if this is the correct limit
        l2_string_t title[18]; // Hardcoded, not sure if this is the correct limit
        int after_title_empty[2];
        int karma;
        int abnormal_effect;
        int c2_empty[4];
        char c2_char_empty;
        char team_circle;
        double ignore_collision_radius;
        double ignore_collision_height;
        int c4_empty;
} server_packet_npc_info_t;

void server_packet_npc_info(packet *dest, character_t *npc);

#endif
