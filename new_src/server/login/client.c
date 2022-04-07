#include <assert.h>
#include <openssl/bn.h>
#include <openssl/rsa.h>
#include <openssl/blowfish.h>
#include "../../include/util.h"
#include "include/client.h"

#ifndef MAX_CLIENTS
#define MAX_CLIENTS (32)
// #error MAX_CLIENTS must be defined as an int.
#endif

struct rsa {
    BIGNUM *e;
    RSA *key;
};

struct blowfish {
    BF_KEY key;
};

struct client_keys {
    struct client *client;
    struct rsa rsa;
    struct blowfish blowfish;
};

static struct client_keys keys[MAX_CLIENTS] = { 0 };

static void blowfish_init(struct blowfish *src)
{
    byte raw_key[] = "_;5.]94-31==-%xT!^[$";
    assert(src);
    BF_set_key(&src->key, (int) sizeof(raw_key), raw_key);
}

static int rsa_init(struct rsa *src)
{
    assert(src);

    src->e   = BN_new();
    src->key = RSA_new();

    if (!src->e || !src->key || !BN_dec2bn(&src->e, "65537")) {
        // Don't worry about leaks (will be cleared in client_free)
        return 0;
    }

    RSA_generate_key_ex(src->key, 1024, src->e, 0);

    return 1;
}

static int find_client_keys(struct client *src, struct client_keys **dest)
{
    assert(src);
    assert(dest);

    for (size_t i = 0; i < MAX_CLIENTS; i += 1) {
        if (keys[i].client == src) {
            *dest = &keys[i];
            return 1;
        }
    }

    return 0;
}

static int rsa_decrypt(struct client *client, struct buffer *dest, byte *src)
{
    struct client_keys *key = 0;

    int size = 0;

    assert(client);
    assert(dest);
    assert(dest->buf);
    assert(src);

    if (!find_client_keys(client, &key)) {
        return -1;
    }

    size = RSA_size(key->rsa.key);

    if (dest->size < size) {
        return -1;
    }

    return RSA_private_decrypt(size, src, dest->buf, key->rsa.key, RSA_NO_PADDING);
}

static int rsa_scramble_modulo(struct buffer *dest)
{
    byte temp = 0;
    byte *n = 0;

    assert(dest);
    assert(dest->buf);

    if (dest->size < 128) {
        return 0;
    }

    n = dest->buf;

    for (i32 i = 0; i < 4; i++) {
        temp = n[0x00 + i];
        n[0x00 + i] = n[0x4d + i];
        n[0x4d + i] = temp;
    };

    // Step 2 xor first 0x40 bytes with last 0x40 bytes
    for (i32 i = 0; i < 0x40; i++) {
        n[i] = (byte) (n[i] ^ n[0x40 + i]);
    };

    // Step 3 xor bytes 0x0d-0x10 with bytes 0x34-0x38
    for (i32 i = 0; i < 4; i++) {
        n[0x0d + i] = (byte) (n[0x0d + i] ^ n[0x34 + i]);
    };

    // Step 4 xor last 0x40 bytes with first 0x40 bytes
    for (i32 i = 0; i < 0x40; i++) {
        n[0x40 + i] = (byte) (n[0x40 + i] ^ n[i]);
    };

    return 1;
}

static int blowfish_encrypt(struct client *client, 
                            struct buffer *dest, 
                            struct buffer *src)
{
    struct client_keys *key = 0;

    u32 tmp = 0;

    assert(client);
    assert(dest);
    assert(dest->buf);
    assert(src);
    assert(src->buf);

    if (!find_client_keys(client, &key)) {
        return 0;
    }

    if (dest->size < (src->used + 7) & (~7)) {
        return 0;
    }

    for (size_t i = 0, iters = (src->used + 7) & (~7); i < iters; i += 8) {
        // Blowfish uses big endian
        decode32le(&tmp, ((byte *) src->buf) + i);
        encode32be(((byte *) dest->buf) + i, tmp);
        decode32le(&tmp, ((byte *) src->buf) + i + 4);
        encode32be(((byte *) dest->buf) + i + 4, tmp);

        BF_ecb_encrypt(
            ((byte *) dest->buf) + i,
            ((byte *) dest->buf) + i,
            &key->blowfish.key, BF_ENCRYPT
        );

        // Back to little endian (endianess used by Lineage 2)
        decode32be(&tmp, ((byte *) dest->buf) + i);
        encode32le(((byte *) dest->buf) + i, tmp);
        decode32be(&tmp, ((byte *) dest->buf) + i + 4);
        encode32le(((byte *) dest->buf) + i + 4, tmp);
    }

    return 1;
}

static int blowfish_decrypt(struct client *client, 
                            struct buffer *dest, 
                            struct buffer *src)
{
    struct client_keys *key = 0;

    u32 tmp = 0;

    assert(client);
    assert(dest);
    assert(dest->buf);
    assert(src);
    assert(src->buf);

    if (!find_client_keys(client, &key)) {
        return 0;
    }

    if (dest->size < (src->used + 7) & (~7)) {
        return 0;
    }

    for (size_t i = 0, iters = (src->used + 7) & (~7); i < iters; i += 8) {
        // Blowfish uses big endian
        decode32le(&tmp, ((byte *) src->buf) + i);
        encode32be(((byte *) dest->buf) + i, tmp);
        decode32le(&tmp, ((byte *) src->buf) + i + 4);
        encode32be(((byte *) dest->buf) + i + 4, tmp);

        BF_ecb_encrypt(
            ((byte *) dest->buf) + i,
            ((byte *) dest->buf) + i,
            &key->blowfish.key,
            BF_DECRYPT
        );

        // Back to little endian (endianess used by Lineage 2)
        decode32be(&tmp, ((byte *) dest->buf) + i);
        encode32le(((byte *) dest->buf) + i, tmp);
        decode32be(&tmp, ((byte *) dest->buf) + i + 4);
        encode32le(((byte *) dest->buf) + i + 4, tmp);
    }

    return 1;
}

int client_init(struct client *src)
{
    struct client_keys *key = 0;

    assert(src);

    // Find free usable keys.
    for (size_t i = 0; i < MAX_CLIENTS; i += 1) {
        if (!keys[i].client) {
            key = &keys[i];
            key->client = src;
            break;
        }
    }

    if (!key || !rsa_init(&key->rsa)) {
        client_free(src);
        return 0;
    }

    blowfish_init(&key->blowfish);
    return 1;
}

void client_free(struct client *src)
{
    struct client_keys *key = 0;

    assert(src);

    if (find_client_keys(src, &key)) {
        if (key->rsa.key) RSA_free(key->rsa.key);
        if (key->rsa.e) BN_free(key->rsa.e);
        *key = (struct client_keys) { 0 };
    }

    *src = (struct client) { 0 };
}

int client_rsa_modulus(struct client *src, struct buffer *dest)
{
    struct client_keys *key = 0;
    const BIGNUM *n = 0;

    assert(src);
    assert(dest);
    assert(dest->buf);

    if (!find_client_keys(src, &key)) {
        return 0;
    }

    RSA_get0_key(key->rsa.key, &n, 0, 0);

    if (BN_num_bytes(n) > dest->size) {
        return 0;
    }

    BN_bn2bin(n, (unsigned char *) dest->buf);
    return rsa_scramble_modulo(dest);
}

int client_encrypt_packet(struct client *client,
                          struct buffer *dest,
                          struct packet *src)
{
    u16 padded_size = 0;

    assert(client);
    assert(dest);
    assert(dest->buf);
    assert(src);

    padded_size = packet_padded_size(src);

    // Copy packet size to dest.
    *((u16 *) dest->buf) = padded_size;
    return blowfish_encrypt(
        client,
        &B(((byte *) dest->buf) + 2, dest->size - 2, 0),
        &B(packet_body(src), padded_size, padded_size)
    );
}

int client_decrypt_packet(struct client *client,
                          struct packet *dest,
                          struct packet *src)
{
    struct client_keys *key = 0;
    u16 src_size = 0;

    int blowfish_succeed = 0;

    assert(client);
    assert(dest);
    assert(src);

    if (!find_client_keys(client, &key)) {
        return 0;
    }

    // Copy packet size (without including the packet size header)
    src_size = packet_size(src) - 2;
    *((u16 *) dest->buf) = src_size;

    // Copy decrypted packet body.
    blowfish_succeed = blowfish_decrypt(
        client,
        &B(packet_body(dest), sizeof(*dest) - 2, 0),
        &B(packet_body(src), src_size, src_size)
    );

    if (!blowfish_succeed) {
        return 0;
    }

    // Copy decrypted content (ignoring byte containing packet type).
    return rsa_decrypt(
        client,
        &B(packet_body(dest) + 1, sizeof(*dest) - 3, 0),
        packet_body(dest) + 1
    );
}