#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <dlfcn.h>
#include "../../include/util.h"
#include "../../include/socket.h"

#define GAME_SERVER_LIB_PATH "./game_server_lib.so"

typedef void (*send_response_cb)(socket_t *, byte_t *, size_t);
typedef void (*save_sessions_cb)(void *, size_t);

typedef void (*load_cb)(send_response_cb, save_sessions_cb);
typedef void (*unload_cb)(void);
typedef void (*load_sessions_cb)(void *, size_t);
typedef void (*new_conn_cb)(socket_t *);
typedef void (*new_req_cb)(socket_t *, byte_t *, size_t);
typedef void (*disconnect_cb)(socket_t *);

typedef struct {
        void *sessions;
        size_t session_size;

        void *lib;
        load_cb load;
        unload_cb unload;

        load_sessions_cb load_sessions;
        new_conn_cb new_conn;
        new_req_cb new_req;
        disconnect_cb disconnect;
} gs_lib_t;

static gs_lib_t gs_lib = { 0 };

// Load game server in memory.
// On success, 1 is returned. On error 0.
static int load_library(void)
{
        if (gs_lib.lib) {
                dlclose(gs_lib.lib);
        }

        gs_lib.lib = dlopen(GAME_SERVER_LIB_PATH, RTLD_LAZY);

        if (!gs_lib.lib) {
                printf("Failed to load game server library.\n");
                printf("%s.\n", dlerror());
                return 0;
        }

        return 1;
}

// Load function from game server library.
// On success, the function's handler is returned. On error 0.
static void *load_lib_function(char *name)
{
        void *function = 0;

        if (!gs_lib.lib) {
                return 0;
        }

        function = dlsym(gs_lib.lib, name);

        if (!function) {
                printf("Failed to load function %s.\n", name);
                printf("%s.\n", dlerror());
                return 0;
        }

        return function;
}

static void send_response(socket_t *socket, byte_t *buf, size_t n)
{
        socket_send(socket, buf, n);
}

static void save_sessions(void *sessions, size_t n)
{
        if (!gs_lib.sessions) {
                gs_lib.sessions     = malloc(n);
                gs_lib.session_size = n;
        }

        memcpy(gs_lib.sessions, sessions, n);
}

static int init_gs_lib(void)
{
        int all_load = 0;

        if (gs_lib.unload) {
                gs_lib.unload();
        }

        load_library();

        *(void **) (&gs_lib.load) = load_lib_function("game_server_lib_load");
        *(void **) (&gs_lib.unload) =
                load_lib_function("game_server_lib_unload");
        *(void **) (&gs_lib.load_sessions) =
                load_lib_function("game_server_lib_load_sessions");
        *(void **) (&gs_lib.new_conn) =
                load_lib_function("game_server_lib_new_conn");
        *(void **) (&gs_lib.new_req) =
                load_lib_function("game_server_lib_new_req");
        *(void **) (&gs_lib.disconnect) =
                load_lib_function("game_server_lib_disconnect");

        all_load =
                (gs_lib.lib && gs_lib.load && gs_lib.unload &&
                 gs_lib.load_sessions && gs_lib.new_conn && gs_lib.new_req &&
                 gs_lib.disconnect);

        if (!all_load) {
                return 0;
        }

        gs_lib.load(send_response, save_sessions);
        gs_lib.load_sessions(gs_lib.sessions, gs_lib.session_size);

        return 1;
}

static void on_request(socket_t *socket, socket_ev_t ev, byte_t *buf, size_t n)
{
        // Todo: only load if required.
        if (!init_gs_lib()) {
                printf("Unable to properly load gameserver library.\n");
                return;
        }

        switch (ev) {
        case CONN:
                printf("New connection.\n");
                gs_lib.new_conn(socket);
                break;
        case REQ:
                printf("New request.\n");
                gs_lib.new_req(socket, buf, n);
                break;
        case CLOSED:
                printf("Disconnect.\n");
                gs_lib.disconnect(socket);
                break;
        default:
                break;
        }

        fflush(stdout);
}

int main(/* int argc, char **argv */)
{
        socket_t *socket = 0;

        socket = socket_create(7777);

        if (!socket) {
                printf("Game server socket couldn't be created.\n");
                return 1;
        }

        if (!init_gs_lib()) {
                printf("Unable to load game server library.\n");
                return 1;
        }

        if (!socket_handle_requests(socket, on_request)) {
                printf("Game server request can't be handled.\n");
                return 1;
        }

        printf("Game server listening for requests.\n");

        return 0;
}
