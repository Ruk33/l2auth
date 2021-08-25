#ifndef INCLUDE_GS_AI_H
#define INCLUDE_GS_AI_H

#include "packet.h"
#include "gs_types.h"

void gs_ai_tick(
        struct gs_state *gs,
        struct gs_character *character,
        double delta);

void gs_ai_handle_request(
        struct gs_state *gs,
        struct gs_character *character,
        packet_t *request);

#endif
