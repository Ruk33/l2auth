#ifndef PLAYER_ENTITY_H
#define PLAYER_ENTITY_H

#include <stdlib.h>
#include "../client.h"
#include "../dto/vec3.h"
#include "../dto/character.h"
#include "../request_handler/create_char/request.h"

void player_create(struct Client *client, struct CreateCharRequest *request);

/**
 * Send current active player information
 * from client to another
 */
void player_send_info(struct Client *from, struct Client *to);

/**
 * Validate current active player from
 * client to another
 */
void player_validate_location_to(struct Client *from, struct Client *to, struct Vec3 *location, int heading);

/**
 * Move current active player from client
 * and notify to client
 */
void player_move_and_notify(struct Client *from, struct Client *to, struct Vec3 *prev_location, struct Vec3 *new_location);

void player_entity_action(struct Client *client, struct Character *target);

void player_say(struct Client *from, struct Client *to, char *msg, size_t msg_len);

void player_unsafe_say(struct Client *client, const char *msg);

#endif
