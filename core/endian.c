#ifndef L2AUTH_ENDIAN_C
#define L2AUTH_ENDIAN_C

unsigned int decode32le(const void* src)
{
    const unsigned char* buf = src;
    return (unsigned int) buf[0] | ((unsigned int) buf[1] << 8)
            | ((unsigned int) buf[2] << 16) | ((unsigned int) buf[3] << 24);
}

unsigned int decode32be(const void* src)
{
    const unsigned char* buf = src;
    return (unsigned int) buf[3] | ((unsigned int) buf[2] << 8)
            | ((unsigned int) buf[1] << 16) | ((unsigned int) buf[0] << 24);
}

void encode32le(void* dst, unsigned int val)
{
    unsigned char* buf = dst;
    buf[0] = val;
    buf[1] = val >> 8;
    buf[2] = val >> 16;
    buf[3] = val >> 24;
}

void encode32be(void* dst, unsigned int val)
{
    unsigned char* buf = dst;
    buf[3] = val;
    buf[2] = val >> 8;
    buf[1] = val >> 16;
    buf[0] = val >> 24;
}

#endif //L2AUTH_ENDIAN_C