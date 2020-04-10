#ifndef L2AUTH_LOGIN_HANDLER_REQUEST_SERVER_LIST_H
#define L2AUTH_LOGIN_HANDLER_REQUEST_SERVER_LIST_H

#include <core/l2_client.h>
#include <login/server.h>

void login_handler_request_server_list
(
        struct LoginServer* server,
        struct L2Client* client
);

#endif
