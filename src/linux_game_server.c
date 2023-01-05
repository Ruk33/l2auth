#include <unistd.h> // close
#include "include/game_server.h"

struct connection {
    int socket;
    struct game_session *session;
    size_t written;
    size_t responses_served;
};

static struct game_state state = {0};
static struct connection connections[MAX_CONNECTIONS] = {0};

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
        conn->session = game_server_new_conn(&state);
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
            game_server_disconnect(&state, conn->session);
        zero(conn);
        break;
    case NETWORK_READ:
        log("new chunk of request received.");
        conn = get_connection_from(socket);
        if (!conn)
            return;
        if (!conn->session)
            return;
        game_server_request(&state, conn->session, read, len);
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
        // todo: make sure we can re-use responses that we
        // have already sent. now, that's not the case, the entire
        // queue has to be flushed in order to reuse buckets.
        for (
            size_t i = conn->responses_served;
            i < conn->session->response_queue_count;
            i++
        ) {
            u16 response_size = packet_size(&conn->session->response_queue[i]);
            printf("response size: %d\n", response_size);
            conn->written += network_write(
                socket,
                conn->session->response_queue[i].buf + conn->written,
                response_size - conn->written
            );
            if (response_size > conn->written)
                return;
            zero(&conn->session->response_queue[i]);
            conn->written = 0;
            conn->responses_served++;
        }
        conn->responses_served = 0;
        conn->session->response_queue_count = 0;
        break;
    default:
        break;
    }
}

int main(void)
{
    u16 port = 7777;
    int server_fd = network_port(port);
    if (server_fd == -1) {
        log("unable to create game server socket.");
        return -1;
    }

    log("game server started. listening for connections.");
    network_listen(server_fd, socket_event_handler);

    return 0;
}