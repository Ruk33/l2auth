#ifndef STATE_H
#define STATE_H

#include "character.h"
#include "client.h"

struct state {
    struct client clients[1024];
    size_t recycled_clients[1024];
    size_t client_count;
    struct character characters[1024];
    size_t recycled_characters[1024];
    size_t character_count;
};

struct client *state_get_free_client(struct state *src);

void state_add_player(struct state *state, struct client *client, struct character *src);

struct character *state_add_character(struct state *state, struct character *src);

void state_release_client(struct state *state, struct client *src);

#endif