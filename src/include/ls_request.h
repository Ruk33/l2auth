#ifndef INCLUDE_LS_REQUEST_H
#define INCLUDE_LS_REQUEST_H

#include "util.h"
#include "platform.h"
#include "ls_types.h"

void ls_request_new_conn(struct ls_state *ls, struct platform_socket *socket);

void ls_request(
        struct ls_state *ls,
        struct platform_socket *socket,
        byte_t *buf,
        size_t n);

void ls_request_disconnect(struct ls_state *ls, struct platform_socket *socket);

#endif
