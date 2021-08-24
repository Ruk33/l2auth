#ifndef INCLUDE_LS_TYPES_H
#define INCLUDE_LS_TYPES_H

struct ls_rsa;

struct ls_blowfish;

struct ls_session {
        size_t id;
        struct os_io *socket;
        struct ls_blowfish *blowfish;
        struct ls_rsa *rsa;
        i32_t playOK1;
        i32_t playOK2;
};

struct ls_account {
        char username[14];
        byte_t encrypted_password[64];
};

#endif
