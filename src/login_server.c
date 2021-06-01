#include "include/util.h"
#include "include/socket.h"
#include "include/login_server.h"

static send_response_cb send_response = 0;

void login_server_set_send_response(send_response_cb cb)
{
        send_response = cb;
}

// void login_server_log(char *msg)
// {
//         printf("%s\n", msg);
//         fflush(stdout);
// }

void login_server_send(socket_t *socket, byte_t *buf, size_t n)
{
        login_server_log("Sending packet of size %ld", n);
        send_response(socket, buf, n);
}
