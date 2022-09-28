#include "include/l2auth.h"
#include "include/packet.h"

static void rsa_scramble_modulo(struct rsa_modulus *dest)
{
    assert(dest);

    byte temp = 0;
    byte *n = n = dest->buf;

    for (int i = 0; i < 4; i++) {
        temp = n[i];
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
    int result = RSA_private_decrypt(
        size,
        packet_body(src) + 1,
        packet_body(src) + 1,
        session->rsa_key,
        RSA_NO_PADDING
    );

    if (result == -1) {
        // must be at least 256 as stated by openssl documentation.
        // see https://www.openssl.org/docs/man1.1.1/man3/ERR_error_string.html
        char rsa_err[256] = {0};
        ERR_error_string_n(ERR_get_error(), rsa_err, sizeof(rsa_err));
        log("rsa decrypt problem: %s", rsa_err);
    }

    return result;
}

static void blowfish_encrypt(struct login_session *session, struct packet *src)
{
    assert(session);
    assert(src);

    u32 tmp = 0;
    for (size_t i = 0, iters = packet_size(src); i < iters; i += 8) {
        // blowfish uses big endian
        tmp = *((u32 *)(packet_body(src) + i));
        *((u32 *)(packet_body(src) + i)) = le32_to_be(tmp);
        tmp = *((u32 *)(packet_body(src) + i + 4));
        *((u32 *)(packet_body(src) + i + 4)) = le32_to_be(tmp);

        BF_ecb_encrypt(
            packet_body(src) + i,
            packet_body(src) + i,
            &session->blowfish_key,
            BF_ENCRYPT
        );

        // back to little endian (endianess used by lineage 2)
        tmp = be32_to_le(*((u32 *)(packet_body(src) + i)));
        *((u32 *)(packet_body(src) + i)) = tmp;
        tmp = be32_to_le(*((u32 *)(packet_body(src) + i + 4)));
        *((u32 *)(packet_body(src) + i + 4)) = tmp;
    }
}

static void blowfish_decrypt(struct login_session *session, struct packet *src)
{
    assert(session);
    assert(src);

    u32 tmp = 0;
    for (size_t i = 0, iters = packet_size(src); i < iters; i += 8) {
        // blowfish uses big endian
        tmp = *((u32 *)(packet_body(src) + i));
        *((u32 *)(packet_body(src) + i)) = le32_to_be(tmp);
        tmp = *((u32 *)(packet_body(src) + i + 4));
        *((u32 *)(packet_body(src) + i + 4)) = le32_to_be(tmp);

        BF_ecb_encrypt(
            packet_body(src) + i,
            packet_body(src) + i,
            &session->blowfish_key,
            BF_DECRYPT
        );

        // Back to little endian (endianess used by Lineage 2)
        tmp = be32_to_le(*((u32 *)(packet_body(src) + i)));
        *((u32 *)(packet_body(src) + i)) = tmp;
        tmp = be32_to_le(*((u32 *)(packet_body(src) + i + 4)));
        *((u32 *)(packet_body(src) + i + 4)) = tmp;
    }
}

void login_session_init(struct login_session *src)
{
    assert(src);

    byte raw_key[] = "_;5.]94-31==-%xT!^[$";
    BF_set_key(&src->blowfish_key, (int) (sizeof(raw_key)), raw_key);

    src->rsa_e = BN_new();
    src->rsa_key = RSA_new();
    BN_dec2bn(&src->rsa_e, "65537");
    RSA_generate_key_ex(src->rsa_key, 1024, src->rsa_e, 0);

    src->active = 1;
}

void login_session_release(struct login_session *src)
{
    assert(src);
    RSA_free(src->rsa_key);
    BN_free(src->rsa_e);
    memset(src, 0, sizeof(*src));
}

void login_session_rsa_modulus(struct rsa_modulus *dest, struct login_session *src)
{
    assert(dest);
    assert(src);

    const BIGNUM *n = 0;
    RSA_get0_key(src->rsa_key, &n, 0, 0);
    BN_bn2bin(n, dest->buf);
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

    u16 size = packet_size(src);
    // ignore if not even the packet size header is here.
    if (size <= 2)
        return 0;
    // copy decrypted packet body.
    blowfish_decrypt(session, src);
    // copy decrypted content (ignoring byte containing packet type).
    int decrypt_result = rsa_decrypt(session, src);
    return decrypt_result != -1;
}
