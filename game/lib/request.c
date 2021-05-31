#include <headers.h>
#include "request.h"

void request_send_response(request_t *r, packet *p, packet_size s)
{
        assert(r);
        assert(p);
        r->host->send_response(r->host->gs, r->socket, p, (size_t) s);
}

void assert_valid_request(request_t *request)
{
        assert(request);
        assert(request->storage);
        assert(request->host);
        assert(request->packet);
}
