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

    client->response = (struct packet) { 0 };
    packet_gg_auth_to(&client->response, &gg_auth);

    client_encrypt_packet(
        client,
        &B(client->response.buf, sizeof(client->response.buf), 0),
        &client->response
    );
}

static void on_auth_login(struct state *state, struct client *client)
{
    struct packet_auth_login auth_login = { 0 };

    assert(state);
    assert(client);

    printf("handling auth login\n");
    packet_auth_login_from(&auth_login, &client->request.packet);
    printf("username: '%s' password: '%s'\n", auth_login.username.buf, auth_login.password.buf);
}

struct client *login_on_new_connection(struct state *state)
{
    struct client *client = 0;
    struct packet_init init = { 0 };

    // Todo: re-check from where does this session id comes from.
    struct session_id session_id = { 0xfd, 0x8a, 0x22, 0x00 };
    // Chronicle 4 protocol only :)
    struct protocol protocol = { 0x5a, 0x78, 0x00, 0x00 };

    assert(state);
    printf("houston this is server here, we got a new client\n");

    client = state_get_free_client(state);
    if (!client || !client_init(client)) {
        return 0;
    }

    init.session_id = session_id;
    init.protocol = protocol;
    if (!client_rsa_modulus(client, &BFAE(init.modulus.buf))) {
        printf("unable to copy modulus.\n");
        printf("this client SHOULD be dropped!\n");
    }
    
    client->response = (struct packet) { 0 };
    packet_init_to(&client->response, &init);

    return client;
}

void login_on_disconnect(struct state *state, struct client *src)
{
    assert(state);
    assert(src);
    client_free(src);
    state_release_client(state, src);
}

void login_on_request(struct state *state, struct client *client, struct buffer *req)
{
    struct packet *req_packet = 0;

    assert(state);
    assert(client);
    assert(req);
    assert(req->buf);

    printf("new chunk of request: %ld\n", req->used);
    req_packet = &client->request.packet;

    cpy_bytes(
        &B(req_packet->buf, sizeof(req_packet->buf), client->request.received),
        req,
        req->used
    );
    client->request.received += req->used;
    client->request.is_partial = packet_size(req_packet) != client->request.received;

    // Make sure the entire packet has arrived.
    if (client->request.is_partial) {
        printf("partial request found\n");
        return;
    }

    // We have received all the data. Reset the counter.
    client->request.received = 0;

    // If we have reached this point, the entire packet
    // has been received and it's ready to be handled.

    if (!client_decrypt_packet(client, &client->request.packet, &client->request.packet)) {
        printf("houston, there was a problem decrypting the package.\n");
        // return;
    }

    printf("new packet received: %d\n", packet_type(&client->request.packet));

    switch (packet_type(&client->request.packet)) {
    case 0x00: // Auth login
        on_auth_login(state, client);
        break;
    case 0x02: // Login server
        printf("handling login server\n");
        break;
    case 0x05: // Request server list
        printf("handing server list\n");
        break;
    case 0x07: // GG Auth
        on_gg_auth(state, client);
        break;
    default:
        printf("houston, we have a problem, i don't recognize that packet.\n");
        break;
    }
}