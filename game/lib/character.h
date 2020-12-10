#ifndef LIB_CHARACTER_H
#define LIB_CHARACTER_H

#include "vec3.h"

struct Character {
        int id;
        // Hardcoded, not sure if it's the correct max limit
        char name[20];
        char title[20];
        int active;
        double hp;
        double mp;
        double max_hp;
        double max_mp;
        int cp;
        int max_cp;
        int level;
        int clan_id;
        int sex;
        int race_id;
        int class_id;
        int x;
        int y;
        int z;
        int sp;
        int exp;
        int karma;
        int _int;
        int str;
        int con;
        int men;
        int dex;
        int wit;
        int p_attack;
        int m_attack;
        int p_def;
        int m_def;
        int evasion_rate;
        int critical_hit;
};

typedef struct Character character_t;

/**
 * Calculate and update new character's
 * position. This function is usually
 * called when a player clicks on the
 * map to move the character.
 */
void character_move(character_t *character, vec3_t *target);

/**
 * Validate and correct (if necessary)
 * character's position. This function
 * is usually called when the client
 * sends a validate position request.
 */
void character_validate_position(character_t *character, vec3_t *pos, int heading);

#endif
