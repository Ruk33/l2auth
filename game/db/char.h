#ifndef L2AUTH_GAME_DB_CHAR_H
#define L2AUTH_GAME_DB_CHAR_H

#include <stdlib.h>
#include <game/db/conn.h>
#include <game/dto/char.h>

void game_db_char_save(game_db_conn* db, struct GameDtoChar* character);

int game_db_char_list
(
        game_db_conn* db,
        struct GameDtoChar** characters,
        size_t how_many
);

#endif
