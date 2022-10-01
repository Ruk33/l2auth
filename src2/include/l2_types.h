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

// struct character {
// };

struct account {
    struct username username;
    struct encrypted_password password;
    // struct character characters[7];
};

#endif