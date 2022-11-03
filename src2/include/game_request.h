#ifndef GAME_REQUEST_H
#define GAME_REQUEST_H

#include "game_server.h"

struct request_auth {
    struct l2_username username;
    u32 play_ok1;
    u32 play_ok2;
    u32 login_ok1;
    u32 login_ok2;
};

void request_auth_decode(struct request_auth *dest, struct packet *src);

#endif
