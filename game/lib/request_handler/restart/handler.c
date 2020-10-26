#include <assert.h>
#include <log/log.h>
#include <core/l2_raw_packet.h>
#include <core/l2_packet.h>
#include "../../request.h"
#include "../../client.h"
#include "../type.h"
#include "../auth_login/response.h"
#include "../auth_login/handler.h"
#include "response.h"
#include "handler.h"

void restart_handler(struct Request *request)
{
        assert(request);

        struct Client *client = request->client;

        l2_packet *response = NULL;

        response = restart_response(client);

        log_info("Restarting");
        client_encrypt_packet(client, response);
        client_queue_response(client, response);

        client_free_mem(client, response);

        response = auth_login_response(client);

        log_info("Refetching characters");
        client_encrypt_packet(client, response);
        client_queue_response(client, response);

        // client_update_request_handler(client, &auth_login_handler);

        client_free_mem(client, response);
}
