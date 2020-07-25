#include <stdlib.h>
#include <sqlite3.h>
#include <log/log.h>
#include "../conn.h"

size_t conn_handler_size()
{
        return sizeof(sqlite3 *);
}

void conn_open(conn_handler *conn)
{
        sqlite3 *sqlite = conn;
        sqlite3_open("gameserver.db", &sqlite);
}

void conn_close(conn_handler *conn)
{
        sqlite3_close(conn);
}
