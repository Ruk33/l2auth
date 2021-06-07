#include <assert.h>
#include "include/util.h"
#include "include/os_socket.h"
#include "include/conn.h"
#include "include/storage.h"
#include "include/gs_session.h"
#include "include/gs_request.h"
#include "include/gs_lib.h"

void gs_lib_load(
        conn_send_response_cb send_response_cb,
        gs_session_save_cb save_sessions_cb)
{
        assert(send_response_cb);
        assert(save_sessions_cb);

        conn_set_cb(send_response_cb);
        storage_open();
        gs_session_set_save_cb(save_sessions_cb);
}

void gs_lib_unload(void)
{
        storage_close();
        gs_sessions_save();
}

void gs_lib_load_sessions(void *sessions, size_t n)
{
        if (!sessions || !n) {
                return;
        }
        gs_session_load(sessions, n);
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
