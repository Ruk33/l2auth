#include <stdlib.h>
#include <time.h>
#include <core/memory.h>
#include <game/state.h>
#include <game/code.h>
#include <game/handle_client.h>

int main()
{
        size_t preallocated_mem = 10 * 1024 * 1024;

        struct GameState* game_state = calloc(1, sizeof(struct GameState));
        struct GameCode* game_code = calloc(1, sizeof(struct GameCode));

        unsigned short port = 7777;
        size_t max_players = 2;

        srand((unsigned int) time(NULL));

        game_state->memory = malloc(preallocated_mem);
        memory_init(game_state->memory, preallocated_mem);

        game_code_load(game_code);
        game_handle_client_listen(game_state, game_code, port, max_players);

        return 0;
}
