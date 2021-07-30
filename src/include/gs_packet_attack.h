#ifndef INCLUDE_GS_PACKET_ATTACK_H
#define INCLUDE_GS_PACKET_ATTACK_H

#include "util.h"
#include "gs_types.h"
#include "packet.h"

typedef struct {
        u32_t target_id;
        u32_t damage;
        u8_t flags;
} gs_packet_attack_hit_t;

typedef struct {
        u32_t attacker_id;
        i32_t attacker_x;
        i32_t attacker_y;
        i32_t attacker_z;
        gs_packet_attack_hit_t hits[10];
        u16_t hit_count;
} gs_packet_attack_t;

void gs_packet_attack_set_attacker(
        gs_packet_attack_t *dest,
        struct gs_character *src);

void gs_packet_attack_add_hit(
        gs_packet_attack_t *dest,
        gs_packet_attack_hit_t *src);

void gs_packet_attack_pack(packet_t *dest, gs_packet_attack_t *src);

#endif
