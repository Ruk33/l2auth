#include <assert.h>
#include "../../include/util.h"
#include "../../include/packet.h"
#include "include/client_packet.h"

void packet_auth_login_from(struct packet_auth_login *dest, struct packet *src)
{
    byte *content = 0;
    byte *username = 0;
    byte *password = 0;

    assert(dest);
    assert(src);

    content = packet_body(src) + 1;
    username = content + 0x62;
    password = content + 0x70;

    cpy_bytes(&BFAE(dest->username.buf), &B(username, 65535 - 3, 65535 - 5), sizeof(dest->username.buf));
    cpy_bytes(&BFAE(dest->password.buf), &B(password, 65535 - 3, 65535 - 5), sizeof(dest->password.buf));
}