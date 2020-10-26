#include <assert.h>
#include <string.h>
#include <log/log.h>
#include <core/l2_raw_packet.h>
#include <core/l2_packet.h>
#include "host.h"
#include "decrypt.h"
#include "encrypt.h"
#include "dto/character.h"
#include "dto/npc.h"
#include "dto/player.h"
#include "client.h"

struct Client
{
        host_malloc_cb memory_alloc;
        host_mfree_cb memory_free;
        host_send_response_cb send_response;

        int id;
        int conn_encrypted;
        unsigned char decrypt_key[8];
        unsigned char encrypt_key[8];

        request_handler handle_request;

        struct L2SessionKey session;
        struct Player character;
};

struct Client *client_new(int id, host_malloc_cb m, host_mfree_cb f, host_send_response_cb s)
{
        struct Client *client = NULL;
        unsigned char key[] = {0x94, 0x35, 0x00, 0x00, 0xa1, 0x6c, 0x54, 0x87};

        client = m(sizeof(*client));
        client->id = id;
        client->memory_alloc = m;
        client->memory_free = f;
        client->send_response = s;
        client->conn_encrypted = 0;

        memcpy(client->encrypt_key, key, sizeof(client->encrypt_key));
        memcpy(client->decrypt_key, key, sizeof(client->decrypt_key));

        return client;
}

void *client_alloc_mem(struct Client *client, size_t how_much)
{
        assert(client);
        assert(how_much > 0);
        return client->memory_alloc(how_much);
}

void client_free_mem(struct Client *client, void *mem)
{
        assert(client);
        assert(mem);
        client->memory_free(mem);
}

void client_queue_response(struct Client *client, l2_raw_packet *packet)
{
        assert(client);
        assert(packet);

        unsigned char *buf = NULL;
        size_t buf_size = 0;

        buf = (unsigned char *)packet;
        buf_size = (size_t)l2_raw_packet_get_size(packet);

        client->send_response(client->id, buf, buf_size);
}

void client_update_request_handler(struct Client *client, request_handler handler)
{
        assert(client);
        assert(handler);
        client->handle_request = handler;
}

void client_update_session(struct Client *client, struct L2SessionKey *session)
{
        assert(client);
        assert(session);

        memcpy(&client->session, session, sizeof(client->session));

        log_info("Updated client session");
        log_info("Play OK 1: %d", client->session.playOK1);
        log_info("Play OK 2: %d", client->session.playOK2);
        log_info("Login OK 1: %d", client->session.loginOK1);
        log_info("Login OK 2: %d", client->session.loginOK2);
}

struct L2SessionKey *client_session(struct Client *client)
{
        assert(client);

        struct L2SessionKey *session = NULL;

        session = client_alloc_mem(client, sizeof(*session));
        memcpy(session, &client->session, sizeof(*session));

        return session;
}

void client_encrypt_packet(struct Client *client, l2_raw_packet *packet)
{
        assert(client);
        assert(packet);

        unsigned char *packet_content = NULL;
        l2_raw_packet_size packet_size = 0;

        packet_content = l2_raw_packet_content(packet);
        packet_size = l2_raw_packet_get_size(packet);

        if (packet_size > 1)
                packet_size = (unsigned short)(packet_size - 2);

        encrypt(packet_content, packet_size, client->encrypt_key);
}

void client_decrypt_packet(struct Client *client, l2_raw_packet *packet)
{
        assert(client);
        assert(packet);

        unsigned char *packet_content = NULL;
        l2_raw_packet_size packet_size = 0;

        packet_content = l2_raw_packet_content(packet);
        packet_size = l2_raw_packet_get_size(packet);

        if (packet_size > 1)
                packet_size = (unsigned short)(packet_size - 2);

        decrypt(packet_content, packet_size, client->decrypt_key);
}

void client_update_character(struct Client *client, struct Player *character)
{
        assert(client);
        assert(character);

        memcpy(&client->character, character, sizeof(client->character));
}

struct Player *client_player(struct Client *client)
{
        assert(client);

        struct Player player = client->character;
        struct Player *player_copy = NULL;

        player_copy = client_alloc_mem(client, sizeof(*player_copy));
        memcpy(player_copy, &player, sizeof(*player_copy));

        return player_copy;
}

void client_handle_disconnect(struct Client *client)
{
        assert(client);
        client->memory_free(client);
}

int client_is_conn_encrypted(struct Client *client)
{
        assert(client);
        return client->conn_encrypted;
}

void client_encrypt_conn(struct Client *client)
{
        assert(client);
        client->conn_encrypted = 1;
}

int client_id(struct Client *client)
{
        assert(client);
        return client->id;
}
