#include <assert.h>
#include <stdio.h>
#include "../../include/util.h"
#include "../../include/packet.h"
#include "include/packet_decoder.h"
#include "include/packet_encoder.h"
#include "include/server.h"

static void on_gg_auth(struct state *state, struct session *session)
{
	assert(state);
	assert(session);

	struct gg_auth gg_auth = { 0 };

	printf("handling gg auth\n");
	gg_auth.gg_response = 0x0b; // Skip.

	printf("sending response to gg auth (skip it)\n");
	encode_gg_auth(&session->response, &gg_auth);
	session_encrypt_packet(session, &session->response);
}

static void on_auth_login(struct state *state, struct session *session)
{
	assert(state);
	assert(session);

	struct auth_login auth_login = { 0 };
	struct ok ok_response = { 0 };

	printf("handling auth login\n");
	decode_auth_login(&auth_login, &session->request.packet);
	printf("username: '%s' password: '%s'\n", auth_login.username.buf, auth_login.password.buf);
	TODO("validate username and password are corrects before granting access.");
	TODO("don't print username and password in console, this is just for debugging.");

	ok_response.login_ok1 = session->play_ok1;
	ok_response.login_ok2 = session->play_ok2;

	printf("sending packet ok.\n");
	encode_ok(&session->response, &ok_response);
	session_encrypt_packet(session, &session->response);
}

static void on_login_server(struct state *state, struct session *session)
{
	assert(state);
	assert(session);

	struct play_ok play_ok = { 0 };

	printf("handling login server.\n");

	play_ok.play_ok1 = session->play_ok1;
	play_ok.play_ok2 = session->play_ok2;

	printf("sending play ok response.\n");
	encode_play_ok(&session->response, &play_ok);
	session_encrypt_packet(session, &session->response);
}

static void on_request_server_list(struct state *state, struct session *session)
{
	assert(state);
	assert(session);

	struct server_list response = {0};
	struct ipv4 localhost = {{"127.0.0.1"}};
	u32 ip = 0;

	printf("handling request server list.\n");

	if (ipv4_to_u32(&ip, &localhost)) {
		response.count = 1;
	} else {
		response.count = 0;
		printf("unable to parse game server ip, no servers will be sent.\n");
	}

	response.servers[0].age_limit = 18;
	response.servers[0].brackets = 0;
	response.servers[0].extra = 0;
	response.servers[0].id = 1;
	response.servers[0].ip = ip;
	response.servers[0].max_players = 32;
	response.servers[0].players = 5;
	response.servers[0].port = 7777;
	response.servers[0].pvp = 1;
	response.servers[0].status = 1;
	TODO("don't hardcode game servers. sending %d (%s) as ip.", ip, localhost.buf);

	printf("sending server list.\n");
	encode_server_list(&session->response, &response);
	session_encrypt_packet(session, &session->response);
}

struct session *server_create_connection(struct state *state)
{
	assert(state);

	struct session *session = 0;
	struct init init = {0};

	// Todo: re-check from where does this session id comes from.
	struct session_id session_id = {{0xfd, 0x8a, 0x22, 0x00}};
	// Chronicle 4 protocol only :)
	struct protocol protocol = {{0x5a, 0x78, 0x00, 0x00}};

	printf("houston this is server here, we got a new client.\n");

	// Find free session
	for (size_t i = 0; i < ARR_LEN(state->sessions); i += 1) {
		if (state->sessions[i].active)
			continue;
		session = state->sessions + i;
		break;
	}

	if (!session) {
		printf("unable to get a free session instance.\n");
		return 0;
	}
	if (!session_init(session)) {
		printf("unable to initialize client.\n");
		session_release(session);
		return 0;
	}

	init.session_id = session_id;
	init.protocol = protocol;
	if (!session_rsa_modulus(&init.modulus, session)) {
		printf("unable to copy modulus. the client will be dropped.\n");
		session_release(session);
		return 0;
	}

	printf("sending init packet.\n");
	encode_init(&session->response, &init);

	return session;
}

void server_on_disconnect(struct state *state, struct session *src)
{
	assert(state);
	if (src)
		session_release(src);
}

void server_on_request(struct state *state, struct session *session)
{
	assert(state);
	assert(session);

	struct packet *packet = 0;

	packet = &session->request.packet;
	session->request.is_partial = packet_size(packet) != session->request.received;

	// Make sure the entire packet has arrived.
	if (session->request.is_partial)
		return;

	// We have received all the data. Reset the counter.
	session->request.received = 0;

	// If we have reached this point, the entire packet
	// has been received and it's ready to be handled.

	if (!session_decrypt_packet(session, packet)) {
		printf("houston, there was a problem decrypting the package.\n");
		// return;
	}

	printf("new packet received: %d\n", packet_type(packet));
	session->response = (struct packet) { 0 };

	switch (packet_type(packet)) {
	case 0x00: // Auth login
		on_auth_login(state, session);
		break;
	case 0x02: // Login server
		on_login_server(state, session);
		break;
	case 0x05: // Request server list
		on_request_server_list(state, session);
		break;
	case 0x07: // GG Auth
		on_gg_auth(state, session);
		break;
	default:
		printf("houston, we have a problem, i don't recognize that packet.\n");
		break;
	}

	*packet = (struct packet) { 0 };
}
