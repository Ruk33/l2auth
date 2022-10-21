#ifndef LOGIN_SESSION_H
#define LOGIN_SESSION_H

#include <openssl/err.h>
#include <openssl/bn.h>
#include <openssl/rsa.h>
#include <openssl/blowfish.h>
#include "coroutine.h"
#include "l2auth.h"

struct rsa_modulus {
    byte buf[128];
};

struct login_session {
    u32 id;
    u32 play_ok1;
    u32 play_ok2;
    int active;
    int closed;
    size_t read;
    struct packet response;
    struct packet request;
    BIGNUM *rsa_e;
    RSA *rsa_key;
    BF_KEY blowfish_key;
    struct co_ctx state;
};

int login_session_init(struct login_session *src);
void login_session_drop(struct login_session *src);
void login_session_release(struct login_session *src);
void login_session_rsa_modulus(struct rsa_modulus *dest, struct login_session *src);
void login_session_encrypt_packet(struct login_session *session, struct packet *src);
int login_session_decrypt_packet(struct login_session *session, struct packet *src);

#endif
