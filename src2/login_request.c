#include "include/login_server.h"

void request_auth_login_decode(struct request_auth_login *dest, struct packet *src)
{
    assert(dest);
    assert(src);

    byte *content = packet_body(src) + 1;
    byte *username = content + 0x62;
    byte *password = content + 0x70;

    str_cpy(dest->username.buf, username);
    str_cpy(dest->password.buf, password);
}
