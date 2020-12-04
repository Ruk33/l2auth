#ifndef CLIENT_REQUEST_LOGIN_SERVER_H
#define CLIENT_REQUEST_LOGIN_SERVER_H

#include "../storage/session.h"

/**
 * This is the last request made by the client
 * on the loginserver side. If everything is
 * correct, the client will disconnect from the
 * loginserver and connect to the gameserver.
 */
void client_request_login_server(struct StorageSession *session, int fd);

#endif
