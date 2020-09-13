#ifndef WORLD_H
#define WORLD_H

#include "../host.h"
#include "../dto/character.h"
#include "../dto/npc.h"
#include "../dto/pc.h"

struct World;

struct World *world_new(host_malloc_cb m, host_mfree_cb f);

void world_update_player(struct World *world, struct Pc *player);

struct Character *world_get_character(struct World *world, int id);

struct Pc *world_get_player(struct World *world, int id);

void world_spawn_npc(struct World *world, struct Npc *npc);

#endif
