#ifndef INCLUDE_GS_SERVER_PACKETS_H
#define INCLUDE_GS_SERVER_PACKETS_H

#include "platform.h"
#include "packet.h"

struct gs_packet_begin_rotation {
    u32_t obj_id;
    i32_t degree;
    i32_t side;
};

struct gs_packet_attack_hit {
    u32_t target_id;
    u32_t damage;
    u8_t flags;
};

struct gs_packet_attack {
    u32_t attacker_id;
    i32_t attacker_x;
    i32_t attacker_y;
    i32_t attacker_z;
    struct gs_packet_attack_hit hits[10];
    u16_t hit_count;
};

// Note: this packet seems to be the one in charge
// of playing the being hit/on guard animation.
// Maybe we can rename it to attack landed?
struct gs_packet_auto_attack {
    u32_t target_id;
};

struct gs_packet_create_char {
    u32_t response;
};

struct gs_packet_d0 {
    short unknown;
    int manor_size;
};

struct gs_packet_die {
    u32_t obj_id;
    u32_t to_village;
    u32_t to_hideaway;
    u32_t to_castle;
    u32_t to_siege;
    u32_t sweepable;
    u32_t to_fixed;
};

struct gs_packet_char_select {
    u32_t empty[54];
    l2_string_t name[32];
    u32_t id;
    l2_string_t title[28];
    i32_t playOK1;
    u32_t clan_id;
    u32_t sex;
    u32_t race_id;
    u32_t class_id;
    u32_t active;
    i32_t x;
    i32_t y;
    i32_t z;
    double hp;
    double mp;
    i32_t sp;
    i32_t exp;
    i32_t level;
    i32_t _int;
    i32_t str;
    i32_t con;
    i32_t men;
    i32_t dex;
    i32_t wit;
    u32_t game_time;
};

struct gs_packet_validate_pos {
    u32_t id;
    i32_t x;
    i32_t y;
    i32_t z;
    i32_t heading;
};

struct gs_packet_move {
    u32_t id;
    i32_t prev_x;
    i32_t prev_y;
    i32_t prev_z;
    i32_t new_x;
    i32_t new_y;
    i32_t new_z;
};

struct gs_packet_protocol_version {
    byte_t content[9];
};

struct gs_packet_quest_list {
    byte_t empty[7];
};

struct gs_packet_restart {
    i32_t response;
};

struct gs_packet_revive {
    u32_t obj_id;
};

struct gs_packet_target_selected {
    u32_t target_id;
    u32_t color;
};

struct gs_packet_auth_login_char {
    u32_t empty[10];
    l2_string_t name[32];
    u32_t id;
    // For some reason, name goes twice in the packet
    l2_string_t name_copy[32];
    i32_t playOK1;
    u32_t clan_id;
    u32_t sex;
    u32_t race_id;
    u32_t class_id;
    u32_t active;
    i32_t x;
    i32_t y;
    i32_t z;
    double hp;
    double mp;
    u32_t sp;
    u32_t exp;
    u32_t level;
    i32_t karma;
    u32_t under_obj_id;
    u32_t r_ear_obj_id;
    u32_t l_ear_obj_id;
    u32_t neck_obj_id;
    u32_t r_finger_obj_id;
    u32_t l_finger_obj_id;
    u32_t head_obj_id;
    u32_t r_hand_obj_id;
    u32_t l_hand_obj_id;
    u32_t gloves_obj_id;
    u32_t chest_obj_id;
    u32_t legs_obj_id;
    u32_t feet_obj_id;
    u32_t back_obj_id;
    u32_t lr_hand_obj_id;
    u32_t hair_obj_id;
    u32_t under;
    u32_t r_ear;
    u32_t l_ear;
    u32_t neck;
    u32_t r_finger;
    u32_t l_finger;
    u32_t head;
    u32_t r_hand;
    u32_t l_hand;
    u32_t gloves;
    u32_t chest;
    u32_t legs;
    u32_t feet;
    u32_t back;
    u32_t lr_hand;
    u32_t hair;
    u32_t hair_style_id;
    u32_t hair_color_id;
    u32_t face;
    double max_hp;
    double max_mp;
    u32_t delete_days;
    // Again, for some reason, class goes twice too
    u32_t class_id_copy;
    u32_t auto_select;
    u8_t enchant_effect;
};

// Todo: do we wanna use a more descriptive name for this packet?
struct gs_packet_auth_login {
    u32_t count;
    // Todo: confirm how many characters a user can have in the lobby.
    struct gs_packet_auth_login_char characters[10];
};

struct gs_packet_char_info {
    i32_t x;
    i32_t y;
    i32_t z;
    i32_t heading;
    u32_t id;
    l2_string_t name[32];
    u32_t race_id;
    u32_t sex;
    u32_t class_id;
    // int empty[8]
    // int paperdoll[10]
    u32_t pvp_flag;
    u32_t karma;
    u32_t m_attack_speed;
    u32_t p_attack_speed;
    u32_t run_speed;
    u32_t walk_speed;
    u32_t swim_run_speed;
    u32_t swim_walk_speed;
    u32_t fly_run_speed;
    u32_t fly_walk_speed;
    double movement_multiplier;
    double attack_speed_multiplier;
    double collision_radius;
    double collision_height;
    u32_t hair_style;
    u32_t hair_color;
    u32_t face;
    l2_string_t title[28];
    u32_t clan_id;
    u32_t clan_crest_id;
    u32_t ally_id;
    u32_t ally_crest_id;
    u8_t standing;
    u8_t running;
    u8_t in_combat;
    u8_t alike_dead;
    u8_t invisible;
    u8_t mount_type;
    u8_t private_store_type;
    u16_t cubics;
    u8_t find_party_member;
    u32_t abnormal_effect;
    // char empty_char[2]
    u16_t recommendations_left;
    u8_t mounted;
    u32_t clan_crest_large_id;
    u8_t hero_symbol;
    u8_t hero_aura;
    u8_t fishing;
    u32_t name_color;
};

struct gs_packet_new_char_template {
    u32_t separator_0a[6];
    u32_t separator_46[6];
    u32_t race;
    u32_t _class;
    u32_t str;
    u32_t dex;
    u32_t con;
    u32_t _int;
    u32_t wit;
    u32_t men;
};

struct gs_packet_new_char {
    u32_t count;
    struct gs_packet_new_char_template templates[10];
};

struct gs_packet_npc_info {
    u32_t id;
    u32_t template_id;
    u32_t attackable;
    i32_t x;
    i32_t y;
    i32_t z;
    i32_t heading;
    u32_t m_attack_speed;
    u32_t p_attack_speed;
    u32_t run_speed;
    u32_t walk_speed;
    u32_t swim_run_speed;
    u32_t swim_walk_speed;
    u32_t fly_run_speed;
    u32_t fly_walk_speed;
    double magic_multiplier;
    double movement_multiplier;
    double collision_radius;
    double collision_height;
    u32_t r_hand;
    u32_t l_hand;
    u8_t name_above_char;
    u8_t running;
    u8_t in_combat;
    u8_t alike_dead;
    u8_t summoned;
    l2_string_t name[32];
    l2_string_t title[28];
    u32_t karma; // ?
    u32_t abnormal_effect;
    u8_t team_circle;
};

// Todo: rename, this is not very descriptive.
enum gs_packet_status_type
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

struct gs_packet_status_attr {
    enum gs_packet_status_type type;
    i32_t value;
};

struct gs_packet_status {
    u32_t obj_id;
    struct gs_packet_status_attr attributes[35];
    u32_t count;
};

struct gs_packet_enter_world {
    i32_t x;
    i32_t y;
    i32_t z;
    i32_t heading;
    u32_t id;
    l2_string_t name[32];
    u32_t race_id;
    u32_t sex;
    u32_t class_id;
    u32_t level;
    u32_t exp;
    i32_t str;
    i32_t dex;
    i32_t con;
    i32_t _int;
    i32_t wit;
    i32_t men;
    u32_t max_hp;
    u32_t hp;
    u32_t max_mp;
    u32_t mp;
    u32_t sp;
    i32_t current_load;
    i32_t max_load;
    i32_t unknown;
    i32_t paperdoll_under;
    i32_t paperdoll_r_ear;
    i32_t paperdoll_l_ear;
    i32_t paperdoll_neck;
    i32_t paperdoll_r_finger;
    i32_t paperdoll_l_finger;
    i32_t paperdoll_head;
    i32_t paperdoll_r_hand;
    i32_t paperdoll_l_hand;
    i32_t paperdoll_gloves;
    i32_t paperdoll_chest;
    i32_t paperdoll_legs;
    i32_t paperdoll_feet;
    i32_t paperdoll_back;
    i32_t paperdoll_lr_hand;
    i32_t paperdoll_hair;
    i32_t empty[16];
    u32_t p_attack;
    u32_t p_attack_speed;
    u32_t p_def;
    u32_t evasion_rate;
    u32_t accuracy;
    u32_t critical_hit;
    u32_t m_attack;
    u32_t m_attack_speed;
    u32_t p_attack_speed_copy;
    u32_t m_def;
    u32_t pvp_flag;
    u32_t karma;
    u32_t run_speed;
    u32_t walk_speed;
    u32_t swim_run_speed;
    u32_t swim_walk_speed;
    u32_t fly_run_speed;
    u32_t fly_walk_speed;
    u32_t fly_run_speed_copy;
    u32_t fly_walk_speed_copy;
    double movement_speed_multiplier;
    double attack_speed_multiplier;
    double collision_radius;
    double collision_height;
    u32_t hair_style_id;
    u32_t hair_color_id;
    u32_t face;
    i32_t access_level;
    l2_string_t title[28];
    u32_t clan_id;
    u32_t clan_crest_id;
    u32_t ally_id;
    u32_t ally_crest_id;
    u32_t clan_leader;
    u8_t mount_type;
    u8_t private_store_type;
    u8_t dwarven_craft;
    u32_t pk_kills;
    u32_t pvp_kills;
    u16_t cubics;
    u8_t party_members;
    i32_t abnormal_effect;
    u8_t char_empty[3];
    i32_t clan_privileges;
    i32_t int_empty[9];
    u16_t recommendation_left;
    u16_t recommendation_have;
    u16_t inventory_limit;
    u32_t class_id_copy;
    u32_t max_cp;
    u32_t cp;
    u8_t mounted;
    u8_t team_circle;
    u32_t clan_crest_large_id;
    u8_t hero_symbol;
    u8_t hero;
    i32_t fish_x;
    i32_t fish_y;
    i32_t fish_z;
    u32_t name_color;
};

struct gs_packet_say {
    u32_t character_id;
    u32_t type;
    // Todo: double check name and message limits.
    l2_string_t name[32];
    l2_string_t message[256];
};

struct gs_packet_auto_attack_stop {
    u32_t target_id;
};

struct gs_packet_unselect_target {
    u32_t target_id;
    i32_t target_x;
    i32_t target_y;
    i32_t target_z;
};

struct gs_packet_npc_html_message {
    u32_t message_id;
    // (franco.montenegro) Blame kornez if this is not the right limit <3.
    // If we pass more, the client will crash.
    // (franco.montenegro) confirmed on stream, it was 4kb, I got scammed
    // big time. Chat was wrong all the time.
    l2_string_t message[macro_util_kb(4)];
};

struct gs_packet_change_move_type {
    u32_t obj_id;
    u32_t running;
    u32_t empty;
};

struct gs_packet_skill {
    u32_t passive;
    u32_t level;
    u32_t id;
};

// This packet is sent when the player
// opens up the skill list.
struct gs_packet_skill_list {
    u32_t count;
    // (franco.montenegro) We shouldn't need more than 32
    // skills but if we do, just increase the number.
    struct gs_packet_skill skills[32];
};

struct gs_packet_skill_use {
    u32_t src_id;
    u32_t target_id;
    u32_t skill_id;
    u32_t skill_level;
    u32_t hit_time;
    u32_t reuse_delay;
    u32_t x;
    u32_t y;
    u32_t z;
    u16_t empty_short;
    u32_t empty_int[3];
};

void gs_packet_begin_rotation_pack(packet_t *dest,
                                   struct gs_packet_begin_rotation *src);

void gs_packet_attack_pack(packet_t *dest, struct gs_packet_attack *src);

void gs_packet_auto_attack_pack(packet_t *dest,
                                struct gs_packet_auto_attack *src);

void gs_packet_create_char_pack(packet_t *dest,
                                struct gs_packet_create_char *src);

void gs_packet_d0_pack(packet_t *dest, struct gs_packet_d0 *src);

void gs_packet_die_pack(packet_t *dest, struct gs_packet_die *src);

void gs_packet_leave_world_pack(packet_t *dest);

void gs_packet_char_select_pack(packet_t *dest,
                                struct gs_packet_char_select *src);

void gs_packet_validate_pos_pack(packet_t *dest,
                                 struct gs_packet_validate_pos *src);

void gs_packet_move_pack(packet_t *dest, struct gs_packet_move *src);

void gs_packet_protocol_version_pack(packet_t *dest,
                                     struct gs_packet_protocol_version *src);

void gs_packet_quest_list_pack(packet_t *dest, struct gs_packet_quest_list *src);

void gs_packet_restart_pack(packet_t *dest, struct gs_packet_restart *src);

void gs_packet_revive_pack(packet_t *dest, struct gs_packet_revive *src);

void gs_packet_target_selected_pack(packet_t *dest,
                                    struct gs_packet_target_selected *src);

void gs_packet_auth_login_pack(packet_t *dest, struct gs_packet_auth_login *src);

void gs_packet_char_info_pack(packet_t *dest, struct gs_packet_char_info *src);

void gs_packet_new_char_pack(packet_t *dest, struct gs_packet_new_char *src);

void gs_packet_npc_info_pack(packet_t *dest, struct gs_packet_npc_info *src);

void gs_packet_status_pack(packet_t *dest, struct gs_packet_status *src);

void gs_packet_enter_world_pack(packet_t *dest,
                                struct gs_packet_enter_world *src);

void gs_packet_say_pack(packet_t *dest, struct gs_packet_say *src);

void gs_packet_auto_attack_stop_pack(packet_t *dest,
                                     struct gs_packet_auto_attack_stop *src);

void gs_packet_unselect_target_pack(packet_t *dest,
                                    struct gs_packet_unselect_target *src);

void gs_packet_npc_html_message_pack(packet_t *dest,
                                     struct gs_packet_npc_html_message *src);

void gs_packet_change_move_type_pack(packet_t *dest,
                                     struct gs_packet_change_move_type *src);

void gs_packet_skill_list_pack(packet_t *dest, struct gs_packet_skill_list *src);

void gs_packet_skill_use_pack(packet_t *dest, struct gs_packet_skill_use *src);

#endif