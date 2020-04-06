#ifndef L2AUTH_PACKET_HELPER_H
#define L2AUTH_PACKET_HELPER_H

#include <stdlib.h>

unsigned short packet_helper_size(void* packet);
unsigned short packet_helper_full_size(void* packet);
unsigned char packet_helper_type(void* packet);
void packet_helper_content(void* packet, void* dest, size_t dest_size);

#endif
