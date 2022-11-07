#ifndef GAME_RESPONSE_H
#define GAME_RESPONSE_H

#include "game_server.h"

struct protocol {
    byte buf[9];
};

struct response_protocol {
    struct protocol protocol;
};

struct response_auth_login {
    u32 count;
    // TODO: confirm how many characters a user can have in the lobby.
    // struct character characters[10];
};

struct response_show_creation_screen {
    u32 count;
};

void response_protocol_encode(struct packet *dest, struct response_protocol *src);
void response_auth_login_encode(struct packet *dest, struct response_auth_login *src);
void response_show_creation_screen_encode(struct packet *dest, struct response_show_creation_screen *src);

#endif
