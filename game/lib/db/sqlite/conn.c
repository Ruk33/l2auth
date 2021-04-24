#include <headers.h>
#include <sqlite3.h>
#include "../conn.h"

/**
 * Note: Need a way to log stuff in case conn can't be opened.
 */

int db_conn_open(db_conn_t **db)
{
        /**
         * TODO: refactor hardcoded db path.
         */
        char *db_path = "/mnt/c/Users/Franco/l2auth.db";
        return sqlite3_open(db_path, (sqlite3 **) db) == SQLITE_OK;
}

void db_conn_close(db_conn_t *db)
{
        assert(db);
        sqlite3_close((sqlite3 *) db);
}
