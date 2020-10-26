#include <assert.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <log/log.h>
#include "memory_manager.h"
#include "game_server_lib.h"
#include "request_manager.h"
#include "connection_manager.h"

struct Connection
{
        void *server_data;
        int id;
        pthread_t thread;
        os_socket_handler *socket;
};

struct ConnectionManager
{
        struct Connection **connections;
        int connection_count;
        int max_players;
};

static struct ConnectionManager *connection_manager = NULL;

void connection_manager_init(size_t max_players)
{
        struct Connection **connections = NULL;

        connections = memory_manager_alloc(sizeof(*connections) * max_players);

        connection_manager = memory_manager_alloc(sizeof(*connection_manager));
        connection_manager->connections = connections;
        connection_manager->connection_count = 0;
        connection_manager->max_players = max_players;
}

static void *connection_manager_handle_request(void *conn_p)
{
        assert(conn_p);

        struct Connection *conn = NULL;
        int conn_id = 0;
        os_socket_handler *client_socket = NULL;
        size_t buf_size = 0;
        size_t request_size = 0;
        unsigned char *buf = NULL;

        conn = (struct Connection *)conn_p;
        conn_id = conn->id;
        client_socket = conn->socket;

        buf_size = 65535;
        buf = memory_manager_alloc(buf_size);

        while (1)
        {
                memset(buf, 0, buf_size);
                request_size = os_socket_receive(client_socket, buf, buf_size);

                if (request_size <= 0)
                        break;

                request_manager_enqueue(
                    conn->server_data,
                    conn_id,
                    buf,
                    request_size);
        }

        game_server_lib_disconnect(conn->server_data, conn_id);
        memory_manager_free(buf);

        connection_manager->connections[conn_id] = NULL;

        return NULL;
}

void connection_manager_new_conn(void *server_data, os_socket_handler *conn_socket)
{
        assert(connection_manager);
        assert(conn_socket);

        struct Connection *conn = NULL;

        conn = memory_manager_alloc(sizeof(*conn));
        conn->server_data = server_data;
        conn->socket = conn_socket;
        conn->id = connection_manager->connection_count;

        connection_manager->connections[conn->id] = conn;
        connection_manager->connection_count++;

        game_server_lib_new_conn(server_data, conn->id);

        pthread_create(
            &conn->thread,
            NULL,
            &connection_manager_handle_request,
            conn);
}

void connection_manager_send_response(int conn_id, unsigned char *buf, size_t buf_size)
{
        assert(buf);
        assert(buf_size > 0);

        struct Connection *conn = NULL;

        conn = connection_manager->connections[conn_id];

        if (!conn)
        {
                log_fatal("Could not find connection with id %d", conn_id);
                return;
        }

        os_socket_send(conn->socket, buf, buf_size);
}

void connection_manager_finish(void)
{
        assert(connection_manager);

        for (int i = 0; i < connection_manager->max_players; i++)
        {
                memory_manager_free(connection_manager->connections[i]);
        }

        memory_manager_free(connection_manager->connections);
        memory_manager_free(connection_manager);

        connection_manager = NULL;
}
