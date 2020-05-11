#include <assert.h>
#include <stdlib.h>
#include <game/storage/connection.h>
#include <game/storage/character.h>
#include <game/entity/character.h>
#include "list.h"

int game_service_character_list(struct GameEntityCharacter** characters, size_t how_many)
{
        assert(characters);

        game_storage_connection* db = game_storage_connection_open();
        int count = game_storage_character_list(db, characters, how_many);

        game_storage_connection_close(db);

        return count;
}
