#ifndef INCLUDE_GS_LIB_H
#define INCLUDE_GS_LIB_H

#include "util.h"
#include "gs_types.h"
#include "config.h"
#include "platform.h"
#include "gs_character.h"

void gs_lib_load(struct gs_state *gs);

void gs_lib_unload(void);

void gs_lib_new_conn(struct platform_socket *socket);

void gs_lib_new_req(struct platform_socket *socket, void *buf, size_t n);

void gs_lib_disconnect(struct platform_socket *socket);

void gs_lib_tick(double delta);

#endif
