#include <assert.h>
#include <stdio.h>
#include "../../include/util.h"
#include "../../include/packet.h"
#include "include/session.h"
#include "include/packet_encoder.h"
#include "include/packet_decoder.h"
#include "include/player.h"
#include "include/db.h"
#include "include/server.h"

static struct character_template character_templates[] = {
    // Human fighter
    {
		.race_id = 0,
		.class_id = 0,
		.attrs = {
			.str  = 40,
			.dex  = 30,
			.con  = 43,
			._int = 21,
			.wit  = 11,
			.men  = 25,
		},
	},
    
    // Human fighter copy
    // Not sure why the clients requires it...
    {
		.race_id = 0,
		.class_id = 0,
		.attrs = {
			.str  = 40,
			.dex  = 30,
			.con  = 43,
			._int = 21,
			.wit  = 11,
			.men  = 25,
		},
	},
    
    // Human mage
    {
		.race_id = 0,
		.class_id = 10,
		.attrs = {
			.str  = 22,
			.dex  = 21,
			.con  = 27,
			._int = 41,
			.wit  = 20,
			.men  = 39,
		},
	},
    
    // Elf fighter
    {
		.race_id = 1,
		.class_id = 18,
		.attrs = {
			.str  = 36,
			.dex  = 35,
			.con  = 36,
			._int = 23,
			.wit  = 14,
			.men  = 26,
		},
	},
    
    // Elf mage
    {
		.race_id = 1,
		.class_id = 25,
		.attrs = {
			.str  = 21,
			.dex  = 24,
			.con  = 25,
			._int = 37,
			.wit  = 23,
			.men  = 40,
		},
	},
    
    // Dark elf fighter
    {
		.race_id = 2,
		.class_id = 31,
		.attrs = {
			.str  = 41,
			.dex  = 34,
			.con  = 32,
			._int = 25,
			.wit  = 12,
			.men  = 26,
		},
	},
    
    // Dark elf mage
    {
		.race_id = 2,
		.class_id = 38,
		.attrs = {
			.str  = 23,
			.dex  = 23,
			.con  = 24,
			._int = 44,
			.wit  = 19,
			.men  = 37,
		},
	},
    
    // Orc fighter
    {
		.race_id = 3,
		.class_id = 44,
		.attrs = {
			.str  = 40,
			.dex  = 26,
			.con  = 47,
			._int = 18,
			.wit  = 12,
			.men  = 27,
		},
	},
    
    // Orc mage
    {
		.race_id = 3,
		.class_id = 49,
		.attrs = {
			.str  = 27,
			.dex  = 24,
			.con  = 31,
			._int = 31,
			.wit  = 15,
			.men  = 42,
		},
	},
    
    // Dwarf
    {
		.race_id = 4,
		.class_id = 53,
		.attrs = {
			.str  = 39,
			.dex  = 29,
			.con  = 45,
			._int = 20,
			.wit  = 10,
			.men  = 27,
		},
	},
};

static struct character_template *get_character_template_by_class(u32 class_id)
{
	for (size_t i = 0; i < ARR_LEN(character_templates); i += 1) {
		if (character_templates[i].class_id == class_id) {
			return character_templates + i;
		}
	}
	return 0;
}

static void on_protocol_version(struct state *state, struct session *session)
{
	assert(state);
	assert(session);

	struct protocol_version protocol = {0};    
	printf("handling protocol version.\n");
	encode_protocol_version(&session->response, &protocol);
}

static void on_auth_request(struct state *state, struct session *session)
{
	assert(state);
	assert(session);

	struct auth_request request = {0};
	struct auth_login response = {0};
    
	printf("handling auth request.\n");
	decode_auth_request(&request, &session->request);
	l2_string_to_char((char *) session->username.buf, request.username.buf, sizeof(session->username.buf));
    
	response.count = (u32) get_account_characters(
		response.characters,
		&session->username,
		ARR_LEN(response.characters)
	);
    
	encode_auth_login(&session->response, &response);
	encrypt_packet(session, &session->response);
}

static void on_select_char(struct state *state, struct session *session)
{
	assert(state);
	assert(session);

	struct selected_character request = {0};
	struct select_character response = {0};
	struct character characters[10] = {0};
    
	printf("handling char select.\n");
    
	decode_selected_character(&request, &session->request);
	
	get_account_characters(characters, &session->username, ARR_LEN(characters));
	struct character *character = character = &characters[request.index];
    
	TODO("add player to state.");
	// state_add_player(state, client, character);
    
	response.name = character->name;
	response.title = character->title;
	response.play_ok1 = 1994;
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
    
	encode_select_character(&session->response, &response);
	encrypt_packet(session, &session->response);
}

static void on_auto_ss_bsps(struct state *state, struct session *session)
{
	assert(state);
	assert(session);

	struct d0 d0 = {0};    
	printf("handling auto ss bsps.\n");
	encode_d0(&session->response, &d0);
	encrypt_packet(session, &session->response);
}

static void on_quest_list(struct state *state, struct session *session)
{
	assert(state);
	assert(session);

	struct quest_list quest = {0};    
	printf("handling quest list.\n");
	encode_quest_list(&session->response, &quest);
	encrypt_packet(session, &session->response);
}

static void on_enter_world(struct state *state, struct session *session)
{
	assert(state);
	assert(session);

	printf("handling enter world.\n");

	struct enter_world enter_world = {0};    
	enter_world.name = session->character->name;
	enter_world.title = session->character->title;
	enter_world.position = session->character->position;
	enter_world.id = session->character->id;
	enter_world.race_id = session->character->race_id;
	enter_world.sex = session->character->sex;
	enter_world.class_id = session->character->class_id;
	enter_world.level = session->character->level;
	enter_world.exp = session->character->exp;
	enter_world.attrs = session->character->attrs;
	enter_world.hp = session->character->hp;
	enter_world.mp = session->character->mp;
	enter_world.max_hp = session->character->max_hp;
	enter_world.max_mp = session->character->max_mp;
	enter_world.cp = session->character->cp;
	enter_world.max_cp = session->character->max_cp;
	enter_world.sp = session->character->sp;
	enter_world.exp = session->character->exp;
	enter_world.p_attack = session->character->p_attack;
	enter_world.m_attack = session->character->m_attack;
	enter_world.p_def = session->character->p_def;
	enter_world.m_def = session->character->m_def;
	enter_world.evasion_rate = session->character->evasion;
	enter_world.accuracy = session->character->accuracy;
	enter_world.critical_hit = session->character->critical_hit;
	enter_world.hair_style_id = session->character->hair_style_id;
	enter_world.hair_color_id = session->character->hair_color_id;
	enter_world.face_id = session->character->face_id;
	enter_world.inventory_limit = session->character->inventory_limit;
	enter_world.run_speed = session->character->run_speed;
	enter_world.walk_speed = session->character->walk_speed;
	enter_world.p_attack_speed = session->character->p_attack_speed;
	enter_world.m_attack_speed = session->character->m_attack_speed;
	enter_world.movement_speed_multiplier = session->character->movement_speed_multiplier;
	enter_world.attack_speed_multiplier = session->character->attack_speed_multiplier;
	enter_world.collision_radius = session->character->collision_radius;
	enter_world.collision_height = session->character->collision_height;
	enter_world.name_color = session->character->name_color;
	enter_world.current_load = session->character->current_load;
	enter_world.max_load = session->character->max_load;
    
	encode_enter_world(&session->response, &enter_world);
	encrypt_packet(session, &session->response);
}

static void on_show_creation_screen(struct state *state, struct session *session)
{
	assert(state);
	assert(session);

	struct show_creation_screen response = {0};
	// struct client_packet_create_character request = { 0 };
    
	// client_packet_create_char_decode(&request, &client->request);
	response.count = ARR_LEN(character_templates);
	for (u32 i = 0; i < response.count; i += 1) {
		response.templates[i] = character_templates[i];
	}
    
	encode_show_creation_screen(&session->response, &response);
	encrypt_packet(session, &session->response);
}

static void on_create_character(struct state *state, struct session *session)
{
	assert(state);
	assert(session);
	
	struct create_character_request request = {0};
	// struct server_packet_create_character response = { 0 };
	struct auth_login response = {0};
	struct character character = {0};
	struct character_template *template = 0;
        
	TODO(
		"When creating the character, check if the character's "
        "name is available, if not, send create_character "
        "response error."
	);
    
	decode_create_character_request(&request, &session->request);

	char name[32] = {0};
	l2_string_to_char(name, request.name.buf, sizeof(name));
    
	printf(
		"create new character with name %s, "
        "race: %d, "
		"sex: %d, "
		"class: %d, "
		"hair style: %d, "
		"hair color: %d, "
		"face: %d\n",
		name,
		request.race_id,
		request.sex,
		request.class_id,
		request.hair_style_id,
		request.hair_color_id,
		request.face_id
	);
    
	template = get_character_template_by_class(request.class_id);
    
	if (template) {
        printf("template found.\n");
		character.name = request.name;
		character.race_id = request.race_id;
		character.sex = request.sex;
		character.class_id = request.class_id;
		character.hair_style_id = request.hair_style_id;
		character.hair_color_id = request.hair_color_id;
		character.face_id = request.face_id;
		character.attrs = template->attrs;
        character.hp = 40;
        character.mp = 40;
        character.max_hp = 40;
        character.max_mp = 40;
        character.level = 1;
		// Talking island.
		character.position.x = -83968;
		character.position.y = 244634;
		character.position.z = -3730;
        
		save_character(&session->username, &character);
	}
    
	response.count = (u32) get_account_characters(
		response.characters,
		&session->username,
		ARR_LEN(response.characters)
	);
    
	encode_auth_login(&session->response, &response);
	encrypt_packet(session, &session->response);
}

struct session *server_on_new_connection(struct state *state)
{
	assert(state);
    
	// Find free session.
	struct session *session = 0;
	for (size_t i = 0; i < ARR_LEN(state->sessions); i += 1) {
		if (state->sessions[i].id)
			continue;
		session = state->sessions + i;
		break;
	}
    
	if (!session) {
		printf("no free client found.\n");
		return 0;
	}
    
	session_init(session);
	return session;
}

void server_on_disconnect(struct state *state, struct session *src)
{
	assert(state);
	assert(src);
	TODO("remove player from world if required when disconnected.");
	// state_release_client(state, src);
}

void server_on_request(struct state *state, struct session *session)
{
	assert(state);
	assert(session);
    
	printf("new chunk of request.\n");
	session->partial = packet_size(&session->request) != session->received;
	session->response = (struct packet) {0};
    
	if (session->partial) {
		printf("found partial chunk.\n");
		return;
	}
    
	session->received = 0;
	decrypt_packet(session, &session->request);
	session->conn_encrypted = 1;
    
	switch (packet_type(&session->request)) {
	case 0x00: // Protocol version
		on_protocol_version(state, session);
		break;
	case 0x08: // Auth request
		on_auth_request(state, session);
		break;
	case 0x0d: // Selected char.
		on_select_char(state, session);
		break;
	case 0xd0: // Auto ss bsps.
		on_auto_ss_bsps(state, session);
		break;
	case 0x63: // Quest list.
		on_quest_list(state, session);
		break;
	case 0x03: // Enter world.
		on_enter_world(state, session);
		break;
	case 0x0e: // From lobby, create new character
		on_show_creation_screen(state, session);
		break;
	case 0x0b: // Create new character
		on_create_character(state, session);
		break;
	default:
		if (session->character)
			handle_player_request(state, session);
		else
			printf("i don't recognize that packet, ignoring.\n");
		break;
	}
    
	session->request = (struct packet) {0};
}

void server_on_tick(struct state *state, seconds delta)
{
	assert(state);
	for (size_t i = 0; i < ARR_LEN(state->sessions); i += 1) {
		if (!state->sessions[i].character)
			continue;
		update_character(state, state->sessions[i].character, delta);
	}
}

void broadcast_packet(struct state *state, struct packet *packet)
{
	assert(state);
	assert(packet);

	// for (size_t i = 0; i < state->client_count; i += 1) {
	// 	struct session *session = state->sessions + i;
	// 	if (!session->conn_encrypted)
	// 		continue;
	// 	client_queue_response(client, packet);
	// }
}
