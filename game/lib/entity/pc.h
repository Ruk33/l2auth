#ifndef PC_ENTITY_H
#define PC_ENTITY_H

#include <stdlib.h>
#include "../client.h"
#include "../dto/character.h"

void player_entity_action
(struct Client *client, struct Character *target);

void player_say
(struct Client *client, char *msg, size_t msg_len);

#endif
