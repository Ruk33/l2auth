#include <assert.h>
#include <stdio.h>
#include "../socket.h"
#include "../packet.h"
#include "../encrypt_packet.h"
#include "../storage/session.h"
#include "../server_packet/play_ok.h"
#include "login_server.h"

void client_request_login_server(struct StorageSession *session, int fd)
{
        assert(session);
        assert(fd > 0);

        unsigned char response[SERVER_PACKET_PLAY_OK_FULL_SIZE_ENCRYPTED] = {0};

        printf("Client wants to log in to game server.\n");

        server_packet_play_ok(response, session->playOK1, session->playOK2);
        encrypt_packet(&session->blowfish, response, response);
        socket_send(fd, response, (size_t) packet_size(response));
}
