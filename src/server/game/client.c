#include <assert.h>
#include "../../include/util.h"
#include "include/client.h"

void client_init(struct client *src)
{
    struct key key = {{ 0x94, 0x35, 0x00, 0x00, 0xa1, 0x6c, 0x54, 0x87 }};

    assert(src);

    src->id = 1;
    src->encrypt_key = key;
    src->decrypt_key = key;
}

void client_encrypt(struct client *client, struct packet *src)
{
    u16 src_size = 0;
    byte *src_body = 0;

    i32 temp  = 0;
    i32 temp2 = 0;
    u32 old = 0;

    assert(client);
    assert(src);

    src_size = packet_size(src);
    src_body = packet_body(src);

    for (u16 i = 0; i < src_size; i++) {
        temp2 = src_body[i] & 0xff;
        src_body[i] = (byte) (temp2 ^ client->encrypt_key.buf[i & 7] ^ temp);
        temp = src_body[i];
    }

    old  = ((u32) client->encrypt_key.buf[0] & 0xff);
    old |= ((u32) client->encrypt_key.buf[1] << 8 & 0xff00);
    old |= ((u32) client->encrypt_key.buf[2] << 0x10 & 0xff0000);
    old |= ((u32) client->encrypt_key.buf[3] << 0x18 & 0xff000000);

    old += src_size;

    client->encrypt_key.buf[0] = (byte) (old & 0xff);
    client->encrypt_key.buf[1] = (byte) (old >> 0x08 & 0xff);
    client->encrypt_key.buf[2] = (byte) (old >> 0x10 & 0xff);
    client->encrypt_key.buf[3] = (byte) (old >> 0x18 & 0xff);

    // Update packet size.
    *((u16 *) src) = packet_padded_size(src);
}

void client_decrypt(struct client *client, struct packet *src)
{
    byte *src_body  = 0;
    u16 body_size = 0;

    i32 temp  = 0;
    i32 temp2 = 0;
    u32 old = 0;

    assert(client);
    assert(src);

    if (!client->conn_encrypted) {
        return;
    }

    body_size = packet_size(src);
    body_size = body_size > 1 ? body_size - 2 : body_size;

    src_body  = packet_body(src);

    for (u16 i = 0; i < body_size; i++) {
        temp2 = src_body[i];
        src_body[i] = (byte) (temp2 ^ client->decrypt_key.buf[i & 7] ^ temp);
        temp = temp2;
    }

    old  = (i32) client->decrypt_key.buf[0] & 0xff;
    old |= (i32) client->decrypt_key.buf[1] << 8 & 0xff00;
    old |= (i32) client->decrypt_key.buf[2] << 0x10 & 0xff0000;
    old |= (i32) client->decrypt_key.buf[3] << 0x18 & 0xff000000;

    old += body_size;

    client->decrypt_key.buf[0] = (byte) (old & 0xff);
    client->decrypt_key.buf[1] = (byte) (old >> 0x08 & 0xff);
    client->decrypt_key.buf[2] = (byte) (old >> 0x10 & 0xff);
    client->decrypt_key.buf[3] = (byte) (old >> 0x18 & 0xff);
}