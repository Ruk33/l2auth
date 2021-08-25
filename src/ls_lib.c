#include <assert.h>
#include "include/os_io.h"

#include "util.c"
#include "recycle_id.c"
#include "conn.c"
#include "packet.c"
#include "storage/sqlite.c"
#include "ls_request.c"
#include "ls_session.c"
#include "ls_server_packets.c"
#include "include/ls_lib.h"

void ls_lib_load(struct ls_lib *ls)
{
        assert(ls);
        assert(ls->send_response);
        assert(ls->disconnect);
        assert(ls->text_ip_to_u32);

        conn_set_send_response(ls->send_response);
        conn_set_disconnect(ls->disconnect);
        conn_set_text_ip_to_u32(ls->text_ip_to_u32);
}

void ls_lib_new_conn(struct os_io *socket)
{
        if (!socket) {
                return;
        }
        ls_request_new_conn(socket);
}

void ls_lib_new_req(struct os_io *socket, void *buf, size_t n)
{
        if (!socket) {
                return;
        }
        ls_request(socket, buf, n);
}

void ls_lib_disconnect(struct os_io *socket)
{
        if (!socket) {
                return;
        }
        ls_request_disconnect(socket);
}
