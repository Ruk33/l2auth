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

struct request_move {
    i32 destination_x;
    i32 destination_y;
    i32 destination_z;
    i32 origin_x;
    i32 origin_y;
    i32 origin_z;
};

struct request_validate_position {
    i32 x;
    i32 y;
    i32 z;
    i32 heading;
};

struct request_action {
    u32 obj_id;
    i32 origin_x;
    i32 origin_y;
    i32 origin_z;
    u8 action_id; // 0 simple click, 1 shift click.
};

void request_auth_decode(struct request_auth *dest, struct packet *src);
void request_create_character_decode(struct request_create_character *dest, struct packet *src);
void request_selected_character_decode(struct request_selected_character *dest, struct packet *src);
void request_move_decode(struct request_move *dest, struct packet *src);
void request_validate_position_decode(struct request_validate_position *dest, struct packet *src);
void request_action_decode(struct request_action *dest, struct packet *src);

#endif
