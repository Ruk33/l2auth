#ifndef LIB_REQUEST_H
#define LIB_REQUEST_H

#include <stddef.h>
#include <sys/types.h>
#include <host.h>
#include <session.h>
#include <packet.h>
#include <db/conn.h>

typedef struct {
        int        socket;
        db_conn_t *storage;
        host_t *   host;
        packet *   packet;
        ssize_t    size;
} request_t;

void request_send_response(request_t *r, packet *p, packet_size s);

void assert_valid_request(request_t *request);

#endif
