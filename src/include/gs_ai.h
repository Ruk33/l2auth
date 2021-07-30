#ifndef INCLUDE_GS_AI_H
#define INCLUDE_GS_AI_H

#include "packet.h"
#include "gs_types.h"

static void
gs_ai_tick(struct gs_state *state, struct gs_character *character, double delta);

static void gs_ai_handle_request(
        struct gs_state *state,
        struct gs_character *character,
        packet_t *request);

#endif
