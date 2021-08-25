#include <assert.h>

#include "include/os_io.h"
#include "include/ls_types.h"
#include "include/ls_lib.h"

#include "util.c"
#include "packet.c"
#include "storage/sqlite.c"
#include "ls_request.c"
#include "ls_session.c"
#include "ls_server_packets.c"

static struct ls_state *g_ls = 0;

void ls_lib_load(struct ls_state *ls)
{
        assert(ls);
        assert(ls->send_response);
        assert(ls->disconnect);
        assert(ls->text_ip_to_u32);

        g_ls = ls;
}

void ls_lib_new_conn(struct os_io *socket)
{
        if (!g_ls || !socket) {
                return;
        }
        ls_request_new_conn(g_ls, socket);
}

void ls_lib_new_req(struct os_io *socket, void *buf, size_t n)
{
        if (!g_ls || !socket) {
                return;
        }
        ls_request(g_ls, socket, buf, n);
}

void ls_lib_disconnect(struct os_io *socket)
{
        if (!g_ls || !socket) {
                return;
        }
        ls_request_disconnect(g_ls, socket);
}
