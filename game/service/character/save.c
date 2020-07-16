#include <assert.h>
#include <log/log.h>
#include <game/storage/connection.h>
#include <game/storage/character.h>
#include <game/entity/player.h>
#include "save.h"

void game_service_character_save(struct Player* player)
{
        assert(player);

        log_info("Persisting character into database");

        game_storage_connection* db = game_storage_connection_open();
        game_storage_character_save(db, player);
        game_storage_connection_close(db);
}
