#include "request.h"

void assert_valid_request(request_t *request)
{
        assert(request);
        assert(request->session);
        assert(request->storage);
        assert(request->host);
        assert(request->packet);
}
