#ifndef L2AUTH_L2_SERVER_C
#define L2AUTH_L2_SERVER_C

#include <stdlib.h>
#include <core/l2_socket.c>
#include <core/l2_client.c>
#include <core/l2_packet.c>
//#include <core/l2_client_packet_type.c>
#include <packet/client/type.c>
#include <util/get_client_packet_type.c>
#include <login/packet/init.c>
#include <login/packet/fail.c>
#include <login/packet/gg_auth.c>
#include <login/session_key.c>
#include <login/handler/request_auth_login.c>

void l2_server_create
(
        struct l2_socket *server,
        struct l2_socket_strategy *socket_type,
        unsigned short port
)
{
        l2_socket_connect(server, socket_type);
        l2_socket_bind(server, port);
        l2_socket_listen(server);
}

void l2_server_accept_and_handle_connection
(
        struct l2_socket *server
)
{
        struct l2_client client;
        struct LoginSessionKey* session_key = login_session_key_create();

        l2_packet *server_packet;
        l2_packet *client_packet;

        unsigned char *decrypted_packet = calloc(65535, sizeof(char));

        l2_client_accept(&client, server);

        server_packet = login_packet_init(&client.rsa_key);
        l2_client_send_packet(&client, server_packet);

        while (1) {
                server_packet = NULL;
                client_packet = l2_client_wait_and_decrypt_packet(&client);

                if (l2_client_connection_ended(&client)) {
                        break;
                }

                l2_client_decrypt_client_packet(
                        &client,
                        client_packet,
                        decrypted_packet
                );

                switch (get_client_packet_type(client_packet)) {
                case PACKET_CLIENT_TYPE_REQUEST_AUTH_LOGIN:
                        server_packet = login_handler_request_auth_login(
                                decrypted_packet,
                                session_key
                        );
                        break;
                case PACKET_CLIENT_TYPE_GG_AUTH:
                        server_packet = login_packet_gg_auth(
                                LOGIN_PACKET_GG_AUTH_RESPONSE_SKIP_GG
                        );
                        break;
                default:
                        // ignore invalid packet
                        break;
                }

                l2_client_encrypt_and_send_packet(
                        &client,
                        server_packet
                );
        }

        free(decrypted_packet);
}

void l2_server_wait_and_accept_connections
(
        struct l2_socket *server
)
{
        l2_server_accept_and_handle_connection(server);
}

#endif
