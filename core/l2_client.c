#ifndef L2AUTH_L2_CLIENT_C
#define L2AUTH_L2_CLIENT_C

#include <sys/types.h>
#include <log/log.h>
#include <core/l2_socket.c>
#include <core/l2_rsa_key.c>
#include <core/l2_blowfish_key.c>
#include <core/l2_packet.c>
#include <core/send_packet.c>
#include <packet/server/encrypt.c>
#include <packet/client/decrypt.c>

#define L2_CLIENT_MAX_DATA_TO_RECEIVE_IN_BYTES 65535

struct l2_client
{
        struct l2_socket socket;
        struct l2_rsa_key rsa_key;
        struct l2_blowfish_key blowfish_key;

        ssize_t received_data_size;
        unsigned char received_data[L2_CLIENT_MAX_DATA_TO_RECEIVE_IN_BYTES];

        FILE* log_file;
};

void l2_client_create(struct l2_client* client)
{
        unsigned char blowfish_key[] = "_;5.]94-31==-%xT!^[$";
        l2_blowfish_key_build(&client->blowfish_key, blowfish_key, sizeof(blowfish_key));
        l2_rsa_key_build(&client->rsa_key);
}

void l2_client_accept
(
        struct l2_client* client,
        struct l2_socket* server
)
{
        l2_socket_accept(server, &client->socket);
        l2_client_create(client);
        log_info("Connection accepted");
}

void l2_client_close(struct l2_socket* server)
{
        l2_socket_close(server);
        log_info("Connection closed");
}

void l2_client_send_packet
(
        struct l2_client* client,
        l2_raw_packet* packet
)
{
        l2_raw_packet_size packet_size = l2_raw_packet_get_size(packet);
        char printable[packet_size * 3];

        send_packet(&client->socket, packet);
        log_info("Sent packet");

        log_info("Packet size: %d", packet_size);

        for (int i = 0; i < packet_size; i++)
                sprintf(printable + i * 3, "%02X ", packet[i]);

        log_info("Packet bytes: %s", printable);
}

void l2_client_encrypt_and_send_packet
(
        struct l2_client* client,
        l2_raw_packet* packet
)
{
        l2_raw_packet *encrypted_packet = packet_server_encrypt(packet, &client->blowfish_key);
        l2_client_send_packet(client, encrypted_packet);
}

l2_raw_packet* l2_client_wait_and_decrypt_packet
(
        struct l2_client* client
)
{
        client->received_data_size = l2_socket_receive(
                &client->socket,
                client->received_data,
                L2_CLIENT_MAX_DATA_TO_RECEIVE_IN_BYTES
        );

        log_info("Received data from client");

        return packet_client_decrypt(
                &client->blowfish_key,
                client->received_data,
                (unsigned short) client->received_data_size
        );
}

int l2_client_decrypt_client_packet
(
        struct l2_client* client,
        l2_packet* packet,
        unsigned char* dest
)
{
        unsigned short packet_content_size = l2_packet_get_content_size(packet);
        unsigned char packet_content[packet_content_size];

        l2_packet_content(packet, packet_content, 0, packet_content_size);

        return l2_rsa_key_decrypt(
                &client->rsa_key,
                packet_content,
                dest
        );
}

int l2_client_connection_ended(struct l2_client* client)
{
        return client->received_data_size <= 0;
}

#endif
