#include <assert.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <log/log.h>
#include "code.h"
#include "request_manager.h"
#include "connection_manager.h"

struct Connection {
        void *server_data;
        int id;
        pthread_t thread;
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

static void *connection_manager_handle_request
(void *conn_p)
{
        assert(conn_p);

        struct Connection *conn = NULL;
        int conn_id = 0;
        os_socket_handler *client_socket = NULL;
        size_t buf_size = 0;
        size_t request_size = 0;
        unsigned char *buf = NULL;

        conn = (struct Connection *) conn_p;
        conn_id = conn->id;
        client_socket = conn->socket;

        buf_size = 65535;
        buf = code_malloc(buf_size);

        while (1) {
                memset(buf, 0, buf_size);
                request_size = os_socket_receive(client_socket, buf, buf_size);

                if (request_size <= 0) break;

                request_manager_enqueue(
                        conn->server_data,
                        conn_id,
                        buf,
                        request_size
                );
        }

        code_handle_disconnect(conn->server_data, conn_id);
        code_mfree(buf);

        connection_manager->connections[conn_id] = NULL;

        return NULL;
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

        pthread_create(
                &conn->thread,
                NULL,
                &connection_manager_handle_request,
                conn
        );
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
