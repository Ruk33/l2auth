#ifndef CHARACTER_ENTITY_H
#define CHARACTER_ENTITY_H

#include "../dto/character.h"
#include "../dto/vec3.h"

void character_entity_run_to(struct Character *character, struct Vec3 *location);

void character_entity_walk_to(struct Character *character, struct Vec3 *location);

#endif
