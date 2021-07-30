#include <assert.h>
#include <math.h>
#include "include/config.h"
#include "include/log.h"
#include "include/packet.h"
#include "include/gs_types.h"
#include "include/gs_character.h"
#include "include/gs_packet_action_request.h"
#include "include/gs_packet_validate_pos_request.h"
#include "include/gs_packet_move_request.h"
#include "include/gs_ai.h"

static void
gs_ai_on_npc_attacked(struct gs_character *npc, struct gs_character *attacker)
{
        assert(npc);
        assert(attacker);
        npc->ai.state     = AI_ATTACKING;
        npc->ai.target_id = attacker->id;
}

static void gs_ai_move(
        struct gs_state *state,
        struct gs_character *src,
        struct gs_point *where)
{
        double dx = 0;
        double dy = 0;
        double d  = 0;

        double _sin = 0;
        double _cos = 0;

        assert(state);
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
        src->ai.move_data.move_start_time = state->game_ticks;

        src->ai.move_data.origin = src->position;

        if (src->ai.move_data.ticks_to_move < 1) {
                src->ai.move_data.ticks_to_move = 1;
        }

        src->heading = (i32_t)(
                atan2(-_sin, -_cos) * 10430.378350470452724949566316381);
        src->heading += 32768;

        gs_character_move(state, src, where);
}

static void gs_ai_attack(
        struct gs_state *state,
        struct gs_character *attacker,
        struct gs_character *target)
{
        struct gs_point walk_to = { 0 };

        assert(state);
        assert(attacker);

        if (!target) {
                attacker->ai.target_id = 0;
                attacker->ai.state     = AI_IDLE;
                return;
        }

        attacker->ai.target_id = target->id;
        attacker->ai.state     = AI_ATTACKING;

        if (gs_character_distance(attacker, target) > 80) {
                walk_to = target->position;
                gs_ai_move(state, attacker, &walk_to);
                attacker->ai.state = AI_MOVING_TO_ATTACK;
                return;
        }

        if (attacker->ai.attack_cd > 0) {
                return;
        }

        if (gs_character_is_npc(target)) {
                gs_ai_on_npc_attacked(target, attacker);
        }

        attacker->ai.attack_cd = 30;
        gs_character_attack(state, attacker, target);
}

static void
gs_ai_select_target(struct gs_character *src, struct gs_character *target)
{
        assert(src);
        assert(target);

        src->ai.state     = AI_TARGET_SELECTED;
        src->ai.target_id = target->id;

        gs_character_select_target(src, target);
}

static void gs_ai_handle_move_request(
        struct gs_state *state,
        struct gs_character *character,
        packet_t *packet)
{
        gs_packet_move_request_t move_request = { 0 };

        struct gs_point point = { 0 };

        assert(state);
        assert(character);
        assert(packet);

        gs_packet_move_request_unpack(&move_request, packet);

        point.x = move_request.x;
        point.y = move_request.y;
        point.z = move_request.z;

        gs_ai_move(state, character, &point);
}

static void
gs_ai_handle_val_pos_request(struct gs_character *character, packet_t *packet)
{
        gs_packet_validate_pos_request_t validate_request = { 0 };

        struct gs_character client_position = { 0 };

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
                gs_character_validate_position(&client_position);
        } else {
                gs_character_validate_position(character);
        }
}

static void gs_ai_handle_action_request(
        struct gs_state *state,
        struct gs_character *character,
        packet_t *packet)
{
        gs_packet_action_request_t action = { 0 };

        struct gs_character *target = 0;

        assert(state);
        assert(character);
        assert(packet);

        gs_packet_action_request_unpack(&action, packet);

        target = gs_character_find_by_id(state, action.target_id);

        if (!target) {
                log("selected target %d not found, ignoring", action.target_id);
                return;
        }

        gs_ai_select_target(character, target);
}

static void gs_ai_handle_attack_request(
        struct gs_state *state,
        struct gs_character *character,
        packet_t *packet)
{
        gs_packet_action_request_t action = { 0 };

        struct gs_character *target = 0;

        assert(state);
        assert(character);
        assert(packet);

        gs_packet_action_request_unpack(&action, packet);

        target = gs_character_find_by_id(state, action.target_id);

        if (!target) {
                log("attacking %d not found, ignoring", action.target_id);
                return;
        }

        gs_ai_attack(state, character, target);
}

static void gs_ai_idle_state(
        struct gs_state *state,
        struct gs_character *character,
        packet_t *request)
{
        assert(state);
        assert(character);
        assert(request);

        switch (packet_type(request)) {
        case 0x01: // Moving
                gs_ai_handle_move_request(state, character, request);
                break;
        case 0x04: // Action.
                gs_ai_handle_action_request(state, character, request);
                break;
        case 0x09: // Logout.
                log("Logout, todo");
                break;
        case 0x38: // Say.
                log("Say, todo");
                break;
        case 0x46: // Restart.
                gs_character_spawn_random_orc(state);
                break;
        case 0x48: // Validate position.
                gs_ai_handle_val_pos_request(character, request);
                break;
        case 0xcd: // Show map.
                log("Show map, todo");
                break;
        default:
                log("Unable to handle packet.");
                break;
        }
}

static void gs_ai_moving_state(
        struct gs_state *state,
        struct gs_character *character,
        packet_t *request)
{
        assert(state);
        assert(character);
        assert(request);

        switch (packet_type(request)) {
        default:
                gs_ai_idle_state(state, character, request);
                break;
        }
}

static void gs_ai_target_selected_state(
        struct gs_state *state,
        struct gs_character *character,
        packet_t *request)
{
        assert(state);
        assert(character);
        assert(request);

        switch (packet_type(request)) {
        case 0x04:
                gs_ai_handle_attack_request(state, character, request);
                break;
        default:
                gs_ai_idle_state(state, character, request);
                break;
        }
}

static void gs_ai_moving_to_attack_state(
        struct gs_state *state,
        struct gs_character *character,
        packet_t *request)
{
        assert(state);
        assert(character);
        assert(request);

        switch (packet_type(request)) {
        default:
                gs_ai_idle_state(state, character, request);
                break;
        }
}

static void gs_ai_attacking_state(
        struct gs_state *state,
        struct gs_character *character,
        packet_t *request)
{
        assert(state);
        assert(character);
        assert(request);

        switch (packet_type(request)) {
        default:
                gs_ai_idle_state(state, character, request);
                break;
        }
}

static void
gs_ai_tick(struct gs_state *state, struct gs_character *character, double delta)
{
        struct gs_character *target    = 0;
        struct gs_move_data *move_data = 0;

        u64_t elapsed = 0;

        assert(state);
        assert(character);

        if (character->ai.attack_cd > 0) {
                character->ai.attack_cd -= delta * 100;
        }

        if (character->ai.target_id) {
                target =
                        gs_character_find_by_id(state, character->ai.target_id);
        }

        switch (character->ai.state) {
        case AI_IDLE:
                break;
        case AI_MOVING:
                move_data = &character->ai.move_data;

                if (move_data->move_timestamp == state->game_ticks) {
                        return;
                }

                elapsed = state->game_ticks - move_data->move_start_time;

                if (elapsed >= move_data->ticks_to_move) {
                        move_data->move_timestamp = state->game_ticks;

                        character->position = move_data->destination;
                        character->ai.state = AI_IDLE;

                        return;
                }

                character->position.x =
                        move_data->origin.x +
                        (i32_t)(elapsed * move_data->x_speed_ticks);
                character->position.y =
                        move_data->origin.y +
                        (i32_t)(elapsed * move_data->y_speed_ticks);

                move_data->move_timestamp = state->game_ticks;
                break;
        case AI_TARGET_SELECTED:
                break;
        case AI_ATTACKING:
                gs_ai_attack(state, character, target);
                break;
        case AI_MOVING_TO_ATTACK:
                move_data = &character->ai.move_data;

                if (move_data->move_timestamp == state->game_ticks) {
                        return;
                }

                elapsed = state->game_ticks - move_data->move_start_time;

                if (elapsed >= move_data->ticks_to_move) {
                        move_data->move_timestamp = state->game_ticks;

                        character->position = move_data->destination;
                        gs_ai_attack(state, character, target);

                        return;
                }

                character->position.x =
                        move_data->origin.x +
                        (i32_t)(elapsed * move_data->x_speed_ticks);
                character->position.y =
                        move_data->origin.y +
                        (i32_t)(elapsed * move_data->y_speed_ticks);

                move_data->move_timestamp = state->game_ticks;
                gs_ai_attack(state, character, target);
                break;
        default:
                break;
        }
}

static void gs_ai_handle_request(
        struct gs_state *state,
        struct gs_character *character,
        packet_t *request)
{
        assert(state);
        assert(character);
        assert(request);

        switch (character->ai.state) {
        case AI_IDLE:
                gs_ai_idle_state(state, character, request);
                break;
        case AI_MOVING:
                gs_ai_moving_state(state, character, request);
                break;
        case AI_TARGET_SELECTED:
                gs_ai_target_selected_state(state, character, request);
                break;
        case AI_MOVING_TO_ATTACK:
                gs_ai_moving_to_attack_state(state, character, request);
                break;
        case AI_ATTACKING:
                gs_ai_attacking_state(state, character, request);
                break;
        default:
                break;
        }
}
