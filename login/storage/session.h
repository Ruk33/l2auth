#ifndef STORAGE_SESSION_H
#define STORAGE_SESSION_H

#include "../rsa.h"
#include "../blowfish.h"

struct StorageSession {
        int rsa_decrypted;
        struct Rsa rsa;
        struct Blowfish blowfish;
        int playOK1;
        int playOK2;
};

#endif
