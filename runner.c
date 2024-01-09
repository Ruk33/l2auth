#include <stddef.h> // size_t
#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

#include "asocket.h"

typedef int on_init(void **buf);
typedef void on_connection(void **buf, int socket);
typedef void on_request(void **buf, int socket, void *request, size_t len);
typedef void on_response(void **buf, int socket);
typedef void on_disconnect(void **buf, int socket);
typedef int on_tick(void **buf);

static int lib_load(void);
static int lib_has_new_version(void);

struct state {
    void *buf;
    void *lib;
    
    on_init *on_init;
    on_connection *on_connection;
    on_request *on_request;
    on_response *on_response;
    on_disconnect *on_disconnect;
    on_tick *on_tick;
    
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
    
    state.on_init = (on_init *) GetProcAddress(state.lib, "on_init");
    state.on_connection = (on_connection *) GetProcAddress(state.lib, "on_connection");
    state.on_request = (on_request *) GetProcAddress(state.lib, "on_request");
    state.on_response = (on_response *) GetProcAddress(state.lib, "on_response");
    state.on_disconnect = (on_disconnect *) GetProcAddress(state.lib, "on_disconnect");
    state.on_tick = (on_tick *) GetProcAddress(state.lib, "on_tick");
    
    return 1;
}

static int lib_has_new_version(void)
{
    WIN32_FILE_ATTRIBUTE_DATA data = {0};
    GetFileAttributesEx("game_server.dll", GetFileExInfoStandard, &data);
    int has_new_version = CompareFileTime(&state.lib_write_time, &data.ftLastWriteTime) != 0;
    return has_new_version;
}
#endif

static void load_lib_if_required(void)
{
#ifdef _WIN32
    if (lib_has_new_version()) {
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
    }
#endif
}

void handle_event(int socket, enum asocket_event event, void *read, size_t len)
{
    load_lib_if_required();
    
    switch (event) {
        case ASOCKET_NEW_CONN:
        state.on_connection(&state.buf, socket);
        break;
        
        case ASOCKET_CLOSED:
        state.on_disconnect(&state.buf, socket);
        break;
        
        case ASOCKET_READ:
        state.on_request(&state.buf, socket, read, len);
        break;
        
        case ASOCKET_CAN_WRITE:
        state.on_response(&state.buf, socket);
        break;
        
        // case ASOCKET_TIMEOUT:
        // state.on_tick(&state.buf);
        // break;
        
        default:
        break;
    }
}

DWORD timer_thread(LPVOID param)
{
    param = param;
    while (1) {
        Sleep(1000);
        load_lib_if_required();
        state.on_tick(&state.buf);
    }
    return 0;
}

int main()
{
    if (!lib_load())
        return 0;
    
    if (!state.on_init(&state.buf))
        return 0;
    
    CreateThread(0, 0, timer_thread, 0, 0, 0);
    
    int socket = asocket_port(7777);
    asocket_listen(socket, handle_event);
    
    return 0;
}
