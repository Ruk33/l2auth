#ifndef UNICODE
#define UNICODE
#endif

#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "../../include/gs_types.h"

#include "../os_io.c"
#include "../../util.c"

#define GAME_SERVER_LIB_PATH     ("game_server_lib.dll")
#define GAME_SERVER_LIB_TMP_PATH ("game_server_lib_2.dll")

struct lib {
    HMODULE handle;
    FILETIME load_time;
    void (*on_load)(struct gs_state *);
    void (*on_unload)(void);
    void (*on_new_conn)(struct os_io *);
    void (*on_new_req)(struct os_io *, void *, size_t);
    void (*on_disconnect)(struct os_io *);
    void (*on_tick)(double);
};

static struct lib lib = { 0 };

static struct gs_state *game_server = 0;

static void *load_lib_function(char *name)
{
    void *function = 0;

    assert(name);

    if (!lib.handle) {
        return 0;
    }

    function = (void *) GetProcAddress(lib.handle, name);

    if (!function) {
        printf("failed to load function %s.\n", name);
    }

    return function;
}

static void internal_send_response(struct os_io *io, void *buf, size_t n)
{
    if (!io) {
        printf("internal send response, no io? ignoring.\n");
        return;
    }

    os_io_write(io, buf, n);
}

static void internal_disconnect(struct os_io *socket)
{
    if (!socket) {
        return;
    }
    lib.on_disconnect(socket);
    os_io_close(socket);
}

static int init_gs_lib(void)
{
    HANDLE file = 0;

    HMODULE already_loaded = 0;

    WIN32_FILE_ATTRIBUTE_DATA data = { 0 };

    FILETIME file_time = { 0 };

    int all_load = 0;

    GetFileAttributesExA(GAME_SERVER_LIB_PATH, GetFileExInfoStandard, &data);
    file_time = data.ftLastWriteTime;

    // Don't load if there are no changes in the lib.
    if (lib.handle && CompareFileTime(&lib.load_time, &file_time) == 0) {
        return 1;
    }

    if (lib.handle) {
        lib.on_unload();

        already_loaded = GetModuleHandleA(GAME_SERVER_LIB_TMP_PATH);

        while (already_loaded) {
            FreeLibrary(already_loaded);
            already_loaded = GetModuleHandleA(GAME_SERVER_LIB_TMP_PATH);
        }
    }

    lib = (struct lib){ 0 };

    if (!CopyFileA(GAME_SERVER_LIB_PATH, GAME_SERVER_LIB_TMP_PATH, 0)) {
        printf("unable to copy library %d.\n", GetLastError());
        return 0;
    }

    lib.handle = LoadLibraryA(GAME_SERVER_LIB_TMP_PATH);

    if (!lib.handle) {
        printf("failed to load library: %d.\n", GetLastError());
        return 0;
    }

    *(void **) (&lib.on_load)       = load_lib_function("gs_lib_load");
    *(void **) (&lib.on_unload)     = load_lib_function("gs_lib_unload");
    *(void **) (&lib.on_new_conn)   = load_lib_function("gs_lib_new_conn");
    *(void **) (&lib.on_new_req)    = load_lib_function("gs_lib_new_req");
    *(void **) (&lib.on_disconnect) = load_lib_function("gs_lib_disconnect");
    *(void **) (&lib.on_tick)       = load_lib_function("gs_lib_tick");

    all_load = (lib.handle && lib.on_load && lib.on_unload && lib.on_new_conn &&
                lib.on_new_req && lib.on_disconnect && lib.on_tick);

    if (!all_load) {
        lib = (struct lib){ 0 };
        return 0;
    }

    lib.load_time = file_time;
    lib.on_load(game_server);

    return 1;
}

static void
on_io_event(struct os_io *socket, os_io_event_t event, void *buf, size_t n)
{
    assert(game_server->send_response);

    if (!init_gs_lib()) {
        printf("unable to load game server library.\n");
        printf("ignoring request.\n");
        return;
    }

    switch (event) {
    case OS_IO_SOCKET_CONNECTION:
        lib.on_new_conn(socket);
        break;
    case OS_IO_SOCKET_REQUEST:
        lib.on_new_req(socket, buf, n);
        break;
    case OS_IO_SOCKET_DISCONNECTED:
        lib.on_disconnect(socket);
        // Todo: closing the socket here indeeds closes it
        // but also terminates the server. Investigate.
        // os_socket_close(socket);
        break;
    case OS_IO_TIMER_TICK:
        lib.on_tick(0.1);
        break;
    default:
        break;
    }

    fflush(stdout);
}

int main(/* int argc, char **argv */)
{
    struct os_io *socket = 0;

    game_server = calloc(1, sizeof(*game_server));

    game_server->send_response = internal_send_response;
    game_server->disconnect    = internal_disconnect;

    socket = os_io_socket_create(7777, 30);
    os_io_timer(0.1);

    if (!socket) {
        printf("game server socket couldn't be created.\n");
        os_io_close(socket);
        return EXIT_FAILURE;
    }

    if (!os_io_listen(on_io_event)) {
        printf("game server request can't be handled.\n");
        os_io_close(socket);
        return EXIT_FAILURE;
    }

    os_io_close(socket);

    printf("shutting down.\n");

    return EXIT_SUCCESS;
}
