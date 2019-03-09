#ifndef L2AUTH_SERVER_PACKET_LOGIN_FAIL_C
#define L2AUTH_SERVER_PACKET_LOGIN_FAIL_C

#include <core/l2_packet.c>

enum login_fail_reason
{
        LOGIN_FAIL_REASON_USER_OR_PASSWORD_WRONG = 0x03
};

l2_packet* server_packet_login_fail(enum login_fail_reason reason)
{
        l2_packet_type type = 0x01;

        return l2_packet_new(
                type,
                (unsigned char*) &reason,
                sizeof(reason)
        );
}

#endif //L2AUTH_SERVER_PACKET_LOGIN_FAIL_C
