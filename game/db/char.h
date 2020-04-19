#ifndef L2AUTH_GAME_DB_CHAR_H
#define L2AUTH_GAME_DB_CHAR_H

#include <stdlib.h>
#include <game/db/conn.h>
#include <game/dto/char.h>

/**
 * Persist character into database
 */
void game_db_char_save(game_db_conn* db, struct GameDtoChar* character);

/**
 * Get how_many characters from database
 * Returns characters found
 */
int game_db_char_list
(
        game_db_conn* db,
        struct GameDtoChar** characters,
        size_t how_many
);

/**
 * Get character from index
 */
void game_db_char_get_from_index
(
        game_db_conn* db,
        struct GameDtoChar* character
);

#endif
