#ifndef INCLUDE_GS_PACKET_STATUS_H
#define INCLUDE_GS_PACKET_STATUS_H

#include "util.h"
#include "packet.h"

enum gs_packet_status_type {
        STATUS_LEVEL = 0x01,
        STATUS_EXP   = 0x02,
        STATUS_STR   = 0x03,
        STATUS_DEX   = 0x04,
        STATUS_CON   = 0x05,
        STATUS_INT   = 0x06,
        STATUS_WIT   = 0x07,
        STATUS_MEN   = 0x08,

        STATUS_CUR_HP = 0x09,
        STATUS_MAX_HP = 0x0a,
        STATUS_CUR_MP = 0x0b,
        STATUS_MAX_MP = 0x0c,

        STATUS_SP       = 0x0d,
        STATUS_CUR_LOAD = 0x0e,
        STATUS_MAX_LOAD = 0x0f,

        STATUS_P_ATK    = 0x11,
        STATUS_ATK_SPD  = 0x12,
        STATUS_P_DEF    = 0x13,
        STATUS_EVASION  = 0x14,
        STATUS_ACCURACY = 0x15,
        STATUS_CRITICAL = 0x16,
        STATUS_M_ATK    = 0x17,
        STATUS_CAST_SPD = 0x18,
        STATUS_M_DEF    = 0x19,
        STATUS_PVP_FLAG = 0x1a,
        STATUS_KARMA    = 0x1b,

        STATUS_CUR_CP = 0x21,
        STATUS_MAX_CP = 0x22,
};

struct gs_packet_status_attr {
        enum gs_packet_status_type type;
        i32_t value;
};

struct gs_packet_status {
        u32_t obj_id;
        struct gs_packet_status_attr attributes[35];
        u32_t count;
};

void gs_packet_status_add(
        struct gs_packet_status *dest,
        struct gs_packet_status_attr *src);

void gs_packet_status_pack(packet_t *dest, struct gs_packet_status *src);

#endif
