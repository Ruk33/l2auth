#ifndef INCLUDE_GS_REQUEST_H
#define INCLUDE_GS_REQUEST_H

#include "util.h"
#include "os_io.h"

void gs_request_new_conn(os_io_t *socket);

void gs_request(os_io_t *socket, byte_t *buf, size_t n);

void gs_request_disconnect(os_io_t *socket);

#endif
