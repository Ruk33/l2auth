#ifndef INCLUDE_GS_CHARACTER_H
#define INCLUDE_GS_CHARACTER_H

#include "gs_session.h"
#include "gs_packet_create_char_request.h"

typedef struct {
        gs_session_t *session;
        int id;
        char name[32];
        int race;
        int sex;
        int _class;
        int _int;
        int str;
        int con;
        int men;
        int dex;
        int wit;
        int hair_style;
        int hair_color;
        int face;
        int level;
        int exp;
        int sp;
        int hp;
        int mp;
        int cp;
        int max_hp;
        int max_mp;
        int max_cp;
        int p_attack;
        int m_attack;
        int p_def;
        int m_def;
        int evasion_rate;
        int critical_hit;
        int run_speed;
        int walk_speed;
        int p_attack_speed;
        int movement_speed_multiplier;
        int attack_speed_multiplier;
        int collision_radius;
        int collision_height;
        int name_color;
        int max_load;
        int x;
        int y;
        int z;
} gs_character_t;

// Fill character d from client request (made when
// creating a new character).
void gs_character_from_request(
        gs_character_t *dest,
        gs_packet_create_char_request_t *src);

// Spawn character in the world and notify close players.
// The session will get assign to the character.
void gs_character_spawn(gs_session_t *session, gs_character_t *src);

#endif
