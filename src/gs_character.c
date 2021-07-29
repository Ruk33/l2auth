#include <assert.h>
#include <math.h>
#include "include/config.h"
#include "include/util.h"
#include "include/log.h"
#include "include/packet.h"
#include "include/conn.h"
#include "include/l2_string.h"
#include "include/gs_session.h"
#include "include/gs_random_id.h"
#include "include/gs_packet_create_char_request.h"
#include "include/gs_packet_move.h"
#include "include/gs_packet_move_request.h"
#include "include/gs_packet_validate_pos.h"
#include "include/gs_packet_validate_pos_request.h"
#include "include/gs_packet_char_info.h"
#include "include/gs_packet_npc_info.h"
#include "include/gs_packet_action_request.h"
#include "include/gs_packet_target_selected.h"
#include "include/gs_packet_auto_attack.h"
#include "include/gs_packet_attack.h"
#include "include/gs_character.h"

static gs_character_t *characters = 0;
static size_t *character_count    = 0;

static packet_t response[65536] = { 0 };

static int is_npc(gs_character_t *src)
{
        assert(src);
        return src->session ? 0 : 1;
}

static double distance_from_point(gs_character_t *a, i32_t x, i32_t y, i32_t z)
{
        double dx = 0;
        double dy = 0;
        double dz = 0;

        assert(a);

        dx = x - a->x;
        dy = y - a->y;
        dz = z - a->z;

        return sqrt(dx * dx + dy * dy + dz * dz);
}

static double distance(gs_character_t *a, gs_character_t *b)
{
        assert(a);
        assert(b);
        return distance_from_point(a, b->x, b->y, b->z);
}

static gs_character_t *find_by_id(u32_t id)
{
        for (size_t i = 0; i < *character_count; i += 1) {
                if (characters[i].id == id) {
                        return &characters[i];
                }
        }

        return 0;
}

static void encrypt_and_send_packet(gs_character_t *from, packet_t *packet)
{
        assert(from);
        assert(packet);

        if (is_npc(from)) {
                return;
        }

        gs_session_encrypt(from->session, packet, packet);
        conn_send_packet(from->session->socket, packet);
}

static void move(gs_character_t *character, i32_t x, i32_t y, i32_t z)
{
        gs_packet_move_t move_response = { 0 };

        assert(character);

        character->target_x = x;
        character->target_y = y;
        character->target_z = z;
        character->state    = MOVING;

        gs_packet_move(&move_response, character);

        for (size_t i = 0; i < *character_count; i += 1) {
                bytes_zero(response, sizeof(response));
                gs_packet_move_pack(response, &move_response);
                encrypt_and_send_packet(&characters[i], response);
        }
}

static void on_npc_attacked(gs_character_t *npc, gs_character_t *attacker)
{
        assert(npc);
        assert(attacker);

        npc->state     = ATTACKING;
        npc->target_id = attacker->id;
}

static void attack(gs_character_t *attacker, gs_character_t *target)
{
        gs_packet_auto_attack_t auto_attack = { 0 };

        gs_packet_attack_t attack  = { 0 };
        gs_packet_attack_hit_t hit = { 0 };

        double d = 0;
        double a = 0;
        i32_t x  = 0;
        i32_t y  = 0;

        assert(attacker);

        if (!target) {
                attacker->target_id = 0;
                attacker->state     = IDLE;
                return;
        }

        attacker->target_id = target->id;
        attacker->state     = ATTACKING;

        d = distance(attacker, target);

        if (d > 30) {
                a = atan2(target->y - attacker->y, target->x - attacker->x);
                x = target->x - 20 * cos(a);
                y = target->y - 20 * sin(a);
                move(attacker, x, y, target->z);
                attacker->state = MOVING_TO_ATTACK;
                return;
        }

        if (attacker->attack_cd > 0) {
                return;
        }

        if (is_npc(target)) {
                on_npc_attacked(target, attacker);
        }

        attacker->attack_cd = 2;

        auto_attack.target_id = target->id;

        hit.damage    = attacker->p_attack;
        hit.target_id = target->id;
        gs_packet_attack_set_attacker(&attack, attacker);
        gs_packet_attack_add_hit(&attack, &hit);

        for (size_t i = 0; i < *character_count; i += 1) {
                bytes_zero(response, sizeof(response));
                gs_packet_auto_attack_pack(response, &auto_attack);
                encrypt_and_send_packet(&characters[i], response);

                bytes_zero(response, sizeof(response));
                gs_packet_attack_pack(response, &attack);
                encrypt_and_send_packet(&characters[i], response);
        }
}

static void select_target(gs_character_t *character, gs_character_t *target)
{
        gs_packet_target_selected_t selected = { 0 };

        assert(character);
        assert(target);

        character->target_id = target->id;
        character->state     = TARGET_SELECTED;

        selected.target_id = target->id;
        selected.color     = 0;

        bytes_zero(response, sizeof(response));
        gs_packet_target_selected_pack(response, &selected);
        encrypt_and_send_packet(character, response);
}

static void handle_move_request(gs_character_t *character, packet_t *packet)
{
        gs_packet_move_request_t move_request = { 0 };

        assert(character);
        assert(packet);

        gs_packet_move_request_unpack(&move_request, packet);
        move(character, move_request.x, move_request.y, move_request.z);
}

static void handle_val_pos_request(gs_character_t *character, packet_t *packet)
{
        gs_packet_validate_pos_request_t validate_request = { 0 };
        gs_packet_validate_pos_t validate_response        = { 0 };

        double a = 0;

        assert(character);
        assert(packet);

        gs_packet_validate_pos_request_unpack(&validate_request, packet);

        a =
                atan2(character->target_y - character->y,
                      character->target_x - character->x);
        log("old x: %d - old y: %d", character->x, character->y);
        log("new x: %d - new y: %d", validate_request.x, validate_request.y);
        log("my  x: %d - my y : %d",
            (i32_t)(character->x + 120 * sin(a)),
            (i32_t)(character->y + 120 * cos(a)));

        // If it's close enough to current location, update. If not, don't.
        // character->x = validate_request.x;
        // character->y = validate_request.y;

        // todo: can we calculate z ourselves without relying on the client?
        character->z = validate_request.z;
        // character->heading = validate_request.heading;

        bytes_zero(response, sizeof(response));
        gs_packet_validate_pos(&validate_response, character);
        gs_packet_validate_pos_pack(response, &validate_response);
        encrypt_and_send_packet(character, response);
}

static void handle_action_request(gs_character_t *character, packet_t *packet)
{
        gs_packet_action_request_t action = { 0 };

        gs_character_t *target = 0;

        assert(character);
        assert(packet);

        gs_packet_action_request_unpack(&action, packet);

        target = find_by_id(action.target_id);

        if (!target) {
                log("selected target %d not found, ignoring", action.target_id);
                return;
        }

        select_target(character, target);
}

static void handle_attack_request(gs_character_t *character, packet_t *packet)
{
        gs_packet_action_request_t action = { 0 };

        gs_character_t *target = 0;

        assert(character);
        assert(packet);

        gs_packet_action_request_unpack(&action, packet);

        target = find_by_id(action.target_id);

        if (!target) {
                log("attacking %d not found, ignoring", action.target_id);
                return;
        }

        attack(character, target);
}

static void handle_tick(gs_character_t *character, double delta)
{
        gs_character_t *target = 0;

        double d = 0;

        double dx            = 0;
        double dy            = 0;
        double _sin          = 0;
        double _cos          = 0;
        double x_speed_ticks = 0;
        double y_speed_ticks = 0;

        assert(character);
        PREVENT_UNUSED_WARNING(delta);

        character->attack_cd -= character->attack_cd ? 1 : 0;

        if (character->target_id) {
                target = find_by_id(character->target_id);
        }

        switch (character->state) {
        case SPAWN:
                // log("tick: spawn state");
                break;
        case IDLE:
                // log("tick: idle state");
                break;
        case MOVING:
                dx            = character->target_x - character->x;
                dy            = character->target_y - character->y;
                d             = sqrt(dx * dx + dy * dy);
                _cos          = dx / d;
                _sin          = dy / d;
                x_speed_ticks = _cos * 60;
                y_speed_ticks = _sin * 60;

                character->heading =
                        atan2(-_sin, -_cos) * 10430.378350470452724949566316381;
                character->heading += 32768;
                character->x += x_speed_ticks;
                character->y += y_speed_ticks;

                // a =
                //         atan2(character->target_y - character->y,
                //               character->target_x - character->x);
                // character->x = character->x + 60 * cos(a);
                // character->y = character->y + 60 * sin(a);

                // d = distance_from_point(
                //         character,
                //         character->target_x,
                //         character->target_y,
                //         character->target_z);
                // log("moving, distance: %f", d);

                if (d < 20) {
                        character->x     = character->target_x;
                        character->y     = character->target_y;
                        character->state = IDLE;
                }

                // log("tick: moving state");
                break;
        case TARGET_SELECTED:
                // log("tick: selected target");
                break;
        case ATTACKING:
                // log("tick: attacking state");
                attack(character, target);
                break;
        case MOVING_TO_ATTACK:
                dx            = character->target_x - character->x;
                dy            = character->target_y - character->y;
                d             = sqrt(dx * dx + dy * dy);
                _cos          = dx / d;
                _sin          = dy / d;
                x_speed_ticks = _cos * 60;
                y_speed_ticks = _sin * 60;

                character->heading =
                        atan2(-_sin, -_cos) * 10430.378350470452724949566316381;
                character->heading += 32768;
                character->x += x_speed_ticks;
                character->y += y_speed_ticks;

                // log("tick: moving to attack state");
                attack(character, target);
                break;
        default:
                break;
        }
}

static void spawn_random_orc(void)
{
        gs_character_t orc = { 0 };

        gs_random_id(&orc.id);

        orc.x                = -84023;
        orc.y                = 244598;
        orc.z                = -3730;
        orc.collision_radius = 14;
        orc.collision_height = 25;
        orc.level            = 8;
        orc.sex              = 0;
        orc.hp               = 197;
        orc.mp               = 102;
        orc.str              = 40;
        orc.con              = 43;
        orc.dex              = 30;
        orc._int             = 21;
        orc.wit              = 20;
        orc.men              = 10;
        orc.exp              = 293;
        orc.sp               = 10;
        orc.p_attack         = 41;
        orc.p_def            = 55;
        orc.m_attack         = 6;
        orc.m_def            = 45;
        orc.p_attack_speed   = 249;
        orc.m_attack_speed   = 227;
        orc.walk_speed       = 45;
        orc.run_speed        = 110;

        bytes_cpy_str(orc.name, "Orc", sizeof(orc.name) - 1);
        bytes_cpy_str(orc.title, "Archer", sizeof(orc.title) - 1);

        gs_character_spawn(&orc);
}

static void idle_state(gs_character_t *character, packet_t *packet)
{
        assert(character);
        assert(packet);

        switch (packet_type(packet)) {
        case 0x01: // Move backwards.
                handle_move_request(character, packet);
                break;
        case 0x04: // Action.
                handle_action_request(character, packet);
                break;
        case 0x09: // Logout.
                log("TODO: Logout");
                break;
        case 0x38: // Say.
                log("TODO: Say");
                break;
        case 0x46: // Restart.
                log("TODO: Restart");
                spawn_random_orc();
                break;
        case 0x48: // Validate position.
                handle_val_pos_request(character, packet);
                break;
        case 0xcd: // Show map.
                log("TODO: Show map");
                break;
        default:
                log("can't handle packet from in world state.");
                break;
        }
}

static void target_selected_state(gs_character_t *character, packet_t *packet)
{
        assert(character);
        assert(packet);

        switch (packet_type(packet)) {
        case 0x04: // Action.
                handle_attack_request(character, packet);
                break;
        default:
                idle_state(character, packet);
                break;
        }
}

void gs_character_set(gs_character_t *src, size_t *count)
{
        characters      = src;
        character_count = count;
}

void gs_character_from_request(
        gs_character_t *dest,
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
        dest->con        = src->con;
        dest->dex        = src->dex;
        dest->men        = src->men;
        dest->str        = src->str;
        dest->wit        = src->wit;
        dest->_int       = src->_int;

        // Talking Island
        dest->x = -83968;
        dest->y = 244634;
        dest->z = -3730;

        dest->level                     = 1;
        dest->exp                       = 50;
        dest->sp                        = 10;
        dest->hp                        = 400;
        dest->mp                        = 400;
        dest->cp                        = 200;
        dest->max_hp                    = 400;
        dest->max_mp                    = 400;
        dest->max_cp                    = 200;
        dest->p_attack                  = 42;
        dest->m_attack                  = 42;
        dest->p_def                     = 42;
        dest->m_def                     = 42;
        dest->evasion_rate              = 2;
        dest->accuracy                  = 2;
        dest->critical_hit              = 2;
        dest->run_speed                 = 120;
        dest->walk_speed                = 100;
        dest->p_attack_speed            = 20;
        dest->m_attack_speed            = 20;
        dest->movement_speed_multiplier = 1;
        dest->attack_speed_multiplier   = 1;
        dest->collision_radius          = 9;
        dest->collision_height          = 23;
        dest->max_load                  = 42;
        dest->name_color                = 0xFFFFFF;
}

void gs_character_spawn(gs_character_t *src)
{
        // Todo: maybe rename to player_info?
        static gs_packet_char_info_t char_info = { 0 };

        static gs_packet_npc_info_t npc_info = { 0 };

        assert(src);
        assert(src->id);

        log("Spawning and notifying close players.");

        for (size_t i = 0; i < *character_count; i += 1) {
                // Notify player in the world of the new spawning character.
                bytes_zero(response, sizeof(response));

                if (is_npc(src)) {
                        bytes_zero((byte_t *) &npc_info, sizeof(npc_info));
                        gs_packet_npc_info(&npc_info, src);
                        gs_packet_npc_info_pack(response, &npc_info);
                } else {
                        bytes_zero((byte_t *) &char_info, sizeof(char_info));
                        gs_packet_char_info(&char_info, src);
                        gs_packet_char_info_pack(response, &char_info);
                }

                encrypt_and_send_packet(&characters[i], response);

                // Notify the spawning character of characters already
                // in the world (only if it's a player).
                if (is_npc(src)) {
                        continue;
                }

                bytes_zero(response, sizeof(response));

                if (is_npc(&characters[i])) {
                        bytes_zero((byte_t *) &npc_info, sizeof(npc_info));
                        gs_packet_npc_info(&npc_info, &characters[i]);
                        gs_packet_npc_info_pack(response, &npc_info);
                } else {
                        bytes_zero((byte_t *) &char_info, sizeof(char_info));
                        gs_packet_char_info(&char_info, &characters[i]);
                        gs_packet_char_info_pack(response, &char_info);
                }

                encrypt_and_send_packet(src, response);
        }

        characters[*character_count]       = *src;
        characters[*character_count].state = SPAWN;

        *character_count += 1;
}

gs_character_t *gs_character_from_session(gs_session_t *session)
{
        assert(session);

        for (size_t i = 0; i < *character_count; i += 1) {
                if (characters[i].session == session) {
                        return &characters[i];
                }
        }

        return 0;
}

void gs_character_tick(double delta)
{
        for (size_t i = 0; i < *character_count; i += 1) {
                handle_tick(&characters[i], delta);
        }
}

void gs_character_request(gs_character_t *character, packet_t *packet)
{
        assert(character);
        assert(packet);

        switch (character->state) {
        case SPAWN:
        case IDLE:
        case MOVING:
                idle_state(character, packet);
                break;
        case TARGET_SELECTED:
        case ATTACKING:
        case MOVING_TO_ATTACK:
                target_selected_state(character, packet);
                break;
        default:
                break;
        }
}
