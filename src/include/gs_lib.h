#ifndef INCLUDE_GS_LIB_H
#define INCLUDE_GS_LIB_H

#include "util.h"
#include "config.h"
#include "conn.h"
#include "gs_session.h"

typedef struct {
        conn_send_response_cb send_response;
        gs_session_t sessions[MAX_CLIENTS];
        size_t session_count;
} gs_lib_t;

void gs_lib_load(conn_send_response_cb cb, byte_t *sessions);

void gs_lib_unload(void);

void gs_lib_new_conn(void *socket);

void gs_lib_new_req(void *socket, byte_t *buf, size_t n);

void gs_lib_disconnect(void *socket);

#endif
