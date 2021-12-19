#include <assert.h>
#include <stdlib.h>
#include <math.h>
#include "include/config.h"
#include "include/packet.h"
#include "include/gs_types.h"
#include "include/gs_character.h"
#include "include/gs_client_packets.h"
#include "include/gs_ai.h"

#define GS_AI_SHOW_NPC_HTML_ARRAY_MESSAGE(gs, character, src) \
        gs_character_show_npc_html_message(gs, character, src, sizeof(src))

// Packet type from client.
enum request_type {
    request_type_none              = 0,
    request_type_moving            = 0x01,
    request_type_action            = 0x04,
    request_type_logout            = 0x09,
    request_type_say               = 0x38,
    request_type_restart           = 0x46,
    request_type_validate_position = 0x48,
    request_type_show_map          = 0xcd,
    request_type_bypass            = 0x21,
    request_type_attack            = 0x0a,
    request_type_revive            = 0x6d,
};

// This table/array represents all the actions (requests made by clients)
// that can be handled depending of the ai's state.
// It's meant to avoid situations that can't happen. For instance,
// a dead player can't handle a character walk/movement request.
static enum request_type g_actions_by_state[][10] = {
    [AI_IDLE]               = {
        request_type_moving, request_type_action, request_type_logout, 
        request_type_say, request_type_restart, request_type_validate_position,
        request_type_show_map,
    },
    [AI_MOVING]             = {
        request_type_moving, request_type_action, request_type_logout,
        request_type_say, request_type_restart, request_type_validate_position, 
        request_type_show_map,
    },
    [AI_TARGET_SELECTED]    = {
        request_type_moving, request_type_action, request_type_logout,
        request_type_say, request_type_restart, request_type_validate_position, 
        request_type_show_map, request_type_bypass, request_type_attack,
    },
    [AI_MOVING_TO_ATTACK]   = {
        request_type_moving, request_type_action, request_type_logout,
        request_type_say, request_type_restart, request_type_validate_position, 
        request_type_show_map, request_type_attack,
    },
    [AI_HAS_AGRO]          = {
        request_type_moving, request_type_action, request_type_say, 
        request_type_validate_position, request_type_show_map, request_type_attack,
    },
    [AI_LAUNCHED_ATTACK]    = {
        request_type_moving, request_type_action, request_type_say, 
        request_type_validate_position, request_type_show_map, request_type_attack,
    },
    [AI_MOVING_TO_INTERACT] = {
        request_type_moving, request_type_action, request_type_logout, 
        request_type_say, request_type_restart, request_type_validate_position,
        request_type_show_map,
    },
    [AI_INTERACTING]        = {
        request_type_moving, request_type_action, request_type_logout, 
        request_type_say, request_type_restart, request_type_validate_position,
        request_type_show_map,
    },
    [AI_DEAD]               = {
        request_type_logout, request_type_say, request_type_restart, 
        request_type_show_map, request_type_revive,
    },
};

static i32_t gs_ai_random_number(i32_t a, i32_t b)
{
        return rand() % (b + 1 - a) + a;
}

static void gs_ai_on_npc_attacked(
        struct gs_state *gs,
        struct gs_character *npc,
        struct gs_character *attacker)
{
        assert(gs);
        assert(npc);
        assert(attacker);

        npc->ai.state     = AI_HAS_AGRO;
        npc->ai.target_id = attacker->id;

        gs_character_run(gs, npc);
}

static void gs_ai_on_npc_interact(
        struct gs_state *gs,
        struct gs_character *npc,
        struct gs_character *player)
{
        assert(gs);
        assert(npc);
        assert(player);

        // Todo: don't hardcode the message.
        GS_AI_SHOW_NPC_HTML_ARRAY_MESSAGE(
                gs, player, "<html><body>Hi, this is a test!</body></html>");
}

static void gs_ai_on_dead(
        struct gs_state *gs,
        struct gs_character *dead,
        struct gs_character *killer)
{
        assert(gs);
        assert(dead);
        assert(killer);

        dead->ai       = (struct gs_ai){ 0 };
        dead->ai.state = AI_DEAD;

        killer->ai.move_data = (struct gs_move_data){ 0 };
        killer->ai.state     = AI_IDLE;
        killer->ai.target_id = 0;

        gs_character_stop_auto_attack(gs, killer, dead);
        gs_character_stop_auto_attack(gs, dead, killer);

        if (gs_character_is_npc(killer)) {
                gs_character_walk(gs, killer);
        }
}

static void gs_ai_on_revive(struct gs_state *gs, struct gs_character *src)
{
        assert(gs);
        assert(src);

        src->ai       = (struct gs_ai){ 0 };
        src->stats.hp = src->stats.max_hp;

        gs_character_send_status(gs, src, src);
}

static void gs_ai_go_idle(struct gs_state *gs, struct gs_character *src)
{
        assert(gs);
        assert(src);

        src->ai.move_data = (struct gs_move_data){ 0 };
        src->ai.state     = AI_IDLE;
}

static void
gs_ai_move(struct gs_state *gs, struct gs_character *src, struct gs_point *where)
{
        double speed = 0;

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
        speed = src->running ? src->stats.run_speed : src->stats.walk_speed;

        dx = where->x - src->position.x;
        dy = where->y - src->position.y;
        d  = sqrt(dx * dx + dy * dy);

        _sin = dy / d;
        _cos = dx / d;
        d -= 10 /* 10 as offset */ - 5;

        src->ai.move_data.heading       = 0;
        src->ai.move_data.destination   = *where;
        src->ai.move_data.ticks_to_move = (u32_t)(TICKS_PER_SECOND * d / speed);
        src->ai.move_data.x_speed_ticks = _cos * speed / TICKS_PER_SECOND;
        src->ai.move_data.y_speed_ticks = _sin * speed / TICKS_PER_SECOND;
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
                gs_ai_go_idle(gs, attacker);
                return;
        }

        attacker->ai.target_id = target->id;
        attacker->ai.state     = AI_HAS_AGRO;

        if (attacker->ai.attack_cd > 0) {
                return;
        }

        // If the attacker is too far away
        // from the target, make it walk closer.
        if (gs_character_distance(attacker, target) > 80) {
                // (franco.montenegro) We may want to introduce some
                // vector math to make this easier, right?
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
                gs_ai_on_npc_attacked(gs, target, attacker);
        }

        // (franco.montenegro) Properly calculate this value 
        // based on attack speed.
        attacker->ai.attack_cd = 10;
        // (franco.montenegro) Do we really need this new state or
        // can we work with just AI_ATTACK?
        attacker->ai.state = AI_LAUNCHED_ATTACK;
        gs_character_attack(gs, attacker, target);
}

static void gs_ai_interact(
        struct gs_state *gs,
        struct gs_character *src,
        struct gs_character *target)
{
        struct gs_point walk_to = { 0 };
        double walk_angle       = 0;

        assert(gs);
        assert(src);

        if (!target) {
                src->ai.target_id = 0;
                gs_ai_go_idle(gs, src);
                return;
        }

        src->ai.target_id = target->id;
        src->ai.state     = AI_INTERACTING;

        if (gs_character_distance(src, target) > 80) {
                walk_angle =
                        gs_character_angle_to_point(src, &target->position);
                walk_to.x = target->position.x - 40 * cos(walk_angle);
                walk_to.y = target->position.y - 40 * sin(walk_angle);
                walk_to.z = target->position.z;
                gs_ai_move(gs, src, &walk_to);
                src->ai.state = AI_MOVING_TO_INTERACT;
                return;
        }

        if (gs_character_is_npc(target)) {
                gs_ai_on_npc_interact(gs, target, src);
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
                gs_ai_go_idle(gs, character);
                return;
        }

        gs_ai_select_target(gs, character, target);
}

static void gs_ai_handle_attack_request(
        struct gs_state *gs,
        struct gs_character *character,
        packet_t *packet)
{
        struct gs_packet_attack_request attack = { 0 };

        struct gs_character *target = 0;

        assert(gs);
        assert(character);
        assert(packet);

        gs_packet_attack_request_unpack(&attack, packet);

        target = gs_character_find_by_id(gs, attack.target_id);

        if (!target) {
                character->ai.target_id = 0;
                gs_ai_go_idle(gs, character);
                return;
        }

        gs_ai_attack(gs, character, target);
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

        UTIL_SET_ZERO_ARRAY(message);

        gs_packet_say_request_unpack(&say, packet);
        L2_STRING_TO_CHAR_ARRAY(message, say.message, say.size);
        gs_character_say(gs, character, message, sizeof(message));
}

static void gs_ai_handle_interaction_request(
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
                gs_ai_go_idle(gs, character);
                return;
        }

        gs_ai_interact(gs, character, target);
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

static void gs_ai_handle_bypass_request(
        struct gs_state *gs,
        struct gs_character *character,
        packet_t *request)
{
        struct gs_packet_bypass_request bypass_request = { 0 };

        // Todo: remove. This is just for testing.
        char command[64] = { 0 };

        assert(gs);
        assert(character);
        assert(request);

        gs_packet_bypass_request_unpack(&bypass_request, request);

        L2_STRING_TO_CHAR_ARRAY(
                command, bypass_request.command, bypass_request.size);

        log_normal("command is: %s", command);
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
                GS_AI_SHOW_NPC_HTML_ARRAY_MESSAGE(
                        gs,
                        character,
                        "<html><body>And so... a chat window popped out. <a action=\"bypass -h npc_42_support player\">Magic support</a></body></html>");
                gs_character_spawn_random_orc(gs, &character->position);
                break;
        case 0x21: // Bypass.
                gs_ai_handle_bypass_request(gs, character, request);
                GS_AI_SHOW_NPC_HTML_ARRAY_MESSAGE(
                        gs,
                        character,
                        "<html><body>It works! even though it breaks if you send more than 3kb of data!</body></html>");
                break;
        case 0x0a: // Attack request
                gs_ai_handle_attack_request(gs, character, request);
                break;
        default:
                log_normal("unable to handle packet %x.", packet_type(request));
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
                gs_ai_handle_interaction_request(gs, character, request);
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
        case AI_HAS_AGRO:
                gs_ai_update_character_position(gs, character, delta);
                gs_ai_attack(gs, character, target);
                break;
        case AI_LAUNCHED_ATTACK:
                // Find a better way to implement these behaviors
                // Maybe making these many checks will become unmaintainable.
                if (character->stats.hp > 0 && character->ai.attack_cd <= 0) {
                        if (target) {
                                // (franco.montenegro) Implement properly.
                                target->stats.hp = target->stats.hp > 30 ? target->stats.hp - 30 : 0;
                                character->ai.state = AI_HAS_AGRO;

                                // Make sure both, attacker and target
                                // get their status updated.
                                // (franco.montenegro) I think this should be broadcasted
                                gs_character_send_status(gs, target, target);
                                gs_character_send_status(gs, target, character);

                                if (target->stats.hp == 0) {
                                        gs_character_die(gs, target);
                                        gs_ai_go_idle(gs, character);
                                }
                        } else {
                                gs_ai_go_idle(gs, character);
                        }
                }
                break;
        case AI_MOVING_TO_ATTACK:
                gs_ai_update_character_position(gs, character, delta);
                gs_ai_attack(gs, character, target);
                break;
        case AI_INTERACTING:
                break;
        case AI_MOVING_TO_INTERACT:
                gs_ai_update_character_position(gs, character, delta);
                gs_ai_interact(gs, character, target);
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
        enum request_type *allowed_by_state = 0;
        int can_be_handled = 0;

        assert(gs);
        assert(character);
        assert(request);

        allowed_by_state = g_actions_by_state[character->ai.state];

        for (size_t i = 0; i < UTIL_ARRAY_LEN(g_actions_by_state[0]); i += 1) {
            if (allowed_by_state[i] == packet_type(request)) {
                can_be_handled = 1;
                break;
            }
        }

        if (!can_be_handled) {
            log_normal("unable to handle request by current state.");
            return;
        }

        switch (packet_type(request)) {
        case request_type_none:
            break;
        case request_type_moving:
            gs_ai_handle_move_request(gs, character, request);
            break;
        case request_type_action:
            gs_ai_handle_action_request(gs, character, request);
            break;
        case request_type_logout:
            break;
        case request_type_say:
            gs_ai_handle_say(gs, character, request);
            break;
        case request_type_restart:
            gs_ai_handle_restart_request(gs, character);
            break;
        case request_type_validate_position:
            gs_ai_handle_val_pos_request(gs, character, request);
            break;
        case request_type_show_map:
            break;
        case request_type_bypass:
            break;
        case request_type_attack:
            gs_ai_handle_attack_request(gs, character, request);
            break;
        case request_type_revive:
            gs_ai_handle_revive_request(gs, character, request);
            break;
        default:
            break;
        }
}
