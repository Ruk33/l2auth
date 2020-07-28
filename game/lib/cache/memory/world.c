#include <string.h>
#include <log/log.h>
#include "../../entity/pc.h"
#include "../world.h"

struct World {
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

struct Pc *world_get_player(struct World *world, int id)
{
        struct Pc *player = world->l2_server->alloc(sizeof(struct Pc));

        log_info("Getting player stored with id %d", id);
        memcpy(player, &world->player, sizeof(struct Pc));

        return player;
}
