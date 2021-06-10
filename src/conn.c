#include <assert.h>
#include "include/util.h"
#include "include/log.h"
#include "include/os_socket.h"
#include "include/conn.h"

static conn_send_response_cb send_response = 0;

void conn_set_cb(conn_send_response_cb cb)
{
        assert(cb);
        send_response = cb;
}

void conn_send_response(os_socket_t *socket, byte_t *buf, size_t n)
{
        assert(send_response && "Send response callback is not set.");
        send_response(socket, buf, n);
}
