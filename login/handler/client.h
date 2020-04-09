#ifndef L2AUTH_LOGIN_HANDLER_CLIENT_H
#define L2AUTH_LOGIN_HANDLER_CLIENT_H

#include <core/l2_client.h>
#include <login/server.h>

void login_handler_client(struct LoginServer* server, struct L2Client* client);

#endif
