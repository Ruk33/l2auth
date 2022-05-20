#include <assert.h>
#include <stdio.h>
#include "../../include/util.h"
#include "../../include/packet.h"
#include "include/state.h"
#include "include/client.h"
#include "include/server_packet.h"
#include "include/client_packet.h"
#include "include/game.h"

static void on_protocol_version(struct state *state, struct client *client)
{
	struct packet_protocol_version protocol = {0};

	assert(state);
	assert(client);

	printf("handling protocol version.\n");
	packet_protocol_version_to(&client->response, &protocol);
}

static void on_auth_request(struct state *state, struct client *client)
{
	// struct packet_auth_request auth_request = { 0 };
	struct packet_auth_login auth_login = {0};

	assert(state);
	assert(client);

	printf("handling auth request.\n");

	auth_login.count = 1;
	auth_login.characters[0].playOK1 = 1994;
	auth_login.characters[0].active = 1;
	auth_login.characters[0].class_id = 1;
	auth_login.characters[0].race_id = 1;
	auth_login.characters[0].face = 1;
	auth_login.characters[0].hair_color_id = 1;
	auth_login.characters[0].hair_style_id = 1;
	auth_login.characters[0].hp = 100;
	auth_login.characters[0].id = 99;
	auth_login.characters[0].level = 1;
	auth_login.characters[0].max_hp = 100;
	auth_login.characters[0].max_mp = 100;
	auth_login.characters[0].mp = 10;
	auth_login.characters[0].sex = 1;
	auth_login.characters[0].position.x = 99;
	auth_login.characters[0].position.y = 99;
	auth_login.characters[0].position.z = 99;
	auth_login.characters[0].sp = 10;
	auth_login.characters[0].exp = 10;

	packet_auth_login_to(&client->response, &auth_login);
	client_encrypt(client, &client->response);
}

static void on_select_char(struct state *state, struct client *client)
{
	struct packet_char_select_request request = {0};
	struct packet_char_select char_select = {0};
	struct character character = {0};

	assert(state);
	assert(client);

	printf("handling char select.\n");

	packet_char_select_request_from(&request, &client->request);
	state_add_player(state, client, &character);

	L2_STRING_FROM_CHAR(char_select.name.buf, "franco");
	L2_STRING_FROM_CHAR(char_select.title.buf, "yep");
	char_select.playOK1 = 1994;
	char_select.id = 1;
	char_select.active = 1;
	char_select.race_id = 1;
	char_select.class_id = 1;
	char_select.exp = 10;
	char_select.sp = 10;
	char_select.level = 1;
	char_select.hp = 100;
	char_select.mp = 100;
	char_select.attrs.con = 10;
	char_select.attrs.dex = 10;
	char_select.attrs.men = 10;
	char_select.attrs.str = 10;
	char_select.attrs.wit = 10;
	char_select.attrs._int = 10;
	char_select.sex = 1;
	// Talking Island
	char_select.position.x = -83968;
	char_select.position.y = 244634;
	char_select.position.z = -3730;
	char_select.game_time = 42;

	packet_char_select_to(&client->response, &char_select);
	client_encrypt(client, &client->response);
}

static void on_auto_ss_bsps(struct state *state, struct client *client)
{
	struct packet_d0 d0 = {0};

	assert(state);
	assert(client);

	printf("handling auto ss bsps.\n");
	packet_d0_to(&client->response, &d0);
	client_encrypt(client, &client->response);
}

static void on_quest_list(struct state *state, struct client *client)
{
	struct packet_quest_list quest = {0};

	assert(state);
	assert(client);

	printf("handling quest list.\n");
	packet_quest_list_to(&client->response, &quest);
	client_encrypt(client, &client->response);
}

static void on_enter_world(struct state *state, struct client *client)
{
	struct packet_enter_world enter_world = {0};

	assert(state);
	assert(client);

	printf("handling enter world.\n");

	L2_STRING_FROM_CHAR(enter_world.name.buf, "franco");
	enter_world.position.heading = 1;
	enter_world.position.x = -83968;
	enter_world.position.y = 244634;
	enter_world.position.z = -3730;
	enter_world.id = 1;
	enter_world.race_id = 1;
	enter_world.sex = 1;
	enter_world.class_id = 1;
	enter_world.level = 1;
	enter_world.exp = 1;
	enter_world.attrs.str = 10;
	enter_world.attrs.dex = 10;
	enter_world.attrs.con = 10;
	enter_world.attrs._int = 10;
	enter_world.attrs.wit = 10;
	enter_world.attrs.men = 10;
	enter_world.hp = 10;
	enter_world.mp = 10;
	enter_world.max_hp = 10;
	enter_world.max_mp = 10;
	enter_world.cp = 10;
	enter_world.max_cp = 10;
	enter_world.sp = 10;
	enter_world.exp = 10;
	enter_world.p_attack = 10;
	enter_world.m_attack = 10;
	enter_world.p_def = 10;
	enter_world.m_def = 10;
	enter_world.evasion_rate = 10;
	enter_world.accuracy = 10;
	enter_world.critical_hit = 10;
	enter_world.hair_style_id = 1;
	enter_world.hair_color_id = 1;
	enter_world.face = 1;
	enter_world.inventory_limit = 10;
	enter_world.run_speed = 10;
	enter_world.walk_speed = 10;
	enter_world.p_attack_speed = 10;
	enter_world.m_attack_speed = 10;
	enter_world.movement_speed_multiplier = 1;
	enter_world.attack_speed_multiplier = 1;
	enter_world.collision_radius = 10;
	enter_world.collision_height = 10;
	enter_world.name_color = 0xFFFFFF;
	enter_world.current_load = 1;
	enter_world.max_load = 10;

	packet_enter_world_to(&client->response, &enter_world);
	client_encrypt(client, &client->response);
}

struct client *game_on_new_connection(struct state *state)
{
	struct client *client = 0;

	assert(state);

	client = state_get_free_client(state);

	if (!client) {
		printf("no free client found.\n");
		return 0;
	}

	client_init(client);

	return client;
}

void game_on_disconnect(struct state *state, struct client *src)
{
	assert(state);
	assert(src);
}

void game_on_request(struct state *state, struct client *client)
{
	assert(state);
	assert(client);

	printf("new chunk of request.\n");
	client->partial = packet_size(&client->request) != client->received;
	client->response = (struct packet){0};

	if (client->partial) {
		printf("found partial chunk.\n");
		return;
	}

	client->received = 0;
	client_decrypt(client, &client->request);
	client->conn_encrypted = 1;

	switch (packet_type(&client->request)) {
	case 0x00: // Protocol version
		on_protocol_version(state, client);
		break;
	case 0x08: // Auth request
		on_auth_request(state, client);
		break;
	case 0x0d: // Selected char.
		on_select_char(state, client);
		break;
	case 0xd0: // Auto ss bsps.
		on_auto_ss_bsps(state, client);
		break;
	case 0x63: // Quest list.
		on_quest_list(state, client);
		break;
	case 0x03: // Enter world.
		on_enter_world(state, client);
		break;
	case 0x0e: // New character
		break;
	default:
		if (client->character) {
			player_on_request(client);
		} else {
			printf("i don't recognize that packet, ignoring.\n");
		}
		break;
	}

	client->request = (struct packet){0};
}

void game_on_tick(struct state *state, seconds delta)
{
	assert(state);
	delta = delta;
}
