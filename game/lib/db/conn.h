#ifndef LIB_STORAGE_SERVER_H
#define LIB_STORAGE_SERVER_H

typedef void db_conn_t;

/**
 * On success, a db conn handler is returned.
 * On error, NULL.
 */
db_conn_t *db_conn_open(void);

void db_conn_close(db_conn_t *conn);

#endif
