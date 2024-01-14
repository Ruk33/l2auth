#include <stddef.h> // size_t
#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

#include "net.h"
#include "pevent.h"

static int lib_load(void);
static int lib_has_new_version(void);

struct state {
    void *buf;
    void *lib;
    peventf *on_pevent;
#ifdef _WIN32
    FILETIME lib_write_time;
#endif
};

static struct state state = {0};

#ifdef _WIN32
static int lib_load(void)
{
    if (state.lib) {
        FreeLibrary(state.lib);
        state.lib = 0;
    }
    // create a copy of the dll and then load the copy,
    // not the original file, otherwise, windows will
    // complain when trying to make a change to it.
    if (CopyFile("game_server.dll", ".game_server.dll", FALSE) == 0)
        return 0;

    state.lib = LoadLibraryA(".game_server.dll");
    if (!state.lib)
        return 0;
    
    WIN32_FILE_ATTRIBUTE_DATA data = {0};
    GetFileAttributesEx("game_server.dll", GetFileExInfoStandard, &data);
    
    state.lib_write_time = data.ftLastWriteTime;
    state.on_pevent = (peventf *) GetProcAddress(state.lib, "on_pevent");
    
    return 1;
}

static int lib_has_new_version(void)
{
    WIN32_FILE_ATTRIBUTE_DATA data = {0};
    GetFileAttributesEx("game_server.dll", GetFileExInfoStandard, &data);
    int has_new_version = CompareFileTime(&state.lib_write_time, &data.ftLastWriteTime) != 0;
    return has_new_version;
}

static void load_lib_if_required(void)
{
    if (lib_has_new_version()) {
        state.on_pevent(&state.buf, pevent_before_reload, 0);

        int tries = 5;
        int loaded = 0;
        for (int i = 0; i < tries; i++) {
            if (lib_load()) {
                loaded = 1;
                break;
            }
            Sleep(1000);
        }

        if (!loaded)
            ExitProcess(0);

        state.on_pevent(&state.buf, pevent_after_reload, 0);
    }
}
#endif

static void handle_net_event(int socket, enum net_event event, void *read, size_t len)
{
    load_lib_if_required();
    
    union ppayload payload = {0};
    payload.pevent_socket.socket = socket;
    payload.pevent_socket.read = read;
    payload.pevent_socket.len = len;

    switch (event) {
        case net_conn:
        state.on_pevent(&state.buf, pevent_socket_connection, &payload);
        break;
        
        case net_closed:
        state.on_pevent(&state.buf, pevent_socket_disconnected, &payload);
        break;
        
        case net_read:
        state.on_pevent(&state.buf, pevent_socket_request, &payload);
        break;
        
        default:
        break;
    }
}

int main()
{
    if (!lib_load())
        return 0;
    
    if (!state.on_pevent(&state.buf, pevent_init, 0))
        return 0;

    int socket = net_port(7777);
    net_listen(socket, handle_net_event);
    
    return 0;
}
