#ifndef L2AUTH_LOGIN_HANDLER_NEW_CONNECTION_H
#define L2AUTH_LOGIN_HANDLER_NEW_CONNECTION_H

#include <core/l2_client.h>
#include <login/server.h>

void login_handler_new_connection
(
        struct LoginServer* server,
        struct L2Client* client
);

#endif
