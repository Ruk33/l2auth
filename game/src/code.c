#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dlfcn.h>
#include <string.h>
#include <log/log.h>
#include "code.h"

static time_t lib_last_modified()
{
        struct stat file_stat;
        int err = stat("./build/libgameserver.so", &file_stat);
    
        if (err != 0) {
                log_fatal("Error while checking if lib changed");
                return 0;
        }

        return file_stat.st_mtime;
}

void code_load(struct Code *code)
{
        time_t llm = lib_last_modified();

        if (llm <= code->last_changed) return;

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
        memcpy(&code->last_changed, &llm, sizeof(llm));

        if (code->new_conn && code->handle_request && code->handle_disconnect) {
                log_info("Functions successfully loaded");
                return;
        }

        if (!code->new_conn) log_fatal("Failed to load entry_new_conn");
        if (!code->handle_request) log_fatal("Failed to load entry_handle_request");
        if (!code->handle_disconnect) log_fatal("Failed to load entry_handle_disconnect");
}
