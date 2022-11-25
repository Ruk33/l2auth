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

struct l2_character_title {
    // check if this limit is ok.
    l2_string buf[16];
};

struct attributes {
    i32 _int;
    i32 str;
    i32 con;
    i32 men;
    i32 dex;
    i32 wit;
};

struct l2_character {
    u32 id;
    u32 game_time;
    // enum character_state state;
    // struct v3 position;
    i32 x;
    i32 y;
    i32 z;
    // struct v3 velocity;
    // struct v3 moving_to;
    struct l2_character_name name;
    struct l2_character_title title;
    u32 play_ok1;
    u32 clan_id;
    u32 sex;
    u32 race_id;
    u32 class_id;
    u32 active;
    double hp;
    double mp;
    double max_hp;
    double max_mp;
    u32 cp;
    u32 max_cp;
    struct attributes attrs;
    u32 sp;
    u32 exp;
    u32 level;
    u32 karma;
    u32 under_obj_id;
    u32 r_ear_obj_id;
    u32 l_ear_obj_id;
    u32 neck_obj_id;
    u32 r_finger_obj_id;
    u32 l_finger_obj_id;
    u32 head_obj_id;
    u32 r_hand_obj_id;
    u32 l_hand_obj_id;
    u32 gloves_obj_id;
    u32 chest_obj_id;
    u32 legs_obj_id;
    u32 feet_obj_id;
    u32 back_obj_id;
    u32 lr_hand_obj_id;
    u32 hair_obj_id;
    u32 under_id;
    u32 r_ear_id;
    u32 l_ear_id;
    u32 neck_id;
    u32 r_finger_id;
    u32 l_finger_id;
    u32 head_id;
    u32 r_hand_id;
    u32 l_hand_id;
    u32 gloves_id;
    u32 chest_id;
    u32 legs_id;
    u32 feet_id;
    u32 back_id;
    u32 lr_hand_id;
    u32 hair_id;
    u32 hair_style_id;
    u32 hair_color_id;
    u32 face_id;
    u32 delete_days;
    u32 auto_select;
    u8 enchant_effect;
    u32 p_attack;
    u32 m_attack;
    u32 p_def;
    u32 m_def;
    u32 evasion;
    u32 accuracy;
    u32 critical_hit;
    u16 inventory_limit;
    u32 run_speed;
    u32 walk_speed;
    u32 p_attack_speed;
    u32 m_attack_speed;
    double movement_speed_multiplier;
    double attack_speed_multiplier;
    double collision_radius;
    double collision_height;
    u32 name_color;
    u32 current_load;
    u32 max_load;
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

struct account {
    struct username username;
    struct encrypted_password password;
    // todo: confirm how many characters we can have in the lobby or
    // per account.
    struct l2_character characters[7];
    int characters_count;
};

#endif
