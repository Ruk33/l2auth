#ifndef L2AUTH_LOGIN_HANDLER_REQUEST_LOGIN_SERVER_H
#define L2AUTH_LOGIN_HANDLER_REQUEST_LOGIN_SERVER_H

#include <core/l2_packet.h>
#include <login/dto/session_key.h>

l2_packet* login_handler_request_login_server
(
        struct LoginDtoSessionKey *session_key
);

#endif
