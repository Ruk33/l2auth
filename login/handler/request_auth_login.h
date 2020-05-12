#ifndef L2AUTH_LOGIN_HANDLER_REQUEST_AUTH_LOGIN_H
#define L2AUTH_LOGIN_HANDLER_REQUEST_AUTH_LOGIN_H

#include <login/server.h>
#include <login/client.h>
#include <core/l2_packet.h>
#include <core/session_key.h>

void login_handler_request_auth_login
(
        struct LoginServer* server,
        struct LoginClient* client,
        l2_packet* packet
);

#endif
