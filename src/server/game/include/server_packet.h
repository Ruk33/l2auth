#ifndef SERVER_PACKET_H
#define SERVER_PACKET_H

#include "../../../include/util.h"
#include "../../../include/packet.h"
#include "l2_string.h"
#include "types.h"

struct packet_protocol_version {
    byte content[9];
};

struct packet_d0 {
    u32 manor_size;
};

struct packet_char_select {
    u32 id;
    i32 playOK1;
    u32 race_id;
    u32 class_id;
    u32 clan_id;
    struct char_name name;
    struct char_title title;
    u32 sex;
    u32 active;
    struct char_pos position;
    double hp;
    double mp;
    u32 sp;
    u32 exp;
    u32 level;
    struct char_attr attrs;
    u32 game_time;
};

struct packet_validate_pos {
    u32 id;
    struct char_pos position;
};

struct packet_move {
    u32 id;
    struct char_pos prev_pos;
    struct char_pos new_pos;
};

struct packet_restart {
    i32 response;
};

struct packet_revive {
    u32 obj_id;
};

struct packet_target_selected {
    u32 target_id;
    u32 color;
};

struct packet_auth_login_char {
    u32 id;
    struct char_name name;
    i32 playOK1;
    u32 clan_id;
    u32 sex;
    u32 race_id;
    u32 class_id;
    u32 active;
    struct char_pos position;
    double hp;
    double mp;
    double max_hp;
    double max_mp;
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
    u32 under;
    u32 r_ear;
    u32 l_ear;
    u32 neck;
    u32 r_finger;
    u32 l_finger;
    u32 head;
    u32 r_hand;
    u32 l_hand;
    u32 gloves;
    u32 chest;
    u32 legs;
    u32 feet;
    u32 back;
    u32 lr_hand;
    u32 hair;
    u32 hair_style_id;
    u32 hair_color_id;
    u32 face;
    u32 delete_days;
    u32 auto_select;
    u8 enchant_effect;
};

struct packet_auth_login {
    u32 count;
    // Todo: confirm how many characters a user can have in the lobby.
    struct packet_auth_login_char characters[10];
};

struct packet_char_info {
    u32 id;
    struct char_pos position;
    struct char_name name;
    struct char_title title;
    u32 race_id;
    u32 sex;
    u32 class_id;
    u32 pvp_flag;
    u32 karma;
    u32 m_attack_speed;
    u32 p_attack_speed;
    u32 run_speed;
    u32 walk_speed;
    u32 swim_run_speed;
    u32 swim_walk_speed;
    u32 fly_run_speed;
    u32 fly_walk_speed;
    double movement_multiplier;
    double attack_speed_multiplier;
    double collision_radius;
    double collision_height;
    u32 hair_style;
    u32 hair_color;
    u32 face;
    u32 clan_id;
    u32 clan_crest_id;
    u32 ally_id;
    u32 ally_crest_id;
    u8 standing;
    u8 running;
    u8 in_combat;
    u8 alike_dead;
    u8 invisible;
    u8 mount_type;
    u8 private_store_type;
    u16 cubics;
    u8 find_party_member;
    u32 abnormal_effect;
    u16 recommendations_left;
    u8 mounted;
    u32 clan_crest_large_id;
    u8 hero_symbol;
    u8 hero_aura;
    u8 fishing;
    u32 name_color;
};

struct packet_new_char_template {
    u32 race_id;
    u32 class_id;
    struct char_attr attrs;
};

struct packet_new_char {
    u32 count;
    struct packet_new_char_template templates[10];
};

struct packet_npc_info {
    u32 id;
    u32 template_id;
    u32 attackable;
    struct char_pos position;
    u32 m_attack_speed;
    u32 p_attack_speed;
    u32 run_speed;
    u32 walk_speed;
    u32 swim_run_speed;
    u32 swim_walk_speed;
    u32 fly_run_speed;
    u32 fly_walk_speed;
    double magic_multiplier;
    double movement_multiplier;
    double collision_radius;
    double collision_height;
    u32 r_hand;
    u32 l_hand;
    u8 name_above_char;
    u8 running;
    u8 in_combat;
    u8 alike_dead;
    u8 summoned;
    struct char_name name;
    struct char_title title;
    u32 karma;
    u32 abnormal_effect;
    u8 team_circle;
};

enum packet_status_type
{
    STATUS_LEVEL = 0x01,
    STATUS_EXP   = 0x02,
    STATUS_STR   = 0x03,
    STATUS_DEX   = 0x04,
    STATUS_CON   = 0x05,
    STATUS_INT   = 0x06,
    STATUS_WIT   = 0x07,
    STATUS_MEN   = 0x08,

    STATUS_CUR_HP = 0x09,
    STATUS_MAX_HP = 0x0a,
    STATUS_CUR_MP = 0x0b,
    STATUS_MAX_MP = 0x0c,

    STATUS_SP       = 0x0d,
    STATUS_CUR_LOAD = 0x0e,
    STATUS_MAX_LOAD = 0x0f,

    STATUS_P_ATK    = 0x11,
    STATUS_ATK_SPD  = 0x12,
    STATUS_P_DEF    = 0x13,
    STATUS_EVASION  = 0x14,
    STATUS_ACCURACY = 0x15,
    STATUS_CRITICAL = 0x16,
    STATUS_M_ATK    = 0x17,
    STATUS_CAST_SPD = 0x18,
    STATUS_M_DEF    = 0x19,
    STATUS_PVP_FLAG = 0x1a,
    STATUS_KARMA    = 0x1b,

    STATUS_CUR_CP = 0x21,
    STATUS_MAX_CP = 0x22,
};

struct packet_status_attr {
    enum packet_status_type type;
    u32 value;
};

struct packet_status {
    u32 obj_id;
    struct packet_status_attr attributes[35];
    u32 count;
};

struct packet_enter_world {
    u32 id;
    struct char_pos position;
    struct char_name name;
    u32 race_id;
    u32 sex;
    u32 class_id;
    u32 level;
    u32 exp;
    struct char_attr attrs;
    u32 max_hp;
    u32 hp;
    u32 max_mp;
    u32 mp;
    u32 sp;
    u32 current_load;
    u32 max_load;
    u32 paperdoll_under;
    u32 paperdoll_r_ear;
    u32 paperdoll_l_ear;
    u32 paperdoll_neck;
    u32 paperdoll_r_finger;
    u32 paperdoll_l_finger;
    u32 paperdoll_head;
    u32 paperdoll_r_hand;
    u32 paperdoll_l_hand;
    u32 paperdoll_gloves;
    u32 paperdoll_chest;
    u32 paperdoll_legs;
    u32 paperdoll_feet;
    u32 paperdoll_back;
    u32 paperdoll_lr_hand;
    u32 paperdoll_hair;
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
    u32 face;
    i32 access_level;
    struct char_title title;
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

struct packet_say {
    u32 obj_id;
    u32 type;
    // Todo: double check name and message limits.
    struct char_name name;
    struct char_message message;
};

struct packet_attack_hit {
    u32 target_id;
    u32 damage;
    u8 flags;
};

struct packet_attack {
    u32 attacker_id;
    struct char_pos attacker_pos;
    struct packet_attack_hit hits[10];
    u16 hit_count;
};

struct packet_auto_attack_stop {
    u32 target_id;
};

struct packet_unselect_target {
    u32 target_id;
    struct char_pos target_pos;
};

struct packet_die {
    u32 obj_id;
    u32 to_village;
    u32 to_hideaway;
    u32 to_castle;
    u32 to_siege;
    u32 sweepable;
    u32 to_fixed;
};

struct packet_npc_html_message {
    u32 message_id;
    struct npc_window_message message;
};

struct packet_change_move_type {
    u32 obj_id;
    u32 running;
};

struct packet_skill {
    u32 id;
    u32 passive;
    u32 level;
};

struct packet_skill_list {
    struct packet_skill skills[32];
    u32 count;
};

struct packet_skill_use {
    u32 src_id;
    u32 target_id;
    u32 skill_id;
    u32 skill_level;
    u32 hit_time;
    u32 reuse_delay;
    struct char_pos position;
};

void packet_protocol_version_to(struct packet *dest, struct packet_protocol_version *src);

void packet_d0_to(struct packet *dest, struct packet_d0 *src);

void packet_char_select_to(struct packet *dest, struct packet_char_select *src);

void packet_validate_pos_to(struct packet *dest, struct packet_validate_pos *src);

void packet_move_to(struct packet *dest, struct packet_move *src);

void packet_restart_to(struct packet *dest, struct packet_restart *src);

void packet_revive_to(struct packet *dest, struct packet_revive *src);

void packet_target_selected_to(struct packet *dest, struct packet_target_selected *src);

void packet_auth_login_char_to(struct packet *dest, struct packet_auth_login_char *src);

void packet_auth_login_to(struct packet *dest, struct packet_auth_login *src);

void packet_char_info_to(struct packet *dest, struct packet_char_info *src);

void packet_new_char_template_to(struct packet *dest, struct packet_new_char_template *src);

void packet_new_char_to(struct packet *dest, struct packet_new_char *src);

void packet_npc_info_to(struct packet *dest, struct packet_npc_info *src);

void packet_status_to(struct packet *dest, struct packet_status *src);

void packet_enter_world_to(struct packet *dest, struct packet_enter_world *src);

void packet_say_to(struct packet *dest, struct packet_say *src);

void packet_attack_to(struct packet *dest, struct packet_attack *src);

void packet_auto_attack_stop_to(struct packet *dest, struct packet_auto_attack_stop *src);

void packet_unselect_target_to(struct packet *dest, struct packet_unselect_target *src);

void packet_die_to(struct packet *dest, struct packet_die *src);

void packet_npc_html_message_to(struct packet *dest, struct packet_npc_html_message *src);

void packet_change_move_type_to(struct packet *dest, struct packet_change_move_type *src);

void packet_skill_list_to(struct packet *dest, struct packet_skill_list *src);

void packet_skill_use_to(struct packet *dest, struct packet_skill_use *src);

#endif