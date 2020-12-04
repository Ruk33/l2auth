#ifndef CLIENT_REQUEST_AUTH_LOGIN_H
#define CLIENT_REQUEST_AUTH_LOGIN_H

#include "../storage/session.h"

/**
 * After sending the response from the
 * gg auth request, the client will make
 * this request, where the username and password
 * will be sent. This is where we will check
 * and if the credentials are valid and 
 * allow the client to continue in the auth
 * process if everything is correct.
 */
void client_request_auth_login(struct StorageSession *session, int fd, unsigned char *request);

#endif
