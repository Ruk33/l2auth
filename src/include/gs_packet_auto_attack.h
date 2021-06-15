#ifndef INCLUDE_GS_PACKET_AUTO_ATTACK_H
#define INCLUDE_GS_PACKET_AUTO_ATTACK_H

#include "util.h"
#include "packet.h"

// Note: this packet seems to be the one in charge
// of playing the being hitted/on guard animation.
// Maybe we can rename it to attack landed?
typedef struct {
        u32_t target_id;
} gs_packet_auto_attack_t;

void gs_packet_auto_attack_pack(packet_t *dest, gs_packet_auto_attack_t *src);

#endif
