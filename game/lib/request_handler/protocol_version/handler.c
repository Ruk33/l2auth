#include <assert.h>
#include <log/log.h>
#include <core/l2_raw_packet.h>
#include <core/l2_packet.h>
#include <core/byte_reader.h>
#include "../../request.h"
#include "../../client.h"
#include "../type.h"
#include "response.h"
#include "next_handler.h"
#include "handler.h"

void protocol_version_handler(struct Request *request)
{
        assert(request);

        struct Client *client = request->client;
        l2_raw_packet *packet = request->packet;

        unsigned char *content = NULL;
        int protocol_version = 0;
        l2_packet *response = NULL;

        content = l2_packet_content(packet);

        byte_reader_cpy_int_n_mv(content, &protocol_version);
        log_info("Protocol version %d", protocol_version);

        response = protocol_version_response(client);

        client_queue_response(client, response);
        client_update_request_handler(client, &protocol_version_next_handler);

        client_free_mem(client, response);
}
