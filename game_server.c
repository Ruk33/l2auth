/*
 * Main file/executable start.
 *
 * The game server is "split" in two, one part, is the executable,
 * the program that starts the game server. The other part, is how
 * the game server will behave. This allows to make modifications
 * to how the game server behaves without having to shut it down.
 */
#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <winsock2.h>
#endif

#ifdef __linux__
#include <dlfcn.h>
#include <sys/stat.h>
#include <unistd.h>
#include <pthread.h>
#endif

#include <stdio.h>
#include <stdint.h>

#include "utils.h"
#include "library.c"
#include "thread.c"

#ifdef _WIN32
#include "net_windows.c"
#endif

#ifdef __linux__
#include "net_linux.c"
#endif

typedef void init_cb(byte **memory);

typedef void update_cb(byte **memory);

typedef void handle_request_cb(byte **memory, struct net_socket socket, enum net_event event, byte *read, int size);

static struct library library = {0};

static byte *memory = 0;

static init_cb *init_from_library = 0;

static update_cb *update_from_library = 0;

static handle_request_cb *handle_net_request_in_library = 0;

void net_request_handler(struct net_socket socket, enum net_event event, void *read, int bytes)
{
    handle_net_request_in_library(&memory, socket, event, read, bytes);
}

int net_thread_loop()
{
    trace("listening for requests in port 7777" nl);

    struct net_socket socket = net_port(7777);

    net_block_and_listen(socket, net_request_handler);

    return 0;
}

int main()
{
    trace("starting game server" nl);

    load_library(&library, "game_server.dll");

    init_from_library = (init_cb *) load_function(&library, "init");

    update_from_library = (update_cb *) load_function(&library, "update");

    handle_net_request_in_library = (handle_request_cb *) load_function(&library, "handle_request");

    init_from_library(&memory);

    thread_run(0, net_thread_loop);

    while (1) {
        load_library(&library, "game_server_lib.dll");

        update_from_library = (update_cb *) load_function(&library, "update");

        handle_net_request_in_library = (handle_request_cb *) load_function(&library, "handle_request");

        /*
         * ~60fps
         */
        thread_sleep(16);

        update_from_library(&memory);
    }

    return 0;
}
