#ifndef PLAYER_ENTITY_H
#define PLAYER_ENTITY_H

#include <stdlib.h>
#include "../client.h"
#include "../dto/character.h"
#include "../request_handler/create_char/request.h"

void player_create(struct Client *client, struct CreateCharRequest *request);

void player_entity_action(struct Client *client, struct Character *target);

void player_say(struct Client *from, struct Client *to, char *msg, size_t msg_len);

void player_unsafe_say(struct Client *client, const char *msg);

#endif
