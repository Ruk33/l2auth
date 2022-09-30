#ifndef ENDIAN_H
#define ENDIAN_H

#include "l2auth.h"

u32 host_to_le32(u32 src);
u32 le32_to_host(u32 src);
u32 host_to_be32(u32 src);
u32 be32_to_host(u32 src);

#endif
