#include <assert.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dlfcn.h>
#include <string.h>
#include <log/log.h>
#include <os/memory.h>
#include "code.h"

typedef void *(* code_malloc_cb)
(size_t how_much);

typedef void (* code_mfree_cb)
(void *memory);

struct Code {
        memory *mem;

        time_t last_changed;
        void *library;

        void *(* handle_server_initialize)
        (code_malloc_cb m, code_mfree_cb f, send_reponse_cb s);

        void (* handle_new_conn)
        (void *server_data, int client_id);

        void (* handle_request)
        (void *server_data, int client_id, unsigned char *buf, size_t buf_size);
};

// Code is a shit name, I know, needs to be updated
// Code represents the library/business code to be loaded dinamically
// allowing hot code reload improving development flow
static struct Code *code = NULL;

void *code_malloc
(size_t how_much)
{
        assert(code);
        assert(code->mem);
        assert(how_much > 0);
        return memory_alloc(code->mem, how_much);
}

void code_mfree
(void *mem)
{
        assert(code);
        assert(mem);
        memory_free(mem);
}

static time_t lib_last_modified
(void)
{
        struct stat file_stat;
        int err = stat("./build/libgameserver.so", &file_stat);
    
        if (err != 0) {
                log_fatal("Error while checking if lib changed");
                return 0;
        }

        return file_stat.st_mtime;
}

static void code_load_if_required
(void)
{
        assert(code);

        time_t llm = lib_last_modified();
        int did_not_changed = llm <= code->last_changed;

        if (did_not_changed) return;

        if (code->library) dlclose(code->library);
        code->library = dlopen("./build/libgameserver.so", RTLD_LAZY);
        code->last_changed = llm;

        log_info("Loading library. Result: %s", dlerror());

        code->handle_server_initialize = dlsym(code->library, "entry_initialize_server");
        log_info("Loading entry_initialize_server. Result: %s", dlerror());

        code->handle_new_conn = dlsym(code->library, "entry_new_conn");
        log_info("Loading entry_new_conn. Result: %s", dlerror());

        code->handle_request = dlsym(code->library, "entry_handle_request");
        log_info("Loading entry_handle_request. Result: %s", dlerror());
}

void *code_initialize_server
(send_reponse_cb s)
{
        assert(code);
        code_load_if_required();
        return code->handle_server_initialize(&code_malloc, &code_mfree, s);
}

void code_request_handle
(void *server_data, int client_id, unsigned char *buf, size_t buf_size)
{
        assert(code);
        assert(buf);
        code_load_if_required();
        code->handle_request(server_data, client_id, buf, buf_size);
}

void code_new_conn
(void *server_data, int client_id)
{
        assert(code);
        code_load_if_required();
        code->handle_new_conn(server_data, client_id);
}

void code_init
(memory *mem)
{
        assert(mem);

        code = memory_alloc(mem, sizeof(*code));
        code->mem = mem;
        code->last_changed = 0;
        code->library = NULL;
        code->handle_server_initialize = NULL;
        code->handle_new_conn = NULL;
        code->handle_request = NULL;

        code_load_if_required();
}
