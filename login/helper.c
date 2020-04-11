#include <string.h>
#include <login/helper.h>

void login_helper_username_from_packet
(
        unsigned char *decrypted_packet,
        unsigned char *dest
)
{
        memcpy(dest, decrypted_packet + 0x62, 14);
}

void login_helper_password_from_packet
(
        unsigned char *decrypted_packet,
        unsigned char* dest
)
{
        memcpy(dest, decrypted_packet + 0x70, 16);
}
