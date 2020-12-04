#include <assert.h>
#include <stdio.h>
#include <data_structures/list.h>
#include "../packet.h"
#include "../decrypt_packet.h"
#include "../storage/server_manager.h"
#include "../storage/game_server_manager.h"
#include "../storage/session.h"
#include "../client_packet/type.h"
#include "auth_login.h"
#include "gg_auth.h"
#include "server_list.h"
#include "login_server.h"
#include "handle.h"

void client_request_handle(struct StorageServerManager *server_manager, int fd, unsigned char *request, size_t request_size)
{
        assert(server_manager);
        assert(fd > 0);
        assert(request);
        assert(request_size);

        struct StorageSession *session = NULL;
        struct List *servers = NULL;

        unsigned char *packet = calloc(65536, 1);

        session = storage_session_manager_get(&server_manager->session_manager, fd);

        if (!session) {
                printf("WARNING, client with id %d was not found in the sessions... ignoring", fd);
                return;
        }

        decrypt_packet(&session->blowfish, packet, request, request_size);
        printf("Detected packet type %02X.\n", packet_type(packet));

        switch (packet_type(packet)) {
        case CLIENT_PACKET_TYPE_AUTH_LOGIN:
                client_request_auth_login(session, fd, packet);
                break;
        case CLIENT_PACKET_TYPE_LOGIN_SERVER:
                client_request_login_server(session, fd);
                break;
        case CLIENT_PACKET_TYPE_REQUEST_SERVER_LIST:
                servers = storage_game_server_manager_servers(&server_manager->game_server_manager);
                client_request_server_list(session, servers, fd);
                break;
        case CLIENT_PACKET_TYPE_GG_AUTH:
                client_request_gg_auth(session, fd);
                break;
        default:
                printf("Ignoring unknown packet...\n");
                break;
        }

        free(packet);
}
