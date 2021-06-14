#include <stdio.h>
#include <unistd.h>
#include <stddef.h>
#include <dlfcn.h>
#include <sys/wait.h>
#include "../../include/os_socket.h"
#include "../../include/gs_lib.h"

#define GAME_SERVER_LIB_PATH "./game_server_lib.so"

static void *handle = 0;
static void (*on_load)(gs_lib_t *);
static void (*on_unload)(void);
static void (*on_new_conn)(os_socket_t *);
static void (*on_new_req)(os_socket_t *, unsigned char *, size_t);
static void (*on_disconnect)(os_socket_t *);

static gs_lib_t *gs_lib = 0;

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
                printf("failed to load function %s.\n", name);
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
                printf("failed to unload game server library.\n");
                printf("%s.\n", dlerror());
                return 0;
        }

        handle = dlopen(GAME_SERVER_LIB_PATH, RTLD_LAZY);

        if (!handle) {
                printf("failed to load game server library.\n");
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

        on_load(gs_lib);

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
                printf("unable to properly load gameserver library.\n");
                return;
        }

        switch (ev) {
        case CONN:
                printf("new connection.\n");
                on_new_conn(socket);
                break;
        case REQ:
                printf("new request.\n");
                on_new_req(socket, buf, n);
                break;
        case CLOSED:
                printf("disconnect.\n");
                on_disconnect(socket);
                break;
        default:
                break;
        }

        fflush(stdout);
}

static int handle_requests(os_socket_t *socket, gs_lib_t *parent_gs_lib)
{
        printf("game server listening for requests.\n");

        // The gs lib resides in the parent
        // so the information doesn't get lost
        // in the case the child process dies and
        // gets restarted.
        gs_lib                = parent_gs_lib;
        gs_lib->send_response = internal_send_response;

        if (!init_gs_lib()) {
                printf("unable to load game server library.\n");
                return 0;
        }

        if (!os_socket_handle_requests(socket, internal_on_request)) {
                printf("game server request can't be handled.\n");
                return 0;
        }

        return 1;
}

// Requests will be handled by a clone/fork of the process
// to prevent possible crashes or errors to blow up the entire server.
static int spawn_process(os_socket_t *socket, gs_lib_t *parent_gs_lib)
{
        pid_t pid   = 0;
        int wstatus = 0;

        pid = fork();

        switch (pid) {
        case -1:
                printf("spawn process failed.\n");
                return 0;
        case 0: // child
                printf("spawned child process. pid: %d.\n", pid);
                return handle_requests(socket, parent_gs_lib);
        default: // parent
                if (waitpid(pid, &wstatus, 0) == -1) {
                        printf("failed to wait for child process.\n");
                        return 0;
                }
                if (WIFEXITED(wstatus)) {
                        printf("child stopped successfully.\n");
                        return 1;
                }

                printf("child died unexpectedly. trying to respawn.\n");
                return spawn_process(socket, parent_gs_lib);
        }
}

int main(/* int argc, char **argv */)
{
        static gs_lib_t parent_gs_lib = { 0 };

        os_socket_t *socket = 0;

        socket = os_socket_create(7777);

        if (!socket) {
                printf("game server socket couldn't be created.\n");
                return 0;
        }

        if (!spawn_process(socket, &parent_gs_lib)) {
                return 0;
        }

        return 1;
}
