#ifndef INCLUDE_GS_PACKET_AUTH_LOGIN_H
#define INCLUDE_GS_PACKET_AUTH_LOGIN_H

#include "packet.h"
#include "l2_string.h"
#include "gs_character.h"

typedef struct {
        int empty[10];
        l2_string_t name[28];
        int id;
        // For some reason, name goes twice in the packet
        l2_string_t name_copy[28];
        int playOK1;
        int clan_id;
        int sex;
        int race_id;
        int class_id;
        int active;
        int x;
        int y;
        int z;
        double hp;
        double mp;
        int sp;
        int exp;
        int level;
        int karma;
        int under_obj_id;
        int r_ear_obj_id;
        int l_ear_obj_id;
        int neck_obj_id;
        int r_finger_obj_id;
        int l_finger_obj_id;
        int head_obj_id;
        int r_hand_obj_id;
        int l_hand_obj_id;
        int gloves_obj_id;
        int chest_obj_id;
        int legs_obj_id;
        int feet_obj_id;
        int back_obj_id;
        int lr_hand_obj_id;
        int hair_obj_id;
        int under;
        int r_ear;
        int l_ear;
        int neck;
        int r_finger;
        int l_finger;
        int head;
        int r_hand;
        int l_hand;
        int gloves;
        int chest;
        int legs;
        int feet;
        int back;
        int lr_hand;
        int hair;
        int hair_style_id;
        int hair_color_id;
        int face;
        double max_hp;
        double max_mp;
        int delete_days;
        // Again, for some reason, class goes twice too
        int class_id_copy;
        int auto_select;
        char enchant_effect;
} gs_packet_auth_login_char_t;

typedef struct {
        int count;
        gs_packet_auth_login_char_t characters[10];
} gs_packet_auth_login_t;

void gs_packet_auth_login_add_character(
        gs_packet_auth_login_t *dest,
        gs_character_t *src,
        int playOK1);

void gs_packet_auth_login_pack(packet_t *dest, gs_packet_auth_login_t *src);

#endif
