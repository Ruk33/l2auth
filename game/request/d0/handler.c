#include <assert.h>
#include <log/log.h>
#include <core/byte_reader.h>
#include <game/request.h>
#include "handler.h"

void game_request_d0_handler(struct GameRequest* request)
{
        assert(request);

        unsigned char* content = l2_packet_content(request->packet);
        unsigned short id = 0;

        byte_reader_cpy_int_n_mv(content, &id);
        log_info("D0 ID: %d", id);

        switch (id) {
                case 8:
                        log_info("Requesting Manor List, ignoring");
                        break;
                default:
                        log_fatal("Unknown id to handle from packet D0");
                        break;
        }
}
