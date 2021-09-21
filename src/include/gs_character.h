#ifndef INCLUDE_GS_CHARACTER_H
#define INCLUDE_GS_CHARACTER_H

#include "util.h"
#include "gs_types.h"
#include "gs_session.h"
#include "gs_client_packets.h"

// Check if the character is a non playable character.
// Players are characters too, the difference is, a player
// is controller by a person.
int gs_character_is_npc(struct gs_character *src);

double
gs_character_angle_to_point(struct gs_character *src, struct gs_point *p);

// Get distance between a character and a point.
double
gs_character_distance_from_point(struct gs_character *src, struct gs_point *p);

// Get distance between two players.
double gs_character_distance(struct gs_character *a, struct gs_character *b);

// Get character by id.
// If not found, NULL is returned.
struct gs_character *gs_character_find_by_id(struct gs_state *gs, u32_t id);

// Broadcast a chat message.
// Todo: make sure packet is only broadcasted to close players.
void gs_character_say(
        struct gs_state *gs,
        struct gs_character *from,
        char *message,
        size_t message_size);

// Sends hp and max hp info of from to to character.
// Todo: rename?
void gs_character_send_status(
        struct gs_state *gs,
        struct gs_character *from,
        struct gs_character *to);

// Revive and teleport character using option sent by client.
// Of course, this option has to be checked since
// the player can be cheating.
// By default, revive to village will be used.
void gs_character_revive(
        struct gs_state *gs,
        struct gs_character *src,
        enum gs_packet_revive_request_option where);

// Send move packet to client and broadcast it to all players.
// THIS FUNCTION WON'T update the character's actual position!
// Todo: make sure the packet gets broadcasted only to close players.
// Todo: maybe rename function to gs_character_notify_move?
void gs_character_move(
        struct gs_state *gs,
        struct gs_character *character,
        struct gs_point *p);

// Attack and broadcast packet to all players.
// Todo: make sure the packet gets broadcasted only to close players.
void gs_character_attack(
        struct gs_state *gs,
        struct gs_character *attacker,
        struct gs_character *target);

void gs_character_stop_auto_attack(
        struct gs_state *gs,
        struct gs_character *attacker,
        struct gs_character *target);

// Selects a target (sending a packet to client).
void gs_character_select_target(
        struct gs_state *gs,
        struct gs_character *character,
        struct gs_character *target);

// Sends the correct position of a character to the client.
void gs_character_validate_position(
        struct gs_state *gs,
        struct gs_character *character);

// Todo: remove function or move it to fun.c file?
void gs_character_spawn_random_orc(
        struct gs_state *gs,
        struct gs_point *location);

// Utility function that fills dest character using
// parameters/values sent by the client (through src packet).
void gs_character_from_request(
        struct gs_character *dest,
        struct gs_packet_create_char_request *src);

// Spawn new character (npc or playable) and broadcast packet to all players.
// Todo: make sure the packet is broadcasted only to close players.
void gs_character_spawn(struct gs_state *gs, struct gs_character *src);

// Todo: make sure we broadcast this message!
void gs_character_restart(struct gs_state *gs, struct gs_character *character);

void gs_character_show_npc_html_message(
        struct gs_state *gs,
        struct gs_character *character,
        char *message,
        size_t message_size);

// Change character's movement type to be walking.
void gs_character_walk(struct gs_state *gs, struct gs_character *src);

// Change character's movement type to be running.
void gs_character_run(struct gs_state *gs, struct gs_character *src);

// Get character from session.
// If not found, NULL is returned.
struct gs_character *
gs_character_from_session(struct gs_state *gs, struct gs_session *session);

u32_t gs_character_get_free_id(struct gs_state *gs);

// "Add" a character to the list of in game characters
// without broadcasting spawn packet.
// Only used for players when entering the game so the proper
// character id can be used for those initial packets
// sent when entering the world.
void gs_character_add(struct gs_state *gs, struct gs_character *src);

void gs_character_disconnect(struct gs_state *gs, struct gs_character *src);

#endif
