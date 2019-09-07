#ifndef L2AUTH_LOGIN_PACKET_FAIL_C
#define L2AUTH_LOGIN_PACKET_FAIL_C

#include <core/l2_packet.c>

enum login_packet_fail_reason
{
        LOGIN_PACKET_FAIL_REASON_USER_OR_PASSWORD_WRONG = 0x03
};

l2_packet* login_packet_fail(enum login_packet_fail_reason reason)
{
        l2_packet_type type = 0x01;

        return l2_packet_new(
                type,
                (unsigned char*) &reason,
                sizeof(reason)
        );
}

#endif
