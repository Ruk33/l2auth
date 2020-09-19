#include <assert.h>
#include <stdlib.h>
#include <sqlite3.h>
#include "../conn.h"

conn_handler *conn_open
(void)
{
        sqlite3 *sqlite = NULL;
        sqlite3_open("gameserver.db", &sqlite);
        return sqlite;
}

void conn_close
(conn_handler *conn)
{
        assert(conn);
        sqlite3_close(conn);
}
