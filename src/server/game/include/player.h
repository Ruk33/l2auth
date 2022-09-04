#ifndef PLAYER_H
#define PLAYER_H

#include "types.h"

void handle_player_request(struct state *state, struct session *session);

void update_player(struct state *state, struct session *session, seconds delta);

#endif
