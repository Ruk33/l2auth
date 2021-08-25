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

static struct ls_rsa rsa_keys[MAX_CLIENTS]           = { 0 };
static struct ls_blowfish blowfish_keys[MAX_CLIENTS] = { 0 };

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

static int ls_session_rsa_decrypt(struct ls_rsa *rsa, byte_t *dest, byte_t *src)
{
        int size = 0;

        assert(rsa);
        assert(rsa->key);
        assert(dest);
        assert(src);

        size = RSA_size(rsa->key);

        return RSA_private_decrypt(size, src, dest, rsa->key, RSA_NO_PADDING);
}

struct ls_session *ls_session_new(struct ls_state *ls, struct os_io *socket)
{
        struct ls_session *session = 0;

        size_t id = 0;

        assert(ls);
        assert(socket);
        assert(ls->session_count < arr_size(ls->sessions));

        ls->session_count += recycle_id_get(&id, ls->session_instances);
        session = &ls->sessions[id];

        session->id       = id;
        session->socket   = socket;
        session->blowfish = &blowfish_keys[id];
        session->rsa      = &rsa_keys[id];

        ls_session_blowfish_init(session->blowfish);
        ls_session_rsa_init(session->rsa);

        return session;
}

void ls_session_free(struct ls_state *ls, struct ls_session *session)
{
        assert(ls);
        assert(session);

        RSA_free(session->rsa->key);
        BN_free(session->rsa->e);

        recycle_id(ls->session_instances, session->id);

        *session->rsa      = (struct ls_rsa){ 0 };
        *session->blowfish = (struct ls_blowfish){ 0 };
        *session           = (struct ls_session){ 0 };
}

struct ls_session *ls_session_find(struct ls_state *ls, struct os_io *socket)
{
        assert(ls);
        assert(socket);

        for (size_t i = 0; i < ls->session_count; i += 1) {
                if (ls->sessions[i].socket == socket) {
                        return &ls->sessions[i];
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

void ls_session_send_packet(
        struct ls_state *ls,
        struct ls_session *session,
        packet_t *src)
{
        assert(ls);
        assert(ls->send_response);
        assert(session);
        assert(session->socket);
        assert(src);
        ls->send_response(session->socket, src, (size_t) packet_size(src));
}

void ls_session_disconnected(struct ls_state *ls, struct ls_session *session)
{
        assert(ls);
        assert(session);
        ls_session_free(ls, session);
}

void ls_session_disconnect(struct ls_state *ls, struct ls_session *session)
{
        assert(ls);
        assert(ls->disconnect);
        assert(session);
        assert(session->socket);

        ls->disconnect(session->socket);
        ls_session_disconnected(ls, session);
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
        u16_t src_size      = 0;
        packet_t *src_body  = 0;
        packet_t *dest_body = 0;

        assert(session);
        assert(session->blowfish);
        assert(dest);
        assert(src);

        // Packet size without header size (2 bytes).
        src_size  = packet_size(src) - 2;
        src_body  = packet_body(src);
        dest_body = packet_body(dest);

        // Copy packet size.
        bytes_cpy(dest, src, 2);

        // Copy decrypted packet body.
        ls_session_blowfish_decrypt(
                session->blowfish, dest_body, src_body, src_size);
        // Copy decrypted content (ignoring byte containing packet type).
        ls_session_rsa_decrypt(session->rsa, dest_body + 1, dest_body + 1);
}
