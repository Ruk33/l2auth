#include <assert.h>
#include "include/state.h"

struct client *state_get_free_client(struct state *src)
{
    assert(src);
    // Todo: Implement properly.
    src->count += 1;
    return &src->clients[src->count - 1];
}

void state_release_client(struct state *state, struct client *src)
{
    assert(state);
    assert(src);
    // Todo: Implement properly.
    state->count -= 1;
}