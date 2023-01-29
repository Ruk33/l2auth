#include <unistd.h> // close
#include <string.h> // memset, strncmp
#include <stdio.h>  // printf
#include "include/login_server.h"

struct connection {
    int socket;
    struct login_session *session;
    size_t written;
};

static struct state state = {0};
static struct connection connections[MAX_CONNECTIONS] = {0};

void _putchar(char character)
{
    putc(character, stderr);
}

struct connection *get_connection_from(int socket)
{
    struct connection *result = 0;
    for_each(struct connection, connection, connections) {
        if (connection->socket != socket)
            continue;
        result = connection;
        break;
    }
    return result;
}

static void socket_event_handler(int socket, enum network_event event, void *read, size_t len)
{
    struct connection *conn = 0;

    switch (event) {
    case NETWORK_NEW_CONN:
        log("new connection.");
        conn = get_connection_from(0);
        if (!conn) {
            log("unable to find free connection to use. the client will be dropped.");
            close(socket);
            return;
        }
        conn->socket = socket;
        conn->session = login_server_new_conn(&state);
        if (conn->session)
            return;
        log("session was not able to be created. the client will be dropped.");
        zero(conn);
        close(socket);
        break;
    case NETWORK_CLOSED:
        log("client closed the connection.");
        conn = get_connection_from(socket);
        if (!conn)
            return;
        if (conn->session)
            login_server_disconnect(&state, conn->session);
        zero(conn);
        break;
    case NETWORK_READ:
        log("new chunk of request received.");
        conn = get_connection_from(socket);
        if (!conn)
            return;
        if (!conn->session)
            return;
        login_server_request(&state, conn->session, read, len);
        if (!conn->session->closed)
            return;
        zero(conn);
        close(socket);
        break;
    case NETWORK_CAN_WRITE:
        conn = get_connection_from(socket);
        if (!conn)
            return;
        if (!conn->session)
            return;
        if (conn->session->closed) {
            zero(conn);
            close(socket);
            return;
        }
        // check if there is a response pending to be written.
        u16 response_size = packet_size(&conn->session->response);
        if (!response_size)
            return;
        log("writing chunk of response.");
        conn->written += network_write(
            socket,
            conn->session->response.buf + conn->written,
            response_size - conn->written
        );
        // reset if the entire response was sent.
        if (response_size <= conn->written) {
            log("entire response sent.");
            zero(&conn->session->response);
            conn->written = 0;
        }
        break;
    default:
        break;
    }
}

static void create_initial_game_servers(void)
{
    // create localhost server.
    struct ipv4 ip_localhost = {{"127.0.0.1"}};
    struct server localhost = {0};

    if (!network_ipv4_to_u32(&localhost.ip, &ip_localhost)) {
        log("unable to parse localhost ip.");
        return;
    }

    localhost.id = 1;
    localhost.age_limit = 18;
    localhost.brackets = 0;
    localhost.extra = 0;
    localhost.port = 7777;
    localhost.pvp = 1;
    localhost.status = 1;

    if (storage_create_server(&localhost))
        log("initial localhost server was successfully created.");

    // create test server.
    struct ipv4 ip_zero = {{"0.0.0.0"}};
    struct server test = {0};

    if (!network_ipv4_to_u32(&test.ip, &ip_zero)) {
        log("unable to parse 0.0.0.0 ip.");
        return;
    }

    test.id = 2;
    test.age_limit = 18;
    test.brackets = 1;
    test.extra = 0;
    test.port = 7778;
    test.pvp = 1;
    test.status = 1;

    if (storage_create_server(&test))
        log("initial test server was successfully created.");
}

int main(void)
{
    u16 port = 2106;
    int server_fd = network_port(port);
    if (server_fd == -1) {
        log("unable to create login server socket.");
        return -1;
    }

    log("login server started. listening for connections.");
    create_initial_game_servers();
    network_listen(server_fd, socket_event_handler);

    return 0;
}
