#include <assert.h>
#include <sqlite3.h>
#include "../connection.h"

game_storage_connection* game_storage_connection_open()
{
        sqlite3* sqlite;
        sqlite3_open("gameserver.db", &sqlite);
        return sqlite;
}

void game_storage_connection_close(game_storage_connection* db)
{
        assert(db);
        sqlite3_close(db);
}
