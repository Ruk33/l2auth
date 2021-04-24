#ifndef LIB_DB_SERVER_H
#define LIB_DB_SERVER_H

typedef void db_conn_t;

void db_conn_open(db_conn_t **db);

void db_conn_close(db_conn_t *db);

#endif
