#include <assert.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dlfcn.h>
#include <string.h>
#include <pthread.h>
#include <log/log.h>
#include <os/memory.h>
#include "code.h"

typedef void *(* code_malloc_cb)
(size_t how_much);

typedef void (* code_mfree_cb)
(void *memory);

#define LIB_PATH "./build/libgameserver.so"

#define DLEXPR_OR_ERR(expr)                             \
        do {                                            \
                if (!(expr)) log_fatal(dlerror());      \
        } while (0);

struct Code {
        memory *mem;

        time_t last_changed;
        void *library;

        void *(* server_init)
        (code_malloc_cb m, code_mfree_cb f, send_reponse_cb s);

        void (* new_conn)
        (void *server_data, int client_id);

        void (* handle_request)
        (void *server_data, int client_id, unsigned char *buf, size_t buf_size);
};

// Code is a shit name, I know, needs to be updated
// Code represents the library/business code to be loaded dinamically
// allowing hot code reload improving development flow
static struct Code *code = NULL;
static pthread_mutex_t thread_mutex = PTHREAD_MUTEX_INITIALIZER;

void *code_malloc
(size_t how_much)
{
        assert(code);
        assert(code->mem);
        assert(how_much > 0);

        void *mem = NULL;

        pthread_mutex_lock(&thread_mutex);
        mem = memory_alloc(code->mem, how_much);
        pthread_mutex_unlock(&thread_mutex);

        return mem;
}

void code_mfree
(void *mem)
{
        assert(code);
        assert(mem);

        pthread_mutex_lock(&thread_mutex);
        memory_free(mem);
        pthread_mutex_unlock(&thread_mutex);
}

static time_t lib_last_modified
(void)
{
        struct stat file_stat;
        int err = stat(LIB_PATH, &file_stat);
    
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

        time_t llm = 0;
        int did_not_changed = 0;

        llm = lib_last_modified();
        did_not_changed = llm <= code->last_changed;

        if (did_not_changed) return;
        if (code->library) dlclose(code->library);

        DLEXPR_OR_ERR(code->library = dlopen(LIB_PATH, RTLD_LAZY));

        if (!code->library) {
                log_fatal("Dynamic library could not be loaded.");
                exit(EXIT_FAILURE);
                return;
        }

        code->last_changed = llm;

        DLEXPR_OR_ERR(
                code->server_init = dlsym(
                        code->library,
                        "entry_initialize_server"
                )
        );

        DLEXPR_OR_ERR(
                code->new_conn = dlsym(
                        code->library,
                        "entry_new_conn"
                )
        );

        DLEXPR_OR_ERR(
                code->handle_request = dlsym(
                        code->library,
                        "entry_handle_request"
                )
        );
}

void *code_initialize_server
(send_reponse_cb s)
{
        assert(code);
        code_load_if_required();
        return code->server_init(&code_malloc, &code_mfree, s);
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
        code->new_conn(server_data, client_id);
}

void code_init
(memory *mem)
{
        assert(mem);

        code = memory_alloc(mem, sizeof(*code));
        code->mem = mem;
        code->last_changed = 0;
        code->library = NULL;
        code->server_init = NULL;
        code->new_conn = NULL;
        code->handle_request = NULL;

        code_load_if_required();
}
