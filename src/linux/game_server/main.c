#include <stdio.h>
#include <dlfcn.h>
#include "../../include/util.h"
#include "../../include/socket.h"

#define GAME_SERVER_LIB_PATH "./game_server_lib.so"

typedef struct {
        void *lib;
        void (*initialize)(void (*)(socket_t *, byte_t *, size_t));
        void (*new_conn)(socket_t *);
        void (*new_req)(socket_t *, byte_t *, size_t);
        void (*disconnect)(socket_t *);
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

static int init_gs_lib(void)
{
        load_library();
        *(void **) (&gs_lib.initialize) =
                load_lib_function("game_server_lib_load");
        *(void **) (&gs_lib.new_conn) =
                load_lib_function("game_server_lib_new_conn");
        *(void **) (&gs_lib.new_req) =
                load_lib_function("game_server_lib_new_req");
        *(void **) (&gs_lib.disconnect) =
                load_lib_function("game_server_lib_disconnect");

        return gs_lib.lib && gs_lib.initialize && gs_lib.new_conn &&
               gs_lib.new_req && gs_lib.disconnect;
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

        gs_lib.initialize(send_response);

        if (!socket_handle_requests(socket, on_request)) {
                printf("Game server request can't be handled.\n");
                return 1;
        }

        printf("Game server listening for requests.\n");

        return 0;
}
