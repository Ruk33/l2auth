#include <assert.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dlfcn.h>
#include <log/log.h>
#include "memory_manager.h"
#include "game_server_lib.h"

typedef void *(* malloc_cb)
(size_t how_much);

typedef void (* free_cb)
(void *mem);

#define LIB_PATH "./build/libgameserver.so"

#define DLEXPR_OR_ERR(expr)                             \
        do {                                            \
                if (!(expr)) log_fatal(dlerror());      \
        } while (0);

struct GameServerLib {
        time_t last_changed;
        void *library;

        void *(* server_init)
        (malloc_cb m, free_cb f, send_reponse_cb s);

        void (* new_conn)
        (void *server_data, int client_id);

        void (* request)
        (void *server_data, int client_id, unsigned char *buf, size_t buf_size);

        void (* disconnect)
        (void *server_data, int client_id);
};

static struct GameServerLib *game_server_lib = NULL;

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

static void game_server_lib_load_if_required
(void)
{
        assert(game_server_lib);

        time_t llm = 0;
        int did_not_changed = 0;

        llm = lib_last_modified();
        did_not_changed = llm <= game_server_lib->last_changed;

        if (did_not_changed) return;
        if (game_server_lib->library) dlclose(game_server_lib->library);

        DLEXPR_OR_ERR(game_server_lib->library = dlopen(LIB_PATH, RTLD_LAZY));

        if (!game_server_lib->library) {
                log_fatal("Dynamic library could not be loaded.");
                exit(EXIT_FAILURE);
                return;
        }

        game_server_lib->last_changed = llm;

        DLEXPR_OR_ERR(
                game_server_lib->server_init = dlsym(
                        game_server_lib->library,
                        "entry_initialize_server"
                )
        );

        DLEXPR_OR_ERR(
                game_server_lib->new_conn = dlsym(
                        game_server_lib->library,
                        "entry_new_conn"
                )
        );

        DLEXPR_OR_ERR(
                game_server_lib->request = dlsym(
                        game_server_lib->library,
                        "entry_handle_request"
                )
        );

        DLEXPR_OR_ERR(
                game_server_lib->disconnect = dlsym(
                        game_server_lib->library,
                        "entry_handle_disconnect"
                )
        );
}

void *game_server_lib_init_server
(send_reponse_cb s)
{
        assert(game_server_lib);
        game_server_lib_load_if_required();
        return game_server_lib->server_init(
                &memory_manager_alloc,
                &memory_manager_free,
                s
        );
}

void game_server_lib_new_conn
(void *server_data, int client_id)
{
        assert(game_server_lib);

        game_server_lib_load_if_required();
        game_server_lib->new_conn(server_data, client_id);
}

void game_server_lib_request
(void *server_data, int client_id, unsigned char *buf, size_t buf_size)
{
        assert(game_server_lib);
        assert(buf);

        game_server_lib_load_if_required();
        game_server_lib->request(server_data, client_id, buf, buf_size);
}

void game_server_lib_disconnect
(void *server_data, int client_id)
{
        assert(game_server_lib);
        game_server_lib_load_if_required();
        game_server_lib->disconnect(server_data, client_id);
}

void game_server_lib_init
(void)
{
        game_server_lib = memory_manager_alloc(sizeof(*game_server_lib));

        game_server_lib->last_changed = 0;
        game_server_lib->library = NULL;
        game_server_lib->server_init = NULL;
        game_server_lib->new_conn = NULL;
        game_server_lib->request = NULL;
        game_server_lib->disconnect = NULL;

        game_server_lib_load_if_required();
}

void game_server_lib_finish
(void)
{
        assert(game_server_lib);

        memory_manager_free(game_server_lib);
        game_server_lib = NULL;
}
