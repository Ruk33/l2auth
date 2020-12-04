#ifndef CLIENT_REQUEST_SERVER_LIST_H
#define CLIENT_REQUEST_SERVER_LIST_H

#include <data_structures/list.h>
#include "../storage/session.h"

/**
 * The request name is self explanatory but,
 * with this request, the client will ask
 * for all the game servers we have available.
 * Check storage/game_sever.h and storage/game_server_manager.h
 * where the servers are stored. These stored
 * game servers are later return to the client.
 */
void client_request_server_list(struct StorageSession *session, struct List *servers, int fd);

#endif
