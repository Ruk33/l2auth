#ifndef LOGIN_SERVER_H
#define LOGIN_SERVER_H

#include "l2auth.h"

struct state {
    struct login_session sessions[MAX_CONNECTIONS];
};

struct login_session *login_server_new_conn(struct state *state);
void login_server_request(struct state *state, struct login_session *session, void *buf, size_t n);
void login_server_disconnect(struct state *state, struct login_session *session);

#endif
