#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include "socket.h"
#include "work_queue.h"
#include "server.h"

static int handle_connection(int fd, void *data)
{
        int client_socket = 0;

        assert(data);

        printf("Accepting new connection.\n");
        client_socket = socket_accept(fd);

        if (client_socket == -1) {
                printf("Failed to accept new connection.\n");
                return -1;
        }

        work_queue_new_connection(data, client_socket);

        return client_socket;
}

static void
handle_request(int fd, void *data, unsigned char *request, ssize_t request_size)
{
        assert(data);
        assert(request);
        assert(request_size);

        printf("Handling new request from %d of size %ld.\n", fd, request_size);
        work_queue_client_request(data, fd, request, request_size);
}

static void handle_disconnect(int fd, void *data)
{
        assert(data);

        printf("Client %d disconnected.\n", fd);
        work_queue_disconnected(data, fd);
}

void *server_start(void *data)
{
        socket_conn_t conn = { 0 };

        unsigned short port = 0;

        size_t max_connections = 0;

        int handle_requests_result = 0;

        /**
         * TODO: Make these configurable
         */
        port            = 7777;
        max_connections = 10;

        conn.fd            = socket_create(port, max_connections);
        conn.data          = data;
        conn.on_connect    = &handle_connection;
        conn.on_request    = &handle_request;
        conn.on_disconnect = &handle_disconnect;

        if (conn.fd == -1) {
                printf("Unable to create socket.\n");
                return data;
        }

        printf("Waiting for requests...\n");
        handle_requests_result = socket_handle_requests(&conn);

        if (handle_requests_result == -1) {
                printf("Unable to handle requests.\n");
                return data;
        }

        return data;
}
