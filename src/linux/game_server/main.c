// Required for MAP_ANONYMOUS to be defined.
#define _DEFAULT_SOURCE

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <dlfcn.h>
#include <sys/mman.h>
#include <sys/wait.h>

#include "../os_io.c"
#include "../../util.c"
#include "../../recycle_id.c"

#include "../../include/gs_lib.h"

#define GAME_SERVER_LIB_PATH "./game_server_lib.so"

static void *handle = 0;
static void (*on_load)(gs_lib_t *);
static void (*on_unload)(void);
static void (*on_new_conn)(os_io_t *);
static void (*on_new_req)(os_io_t *, void *, size_t);
static void (*on_disconnect)(os_io_t *);
static void (*on_tick)(double);

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

static void internal_send_response(os_io_t *io, void *buf, size_t n)
{
        os_io_write(io, buf, n);
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
        *(void **) (&on_tick)       = load_lib_function("gs_lib_tick");

        all_load =
                (handle && on_load && on_unload && on_new_conn && on_new_req &&
                 on_disconnect && on_tick);

        if (!all_load) {
                return 0;
        }

        on_load(gs_lib);

        return 1;
}

// When a new request arrives, the parent process will spawn
// a child to handle the request. This way, if the request fails
// or crashes, the server will keep running.
static void
child_io_event(os_io_t *socket, os_io_event_t event, void *buf, size_t n)
{
        // Required?
        gs_lib->send_response = internal_send_response;

        if (!init_gs_lib()) {
                printf("unable to load game server library.\n");
                exit(EXIT_FAILURE);
        }

        switch (event) {
        case OS_IO_SOCKET_CONNECTION:
                // printf("new connection.\n");
                on_new_conn(socket);
                break;
        case OS_IO_SOCKET_REQUEST:
                // printf("new request.\n");
                on_new_req(socket, buf, n);
                break;
        case OS_IO_SOCKET_DISCONNECTED:
                // printf("disconnect.\n");
                on_disconnect(socket);
                // Todo: closing the socket here indeeds closes it
                // but also terminates the server. Investigate.
                // os_socket_close(socket);
                break;
        case OS_IO_TIMER_TICK:
                on_tick(0);
                break;
        default:
                break;
        }

        fflush(stdout);
        exit(EXIT_SUCCESS);
}

static void
parent_io_event(os_io_t *socket, os_io_event_t event, void *buf, size_t n)
{
        pid_t pid   = 0;
        int wstatus = 0;

        pid = fork();

        switch (pid) {
        case -1:
                printf("spawn process failed.\n");
                exit(EXIT_FAILURE);
        case 0: // child
                // printf("spawned child process. pid: %d.\n", pid);
                child_io_event(socket, event, buf, n);
                break;
        default: // parent
                if (waitpid(pid, &wstatus, 0) == -1) {
                        printf("failed to wait for child process.\n");
                        exit(EXIT_FAILURE);
                }
                if (WIFEXITED(wstatus)) {
                        // printf("child stopped successfully.\n");
                        return;
                }

                // Note: Should we back the game state before
                // the request crashed?

                printf("child died unexpectedly, closing client connection.\n");
                child_io_event(socket, OS_IO_SOCKET_DISCONNECTED, buf, n);

                // Todo: Doesn't work as expected. Investigate.
                // os_socket_close(socket);
        }
}

int main(/* int argc, char **argv */)
{
        os_io_t *timer  = 0;
        os_io_t *socket = 0;

        // Will be shared across child proccesses.
        gs_lib =
                mmap(NULL,
                     sizeof(*gs_lib),
                     PROT_READ | PROT_WRITE,
                     MAP_SHARED | MAP_ANONYMOUS,
                     -1,
                     0);

        timer  = os_io_timer(1);
        socket = os_io_socket_create(7777, 30);

        if (!timer) {
                printf("game server timer couldn't be created.\n");
                os_io_close(timer);
                os_io_close(socket);
                return EXIT_FAILURE;
        }

        if (!socket) {
                printf("game server socket couldn't be created.\n");
                os_io_close(timer);
                os_io_close(socket);
                return EXIT_FAILURE;
        }

        if (!os_io_listen(parent_io_event)) {
                printf("game server request can't be handled.\n");
                os_io_close(timer);
                os_io_close(socket);
                return EXIT_FAILURE;
        }

        os_io_close(timer);
        os_io_close(socket);

        return EXIT_SUCCESS;
}
