#include <assert.h>
#include "include/config.h"
#include "include/util.h"
#include "include/gs_types.h"
#include "include/gs_server_packets.h"
#include "include/gs_client_packets.h"
#include "include/packet.h"
#include "include/l2_string.h"
#include "include/gs_session.h"
#include "include/gs_character.h"

#define macro_gs_character_each(character, state) \
    macro_util_list_each(state->list_characters, struct gs_character, character)

int gs_character_is_npc(struct gs_character *src)
{
    assert(src);
    return src->session ? 0 : 1;
}

struct gs_character *gs_character_find_by_id(struct gs_state *gs, u32_t id)
{
    struct gs_character *character = 0;

    assert(gs);

    macro_gs_character_each(character, gs)
    {
        if (character->id == id) {
            return character;
        }
    }

    return 0;
}

static void gs_character_encrypt_and_send_packet(struct gs_state *gs,
                                                 struct gs_character *from,
                                                 packet_t *packet)
{
    assert(gs);
    assert(from);
    assert(packet);

    if (gs_character_is_npc(from)) {
        return;
    }

    gs_session_encrypt(from->session, packet, packet);
    gs_session_send_packet(gs, from->session, packet);
}

void gs_character_broadcast_ignoring_src(struct gs_state *gs,
                                         struct gs_character *src,
                                         packet_t *packet)
{
    packet_t response[2048] = { 0 };

    struct gs_character *character = 0;

    u64_t safe_packet_size = 0;

    assert(gs);
    assert(src);
    assert(packet);

    if (sizeof(response) < packet_size(packet)) {
        log_normal(
            "WARNING - packet size %d bigger than buf %ld. Packet won't be "
            "sent. Maybe increase buf size?",
            packet_size(packet),
            sizeof(response));
        return;
    }

    // This way maybe we can clear less space than what we really need.
    safe_packet_size = (u64_t) (packet_size(packet) * 2);
    safe_packet_size = macro_util_min(sizeof(response), safe_packet_size);

    macro_gs_character_each(character, gs)
    {
        if (gs_character_is_npc(character)) {
            continue;
        }
        if (character->id == src->id) {
            continue;
        }

        util_set_zero(response, safe_packet_size);
        macro_util_cpy_bytes_to_arr(response, packet, packet_size(packet));
        gs_character_encrypt_and_send_packet(gs, character, response);
    }
}

static void gs_character_broadcast_packet(struct gs_state *gs,
                                          struct gs_character *from,
                                          packet_t *packet)
{
    packet_t response[2048] = { 0 };

    struct gs_character *character = 0;

    u64_t safe_packet_size = 0;

    assert(gs);
    assert(from);
    assert(packet);

    if (sizeof(response) < packet_size(packet)) {
        log_normal(
            "WARNING - packet size %d bigger than buf %ld. Packet won't be "
            "sent. Maybe increase buf size?",
            packet_size(packet),
            sizeof(response));
        return;
    }

    // This way maybe we can clear less space than what we really need.
    safe_packet_size = (u64_t) (packet_size(packet) * 2);
    safe_packet_size = macro_util_min(sizeof(response), safe_packet_size);

    macro_gs_character_each(character, gs)
    {
        if (gs_character_is_npc(character)) {
            continue;
        }

        util_set_zero(response, safe_packet_size);
        macro_util_cpy_bytes_to_arr(response, packet, packet_size(packet));
        gs_character_encrypt_and_send_packet(gs, character, response);
    }
}

void gs_character_action_failed(struct gs_state *gs, struct gs_character *src)
{
    byte_t type           = 0x00;
    packet_t response[16] = { 0 };

    assert(gs);
    assert(src);

    type = 0x25;
    macro_packet_append_val(response, type);

    gs_character_encrypt_and_send_packet(gs, src, response);
}

void gs_character_face_to(struct gs_state *gs,
                          struct gs_character *src,
                          i32_t degree)
{
    packet_t response[64]                           = { 0 };
    struct gs_packet_begin_rotation rotation_packet = { 0 };

    assert(gs);
    assert(src);

    rotation_packet.obj_id = src->id;
    rotation_packet.degree = degree;
    rotation_packet.side   = 1;

    gs_packet_begin_rotation_pack(response, &rotation_packet);
    gs_character_broadcast_packet(gs, src, response);
}

void gs_character_say(struct gs_state *gs,
                      struct gs_character *from,
                      char *message,
                      size_t message_size)
{
    struct gs_packet_say say = { 0 };
    packet_t response[256]   = { 0 };

    assert(gs);
    assert(from);
    assert(message);

    say.character_id = from->id;

    macro_l2_str_arr_from_char_arr(say.name, from->name);
    l2_string_from_char(say.message,
                        message,
                        sizeof(say.message),
                        message_size,
                        message_size);

    gs_packet_say_pack(response, &say);
    gs_character_broadcast_packet(gs, from, response);
}

void gs_character_send_status(struct gs_state *gs,
                              struct gs_character *from,
                              struct gs_character *to)
{
    packet_t response[512] = { 0 };

    struct gs_packet_status status = { 0 };

    struct gs_packet_status_attr attr = { 0 };

    assert(gs);
    assert(from);
    assert(to);

    status.obj_id = from->id;

    attr.type            = STATUS_CUR_HP;
    attr.value           = (i32_t) from->stats.hp;
    status.attributes[0] = attr;

    attr.type            = STATUS_MAX_HP;
    attr.value           = (i32_t) from->stats.max_hp;
    status.attributes[1] = attr;

    status.count = 2;

    gs_packet_status_pack(response, &status);
    gs_character_broadcast_packet(gs, from, response);
}

void gs_character_revive(struct gs_state *gs,
                         struct gs_character *src,
                         enum gs_packet_revive_request_option where)
{
    struct gs_packet_revive revive = { 0 };

    packet_t response[32] = { 0 };

    assert(gs);
    assert(src);

    revive.obj_id = src->id;
    gs_packet_revive_pack(response, &revive);
    gs_character_broadcast_packet(gs, src, response);

    switch (where) {
    case REVIVE_IN_CLAN_HALL:
    case REVIVE_IN_CASTLE:
    case REVIVE_IN_SIEGE_HQ:
    case REVIVE_IN_FIXED:
    default:
        break;
    }
}

void gs_character_move(struct gs_state *gs,
                       struct gs_character *character,
                       struct gs_point *p)
{
    struct gs_packet_move move_response = { 0 };

    packet_t packet[64] = { 0 };

    assert(gs);
    assert(character);
    assert(p);

    move_response.id     = character->id;
    move_response.prev_x = character->position.x;
    move_response.prev_y = character->position.y;
    move_response.prev_z = character->position.z;
    move_response.new_x  = character->ai.moving_to.x;
    move_response.new_y  = character->ai.moving_to.y;
    move_response.new_z  = character->ai.moving_to.z;

    gs_packet_move_pack(packet, &move_response);
    gs_character_broadcast_packet(gs, character, packet);
}

void gs_character_die(struct gs_state *gs, struct gs_character *src)
{
    struct gs_packet_die die = { 0 };

    packet_t response[64] = { 0 };

    assert(gs);
    assert(src);

    die.obj_id     = src->id;
    die.to_village = 1;
    die.to_fixed   = 1;

    gs_packet_die_pack(response, &die);
    gs_character_broadcast_packet(gs, src, response);
}

// (franco.montenegro) We should rename this function. The attack
// is launched but the actual hit/damage gets done in the tick
// function. Otherwise, the damage gets applied even after
// the hit reached the target.
void gs_character_launch_attack(struct gs_state *gs,
                                struct gs_character *attacker,
                                struct gs_character *target)
{
    // (franco.montenegro) auto_attack really means,
    // "go into aggro mode"
    struct gs_packet_auto_attack auto_attack = { 0 };

    // (franco.montenegro) this is the packet that
    // actually launches the attack.
    struct gs_packet_attack attack  = { 0 };
    struct gs_packet_attack_hit hit = { 0 };

    packet_t auto_attack_packet[32] = { 0 };
    packet_t attack_packet[256]     = { 0 };

    double d = 0;
    double a = 0;
    i32_t x  = 0;
    i32_t y  = 0;

    assert(gs);
    assert(attacker);
    assert(target);

    auto_attack.target_id = target->id;

    hit.damage    = attacker->stats.p_attack;
    hit.target_id = target->id;

    attack.attacker_id = attacker->id;
    attack.attacker_x  = attacker->position.x;
    attack.attacker_y  = attacker->position.y;
    attack.attacker_z  = attacker->position.z;
    attack.hit_count   = 1;
    attack.hits[0]     = hit;

    gs_packet_auto_attack_pack(auto_attack_packet, &auto_attack);
    gs_packet_attack_pack(attack_packet, &attack);

    gs_character_broadcast_packet(gs, attacker, auto_attack_packet);
    gs_character_broadcast_packet(gs, attacker, attack_packet);
}

void gs_character_stop_auto_attack(struct gs_state *gs,
                                   struct gs_character *attacker,
                                   struct gs_character *target)
{
    struct gs_packet_auto_attack_stop auto_attack_stop = { 0 };

    packet_t response[16] = { 0 };

    assert(gs);
    assert(attacker);
    assert(target);

    auto_attack_stop.target_id = target->id;

    gs_packet_auto_attack_stop_pack(response, &auto_attack_stop);
    gs_character_broadcast_packet(gs, attacker, response);
}

void gs_character_select_target(struct gs_state *gs,
                                struct gs_character *character,
                                struct gs_character *target)
{
    struct gs_packet_target_selected selected = { 0 };

    packet_t response[32] = { 0 };

    assert(gs);
    assert(character);
    assert(target);

    selected.target_id = target->id;
    selected.color     = 0;

    gs_packet_target_selected_pack(response, &selected);
    gs_character_encrypt_and_send_packet(gs, character, response);
}

void gs_character_validate_position(struct gs_state *gs,
                                    struct gs_character *character)
{
    struct gs_packet_validate_pos validate_response = { 0 };

    packet_t response[64] = { 0 };

    assert(gs);
    assert(character);

    validate_response.id      = character->id;
    validate_response.heading = character->heading;
    validate_response.x       = character->position.x;
    validate_response.y       = character->position.y;
    validate_response.z       = character->position.z;

    gs_packet_validate_pos_pack(response, &validate_response);
    // gs_character_encrypt_and_send_packet(gs, character, response);
    gs_character_broadcast_packet(gs, character, response);
}

void gs_character_spawn_random_orc(struct gs_state *gs,
                                   struct gs_point *location)
{
    struct gs_character orc = { 0 };

    assert(gs);
    assert(location);

    orc.id                   = gs_character_get_free_id(gs);
    orc.template_id          = 7082;
    orc.position.x           = location->x;
    orc.position.y           = location->y;
    orc.position.z           = location->z;
    orc.collision_radius     = 8;
    orc.collision_height     = 25;
    orc.level                = 10;
    orc.sex                  = 0;
    orc.stats.hp             = 197;
    orc.stats.max_hp         = 197;
    orc.stats.mp             = 102;
    orc.stats.str            = 40;
    orc.stats.con            = 43;
    orc.stats.dex            = 30;
    orc.stats._int           = 21;
    orc.stats.wit            = 20;
    orc.stats.men            = 10;
    orc.exp                  = 293;
    orc.sp                   = 10;
    orc.stats.p_attack       = 41;
    orc.stats.p_def          = 55;
    orc.stats.m_attack       = 6;
    orc.stats.m_def          = 45;
    orc.stats.p_attack_speed = 249;
    orc.stats.m_attack_speed = 227;
    orc.stats.walk_speed     = 45;
    orc.stats.run_speed      = 110;
    orc.revive_after_killed  = 1;
    orc.revive_after_cd      = 30;

    macro_util_cpy_str_arr(orc.name, "Beti");
    macro_util_cpy_str_arr(orc.title, "Merchant");

    gs_character_spawn(gs, &orc);
}

void gs_character_from_request(struct gs_character *dest,
                               struct gs_packet_create_char_request *src)
{
    assert(dest);
    assert(src);

    macro_l2_str_arr_to_char_arr(dest->name, src->name);

    dest->race       = src->race;
    dest->_class     = src->_class;
    dest->sex        = src->sex;
    dest->face       = src->face;
    dest->hair_color = src->hair_color;
    dest->hair_style = src->hair_style;
    dest->stats.con  = src->con;
    dest->stats.dex  = src->dex;
    dest->stats.men  = src->men;
    dest->stats.str  = src->str;
    dest->stats.wit  = src->wit;
    dest->stats._int = src->_int;

    // Talking Island
    dest->position.x = -83968;
    dest->position.y = 244634;
    dest->position.z = -3730;

    dest->level                           = 1;
    dest->exp                             = 50;
    dest->sp                              = 10;
    dest->stats.hp                        = 400;
    dest->stats.mp                        = 400;
    dest->stats.cp                        = 200;
    dest->stats.max_hp                    = 400;
    dest->stats.max_mp                    = 400;
    dest->stats.max_cp                    = 200;
    dest->stats.p_attack                  = 42;
    dest->stats.m_attack                  = 42;
    dest->stats.p_def                     = 42;
    dest->stats.m_def                     = 42;
    dest->stats.evasion_rate              = 2;
    dest->stats.accuracy                  = 2;
    dest->stats.critical_hit              = 2;
    dest->stats.run_speed                 = 120;
    dest->stats.walk_speed                = 100;
    dest->stats.p_attack_speed            = 20;
    dest->stats.m_attack_speed            = 20;
    dest->stats.movement_speed_multiplier = 1;
    dest->stats.attack_speed_multiplier   = 1;
    dest->collision_radius                = 9;
    dest->collision_height                = 23;
    dest->max_load                        = 42;
    dest->name_color                      = 0xFFFFFF;
}

static void gs_character_set_npc_info(struct gs_packet_npc_info *dest,
                                      struct gs_character *src)
{
    assert(dest);
    assert(src);
    assert(src->id);
    assert(src->template_id);

    macro_l2_str_arr_from_char_arr(dest->name, src->name);
    macro_l2_str_arr_from_char_arr(dest->title, src->title);

    dest->id                  = src->id;
    dest->template_id         = src->template_id + 1000000;
    dest->attackable          = 1;
    dest->x                   = src->position.x;
    dest->y                   = src->position.y;
    dest->z                   = src->position.z;
    dest->heading             = src->heading;
    dest->m_attack_speed      = src->stats.m_attack_speed;
    dest->p_attack_speed      = src->stats.p_attack_speed;
    dest->run_speed           = src->stats.run_speed;
    dest->walk_speed          = src->stats.walk_speed;
    dest->swim_run_speed      = src->stats.run_speed;
    dest->swim_walk_speed     = src->stats.walk_speed;
    dest->fly_run_speed       = src->stats.run_speed;
    dest->fly_walk_speed      = src->stats.walk_speed;
    dest->magic_multiplier    = 1;
    dest->movement_multiplier = src->stats.movement_speed_multiplier;
    dest->collision_radius    = src->collision_radius;
    dest->collision_height    = src->collision_height;
    dest->r_hand              = 0;
    dest->l_hand              = 0;
    dest->name_above_char     = 1;
    dest->running             = 0;
    dest->in_combat           = 0;
    dest->alike_dead          = src->stats.hp == 0;
    dest->summoned            = 0;
    dest->karma               = 0;
    dest->abnormal_effect     = 0;
    dest->team_circle         = 0;
}

static void gs_character_set_player_info(struct gs_packet_char_info *dest,
                                         struct gs_character *src)
{
    assert(dest);
    assert(src);

    macro_util_set_arr_zero(dest->name);
    macro_util_set_arr_zero(dest->title);

    macro_l2_str_arr_from_char_arr(dest->name, src->name);
    macro_l2_str_arr_from_char_arr(dest->title, src->title);

    dest->x                       = src->position.x;
    dest->y                       = src->position.y;
    dest->z                       = src->position.z;
    dest->heading                 = src->heading;
    dest->id                      = src->id;
    dest->race_id                 = src->race;
    dest->sex                     = src->sex;
    dest->class_id                = src->_class;
    dest->pvp_flag                = 0;
    dest->karma                   = 0;
    dest->m_attack_speed          = src->stats.m_attack_speed;
    dest->p_attack_speed          = src->stats.p_attack_speed;
    dest->run_speed               = src->stats.run_speed;
    dest->walk_speed              = src->stats.walk_speed;
    dest->swim_run_speed          = src->stats.run_speed;
    dest->swim_walk_speed         = src->stats.walk_speed;
    dest->fly_run_speed           = src->stats.run_speed;
    dest->fly_walk_speed          = src->stats.walk_speed;
    dest->movement_multiplier     = src->stats.movement_speed_multiplier;
    dest->attack_speed_multiplier = src->stats.attack_speed_multiplier;
    dest->collision_radius        = src->collision_radius;
    dest->collision_height        = src->collision_height;
    dest->hair_style              = src->hair_style;
    dest->hair_color              = src->hair_color;
    dest->face                    = src->face;
    dest->clan_id                 = 0;
    dest->clan_crest_id           = 0;
    dest->ally_id                 = 0;
    dest->ally_crest_id           = 0;
    dest->standing                = 1;
    dest->running                 = 1;
    dest->in_combat               = 0;
    dest->alike_dead              = 0;
    dest->invisible               = 0;
    dest->mount_type              = 0;
    dest->private_store_type      = 0;
    dest->cubics                  = 0;
    dest->find_party_member       = 0;
    dest->abnormal_effect         = 0;
    dest->recommendations_left    = 0;
    dest->mounted                 = 0;
    dest->clan_crest_large_id     = 0;
    dest->hero_symbol             = 0;
    dest->hero_aura               = 0;
    dest->fishing                 = 0;
    dest->name_color              = 0xFFFFFF;
}

void gs_character_spawn(struct gs_state *gs, struct gs_character *spawning)
{
    static struct gs_packet_char_info char_info = { 0 };
    static struct gs_packet_npc_info npc_info   = { 0 };

    static packet_t response[512] = { 0 };

    struct gs_character *character = 0;

    u32_t id = 0;

    int already_in_list = 0;

    assert(gs);
    assert(spawning);
    assert(spawning->id);

    id = spawning->id;

    log_normal("spawning character with id %d. notifying close players.", id);

    macro_gs_character_each(character, gs)
    {
        if (character->id == spawning->id) {
            already_in_list = 1;
            continue;
        }

        // Notify player in the world of the new spawning character.
        macro_util_set_arr_zero(response);

        if (gs_character_is_npc(spawning)) {
            npc_info = (struct gs_packet_npc_info){ 0 };
            gs_character_set_npc_info(&npc_info, spawning);
            gs_packet_npc_info_pack(response, &npc_info);
        } else {
            char_info = (struct gs_packet_char_info){ 0 };
            gs_character_set_player_info(&char_info, spawning);
            gs_packet_char_info_pack(response, &char_info);
        }

        gs_character_encrypt_and_send_packet(gs, character, response);

        // Notify the spawning character of characters already
        // in the world (only if it's a player).
        if (gs_character_is_npc(spawning)) {
            continue;
        }

        macro_util_set_arr_zero(response);

        if (gs_character_is_npc(character)) {
            npc_info = (struct gs_packet_npc_info){ 0 };
            gs_character_set_npc_info(&npc_info, character);
            gs_packet_npc_info_pack(response, &npc_info);
        } else {
            char_info = (struct gs_packet_char_info){ 0 };
            gs_character_set_player_info(&char_info, character);
            gs_packet_char_info_pack(response, &char_info);
        }

        gs_character_encrypt_and_send_packet(gs, spawning, response);
    }

    if (already_in_list) {
        return;
    }

    gs_character_add(gs, spawning);
}

void gs_character_restart(struct gs_state *gs, struct gs_character *character)
{
    struct gs_packet_restart restart = { 0 };

    packet_t response[32] = { 0 };

    assert(gs);
    assert(character);

    restart.response = 0x01; // ok!

    gs_packet_restart_pack(response, &restart);
    gs_character_encrypt_and_send_packet(gs, character, response);

    gs_character_disconnect(gs, character);
}

void gs_character_show_npc_html_message(struct gs_state *gs,
                                        struct gs_character *character,
                                        char *message,
                                        size_t message_size)
{
    static struct gs_packet_npc_html_message html_message = { 0 };

    static packet_t response[16384] = { 0 };

    assert(gs);
    assert(character);
    assert(message);

    html_message = (struct gs_packet_npc_html_message){ 0 };

    macro_util_set_arr_zero(response);

    html_message.message_id = 1;

    l2_string_from_char(html_message.message,
                        message,
                        sizeof(html_message.message),
                        message_size,
                        message_size);

    gs_packet_npc_html_message_pack(response, &html_message);
    gs_character_encrypt_and_send_packet(gs, character, response);
}

static void gs_character_broadcast_move_type(struct gs_state *gs,
                                             struct gs_character *src)
{
    struct gs_packet_change_move_type move_type = { 0 };

    packet_t response[32] = { 0 };

    assert(gs);
    assert(src);

    move_type.obj_id  = src->id;
    move_type.running = (u32_t) src->running;

    gs_packet_change_move_type_pack(response, &move_type);
    gs_character_broadcast_packet(gs, src, response);
}

void gs_character_walk(struct gs_state *gs, struct gs_character *src)
{
    assert(gs);
    assert(src);
    src->running = 0;
    gs_character_broadcast_move_type(gs, src);
}

void gs_character_run(struct gs_state *gs, struct gs_character *src)
{
    assert(gs);
    assert(src);
    src->running = 1;
    gs_character_broadcast_move_type(gs, src);
}

struct gs_character *gs_character_from_session(struct gs_state *gs,
                                               struct gs_session *session)
{
    struct gs_character *character = 0;

    assert(gs);
    assert(session);

    macro_gs_character_each(character, gs)
    {
        if (character->session == session) {
            return character;
        }
    }

    return 0;
}

u32_t gs_character_get_free_id(struct gs_state *gs)
{
    assert(gs);

    // Don't use id 0, it causes issues with packets
    // sent to the client.
    for (u64_t i = 1, max = macro_util_arr_len(gs->characters); i < max;
         i += 1) {
        if (!gs->characters[i].id) {
            return (u32_t) i;
        }
    }

    return 0;
}

void gs_character_add(struct gs_state *gs, struct gs_character *src)
{
    assert(gs);
    assert(src);
    assert(src->id);

    gs->characters[src->id] = *src;
    util_list_add(gs->list_characters,
                  macro_util_arr_len(gs->list_characters),
                  &gs->characters[src->id]);
}

void gs_character_disconnect(struct gs_state *gs, struct gs_character *src)
{
    assert(gs);
    assert(src);

    util_list_remove(gs->list_characters, src);
    *src = (struct gs_character){ 0 };
}
