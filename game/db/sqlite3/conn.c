#include <assert.h>
#include <sqlite3.h>
#include <game/db/conn.h>

game_db_conn* game_db_conn_open()
{
        sqlite3* sqlite;
        sqlite3_open("gameserver.db", &sqlite);
        return sqlite;
}

void game_db_conn_close(game_db_conn* db)
{
        assert(db);
        sqlite3_close(db);
}
