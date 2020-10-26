#include <assert.h>
#include "host.h"
#include "client.h"
#include "world_client_state.h"
#include "world_state.h"

struct WorldState
{
        struct WorldClientState *clients;
        host_malloc_cb m;
        host_mfree_cb f;
};

struct WorldState *world_state_new(host_malloc_cb m, host_mfree_cb f)
{
        struct WorldState *state = NULL;

        state = m(sizeof(*state));
        state->m = m;
        state->f = f;
        state->clients = world_client_state_new(m, f);

        return state;
}

struct Client *world_state_find_client(struct WorldState *world, int client_id)
{
        assert(world);
        return world_client_state_find(world->clients, client_id);
}

void world_state_spawn_client(struct WorldState *world, struct Client *client)
{
        assert(world);
        assert(client);

        world_client_state_spawn_client(world->clients, client);
}

void world_state_unspawn_client(struct WorldState *world, struct Client *client)
{
        assert(world);
        assert(client);

        world_client_state_unspawn_client(world->clients, client);
}

void world_state_client_info(struct WorldState *world, struct Client *client)
{
        assert(world);
        assert(client);

        world_client_state_info(world->clients, client);
}

void world_state_client_validate_location(struct WorldState *world, struct Client *client, struct Vec3 *location, int heading)
{
        assert(world);
        assert(client);
        assert(location);

        world_client_state_validate_location(world->clients, client, location, heading);
}

void world_state_client_move(struct WorldState *world, struct Client *client, struct Vec3 *prev_location, struct Vec3 *new_location)
{
        assert(world);
        assert(client);
        assert(prev_location);
        assert(new_location);

        world_client_state_move(world->clients, client, prev_location, new_location);
}

void world_state_client_say(struct WorldState *world, struct Client *client, char *buf, size_t buf_size)
{
        assert(world);
        assert(client);
        assert(buf);
        assert(buf_size > 0);

        world_client_state_say(world->clients, client, buf, buf_size);
}
