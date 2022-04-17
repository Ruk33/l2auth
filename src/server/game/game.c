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
    struct packet_protocol_version protocol = { 0 };

    assert(state);
    assert(client);

    printf("handling protocol version.\n");
    packet_protocol_version_to(&client->response, &protocol);
}

static void on_auth_request(struct state *state, struct client *client)
{
    // struct packet_auth_request auth_request = { 0 };
    struct packet_auth_login auth_login = { 0 };

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

    client->partial = packet_size(&client->request) != client->received;

    if (client->partial) {
        printf("found partial chunk.\n");
        return;
    }

    client->received = 0;

    client_decrypt(client, &client->request);
    client->conn_encrypted = 1;
    client->response = (struct packet) { 0 };

    switch (packet_type(&client->request)) {
    case 0x00: // Protocol version
        on_protocol_version(state, client);
        break;
    case 0x08: // Auth request
        on_auth_request(state, client);
        break;
    default:
        printf("i don't recognize that packet, ignoring.\n");
        break;
    }

    client->request = (struct packet) { 0 };
}

void game_on_tick(struct state *state, seconds delta)
{
    assert(state);
    delta = delta;
}