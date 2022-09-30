#define _DEFAULT_SOURCE
#include <endian.h>

#include "include/l2auth.h"

u32 host_to_le32(u32 src)
{
    return htole32(src);
}

u32 le32_to_host(u32 src)
{
    return le32toh(src);
}

u32 host_to_be32(u32 src)
{
    return htobe32(src);
}

u32 be32_to_host(u32 src)
{
    return be32toh(src);
}
