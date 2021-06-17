#include <stdio.h>
#include "../../include/os_io.h"
#include "../../include/ls_lib.h"

static void send_response(os_io_t *socket, void *buf, size_t n)
{
        os_io_write(socket, buf, n);
}

static void
on_request(os_io_t *socket, os_io_event_t event, void *buf, size_t n)
{
        switch (event) {
        case OS_IO_SOCKET_CONNECTION:
                printf("New connection.\n");
                ls_lib_new_conn(socket);
                break;
        case OS_IO_SOCKET_REQUEST:
                printf("New request.\n");
                ls_lib_new_req(socket, buf, n);
                break;
        case OS_IO_SOCKET_DISCONNECTED:
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
        os_io_t *socket = 0;

        socket = os_io_socket_create(2106, 30);

        if (!socket) {
                printf("login server socket couldn't be created.\n");
                return 1;
        }

        ls_lib_load(send_response);

        if (!os_io_listen(on_request)) {
                printf("login server request can't be handled.\n");
                return 1;
        }

        printf("login server listening for requests.\n");

        return 0;
}
