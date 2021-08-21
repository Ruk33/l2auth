#include <assert.h>
#include <openssl/bn.h>
#include <openssl/rsa.h>
#include <openssl/blowfish.h>
#include "include/util.h"
#include "include/recycle_id.h"
#include "include/config.h"
#include "include/os_io.h"
#include "include/ls_session.h"

struct ls_rsa {
        BIGNUM *e;
        RSA *key;
};

struct ls_blowfish {
        BF_KEY key;
};

static struct ls_session sessions[MAX_CLIENTS]       = { 0 };
static struct ls_rsa rsa_keys[MAX_CLIENTS]           = { 0 };
static struct ls_blowfish blowfish_keys[MAX_CLIENTS] = { 0 };

static size_t session_instances[MAX_CLIENTS] = { 0 };
static size_t session_count                  = 0;

static void ls_session_blowfish_init(struct ls_blowfish *dest)
{
        byte_t raw_key[] = "_;5.]94-31==-%xT!^[$";

        assert(dest);
        BF_set_key(&dest->key, (int) sizeof(raw_key), raw_key);
}

static void ls_session_blowfish_encrypt(
        struct ls_blowfish *bf,
        byte_t *dest,
        byte_t *src,
        size_t n)
{
        assert(bf);
        assert(dest);
        assert(src);

        for (size_t i = 0, iters = (n + 7) & (~7); i < iters; i += 8) {
                // Blowfish uses big endian
                encode32be(dest + i, decode32le(src + i));
                encode32be(dest + i + 4, decode32le(src + i + 4));

                BF_ecb_encrypt((dest + i), (dest + i), &bf->key, BF_ENCRYPT);

                // Back to little endian (endianess used by Lineage 2)
                encode32le(dest + i, decode32be(dest + i));
                encode32le(dest + i + 4, decode32be(dest + i + 4));
        }
}

static void ls_session_blowfish_decrypt(
        struct ls_blowfish *bf,
        byte_t *dest,
        byte_t *src,
        size_t n)
{
        assert(bf);
        assert(dest);
        assert(src);

        for (size_t i = 0, iters = (n + 7) & (~7); i < iters; i += 8) {
                // Blowfish uses big endian
                encode32be(dest + i, decode32le(src + i));
                encode32be(dest + i + 4, decode32le(src + i + 4));

                BF_ecb_encrypt((dest + i), (dest + i), &bf->key, BF_DECRYPT);

                // Back to little endian (endianess used by Lineage 2)
                encode32le(dest + i, decode32be(dest + i));
                encode32le(dest + i + 4, decode32be(dest + i + 4));
        }
}

static void ls_session_rsa_scramble_modulo(byte_t *n)
{
        byte_t temp = 0;

        int i = 0;

        assert(n);

        for (i = 0; i < 4; i++) {
                temp        = n[0x00 + i];
                n[0x00 + i] = n[0x4d + i];
                n[0x4d + i] = temp;
        };

        // step 2 xor first 0x40 bytes with last 0x40 bytes
        for (i = 0; i < 0x40; i++) {
                n[i] = (byte_t)(n[i] ^ n[0x40 + i]);
        };

        // step 3 xor bytes 0x0d-0x10 with bytes 0x34-0x38
        for (i = 0; i < 4; i++) {
                n[0x0d + i] = (byte_t)(n[0x0d + i] ^ n[0x34 + i]);
        };

        // step 4 xor last 0x40 bytes with first 0x40 bytes
        for (i = 0; i < 0x40; i++) {
                n[0x40 + i] = (byte_t)(n[0x40 + i] ^ n[i]);
        };
}

static void ls_session_rsa_init(struct ls_rsa *dest)
{
        assert(dest);

        dest->e   = BN_new();
        dest->key = RSA_new();

        assert(dest->e);
        assert(dest->key);

        BN_dec2bn(&dest->e, "65537");
        RSA_generate_key_ex(dest->key, 1024, dest->e, 0);
}

static size_t ls_session_rsa_size(struct ls_rsa *src)
{
        assert(src);
        return (size_t) RSA_size(src->key);
}

static int ls_session_rsa_decrypt(struct ls_rsa *rsa, byte_t *dest, byte_t *src)
{
        int size = 0;

        assert(rsa);
        assert(dest);
        assert(src);

        size = (int) ls_session_rsa_size(rsa);

        return RSA_private_decrypt(size, src, dest, rsa->key, RSA_NO_PADDING);
}

struct ls_session *ls_session_new(struct os_io *socket)
{
        struct ls_session *session = 0;

        size_t id = 0;

        assert(socket);
        assert(session_count < arr_size(sessions));

        session_count += recycle_id_get(&id, session_instances);
        session = &sessions[id];

        session->socket   = socket;
        session->blowfish = &blowfish_keys[id];
        session->rsa      = &rsa_keys[id];

        ls_session_blowfish_init(session->blowfish);
        ls_session_rsa_init(session->rsa);

        return session;
}

struct ls_session *ls_session_find(struct os_io *socket)
{
        assert(socket);

        for (size_t i = 0; i < session_count; i += 1) {
                if (sessions[i].socket == socket) {
                        return &sessions[i];
                }
        }

        return 0;
}

void ls_session_rsa_modulus(struct ls_session *session, byte_t *dest)
{
        const BIGNUM *n = 0;

        assert(session);
        assert(session->rsa);
        assert(dest);

        RSA_get0_key(session->rsa->key, &n, 0, 0);
        BN_bn2bin(n, dest);
        ls_session_rsa_scramble_modulo(dest);
}

void ls_session_encrypt_packet(
        struct ls_session *session,
        byte_t *dest,
        packet_t *src)
{
        u16_t body_size      = 0;
        u16_t encrypted_size = 0;

        assert(session);
        assert(session->blowfish);
        assert(dest);
        assert(src);

        body_size      = packet_size(src) - 2;
        encrypted_size = ((body_size + 7) & (~7)) + 2;

        bytes_cpy(dest, (byte_t *) &encrypted_size, 2);
        ls_session_blowfish_encrypt(
                session->blowfish, dest + 2, src + 2, body_size);
}

void ls_session_decrypt_packet(
        struct ls_session *session,
        packet_t *dest,
        byte_t *src)
{
        u16_t src_size = 0;

        assert(session);
        assert(session->blowfish);
        assert(dest);
        assert(src);

        src_size = packet_size(src);

        bytes_cpy(dest, src, 2);
        ls_session_blowfish_decrypt(
                session->blowfish, dest + 2, src + 2, src_size - 2);
}
