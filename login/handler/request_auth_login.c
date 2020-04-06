#ifndef L2AUTH_LOGIN_HANDLER_REQUEST_AUTH_LOGIN_C
#define L2AUTH_LOGIN_HANDLER_REQUEST_AUTH_LOGIN_C

#include <log/log.h>
#include <core/l2_packet.h>
#include <packet/helper.h>
#include <login/helper.h>
#include <login/packet/ok.h>
#include <login/dto/session_key.h>
#include <login/packet/fail.h>
#include <login/handler/request_auth_login.h>

l2_packet* login_handler_request_auth_login
(
        unsigned char* packet,
        struct LoginDtoSessionKey* session_key
)
{
        unsigned char username[14];
        unsigned char password[16];

        if (!packet) {
                log_fatal("Trying to handle auth login request but no packet was passed");
                return login_packet_fail(LOGIN_PACKET_FAIL_REASON_USER_OR_PASSWORD_WRONG);
        }

        if (!session_key) {
                log_fatal("Trying to handle auth login request but no session key was passed");
                return login_packet_fail(LOGIN_PACKET_FAIL_REASON_USER_OR_PASSWORD_WRONG);
        }

        login_helper_username_from_packet(packet, username);
        login_helper_password_from_packet(packet, password);

        log_info("Username %s", username);
        log_info("Password %s", password);

        return login_packet_ok(session_key);
}

#endif
