#ifndef INCLUDE_LS_LIB_H
#define INCLUDE_LS_LIB_H

#include "util.h"
#include "os_io.h"
#include "conn.h"

void ls_lib_load(conn_send_response_cb cb);

void ls_lib_new_conn(struct os_io *socket);

void ls_lib_new_req(struct os_io *socket, void *buf, size_t n);

void ls_lib_disconnect(struct os_io *socket);

#endif
