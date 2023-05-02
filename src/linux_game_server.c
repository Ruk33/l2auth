#include <stdlib.h>     // exit
#include <unistd.h>     // close
#include <dlfcn.h>      // dlopen, dlclose, dlerror, dlsym
#include <time.h>       // time_t
#include <signal.h>     // signal
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>      // fprintf, stderr, 
#include "include/game_server.h"

struct connection {
    int socket;
    struct game_session *session;
    size_t written;
    size_t sending_index;
};

struct game_server_lib {
    void *handle;
    struct game_session *(*game_server_new_conn)(struct game_state *state);
    void (*game_server_request)(struct game_state *state, struct game_session *session, void *buf, size_t n);
    void (*game_server_disconnect)(struct game_state *state, struct game_session *session);
    time_t last_time_loaded;
};

static struct game_state state = {0};
static struct connection connections[MAX_CONNECTIONS] = {0};
static struct game_server_lib lib = {0};

void _putchar(char character)
{
    state.output[state.output_size] = character;
    state.output_size++;
}

static struct game_session *empty_game_server_new_conn(struct game_state *state)
{
    assert(state);
    return 0;
}

static void emtpy_game_server_request(struct game_state *state, struct game_session *session, void *buf, size_t n)
{
    assert(state);
    // avoid getting unused warnings.
    session = session;
    buf = buf;
    n = n;
}

static void empty_game_server_disconnect(struct game_state *state, struct game_session *session)
{
    assert(state);
    assert(session);
}

time_t game_server_lib_change_time(void)
{
    struct stat file_stat = {0};
    int err = stat("./game_server_lib", &file_stat);
    if (err != 0) {
        log("unable to check if the game server library changed.");
        return 0;
    }
    return file_stat.st_mtime;
}

static void load_game_server_lib(void)
{
    int library_change_time = game_server_lib_change_time();
    if (library_change_time <= lib.last_time_loaded)
        return;
    // close if the library is already open.
    if (lib.handle)
        dlclose(lib.handle);
    // load library
    lib.handle = dlopen("./game_server_lib", RTLD_LAZY);
    lib.game_server_new_conn = empty_game_server_new_conn;
    lib.game_server_disconnect = empty_game_server_disconnect;
    lib.game_server_request = emtpy_game_server_request;
    if (!lib.handle) {
        log("unable to load game server library: %s.", dlerror());
        return;
    }
    lib.game_server_new_conn = dlsym(lib.handle, "game_server_new_conn");
    if (!lib.game_server_new_conn) {
        log("unable to load game server new connection function: %s.", dlerror());
        lib.game_server_new_conn = empty_game_server_new_conn;
    }
    lib.game_server_request = dlsym(lib.handle, "game_server_request");
    if (!lib.game_server_request) {
        log("unable to load game server request function: %s.", dlerror());
        lib.game_server_request = emtpy_game_server_request;
    }
    lib.game_server_disconnect = dlsym(lib.handle, "game_server_disconnect");
    if (!lib.game_server_disconnect) {
        log("unable to load game server disconnect function: %s.", dlerror());
        lib.game_server_disconnect = empty_game_server_disconnect;
    }
    log("game server library loaded.");
    lib.last_time_loaded = library_change_time;
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

static void flush_logs(void)
{
    // ignore if there is nothing to print.
    if (!state.output_size)
        return;
    time_t t = time(0);
    struct tm *tm = localtime(&t);
    // example, Wed Feb  1 12:56:03 2023
    char date_time[64] = {0};
    strftime(date_time, sizeof(date_time), "%c", tm);
    char *line = state.output;
    for (char *c = state.output; c < state.output + state.output_size; c++) {
        switch (*c) {
        case '\n':
            // +1, print new line.
            fprintf(stderr, "%s - %.*s", date_time, (uint)(c - line + 1), line);
            // +1, skip new line.
            line = c + 1;
        break;
        default:
            break;
        }
    }
    state.output_size = 0;
}

static void flush_connections(void)
{
    for_each(struct connection, conn, connections) {
        int socket = conn->socket;
        if (!conn->session)
            continue;
        if (conn->session->closed) {
            zero(conn);
            close(socket);
            continue;
        }
        while (conn->session->responses_count > 0) {
            size_t i = conn->sending_index;
            u16 response_size = packet_size(&conn->session->responses[i]);
            log("response size: %d", response_size);
            conn->written += network_write(
                socket,
                conn->session->responses[i].buf + conn->written,
                response_size - conn->written
            );
            if (response_size > conn->written)
                return;
            zero(&conn->session->responses[i]);
            conn->written = 0;
            conn->sending_index++;
            conn->session->responses_count--;
            if (conn->sending_index == arr_len(conn->session->responses))
                conn->sending_index = 0;
        }
    }
}

static void socket_event_handler(int socket, enum network_event event, void *read, size_t len)
{
    struct connection *conn = 0;

    load_game_server_lib();
    flush_logs();

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
        conn->session = lib.game_server_new_conn(&state);
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
            lib.game_server_disconnect(&state, conn->session);
        zero(conn);
        break;
    case NETWORK_READ:
        log("new chunk of request received.");
        conn = get_connection_from(socket);
        if (!conn)
            return;
        if (!conn->session)
            return;
        lib.game_server_request(&state, conn->session, read, len);
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
        flush_connections();
        break;
    case NETWORK_TICK:
        lib.game_server_request(&state, 0, 0, 0);
        flush_connections();
        break;
    default:
        break;
    }
}

static void segfault_handler(int sig_num)
{
    flush_logs();
    exit(sig_num);
}

int main(void)
{
    // catch segfault.
    signal(SIGSEGV, segfault_handler);

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
