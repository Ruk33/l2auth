#ifndef INCLUDE_GS_PACKET_REVIVE_REQUEST_H
#define INCLUDE_GS_PACKET_REVIVE_REQUEST_H

#include "util.h"
#include "packet.h"

enum gs_packet_revive_request_option {
        REVIVE_IN_CLAN_HALL = 1,
        REVIVE_IN_CASTLE    = 2,
        REVIVE_IN_SIEGE_HQ  = 3,
        REVIVE_IN_FIXED     = 4,
};

struct gs_packet_revive_request {
        enum gs_packet_revive_request_option option_chosen;
};

void gs_packet_revive_request_unpack(
        struct gs_packet_revive_request *dest,
        packet_t *src);

#endif
