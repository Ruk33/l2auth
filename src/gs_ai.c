#include <assert.h>
#include <math.h>
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

static void gs_ai_move(struct gs_character *src, struct gs_point *where)
{
        assert(src);
        assert(where);

        src->ai.moving_to = *where;
        src->ai.state     = AI_MOVING;

        gs_character_move(src, where);
}

static void
gs_ai_attack(struct gs_character *attacker, struct gs_character *target)
{
        struct gs_point walk_to = { 0 };

        double d = 0;
        double a = 0;

        assert(attacker);

        if (!target) {
                attacker->ai.target_id = 0;
                attacker->ai.state     = AI_IDLE;
                return;
        }

        attacker->ai.target_id = target->id;
        attacker->ai.state     = AI_ATTACKING;

        d = gs_character_distance(attacker, target);

        if (d > 30) {
                a = gs_character_angle_to_point(attacker, &target->position);
                walk_to.x = target->position.x - 20 * cos(a);
                walk_to.y = target->position.y - 20 * sin(a);
                walk_to.z = target->position.z;
                gs_ai_move(attacker, &walk_to);
                attacker->ai.state = AI_MOVING_TO_ATTACK;
                return;
        }

        if (attacker->ai.attack_cd > 0) {
                return;
        }

        if (gs_character_is_npc(target)) {
                gs_ai_on_npc_attacked(target, attacker);
        }

        attacker->ai.attack_cd = 100;
        gs_character_attack(attacker, target);
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

static void
gs_ai_handle_move_request(struct gs_character *character, packet_t *packet)
{
        gs_packet_move_request_t move_request = { 0 };

        struct gs_point point = { 0 };

        assert(character);
        assert(packet);

        gs_packet_move_request_unpack(&move_request, packet);

        point.x = move_request.x;
        point.y = move_request.y;
        point.z = move_request.z;

        gs_ai_move(character, &point);
}

static void
gs_ai_handle_val_pos_request(struct gs_character *character, packet_t *packet)
{
        gs_packet_validate_pos_request_t validate_request = { 0 };

        // double a = 0;

        assert(character);
        assert(packet);

        gs_packet_validate_pos_request_unpack(&validate_request, packet);

        // a =
        //         atan2(character->target_y - character->y,
        //               character->target_x - character->x);
        // log("old x: %d - old y: %d", character->x, character->y);
        // log("new x: %d - new y: %d", validate_request.x, validate_request.y);
        // log("my  x: %d - my y : %d",
        //     (i32_t)(character->x + 120 * sin(a)),
        //     (i32_t)(character->y + 120 * cos(a)));

        // If it's close enough to current location, update. If not, don't.
        // character->x = validate_request.x;
        // character->y = validate_request.y;

        // todo: can we calculate z ourselves without relying on the client?
        character->position.z = validate_request.z;
        character->heading    = validate_request.heading;

        gs_character_validate_position(character);
}

static void
gs_ai_handle_action_request(struct gs_character *character, packet_t *packet)
{
        gs_packet_action_request_t action = { 0 };

        struct gs_character *target = 0;

        assert(character);
        assert(packet);

        gs_packet_action_request_unpack(&action, packet);

        target = gs_character_find_by_id(action.target_id);

        if (!target) {
                log("selected target %d not found, ignoring", action.target_id);
                return;
        }

        gs_ai_select_target(character, target);
}

static void
gs_ai_handle_attack_request(struct gs_character *character, packet_t *packet)
{
        gs_packet_action_request_t action = { 0 };

        struct gs_character *target = 0;

        assert(character);
        assert(packet);

        gs_packet_action_request_unpack(&action, packet);

        target = gs_character_find_by_id(action.target_id);

        if (!target) {
                log("attacking %d not found, ignoring", action.target_id);
                return;
        }

        gs_ai_attack(character, target);
}

static void gs_ai_idle_state(struct gs_character *character, packet_t *request)
{
        assert(character);
        assert(request);

        switch (packet_type(request)) {
        case 0x01: // Moving
                gs_ai_handle_move_request(character, request);
                break;
        case 0x04: // Action.
                gs_ai_handle_action_request(character, request);
                break;
        case 0x09: // Logout.
                log("Logout, todo");
                break;
        case 0x38: // Say.
                log("Say, todo");
                break;
        case 0x46: // Restart.
                log("Restart, todo");
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

static void
gs_ai_moving_state(struct gs_character *character, packet_t *request)
{
        assert(character);
        assert(request);

        switch (packet_type(request)) {
        default:
                gs_ai_idle_state(character, request);
                break;
        }
}

static void
gs_ai_target_selected_state(struct gs_character *character, packet_t *request)
{
        assert(character);
        assert(request);

        switch (packet_type(request)) {
        case 0x04:
                gs_ai_handle_attack_request(character, request);
                break;
        default:
                gs_ai_idle_state(character, request);
                break;
        }
}

static void
gs_ai_moving_to_attack_state(struct gs_character *character, packet_t *request)
{
        assert(character);
        assert(request);

        switch (packet_type(request)) {
        default:
                break;
        }
}

static void
gs_ai_attacking_state(struct gs_character *character, packet_t *request)
{
        assert(character);
        assert(request);

        switch (packet_type(request)) {
        default:
                break;
        }
}

static void gs_ai_tick(struct gs_character *character, double delta)
{
        struct gs_character *target = 0;

        double d = 0;

        double dx            = 0;
        double dy            = 0;
        double _sin          = 0;
        double _cos          = 0;
        double x_speed_ticks = 0;
        double y_speed_ticks = 0;

        assert(character);
        PREVENT_UNUSED_WARNING(delta);

        if (character->ai.attack_cd > 0) {
                character->ai.attack_cd -= 10 * delta;
        }

        if (character->ai.target_id) {
                target = gs_character_find_by_id(character->ai.target_id);
        }

        switch (character->ai.state) {
        case AI_IDLE:
                break;
        case AI_MOVING:
                dx   = character->ai.moving_to.x - character->position.x;
                dy   = character->ai.moving_to.y - character->position.y;
                d    = sqrt(dx * dx + dy * dy);
                _cos = dx / d;
                _sin = dy / d;
                x_speed_ticks = _cos * 120 / 10;
                y_speed_ticks = _sin * 120 / 10;

                character->heading =
                        atan2(-_sin, -_cos) * 10430.378350470452724949566316381;
                character->heading += 32768;

                log("moving from: %d %d",
                    character->position.x,
                    character->position.y);
                character->position.x += (i32_t)(x_speed_ticks * delta);
                character->position.y += (i32_t)(y_speed_ticks * delta);
                log("moving to: %d %d (%d %d)",
                    character->position.x,
                    character->position.y,
                    (i32_t)(x_speed_ticks * delta),
                    (i32_t)(y_speed_ticks * delta));

                if (d < 20) {
                        character->position.x = character->ai.moving_to.x;
                        character->position.y = character->ai.moving_to.y;
                        character->ai.state   = AI_IDLE;
                }
                break;
        case AI_TARGET_SELECTED:
                break;
        case AI_ATTACKING:
                gs_ai_attack(character, target);
                break;
        case AI_MOVING_TO_ATTACK:
                dx   = character->ai.moving_to.x - character->position.x;
                dy   = character->ai.moving_to.y - character->position.y;
                d    = sqrt(dx * dx + dy * dy);
                _cos = dx / d;
                _sin = dy / d;
                x_speed_ticks = _cos * 60;
                y_speed_ticks = _sin * 60;

                character->heading =
                        atan2(-_sin, -_cos) * 10430.378350470452724949566316381;
                character->heading += 32768;
                character->position.x += x_speed_ticks * delta;
                character->position.y += y_speed_ticks * delta;

                gs_ai_attack(character, target);
                break;
        default:
                break;
        }
}

static void
gs_ai_handle_request(struct gs_character *character, packet_t *request)
{
        assert(character);
        assert(request);

        switch (character->ai.state) {
        case AI_IDLE:
                gs_ai_idle_state(character, request);
                break;
        case AI_MOVING:
                gs_ai_moving_state(character, request);
                break;
        case AI_TARGET_SELECTED:
                gs_ai_target_selected_state(character, request);
                break;
        case AI_MOVING_TO_ATTACK:
                gs_ai_moving_to_attack_state(character, request);
                break;
        case AI_ATTACKING:
                gs_ai_attacking_state(character, request);
                break;
        default:
                break;
        }
}
