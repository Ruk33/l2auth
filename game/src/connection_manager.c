#include <assert.h>
#include <stdlib.h>
#include <log/log.h>
#include "code.h"
#include "request_queue.h"
#include "connection_manager.h"

struct Connection {
        void *server_data;
        os_socket_handler *socket;
};

struct ConnectionManager {
        struct Connection **connections;
        int connection_count;
};

static struct ConnectionManager *connection_manager = NULL;

void connection_manager_init
(size_t max_players)
{
        connection_manager = code_malloc(sizeof(*connection_manager));
        connection_manager->connections = code_malloc(
                sizeof(*connection_manager->connections) * max_players
        );
        connection_manager->connection_count = 0;
}

static void connection_manager_handle_request
(os_socket_handler *client_socket, int conn_id)
{
        assert(connection_manager);
        assert(client_socket);

        struct Connection *conn = NULL;
        size_t buf_size = 0;
        size_t request_size = 0;
        unsigned char *buf = NULL;

        conn = connection_manager->connections[conn_id];

        if (!conn) return;

        buf_size = 65535;
        buf = code_malloc(buf_size);
        request_size = os_socket_receive(client_socket, buf, buf_size);
        request_queue_enqueue(conn->server_data, conn_id, buf, request_size);

        code_mfree(buf);

        connection_manager_handle_request(client_socket, conn_id);
}

void connection_manager_new_conn
(void *server_data, os_socket_handler *conn_socket)
{
        assert(connection_manager);
        assert(conn_socket);

        struct Connection *conn = NULL;
        int conn_id = 0;

        conn = code_malloc(sizeof(*conn));
        conn->server_data = server_data;
        conn->socket = conn_socket;

        conn_id = connection_manager->connection_count;
        connection_manager->connections[conn_id] = conn;
        connection_manager->connection_count++;

        code_new_conn(server_data, conn_id);
        connection_manager_handle_request(conn_socket, conn_id);
}

void connection_manager_send_response
(int conn_id, unsigned char *buf, size_t buf_size)
{
        assert(buf);
        assert(buf_size > 0);

        struct Connection *conn = NULL;

        conn = connection_manager->connections[conn_id];

        if (!conn) {
                log_fatal("Could not find connection with id %d", conn_id);
                return;
        }

        os_socket_send(conn->socket, buf, buf_size);
}
