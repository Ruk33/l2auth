#ifndef L2AUTH_LOGIN_HANDLER_REQUEST_AUTH_LOGIN_H
#define L2AUTH_LOGIN_HANDLER_REQUEST_AUTH_LOGIN_H

#include <core/l2_packet.h>
#include <login/dto/session_key.h>

l2_packet* login_handler_request_auth_login
(
        unsigned char* packet,
        struct LoginDtoSessionKey* session_key
);

#endif
