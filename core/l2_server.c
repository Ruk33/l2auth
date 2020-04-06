#ifndef L2AUTH_L2_SERVER_C
#define L2AUTH_L2_SERVER_C

#include <stdlib.h>
#include <log/log.h>
#include <core/l2_socket.h>
#include <core/l2_client.h>
#include <core/l2_packet.h>
#include <packet/client/type.h>
#include <login/packet/init.h>
#include <login/packet/fail.h>
#include <login/packet/gg_auth.h>
#include <login/dto/session_key.h>
#include <login/handler/request_auth_login.h>
#include <login/handler/request_server_list.h>
#include <login/handler/request_login_server.h>
#include <core/l2_server.h>

void l2_server_create
(
        struct L2Socket *server,
        struct L2SocketStrategy *socket_type,
        unsigned short port
)
{
        if (!server) {
                log_fatal("Trying to create l2 server but none was passed");
                return;
        }
        if (!socket_type) {
                log_fatal("Trying to create l2 server but no socket strategy was passed");
                return;
        }
        l2_socket_connect(server, socket_type);
        l2_socket_bind(server, port);
        l2_socket_listen(server);
}

void l2_server_accept_and_handle_connection
(
        struct L2Socket *server
)
{
        struct L2Client* client = l2_client_new();
        struct LoginDtoSessionKey* session_key;

        l2_packet *server_packet;
        l2_packet *client_packet;

        unsigned char *decrypted_packet;

        if (!server) {
                log_fatal("Trying to accept and handle connection but no server was passed");
                return;
        }

        session_key = login_session_key_create();
        decrypted_packet = calloc(65535, sizeof(char));

        l2_client_accept(client, server);

        server_packet = login_packet_init(client->rsa_key);
        l2_client_send_packet(client, server_packet);

        while (1) {
                server_packet = NULL;
                client_packet = l2_client_wait_and_decrypt_packet(client);

                if (l2_client_connection_ended(client)) {
                        log_info("Client connection closed");
                        break;
                }

                if (!client_packet) {
                        log_error("Client packet was not able to decrypt");
                        continue;
                }

                l2_client_decrypt_client_packet(
                        client,
                        client_packet,
                        decrypted_packet
                );

                switch (l2_packet_get_type(client_packet)) {
                case PACKET_CLIENT_TYPE_REQUEST_AUTH_LOGIN:
                        server_packet = login_handler_request_auth_login(
                                decrypted_packet,
                                session_key
                        );
                        break;
                case PACKET_CLIENT_TYPE_REQUEST_LOGIN_SERVER:
                        server_packet = login_handler_request_login_server(
                                session_key
                        );
                        break;
                case PACKET_CLIENT_TYPE_REQUEST_SERVER_LIST:
                        server_packet = login_handler_request_server_list();
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
                        client,
                        server_packet
                );
        }

        free(decrypted_packet);
}

void l2_server_wait_and_accept_connections
(
        struct L2Socket *server
)
{
        if (!server) {
                log_fatal("Trying to wait and accept for connections but no socket was passed");
                return;
        }
        l2_server_accept_and_handle_connection(server);
}

#endif
