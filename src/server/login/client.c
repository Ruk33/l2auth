#include <assert.h>
#include <stdio.h>
#include <endian.h>
#include <openssl/err.h>
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

    return RSA_generate_key_ex(src->key, 1024, src->e, 0);
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

static int rsa_decrypt(struct client *client, struct packet *src)
{
    struct client_keys *key = 0;
    // Must be at least 256 as stated by openssl
    // documentation.
    // See https://www.openssl.org/docs/man1.1.1/man3/ERR_error_string.html
    char rsa_err[256] = { 0 };

    int size = 0;
    int result = 0;

    assert(client);
    assert(src);

    if (!find_client_keys(client, &key)) {
        printf("unable to find client rsa key, failing to decrypt.\n");
        return -1;
    }

    size = RSA_size(key->rsa.key);

    result = RSA_private_decrypt(
        size,
        packet_body(src) + 1,
        packet_body(src) + 1,
        key->rsa.key,
        RSA_NO_PADDING
    );

    if (result == -1) {
        ERR_error_string_n(ERR_get_error(), rsa_err, sizeof(rsa_err));
        printf("decrypt problem: %s\n", rsa_err);
    }

    return result;
}

static int rsa_scramble_modulo(struct client_modulus *dest)
{
    byte temp = 0;
    byte *n = 0;

    assert(dest);

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

static int blowfish_encrypt(struct client *client, struct packet *src)
{
    struct client_keys *key = 0;

    u32 tmp = 0;

    assert(client);
    assert(src);

    if (!find_client_keys(client, &key)) {
        return 0;
    }

    for (size_t i = 0, iters = ((packet_size(src) + 7) & (~7)); i < iters; i += 8) {
        // Blowfish uses big endian
        tmp = *((u32 *)(packet_body(src) + i));
        *((u32 *)(packet_body(src) + i)) = htobe32(tmp);
        tmp = *((u32 *)(packet_body(src) + i + 4));
        *((u32 *)(packet_body(src) + i + 4)) = htobe32(tmp);

        BF_ecb_encrypt(
            packet_body(src) + i,
            packet_body(src) + i,
            &key->blowfish.key, BF_ENCRYPT
        );

        // Back to little endian (endianess used by Lineage 2)
        tmp = be32toh(*((u32 *)(packet_body(src) + i)));
        *((u32 *)(packet_body(src) + i)) = tmp;
        tmp = be32toh(*((u32 *)(packet_body(src) + i + 4)));
        *((u32 *)(packet_body(src) + i + 4)) = tmp;
    }

    return 1;
}

static int blowfish_decrypt(struct client *client, struct packet *src)
{
    struct client_keys *key = 0;

    u32 tmp = 0;

    assert(client);
    assert(src);

    if (!find_client_keys(client, &key)) {
        printf("blowfish key not found for client. unable to decrypt.\n");
        return 0;
    }

    if (!packet_size(src)) {
        printf("decrypting empty packet with blowfish? failing.\n");
        return 0;
    }

    for (size_t i = 0, iters = ((packet_size(src) + 7) & (~7)); i < iters; i += 8) {
        // Blowfish uses big endian
        tmp = *((u32 *)(packet_body(src) + i));
        *((u32 *)(packet_body(src) + i)) = htobe32(tmp);
        tmp = *((u32 *)(packet_body(src) + i + 4));
        *((u32 *)(packet_body(src) + i + 4)) = htobe32(tmp);

        BF_ecb_encrypt(
            packet_body(src) + i,
            packet_body(src) + i,
            &key->blowfish.key,
            BF_DECRYPT
        );

        // Back to little endian (endianess used by Lineage 2)
        tmp = be32toh(*((u32 *)(packet_body(src) + i)));
        *((u32 *)(packet_body(src) + i)) = tmp;
        tmp = be32toh(*((u32 *)(packet_body(src) + i + 4)));
        *((u32 *)(packet_body(src) + i + 4)) = tmp;
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

int client_rsa_modulus(struct client *src, struct client_modulus *dest)
{
    struct client_keys *key = 0;
    const BIGNUM *n = 0;

    assert(src);
    assert(dest);

    if (!find_client_keys(src, &key)) {
        return 0;
    }

    RSA_get0_key(key->rsa.key, &n, 0, 0);
    BN_bn2bin(n, dest->buf);

    return rsa_scramble_modulo(dest);
}

void client_gen_ok_ids(struct client *src)
{
    assert(src);
    src->playOK1 = 1994;
    src->playOK2 = 4991;
    TODO("get random login ok 1 and login ok 2 instead of hardcoding the values.");
}

int client_encrypt_packet(struct client *client, struct packet *src)
{
    int blowfish_succeed = 0;
    assert(client);
    assert(src);

    blowfish_succeed = blowfish_encrypt(client, src);

    if (blowfish_succeed) {
        *((u16 *) src->buf) = packet_padded_size(src);
    }

    return blowfish_succeed;
}

int client_decrypt_packet(struct client *client, struct packet *src)
{
    struct client_keys *key = 0;
    u16 src_size = 0;

    int blowfish_succeed = 0;

    assert(client);
    assert(src);

    if (!find_client_keys(client, &key)) {
        return 0;
    }

    // Packet size without including the packet size header.
    src_size = MAX(0, packet_size(src) - 2);

    if (!src_size) {
        return 0;
    }

    // Copy decrypted packet body.
    blowfish_succeed = blowfish_decrypt(client, src);

    if (!blowfish_succeed) {
        return 0;
    }

    // Copy decrypted content (ignoring byte containing packet type).
    return rsa_decrypt(client, src) != -1;
}