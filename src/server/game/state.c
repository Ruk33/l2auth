#include "include/client.h"
#include "include/state.h"

static int free_id(size_t *dest, struct state *src)
{
    assert(src);

    *dest = src->recycled_clients[0];

    if (src->recycled_clients[*dest]) {
        src->recycled_clients[0] = src->recycled_clients[*dest];
        return 0;
    }

    src->recycled_clients[0] = *dest + 1;
    return 1;
}

struct client *state_get_free_client(struct state *src)
{
    size_t id = 0;
    assert(src);
    src->client_count += free_id(&id, src);
    src->clients[id].id = id;
    return &src->clients[id];
}

void state_release_client(struct state *state, struct client *src)
{
    assert(state);
    assert(src);
    state->recycled_clients[src->id] = state->recycled_clients[0];
    state->recycled_clients[0] = src->id;
    *src = (struct client) { 0 };
}