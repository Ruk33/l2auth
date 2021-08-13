#include <assert.h>
#include <math.h>
#include "include/config.h"
#include "include/util.h"
#include "include/list.h"
#include "include/recycle_id.h"
#include "include/gs_types.h"
#include "include/log.h"
#include "include/packet.h"
#include "include/conn.h"
#include "include/l2_string.h"
#include "include/gs_session.h"
#include "include/gs_random_id.h"
#include "include/gs_packet_create_char_request.h"
#include "include/gs_packet_revive_request.h"
#include "include/gs_packet_move.h"
#include "include/gs_packet_validate_pos.h"
#include "include/gs_packet_char_info.h"
#include "include/gs_packet_npc_info.h"
#include "include/gs_packet_target_selected.h"
#include "include/gs_packet_auto_attack.h"
#include "include/gs_packet_attack.h"
#include "include/gs_packet_revive.h"
#include "include/gs_packet_status.h"
#include "include/gs_packet_restart.h"
#include "include/gs_packet_die.h"
#include "include/gs_character.h"

#define gs_character_each(character, state) \
        list_each(struct gs_character, character, state->list_characters)

static int gs_character_is_npc(struct gs_character *src)
{
        assert(src);
        return src->session ? 0 : 1;
}

static double
gs_character_angle_to_point(struct gs_character *src, struct gs_point *p)
{
        assert(src);
        assert(p);
        return atan2(p->y - src->position.y, p->x - src->position.x);
}

static double
gs_character_distance_to_point(struct gs_character *src, struct gs_point *p)
{
        double dx = 0;
        double dy = 0;
        double dz = 0;

        assert(src);
        assert(p);

        dx = p->x - src->position.x;
        dy = p->y - src->position.y;
        dz = p->z - src->position.z;

        return sqrt(dx * dx + dy * dy + dz * dz);
}

static double
gs_character_distance(struct gs_character *a, struct gs_character *b)
{
        assert(a);
        assert(b);
        return gs_character_distance_to_point(a, &b->position);
}

static struct gs_character *
gs_character_find_by_id(struct gs_state *state, u32_t id)
{
        struct gs_character *character = 0;

        assert(state);

        gs_character_each(character, state)
        {
                if (character->id == id) {
                        return character;
                }
        }

        return 0;
}

static void
gs_character_encrypt_and_send_packet(struct gs_character *from, packet_t *packet)
{
        assert(from);
        assert(packet);

        if (gs_character_is_npc(from)) {
                return;
        }

        gs_session_encrypt(from->session, packet, packet);
        conn_send_packet(from->session->socket, packet);
}

// Todo: do we really need from?
static void gs_character_broadcast_packet(
        struct gs_state *state,
        struct gs_character *from,
        packet_t *packet)
{
        static packet_t response[65536] = { 0 };

        struct gs_character *character = 0;

        size_t safe_packet_size = 0;

        assert(state);
        assert(from);
        assert(packet);

        // This way maybe we can clear less space than what we really need.
        safe_packet_size = (size_t)(packet_size(packet) * 2);
        safe_packet_size = _min(sizeof(response), safe_packet_size);

        gs_character_each(character, state)
        {
                if (gs_character_is_npc(character)) {
                        continue;
                }

                bytes_zero(response, safe_packet_size);
                bytes_cpy(response, packet, (size_t) packet_size(packet));
                gs_character_encrypt_and_send_packet(character, response);
        }
}

static void
gs_character_send_status(struct gs_character *from, struct gs_character *to)
{
        static packet_t response[512] = { 0 };

        struct gs_packet_status status = { 0 };

        struct gs_packet_status_attr attr = { 0 };

        assert(from);
        assert(to);

        if (gs_character_is_npc(to)) {
                return;
        }

        bytes_zero(response, sizeof(response));

        status.obj_id = from->id;

        attr.type  = STATUS_CUR_HP;
        attr.value = (i32_t) from->stats.hp;
        gs_packet_status_add(&status, &attr);

        attr.type  = STATUS_MAX_HP;
        attr.value = (i32_t) from->stats.max_hp;
        gs_packet_status_add(&status, &attr);

        gs_packet_status_pack(response, &status);
        gs_character_encrypt_and_send_packet(to, response);
}

static void gs_character_revive(
        struct gs_state *state,
        struct gs_character *src,
        enum gs_packet_revive_request_option where)
{
        struct gs_packet_revive revive = { 0 };

        packet_t response[32] = { 0 };

        assert(state);
        assert(src);

        revive.obj_id = src->id;
        gs_packet_revive_pack(response, &revive);
        gs_character_broadcast_packet(state, src, response);

        switch (where) {
        case REVIVE_IN_CLAN_HALL:
        case REVIVE_IN_CASTLE:
        case REVIVE_IN_SIEGE_HQ:
        case REVIVE_IN_FIXED:
        default:
                break;
        }
}

static void gs_character_move(
        struct gs_state *state,
        struct gs_character *character,
        struct gs_point *p)
{
        gs_packet_move_t move_response = { 0 };

        packet_t packet[64] = { 0 };

        assert(state);
        assert(character);
        assert(p);

        gs_packet_move(&move_response, character);
        gs_packet_move_pack(packet, &move_response);
        gs_character_broadcast_packet(state, character, packet);
}

static void gs_character_die(struct gs_state *state, struct gs_character *src)
{
        struct gs_packet_die die = { 0 };

        packet_t response[64] = { 0 };

        assert(state);
        assert(src);

        gs_packet_die_add_options(&die, src);
        gs_packet_die_pack(response, &die);
        gs_character_broadcast_packet(state, src, response);
}

static void gs_character_attack(
        struct gs_state *state,
        struct gs_character *attacker,
        struct gs_character *target)
{
        gs_packet_auto_attack_t auto_attack = { 0 };

        gs_packet_attack_t attack  = { 0 };
        gs_packet_attack_hit_t hit = { 0 };

        packet_t auto_attack_packet[32] = { 0 };
        packet_t attack_packet[256]     = { 0 };

        double d = 0;
        double a = 0;
        i32_t x  = 0;
        i32_t y  = 0;

        int was_already_dead = 0;

        assert(state);
        assert(attacker);
        assert(target);

        hit.damage    = attacker->stats.p_attack;
        hit.target_id = target->id;

        // todo: implement properly.
        was_already_dead = target->stats.hp == 0;
        target->stats.hp = target->stats.hp > 30 ? target->stats.hp - 30 : 0;

        gs_packet_attack_set_attacker(&attack, attacker);
        gs_packet_attack_add_hit(&attack, &hit);

        gs_packet_auto_attack_pack(auto_attack_packet, &auto_attack);
        gs_packet_attack_pack(attack_packet, &attack);

        gs_character_broadcast_packet(state, attacker, auto_attack_packet);
        gs_character_broadcast_packet(state, attacker, attack_packet);

        // Allow to hit dead bodies. But since they
        // were already dead, we don't need to re-send
        // the life or dead packet.
        if (was_already_dead) {
                return;
        }

        // When the life of the target gets modified
        // make sure the attacker and the target
        // are notified & updated.
        gs_character_send_status(target, target);
        gs_character_send_status(target, attacker);

        if (target->stats.hp == 0) {
                gs_character_die(state, target);
        }
}

static void gs_character_select_target(
        struct gs_character *character,
        struct gs_character *target)
{
        gs_packet_target_selected_t selected = { 0 };

        packet_t response[32] = { 0 };

        assert(character);
        assert(target);

        selected.target_id = target->id;
        selected.color     = 0;

        gs_packet_target_selected_pack(response, &selected);
        gs_character_encrypt_and_send_packet(character, response);
}

static void gs_character_validate_position(struct gs_character *character)
{
        gs_packet_validate_pos_t validate_response = { 0 };

        packet_t response[64] = { 0 };

        assert(character);

        gs_packet_validate_pos(&validate_response, character);
        gs_packet_validate_pos_pack(response, &validate_response);
        gs_character_encrypt_and_send_packet(character, response);
}

static void gs_character_spawn_random_orc(struct gs_state *state)
{
        struct gs_character orc = { 0 };

        assert(state);

        orc.id                   = gs_character_get_free_id(state);
        orc.position.x           = -84023;
        orc.position.y           = 244598;
        orc.position.z           = -3730;
        orc.collision_radius     = 14;
        orc.collision_height     = 25;
        orc.level                = 8;
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

        bytes_cpy_str(orc.name, "Orc", sizeof(orc.name) - 1);
        bytes_cpy_str(orc.title, "Archer", sizeof(orc.title) - 1);

        gs_character_spawn(state, &orc);
}

static void gs_character_from_request(
        struct gs_character *dest,
        gs_packet_create_char_request_t *src)
{
        assert(dest);
        assert(src);

        l2_string_to_char(dest->name, src->name, sizeof(dest->name));

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

static void
gs_character_spawn(struct gs_state *state, struct gs_character *spawning)
{
        static gs_packet_char_info_t char_info = { 0 };
        static gs_packet_npc_info_t npc_info   = { 0 };

        static packet_t response[512] = { 0 };

        struct gs_character *character = 0;

        u32_t id = 0;

        int already_in_list = 0;

        assert(state);
        assert(spawning);
        assert(spawning->id);

        id = spawning->id;

        log("spawning character with id %d. notifying close players.", id);

        gs_character_each(character, state)
        {
                if (character->id == spawning->id) {
                        already_in_list = 1;
                        continue;
                }

                // Notify player in the world of the new spawning character.
                bytes_zero(response, sizeof(response));

                if (gs_character_is_npc(spawning)) {
                        bytes_zero((byte_t *) &npc_info, sizeof(npc_info));
                        gs_packet_npc_info(&npc_info, spawning);
                        gs_packet_npc_info_pack(response, &npc_info);
                } else {
                        bytes_zero((byte_t *) &char_info, sizeof(char_info));
                        gs_packet_char_info(&char_info, spawning);
                        gs_packet_char_info_pack(response, &char_info);
                }

                gs_character_encrypt_and_send_packet(character, response);

                // Notify the spawning character of characters already
                // in the world (only if it's a player).
                if (gs_character_is_npc(spawning)) {
                        continue;
                }

                bytes_zero(response, sizeof(response));

                if (gs_character_is_npc(character)) {
                        bytes_zero((byte_t *) &npc_info, sizeof(npc_info));
                        gs_packet_npc_info(&npc_info, character);
                        gs_packet_npc_info_pack(response, &npc_info);
                } else {
                        bytes_zero((byte_t *) &char_info, sizeof(char_info));
                        gs_packet_char_info(&char_info, character);
                        gs_packet_char_info_pack(response, &char_info);
                }

                gs_character_encrypt_and_send_packet(spawning, response);
        }

        if (already_in_list) {
                return;
        }

        gs_character_add(state, spawning);
}

static void
gs_character_restart(struct gs_state *state, struct gs_character *character)
{
        gs_packet_restart_t restart = { 0 };

        packet_t response[32] = { 0 };

        assert(state);
        assert(character);

        restart.response = 0x01; // ok!

        gs_packet_restart_pack(response, &restart);
        gs_character_encrypt_and_send_packet(character, response);

        gs_character_disconnect(state, character);
}

static struct gs_character *
gs_character_from_session(struct gs_state *state, struct gs_session *session)
{
        struct gs_character *character = 0;

        assert(state);
        assert(session);

        gs_character_each(character, state)
        {
                if (character->session == session) {
                        return character;
                }
        }

        return 0;
}

static u32_t gs_character_get_free_id(struct gs_state *state)
{
        assert(state);

        // Don't use id 0, it causes issues with packets
        // sent to the client.
        for (size_t i = 1, max = arr_size(state->characters); i < max; i += 1) {
                if (!state->characters[i].id) {
                        return (u32_t) i;
                }
        }

        return 0;
}

static void gs_character_add(struct gs_state *state, struct gs_character *src)
{
        assert(state);
        assert(src);
        assert(src->id);

        state->characters[src->id] = *src;
        list_add(state->list_characters, &state->characters[src->id]);
}

static void
gs_character_disconnect(struct gs_state *state, struct gs_character *src)
{
        assert(state);
        assert(src);

        list_remove(state->list_characters, src);
        *src = (struct gs_character){ 0 };
}
