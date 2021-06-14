// Required for MAP_ANONYMOUS to be defined.
#define _DEFAULT_SOURCE

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <dlfcn.h>
#include <sys/mman.h>
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

// When a new request arrives, the parent process will spawn
// a child to handle the request. This way, if the request fails
// or crashes, the server will keep running.
static void child_handle_request(
        os_socket_t *socket,
        socket_ev_t ev,
        unsigned char *buf,
        size_t n)
{
        // Required?
        gs_lib->send_response = internal_send_response;

        if (!init_gs_lib()) {
                printf("unable to load game server library.\n");
                exit(EXIT_FAILURE);
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
        exit(EXIT_SUCCESS);
}

static void parent_handle_request(
        os_socket_t *socket,
        socket_ev_t ev,
        unsigned char *buf,
        size_t n)
{
        pid_t pid   = 0;
        int wstatus = 0;

        pid = fork();

        switch (pid) {
        case -1:
                printf("spawn process failed.\n");
                exit(EXIT_FAILURE);
        case 0: // child
                printf("spawned child process. pid: %d.\n", pid);
                child_handle_request(socket, ev, buf, n);
                break;
        default: // parent
                if (waitpid(pid, &wstatus, 0) == -1) {
                        printf("failed to wait for child process.\n");
                        exit(EXIT_FAILURE);
                }
                if (WIFEXITED(wstatus)) {
                        printf("child stopped successfully.\n");
                        return;
                }

                printf("child died unexpectedly. trying to respawn.\n");
                parent_handle_request(socket, ev, buf, n);
        }
}

int main(/* int argc, char **argv */)
{
        os_socket_t *socket = 0;

        // Will be shared across child proccesses.
        gs_lib =
                mmap(NULL,
                     sizeof(*gs_lib),
                     PROT_READ | PROT_WRITE,
                     MAP_SHARED | MAP_ANONYMOUS,
                     -1,
                     0);

        socket = os_socket_create(7777);

        if (!socket) {
                printf("game server socket couldn't be created.\n");
                return EXIT_FAILURE;
        }

        if (!os_socket_handle_requests(socket, parent_handle_request)) {
                printf("game server request can't be handled.\n");
                return EXIT_FAILURE;
        }

        return EXIT_SUCCESS;
}
