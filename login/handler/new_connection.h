#ifndef L2AUTH_LOGIN_HANDLER_NEW_CONNECTION_H
#define L2AUTH_LOGIN_HANDLER_NEW_CONNECTION_H

#include <login/server.h>
#include <login/client.h>

void login_handler_new_connection
(
        struct LoginServer* server,
        struct LoginClient* client
);

#endif
