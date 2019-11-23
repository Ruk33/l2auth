#ifndef L2AUTH_LOGIN_HANDLER_REQUEST_AUTH_LOGIN_C
#define L2AUTH_LOGIN_HANDLER_REQUEST_AUTH_LOGIN_C

#include <log/log.h>
#include <core/l2_packet.c>
#include <packet/helper.c>
#include <login/helper.c>
#include <login/packet/ok.c>
#include <login/session_key.c>
//#include <login/packet/fail.c>

l2_packet* login_handler_request_auth_login
(
        unsigned char* packet,
        struct LoginSessionKey* session_key
)
{
        unsigned char username[14];
        unsigned char password[16];

        login_helper_username_from_packet(packet, username);
        login_helper_password_from_packet(packet, password);

        log_info("Username %s", username);
        log_info("Password %s", password);

        return login_packet_ok(session_key);
        //return login_packet_fail(LOGIN_PACKET_FAIL_REASON_USER_OR_PASSWORD_WRONG);
}

#endif
