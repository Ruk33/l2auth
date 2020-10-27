#include <assert.h>
#include <log/log.h>
#include "host.h"
#include "client.h"
#include "client_manager.h"
#include "world_manager.h"

struct WorldManager
{
        struct ClientManager *clients;
        host_malloc_cb m;
        host_mfree_cb f;
};

void world_manager_tick(struct WorldManager *manager)
{
        assert(manager);
}

struct WorldManager *world_manager_new(host_malloc_cb m, host_mfree_cb f)
{
        struct WorldManager *manager = NULL;

        manager = m(sizeof(*manager));
        manager->m = m;
        manager->f = f;
        manager->clients = client_manager_new(m, f);

        return manager;
}

void world_manager_client_connects(struct WorldManager *manager, struct Client *client)
{
        assert(manager);
        assert(client);
        client_manager_client_connects(manager->clients, client);
}

struct Client *world_manager_find_client_by_id(struct WorldManager *manager, int client_id)
{
        assert(manager);
        return client_manager_client_by_id(manager->clients, client_id);
}

void world_manager_spawn_char_in_world(struct WorldManager *manager, struct Client *client)
{
        assert(manager);
        assert(client);

        client_manager_spawn_char_in_world(manager->clients, client);
}

void world_manager_disconnect_client(struct WorldManager *manager, struct Client *client)
{
        assert(manager);
        assert(client);

        client_manager_disconnect_client(manager->clients, client);
}

void world_manager_display_active_char(struct WorldManager *manager, struct Client *client)
{
        assert(manager);
        assert(client);

        client_manager_display_active_char(manager->clients, client);
}

void world_manager_move_active_char(struct WorldManager *manager, struct Client *client, struct Vec3 *prev_location, struct Vec3 *new_location)
{
        assert(manager);
        assert(client);
        assert(prev_location);
        assert(new_location);

        client_manager_move_active_char(
            manager->clients,
            client,
            prev_location,
            new_location);
}

void world_manager_active_char_talk(struct WorldManager *manager, struct Client *client, char *buf, size_t buf_size)
{
        assert(manager);
        assert(client);
        assert(buf);
        assert(buf_size > 0);

        client_manager_active_char_talk(
            manager->clients,
            client,
            buf,
            buf_size);
}
