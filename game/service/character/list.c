#include <assert.h>
#include <stdlib.h>
#include <game/storage/connection.h>
#include <game/storage/character.h>
#include <game/entity/player.h>
#include "list.h"

int game_service_character_list(struct Player** players, size_t how_many)
{
        assert(players);

        game_storage_connection* db = game_storage_connection_open();
        int count = game_storage_character_list(db, players, how_many);

        game_storage_connection_close(db);

        return count;
}
