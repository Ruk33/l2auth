#include <stdio.h>
#include <stddef.h>
#include <dlfcn.h>
#include "../../include/os_socket.h"
#include "../../include/gs_lib.h"

#define GAME_SERVER_LIB_PATH "./game_server_lib.so"

static void *handle = 0;
static void (*on_load)(gs_lib_t *);
static void (*on_unload)(void);
static void (*on_new_conn)(os_socket_t *);
static void (*on_new_req)(os_socket_t *, unsigned char *, size_t);
static void (*on_disconnect)(os_socket_t *);

static gs_lib_t gs_lib = { 0 };

// Load function from game server library.
// On success, the function's handler is returned. On error 0.
static void *load_lib_function(char *name)
{
        void *function = 0;

        if (!handle) {
                return 0;
        }

        function = dlsym(handle, name);

        if (!function) {
                printf("Failed to load function %s.\n", name);
                printf("%s.\n", dlerror());
                return 0;
        }

        return function;
}

static void
internal_send_response(os_socket_t *socket, unsigned char *buf, size_t n)
{
        os_socket_send(socket, buf, n);
}

static int init_gs_lib(void)
{
        int all_load = 0;

        if (on_unload) {
                on_unload();
        }

        if (handle && dlclose(handle) != 0) {
                printf("Failed to unload game server library.\n");
                printf("%s.\n", dlerror());
                return 0;
        }

        handle = dlopen(GAME_SERVER_LIB_PATH, RTLD_LAZY);

        if (!handle) {
                printf("Failed to load game server library.\n");
                printf("%s.\n", dlerror());
                return 0;
        }

        *(void **) (&on_load)       = load_lib_function("gs_lib_load");
        *(void **) (&on_unload)     = load_lib_function("gs_lib_unload");
        *(void **) (&on_new_conn)   = load_lib_function("gs_lib_new_conn");
        *(void **) (&on_new_req)    = load_lib_function("gs_lib_new_req");
        *(void **) (&on_disconnect) = load_lib_function("gs_lib_disconnect");

        all_load =
                (handle && on_load && on_unload && on_new_conn && on_new_req &&
                 on_disconnect);

        if (!all_load) {
                return 0;
        }

        on_load(&gs_lib);

        return 1;
}

static void internal_on_request(
        os_socket_t *socket,
        socket_ev_t ev,
        unsigned char *buf,
        size_t n)
{
        // Todo: only load if required.
        if (!init_gs_lib()) {
                printf("Unable to properly load gameserver library.\n");
                return;
        }

        switch (ev) {
        case CONN:
                printf("New connection.\n");
                on_new_conn(socket);
                break;
        case REQ:
                printf("New request.\n");
                on_new_req(socket, buf, n);
                break;
        case CLOSED:
                printf("Disconnect.\n");
                on_disconnect(socket);
                break;
        default:
                break;
        }

        fflush(stdout);
}

int main(/* int argc, char **argv */)
{
        os_socket_t *socket = 0;

        socket = os_socket_create(7777);

        gs_lib.send_response = internal_send_response;

        if (!socket) {
                printf("Game server socket couldn't be created.\n");
                return 1;
        }

        if (!init_gs_lib()) {
                printf("Unable to load game server library.\n");
                return 1;
        }

        if (!os_socket_handle_requests(socket, internal_on_request)) {
                printf("Game server request can't be handled.\n");
                return 1;
        }

        printf("Game server listening for requests.\n");

        return 0;
}
