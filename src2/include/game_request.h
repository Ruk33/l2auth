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

struct request_create_character {
    struct l2_character_name name;
    u32 race_id;
    u32 sex;
    u32 class_id;
    u32 hair_style_id;
    u32 hair_color_id;
    u32 face_id;
};

struct request_selected_character {
    u32 index;
};

void request_auth_decode(struct request_auth *dest, struct packet *src);
void request_create_character_decode(struct request_create_character *dest, struct packet *src);
void request_selected_character_decode(struct request_selected_character *dest, struct packet *src);

#endif
