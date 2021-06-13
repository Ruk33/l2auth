#include <stdio.h>
#include "../../include/os_socket.h"
#include "../../include/ls_lib.h"

static void send_response(os_socket_t *socket, unsigned char *buf, size_t n)
{
        os_socket_send(socket, buf, n);
}

static void
on_request(os_socket_t *socket, socket_ev_t ev, unsigned char *buf, size_t n)
{
        switch (ev) {
        case CONN:
                printf("New connection.\n");
                ls_lib_new_conn(socket);
                break;
        case REQ:
                printf("New request.\n");
                ls_lib_new_req(socket, buf, n);
                break;
        case CLOSED:
                printf("Disconnect.\n");
                ls_lib_disconnect(socket);
                break;
        default:
                break;
        }

        fflush(stdout);
}

int main(/* int argc, char **argv */)
{
        os_socket_t *socket = 0;

        socket = os_socket_create(2106);

        if (!socket) {
                printf("Login server socket couldn't be created.\n");
                return 1;
        }

        ls_lib_load(send_response);

        if (!os_socket_handle_requests(socket, on_request)) {
                printf("Login server request can't be handled.\n");
                return 1;
        }

        printf("Login server listening for requests.\n");

        return 0;
}
