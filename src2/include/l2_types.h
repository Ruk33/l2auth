#ifndef L2_TYPES_H
#define L2_TYPES_H

#include "l2auth.h"

struct username {
    char buf[32];
};

struct password {
    char buf[32];
};

struct encrypted_password {
    byte buf[256];
};

struct server {
    u8 id;
    u32 ip;
    u32 port;
    u8 age_limit;
    u8 pvp;
    u16 players;
    u16 max_players;
    u8 status;
    u32 extra;
    u8 brackets;
};

// struct character {
// };

struct account {
    struct username username;
    struct encrypted_password password;
    // struct character characters[7];
};

#endif
