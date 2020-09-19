#ifndef STORAGE_CLIENT_H
#define STORAGE_CLIENT_H

#include "conn.h"
#include "../client.h"
#include "../entity/player.h"

struct Player **storage_characters_all
(struct Client *client);

struct Player *storage_character_get
(struct Client *client, int index);

void storage_character_save
(struct Client *client, struct Player *character);

#endif
