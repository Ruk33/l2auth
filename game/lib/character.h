#ifndef LIB_CHARACTER_H
#define LIB_CHARACTER_H

#include "session.h"
#include "vec3.h"

typedef enum {
        SPAWN,
        IDLE,
        RUNNING,
        WALKING,
} character_state_t;

typedef struct {
        session_t *       session;
        character_state_t state;
        int               id;
        char   name[20];  // Hardcoded, not sure if it's the correct max limit
        char   title[20]; // Hardcoded, not sure if it's the correct max limit
        int    active;
        double hp;
        double mp;
        double max_hp;
        double max_mp;
        int    cp;
        int    max_cp;
        int    level;
        int    clan_id;
        int    sex;
        int    race_id;
        int    class_id;
        int    x;
        int    y;
        int    z;
        int    sp;
        int    exp;
        int    karma;
        int    _int;
        int    str;
        int    con;
        int    men;
        int    dex;
        int    wit;
        int    p_attack;
        int    m_attack;
        int    p_def;
        int    m_def;
        int    evasion_rate;
        int    critical_hit;
        int    target_x;
        int    target_y;
        int    target_z;
} character_t;

/**
 * Calculate and update new character's
 * position. This function is usually
 * called when a player clicks on the
 * map to move the character.
 */
void character_move(character_t *character, vec3_t *target);

/**
 * When a character moves, its target position
 * get updated and the position gets updated
 * every second with this function.
 */
void character_move_forward(character_t *character);

/**
 * Make sure the character's position is close
 * to the position calculated by the server.
 * When moving, the client sends periodically
 * a packet (validate position) with the server
 * to make sure the position of the character
 * is in sync.
 * If the position sent by the client (position_to_validate)
 * is close enough to the character's position, we
 * don't apply any correction.
 * If the position is too far from the character's
 * position, we correct the position by setting
 * to the position calculated by the server. This
 * prevents the client from cheating or, in case
 * of slow connections, to have a more correct position.
 */
void character_validate_position(
        character_t *character,
        position_t * validated_position,
        position_t * position_to_validate,
        int          heading);

/**
 * Update character's internal state.
 * Used for state machine.
 */
void character_update_state(character_t *character, character_state_t new_state);

#endif
