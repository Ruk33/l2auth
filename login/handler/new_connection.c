#include <assert.h>
#include <stdlib.h>
#include "../socket.h"
#include "../server_packet/init.h"
#include "../packet.h"
#include "../storage/server_manager.h"
#include "../storage/session_manager.h"
#include "../storage/session.h"
#include "new_connection.h"

void handler_new_connection(struct StorageServerManager *server_manager, int fd)
{
        assert(server_manager);
        assert(fd > 0);
        
        unsigned char response[SERVER_PACKET_INIT_FULL_SIZE] = {0};
        struct StorageSession *session = NULL;

        storage_session_manager_add(&server_manager->session_manager, fd);
        session = storage_session_manager_get(&server_manager->session_manager, fd);

        server_packet_init(response, &session->rsa);
        socket_send(fd, response, (size_t) packet_size(response));
}
