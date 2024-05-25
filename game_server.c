#include <assert.h>
#include <stdio.h>

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

#ifdef __linux__
#include <unistd.h>
#endif

#include "library.h"
#include "net.h"
#include "pevent.h"

struct state {
    void *buf;
    struct library gameserver;
};

static struct state state = {0};

static void handle_net_event(int socket, enum net_event event, void *read, unsigned long long len)
{
    enum library_status library_status = library_failed;
    peventf *on_pevent = 0;
    /*
     * try loading the library n amount of times.
     */
    for (int i = 0; i < 5; i++) {
        library_status = library_load(&state.gameserver);
        if (library_status == library_needs_reload) {
            /*
             * before reloading the library, make sure
             * to run the before reload event.
             */
            on_pevent = (peventf *) library_function(&state.gameserver, "on_pevent");
            assert(on_pevent);
            on_pevent(&state.buf, pevent_before_reload, 0);
            /*
             * if the library can't be reloaded, just exit.
             */
            library_status = library_load(&state.gameserver);
            if (library_status != library_reloaded) {
                library_status = library_failed;
                break;
            }
            /*
             * library reloaded! run after reload event.
             */
            on_pevent = (peventf *) library_function(&state.gameserver, "on_pevent");
            assert(on_pevent);
            on_pevent(&state.buf, pevent_after_reload, 0);
        }
        /*
         * the library has been loaded, we can exit the loop.
         */
        if (library_status != library_failed)
            break;
        /*
         * before trying to load again, sleep for a second
         * letting the os make the required writing/processing
         * in case the library was updated.
         */
#ifdef _WIN32
        Sleep(1000);
#endif
#ifdef __linux__
        sleep(1);
#endif
    }
    /*
     * if the library couldn't be loaded, crash the server.
     */
    if (library_status == library_failed) {
        fprintf(stderr, "failed to load library.\n");
        assert(!"game server library couldn't be loaded");
        return;
    }

    on_pevent = (peventf *) library_function(&state.gameserver, "on_pevent");
    assert(on_pevent);
    
    union ppayload payload = {0};
    payload.pevent_socket.socket = socket;
    payload.pevent_socket.read = read;
    payload.pevent_socket.len = len;

    switch (event) {
        case net_conn:
        on_pevent(&state.buf, pevent_socket_connection, &payload);
        break;
        
        case net_closed:
        on_pevent(&state.buf, pevent_socket_disconnected, &payload);
        break;
        
        case net_read:
        on_pevent(&state.buf, pevent_socket_request, &payload);
        break;

        default:
        break;
    }
}

int main()
{
#ifdef _WIN32
    state.gameserver.path = "game_server.dll";
#endif

#ifdef __linux__
    state.gameserver.path = "./game_server.so";
#endif

    if (library_load(&state.gameserver) == library_failed) {
        fprintf(stderr, "failed to load game server library.\n");
        return 0;
    }

    peventf *on_pevent = (peventf *) library_function(&state.gameserver, "on_pevent");
    assert(on_pevent);

    if (!on_pevent(&state.buf, pevent_init, 0))
        return 0;

    unsigned short port = 7777;
    int socket = net_port(port);
    fprintf(stderr, "game server listening on port %d\n", port);
    net_listen(socket, handle_net_event);
    
    return 0;
}
