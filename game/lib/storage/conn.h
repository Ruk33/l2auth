#ifndef STORAGE_CONN_H
#define STORAGE_CONN_H

#include <stdlib.h>

typedef void conn_handler;

size_t conn_handler_size();
void conn_open(conn_handler *conn);
void conn_close(conn_handler *conn);

#endif
