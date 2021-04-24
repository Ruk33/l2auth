#include <headers.h>
#include <sqlite3.h>
#include "../conn.h"

/**
 * Note: Need a way to log stuff in case conn can't be opened.
 */

void db_conn_open(db_conn_t **db)
{
        sqlite3_open("/mnt/c/Users/Franco/l2auth.db", (sqlite3 **) db);
}

void db_conn_close(db_conn_t *db)
{
        assert(db);
        sqlite3_close((sqlite3 *) db);
}
