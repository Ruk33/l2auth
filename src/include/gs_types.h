#ifndef INCLUDE_GS_TYPES_H
#define INCLUDE_GS_TYPES_H

#include <time.h>

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
        AI_DEAD,
};

struct gs_session {
        u32_t id;
        struct os_io *socket;

        enum gs_session_state state;

        byte_t encrypt_key[8];
        byte_t decrypt_key[8];

        i32_t playOK1;
        i32_t playOK2;
        i32_t loginOK1;
        i32_t loginOK2;

        char username[MAX_USERNAME_SIZE];
        int conn_encrypted;
        size_t character_index;
};

struct gs_point {
        i32_t x;
        i32_t y;
        i32_t z;
};

// TODO: Refactor, find our own way.
// Thanks to L2J project for providing this first iteration.
struct gs_move_data {
        u64_t move_timestamp;
        struct gs_point destination;
        struct gs_point origin;
        i32_t heading;
        u64_t move_start_time;
        u32_t ticks_to_move;
        double x_speed_ticks;
        double y_speed_ticks;
};

struct gs_ai {
        enum gs_ai_state state;
        u32_t target_id;
        struct gs_point moving_to;
        struct gs_move_data move_data;
        double attack_cd;
        double idle_cd;
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
        u32_t template_id;
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
        u8_t running;
};

struct gs_state {
        struct gs_session sessions[MAX_CLIENTS];
        struct list list_sessions[MAX_CLIENTS];

        struct gs_character characters[MAX_CHARACTERS];
        struct list list_characters[MAX_CHARACTERS];

        u64_t game_ticks;
        time_t game_start_time;

        void (*send_response)(struct os_io *socket, void *buf, size_t n);
        void (*disconnect)(struct os_io *socket);
};

#endif
