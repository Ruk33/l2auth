#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include "../blowfish.h"
#include "../socket.h"
#include "../packet.h"
#include "../encrypt_packet.h"
#include "../storage/session.h"
#include "../server_packet/gg_auth.h"
#include "gg_auth.h"

void client_request_gg_auth(struct StorageSession *session, int fd)
{
        assert(session);
        assert(fd > 0);

        unsigned char response[SERVER_PACKET_GG_AUTH_FULL_SIZE_ENCRYPTED] = {0};

        printf("Handling gg auth request.\n");

        server_packet_gg_auth(response, SERVER_PACKET_GG_AUTH_RESPONSE_SKIP_GG);
        encrypt_packet(&session->blowfish, response, response);
        socket_send(fd, response, (size_t) packet_size(response));
}
