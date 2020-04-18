#include <assert.h>
#include <stdlib.h>
#include <game/db/conn.h>
#include <game/db/char.h>
#include <game/dto/char.h>
#include <game/service/char_list.h>

int game_service_char_list(struct GameDtoChar** characters, size_t how_many)
{
        assert(characters);

        game_db_conn* db = game_db_conn_open();
        int count = game_db_char_list(db, characters, how_many);

        game_db_conn_close(db);

        return count;
}
