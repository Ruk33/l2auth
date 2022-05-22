#include "include/types.h"
#include "include/client.h"
#include "include/client_packet.h"

static void on_move(struct client *client)
{
	struct client_packet_move request = { 0 };

	assert(client);
	assert(client->character);

	client_packet_move_request_decode(&request, &client->request);
	character_move(client->character, &request.position);
}

// static void on_action(struct client *client);

// static void on_logout(struct client *client);

void player_on_request(struct client *client)
{
	assert(client);
	assert(client->character);

	switch (packet_type(&client->request)) {
	// In game requests.
	case 0x01: // Move
		on_move(client);
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
