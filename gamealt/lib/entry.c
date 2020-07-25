#include <log/log.h>
#include "../shared/l2_server.h"
#include "../shared/request.h"
#include "../shared/client_id.h"
#include "client.h"
#include "entry.h"

void *entry_new_conn(struct L2Server *l2_server, client_id id)
{
        return client_new(l2_server, id);
}

void entry_handle_request(struct Request *request)
{
        client_handle_request(request->client, request->buf, request->buf_size);
}

void entry_handle_disconnect(struct Request *request)
{
        client_handle_disconnect(request->client);
}
