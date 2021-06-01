#include "include/util.h"
#include "include/socket.h"
#include "include/conn.h"

static conn_send_response_cb send_response = 0;

void conn_set_cb(conn_send_response_cb cb)
{
        send_response = cb;
}

void conn_send_response(socket_t *socket, byte_t *buf, size_t n)
{
        send_response(socket, buf, n);
}
