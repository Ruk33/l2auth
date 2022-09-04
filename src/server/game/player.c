#include "include/types.h"
#include "include/session.h"
#include "include/packet_decoder.h"
#include "include/packet_encoder.h"
#include "include/character.h"

static void on_move(struct state *state, struct session *session)
{
	assert(session);
	assert(session->character);

	struct move_request request = {0};
	struct move response = {0};

	printf("handling movement.\n");
	decode_move_request(&request, &session->request);
	character_move_towards(session->character, request.position);

	// Broadcast movement to other players.
	response.id = session->character->id;
	response.prev_position = session->character->position;
	response.new_position = request.position;
	encode_move(&session->response, &response);
	broadcast_packet(state, &session->response);
	encrypt_packet(session, &session->response);
}

void handle_player_request(struct state *state, struct session *session)
{
	assert(session);
	assert(session->character);

	switch (packet_type(&session->request)) {
	// In game requests.
	case 0x01: // Move
		on_move(state, session);
		break;
	case 0x04: // Action
	case 0x09: // Logout
	case 0x38: // Say
	case 0x46: // Restart
	case 0x48: // Validate position
	case 0xcd: // Show map
	case 0x21: // Bypass
	case 0x0a: // Attack
	case 0x6d: // Revive
	case 0x3f: // Skill list
	case 0x2f: // Skill use
		break;
	default:
		break;
	}
}

void update_player(struct state *state, struct session *session, seconds delta)
{
	assert(state);
	assert(session);
	assert(session->character);
	delta = delta;
	// character_tick(client->character, delta);
}
