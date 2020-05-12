#include <assert.h>
#include <log/log.h>
#include <core/l2_packet.h>
#include <core/session_key.h>
#include <login/connection.h>
#include <login/server.h>
#include <login/client.h>
#include <login/handler/new_connection.h>
#include <login/handler/request_auth_login.h>
#include <login/handler/request_login_server.h>
#include <login/handler/request_server_list.h>
#include <login/handler/gg.h>
#include <login/packet/client_type.h>
#include <login/handler/client.h>

void login_handler_client(struct LoginConnection* conn)
{
        assert(conn);

        struct LoginServer* server = conn->server;
        struct LoginClient* client = conn->client;

        l2_packet* client_packet = NULL;

        log_info("Handling new client from loginserver");
        login_handler_new_connection(server, client);

        while (1) {
                client_packet = login_client_wait_and_decrypt_packet(client);

                if (login_client_connection_ended(client)) break;

                if (!client_packet) {
                        log_error("Client packet was not able to decrypt");
                        continue;
                }

                switch (l2_packet_get_type(client_packet)) {
                case LOGIN_PACKET_CLIENT_TYPE_REQUEST_AUTH_LOGIN:
                        login_handler_request_auth_login(
                                server,
                                client,
                                client_packet
                        );
                        break;
                case LOGIN_PACKET_CLIENT_TYPE_REQUEST_LOGIN_SERVER:
                        login_handler_request_login_server(
                                server,
                                client
                        );
                        break;
                case LOGIN_PACKET_CLIENT_TYPE_REQUEST_SERVER_LIST:
                        login_handler_request_server_list(
                                server,
                                client
                        );
                        break;
                case LOGIN_PACKET_CLIENT_TYPE_GG_AUTH:
                        login_handler_gg(server, client);
                        break;
                default:
                        log_info(
                                "Packet type %d not being handled",
                                l2_packet_get_type(client_packet)
                        );
                        break;
                }
        }

        log_info("Loginserver client connection closed");
}
