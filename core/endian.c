#include <assert.h>
#include <core/endian.h>

unsigned int decode32le(const void* src)
{
        assert(src);
        const unsigned char* buf = src;
        return (unsigned int) buf[0] | ((unsigned int) buf[1] << 8)
                | ((unsigned int) buf[2] << 16) | ((unsigned int) buf[3] << 24);
}

unsigned int decode32be(const void* src)
{
        assert(src);
        const unsigned char* buf = src;
        return (unsigned int) buf[3] | ((unsigned int) buf[2] << 8)
                | ((unsigned int) buf[1] << 16) | ((unsigned int) buf[0] << 24);
}

void encode32le(void* dst, unsigned int val)
{
        assert(dst);
        unsigned char* buf = dst;
        buf[0] = (unsigned char) val;
        buf[1] = (unsigned char) (val >> 8);
        buf[2] = (unsigned char) (val >> 16);
        buf[3] = (unsigned char) (val >> 24);
}

void encode32be(void* dst, unsigned int val)
{
        assert(dst);
        unsigned char* buf = dst;
        buf[3] = (unsigned char) val;
        buf[2] = (unsigned char) (val >> 8);
        buf[1] = (unsigned char) (val >> 16);
        buf[0] = (unsigned char) (val >> 24);
}
