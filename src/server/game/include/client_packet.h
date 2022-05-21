#ifndef CLIENT_PACKET_H
#define CLIENT_PACKET_H

#include "../../../include/util.h"
#include "../../../include/packet.h"
#include "l2_string.h"
#include "types.h"

struct client_packet_action {
    u32 target_id;
    struct char_pos origin_pos;
    u8 action; // 0 = click, 1 = shift click
};

struct client_packet_attack {
    u32 target_id;
    struct char_pos origin_pos;
    u8 action; // 0 = click, 1 = shift click
};

struct packet_auth_request {
    struct username username;
    i32 playOK1;
    i32 playOK2;
    i32 loginOK1;
    i32 loginOK2;
};

struct packet_char_select_request {
    u32 index;
};

struct packet_create_char_request {
    struct char_name name;
    u32 race_id;
    u32 sex;
    u32 class_id;
    struct char_attr attrs;
    u32 hair_style;
    u32 hair_color;
    u32 face;
};

struct packet_move_request {
    struct char_pos position;
};

enum packet_revive_request_option
{
    REVIVE_IN_CLAN_HALL = 1,
    REVIVE_IN_CASTLE    = 2,
    REVIVE_IN_SIEGE_HQ  = 3,
    REVIVE_FIXED        = 4,
};

struct packet_revive_request {
    enum packet_revive_request_option option_chosen;
};

struct packet_validate_pos_request {
    struct char_pos position;
};

struct packet_say_request {
    // Check maximum length.
    l2_string message[256];
    size_t size;
};

struct packet_bypass_request {
    // Check maximum size.
    l2_string command[128];
    size_t size;
};

struct packet_skill_use_request {
    u32 skill_id;
    u32 ctrl_pressed;
    u8 shift_pressed;
};

void client_packet_action_request_decode(struct client_packet_action *dest, struct packet *src);

void client_packet_attack_request_decode(struct client_packet_attack *dest, struct packet *src);

void client_packet_auth_request_decode(struct packet_auth_request *dest, struct packet *src);

void client_packet_char_select_request_decode(struct packet_char_select_request *dest, struct packet *src);

void client_packet_create_char_request_decode(struct packet_create_char_request *dest, struct packet *src);

void client_packet_move_request_decode(struct packet_move_request *dest, struct packet *src);

void client_packet_revive_request_decode(struct packet_revive_request *dest, struct packet *src);

void client_packet_validate_pos_request_decode(struct packet_validate_pos_request *dest, struct packet *src);

void client_packet_say_request_decode(struct packet_say_request *dest, struct packet *src);

void client_packet_bypass_request_decode(struct packet_bypass_request *dest, struct packet *src);

void client_packet_skill_use_request_decode(struct packet_skill_use_request *dest, struct packet *src);

#endif
