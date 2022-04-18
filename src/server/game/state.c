#include "include/client.h"
#include "include/state.h"

// Get a reusable/free id from src and store it on dest.
// If a new id needs to be "created", 1 will be returned,
// otherwise, 0 is returned.
static int free_id(size_t *dest, size_t *src)
{
    assert(dest);
    assert(src);

    *dest = src[0];

    if (src[*dest]) {
        src[0] = src[*dest];
        return 0;
    }

    src[0] = *dest + 1;
    return 1;
}

static void recycle_id(size_t *dest, size_t id)
{
    assert(dest);
    dest[id] = dest[0];
    dest[0] = id;
}

struct client *state_get_free_client(struct state *src)
{
    size_t id = 0;
    assert(src);
    src->client_count += free_id(&id, src->recycled_clients);
    src->clients[id].id = id;
    return &src->clients[id];
}

void state_add_player(struct state *state, struct client *client, struct character *src)
{
    size_t id = 0;
    assert(state);
    assert(client);
    assert(src);
    state->character_count += free_id(&id, state->recycled_characters);
    state->characters[id] = *src;
    client->character = &state->characters[id];
}

void state_release_client(struct state *state, struct client *src)
{
    assert(state);
    assert(src);
    recycle_id(state->recycled_clients, src->id);
    *src = (struct client) { 0 };
}