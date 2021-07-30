#ifndef INCLUDE_GS_TYPES_H
#define INCLUDE_GS_TYPES_H

#include "config.h"
#include "util.h"
#include "os_io.h"

/*
 * These represent the states of a client's session.
 * It's a state machine, which makes it easier to understand
 * the flow and avoid having to make some checks. For instance,
 * if we are in CHARACTER_SELECTION, we know we have went through
 * PROTOCOL_VERSION and AUTH_REQUEST, so we are dealing with
 * a valid session.
 * Keep these states in order.
 */
enum gs_session_state {
        PROTOCOL_VERSION,
        AUTH_REQUEST,
        CHARACTER_SELECTION,
        CREATING_CHARACTER,
        ENTERING_WORLD,
        IN_WORLD,
};

enum gs_character_state {
        SPAWN,
};

enum gs_ai_state {
        AI_IDLE,
        AI_MOVING,
        AI_TARGET_SELECTED,
        AI_MOVING_TO_ATTACK,
        AI_ATTACKING,
};

struct gs_session {
        u32_t id;
        os_io_t *socket;

        enum gs_session_state state;

        byte_t encrypt_key[8];
        byte_t decrypt_key[8];

        int playOK1;
        int playOK2;
        int loginOK1;
        int loginOK2;

        char username[MAX_USERNAME_SIZE];
        int conn_encrypted;
};

struct gs_point {
        i32_t x;
        i32_t y;
        i32_t z;
};

struct gs_ai {
        enum gs_ai_state state;
        u32_t target_id;
        struct gs_point moving_to;
        double attack_cd;
};

struct gs_stats {
        u32_t hp;
        u32_t mp;
        u32_t cp;
        u32_t max_hp;
        u32_t max_mp;
        u32_t max_cp;
        u32_t _int;
        u32_t str;
        u32_t con;
        u32_t men;
        u32_t dex;
        u32_t wit;
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
};

struct gs_character_template {
        u32_t race;
        u32_t _class;
        struct gs_stats stats;
};

struct gs_character {
        u32_t id;
        struct gs_session *session;
        struct gs_ai ai;
        struct gs_point position;
        struct gs_stats stats;
        char title[32];
        char name[32];
        u32_t name_color;
        i32_t heading;
        u32_t sex;
        u32_t race;
        u32_t _class;
        u32_t hair_style;
        u32_t hair_color;
        u32_t face;
        u32_t level;
        u32_t exp;
        u32_t sp;
        u32_t max_load;
        double collision_radius;
        double collision_height;
};

#endif
