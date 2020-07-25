#include <log/log.h>
#include <core/byte_reader.h>
#include <core/l2_raw_packet.h>
#include <core/l2_packet.h>
#include "../../storage/character.h"
#include "../../entity/pc.h"
#include "../d0/handler.h"
#include "../type.h"
#include "response.h"
#include "handler.h"

void select_character_handler(struct Client *client, l2_raw_packet *packet)
{
        l2_packet_type type = l2_packet_get_type(packet);
        unsigned char* content = l2_packet_content(packet);

        int index = -1;
        struct Pc *character;
        l2_packet *response;

        if (type != REQUEST_TYPE_SELECTED_CHAR) {
                log_fatal("Invalid request to be handled by select character");
                return;
        }

        byte_reader_cpy_int_n_mv(content, &index);
        log_info("Selected char with index %d", index);
        
        character = storage_character_get(NULL, client, index);
        client_update_character(client, character);

        response = select_character_response(client);

        client_encrypt_packet(client, response);
        client_queue_response(client, response);

        client_update_request_handler(client, d0_handler);

        client_free_mem(client, character);
        client_free_mem(client, response);
}
