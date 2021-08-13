#ifndef UNICODE
#define UNICODE
#endif

#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <stdio.h>
#include "../os_io.c"
#include "../../ls_lib.c"

static void _send_response(struct os_io *socket, void *buf, size_t n)
{
        os_io_write(socket, buf, n);
}

static void
on_request(struct os_io *socket, os_io_event_t event, void *buf, size_t n)
{
        switch (event) {
        case OS_IO_SOCKET_CONNECTION:
                printf("new connection.\n");
                ls_lib_new_conn(socket);
                break;
        case OS_IO_SOCKET_REQUEST:
                printf("new request.\n");
                ls_lib_new_req(socket, buf, n);
                break;
        case OS_IO_SOCKET_DISCONNECTED:
                printf("disconnect.\n");
                ls_lib_disconnect(socket);
                break;
        case OS_IO_TIMER_TICK:
                break;
        case OS_IO_SOCKET_READ_ERROR:
                printf("socket error.\n");
                break;
        default:
                break;
        }
}

int main()
{
        struct os_io *socket = 0;

        socket = os_io_socket_create(2106, 30);

        if (!socket) {
                printf("login server socket couldn't be created.\n");
                return 1;
        }

        printf("login server listening for requests.\n");

        ls_lib_load(_send_response);

        if (!os_io_listen(on_request)) {
                printf("login server request can't be handled.\n");
                return 1;
        }

        printf("login server exit.\n");

        return 0;
}
