#include <assert.h>
#include <stdio.h>
#include "../os_io.c"
#include "../../ls_lib.c"

static void _send_response(struct os_io *socket, void *buf, size_t n)
{
        assert(socket);
        os_io_write(socket, buf, n);
}

static void
on_request(struct os_io *socket, os_io_event_t event, void *buf, size_t n)
{
        assert(socket);

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
        struct os_io *socket = 0;

        socket = os_io_socket_create(2106, 30);

        if (!socket) {
                printf("login server socket couldn't be created.\n");
                return 1;
        }

        ls_lib_load(_send_response);

        if (!os_io_listen(on_request)) {
                printf("login server request can't be handled.\n");
                return 1;
        }

        printf("login server listening for requests.\n");

        return 0;
}
