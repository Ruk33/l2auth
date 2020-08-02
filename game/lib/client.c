#include <assert.h>
#include <string.h>
#include <log/log.h>
#include <core/l2_raw_packet.h>
#include <core/l2_packet.h>
#include "../shared/l2_server.h"
#include "../shared/client_id.h"
#include "decrypt.h"
#include "encrypt.h"
#include "request_handler/protocol_version/handler.h"
#include "dto/character.h"
#include "dto/npc.h"
#include "dto/pc.h"
#include "cache/world.h"
#include "client.h"

struct Client {
        client_id id;
        struct L2Server *l2_server;
        struct World *world;
        int conn_encrypted;
        unsigned char decrypt_key[8];
        unsigned char encrypt_key[8];
        request_handler handle_request;
        struct L2SessionKey session;
        struct Pc character;
};

struct Client *client_new(struct L2Server *l2_server, client_id id)
{
        struct Client *client = l2_server->alloc(sizeof(struct Client));
        unsigned char key[] = { 0x94, 0x35, 0x00, 0x00, 0xa1, 0x6c, 0x54, 0x87 };

        client->id = id;
        client->l2_server = l2_server;

        // (franco.montenegro) Fix, we won't create a new world 
        // for each client
        client->world = world_new(l2_server);

        client->conn_encrypted = 0;
        memcpy(client->encrypt_key, key, sizeof(key));
        memcpy(client->decrypt_key, key, sizeof(key));

        client->handle_request = protocol_version_handler;

        return client;
}

void *client_alloc_mem(struct Client *client, size_t how_much)
{
        return client->l2_server->alloc(how_much);
}

void client_free_mem(struct Client *client, void *mem)
{
        client->l2_server->free(mem);
}

void client_handle_request
(
        struct Client *client,
        char *request,
        size_t request_size
)
{
        assert(client);

        l2_raw_packet *raw_packet = (l2_raw_packet *) request;
        unsigned char *packet_content = l2_raw_packet_content(raw_packet);
        unsigned short packet_size = (unsigned short) (request_size - 2);
        l2_raw_packet *packet = client_alloc_mem(
                client,
                l2_raw_packet_calculate_size((unsigned short) request_size)
        );

        l2_raw_packet_init(packet, packet_content, packet_size);

        if (client->conn_encrypted) client_decrypt_packet(client, packet);
        client->conn_encrypted = 1;

        log_info("Packet type %02X", l2_packet_get_type(packet));

        client->handle_request(client, packet);

        client_free_mem(client, packet);
}

void client_queue_response(struct Client *client, l2_raw_packet *packet)
{
        struct Response response;

        response.client_id = client->id;
        response.buf = (char *) packet;
        response.buf_size = (size_t) l2_raw_packet_get_size(packet);

        client->l2_server->queue_response(&response);
}

void client_update_request_handler
(
        struct Client *client,
        request_handler handler
)
{
        client->handle_request = handler;
}

void client_update_session(struct Client *client, struct L2SessionKey *session)
{
        memcpy(&client->session, session, sizeof(struct L2SessionKey));

        log_info("Updated client session");
        log_info("Play OK 1: %d", client->session.playOK1);
        log_info("Play OK 2: %d", client->session.playOK2);
        log_info("Login OK 1: %d", client->session.loginOK1);
        log_info("Login OK 2: %d", client->session.loginOK2);
}

struct L2SessionKey *client_session(struct Client *client)
{
        struct L2SessionKey *session = client_alloc_mem(
                client,
                sizeof(struct L2SessionKey)
        );
        memcpy(session, &client->session, sizeof(struct L2SessionKey));
        return session;
}

void client_encrypt_packet(struct Client *client, l2_raw_packet *packet)
{
        assert(client);
        assert(packet);
        unsigned char *packet_content = l2_raw_packet_content(packet);
        l2_raw_packet_size packet_size = l2_raw_packet_get_size(packet);
        if (packet_size > 1) packet_size = (unsigned short) (packet_size - 2);
        encrypt(packet_content, packet_size, client->encrypt_key);
}

void client_decrypt_packet(struct Client *client, l2_raw_packet *packet)
{
        assert(client);
        assert(packet);
        unsigned char *packet_content = l2_raw_packet_content(packet);
        l2_raw_packet_size packet_size = l2_raw_packet_get_size(packet);
        if (packet_size > 1) packet_size = (unsigned short) (packet_size - 2);
        decrypt(packet_content, packet_size, client->decrypt_key);
}

void client_update_character(struct Client *client, struct Pc *character)
{
        memcpy(&client->character, character, sizeof(struct Pc));
        world_update_player(client->world, character);
}

struct Character *client_character(struct Client *client, int obj_id)
{
        return world_get_character(client->world, obj_id);
}

struct Pc *client_player(struct Client *client)
{
        // struct Pc *character = client_alloc_mem(client, sizeof(struct Pc));
        // memcpy(character, &client->character, sizeof(struct Pc));
        // return character;
        struct Pc player = client->character;
        return world_get_player(client->world, player.character.id);
}

void client_spawn_npc(struct Client *client, struct Npc *npc)
{
        world_spawn_npc(client->world, npc);
}

void client_handle_disconnect(struct Client *client)
{
        client->l2_server->free(client);
}
