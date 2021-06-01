#include "include/util.h"

void bytes_cpy(byte_t *dest, byte_t *src, size_t n)
{
        while (dest && src && n) {
                *dest = *src;
                dest += 1;
                src += 1;
                n -= 1;
        }
}

void bytes_zero(byte_t *dest, size_t n)
{
        while (dest && n) {
                *dest = 0;
                dest += 1;
                n -= 1;
        }
}

u32_t decode32le(byte_t *buf)
{
        return (u32_t) buf[0] | ((u32_t) buf[1] << 8) | ((u32_t) buf[2] << 16) |
               ((u32_t) buf[3] << 24);
}

u32_t decode32be(byte_t *buf)
{
        return (u32_t) buf[3] | ((u32_t) buf[2] << 8) | ((u32_t) buf[1] << 16) |
               ((u32_t) buf[0] << 24);
}

void encode32le(byte_t *buf, u32_t val)
{
        buf[0] = (byte_t) val;
        buf[1] = (byte_t)(val >> 8);
        buf[2] = (byte_t)(val >> 16);
        buf[3] = (byte_t)(val >> 24);
}

void encode32be(byte_t *buf, u32_t val)
{
        buf[3] = (byte_t) val;
        buf[2] = (byte_t)(val >> 8);
        buf[1] = (byte_t)(val >> 16);
        buf[0] = (byte_t)(val >> 24);
}
