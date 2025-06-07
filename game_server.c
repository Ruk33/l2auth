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
#endif

#include <stdio.h>
#include <stdint.h>

#include "utils.c"
#include "library.c"

#ifdef _WIN32
#include "net_windows.c"
#endif

#ifdef __linux__
#include "net_linux.c"
#endif

typedef void init_cb(byte **memory);
typedef void update_cb(byte **memory);
typedef void handle_request_cb(byte **memory, struct net_socket socket, enum net_event event, byte *read, int size);

static byte *memory = 0;

static struct library library = {0};
static init_cb *init_from_library = 0;
static update_cb *update_from_library = 0;
static handle_request_cb *handle_net_request_in_library = 0;

void net_request_handler(struct net_socket socket, enum net_event event, void *read, int bytes)
{
    load_library(&library, "game_server_lib.dll");
    update_from_library = (update_cb *) load_function(&library, "update");
    handle_net_request_in_library = (handle_request_cb *) load_function(&library, "handle_request");

    handle_net_request_in_library(&memory, socket, event, read, bytes);
    update_from_library(&memory);
}

int main()
{
    int port = 7777;

    trace("starting game server in port %d" nl, port);

    load_library(&library, "game_server_lib.dll");
    init_from_library = (init_cb *) load_function(&library, "init");
    update_from_library = (update_cb *) load_function(&library, "update");
    handle_net_request_in_library = (handle_request_cb *) load_function(&library, "handle_request");

    init_from_library(&memory);

    struct net_socket socket = net_port(port);

    net_block_and_listen(socket, net_request_handler);

    return 0;
}
