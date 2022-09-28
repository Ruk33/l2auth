#ifndef PACKET_H
#define PACKET_H

#include "l2auth.h"

#define packet_write_arr(dest, src) \
    (packet_write(dest, src, sizeof(src)))

struct packet {
    byte buf[kb(8)];
    byte *read_cursor;
    int checksum_done;
};

u16 packet_size(struct packet *src);
u16 packet_padded_size(struct packet *src);
u8 packet_type(struct packet *src);
void packet_set_type(struct packet *src, u8 type);
byte *packet_body(struct packet *src);
void packet_checksum(struct packet *src);

void packet_write(struct packet *dest, void *src, size_t n);
void packet_write_u8(struct packet *dest, u8 src);
void packet_write_u16(struct packet *dest, u16 src);
void packet_write_u32(struct packet *dest, u32 src);
void packet_write_i8(struct packet *dest, i8 src);
void packet_write_i16(struct packet *dest, i16 src);
void packet_write_i32(struct packet *dest, i32 src);
void packet_write_float(struct packet *dest, float src);
void packet_write_double(struct packet *dest, double src);

void packet_read(void *dest, struct packet *src, size_t n);
void packet_read_u8(u8 *dest, struct packet *src);
void packet_read_u16(u16 *dest, struct packet *src);
void packet_read_u32(u32 *dest, struct packet *src);
void packet_read_i8(i8 *dest, struct packet *src);
void packet_read_i16(i16 *dest, struct packet *src);
void packet_read_i32(i32 *dest, struct packet *src);
void packet_read_float(float *dest, struct packet *src);
void packet_read_double(double *dest, struct packet *src);

#endif
