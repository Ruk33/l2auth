#include <assert.h>
#include <string.h>
#include <log/log.h>
#include <data_structure/hashtable.h>
#include "../../host.h"
#include "../../dto/character.h"
#include "../../dto/npc.h"
#include "../../dto/pc.h"
#include "../world.h"

struct World {
        struct Hashtable *players;
        struct Hashtable *npcs;
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
        world->players = hashtable_new(m, f);
        world->npcs = hashtable_new(m, f);

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
        prev_player = hashtable_get(world->players, id);
        new_player = world->memory_alloc(sizeof(*new_player));

        memcpy(new_player, player, sizeof(*new_player));
        log_info("Updating player stored with id %d", new_player->character.id);

        hashtable_remove(world->players, id);
        hashtable_put(world->players, id, new_player);

        if (prev_player) world->memory_free(prev_player);
}

struct Character *world_get_character(struct World *world, int id)
{
        assert(world);

        struct Character *character = NULL;
        struct Character *character_copy = NULL;

        character = hashtable_get(world->npcs, id);
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

        player = hashtable_get(world->players, id);
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

        prev_character = hashtable_get(world->npcs, npc->character.id);
        new_character = world->memory_alloc(sizeof(*new_character));

        log_info("Spawning npc with id %d", npc->character.id);

        memcpy(new_character, &npc->character, sizeof(*new_character));

        hashtable_remove(world->npcs, new_character->id);
        hashtable_put(world->npcs, new_character->id, new_character);

        if (prev_character) world->memory_free(prev_character);
}
