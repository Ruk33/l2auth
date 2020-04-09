#ifndef L2AUTH_PACKET_H
#define L2AUTH_PACKET_H

#include <core/l2_raw_packet.h>

typedef l2_raw_packet l2_packet;
typedef unsigned char l2_packet_type;

l2_raw_packet_size l2_packet_calculate_size(unsigned short content_size);
void l2_packet_init
(
        l2_packet* packet,
        l2_packet_type type,
        unsigned char* content,
        unsigned short content_size
);
l2_packet* l2_packet_new
(
        l2_packet_type type,
        unsigned char* content,
        unsigned short content_size
);
l2_packet_type l2_packet_get_type(l2_packet* packet);
unsigned short l2_packet_get_content_size(l2_packet* packet);
void l2_packet_content
(
        l2_packet* packet,
        unsigned char* dest,
        unsigned short start,
        unsigned short end
);

#endif
