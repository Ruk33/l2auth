#ifndef INCLUDE_GS_CHARACTER_H
#define INCLUDE_GS_CHARACTER_H

#include "util.h"
#include "gs_types.h"
#include "gs_session.h"
#include "gs_packet_create_char_request.h"

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
static struct gs_character *gs_character_find_by_id(u32_t id);

// Move character to a point.
// The movement gets broadcasted to all players.
// Todo: make sure the packet gets broadcasted only to close players.
static void
gs_character_move(struct gs_character *character, struct gs_point *p);

// Attack and broadcast packet to all players.
// Todo: make sure the packet gets broadcasted only to close players.
static void
gs_character_attack(struct gs_character *attacker, struct gs_character *target);

// Selects a target (sending a packet to client).
static void gs_character_select_target(
        struct gs_character *character,
        struct gs_character *target);

// Sends the correct position of a character to the client.
static void gs_character_validate_position(struct gs_character *character);

// Set in memory characters. Used by gs_lib when the library
// gets reloaded (this way we don't lose the information).
// The game server is implemented as a library. This allows us
// to make changes, recompile and see updates live without
// having to shutdown the server and restart.
// Todo: deprecate
static void gs_character_set(struct gs_character *src, size_t *count);

// Todo: deprecate
static struct gs_character *gs_character_all(void);

// Todo: deprecate
static size_t gs_character_all_count(void);

// Utility function that fills dest character using
// parameters/values sent by the client (through src packet).
static void gs_character_from_request(
        struct gs_character *dest,
        gs_packet_create_char_request_t *src);

// Spawn new character (npc or playable) and broadcast packet to all players.
// Todo: make sure the packet is broadcasted only to close players.
static void gs_character_spawn(struct gs_character *src);

// Get character from session.
// If not found, NULL is returned.
static struct gs_character *
gs_character_from_session(struct gs_session *session);

#endif
