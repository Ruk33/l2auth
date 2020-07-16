#ifndef L2AUTH_GAME_STORAGE_CHARACTER_H
#define L2AUTH_GAME_STORAGE_CHARACTER_H

#include <stdlib.h>
#include <game/entity/player.h>
#include "connection.h"

/**
 * Persist character into database
 */
void game_storage_character_save(game_storage_connection* db, struct Player* player);

/**
 * Get how_many characters from database
 * Returns characters found
 */
int game_storage_character_list
(
        game_storage_connection* db,
        struct Player** players,
        size_t how_many
);

/**
 * Get character from index
 */
void game_storage_character_from_index
(
        game_storage_connection* db,
        struct Player* player,
        int index
);

#endif
