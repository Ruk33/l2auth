#ifndef LOGIN_SERVER_REQUEST_H
#define LOGIN_SERVER_REQUEST_H

#include "login_server.h"

struct request_auth_login {
    struct username username;
    struct password password;
};

void request_auth_login_decode(struct request_auth_login *dest, struct packet *src);

#endif
