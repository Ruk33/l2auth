#ifndef INCLUDE_PLATFORM_H
#define INCLUDE_PLATFORM_H

#include "util.h"

struct platform_socket;

int platform_ip_to_u32(u32_t *dest, char *ip);

#endif
