#ifndef WORLD_H
#define WORLD_H

#include "../../shared/l2_server.h"
#include "../dto/character.h"
#include "../dto/npc.h"
#include "../dto/pc.h"

struct World;

struct World *world_new(struct L2Server *server);

void world_update_player(struct World *world, int id, struct Pc *player);

struct Character *world_get_character(struct World *world, int id);

struct Pc *world_get_player(struct World *world, int id);

void world_spawn_npc(struct World *world, struct Npc *npc);

#endif
