#ifndef L2AUTH_LOGIN_HELPER_H
#define L2AUTH_LOGIN_HELPER_H

void login_helper_username_from_packet
(
        unsigned char *decrypted_packet,
        unsigned char *dest
);
void login_helper_password_from_packet
(
        unsigned char *decrypted_packet,
        unsigned char* dest
);

#endif
