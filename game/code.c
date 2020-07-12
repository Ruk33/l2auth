#include <dlfcn.h>
#include <log/log.h>
#include "code.h"

void game_code_load(struct GameCode* game_code)
{
        if (game_code->library) dlclose(game_code->library);

        game_code->library = dlopen("libgameserver.so", RTLD_LAZY);

        if (!game_code->library) {
                log_fatal("Could not load library");
                return;
        }

        log_info("Library successfully loaded");

        game_code->on_new_client = dlsym(game_code->library, "game_client_handler_new");
        game_code->on_client_request = dlsym(game_code->library, "game_client_handler_request");
        // game_code->on_client_end = dlsym(game_code->library, "game_client_end_handler");

        if (game_code->on_new_client) log_info("game_client_handler_new successfully loaded");
        else log_fatal("Failed to load game_client_handler_new");

        if (game_code->on_client_request) log_info("game_client_handler_request successfully loaded");
        else log_fatal("Failed to load game_client_handler_request");

        // if (!game_code->on_client_end) log_fatal("Failed to load game_client_end_handler");
}
