#ifndef SERVER_H
#define SERVER_H

#include "../../../include/util.h"
#include "session.h"

struct state {
	struct session sessions[32];
};

struct session *server_create_connection(struct state *state);

void server_on_disconnect(struct state *state, struct session *src);

void server_on_request(struct state *state, struct session *session);

#endif