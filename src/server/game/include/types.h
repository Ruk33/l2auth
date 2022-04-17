#ifndef TYPES_H
#define TYPES_H

#include "l2_string.h"

struct username {
    l2_string buf[28];
};

struct char_name {
    // Check if this limit is ok.
    l2_string buf[32];
};

struct char_title {
    // Check if this limit is ok.
    l2_string buf[28];
};

struct char_message {
    // Double check max message len sent from a char.
    l2_string buf[256];
};

struct npc_window_message {
    // 4k limit.
    l2_string buf[2048];
};

struct char_pos {
    i32 x;
    i32 y;
    i32 z;
    i32 heading;
};

struct char_attr {
    i32 _int;
    i32 str;
    i32 con;
    i32 men;
    i32 dex;
    i32 wit;
};

#endif