#include <stdio.h>
#include "../../include/socket.h"
#include "../../include/login_server_lib.h"

static void send_response(socket_t *socket, byte_t *buf, size_t n)
{
        printf("Sending response.\n");
        socket_send(socket, buf, n);
        fflush(stdout);
}

static void on_request(socket_t *socket, socket_ev_t ev, byte_t *buf, size_t n)
{
        switch (ev) {
        case CONN:
                printf("New connection.\n");
                login_server_lib_new_conn(socket);
                break;
        case REQ:
                printf("New request.\n");
                login_server_lib_new_req(socket, buf, n);
                break;
        case CLOSED:
                printf("Disconnect.\n");
                login_server_lib_disconnect(socket);
                break;
        default:
                break;
        }

        fflush(stdout);
}

int main(/*int argc, char **argv*/)
{
        socket_t *socket = 0;

        socket = socket_create(2106);

        if (!socket) {
                printf("Socket couldn't be created.\n");
                return 1;
        }

        login_server_lib_load(send_response);
        socket_handle_requests(socket, on_request);

        return 0;
}
