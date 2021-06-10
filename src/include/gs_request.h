#ifndef INCLUDE_GS_REQUEST_H
#define INCLUDE_GS_REQUEST_H

#include "util.h"

void gs_request_new_conn(void *socket);

void gs_request(void *socket, byte_t *buf, size_t n);

void gs_request_disconnect(void *socket);

#endif
