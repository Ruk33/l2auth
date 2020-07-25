#include <stdlib.h>
#include <log/log.h>
#include <os/socket.h>
#include "../shared/l2_server.h"
#include "../shared/client_id.h"
#include "../shared/request.h"
#include "../shared/response.h"
#include "mem.h"
#include "code.h"
#include "server.h"

struct Server {
        os_socket_handler *socket;
        struct Code code;
        struct L2Server l2_server;
};

struct Client {
        client_id id;
        void *data;
        os_socket_handler *socket;
};

struct Server *server;
struct Client **clients;
client_id last_client_id = 0;

static void queue_response(struct Response *response)
{
        struct Client *client = clients[response->client_id];
        log_info("Queue response");
        log_info("Sending response");
        os_socket_send(client->socket, (unsigned char *) response->buf, response->buf_size);
}

static void handle_requests(struct Client *client)
{
        os_socket_handler *client_socket = client->socket;
        size_t max_request_size = 65535;
        struct Request request;

        request.client = client->data;
        request.buf = mem_alloc(max_request_size);
        request.buf_size = os_socket_receive(client_socket, (unsigned char *) request.buf, max_request_size);

        code_load(&server->code);

        log_info("Handling request for client %d", client->id);
        log_info("Request size: %d", request.buf_size);

        if (request.buf_size > 0) server->code.handle_request(&request);
        else server->code.handle_disconnect(&request);

        mem_free(request.buf);
}

static void accept_connections()
{
        os_socket_handler *client_socket = mem_alloc(os_socket_handler_size());
        struct Client *client;

        os_socket_accept(server->socket, client_socket);
        log_info("Accepted connection");

        last_client_id++;

        client = mem_alloc(sizeof(struct Client));
        client->id = last_client_id;
        client->data = server->code.new_conn(&server->l2_server, client->id);
        client->socket = client_socket;

        clients[client->id] = client;

        log_info("New client id: %d", client->id);

        while (1) handle_requests(client);

        mem_free(client_socket);
        mem_free(client);
        os_socket_close(client_socket);
}

void server_start(unsigned short port, size_t max_players)
{
        server = mem_alloc(sizeof(struct Server));
        clients = mem_alloc(sizeof(struct Client *) * max_players);

        server->socket = mem_alloc(os_socket_handler_size());

        server->l2_server.alloc = mem_alloc;
        server->l2_server.free = mem_free;
        server->l2_server.queue_response = queue_response;

        code_load(&server->code);

        log_info("Listening for connections");

        os_socket_connect(server->socket);
        os_socket_bind(server->socket, port);
        os_socket_listen(server->socket, max_players);

        while (1) accept_connections();

        os_socket_close(server->socket);

        mem_free(clients);
        mem_free(server->socket);
        mem_free(server);
}
