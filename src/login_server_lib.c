#include "include/login_server.h"
#include "include/login_server_request.h"
#include "include/login_server_lib.h"

void login_server_lib_load(send_response_cb cb)
{
        login_server_set_send_response(cb);
}

void login_server_lib_new_conn(socket_t *socket)
{
        login_server_request_new_conn(socket);
}

void login_server_lib_new_req(socket_t *socket, byte_t *buf, size_t n)
{
        login_server_request(socket, buf, n);
}

void login_server_lib_disconnect(socket_t *socket)
{
        login_server_request_disconnect(socket);
}
