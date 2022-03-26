// Required for CLOCK_MONOTONIC to be defined.
#ifndef _DEFAULT_SOURCE
#define _DEFAULT_SOURCE
#endif

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <dlfcn.h>
#include <unistd.h>
#include <time.h>
#include <fcntl.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <sys/timerfd.h>
#include <netinet/in.h>

#include "../../util.c"

#include "../../include/config.h"
#include "../../include/platform.h"
#include "../../include/gs_types.h"

#define GAME_SERVER_LIB_PATH "./game_server_lib.so"

struct lib {
    void *handle;
    time_t load_time;
    void (*on_load)(struct gs_state *);
    void (*on_unload)(void);
    void (*on_new_conn)(struct platform_socket *);
    void (*on_new_req)(struct platform_socket *, void *, size_t);
    void (*on_disconnect)(struct platform_socket *);
    void (*on_tick)(seconds_t);
};

struct platform_socket {
    int fd;
    int is_server;
    int initialized;
};

struct platform_timer {
    int fd;
    struct itimerspec utmr;
};

enum platform_fd_request_type
{
    PLATFORM_SOCKET_NEW_CONNECTION,
    PLATFORM_SOCKET_NEW_REQUEST,
    PLATFORM_SOCKET_FAILED_TO_READ,
    PLATFORM_SOCKET_DISCONNECTED,
    PLATFORM_TIMER_TICK,
};

static struct lib lib = { 0 };

static struct gs_state *game_server = 0;

static int g_epoll_fd = 0;

static struct platform_socket g_sockets[MAX_CLIENTS] = { 0 };

static struct platform_timer g_timer = { 0 };

static void on_request(struct platform_socket *socket,
                       enum platform_fd_request_type type,
                       byte_t *buf,
                       size_t n);

static void on_tick(void);

// Returns 1 on success.
static int fd_to_poll(int fd, void *ptr, int events)
{
    struct epoll_event event = { 0 };

    event.events   = events; // EPOLLIN;
    event.data.ptr = ptr;

    return epoll_ctl(g_epoll_fd, EPOLL_CTL_ADD, fd, &event) != 0;
}

static struct platform_socket *get_free_socket(void)
{
    for (size_t i = 0; i < MAX_CLIENTS; i += 1) {
        if (!g_sockets[i].initialized) {
            return &g_sockets[i];
        }
    }
    return 0;
}

static void socket_non_block(int fd)
{
    int flags = 0;

    flags = fcntl(fd, F_GETFL, 0);
    flags = flags < 0 ? 0 : flags;

    fcntl(fd, F_SETFL, flags | O_NONBLOCK);
}

int socket_accept(struct platform_socket *dest, struct platform_socket *src)
{
    struct sockaddr_in address = { 0 };
    struct sockaddr *address_p = 0;

    socklen_t addrlen = 0;

    assert(dest);
    assert(src);
    assert(src->initialized);
    assert(src->is_server);

    address_p = (struct sockaddr *) &address;

    dest->fd          = accept(src->fd, address_p, &addrlen);
    dest->initialized = 1;
    dest->is_server   = 0;

    if (dest->fd < 0) {
        *dest = (struct platform_socket){ 0 };
        return 0;
    }

    socket_non_block(dest->fd);
    fd_to_poll(dest->fd, dest, EPOLLIN | EPOLLET);

    return 1;
}

static int block_and_listen_for_requests(void)
{
    // (franco.montenegro) I don't know what would be a
    // good size for this one.
    static struct epoll_event events[32] = { 0 };
    // (franco.montenegro) Should we put this memory
    // in the heap?
    static byte_t buf[macro_util_mb(5)] = { 0 };

    struct sockaddr_in server_address    = { 0 };
    struct sockaddr *server_address_p    = 0;
    struct platform_socket server_socket = { 0 };

    struct platform_socket *platform_socket = 0;

    int reusable_enable = 0;

    int ev_count = 0;

    ssize_t read = 0;

    server_socket.fd          = socket(AF_INET, SOCK_STREAM, 0);
    server_socket.initialized = 1;
    server_socket.is_server   = 1;

    if (server_socket.fd < 0) {
        printf("failed to create server socket.\n");
        return 0;
    }

    // Make the socket reusable.
    reusable_enable = 1;

    if (setsockopt(server_socket.fd,
                   SOL_SOCKET,
                   SO_REUSEADDR,
                   &reusable_enable,
                   sizeof(int)) < 0) {
        close(server_socket.fd);
        printf("failed to set server socket as reusable.\n");
        return 0;
    }

    server_address.sin_family      = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port        = htons(7777);

    server_address_p = (struct sockaddr *) &server_address;

    if (bind(server_socket.fd, server_address_p, sizeof(server_address)) < 0) {
        close(server_socket.fd);
        printf("failed to bind server socket.\n");
        return 0;
    }

    if (listen(server_socket.fd, MAX_CLIENTS) < 0) {
        close(server_socket.fd);
        printf("failed to listen in server socket.\n");
        return 0;
    }

    socket_non_block(server_socket.fd);
    fd_to_poll(server_socket.fd, &server_socket, EPOLLIN);

    while (1) {
        ev_count =
            epoll_wait(g_epoll_fd, events, macro_util_arr_len(events), -1);

        if (ev_count < 0) {
            printf("failed to get epoll events.\n");
            break;
        }

        for (int i = 0; i < ev_count; i += 1) {
            // We only care for EPOLLIN (read) events.
            // Skip all the rest.
            if ((events[i].events & EPOLLIN) != EPOLLIN) {
                continue;
            }

            platform_socket = events[i].data.ptr;

            // If there is no socket in the user data,
            // then we are dealing with the timer.
            if (!platform_socket) {
                // Restart the timer.
                timerfd_settime(g_timer.fd, 0, &g_timer.utmr, 0);
                on_request(0, PLATFORM_TIMER_TICK, 0, 0);
                continue;
            }

            if (platform_socket->is_server) {
                on_request(platform_socket,
                           PLATFORM_SOCKET_NEW_CONNECTION,
                           0,
                           0);
                continue;
            }

            read = recv(platform_socket->fd, buf, sizeof(buf), 0);

            if (read < 0) {
                on_request(platform_socket,
                           PLATFORM_SOCKET_FAILED_TO_READ,
                           0,
                           0);
                continue;
            }

            if (read == 0) {
                on_request(platform_socket, PLATFORM_SOCKET_DISCONNECTED, 0, 0);
                epoll_ctl(g_epoll_fd, EPOLL_CTL_DEL, platform_socket->fd, 0);
                *platform_socket = (struct platform_socket){ 0 };
                continue;
            }

            on_request(platform_socket,
                       PLATFORM_SOCKET_NEW_REQUEST,
                       buf,
                       (size_t) read);
        }
    }

    close(server_socket.fd);

    return 1;
}

static void *load_lib_function(char *name)
{
    void *function = 0;

    assert(name);

    if (!lib.handle) {
        return 0;
    }

    function = dlsym(lib.handle, name);

    if (!function) {
        printf("failed to load function %s.\n", name);
        printf("%s.\n", dlerror());
    }

    return function;
}

static int init_gs_lib(void)
{
    struct stat lib_stat = { 0 };

    int all_load = 0;

    stat(GAME_SERVER_LIB_PATH, &lib_stat);

    // Don't load if there are no changes in the lib.
    if (lib.handle && lib_stat.st_mtime == lib.load_time) {
        return 1;
    }

    if (lib.handle) {
        lib.on_unload();

        if (dlclose(lib.handle) != 0) {
            printf("failed to unload game server library.\n");
            printf("%s.\n", dlerror());
        }
    }

    lib = (struct lib){ 0 };

    lib.handle = dlopen(GAME_SERVER_LIB_PATH, RTLD_LAZY);

    if (!lib.handle) {
        printf("failed to load game server library.\n");
        printf("%s.\n", dlerror());
        return 0;
    }

    *(void **) (&lib.on_load)       = load_lib_function("gs_lib_load");
    *(void **) (&lib.on_unload)     = load_lib_function("gs_lib_unload");
    *(void **) (&lib.on_new_conn)   = load_lib_function("gs_lib_new_conn");
    *(void **) (&lib.on_new_req)    = load_lib_function("gs_lib_new_req");
    *(void **) (&lib.on_disconnect) = load_lib_function("gs_lib_disconnect");
    *(void **) (&lib.on_tick)       = load_lib_function("gs_lib_tick");

    all_load = (lib.handle && lib.on_load && lib.on_unload && lib.on_new_conn &&
                lib.on_new_req && lib.on_disconnect && lib.on_tick);

    if (!all_load) {
        lib = (struct lib){ 0 };
        return 0;
    }

    lib.load_time = lib_stat.st_mtime;
    lib.on_load(game_server);

    return 1;
}

static void on_request(struct platform_socket *socket,
                       enum platform_fd_request_type type,
                       byte_t *buf,
                       size_t n)
{
    struct platform_socket *client_socket = 0;

    if (!init_gs_lib()) {
        printf("unable to load game server library.\n");
        printf("ignoring request.\n");
        return;
    }

    switch (type) {
    case PLATFORM_SOCKET_NEW_CONNECTION:
        client_socket = get_free_socket();

        if (!socket_accept(client_socket, socket)) {
            printf("login server unable to accept new connection.\n");
            return;
        }

        lib.on_new_conn(client_socket);
        break;
    case PLATFORM_SOCKET_NEW_REQUEST:
        lib.on_new_req(socket, buf, n);
        break;
    case PLATFORM_SOCKET_FAILED_TO_READ:
        printf("game server failed to read packet.\n");
        break;
    case PLATFORM_SOCKET_DISCONNECTED:
        printf("game server client disconnected.\n");
        break;
    case PLATFORM_TIMER_TICK:
        on_tick();
        break;
    default:
        break;
    }
}

static void send_response(struct platform_socket *socket, void *buf, size_t n)
{
    ssize_t sent = 0;

    if (!n) {
        return;
    }

    if (!socket || !buf) {
        printf("internal send response, no io? ignoring.\n");
        return;
    }

    sent = send(socket->fd, buf, n, 0);

    if (sent < (ssize_t) n) {
        printf(
            "WARNING - send_response wanted to send %ld but %ld were sent.\n",
            n,
            sent);
    }
}

static void disconnect(struct platform_socket *socket)
{
    if (!socket) {
        return;
    }

    lib.on_disconnect(socket);

    shutdown(socket->fd, SHUT_RDWR);
    close(socket->fd);

    *socket = (struct platform_socket){ 0 };
}

static void on_tick(void)
{
    static long last_tick_ms = 0;

    struct timespec now = { 0 };
    long now_ms   = 0;
    long delta_ms = 0;

    if (clock_gettime(CLOCK_REALTIME, &now) != 0) {
        printf("ERROR! FAILED TO CALCULATE GAME LOOP DELTA\n");
        return;
    }

    // If the timeout used is equal or longer than a second
    // make sure to use tv_sec, otherwise, use tv_nsec
    now_ms   = now.tv_sec * 1000;
    delta_ms = now_ms - last_tick_ms;
    last_tick_ms = now_ms;

    lib.on_tick((float) delta_ms / 1000);
}

static int init_timer(void)
{
    u32_t ms_timeout = 0;

    ms_timeout = 1000;

    g_timer.fd = timerfd_create(CLOCK_MONOTONIC, TFD_NONBLOCK);

    if (g_timer.fd < 0) {
        printf("failed to create timer fd.\n");
        return 0;
    }

    // Not really needed but left for documentation.
    // Also it may come in handy if we later decide
    // to change the timeout.
    if (ms_timeout >= 1000) {
        g_timer.utmr.it_value.tv_sec    = (long) (ms_timeout / 1000);
        g_timer.utmr.it_interval.tv_sec = g_timer.utmr.it_value.tv_sec;
    } else {
        g_timer.utmr.it_value.tv_nsec    = (long) (ms_timeout * 1000000);
        g_timer.utmr.it_interval.tv_nsec = g_timer.utmr.it_value.tv_nsec;
    }

    if (timerfd_settime(g_timer.fd, 0, &g_timer.utmr, 0) < 0) {
        printf("failed to set time\n");
        return 0;
    }

    fd_to_poll(g_timer.fd, 0, EPOLLIN);

    return 1;
}

static i32_t random_i32(i32_t min, i32_t max)
{
    return rand() % (max + 1 - min) + min;
}

int main(/* int argc, char **argv */)
{
    g_epoll_fd = epoll_create1(0);

    if (g_epoll_fd < 0) {
        printf("failed to create epoll.\n");
        return 1;
    }

    if (!init_timer()) {
        printf("unable to initialize game server timer.\n");
        return 1;
    }

    game_server = calloc(1, sizeof(*game_server));

    if (!game_server) {
        printf("failed to allocate memory for game server state.\n");
        return 1;
    }

    srand(time(0));

    game_server->send_response = send_response;
    game_server->disconnect    = disconnect;
    game_server->random_i32    = random_i32;

    printf("game server started.\n");

    if (!block_and_listen_for_requests()) {
        printf("game server socket unable to listen for new connections.\n");
        return 1;
    }

    close(g_epoll_fd);

    return 0;
}
