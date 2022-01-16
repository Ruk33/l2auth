#ifndef INCLUDE_GS_CHARACTER_H
#define INCLUDE_GS_CHARACTER_H

#include "util.h"
#include "gs_types.h"
#include "gs_session.h"
#include "gs_client_packets.h"

int gs_character_is_npc(struct gs_character *src);

// Get character by id.
// If not found, NULL is returned.
struct gs_character *gs_character_find_by_id(struct gs_state *gs, u32_t id);

// Broadcast a chat message.
// Todo: make sure packet is only broadcasted to close players.
void gs_character_say(struct gs_state *gs,
                      struct gs_character *from,
                      char *message,
                      size_t message_size);

// Sends hp and max hp info of from to to character.
// Todo: rename?
void gs_character_send_status(struct gs_state *gs,
                              struct gs_character *from,
                              struct gs_character *to);

// Revive and teleport character using option sent by client.
// Of course, this option has to be checked since
// the player can be cheating.
// By default, revive to village will be used.
void gs_character_revive(struct gs_state *gs,
                         struct gs_character *src,
                         enum gs_packet_revive_request_option where);

// Send move packet to client and broadcast it to all players.
// THIS FUNCTION WON'T update the character's actual position!
// Todo: make sure the packet gets broadcasted only to close players.
// Todo: maybe rename function to gs_character_notify_move?
void gs_character_move(struct gs_state *gs,
                       struct gs_character *character,
                       struct gs_point *p);

// Launches auto attack.
// The damage won't be done until the hit reaches the target,
// which is performed in the game server tick function.
void gs_character_launch_attack(struct gs_state *gs,
                         struct gs_character *attacker,
                         struct gs_character *target);

// (franco.montenegro) Rename. The function's name
// doesn't represent what the function does. The only
// thing that it does is to broadcast the character's death
// and display the "resurrect window"
void gs_character_die(struct gs_state *state, struct gs_character *character);

void gs_character_stop_auto_attack(struct gs_state *gs,
                                   struct gs_character *attacker,
                                   struct gs_character *target);

// Selects a target (sending a packet to client).
void gs_character_select_target(struct gs_state *gs,
                                struct gs_character *character,
                                struct gs_character *target);

// Sends the correct position of a character to the client.
void gs_character_validate_position(struct gs_state *gs,
                                    struct gs_character *character);

// Todo: remove function or move it to fun.c file?
void gs_character_spawn_random_orc(struct gs_state *gs,
                                   struct gs_point *location);

// Utility function that fills dest character using
// parameters/values sent by the client (through src packet).
void gs_character_from_request(struct gs_character *dest,
                               struct gs_packet_create_char_request *src);

// Spawn new character (npc or playable) and broadcast packet to all players.
// Todo: make sure the packet is broadcasted only to close players.
void gs_character_spawn(struct gs_state *gs, struct gs_character *src);

// Todo: make sure we broadcast this message!
void gs_character_restart(struct gs_state *gs, struct gs_character *character);

void gs_character_show_npc_html_message(struct gs_state *gs,
                                        struct gs_character *character,
                                        char *message,
                                        size_t message_size);

// Change character's movement type to be walking.
void gs_character_walk(struct gs_state *gs, struct gs_character *src);

// Change character's movement type to be running.
void gs_character_run(struct gs_state *gs, struct gs_character *src);

// Get character from session.
// If not found, NULL is returned.
struct gs_character *gs_character_from_session(struct gs_state *gs,
                                               struct gs_session *session);

u32_t gs_character_get_free_id(struct gs_state *gs);

// "Add" a character to the list of in game characters
// without broadcasting spawn packet.
// Only used for players when entering the game so the proper
// character id can be used for those initial packets
// sent when entering the world.
void gs_character_add(struct gs_state *gs, struct gs_character *src);

void gs_character_disconnect(struct gs_state *gs, struct gs_character *src);

#endif
