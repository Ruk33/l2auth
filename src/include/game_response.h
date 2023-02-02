#ifndef GAME_RESPONSE_H
#define GAME_RESPONSE_H

#include "game_server.h"
#include "l2auth.h"

struct protocol {
    byte buf[9];
};

struct response_protocol {
    struct protocol protocol;
};

struct response_auth_login {
    u32 count;
    // TODO: confirm how many characters a user can have in the lobby.
    struct l2_character characters[10];
};

struct response_show_creation_screen {
    u32 count;
};

struct response_selected_character {
    u32 id;
    u32 play_ok1;
    u32 race_id;
    u32 class_id;
    u32 clan_id;
    struct l2_character_name name;
    struct l2_character_title title;
    u32 sex;
    u32 active;
    // todo: make use of vectors!
    // struct v3 position;
    i32 x;
    i32 y;
    i32 z;
    double hp;
    double mp;
    u32 sp;
    u32 exp;
    u32 level;
    struct attributes attrs;
    u32 game_time;
};

struct response_d0 {
    u32 manor_size;
};

struct response_quest_list {
    // todo: complete me!
    byte empty[7];
};

struct response_enter_world {
    u32 id;
    //struct v3 position;
    i32 x;
    i32 y;
    i32 z;
    i32 heading;
    struct l2_character_name name;
    u32 race_id;
    u32 sex;
    u32 class_id;
    u32 level;
    u32 exp;
    struct attributes attrs;
    u32 max_hp;
    u32 hp;
    u32 max_mp;
    u32 mp;
    u32 sp;
    u32 current_load;
    u32 max_load;
    u32 paperdoll_under_id;
    u32 paperdoll_r_ear_id;
    u32 paperdoll_l_ear_id;
    u32 paperdoll_neck_id;
    u32 paperdoll_r_finger_id;
    u32 paperdoll_l_finger_id;
    u32 paperdoll_head_id;
    u32 paperdoll_r_hand_id;
    u32 paperdoll_l_hand_id;
    u32 paperdoll_gloves_id;
    u32 paperdoll_chest_id;
    u32 paperdoll_legs_id;
    u32 paperdoll_feet_id;
    u32 paperdoll_back_id;
    u32 paperdoll_lr_hand_id;
    u32 paperdoll_hair_id;
    u32 p_attack;
    u32 p_attack_speed;
    u32 p_def;
    u32 evasion_rate;
    u32 accuracy;
    u32 critical_hit;
    u32 m_attack;
    u32 m_attack_speed;
    u32 m_def;
    u32 pvp_flag;
    u32 karma;
    u32 run_speed;
    u32 walk_speed;
    u32 swim_run_speed;
    u32 swim_walk_speed;
    u32 fly_run_speed;
    u32 fly_walk_speed;
    u32 fly_run_speed_copy;
    u32 fly_walk_speed_copy;
    double movement_speed_multiplier;
    double attack_speed_multiplier;
    double collision_radius;
    double collision_height;
    u32 hair_style_id;
    u32 hair_color_id;
    u32 face_id;
    i32 access_level;
    struct l2_character_title title;
    u32 clan_id;
    u32 clan_crest_id;
    u32 ally_id;
    u32 ally_crest_id;
    u32 clan_leader;
    u8 mount_type;
    u8 private_store_type;
    u8 dwarven_craft;
    u32 pk_kills;
    u32 pvp_kills;
    u16 cubics;
    u8 party_members;
    u32 abnormal_effect;
    u32 clan_privileges;
    u16 recommendation_left;
    u16 recommendation_have;
    u16 inventory_limit;
    u32 max_cp;
    u32 cp;
    u8 mounted;
    u8 team_circle;
    u32 clan_crest_large_id;
    u8 hero_symbol;
    u8 hero;
    i32 fish_x;
    i32 fish_y;
    i32 fish_z;
    u32 name_color;
};

struct response_restart {
    u32 code;
};

struct response_move {
    u32 id;
    i32 destination_x;
    i32 destination_y;
    i32 destination_z;
    i32 origin_x;
    i32 origin_y;
    i32 origin_z;
};

struct response_validate_position {
    u32 id;
    i32 x;
    i32 y;
    i32 z;
    i32 heading;
};

struct response_char_info {
    u32 id;
    u32 poly_id;
    u32 race_id;
    u32 sex;
    u32 class_id;
    u32 helmet_id;
    u32 r_hand_weapon_id;
    u32 l_hand_weapon_id;
    u32 lr_hand_weapon_id;
    u32 gloves_id;
    u32 chest_id;
    u32 legs_id;
    u32 boots_id;
    u32 back_id;
    u32 hair_id;
    // not sure if karma can be negative, so for
    // now use signed.
    i32 karma;
    i32 x;
    i32 y;
    i32 z;
    i32 heading;
    // again, not sure if these can be negatives.
    i32 m_attack_speed;
    i32 p_attack_speed;
    i32 run_speed;
    i32 walk_speed;
    i32 swim_run_speed;
    i32 swim_walk_speed;
    i32 fly_walk_speed;
    i32 fly_run_speed;
    double movement_speed_multiplier;
    double attack_speed_multiplier;
    double collision_radius;
    double collision_height;
    u8 is_running;
    u8 is_in_combat;
    u8 is_alike_dead;
    u8 is_invisible;
    u8 is_sitting;
    u8 mount_type;
    u8 private_store_type;
    u8 hero_symbol;
    u8 hero_aura;
    u8 is_fishing;
    struct l2_character_name name;
    struct l2_character_title title;
    u16 abnormal_effect;
    u32 pvp_flag;
    u32 hair_style_id;
    u32 hair_color_id;
    u32 face_id;
    u32 clan_id;
    u32 clan_crest_id;
    u32 ally_id;
    u32 ally_crest_id;
    u16 cubics_count;
    // confirm the maximum amount of cubics.
    u16 cubic_ids[3];
    u16 recommendation_left;
    u32 clan_crest_large_id;
    i32 fish_x;
    i32 fish_y;
    i32 fish_z;
    u32 name_color;
};

void response_protocol_encode(struct packet *dest, struct response_protocol *src);
void response_auth_login_encode(struct packet *dest, struct response_auth_login *src);
void response_show_creation_screen_encode(struct packet *dest, struct response_show_creation_screen *src);
void response_selected_character_encode(struct packet *dest, struct response_selected_character *src);
void response_d0_encode(struct packet *dest, struct response_d0 *src);
void response_quest_list_encode(struct packet *dest, struct response_quest_list *src);
void response_enter_world_encode(struct packet *dest, struct response_enter_world *src);
void response_restart_encode(struct packet *dest, struct response_restart *src);
void response_move_encode(struct packet *dest, struct response_move *src);
void response_validate_position_encode(struct packet *dest, struct response_validate_position *src);
void response_char_info_encode(struct packet *dest, struct l2_character *character);
void response_action_failed_encode(struct packet *dest);
void response_action_select_target_encode(struct packet *dest, struct l2_character *target);
void response_attack_target(struct packet *dest, struct l2_character *attacker, struct l2_character *target);

#endif
