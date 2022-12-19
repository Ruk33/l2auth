// required by endian.h
#define _DEFAULT_SOURCE
#include <endian.h>
#include "include/l2auth.h"

u32 host_to_le32(u32 src)
{
    u32 result = htole32(src);
    return result;
}

u32 le32_to_host(u32 src)
{
    u32 result = le32toh(src);
    return result;
}

u32 le32_to_be32(u32 src)
{
    u32 host = le32_to_host(src);
    u32 result = host_to_be32(host);
    return result;
}

u32 host_to_be32(u32 src)
{
    u32 result = htobe32(src);
    return result;
}

u32 be32_to_host(u32 src)
{
    u32 result = be32toh(src);
    return result;
}

u32 be32_to_le32(u32 src)
{
    u32 host = be32_to_host(src);
    u32 result = host_to_le32(host);
    return result;
}
