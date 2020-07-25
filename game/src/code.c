#include <dlfcn.h>
#include <log/log.h>
#include "code.h"

void code_load(struct Code *code)
{
        if (code->library) dlclose(code->library);

        code->library = dlopen("libgameserver.so", RTLD_LAZY);

        if (!code->library) {
                log_fatal("Could not load library");
                return;
        }

        log_info("Library successfully loaded");

        code->new_conn = dlsym(code->library, "entry_new_conn");
        code->handle_request = dlsym(code->library, "entry_handle_request");
        code->handle_disconnect = dlsym(code->library, "entry_handle_disconnect");

        if (code->new_conn && code->handle_request && code->handle_disconnect) {
                log_info("Functions successfully loaded");
                return;
        }

        if (!code->new_conn) log_fatal("Failed to load entry_new_conn");
        if (!code->handle_request) log_fatal("Failed to load entry_handle_request");
        if (!code->handle_disconnect) log_fatal("Failed to load entry_handle_disconnect");
}
