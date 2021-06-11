#include <assert.h>
#include "include/util.h"
#include "include/conn.h"
#include "include/log.h"
#include "include/os_socket.h"
#include "include/gs_session.h"
#include "include/gs_character.h"
#include "include/gs_request.h"
#include "include/gs_lib.h"

void gs_lib_load(gs_lib_t *gs_lib)
{
        assert(gs_lib);

        log("GS loaded!");

        conn_set_cb(gs_lib->send_response);
        gs_session_set(gs_lib->sessions, &gs_lib->session_count);
        gs_character_set(gs_lib->characters, &gs_lib->character_count);
}

void gs_lib_unload(void)
{
}

void gs_lib_new_conn(os_socket_t *socket)
{
        assert(socket);
        gs_request_new_conn(socket);
}

void gs_lib_new_req(os_socket_t *socket, byte_t *buf, size_t n)
{
        assert(socket);
        gs_request(socket, buf, n);
}

void gs_lib_disconnect(os_socket_t *socket)
{
        assert(socket);
        gs_request_disconnect(socket);
}
