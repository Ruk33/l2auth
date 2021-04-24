#ifndef LIB_STATE_MACHINE_HANDLE_H
#define LIB_STATE_MACHINE_HANDLE_H

#include <headers.h>
#include <db/conn.h>

/**
 * Entry point for handling the state of the
 * user's session.
 */
void state_machine_handle(
        int        client,
        byte_t *   request,
        ssize_t    request_size,
        host_t *   host,
        db_conn_t *conn);

#endif
