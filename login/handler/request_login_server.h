#ifndef L2AUTH_LOGIN_HANDLER_REQUEST_LOGIN_SERVER_H
#define L2AUTH_LOGIN_HANDLER_REQUEST_LOGIN_SERVER_H

#include <core/l2_server.h>
#include <core/l2_client.h>

void login_handler_request_login_server
(
        struct L2Server* server,
        struct L2Client* client
);

#endif
