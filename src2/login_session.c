#include "include/l2auth.h"

static void log_last_openssl_err(void)
{
    // must be at least 256 as stated by openssl documentation.
    // see https://www.openssl.org/docs/man1.1.1/man3/ERR_error_string.html
    char err[256] = {0};
    ERR_error_string_n(ERR_get_error(), err, sizeof(err) - 1);
    log("openssl error: %s", err);
}

static void rsa_scramble_modulo(struct rsa_modulus *dest)
{
    assert(dest);

    byte *n = dest->buf;
    for (int i = 0; i < 4; i++) {
        byte temp = n[i];
        n[i] = n[0x4d + i];
        n[0x4d + i] = temp;
    };

    // step 2 xor first 0x40 bytes with last 0x40 bytes
    for (int i = 0; i < 0x40; i++) {
        n[i] = (byte) (n[i] ^ n[0x40 + i]);
    };

    // step 3 xor bytes 0x0d-0x10 with bytes 0x34-0x38
    for (int i = 0; i < 4; i++) {
        n[0x0d + i] = (byte) (n[0x0d + i] ^ n[0x34 + i]);
    };

    // step 4 xor last 0x40 bytes with first 0x40 bytes
    for (int i = 0; i < 0x40; i++) {
        n[0x40 + i] = (byte) (n[0x40 + i] ^ n[i]);
    };
}

static int rsa_decrypt(struct login_session *session, struct packet *src)
{
    assert(session);
    assert(src);

    int size = RSA_size(session->rsa_key);
    byte *body_without_type = packet_body(src) + 1;
    int result = RSA_private_decrypt(
        size,
        body_without_type,
        body_without_type,
        session->rsa_key,
        RSA_NO_PADDING
    );

    if (result != -1)
        return 1;

    log_last_openssl_err();
    return 0;
}

static void blowfish_encrypt(struct login_session *session, struct packet *src)
{
    assert(session);
    assert(src);

    u32 tmp = 0;
    for (u16 i = 0, iters = packet_size(src) / 8; i < iters; i++) {
        u32 *body = (u32 *)(packet_body(src) + i * 8);
        u32 *tmp = body;
        // blowfish uses big endian
        *tmp = host_to_be32(*tmp);
        tmp++;
        *tmp = host_to_be32(*tmp);

        BF_ecb_encrypt(
            (byte *) body,
            (byte *) body,
            &session->blowfish_key, 
            BF_ENCRYPT
        );

        tmp = body;
        // back to little endian (endianess used by lineage 2)
        *tmp = be32_to_host(*tmp);
        tmp++;
        *tmp = be32_to_host(*tmp);
    }
}

static void blowfish_decrypt(struct login_session *session, struct packet *src)
{
    assert(session);
    assert(src);

    u32 tmp = 0;
    for (u16 i = 0, iters = (packet_padded_size(src) / 8); i < iters; i++) {
        u32 *body = (u32 *)(packet_body(src) + i * 8);
        u32 *tmp = body;
        // blowfish uses big endian
        *tmp = host_to_be32(*tmp);
        tmp++;
        *tmp = host_to_be32(*tmp);

        BF_ecb_encrypt(
            (byte *) body,
            (byte *) body,
            &session->blowfish_key,
            BF_DECRYPT
        );

        tmp = body;
        // back to little endian (endianess used by lineage 2)
        *tmp = be32_to_host(*tmp);
        tmp++;
        *tmp = be32_to_host(*tmp);
    }
}

int login_session_init(struct login_session *src)
{
    assert(src);

    byte raw_key[] = "_;5.]94-31==-%xT!^[$";
    BF_set_key(&src->blowfish_key, (int) (sizeof(raw_key)), raw_key);

    src->rsa_e = BN_new();
    if (!src->rsa_e) {
        log("unable to allocate memory for blowfish.");
        goto abort;
    }

    src->rsa_key = RSA_new();
    if (!src->rsa_key) {
        log("unable to allocate memory for rsa.");
        goto abort;
    }

    if (!BN_dec2bn(&src->rsa_e, "65537")) {
        log("unable to convert decimal to big num.");
        goto abort;
    }

    if (!RSA_generate_key_ex(src->rsa_key, 1024, src->rsa_e, 0)) {
        log("failed to generate rsa key.");
        goto abort;
    }

    src->active = 1;
    return 1;

abort:
    log_last_openssl_err();
    login_session_release(src);
    return 0;
}

void login_session_release(struct login_session *src)
{
    assert(src);
    if (src->rsa_key)
        RSA_free(src->rsa_key);
    if (src->rsa_e)
        BN_free(src->rsa_e);
    zero(src);
}

void login_session_rsa_modulus(struct rsa_modulus *dest, struct login_session *src)
{
    assert(dest);
    assert(src);

    const BIGNUM *n = 0;
    RSA_get0_key(src->rsa_key, &n, 0, 0);
    if (!BN_bn2bin(n, dest->buf)) {
        log("unable to copy modulus.");
        log_last_openssl_err();
    }
    rsa_scramble_modulo(dest);
}

void login_session_encrypt_packet(struct login_session *session, struct packet *src)
{
    assert(session);
    assert(src);
    packet_checksum(src);
    blowfish_encrypt(session, src);
}

int login_session_decrypt_packet(struct login_session *session, struct packet *src)
{
    assert(session);
    assert(src);

    // u16 size = packet_size(src);
    // ignore if not even the packet size header is here.
    // if (size <= 2) {
    //     log("trying to decrypt what appears to be an empty packet...");
    //     return 0;
    // }
    // copy decrypted packet body.
    blowfish_decrypt(session, src);
    // copy decrypted content (ignoring byte containing packet type).
    int result = rsa_decrypt(session, src);
    return result;
}
