#ifndef L2AUTH_LOGIN_HANDLER_REQUEST_LOGIN_SERVER_H
#define L2AUTH_LOGIN_HANDLER_REQUEST_LOGIN_SERVER_H

#include <core/l2_client.h>
#include <login/server.h>

void login_handler_request_login_server
(
        struct LoginServer* server,
        struct L2Client* client
);

#endif
