#ifndef L2AUTH_L2_SERVER_C
#define L2AUTH_L2_SERVER_C

#include <core/l2_socket.c>
#include <core/l2_client.c>
#include <core/l2_packet.c>
#include <core/l2_client_packet_type.c>
#include <util/get_client_packet_type.c>
#include <server_packet/server_packet_login_fail.c>
#include <server_packet/server_packet_gg_auth.c>

void l2_server_create(
        struct l2_socket *server,
        struct l2_socket_strategy *socket_type,
        unsigned short port
)
{
        l2_socket_connect(server, socket_type);
        l2_socket_bind(server, port);
        l2_socket_listen(server);
}

void l2_server_accept_and_handle_connection(struct l2_socket *server)
{
        struct l2_client client;

        l2_packet *server_packet;
        l2_packet *client_packet;

        l2_client_accept(&client, server);

        server_packet = server_packet_init(&client.rsa_key);
        l2_client_send_packet(&client, server_packet);
        // crete packet | log action | send it
        // treat it as if we were dealing with a pipe
        // free_packet(send_packet(log_and_return_packet(create_packet())))

        while (1) {
                client_packet = l2_client_wait_and_decrypt_packet(&client);

                if (l2_client_connection_ended(&client)) {
                        break;
                }

                switch (get_client_packet_type(client_packet)) {
                case CLIENT_PACKET_TYPE_REQUEST_AUTH_LOGIN:
                        //l2_client_decrypt_client_packet(&client, client_packet, decrypted);
                        //memcpy(username, decrypted + 0x62, 14);
                        //memcpy(password, decrypted + 0x70, 16);
                        server_packet = server_packet_login_fail(LOGIN_FAIL_REASON_USER_OR_PASSWORD_WRONG);
                        l2_client_encrypt_and_send_packet(&client, server_packet);
                        break;
                case CLIENT_PACKET_TYPE_GG_AUTH:
                        server_packet = server_packet_gg_auth(GG_AUTH_RESPONSE_SKIP_GG);
                        l2_client_encrypt_and_send_packet(&client, server_packet);
                        break;
                default:
                        // ignore invalid packet
                        break;
                }
        }

        l2_client_close(server);
}

void l2_server_wait_and_accept_connections(struct l2_socket *server)
{
        l2_server_accept_and_handle_connection(server);
}

#endif //L2AUTH_L2_SERVER_C
