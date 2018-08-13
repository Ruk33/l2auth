#ifndef L2AUTH_L2_CLIENT_C
#define L2AUTH_L2_CLIENT_C

#include <core/l2_socket.c>
#include <core/l2_rsa_key.c>
#include <core/l2_blowfish_key.c>
#include <core/l2_packet.c>
#include <core/send_sendable_packet.c>
#include <core/send_sendable_encrypted_packet.c>
#include <server_packet/server_packet_init.c>
#include <client_packet/client_packet_decrypted.c>

#define L2_CLIENT_MAX_DATA_TO_RECEIVE_IN_BYTES 65535

struct l2_client {
  struct l2_socket socket;
  struct l2_rsa_key rsa_key;
  struct l2_blowfish_key blowfish_key;

  int received_data_size;
  char received_data[L2_CLIENT_MAX_DATA_TO_RECEIVE_IN_BYTES];
};

void l2_client_create(struct l2_client* client)
{
    unsigned char blowfish_key[] = "_;5.]94-31==-%xT!^[$";
    l2_blowfish_key_build(&client->blowfish_key, blowfish_key, sizeof(blowfish_key));
    l2_rsa_key_build(&client->rsa_key, L2_RSA_KEY_DEFAULT_SIZE_IN_BITS, L2_RSA_KEY_DEFAULT_E);
}

void l2_client_accept(struct l2_client* client, struct l2_socket* server)
{
    l2_socket_accept(server, &client->socket);
    l2_client_create(client);
}

void l2_client_send_packet(struct l2_client* client, struct l2_packet* packet)
{
    send_sendable_packet(&client->socket, packet);
}

void l2_client_encrypt_and_send_packet(struct l2_client* client, struct l2_packet* packet)
{
    send_sendable_encrypted_packet(&client->socket, packet, &client->blowfish_key);
}

void l2_client_send_init_packet(struct l2_client* client)
{
    struct l2_packet init_packet;
    server_packet_init(&init_packet, &client->rsa_key);
    l2_client_send_packet(client, &init_packet);
}

void l2_client_wait_and_decrypt_packet(struct l2_client* client, struct l2_packet* client_packet)
{
    client->received_data_size = l2_socket_receive(
            &client->socket,
            client->received_data,
            L2_CLIENT_MAX_DATA_TO_RECEIVE_IN_BYTES
    );

    client_packet_decrypted(
            client_packet,
            &client->blowfish_key,
            client->received_data,
            client->received_data_size
    );
}

int l2_client_connection_ended(struct l2_client* client)
{
    return client->received_data_size<=0;
}

#endif //L2AUTH_L2_CLIENT_C