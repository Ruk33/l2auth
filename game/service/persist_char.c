#include <assert.h>
#include <log/log.h>
#include <game/db/conn.h>
#include <game/db/char.h>
#include <game/dto/char.h>
#include <game/service/persist_char.h>

void game_service_persist_char(struct GameDtoChar* character)
{
        assert(character);

        log_info("Persisting character into database");

        game_db_conn* db = game_db_conn_open();
        game_db_char_save(db, character);
        game_db_conn_close(db);
}
