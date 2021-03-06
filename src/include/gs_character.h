#ifndef INCLUDE_GS_CHARACTER_H
#define INCLUDE_GS_CHARACTER_H

#include "util.h"
#include "gs_session.h"
#include "gs_packet_create_char_request.h"

/*
 * Same as gs_session, these values represent
 * the states in which a character can be.
 * Keep these in order.
 */
typedef enum {
        SPAWN,
        IDLE,
        MOVING,
        TARGET_SELECTED,
        MOVING_TO_ATTACK,
        ATTACKING,
} gs_character_state_t;

typedef struct {
        u32_t id;
        gs_session_t *session;
        gs_character_state_t state;

        i32_t target_x;
        i32_t target_y;
        i32_t target_z;
        i32_t heading;

        u32_t target_id;
        u32_t attack_cd;

        // Fields bellow these line will be saved in database.
        char name[32];
        char title[32];
        u32_t race;
        u32_t sex;
        u32_t _class;
        i32_t _int;
        i32_t str;
        i32_t con;
        i32_t men;
        i32_t dex;
        i32_t wit;
        u32_t hair_style;
        u32_t hair_color;
        u32_t face;
        u32_t level;
        u32_t exp;
        u32_t sp;
        u32_t hp;
        u32_t mp;
        u32_t cp;
        u32_t max_hp;
        u32_t max_mp;
        u32_t max_cp;
        u32_t p_attack;
        u32_t m_attack;
        u32_t p_def;
        u32_t m_def;
        u32_t evasion_rate;
        u32_t accuracy;
        u32_t critical_hit;
        u32_t run_speed;
        u32_t walk_speed;
        u32_t p_attack_speed;
        u32_t m_attack_speed;
        double movement_speed_multiplier;
        double attack_speed_multiplier;
        double collision_radius;
        double collision_height;
        u32_t name_color;
        i32_t max_load;
        i32_t x;
        i32_t y;
        i32_t z;
} gs_character_t;

void gs_character_set(gs_character_t *characters, size_t *count);

// Fill character d from client request (made when
// creating a new character).
void gs_character_from_request(
        gs_character_t *dest,
        gs_packet_create_char_request_t *src);

// Spawn character in the world and notify close players.
// If player, the session must be previously assigned, otherwise, it will
// be threated as a NPC.
void gs_character_spawn(gs_character_t *src);

gs_character_t *gs_character_from_session(gs_session_t *session);

void gs_character_tick(double delta);

void gs_character_request(gs_character_t *character, packet_t *packet);

#endif
