#include <assert.h>
#include <game/storage/connection.h>
#include <game/storage/character.h>
#include <game/entity/character.h>
#include "load.h"

void game_service_character_load(int index, struct GameEntityCharacter* character)
{
        assert(character);

	game_storage_connection* db = game_storage_connection_open();
        game_storage_character_from_index(db, character);
	game_storage_connection_close(db);
}

