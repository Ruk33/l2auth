#ifndef LOGIN_SERVER_TYPES_H
#define LOGIN_SERVER_TYPES_H

#include "../../../include/util.h"

struct username {
    i8 buf[14];
};

struct password {
    i8 buf[16];
};

#endif