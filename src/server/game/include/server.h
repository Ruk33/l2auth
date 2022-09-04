#ifndef SERVER_H
#define SERVER_H

#include "../../../include/util.h"
#include "session.h"
#include "types.h"

struct session *server_create_connection(struct state *state);

void server_on_disconnect(struct state *state, struct session *src);

void server_on_request(struct state *state, struct session *src);

void server_update(struct state *state, seconds delta);

void broadcast_packet(struct state *state, struct packet *packet);

#endif
