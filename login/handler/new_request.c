#include <assert.h>
#include <stdlib.h>
#include "../storage/server_manager.h"
#include "../client_request/handle.h"
#include "new_request.h"

void handler_new_request(struct StorageServerManager *server_manager, int fd, unsigned char *request, size_t request_size)
{
        assert(server_manager);
        assert(fd > 0);
        assert(request);
        assert(request_size);
        client_request_handle(server_manager, fd, request, request_size);
}
