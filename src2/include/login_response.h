#ifndef LOGIN_SERVER_RESPONSE_H
#define LOGIN_SERVER_RESPONSE_H

#include "l2auth.h"

struct session_id {
    byte buf[4];
};

struct protocol {
    byte buf[4];
};

struct response_init {
    struct session_id session_id;
    struct protocol protocol;
    struct rsa_modulus modulus;
};

struct response_auth_login_ok {
    u32 login_ok1;
    u32 login_ok2;
};

struct response_gg_auth {
    u32 gg_response;
};

void response_init_encode(struct packet *dest, struct response_init *src);
void response_auth_login_ok_encode(struct packet *dest, struct response_auth_login_ok *src);
void response_gg_auth_encode(struct packet *dest, struct response_gg_auth *src);

#endif
