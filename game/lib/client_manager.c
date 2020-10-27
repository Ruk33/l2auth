#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <log/log.h>
#include <data_structure/list.h>
#include <core/l2_raw_packet.h>
#include <core/l2_packet.h>
#include "host.h"
#include "client.h"
#include "entity/player.h"
#include "client_manager.h"

struct ClientManager
{
        host_malloc_cb m;
        host_mfree_cb f;
        struct List *clients;
};

struct ClientManager *client_manager_new(host_malloc_cb m, host_mfree_cb f)
{
        struct ClientManager *manager = NULL;

        manager = m(sizeof(*manager));
        manager->m = m;
        manager->f = f;
        manager->clients = list_new(m, f);

        return manager;
}

void client_manager_client_connects(struct ClientManager *manager, struct Client *client)
{
        assert(manager);
        assert(client);
        list_add(manager->clients, client);
}

void client_manager_spawn_char_in_world(struct ClientManager *manager, struct Client *to_be_spawned)
{
        assert(manager);
        assert(to_be_spawned);

        struct ListEntry *iterator = NULL;
        struct Client *client = NULL;

        iterator = list_first(manager->clients);

        client_enter_game(to_be_spawned);

        while (iterator)
        {
                client = list_entry_value(iterator);

                if (client_id(client) != client_id(to_be_spawned))
                {
                        player_send_info(client, to_be_spawned);
                }

                iterator = list_entry_next(iterator);
        }

        // Display to others
        client_manager_display_active_char(manager, to_be_spawned);
}

void client_manager_disconnect_client(struct ClientManager *manager, struct Client *client)
{
        assert(manager);
        assert(client);

        // broadcast un spawn packet to all clients
        client_handle_disconnect(client);

        list_remove(manager->clients, client);
}

void client_manager_display_active_char(struct ClientManager *manager, struct Client *from)
{
        assert(manager);
        assert(from);

        struct ListEntry *iterator = NULL;
        struct Client *client = NULL;

        iterator = list_first(manager->clients);

        while (iterator)
        {
                client = list_entry_value(iterator);

                if (client_id(client) != client_id(from))
                {
                        player_send_info(from, client);
                }

                iterator = list_entry_next(iterator);
        }
}

void client_manager_move_active_char(struct ClientManager *manager, struct Client *from, struct Vec3 *prev_location, struct Vec3 *new_location)
{
        assert(manager);
        assert(from);
        assert(prev_location);
        assert(new_location);

        struct ListEntry *iterator = NULL;
        struct Client *client = NULL;

        iterator = list_first(manager->clients);

        while (iterator)
        {
                client = list_entry_value(iterator);
                player_move_and_notify(from, client, prev_location, new_location);

                iterator = list_entry_next(iterator);
        }
}

void client_manager_active_char_talk(struct ClientManager *manager, struct Client *from, char *buf, size_t buf_size)
{
        assert(manager);
        assert(from);
        assert(buf);
        assert(buf_size > 0);

        struct ListEntry *iterator = NULL;
        struct Client *client = NULL;

        iterator = list_first(manager->clients);

        while (iterator)
        {
                client = list_entry_value(iterator);
                player_say(from, client, buf, buf_size);
                iterator = list_entry_next(iterator);
        }
}

struct ListEntry *client_manager_get_clients(struct ClientManager *manager)
{
        assert(manager);
        return list_first(manager->clients);
}

struct Client *client_manager_client_by_id(struct ClientManager *manager, int id)
{
        assert(manager);

        struct ListEntry *iterator = NULL;
        struct Client *client = NULL;

        iterator = list_first(manager->clients);

        while (iterator)
        {
                client = list_entry_value(iterator);

                if (client_id(client) == id)
                {
                        return client;
                }

                iterator = list_entry_next(iterator);
        }

        return NULL;
}
