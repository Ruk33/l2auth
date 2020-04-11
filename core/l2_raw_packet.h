#ifndef L2AUTH_RAW_PACKET_H
#define L2AUTH_RAW_PACKET_H

typedef unsigned short l2_raw_packet_size;
typedef unsigned char l2_raw_packet;

/**
 * Calculate packet size including metadata
 * Metadata for raw packet is composed of:
 * 
 * packet size (unsigned short)
 */
l2_raw_packet_size l2_raw_packet_calculate_size(unsigned short content_size);

void l2_raw_packet_init
(
        l2_raw_packet* packet,
        unsigned char* content,
        unsigned short content_size
);

/**
 * Get packet size content (ignoring metadata)
 */
l2_raw_packet_size l2_raw_packet_get_size(l2_raw_packet* packet);

void l2_raw_packet_content
(
        l2_raw_packet* packet,
        unsigned char* dest,
        unsigned short start,
        unsigned short end
);

#endif
