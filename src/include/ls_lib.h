#ifndef INCLUDE_LS_LIB_H
#define INCLUDE_LS_LIB_H

#include "util.h"
#include "platform.h"
#include "ls_types.h"

void ls_lib_load(struct ls_state *ls);

void ls_lib_new_conn(struct platform_socket *socket);

void ls_lib_new_req(struct platform_socket *socket, void *buf, size_t n);

void ls_lib_disconnect(struct platform_socket *socket);

#endif
