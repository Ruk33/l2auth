#ifndef INCLUDE_LS_REQUEST_H
#define INCLUDE_LS_REQUEST_H

#include "util.h"
#include "os_io.h"

void ls_request_new_conn(os_io_t *socket);

void ls_request(os_io_t *socket, byte_t *buf, size_t n);

void ls_request_disconnect(os_io_t *socket);

#endif
