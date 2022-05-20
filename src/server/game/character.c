#include <assert.h>
#include "include/types.h"
#include "include/server_packet.h"
#include "include/character.h"
#include "include/client.h"

void character_move(struct client *src, struct char_pos *position)
{
	struct packet_move move = { 0 };

	assert(src);
	assert(position);

	move.id = 42;
	move.new_pos = *position;
	move.prev_pos = *position;

	packet_move_to(&src->response, &move);
	client_encrypt(src, &src->response);
}
