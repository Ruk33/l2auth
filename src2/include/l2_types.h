#ifndef L2_TYPES_H
#define L2_TYPES_H

#include "l2auth.h"

typedef u16 l2_string;

struct username {
    char buf[32];
};

struct l2_username {
    l2_string buf[16];
};

struct l2_character_name {
    // check if this limit is ok.
    l2_string buf[16];
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
