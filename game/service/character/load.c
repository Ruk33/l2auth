#include <assert.h>
#include <game/storage/connection.h>
#include <game/storage/character.h>
#include <game/entity/player.h>
#include "load.h"

void game_service_character_load(int index, struct Player* player)
{
        assert(player);

	game_storage_connection* db = game_storage_connection_open();
        game_storage_character_from_index(db, player, index);
	game_storage_connection_close(db);
}

