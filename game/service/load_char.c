#include <assert.h>
#include <game/db/conn.h>
#include <game/db/char.h>
#include <game/dto/char.h>
#include <game/service/load_char.h>

void game_service_load_char(int index, struct GameDtoChar* character)
{
        assert(character);

	game_db_conn* db = game_db_conn_open();
        game_db_char_get_from_index(db, character);
	game_db_conn_close(db);
}

