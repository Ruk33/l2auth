#ifndef GAME_SERVER_H
#define GAME_SERVER_H

#include "l2auth.h"
#include "game_session.h"
#include "game_server.h"
#include "game_request.h"
#include "game_response.h"

struct game_state {
    struct game_session sessions[MAX_CONNECTIONS];
};

struct game_session *game_server_new_conn(struct game_state *state);
void game_server_request(struct game_state *state, struct game_session *session, void *buf, size_t n);
void game_server_disconnect(struct game_state *state, struct game_session *session);

#endif
