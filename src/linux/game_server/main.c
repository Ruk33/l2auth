#include <stdio.h>
#include "../../include/util.h"
#include "../../include/socket.h"
#include "../../include/game_server_lib.h"

static void send_response(socket_t *socket, byte_t *buf, size_t n)
{
        socket_send(socket, buf, n);
}

static void on_request(socket_t *socket, socket_ev_t ev, byte_t *buf, size_t n)
{
        switch (ev) {
        case CONN:
                printf("New connection.\n");
                game_server_lib_new_conn(socket);
                break;
        case REQ:
                printf("New request.\n");
                game_server_lib_new_req(socket, buf, n);
                break;
        case CLOSED:
                printf("Disconnect.\n");
                game_server_lib_disconnect(socket);
                break;
        default:
                break;
        }

        fflush(stdout);
}

int main(/* int argc, char **argv */)
{
        socket_t *socket = 0;

        socket = socket_create(7777);

        if (!socket) {
                printf("Game server socket couldn't be created.\n");
                return 1;
        }

        // login_server_lib_load(send_response);

        if (!socket_handle_requests(socket, on_request)) {
                printf("Game server request can't be handled.\n");
                return 1;
        }

        printf("Game server listening for requests.\n");

        return 0;
}
