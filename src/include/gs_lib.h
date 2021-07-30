#ifndef INCLUDE_GS_LIB_H
#define INCLUDE_GS_LIB_H

#include "util.h"
#include "gs_types.h"
#include "config.h"
#include "conn.h"
#include "os_io.h"
#include "gs_character.h"

typedef struct {
        conn_send_response_cb send_response;

        struct gs_session sessions[MAX_CLIENTS];
        size_t session_count;

        struct gs_character characters[MAX_CLIENTS];
        size_t character_count;
} gs_lib_t;

void gs_lib_load(gs_lib_t *gs_lib);

void gs_lib_unload(void);

void gs_lib_new_conn(os_io_t *socket);

void gs_lib_new_req(os_io_t *socket, void *buf, size_t n);

void gs_lib_disconnect(os_io_t *socket);

void gs_lib_tick(double delta);

#endif
