#ifndef PC_ENTITY_H
#define PC_ENTITY_H

#include "../client.h"
#include "../dto/character.h"

void player_entity_action(struct Client *client, struct Character *target);

#endif
