#include <assert.h>
#include <stdio.h>
#include "../../include/util.h"
#include "../../include/packet.h"
#include "include/state.h"
#include "include/client.h"
#include "include/server_packet.h"
#include "include/client_packet.h"
#include "include/player_request.h"
#include "include/db.h"
#include "include/server.h"

static void on_protocol_version(struct state *state, struct client *client)
{
	struct server_packet_protocol_version protocol = {0};

	assert(state);
	assert(client);

	printf("handling protocol version.\n");
	server_packet_protocol_version_encode(&client->response, &protocol);
}

static void on_auth_request(struct state *state, struct client *client)
{
	struct client_packet_auth request = { 0 };
	struct server_packet_auth_login response = { 0 };

	assert(state);
	assert(client);

	printf("handling auth request.\n");
	client_packet_auth_decode(&request, &client->request);
	l2_string_to_char(client->username.buf, request.username.buf, sizeof(client->username.buf));

	response.count = (u32) db_get_account_characters(
		response.characters,
		&client->username,
		ARR_LEN(response.characters)
	);

	server_packet_auth_login_encode(&client->response, &response);
	client_encrypt(client, &client->response);
}

static void on_select_char(struct state *state, struct client *client)
{
	struct client_packet_select_character request = { 0 };
	struct server_packet_select_character response = { 0 };
	struct character characters[10] = { 0 };
	struct character *character = 0;

	assert(state);
	assert(client);

	printf("handling char select.\n");

	client_packet_select_character_decode(&request, &client->request);
	
	db_get_account_characters(characters, &client->username, ARR_LEN(characters));
	character = &characters[request.index];

	state_add_player(state, client, character);

	response.name = character->name;
	response.title = character->title;
	response.playOK1 = 1994;
	response.id = character->id;
	response.active = character->active;
	response.race_id = character->race_id;
	response.class_id = character->class_id;
	response.exp = character->exp;
	response.sp = character->sp;
	response.level = character->level;
	response.hp = character->hp;
	response.mp = character->mp;
	response.attrs = character->attrs;
	response.sex = character->sex;
	response.position = character->position;

	server_packet_select_character_encode(&client->response, &response);
	client_encrypt(client, &client->response);
}

static void on_auto_ss_bsps(struct state *state, struct client *client)
{
	struct server_packet_d0 d0 = {0};

	assert(state);
	assert(client);

	printf("handling auto ss bsps.\n");
	server_packet_d0_encode(&client->response, &d0);
	client_encrypt(client, &client->response);
}

static void on_quest_list(struct state *state, struct client *client)
{
	struct server_packet_quest_list quest = {0};

	assert(state);
	assert(client);

	printf("handling quest list.\n");
	server_packet_quest_list_encode(&client->response, &quest);
	client_encrypt(client, &client->response);
}

static void on_enter_world(struct state *state, struct client *client)
{
	struct server_packet_enter_world enter_world = { 0 };

	assert(state);
	assert(client);

	printf("handling enter world.\n");

	enter_world.name = client->character->name;
	enter_world.title = client->character->title;
	enter_world.position = client->character->position;
	enter_world.id = client->character->id;
	enter_world.race_id = client->character->race_id;
	enter_world.sex = client->character->sex;
	enter_world.class_id = client->character->class_id;
	enter_world.level = client->character->level;
	enter_world.exp = client->character->exp;
	enter_world.attrs = client->character->attrs;
	enter_world.hp = client->character->hp;
	enter_world.mp = client->character->mp;
	enter_world.max_hp = client->character->max_hp;
	enter_world.max_mp = client->character->max_mp;
	enter_world.cp = client->character->cp;
	enter_world.max_cp = client->character->max_cp;
	enter_world.sp = client->character->sp;
	enter_world.exp = client->character->exp;
	enter_world.p_attack = client->character->p_attack;
	enter_world.m_attack = client->character->m_attack;
	enter_world.p_def = client->character->p_def;
	enter_world.m_def = client->character->m_def;
	enter_world.evasion_rate = client->character->evasion;
	enter_world.accuracy = client->character->accuracy;
	enter_world.critical_hit = client->character->critical_hit;
	enter_world.hair_style_id = client->character->hair_style_id;
	enter_world.hair_color_id = client->character->hair_color_id;
	enter_world.face = client->character->face;
	enter_world.inventory_limit = client->character->inventory_limit;
	enter_world.run_speed = client->character->run_speed;
	enter_world.walk_speed = client->character->walk_speed;
	enter_world.p_attack_speed = client->character->p_attack_speed;
	enter_world.m_attack_speed = client->character->m_attack_speed;
	enter_world.movement_speed_multiplier = client->character->movement_speed_multiplier;
	enter_world.attack_speed_multiplier = client->character->attack_speed_multiplier;
	enter_world.collision_radius = client->character->collision_radius;
	enter_world.collision_height = client->character->collision_height;
	enter_world.name_color = client->character->name_color;
	enter_world.current_load = client->character->current_load;
	enter_world.max_load = client->character->max_load;

	server_packet_enter_world_encode(&client->response, &enter_world);
	client_encrypt(client, &client->response);
}

struct client *server_on_new_connection(struct state *state)
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

void server_on_disconnect(struct state *state, struct client *src)
{
	assert(state);
	assert(src);
	TODO("remove player from world if required when disconnected.");
	state_release_client(state, src);
}

void server_on_request(struct state *state, struct client *client)
{
	assert(state);
	assert(client);

	printf("new chunk of request.\n");
	client->partial = packet_size(&client->request) != client->received;
	client->response = (struct packet) { 0 };

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
		if (client->character)
			player_on_request(state, client);
		else
			printf("i don't recognize that packet, ignoring.\n");
		break;
	}

	client->request = (struct packet) { 0 };
}

void server_on_tick(struct state *state, seconds delta)
{
	assert(state);
	for (size_t i = 0; i < state->client_count; i += 1) {
		if (!state->clients[i].character)
			continue;
		player_on_tick(state, &state->clients[i], delta);
	}
}
