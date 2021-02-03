#ifndef LIB_STATE_MACHINE_AUTH_REQUEST_H
#define LIB_STATE_MACHINE_AUTH_REQUEST_H

#include <headers.h>
#include <request.h>

/**
 * The purpose of auth request is to update
 * the session's keys and send all the characters
 * associated to a session.
 * We only need to update the session's keys
 * the first time, meaning, when the player just
 * logs in. After that, it's no longer required and
 * we can use this request to refresh the characters
 * from a session (example, when creating a new char).
 */
void state_machine_auth_request(request_t *request, int update_session);

#endif
