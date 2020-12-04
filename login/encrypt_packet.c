#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "blowfish.h"
#include "packet.h"
#include "encrypt_packet.h"

void encrypt_packet(struct Blowfish *blowfish, unsigned char *dest, unsigned char *packet)
{
        assert(blowfish);
        assert(dest);
        assert(packet);

        size_t size_header = sizeof(short);
        unsigned char *packet_content = packet + size_header;
        unsigned short encrypted_size = size_header;

        encrypted_size += (unsigned short) blowfish_encrypt(
                blowfish,
                dest + size_header,
                packet_content,
                packet_size(packet) - size_header
        );

        memcpy(dest, (unsigned char *) &encrypted_size, size_header);
}
