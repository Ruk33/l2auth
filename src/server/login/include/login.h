#ifndef LOGIN_H
#define LOGIN_H

#include "../../../include/util.h"
#include "state.h"

struct client *login_on_new_connection(struct state *state);

void login_on_disconnect(struct state *state, struct client *src);

void login_on_request(struct state *state, struct client *client);

#endif