#ifndef LIB_DB_SERVER_H
#define LIB_DB_SERVER_H

typedef void db_conn_t;

/**
 * Open a db conn.
 * On success, 1 will be returned.
 * On failure, 0.
 */
int db_conn_open(db_conn_t **db);

void db_conn_close(db_conn_t *db);

#endif
