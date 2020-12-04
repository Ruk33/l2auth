#include <assert.h>
#include <stdlib.h>
#include "blowfish.h"
#include "packet.h"
#include "decrypt_packet.h"

void decrypt_packet(struct Blowfish *blowfish, unsigned char *dest, unsigned char *packet, size_t packet_size)
{
        assert(blowfish);
        assert(dest);
        assert(packet);
        assert(packet_size);

        unsigned char decrypted[65536] = {0};
        size_t size_header = sizeof(short);
        size_t type_header = sizeof(char);

        unsigned char *packet_content = packet + size_header;
        unsigned char packet_type = 0;
        unsigned char *packet_body = NULL;

        blowfish_decrypt(blowfish, decrypted, packet_content, packet_size - size_header);

        packet_type = decrypted[0];
        packet_body = decrypted + type_header;

        packet_build(dest, packet_type, packet_body, packet_size - size_header - type_header);
}
