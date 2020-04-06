#ifndef L2AUTH_ENDIAN_H
#define L2AUTH_ENDIAN_H

unsigned int decode32le(const void* src);
unsigned int decode32be(const void* src);
void encode32le(void* dst, unsigned int val);
void encode32be(void* dst, unsigned int val);

#endif
