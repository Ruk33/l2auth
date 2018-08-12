#ifndef L2AUTH_SERVER_PACKET_LOGIN_FAIL_C
#define L2AUTH_SERVER_PACKET_LOGIN_FAIL_C

#include <core/l2_packet.c>

enum login_fail_reason
{
  LOGIN_FAIL_REASON_USER_OR_PASSWORD_WRONG = 0x03
};

void server_packet_login_fail(struct l2_packet* packet, enum login_fail_reason reason)
{
    char packet_id = 0x01;

    packet_init(packet, packet_id);
    packet_write(packet, (char*) &reason, sizeof(reason));
}

#endif //L2AUTH_SERVER_PACKET_LOGIN_FAIL_C