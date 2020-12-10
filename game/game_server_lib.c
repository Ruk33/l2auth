#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <dlfcn.h>
#include "game_server_lib.h"

#define GAME_SERVER_LIB_PATH "lib/game_server.so"

static void *library = NULL;

static int load_library(void)
{
        if (library)
        {
                dlclose(library);
        }

        library = dlopen(GAME_SERVER_LIB_PATH, RTLD_LAZY);

        if (!library)
        {
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

        if (load_library() == -1)
        {
                return NULL;
        }

        function = dlsym(library, name);

        if (!function)
        {
                printf("Failed to load function %s.\n", name);
                printf("%s.\n", dlerror());
                return NULL;
        }

        return function;
}

void *game_server_lib_handle_init(
        void *(*alloc_cb)(size_t),
        void (*dealloc_cb)(void *)
)
{
        assert(alloc_cb);
        assert(dealloc_cb);

        void *(*handler)() = NULL;
        *(void **) (&handler) = load_function("game_server_init");

        if (!handler)
        {
                return NULL;
        }

        return handler(alloc_cb, dealloc_cb);
}

int game_server_lib_new_connection(int fd, void *data)
{
        assert(fd > 0);
        assert(data);
        void (*handler)() = NULL;
        *(void **) (&handler) = load_function("game_server_new_connection");

        if (!handler)
        {
                return -1;
        }

        handler(fd, data);

        return 1;
}

int game_server_lib_handle_request(
        int fd,
        unsigned char *request,
        size_t request_size,
        void *data,
        void *(*alloc_cb)(size_t),
        void (*dealloc_cb)(void *),
        size_t (*send_response_cb)(int, unsigned char *, size_t),
        void (*close_conn_cb)(int)
)
{
        assert(fd > 0);
        assert(request);
        assert(request_size);
        assert(data);
        assert(alloc_cb);
        assert(dealloc_cb);
        assert(send_response_cb);
        assert(close_conn_cb);

        void (*handler)() = NULL;
        *(void **) (&handler) = load_function("game_server_new_request");

        if (!handler)
        {
                return -1;
        }

        handler(
                fd,
                request,
                request_size,
                data,
                alloc_cb,
                dealloc_cb,
                send_response_cb,
                close_conn_cb
        );

        return 1;
}

int game_server_lib_handle_disconnect(int fd, void *data)
{
        assert(fd > 0);
        assert(data);

        void (*handler)() = NULL;
        *(void **) (&handler) = load_function("game_server_client_disconnected");

        if (!handler)
        {
                return -1;
        }

        handler(fd, data);

        return 1;
}
