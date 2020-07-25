#ifndef ENTRY_H
#define ENTRY_H

#include <stdlib.h>
#include "../shared/l2_server.h"
#include "../shared/client_id.h"
#include "../shared/request.h"

void *entry_new_conn(struct L2Server *l2_server, client_id id);
void entry_handle_request(struct Request *request);
void entry_handle_disconnect(struct Request *request);

#endif
