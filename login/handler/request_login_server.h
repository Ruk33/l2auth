#ifndef L2AUTH_LOGIN_HANDLER_REQUEST_LOGIN_SERVER_H
#define L2AUTH_LOGIN_HANDLER_REQUEST_LOGIN_SERVER_H

#include <login/server.h>
#include <login/client.h>

void login_handler_request_login_server
(
        struct LoginServer* server,
        struct LoginClient* client
);

#endif
