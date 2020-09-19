#ifndef STORAGE_CONN_H
#define STORAGE_CONN_H

#include <data_structure/list.h>

typedef void conn_handler;

conn_handler *conn_open
(void);

void conn_close
(conn_handler *conn);

#endif
