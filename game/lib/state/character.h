#ifndef LIB_STATE_CHARACTER_H
#define LIB_STATE_CHARACTER_H

#include <data_structures/list.h>
#include <character.h>

typedef struct {
        void *handler;
} state_character_t;

/**
 * Add a new character to the state of the world.
 * This means the character is in game and
 * will get assigned a unique object id.
 */
void state_character_add(state_character_t *state, character_t *character);

/**
 * Remove character from state.
 */
void state_character_remove(state_character_t *state, int id);

/**
 * Get a character by id and store in dest.
 * If a character gets found, 1 will be returned.
 * If no character is found, 0 will be returned.
 */
int state_character_get_by_id(
        state_character_t *state,
        character_t *      dest,
        int                id);

/**
 * Add request to move character to a position
 * to a work queue. The work queue will be handled
 * by a timer.
 */
void state_character_move(state_character_t *state, int id, position_t *to);

#endif
