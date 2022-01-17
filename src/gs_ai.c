#include <assert.h>
#include <cglm/cglm.h>
#include "include/config.h"
#include "include/packet.h"
#include "include/gs_types.h"
#include "include/gs_character.h"
#include "include/gs_client_packets.h"
#include "include/gs_ai.h"

#define macro_gs_ai_html_arr_msg(gs, character, src) \
    gs_character_show_npc_html_message(gs, character, src, sizeof(src))

// Packet type from client.
enum request_type
{
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
// that can be handled depending of the AI's state.
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
        request_type_show_map, request_type_attack,
    },
    [AI_DEAD]               = {
        request_type_logout, request_type_say, request_type_restart, 
        request_type_show_map, request_type_revive,
    },
};

static void gs_ai_on_npc_interact(struct gs_state *gs,
                                  struct gs_character *npc,
                                  struct gs_character *player)
{
    assert(gs);
    assert(npc);
    assert(player);

    // vec3 npc_vec    = { 0 };
    // vec3 player_vec = { 0 };
    // float angle     = 0;

    // npc_vec[0] = npc->position.x;
    // npc_vec[1] = npc->position.y;
    // npc_vec[2] = npc->position.z;

    // player_vec[0] = player->position.x;
    // player_vec[1] = player->position.y;
    // player_vec[2] = player->position.z;

    // angle = glm_vec3_angle(npc_vec, player_vec);
    // angle = glm_deg(angle);

    // gs_character_face_to(gs, npc, angle);

    // Todo: don't hardcode the message.
    macro_gs_ai_html_arr_msg(gs,
                             player,
                             "<html><body>Hi, this is a "
                             "test!</body></html>");
}

static void gs_ai_go_idle(struct gs_state *gs, struct gs_character *src)
{
    assert(gs);
    assert(src);

    src->ai.move_data = (struct gs_move_data){ 0 };

    if (src->ai.target_id) {
        src->ai.state = AI_TARGET_SELECTED;
        return;
    }

    src->ai.state = AI_IDLE;
}

static void gs_ai_on_dead(struct gs_state *gs,
                          struct gs_character *dead,
                          struct gs_character *killer)
{
    assert(gs);
    assert(dead);
    assert(killer);

    dead->ai       = (struct gs_ai){ 0 };
    dead->ai.state = AI_DEAD;

    killer->ai.move_data     = (struct gs_move_data){ 0 };
    killer->ai.leave_agro_cd = 100;

    gs_ai_go_idle(gs, killer);

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

static void gs_ai_move(struct gs_state *gs,
                       struct gs_character *src,
                       struct gs_point *where)
{
    vec3 from = { 0 };
    vec3 to   = { 0 };

    assert(gs);
    assert(src);
    assert(where);

    from[0] = src->position.x;
    from[1] = src->position.y;
    from[2] = src->position.z;

    to[0] = where->x;
    to[1] = where->y;
    to[2] = where->z;

    src->ai.moving_to = *where;
    src->ai.state     = AI_MOVING;

    src->ai.move_data.heading         = glm_vec3_angle(from, to);
    src->ai.move_data.destination     = *where;
    src->ai.move_data.move_start_time = gs->game_ticks;
    src->ai.move_data.origin          = src->position;

    gs_character_move(gs, src, where);
}

// Move src character closer to target if required.
// If the src is too far, a movement behavior will begin and 1 will be returned.
// If no movement is required, nothing happens and 0 is returned.
static int gs_ai_move_to_interact_with(struct gs_state *gs,
                                       struct gs_character *src,
                                       struct gs_character *target)
{
    vec3 from         = { 0 };
    vec3 to           = { 0 };
    vec3 interact_vec = { 0 };

    struct gs_point position_to_interact = { 0 };

    float distance          = 0;
    float range_to_interact = 0;

    assert(gs);
    assert(src);
    assert(target);

    // (franco.montenegro) Should this be configurable?
    range_to_interact = 60;

    from[0] = src->position.x;
    from[1] = src->position.y;
    from[2] = src->position.z;

    to[0] = target->position.x;
    to[1] = target->position.y;
    to[2] = target->position.z;

    distance = glm_vec3_distance(from, to);

    // Return if there is no need to walk further/closer to the npc.
    if (distance <= range_to_interact) {
        return 0;
    }

    glm_vec3_sub(to, from, interact_vec);
    glm_vec3_normalize(interact_vec);
    glm_vec3_scale(interact_vec, distance - range_to_interact, interact_vec);
    glm_vec3_add(from, interact_vec, interact_vec);

    position_to_interact.x = interact_vec[0];
    position_to_interact.y = interact_vec[1];
    position_to_interact.z = interact_vec[2];

    gs_ai_move(gs, src, &position_to_interact);

    return 1;
}

static void gs_ai_attack(struct gs_state *gs,
                         struct gs_character *attacker,
                         struct gs_character *target)
{
    assert(gs);
    assert(attacker);

    if (!target) {
        attacker->ai.target_id = 0;
        gs_ai_go_idle(gs, attacker);
        return;
    }

    attacker->ai.target_id = target->id;
    attacker->ai.state     = AI_HAS_AGRO;

    // Prevent multiple attacks if the cool down hasn't
    // expired yet.
    if (attacker->ai.attack_cd > 0) {
        return;
    }

    // If the attacker is too far away
    // from the target, make it walk closer.
    if (gs_ai_move_to_interact_with(gs, attacker, target)) {
        attacker->ai.state = AI_MOVING_TO_ATTACK;
        return;
    }

    // If the target is an NPC, switch it's
    // state to aggro and attack the attacker.
    if (gs_character_is_npc(target)) {
        target->ai.state     = AI_HAS_AGRO;
        target->ai.target_id = attacker->id;
        gs_character_run(gs, target);
    }

    // (franco.montenegro) Properly calculate this value
    // based on attack speed.
    attacker->ai.attack_cd = 10;
    // (franco.montenegro) Do we really need this new state or
    // can we work with just AI_ATTACK?
    attacker->ai.state = AI_LAUNCHED_ATTACK;
    gs_character_launch_attack(gs, attacker, target);
}

static void gs_ai_interact(struct gs_state *gs,
                           struct gs_character *src,
                           struct gs_character *target)
{
    assert(gs);
    assert(src);

    if (!target) {
        src->ai.target_id = 0;
        gs_ai_go_idle(gs, src);
        return;
    }

    src->ai.target_id = target->id;
    src->ai.state     = AI_INTERACTING;

    // If it's too far, walk closer to the target.
    if (gs_ai_move_to_interact_with(gs, src, target)) {
        src->ai.state = AI_MOVING_TO_INTERACT;
        return;
    }

    if (gs_character_is_npc(target)) {
        gs_ai_on_npc_interact(gs, target, src);
    }
}

static void gs_ai_select_target(struct gs_state *gs,
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

static void gs_ai_handle_move_request(struct gs_state *gs,
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

static void gs_ai_handle_val_pos_request(struct gs_state *gs,
                                         struct gs_character *character,
                                         packet_t *packet)
{
    struct gs_packet_validate_pos_request validate_request = { 0 };

    vec3 server_position = { 0 };
    vec3 client_position = { 0 };

    assert(gs);
    assert(character);
    assert(packet);

    gs_packet_validate_pos_request_unpack(&validate_request, packet);
    character->heading = validate_request.heading;

    server_position[0] = character->position.x;
    server_position[1] = character->position.y;
    server_position[2] = character->position.z;

    client_position[0] = validate_request.x;
    client_position[1] = validate_request.y;
    client_position[2] = validate_request.z;

    // If there is too much difference between the client's
    // position and the server's position, correct the position
    // using the server's position.
    if (glm_vec3_distance(server_position, client_position) > 200) {
        gs_character_validate_position(gs, character, character);
    }
}

static void gs_ai_handle_action_request(struct gs_state *gs,
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

    if (target->id == character->ai.target_id) {
        gs_ai_interact(gs, character, target);
        return;
    }

    gs_ai_select_target(gs, character, target);
}

static void gs_ai_handle_attack_request(struct gs_state *gs,
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

static void gs_ai_handle_say(struct gs_state *gs,
                             struct gs_character *character,
                             packet_t *packet)
{
    char message[256] = { 0 };

    struct gs_packet_say_request say = { 0 };

    assert(gs);
    assert(character);
    assert(packet);

    gs_packet_say_request_unpack(&say, packet);
    macro_l2_str_to_char_arr(message, say.message, say.size);
    gs_character_say(gs, character, message, sizeof(message));
}

static void gs_ai_handle_restart_request(struct gs_state *gs,
                                         struct gs_character *character)
{
    assert(gs);
    assert(character);
    gs_character_restart(gs, character);
}

static void gs_ai_handle_revive_request(struct gs_state *gs,
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

static void gs_ai_handle_bypass_request(struct gs_state *gs,
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

    macro_l2_str_to_char_arr(command,
                             bypass_request.command,
                             bypass_request.size);

    log_normal("command is: %s", command);
}

static void gs_ai_update_character_position(struct gs_state *gs,
                                            struct gs_character *character,
                                            double delta)
{
    struct gs_move_data *move_data = 0;

    vec3 position = { 0 };
    vec3 target   = { 0 };
    vec3 velocity = { 0 };

    assert(gs);
    assert(character);

    move_data = &character->ai.move_data;

    // No need to move.
    if (!move_data->move_start_time) {
        return;
    }

    position[0] = character->position.x;
    position[1] = character->position.y;
    position[2] = character->position.z;

    target[0] = move_data->destination.x;
    target[1] = move_data->destination.y;
    target[2] = move_data->destination.z;

    // End movement if the character
    // is really close to the destination target.
    if (glm_vec3_distance(target, position) <= 50) {
        character->position     = character->ai.move_data.destination;
        character->ai.move_data = (struct gs_move_data){ 0 };
        gs_ai_go_idle(gs, character);
        // gs_character_validate_position(gs, character);
        return;
    }

    glm_vec3_sub(target, position, velocity);
    glm_vec3_normalize(velocity);
    glm_vec3_scale(velocity, character->stats.run_speed * delta, velocity);

    character->position.x += velocity[0];
    character->position.y += velocity[1];
    character->position.z += velocity[2];

    // gs_character_validate_position(gs, character);
}

static void gs_ai_npc_initiate_idle_walk(struct gs_state *gs,
                                         struct gs_character *npc)
{
    struct gs_point random_point = { 0 };

    i32_t random_x = 0;
    i32_t random_y = 0;

    assert(gs);
    assert(npc);

    if (!gs_character_is_npc(npc)) {
        return;
    }

    if (npc->ai.idle_cd > 0) {
        return;
    }

    npc->ai.idle_cd = 120;

    // 33% chance of initiating the wondering/idle walk.
    if (gs->random_i32(1, 100) <= 33) {
        return;
    }

    random_x = gs->random_i32(-60, 60);
    random_y = gs->random_i32(-60, 60);

    random_point.x = npc->position.x + random_x;
    random_point.y = npc->position.y + random_y;
    random_point.z = npc->position.z;

    gs_ai_move(gs, npc, &random_point);
}

void gs_ai_tick(struct gs_state *gs,
                struct gs_character *character,
                double delta)
{
    struct gs_character *target = 0;

    assert(gs);
    assert(character);

    if (character->ai.attack_cd > 0) {
        character->ai.attack_cd -= delta; // * 100;
    }

    if (character->ai.idle_cd > 0) {
        character->ai.idle_cd -= delta; // * 100;
    }

    if (character->ai.target_id) {
        target = gs_character_find_by_id(gs, character->ai.target_id);
    }

    if (character->stats.hp <= 0 && character->revive_after_killed) {
        character->revive_after_cd -= delta; // * 100;
        log_normal("reviving timer: %f", character->revive_after_cd);
        if (character->revive_after_cd <= 0) {
            log_normal("revive!");
            // go back to the original value
            character->revive_after_cd = 50;
            gs_ai_on_revive(gs, character);
        }
    }

    // Check if we need to leave aggro state.
    if (character->ai.leave_agro_cd > 0 && target) {
        character->ai.leave_agro_cd -= delta; // * 100;
        if (character->ai.leave_agro_cd <= 0) {
            character->ai.leave_agro_cd = 0;
            // (franco.montenegro) How should we handle
            // the case where the target is null?
            // For instance, it got disconnected.
            gs_character_stop_auto_attack(gs, character, target);
            gs_character_stop_auto_attack(gs, target, character);
        }
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
                target->stats.hp = target->stats.hp > 30 ? target->stats.hp - 30
                                                         : 0;
                character->ai.state = AI_HAS_AGRO;

                // Make sure both, attacker and target
                // get their status updated.
                // (franco.montenegro) I think this should be broadcasted
                gs_character_send_status(gs, target, target);
                gs_character_send_status(gs, target, character);

                if (target->stats.hp == 0) {
                    gs_character_die(gs, target);
                    gs_ai_on_dead(gs, target, character);
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

void gs_ai_handle_request(struct gs_state *gs,
                          struct gs_character *character,
                          packet_t *request)
{
    enum request_type *allowed_by_state = 0;
    int can_be_handled                  = 0;

    assert(gs);
    assert(character);
    assert(request);

    allowed_by_state = g_actions_by_state[character->ai.state];

    // Find if the request can be handled by the current AI's state.
    for (size_t i = 0; i < macro_util_arr_len(g_actions_by_state[0]); i += 1) {
        if (allowed_by_state[i] == packet_type(request)) {
            can_be_handled = 1;
            break;
        }
    }

    log_normal("packet %x received.", packet_type(request));

    if (!can_be_handled) {
        log_normal("unable to handle packet %x by current state.",
                   packet_type(request));
        log_normal("current state: %d", character->ai.state);
        gs_character_action_failed(gs, character);
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
        macro_gs_ai_html_arr_msg(gs,
                                 character,
                                 "<html><body>And so... a chat window "
                                 "popped out. <a action=\"bypass -h "
                                 "npc_42_support player\">Magic "
                                 "support</a></body></html>");
        gs_character_spawn_random_orc(gs, &character->position);
        break;
    case request_type_bypass:
        gs_ai_handle_bypass_request(gs, character, request);
        macro_gs_ai_html_arr_msg(gs,
                                 character,
                                 "<html><body>It works! even though "
                                 "it breaks if you send more than 3kb "
                                 "of data!</body></html>");
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
