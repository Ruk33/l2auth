#include <assert.h>
#include <stdlib.h>
#include <math.h>
#include "include/config.h"
#include "include/packet.h"
#include "include/gs_types.h"
#include "include/gs_character.h"
#include "include/gs_client_packets.h"
#include "include/gs_ai.h"

static i32_t gs_ai_random_number(i32_t a, i32_t b)
{
        return rand() % (b + 1 - a) + a;
}

static void
gs_ai_on_npc_attacked(struct gs_character *npc, struct gs_character *attacker)
{
        assert(npc);
        assert(attacker);
        npc->ai.state     = AI_ATTACKING;
        npc->ai.target_id = attacker->id;
}

static void
gs_ai_on_dead(struct gs_character *dead, struct gs_character *killer)
{
        assert(dead);
        assert(killer);

        dead->ai       = (struct gs_ai){ 0 };
        dead->ai.state = AI_DEAD;

        killer->ai.move_data = (struct gs_move_data){ 0 };
        killer->ai.state     = AI_IDLE;
        killer->ai.target_id = 0;
}

static void gs_ai_on_revive(struct gs_state *gs, struct gs_character *src)
{
        assert(gs);
        assert(src);

        src->ai       = (struct gs_ai){ 0 };
        src->stats.hp = src->stats.max_hp;

        gs_character_send_status(gs, src, src);
}

static void gs_ai_go_idle(struct gs_character *src)
{
        assert(src);
        src->ai.move_data = (struct gs_move_data){ 0 };
        src->ai.state     = AI_IDLE;
}

static void
gs_ai_move(struct gs_state *gs, struct gs_character *src, struct gs_point *where)
{
        double dx = 0;
        double dy = 0;
        double d  = 0;

        double _sin = 0;
        double _cos = 0;

        assert(gs);
        assert(src);
        assert(where);

        src->ai.moving_to = *where;
        src->ai.state     = AI_MOVING;

        /*
         * Todo: refactor.
         * Thanks to l2j project to provide this first iteration of the code.
         */

        dx = where->x - src->position.x;
        dy = where->y - src->position.y;
        d  = sqrt(dx * dx + dy * dy);

        _sin = dy / d;
        _cos = dx / d;
        d -= 10 /* 10 as offset */ - 5;

        src->ai.move_data.heading     = 0;
        src->ai.move_data.destination = *where;
        src->ai.move_data.ticks_to_move =
                (u32_t)(TICKS_PER_SECOND * d / /* speed */ 120);
        src->ai.move_data.x_speed_ticks =
                _cos * /* speed */ 120 / TICKS_PER_SECOND;
        src->ai.move_data.y_speed_ticks =
                _sin * /* speed */ 120 / TICKS_PER_SECOND;
        src->ai.move_data.move_start_time = gs->game_ticks;

        src->ai.move_data.origin = src->position;

        if (src->ai.move_data.ticks_to_move < 1) {
                src->ai.move_data.ticks_to_move = 1;
        }

        src->heading = (i32_t)(
                atan2(-_sin, -_cos) * 10430.378350470452724949566316381);
        src->heading += 32768;

        gs_character_move(gs, src, where);
}

static void gs_ai_attack(
        struct gs_state *gs,
        struct gs_character *attacker,
        struct gs_character *target)
{
        struct gs_point walk_to = { 0 };
        double walk_angle       = 0;

        assert(gs);
        assert(attacker);

        if (!target) {
                attacker->ai.target_id = 0;
                gs_ai_go_idle(attacker);
                return;
        }

        attacker->ai.target_id = target->id;
        attacker->ai.state     = AI_ATTACKING;

        if (attacker->ai.attack_cd > 0) {
                return;
        }

        if (gs_character_distance(attacker, target) > 80) {
                walk_angle = gs_character_angle_to_point(
                        attacker, &target->position);
                walk_to.x = target->position.x - 40 * cos(walk_angle);
                walk_to.y = target->position.y - 40 * sin(walk_angle);
                walk_to.z = target->position.z;
                gs_ai_move(gs, attacker, &walk_to);
                attacker->ai.state = AI_MOVING_TO_ATTACK;
                return;
        }

        if (gs_character_is_npc(target)) {
                gs_ai_on_npc_attacked(target, attacker);
        }

        attacker->ai.attack_cd = 20;
        gs_character_attack(gs, attacker, target);

        if (target->stats.hp == 0) {
                gs_ai_on_dead(target, attacker);
        }
}

static void gs_ai_select_target(
        struct gs_state *gs,
        struct gs_character *src,
        struct gs_character *target)
{
        assert(gs);
        assert(src);
        assert(target);

        src->ai.state     = AI_TARGET_SELECTED;
        src->ai.target_id = target->id;

        gs_character_select_target(gs, src, target);
        gs_character_send_status(gs, target, src);
}

static void gs_ai_handle_move_request(
        struct gs_state *gs,
        struct gs_character *character,
        packet_t *packet)
{
        struct gs_packet_move_request move_request = { 0 };

        struct gs_point point = { 0 };

        assert(gs);
        assert(character);
        assert(packet);

        gs_packet_move_request_unpack(&move_request, packet);

        point.x = move_request.x;
        point.y = move_request.y;
        point.z = move_request.z;

        gs_ai_move(gs, character, &point);
}

static void gs_ai_handle_val_pos_request(
        struct gs_state *gs,
        struct gs_character *character,
        packet_t *packet)
{
        struct gs_packet_validate_pos_request validate_request = { 0 };

        struct gs_character client_position = { 0 };

        assert(gs);
        assert(character);
        assert(packet);

        gs_packet_validate_pos_request_unpack(&validate_request, packet);

        client_position            = *character;
        client_position.position.x = validate_request.x;
        client_position.position.y = validate_request.y;
        client_position.position.z = validate_request.z;

        // todo: can we calculate z ourselves without relying on the client?
        character->position.z = validate_request.z;
        character->heading    = validate_request.heading;

        // todo: refactor
        // Do approximation. If the client position isn't too far
        // away from the server's position, just don't force the update.
        if (gs_character_distance(character, &client_position) < 100) {
                // todo: double check.
                // note: is it really required to send the confirmation?
                // gs_character_validate_position(gs, &client_position);
        } else {
                gs_character_validate_position(gs, character);
        }
}

static void gs_ai_handle_action_request(
        struct gs_state *gs,
        struct gs_character *character,
        packet_t *packet)
{
        struct gs_packet_action_request action = { 0 };

        struct gs_character *target = 0;

        assert(gs);
        assert(character);
        assert(packet);

        gs_packet_action_request_unpack(&action, packet);

        target = gs_character_find_by_id(gs, action.target_id);

        if (!target) {
                character->ai.target_id = 0;
                gs_ai_go_idle(character);
                return;
        }

        gs_ai_select_target(gs, character, target);
}

static void gs_ai_handle_say(
        struct gs_state *gs,
        struct gs_character *character,
        packet_t *packet)
{
        // Todo: double check max message's length.
        static char message[256] = { 0 };

        struct gs_packet_say_request say = { 0 };

        assert(gs);
        assert(character);
        assert(packet);

        bytes_zero((byte_t *) message, sizeof(message));

        gs_packet_say_request_unpack(&say, packet);
        l2_string_to_char(message, say.message, sizeof(message));
        gs_character_say(gs, character, message);
}

static void gs_ai_handle_attack_request(
        struct gs_state *gs,
        struct gs_character *character,
        packet_t *packet)
{
        struct gs_packet_action_request action = { 0 };

        struct gs_character *target = 0;

        assert(gs);
        assert(character);
        assert(packet);

        gs_packet_action_request_unpack(&action, packet);

        if (character->id == action.target_id) {
                return;
        }

        target = gs_character_find_by_id(gs, action.target_id);

        if (!target) {
                character->ai.target_id = 0;
                gs_ai_go_idle(character);
                return;
        }

        if (character->ai.target_id == target->id) {
                gs_ai_attack(gs, character, target);
                return;
        }

        gs_ai_select_target(gs, character, target);
}

static void
gs_ai_handle_restart_request(struct gs_state *gs, struct gs_character *character)
{
        assert(gs);
        assert(character);
        gs_character_restart(gs, character);
}

static void gs_ai_handle_revive_request(
        struct gs_state *gs,
        struct gs_character *character,
        packet_t *request)
{
        struct gs_packet_revive_request revive_request = { 0 };

        assert(gs);
        assert(character);
        assert(request);

        gs_packet_revive_request_unpack(&revive_request, request);
        gs_character_revive(gs, character, revive_request.option_chosen);
        gs_ai_on_revive(gs, character);
}

static void gs_ai_idle_state(
        struct gs_state *gs,
        struct gs_character *character,
        packet_t *request)
{
        assert(gs);
        assert(character);
        assert(request);

        switch (packet_type(request)) {
        case 0x01: // Moving
                gs_ai_handle_move_request(gs, character, request);
                break;
        case 0x04: // Action.
                gs_ai_handle_action_request(gs, character, request);
                break;
        case 0x09: // Logout.
                log_normal("logout, todo");
                break;
        case 0x38: // Say.
                gs_ai_handle_say(gs, character, request);
                break;
        case 0x46: // Restart.
                gs_ai_handle_restart_request(gs, character);
                break;
        case 0x48: // Validate position.
                gs_ai_handle_val_pos_request(gs, character, request);
                break;
        case 0xcd: // Show map.
                log_normal("show map, todo");
                gs_character_spawn_random_orc(gs, &character->position);
                break;
        default:
                log_normal("unable to handle packet.");
                break;
        }
}

static void gs_ai_moving_state(
        struct gs_state *gs,
        struct gs_character *character,
        packet_t *request)
{
        assert(gs);
        assert(character);
        assert(request);

        switch (packet_type(request)) {
        default:
                gs_ai_idle_state(gs, character, request);
                break;
        }
}

static void gs_ai_target_selected_state(
        struct gs_state *gs,
        struct gs_character *character,
        packet_t *request)
{
        assert(gs);
        assert(character);
        assert(request);

        switch (packet_type(request)) {
        case 0x04:
                gs_ai_handle_attack_request(gs, character, request);
                break;
        default:
                gs_ai_idle_state(gs, character, request);
                break;
        }
}

static void gs_ai_moving_to_attack_state(
        struct gs_state *gs,
        struct gs_character *character,
        packet_t *request)
{
        assert(gs);
        assert(character);
        assert(request);

        switch (packet_type(request)) {
        default:
                gs_ai_idle_state(gs, character, request);
                break;
        }
}

static void gs_ai_attacking_state(
        struct gs_state *gs,
        struct gs_character *character,
        packet_t *request)
{
        assert(gs);
        assert(character);
        assert(request);

        switch (packet_type(request)) {
        default:
                gs_ai_idle_state(gs, character, request);
                break;
        }
}

static void gs_ai_dead_state(
        struct gs_state *gs,
        struct gs_character *character,
        packet_t *request)
{
        assert(gs);
        assert(character);
        assert(request);

        switch (packet_type(request)) {
        case 0x6d: // Revive
                gs_ai_handle_revive_request(gs, character, request);
                break;
        default:
                break;
        }
}

static void gs_ai_update_character_position(
        struct gs_state *gs,
        struct gs_character *character,
        double delta)
{
        struct gs_move_data *move_data = 0;

        u64_t elapsed = 0;

        assert(gs);
        assert(character);

        move_data = &character->ai.move_data;

        // No need to move.
        if (!move_data->move_start_time) {
                return;
        }

        if (move_data->move_timestamp == gs->game_ticks) {
                return;
        }

        elapsed = gs->game_ticks - move_data->move_start_time;

        if (elapsed >= move_data->ticks_to_move) {
                move_data->move_timestamp = gs->game_ticks;

                character->position     = move_data->destination;
                character->ai.move_data = (struct gs_move_data){ 0 };
                character->ai.state     = AI_IDLE;

                return;
        }

        character->position.x = move_data->origin.x +
                                (i32_t)(elapsed * move_data->x_speed_ticks);
        character->position.y = move_data->origin.y +
                                (i32_t)(elapsed * move_data->y_speed_ticks);

        move_data->move_timestamp = gs->game_ticks;
}

static void
gs_ai_npc_initiate_idle_walk(struct gs_state *gs, struct gs_character *npc)
{
        struct gs_point random_point = { 0 };

        assert(gs);
        assert(npc);

        if (!gs_character_is_npc(npc)) {
                return;
        }

        if (npc->ai.idle_cd > 0) {
                return;
        }

        npc->ai.idle_cd = 120;

        if (gs_ai_random_number(1, 100) <= 33) {
                return;
        }

        // For npcs the heading is only set when
        // moving. For some reason, it works quite
        // well for "calculating" a random position.
        // It may be a good idea to double check
        // why that's the case.
        random_point.x = npc->position.x - (60) * cos(npc->heading);
        random_point.y = npc->position.y - (60) * sin(npc->heading);
        random_point.z = npc->position.z;
        gs_ai_move(gs, npc, &random_point);
}

void gs_ai_tick(
        struct gs_state *gs,
        struct gs_character *character,
        double delta)
{
        struct gs_character *target = 0;

        assert(gs);
        assert(character);

        if (character->ai.attack_cd > 0) {
                character->ai.attack_cd -= delta * 100;
        }

        if (character->ai.idle_cd > 0) {
                character->ai.idle_cd -= delta * 100;
        }

        if (character->ai.target_id) {
                target = gs_character_find_by_id(gs, character->ai.target_id);
        }

        switch (character->ai.state) {
        case AI_IDLE:
                gs_ai_npc_initiate_idle_walk(gs, character);
                break;
        case AI_MOVING:
                gs_ai_update_character_position(gs, character, delta);
                break;
        case AI_TARGET_SELECTED:
                gs_ai_update_character_position(gs, character, delta);
                break;
        case AI_ATTACKING:
                gs_ai_update_character_position(gs, character, delta);
                gs_ai_attack(gs, character, target);
                break;
        case AI_MOVING_TO_ATTACK:
                gs_ai_update_character_position(gs, character, delta);
                gs_ai_attack(gs, character, target);
                break;
        case AI_DEAD:
                break;
        default:
                break;
        }
}

void gs_ai_handle_request(
        struct gs_state *gs,
        struct gs_character *character,
        packet_t *request)
{
        assert(gs);
        assert(character);
        assert(request);

        switch (character->ai.state) {
        case AI_IDLE:
                gs_ai_idle_state(gs, character, request);
                break;
        case AI_MOVING:
                gs_ai_moving_state(gs, character, request);
                break;
        case AI_TARGET_SELECTED:
                gs_ai_target_selected_state(gs, character, request);
                break;
        case AI_MOVING_TO_ATTACK:
                gs_ai_moving_to_attack_state(gs, character, request);
                break;
        case AI_ATTACKING:
                gs_ai_attacking_state(gs, character, request);
                break;
        case AI_DEAD:
                gs_ai_dead_state(gs, character, request);
                break;
        default:
                break;
        }
}
