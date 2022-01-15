#ifndef INCLUDE_GS_REQUEST_H
#define INCLUDE_GS_REQUEST_H

#include "util.h"
#include "platform.h"
#include "gs_types.h"

void gs_request_new_conn(struct gs_state *state,
                         struct platform_socket *socket);

void gs_request(struct gs_state *state,
                struct platform_socket *socket,
                void *buf,
                size_t n);

void gs_request_disconnect(struct gs_state *state,
                           struct platform_socket *socket);

void gs_request_tick(struct gs_state *state, double delta);

#endif
