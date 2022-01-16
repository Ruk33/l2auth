#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <dlfcn.h>
#include <time.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "../platform.c"
#include "../../util.c"

#include "../../include/config.h"
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
    void (*on_tick)(double);
};

static struct lib lib = { 0 };

static struct gs_state *game_server = 0;
static struct timeval last_tick = { 0 };

static struct platform_socket *g_sockets[MAX_CLIENTS] = { 0 };
static size_t g_socket_instances[MAX_CLIENTS]         = { 0 };

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

static void send_response(struct platform_socket *socket, void *buf, size_t n)
{
    ssize_t sent = 0;

    if (!socket) {
        printf("internal send response, no io? ignoring.\n");
        return;
    }

    if (!platform_socket_send(socket, &sent, buf, n)) {
        printf("internal send response, unable to send response.\n");
        return;
    }
}

static void disconnect(struct platform_socket *socket)
{
    if (!socket) {
        return;
    }
    lib.on_disconnect(socket);
    platform_socket_close(socket);
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
                       enum platform_socket_request_type type,
                       void *buf,
                       size_t n)
{
    size_t free_socket = 0;

    assert(game_server);
    assert(game_server->send_response);
    assert(game_server->disconnect);

    if (!socket) {
        printf("no socket? ignoring request.\n");
        return;
    }

    if (!init_gs_lib()) {
        printf("unable to load game server library.\n");
        printf("ignoring request.\n");
        return;
    }

    switch (type) {
    case PLATFORM_SOCKET_NEW_CONNECTION:
        util_recycle_id_get(&free_socket, g_socket_instances);
        g_sockets[free_socket] = platform_socket_new();

        if (!g_sockets[free_socket]) {
            util_recycle_id(g_socket_instances, free_socket);
            printf(
                "game server unable to get new socket for accepting client.\n");
            return;
        }

        if (!platform_socket_accept(g_sockets[free_socket], socket)) {
            util_recycle_id(g_socket_instances, free_socket);
            printf("login server unable to accept new connection.\n");
            return;
        }

        lib.on_new_conn(g_sockets[free_socket]);
        break;
    case PLATFORM_SOCKET_NEW_REQUEST:
        lib.on_new_req(socket, buf, n);
        break;
    case PLATFORM_SOCKET_FAILED_TO_READ:
        printf("game server failed to read packet.\n");
        break;
    case PLATFORM_SOCKET_READY_TO_WRITE:
        printf("game server ready to write.\n");
        break;
    case PLATFORM_SOCKET_DISCONNECTED:
        printf("game server client disconnected.\n");
        for (size_t i = 0; i < macro_util_arr_len(g_sockets); i += 1) {
            if (g_sockets[i] == socket) {
                util_recycle_id(g_socket_instances, i);
                break;
            }
        }
        break;
    default:
        break;
    }
}

static void on_tick(struct platform_timer *src)
{
    struct timeval now = { 0 };
    suseconds_t delta = 0;

    assert(src);

    if (!init_gs_lib()) {
        printf("unable to load game server library.\n");
        printf("ignoring tick.\n");
        return;
    }

    gettimeofday(&now, 0);
    delta = now.tv_usec - last_tick.tv_usec;
    last_tick = now;

    printf("%f\n", (float) delta / 100);

    // delta in milliseconds
    lib.on_tick((float) delta / 100);
}

static void timer_thread(struct platform_thread *thread)
{
    struct platform_timer *timer = 0;

    assert(thread);

    timer = platform_timer_new();

    if (!timer) {
        printf("game server unable to create timer.\n");
        platform_thread_kill(thread);
        return;
    }

    if (!platform_timer_init(timer, 1000, 1)) {
        printf("game server unable to initialize timer.\n");
        platform_thread_kill(thread);
        return;
    }

    if (!platform_timer_start(timer, 1, on_tick)) {
        printf("game server unable to start timer.\n");
        platform_thread_kill(thread);
        return;
    }
}

static i32_t random_i32(i32_t min, i32_t max)
{
    return rand() % (max + 1 - min) + min;
}

int main(/* int argc, char **argv */)
{
    struct platform_thread *thread = 0;

    size_t free_socket = 0;
    size_t sockets_len = 0;

    game_server = calloc(1, sizeof(*game_server));

    gettimeofday(&last_tick, 0);
    srand(time(0));

    game_server->send_response = send_response;
    game_server->disconnect    = disconnect;
    game_server->random_i32    = random_i32;

    thread = platform_thread_new();

    if (!thread) {
        printf("game server unable to create thread.\n");
        return 1;
    }

    if (!platform_thread_create(thread, timer_thread)) {
        printf("game server unable to initialize thread.\n");
        return 1;
    }

    util_recycle_id_get(&free_socket, g_socket_instances);
    g_sockets[free_socket] = platform_socket_new();

    if (!g_sockets[free_socket]) {
        printf("game server socket couldn't be created.\n");
        return 1;
    }

    if (!platform_socket_init(g_sockets[free_socket], 7777, MAX_CLIENTS)) {
        printf("game server socket unable to initialize.\n");
        return 1;
    }

    printf("game server started.\n");

    sockets_len = macro_util_arr_len(g_sockets);

    if (!platform_socket_listen(*g_sockets, sockets_len, on_request)) {
        printf("game server socket unable to listen for new connections.\n");
        return 1;
    }

    return 0;
}
