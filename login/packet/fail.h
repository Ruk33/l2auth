#ifndef L2AUTH_LOGIN_PACKET_FAIL_H
#define L2AUTH_LOGIN_PACKET_FAIL_H

#include <core/l2_packet.h>

enum login_packet_fail_reason
{
        LOGIN_PACKET_FAIL_REASON_USER_OR_PASSWORD_WRONG = 0x03
};
l2_packet* login_packet_fail(enum login_packet_fail_reason reason);

#endif
