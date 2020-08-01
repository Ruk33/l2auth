#include <string.h>
#include <glib.h>
#include <log/log.h>
#include "../../dto/character.h"
#include "../../dto/npc.h"
#include "../../dto/pc.h"
#include "../world.h"

struct World {
        GHashTable *players;
        GHashTable *npcs;
        struct L2Server *l2_server;
};

static guint int_hash(gconstpointer v)
{
        return (guint) *(const int*) v;
}

static gboolean int_equal(gconstpointer v1, gconstpointer v2)
{
        return *((const int*) v1) == *((const int*) v2);
}

struct World *world_new(struct L2Server *l2_server)
{
        struct World *world = l2_server->alloc(sizeof(struct World));

        world->l2_server = l2_server;
        world->players = g_hash_table_new(int_hash, int_equal);
        world->npcs = g_hash_table_new(int_hash, int_equal);

        return world;
}

void world_update_player(struct World *world, struct Pc *player)
{
        int id = player->character.id;
        struct Pc *prev_player = g_hash_table_lookup(world->players, &id);
        struct Pc *new_player = world->l2_server->alloc(sizeof(struct Pc));

        log_info("Updating player stored with id %d", id);

        memcpy(new_player, player, sizeof(struct Pc));

        g_hash_table_replace(
                world->players,
                &new_player->character.id,
                new_player
        );

        if (prev_player) world->l2_server->free(prev_player);
}

struct Character *world_get_character(struct World *world, int id)
{
        struct Character *character = g_hash_table_lookup(world->npcs, &id);
        struct Character *character_copy = world->l2_server->alloc(
                sizeof(struct Character)
        );

        log_info("Getting character stored with id %d", id);
        memcpy(character_copy, character, sizeof(struct Character));

        return character_copy;
}

struct Pc *world_get_player(struct World *world, int id)
{
        struct Pc *player = g_hash_table_lookup(world->players, &id);
        struct Pc *player_copy = world->l2_server->alloc(sizeof(struct Pc));

        log_info("Getting player stored with id %d", id);
        memcpy(player_copy, player, sizeof(struct Pc));

        return player_copy;
}

void world_spawn_npc(struct World *world, struct Npc *npc)
{
        struct Character *prev_character = g_hash_table_lookup(
                world->npcs,
                &npc->character.id
        );
        struct Character *new_character = world->l2_server->alloc(
                sizeof(struct Character)
        );

        log_info("Spawning npc with id %d", npc->character.id);

        memcpy(new_character, &npc->character, sizeof(struct Character));
        g_hash_table_replace(world->npcs, &new_character->id, new_character);

        if (prev_character) world->l2_server->free(prev_character);
}
