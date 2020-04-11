#ifndef L2AUTH_LOGIN_HANDLER_REQUEST_AUTH_LOGIN_H
#define L2AUTH_LOGIN_HANDLER_REQUEST_AUTH_LOGIN_H

#include <core/l2_server.h>
#include <core/l2_client.h>
#include <core/l2_packet.h>
#include <login/dto/session_key.h>

void login_handler_request_auth_login
(
        struct L2Server* server,
        struct L2Client* client,
        l2_packet* packet
);

#endif
