#ifndef CLIENT_REQUEST_GG_AUTH_H
#define CLIENT_REQUEST_GG_AUTH_H

#include "../storage/session.h"

/**
 * This request is made by the client
 * when username and password has been supplied.
 * After sending a response, the following request
 * made by the client will be auth_login.
 */
void client_request_gg_auth(struct StorageSession *session, int fd);

#endif
