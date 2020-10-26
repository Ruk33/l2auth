#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <data_structure/list.h>
#include <core/l2_raw_packet.h>
#include <core/l2_packet.h>
#include "host.h"
#include "client.h"
#include "entity/player.h"
#include "world_client_state.h"

struct WorldClientState
{
        host_malloc_cb m;
        host_mfree_cb f;
        struct List *clients;
};

// static void broadcast_packet(struct WorldClientState *state, l2_raw_packet *packet)
// {
//         assert(state);
//         assert(packet);

//         struct ListEntry *iterator = NULL;
//         struct Client *client = NULL;
//         l2_raw_packet *packet_copy = NULL;
//         l2_raw_packet_size packet_size = 0;
//         l2_raw_packet_size full_packet_size = 0;

//         iterator = list_first(state->clients);

//         packet_size = l2_raw_packet_get_size(packet);
//         full_packet_size = l2_raw_packet_calculate_size(packet_size);

//         while (iterator)
//         {
//                 client = list_entry_value(iterator);
//                 packet_copy = state->m(full_packet_size);

//                 l2_raw_packet_init(
//                     packet_copy,
//                     l2_raw_packet_content(packet),
//                     packet_size);

//                 client_encrypt_packet(client, packet_copy);
//                 client_queue_response(client, packet_copy);

//                 state->f(packet_copy);
//                 iterator = list_entry_next(iterator);
//         }
// }

struct WorldClientState *world_client_state_new(host_malloc_cb m, host_mfree_cb f)
{
        struct WorldClientState *state = NULL;

        state = m(sizeof(*state));
        state->m = m;
        state->f = f;
        state->clients = list_new(m, f);

        return state;
}

void world_client_state_spawn_client(struct WorldClientState *state, struct Client *client)
{
        assert(state);
        assert(client);

        // broadcast spawn packet to all clients

        list_add(state->clients, client);
}

void world_client_state_unspawn_client(struct WorldClientState *state, struct Client *client)
{
        assert(state);
        assert(client);

        // broadcast un spawn packet to all clients
        client_handle_disconnect(client);

        list_remove(state->clients, client);
}

void world_client_state_say(struct WorldClientState *state, struct Client *from, char *buf, size_t buf_size)
{
        assert(state);
        assert(from);
        assert(buf);
        assert(buf_size > 0);

        struct ListEntry *iterator = NULL;
        struct Client *client = NULL;

        iterator = list_first(state->clients);

        while (iterator)
        {
                client = list_entry_value(iterator);
                player_say(from, client, buf, buf_size);
                iterator = list_entry_next(iterator);
        }
}

struct ListEntry *world_client_state_get_all(struct WorldClientState *state)
{
        assert(state);
        return list_first(state->clients);
}

struct Client *world_client_state_find(struct WorldClientState *state, int id)
{
        assert(state);

        struct ListEntry *iterator = NULL;
        struct Client *client = NULL;

        iterator = list_first(state->clients);

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
