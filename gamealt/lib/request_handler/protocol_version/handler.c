#include <log/log.h>
#include <core/l2_raw_packet.h>
#include <core/l2_packet.h>
#include <core/byte_reader.h>
#include "../../client.h"
#include "../type.h"
#include "response.h"
#include "../auth_login/handler.h"
#include "handler.h"

void protocol_version_handler(struct Client *client, l2_raw_packet *packet)
{
        l2_packet_type type = l2_packet_get_type(packet);
        unsigned char* content = l2_packet_content(packet);
        int protocol_version = 0;
        l2_packet *response;
        
        if (type != REQUEST_TYPE_PROTOCOL_VERSION) {
                log_fatal("Invalid request to be handled by protocol version");
                return;
        }

        byte_reader_cpy_int_n_mv(content, &protocol_version);
        log_info("Protocol version %d", protocol_version);

        response = protocol_version_response(client);
        client_queue_response(client, response);
        client_free_mem(client, response);

        client_update_request_handler(client, auth_login_handler);
}
