#include <assert.h>
#include "include/util.h"
#include "include/conn.h"
#include "include/log.h"
#include "include/gs_session.h"
#include "include/gs_request.h"
#include "include/gs_lib.h"

void gs_lib_load(conn_send_response_cb cb, byte_t *sessions)
{
        assert(cb);
        assert(sessions);

        log("GS loaded!!!!!!!!!!!!");

        conn_set_cb(cb);
        gs_session_set(sessions);
}

void gs_lib_unload(void)
{
}

void gs_lib_new_conn(void *socket)
{
        assert(socket);
        gs_request_new_conn(socket);
}

void gs_lib_new_req(void *socket, byte_t *buf, size_t n)
{
        assert(socket);
        gs_request(socket, buf, n);
}

void gs_lib_disconnect(void *socket)
{
        assert(socket);
        gs_request_disconnect(socket);
}
