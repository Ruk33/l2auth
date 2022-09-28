#ifndef LOGIN_SERVER_REQUEST_H
#define LOGIN_SERVER_REQUEST_H

#include "l2auth.h"

struct username {
    char buf[32];
};

struct password {
    char buf[32];
};

struct request_auth_login {
    struct username username;
    struct password password;
};

void request_auth_login_decode(struct request_auth_login *dest, struct packet *src);

#endif
