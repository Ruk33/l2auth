#ifndef INCLUDE_GS_CHARACTER_H
#define INCLUDE_GS_CHARACTER_H

#include "util.h"
#include "gs_types.h"
#include "gs_session.h"
#include "gs_packet_create_char_request.h"
#include "gs_packet_revive_request.h"

// Check if the character is a non playable character.
// Players are characters too, the difference is, a player
// is controller by a person.
static int gs_character_is_npc(struct gs_character *src);

static double
gs_character_angle_to_point(struct gs_character *src, struct gs_point *p);

// Get distance between a character and a point.
static double
gs_character_distance_from_point(struct gs_character *src, struct gs_point *p);

// Get distance between two players.
static double
gs_character_distance(struct gs_character *a, struct gs_character *b);

// Get character by id.
// If not found, NULL is returned.
static struct gs_character *
gs_character_find_by_id(struct gs_state *state, u32_t id);

// Sends hp and max hp info of from to to character.
// Todo: rename?
static void
gs_character_send_status(struct gs_character *from, struct gs_character *to);

// Revive and teleport character using option sent by client.
// Of course, this option has to be checked since
// the player can be cheating.
// By default, revive to village will be used.
static void gs_character_revive(
        struct gs_state *state,
        struct gs_character *src,
        enum gs_packet_revive_request_option where);

// Send move packet to client and broadcast it to all players.
// THIS FUNCTION WON'T update the character's actual position!
// Todo: make sure the packet gets broadcasted only to close players.
// Todo: maybe rename function to gs_character_notify_move?
static void gs_character_move(
        struct gs_state *state,
        struct gs_character *character,
        struct gs_point *p);

// Attack and broadcast packet to all players.
// Todo: make sure the packet gets broadcasted only to close players.
static void gs_character_attack(
        struct gs_state *state,
        struct gs_character *attacker,
        struct gs_character *target);

// Selects a target (sending a packet to client).
static void gs_character_select_target(
        struct gs_character *character,
        struct gs_character *target);

// Sends the correct position of a character to the client.
static void gs_character_validate_position(struct gs_character *character);

// Todo: remove function or move it to fun.c file?
static void gs_character_spawn_random_orc(struct gs_state *state);

// Utility function that fills dest character using
// parameters/values sent by the client (through src packet).
static void gs_character_from_request(
        struct gs_character *dest,
        gs_packet_create_char_request_t *src);

// Spawn new character (npc or playable) and broadcast packet to all players.
// Todo: make sure the packet is broadcasted only to close players.
static void
gs_character_spawn(struct gs_state *state, struct gs_character *src);

// Todo: make sure we broadcast this message!
static void
gs_character_restart(struct gs_state *state, struct gs_character *character);

// Get character from session.
// If not found, NULL is returned.
static struct gs_character *
gs_character_from_session(struct gs_state *state, struct gs_session *session);

static u32_t gs_character_get_free_id(struct gs_state *state);

// "Add" a character to the list of in game characters
// without broadcasting spawn packet.
// Only used for players when entering the game so the proper
// character id can be used for those initial packets
// sent when entering the world.
static void gs_character_add(struct gs_state *state, struct gs_character *src);

static void
gs_character_disconnect(struct gs_state *state, struct gs_character *src);

#endif
