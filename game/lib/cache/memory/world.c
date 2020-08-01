#include <string.h>
#include <log/log.h>
#include "../../dto/character.h"
#include "../../dto/npc.h"
#include "../../dto/pc.h"
#include "../world.h"

struct World {
        struct Character npc;
        struct Pc player;
        struct L2Server *l2_server;
};

struct World *world_new(struct L2Server *l2_server)
{
        struct World *world = l2_server->alloc(sizeof(struct World));

        world->l2_server = l2_server;

        return world;
}

void world_update_player(struct World *world, int id, struct Pc *player)
{
        log_info("Updating player stored with id %d", id);
        memcpy(&world->player, player, sizeof(struct Pc));
}

struct Character *world_get_character(struct World *world, int id)
{
        struct Character *character = world->l2_server->alloc(sizeof(struct Character));

        log_info("Getting character stored with id %d", id);
        memcpy(character, &world->npc, sizeof(struct Character));

        return character;
}

struct Pc *world_get_player(struct World *world, int id)
{
        struct Pc *player = world->l2_server->alloc(sizeof(struct Pc));

        log_info("Getting player stored with id %d", id);
        memcpy(player, &world->player, sizeof(struct Pc));

        return player;
}

void world_spawn_npc(struct World *world, struct Npc *npc)
{
        log_info("Spawning npc with id %d", npc->character.id);
        memcpy(&world->npc, &npc->character, sizeof(struct Character));
}
