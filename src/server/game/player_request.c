#include "include/types.h"
#include "include/client.h"
#include "include/client_packet.h"
#include "include/server_packet.h"
#include "include/character.h"

static void on_move(struct state *state, struct client *client)
{
	struct client_packet_move request = { 0 };
	struct server_packet_move response = { 0 };
	struct packet packet = { 0 };

	assert(client);
	assert(client->character);

	client_packet_move_request_decode(&request, &client->request);
	character_start_movement(client->character, &request.position);

	// Broadcast movement to other players.
	response.id = client->character->id;
	response.prev_pos = client->character->position;
	response.new_pos = request.position;
	server_packet_move_encode(&packet, &response);
	state_broadcast_packet(state, &packet);
}

void player_on_request(struct state *state, struct client *client)
{
	assert(client);
	assert(client->character);

	switch (packet_type(&client->request)) {
	// In game requests.
	case 0x01: // Move
		on_move(state, client);
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

void player_on_tick(struct state *state, struct client *client, seconds delta)
{
	assert(state);
	assert(client);
	assert(client->character);
	character_tick(client->character, delta);
}
