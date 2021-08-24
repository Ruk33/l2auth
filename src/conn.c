#include <assert.h>
#include "include/util.h"
#include "include/log.h"
#include "include/os_io.h"
#include "include/conn.h"

static conn_send_response_cb send_response = 0;
static conn_disconnect_cb disconnect       = 0;

void conn_set_send_response(conn_send_response_cb cb)
{
        assert(cb);
        send_response = cb;
}

void conn_set_disconnect(conn_disconnect_cb cb)
{
        assert(cb);
        disconnect = cb;
}

void conn_send_response(struct os_io *socket, void *buf, size_t n)
{
        assert(socket);
        assert(send_response && "send response callback is not set.");
        send_response(socket, buf, n);
}

void conn_disconnect(struct os_io *socket)
{
        assert(socket);
        assert(disconnect && "disconnect callback is not set.");
        disconnect(socket);
}
