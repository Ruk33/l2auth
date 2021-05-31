#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <dlfcn.h>
#include "game_server_lib.h"

/**
 * TODO: Use proper types for functions instead of void pointers.
 */

#define GAME_SERVER_LIB_PATH "lib/game_server.so"

typedef enum {
        host_to_gs_new_conn,
        host_to_gs_request,
        host_to_gs_disconnect = 3,
} host_to_gs_msg_type;

typedef struct {
        int socket;
} host_client_conn_msg_t;

typedef struct {
        int            socket;
        unsigned char *request;
        ssize_t        size;
} host_request_msg_t;

typedef struct {
        int socket;
} host_client_exit_msg_t;

static void *library = NULL;

static int load_library(void)
{
        if (library) {
                dlclose(library);
        }

        library = dlopen(GAME_SERVER_LIB_PATH, RTLD_LAZY);

        if (!library) {
                printf("Failed to load game server library.\n");
                printf("%s.\n", dlerror());
                return -1;
        }

        return 1;
}

static void *load_function(char *name)
{
        assert(name);

        void *function = NULL;

        if (load_library() == -1) {
                return NULL;
        }

        function = dlsym(library, name);

        if (!function) {
                printf("Failed to load function %s.\n", name);
                printf("%s.\n", dlerror());
                return NULL;
        }

        return function;
}

void *game_server_lib_handle_init(
        void * memory,
        size_t size,
        void (*msg_to_host_cb)(int type, void *))
{
        void *(*handler)()    = NULL;
        *(void **) (&handler) = load_function("game_server_init");

        assert(memory);
        assert(msg_to_host_cb);

        if (!handler) {
                return NULL;
        }

        return handler(memory, size, msg_to_host_cb);
}

int game_server_lib_new_connection(int fd, void *data)
{
        void (*handler)() = NULL;

        host_client_conn_msg_t payload = { 0 };
        *(void **) (&handler)          = load_function("game_server_request");

        assert(data);

        if (!handler) {
                return -1;
        }

        payload.socket = fd;
        handler(data, host_to_gs_new_conn, &payload, sizeof(payload));

        return 1;
}

int game_server_lib_handle_request(
        int            fd,
        unsigned char *request,
        ssize_t        request_size,
        void *         data)
{
        void (*handler)() = NULL;

        host_request_msg_t payload = { 0 };

        assert(request);
        assert(request_size);
        assert(data);

        *(void **) (&handler) = load_function("game_server_request");

        if (!handler) {
                return -1;
        }

        payload.socket  = fd;
        payload.request = request;
        payload.size    = request_size;

        handler(data, host_to_gs_request, &payload, sizeof(payload));

        return 1;
}

int game_server_lib_handle_disconnect(int fd, void *data)
{
        void (*handler)() = NULL;

        host_client_exit_msg_t payload = { 0 };

        assert(data);

        *(void **) (&handler) = load_function("game_server_request");

        if (!handler) {
                return -1;
        }

        payload.socket = fd;
        handler(data, 3, &payload, sizeof(payload));

        return 1;
}

void game_server_lib_handle_timer_tick(double delta, void *data)
{
        /**
         * TODO: Implement
         */

        if (delta && data) {
        }
        // void (*handler)(double, void *) = NULL;
        // *(void **) (&handler) = load_function("game_server_timer_tick");

        // if (!handler) {
        //         printf("Couldn't load game_server_timer_tick\n");
        //         return;
        // }

        // handler(delta, data);
}
