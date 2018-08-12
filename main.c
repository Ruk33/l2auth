#include <socket_strategy/socket_strategy_windows.c>
#include <core/l2_socket.c>
#include <core/l2_server.c>
#include <core/l2_client.c>
#include <core/l2_packet.c>
#include <core/l2_client_packet_type.c>
#include <server_packet/server_packet_login_fail.c>
#include <util/get_client_packet_type.c>

int main()
{
    struct l2_socket_strategy server_strategy;
    struct l2_socket server;
    struct l2_client client;

    struct l2_packet server_packet;
    struct l2_packet client_packet;

    printf("Listening for connections\n");
    fflush(stdout);

    socket_strategy_windows(&server_strategy);
    l2_server_create(&server, &server_strategy, 2106);
    l2_client_accept(&client, &server);

    printf("Accepted connection!\n");
    fflush(stdout);

    l2_client_send_init_packet(&client);
    printf("Init l2_packet sent!\n");
    fflush(stdout);

    while (1) {
        l2_client_wait_and_decrypt_packet(&client, &client_packet);
        printf("Received data from client\n");
        fflush(stdout);

        if (l2_client_connection_ended(&client)) {
            printf("Connection ended by client\n");
            fflush(stdout);
            break;
        }

        switch (get_client_packet_type(&client_packet)) {
        case CLIENT_PACKET_TYPE_REQUEST_AUTH_LOGIN:
            //
            break;
        case CLIENT_PACKET_TYPE_GG_AUTH:
            //
            break;
        default:
            // ignore invalid packet
            break;
        }

        //decrypt_client_request(&key, client_buffer+2, recv_size-2, client_buffer_decrypt);
        //packet_type = client_buffer_decrypt[0] & 0xff;

        server_packet_login_fail(&server_packet, LOGIN_FAIL_REASON_USER_OR_PASSWORD_WRONG);
        l2_client_encrypt_and_send_packet(&client, &server_packet);
    }

    return 0;
}