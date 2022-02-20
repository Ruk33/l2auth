#ifndef INCLUDE_GS_API_H
#define INCLUDE_GS_API_H

// This file is intended to contain all the relevant
// APIs of the project. This way, functions/APIs can
// be easily found without having to jump between files.
// Each function's prefix represents the file where it's
// implementation resides. For example, gs_character_is_npc
// lives in gs_character.c.

#include "gs_types.h"

// Checks if a character is a NPC (non playable character, such as merchants)
int gs_character_is_npc(struct gs_character *src);

// Find character by id.
// If not found, NULL is returned.
struct gs_character *gs_character_find_by_id(struct gs_state *gs, u32_t id);

// Could be renamed to leave world.
// Todo: For now, this function doesn't do much, but it should
// broadcast a disconnect packet.
void gs_character_disconnect(struct gs_state *gs, struct gs_character *src);

// Spawn new character (NPC or playable) and broadcast packet to all players.
void gs_character_spawn(struct gs_state *gs, struct gs_character *spawning);

// Sends the correct/server position of a character to a client.
// The client asks the server every 1 sec. the correct player's
// position. We can fix/sync a player's position by sending this
// packet.
void gs_character_validate_position(struct gs_state *gs,
                                    struct gs_character *src,
                                    struct gs_character *to);

void gs_character_action_failed(struct gs_state *gs, struct gs_character *src);

// NOT WORKING yet.
void gs_character_face_to(struct gs_state *gs,
                          struct gs_character *src,
                          i32_t degree);

// Chat/send a message from a character. It also works with NPCs.
void gs_character_say(struct gs_state *gs,
                      struct gs_character *from,
                      char *message,
                      size_t message_size);

// Sends HP and MAX HP info of from to to character.
// Todo: Implement the rest of the status we can send (MP, CP, etc.)
void gs_character_send_status(struct gs_state *gs,
                              struct gs_character *from,
                              struct gs_character *to);

// Revive and teleport character using option sent by client.
// By default, revive to village will be used.
void gs_character_revive(struct gs_state *gs,
                         struct gs_character *src,
                         enum gs_packet_revive_request_option where);

// Sends all the skills a character has when opening the skill tab.
void gs_character_send_skill_list(struct gs_state *gs,
                                  struct gs_character *src);

void gs_character_use_skill(struct gs_state *gs, struct gs_character *src, struct gs_skill *skill);

// When a player clicks to move, this is the packet to sent
// and broadcast to other players so everyone gets notified
// that a particular player/character will start moving to
// a location.
// THIS FUNCTION WON'T update the character's actual position!
// See gs_ctrl_move to move a character.
void gs_character_target_point(struct gs_state *gs,
                               struct gs_character *character,
                               struct gs_point *which_point);

// Todo: Rename.
// The function's name doesn't represent what the function does.
// The only thing that it does is to broadcast the character's death
// and display the "resurrect window"
void gs_character_die(struct gs_state *gs, struct gs_character *src);

// Launches auto attack.
// The damage won't be done until the hit reaches the target,
// which is performed in the game server tick function.
void gs_character_launch_attack(struct gs_state *gs,
                                struct gs_character *attacker,
                                struct gs_character *target);

// Stops/leaves aggro state by stopping the auto attack.
void gs_character_stop_auto_attack(struct gs_state *gs,
                                   struct gs_character *attacker,
                                   struct gs_character *target);

void gs_character_select_target(struct gs_state *gs,
                                struct gs_character *character,
                                struct gs_character *target);

void gs_character_restart(struct gs_state *gs, struct gs_character *character);

void gs_character_show_html_message(struct gs_state *gs,
                                    struct gs_character *character,
                                    char *message,
                                    size_t message_size);

// Change character's movement type to be walking
// and broadcast the packet so everyone is notified.
void gs_character_switch_to_walk(struct gs_state *gs, struct gs_character *src);

// Change character's movement type to be running
// and broadcast the packet so everyone is notified.
void gs_character_switch_to_run(struct gs_state *gs, struct gs_character *src);

// Get a character from a session.
// If not found, NULL is returned.
struct gs_character *gs_character_from_session(struct gs_state *gs,
                                               struct gs_session *session);

void gs_ctrl_go_idle(struct gs_state *gs, struct gs_character *src);

// Starts movement so character src moves to a point.
void gs_ctrl_move(struct gs_state *gs,
                  struct gs_character *src,
                  struct gs_point *where);

// Move src character closer to target if required.
// If the src is too far, a movement behavior will begin and 1 will be returned.
// If no movement is required, nothing happens and 0 is returned.
int gs_ctrl_move_to_interact_with(struct gs_state *gs,
                                  struct gs_character *src,
                                  struct gs_character *target);

// Starts an attack from attacker to target.
// The damage will be done when the attack reaches the target.
// This gets accomplished in the tick function of the game server.
void gs_ctrl_attack(struct gs_state *gs,
                    struct gs_character *attacker,
                    struct gs_character *target);

// Try to interact with target.
// If the target doesn't exist, src will go back to idle.
// If the target is too far away, src will walk closer to the target.
void gs_ctrl_interact(struct gs_state *gs,
                      struct gs_character *src,
                      struct gs_character *target);

void gs_ctrl_select_target(struct gs_state *gs,
                           struct gs_character *src,
                           struct gs_character *target);

// Initiates a wondering/idle walk for an NPC
// Makes it look more alive instead of standing idle in
// the same place.
void gs_ctrl_npc_initiate_idle_walk(struct gs_state *gs,
                                    struct gs_character *npc);

// (franco.montenegro) Not sure if this function should be exposed as API.
void gs_ctrl_revive(struct gs_state *gs,
                    struct gs_character *src,
                    enum gs_packet_revive_request_option option);

void gs_ctrl_die(struct gs_state *gs,
                 struct gs_character *dead,
                 struct gs_character *killer);

#endif
