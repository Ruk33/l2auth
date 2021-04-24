#ifndef LIB_REQUEST_H
#define LIB_REQUEST_H

#include <stddef.h>
#include <sys/types.h>
#include <host.h>
#include <session.h>
#include <packet.h>
#include <db/conn.h>

struct Request {
        session_t *session;
        db_conn_t *storage;
        host_t *   host;
        packet *   packet;
        ssize_t    size;
};

typedef struct Request request_t;

void assert_valid_request(request_t *request);

#endif
