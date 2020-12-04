#include <assert.h>
#include <stdio.h>
#include <data_structures/list.h>
#include "../socket.h"
#include "../packet.h"
#include "../encrypt_packet.h"
#include "../storage/session.h"
#include "../server_packet/server_list.h"
#include "server_list.h"

void client_request_server_list(struct StorageSession *session, struct List *servers, int fd)
{
        assert(session);
        assert(servers);
        assert(fd > 0);

        unsigned char response[SERVER_PACKET_SERVER_LIST_FULL_SIZE_ENCRYPTED] = {0};

        printf("Handling server list request...\n");

        server_packet_server_list(response, servers);
        encrypt_packet(&session->blowfish, response, response);
        socket_send(fd, response, (size_t) packet_size(response));
}
