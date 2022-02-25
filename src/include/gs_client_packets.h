#ifndef INCLUDE_GS_CLIENT_PACKETS_H
#define INCLUDE_GS_CLIENT_PACKETS_H

#include "platform.h"
#include "packet.h"
#include "l2_string.h"


struct gs_packet_action_request {
    u32_t target_id;
    i32_t origin_x;
    i32_t origin_y;
    i32_t origin_z;
    u8_t action; // 0 = click, 1 = shift click
};

struct gs_packet_attack_request {
    u32_t target_id;
    i32_t origin_x;
    i32_t origin_y;
    i32_t origin_z;
    u8_t action; // 0 = click, 1 = shift click
};

struct gs_packet_auth_request {
    l2_string_t username[28];
    i32_t playOK1;
    i32_t playOK2;
    i32_t loginOK1;
    i32_t loginOK2;
};

struct gs_packet_char_select_request {
    u32_t index;
};

struct gs_packet_create_char_request {
    l2_string_t name[32];
    u32_t race;
    u32_t sex;
    u32_t _class;
    i32_t _int;
    i32_t str;
    i32_t con;
    i32_t men;
    i32_t dex;
    i32_t wit;
    u32_t hair_style;
    u32_t hair_color;
    u32_t face;
};

struct gs_packet_move_request {
    i32_t x;
    i32_t y;
    i32_t z;
};

enum gs_packet_revive_request_option
{
    REVIVE_IN_CLAN_HALL = 1,
    REVIVE_IN_CASTLE    = 2,
    REVIVE_IN_SIEGE_HQ  = 3,
    REVIVE_FIXED        = 4,
};

struct gs_packet_revive_request {
    enum gs_packet_revive_request_option option_chosen;
};

struct gs_packet_validate_pos_request {
    i32_t x;
    i32_t y;
    i32_t z;
    i32_t heading;
};

struct gs_packet_say_request {
    l2_string_t *message;
    size_t size;
};

struct gs_packet_bypass_request {
    l2_string_t *command;
    size_t size;
};

struct gs_packet_skill_use_request {
    u32_t skill_id;
    u32_t ctrl_pressed;
    u8_t shift_pressed;
};

void gs_packet_action_request_unpack(struct gs_packet_action_request *dest,
                                     packet_t *src);

void gs_packet_attack_request_unpack(struct gs_packet_attack_request *dest,
                                     packet_t *src);

void gs_packet_auth_request_unpack(struct gs_packet_auth_request *dest,
                                   packet_t *src);

void gs_packet_char_select_request_unpack(
    struct gs_packet_char_select_request *dest,
    packet_t *src);

void gs_packet_create_char_request_unpack(
    struct gs_packet_create_char_request *dest,
    packet_t *src);

void gs_packet_move_request_unpack(struct gs_packet_move_request *dest,
                                   packet_t *src);

void gs_packet_revive_request_unpack(struct gs_packet_revive_request *dest,
                                     packet_t *src);

void gs_packet_validate_pos_request_unpack(
    struct gs_packet_validate_pos_request *dest,
    packet_t *src);

void gs_packet_say_request_unpack(struct gs_packet_say_request *dest,
                                  packet_t *src);

void gs_packet_bypass_request_unpack(struct gs_packet_bypass_request *dest,
                                     packet_t *src);

void gs_packet_use_skill_request_unpack(struct gs_packet_skill_use_request *dest,
                                        packet_t *src);

#endif