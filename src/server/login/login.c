#include <assert.h>
#include <stdio.h>
#include "../../include/util.h"
#include "../../include/packet.h"
#include "include/server_packet.h"
#include "include/client_packet.h"
#include "include/state.h"
#include "include/login.h"

static void on_gg_auth(struct state *state, struct client *client)
{
    struct packet_gg_auth gg_auth = { 0 };

    assert(state);
    assert(client);

    printf("handling gg auth\n");
    gg_auth.gg_response = 0x0b; // Skip.

    printf("sending response to gg auth (skip it)\n");
    packet_gg_auth_to(&client->response, &gg_auth);
    client_encrypt_packet(client, &client->response);
}

static void on_auth_login(struct state *state, struct client *client)
{
    struct packet_auth_login auth_login = { 0 };
    struct packet_ok ok_response = { 0 };

    assert(state);
    assert(client);

    printf("handling auth login\n");
    packet_auth_login_from(&auth_login, &client->request.packet);
    printf(
        "username: '%s' password: '%s'\n",
        auth_login.username.buf,
        auth_login.password.buf
    );
    TODO("validate username and password are corrects before granting access.");

    ok_response.loginOK1 = client->playOK1;
    ok_response.loginOK2 = client->playOK2;

    printf("sending packet ok.\n");
    packet_ok_to(&client->response, &ok_response);
    client_encrypt_packet(client, &client->response);
}

static void on_login_server(struct state *state, struct client *client)
{
    struct packet_play_ok play_ok = { 0 };

    assert(state);
    assert(client);

    printf("handling login server.\n");

    play_ok.playOK1 = client->playOK1;
    play_ok.playOK2 = client->playOK2;

    printf("sending play ok response.\n");
    packet_play_ok_to(&client->response, &play_ok);
    client_encrypt_packet(client, &client->response);
}

static void on_request_server_list(struct state *state, struct client *client)
{
    struct packet_server_list server_list = { 0 };
    struct ipv4 localhost = {{ "127.0.0.1" }};
    u32 ip = 0;

    assert(state);
    assert(client);

    printf("handling request server list.\n");

    if (ipv4_to_u32(&ip, &localhost)) {
        server_list.count = 1;
    } else {
        server_list.count = 0;
        printf("unable to parse game server ip, no servers will be sent.\n");
    }

    server_list.servers[0].age_limit = 18;
    server_list.servers[0].brackets = 0;
    server_list.servers[0].extra = 0;
    server_list.servers[0].id = 1;
    server_list.servers[0].ip = ip;
    server_list.servers[0].max_players = 32;
    server_list.servers[0].players = 5;
    server_list.servers[0].port = 7777;
    server_list.servers[0].pvp = 1;
    server_list.servers[0].status = 1;
    TODO("don't hardcode game servers. sending %d (127.0.0.1) as ip.", ip);

    printf("sending server list.\n");
    packet_server_list_to(&client->response, &server_list);
    client_encrypt_packet(client, &client->response);
}

struct client *login_on_new_connection(struct state *state)
{
    struct client *client = 0;
    struct packet_init init = { 0 };

    // Todo: re-check from where does this session id comes from.
    struct session_id session_id = {{ 0xfd, 0x8a, 0x22, 0x00 }};
    // Chronicle 4 protocol only :)
    struct protocol protocol = {{ 0x5a, 0x78, 0x00, 0x00 }};

    assert(state);
    printf("houston this is server here, we got a new client.\n");

    client = state_get_free_client(state);
    if (!client) {
        printf("unable to get a free client instance.\n");
        return 0;
    }
    if (!client_init(client)) {
        printf("unable to initialize client.\n");
        state_release_client(state, client);
        return 0;
    }

    init.session_id = session_id;
    init.protocol = protocol;
    if (!client_rsa_modulus(client, &init.modulus)) {
        printf("unable to copy modulus. the client will be dropped.\n");
        state_release_client(state, client);
        return 0;
    }

    printf("sending init packet.\n");
    packet_init_to(&client->response, &init);
    return client;
}

void login_on_disconnect(struct state *state, struct client *src)
{
    assert(state);

    if (!src) {
        return;
    }

    state_release_client(state, src);
}

void login_on_request(struct state *state, struct client *client)
{
    struct packet *packet = 0;

    assert(state);
    assert(client);

    packet = &client->request.packet;
    client->request.is_partial = packet_size(packet) != client->request.received;

    // Make sure the entire packet has arrived.
    if (client->request.is_partial) {
        printf("partial request found\n");
        return;
    }

    // We have received all the data. Reset the counter.
    client->request.received = 0;

    // If we have reached this point, the entire packet
    // has been received and it's ready to be handled.

    if (!client_decrypt_packet(client, packet)) {
        printf("houston, there was a problem decrypting the package.\n");
        // return;
    }

    printf("new packet received: %d\n", packet_type(packet));
    client->response = (struct packet) { 0 };

    switch (packet_type(packet)) {
    case 0x00: // Auth login
        on_auth_login(state, client);
        break;
    case 0x02: // Login server
        on_login_server(state, client);
        break;
    case 0x05: // Request server list
        on_request_server_list(state, client);
        break;
    case 0x07: // GG Auth
        on_gg_auth(state, client);
        break;
    default:
        printf("houston, we have a problem, i don't recognize that packet.\n");
        break;
    }

    *packet = (struct packet) { 0 };
}