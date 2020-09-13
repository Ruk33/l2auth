#include <assert.h>
#include <string.h>
#include <glib.h>
#include <log/log.h>
#include "../../host.h"
#include "../../dto/character.h"
#include "../../dto/npc.h"
#include "../../dto/pc.h"
#include "../world.h"

struct World {
        GHashTable *players;
        GHashTable *npcs;
        host_malloc_cb memory_alloc;
        host_mfree_cb memory_free;
};

struct World *world_new
(host_malloc_cb m, host_mfree_cb f)
{
        struct World *world = NULL;

        world = m(sizeof(*world));
        world->memory_alloc = m;
        world->memory_free = f;
        world->players = g_hash_table_new(g_int_hash, g_int_equal);
        world->npcs = g_hash_table_new(g_int_hash, g_int_equal);

        return world;
}

void world_update_player(struct World *world, struct Pc *player)
{
        assert(world);
        assert(player);

        int id = 0;
        struct Pc *prev_player = NULL;
        struct Pc *new_player = NULL;

        id = player->character.id;
        prev_player = g_hash_table_lookup(world->players, &id);
        new_player = world->memory_alloc(sizeof(*new_player));

        memcpy(new_player, player, sizeof(*new_player));
        log_info("Updating player stored with id %d", new_player->character.id);

        g_hash_table_replace(
                world->players,
                &new_player->character.id,
                new_player
        );

        if (prev_player) world->memory_free(prev_player);
}

struct Character *world_get_character(struct World *world, int id)
{
        assert(world);

        struct Character *character = NULL;
        struct Character *character_copy = NULL;

        character = g_hash_table_lookup(world->npcs, &id);
        character_copy = world->memory_alloc(sizeof(*character_copy));

        log_info("Getting character stored with id %d", id);
        memcpy(character_copy, character, sizeof(*character_copy));

        return character_copy;
}

struct Pc *world_get_player(struct World *world, int id)
{
        assert(world);

        struct Pc *player = NULL;
        struct Pc *player_copy = NULL;

        player = g_hash_table_lookup(world->players, &id);
        player_copy = world->memory_alloc(sizeof(*player_copy));

        memcpy(player_copy, player, sizeof(*player_copy));
        log_info("Getting player stored with id %d", player_copy->character.id);

        return player_copy;
}

void world_spawn_npc(struct World *world, struct Npc *npc)
{
        assert(world);
        assert(npc);

        struct Character *prev_character = NULL;
        struct Character *new_character = NULL;

        prev_character = g_hash_table_lookup(world->npcs, &npc->character.id);
        new_character = world->memory_alloc(sizeof(*new_character));

        log_info("Spawning npc with id %d", npc->character.id);

        memcpy(new_character, &npc->character, sizeof(*new_character));
        g_hash_table_replace(world->npcs, &new_character->id, new_character);

        if (prev_character) world->memory_free(prev_character);
}
