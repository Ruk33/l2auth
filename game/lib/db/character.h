#ifndef LIB_DB_CHARACTER_H
#define LIB_DB_CHARACTER_H

#include <headers.h>
#include <character.h>
#include "conn.h"

/**
 * These characters represent characters in the world.
 * Characters can refer to NPC or Players.
 * This information is meant to be stored temporarily
 * (think of it as a cache).
 */

typedef unsigned int char_obj_id_t;

/**
 * Add a character to the world.
 * Returns its unique identifier.
 */
char_obj_id_t db_character_add(db_conn_t *db, character_t *character);

/**
 * Get character with id and store it in dest.
 */
void db_character_get(db_conn_t *db, character_t *dest, char_obj_id_t id);

/**
 * Remove character from world.
 */
void db_character_remove(db_conn_t *db, char_obj_id_t id);

/**
 * Update character with values stored in character.
 */
void db_character_update(db_conn_t *db, char_obj_id_t id, character_t *src);

/**
 * Get up until max amount of characters inside of radius.
 */
size_t db_character_in_radius(
        db_conn_t *  db,
        character_t *dest,
        size_t       max,
        position_t * pos,
        int          radius);

#endif
