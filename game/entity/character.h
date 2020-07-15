#ifndef L2AUTH_GAME_ENTITY_CHARACTER_H
#define L2AUTH_GAME_ENTITY_CHARACTER_H

#include "vec3.h"

struct Character {
        int id;
        char name[35];
        char title[16];
        int level;
        int str;
        int con;
        int dex;
        int _int;
        int men;
        int wit;
        int sex;
        int exp;
        int sp;
        int p_attack;
        int p_defense;
        int m_attack;
        int m_defense;
        int attack_speed;
        int m_attack_speed;
        int walk_speed;
        int run_speed;
        int hp;
        int current_hp;
        int mp;
        int current_mp;
        int heading;
        int x;
        int y;
        int z;
        int accuracy;
        int critical;
        int evasion;
};

void character_run_to(struct Character *character, struct Vec3 *location);

void character_walk_to(struct Character *character, struct Vec3 *location);

#endif
