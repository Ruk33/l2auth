#include "include/game_server.h"

void game_session_encrypt_packet(struct game_session *session, struct packet *src)
{
    assert(session);
    assert(src);

    // should we perform checksum here or should we
    // do it in each encoding function?
    packet_checksum(src);

    u16 src_size = packet_size(src);
    byte *src_body = packet_body(src);

    i32 temp = 0;
    i32 temp2 = 0;
    // -2, ignore first two bytes of packet size.
    for (u16 i = 0; i < (src_size - 2); i++) {
        temp2 = src_body[i] & 0xff;
        src_body[i] = (byte) (temp2 ^ session->encrypt_key.buf[i & 7] ^ temp);
        temp = src_body[i];
    }

    u32 old = ((u32) session->encrypt_key.buf[0] & 0xff);
    old |= ((u32) session->encrypt_key.buf[1] << 8 & 0xff00);
    old |= ((u32) session->encrypt_key.buf[2] << 0x10 & 0xff0000);
    old |= ((u32) session->encrypt_key.buf[3] << 0x18 & 0xff000000);

    old += (src_size - 2);

    session->encrypt_key.buf[0] = (byte) (old & 0xff);
    session->encrypt_key.buf[1] = (byte) (old >> 0x08 & 0xff);
    session->encrypt_key.buf[2] = (byte) (old >> 0x10 & 0xff);
    session->encrypt_key.buf[3] = (byte) (old >> 0x18 & 0xff);

    // update packet size.
    *((u16 *) src) = src_size;
}

void game_session_decrypt_packet(struct game_session *session, struct packet *src)
{
    assert(session);
    assert(src);

    i32 temp = 0;
    i32 temp2 = 0;

    if (!session->conn_encrypted)
        return;

    u16 body_size = packet_size(src);
    body_size = body_size > 1 ? body_size - 2 : body_size;

    byte *src_body  = packet_body(src);

    for (u16 i = 0; i < body_size; i++) {
        temp2 = src_body[i];
        src_body[i] = (byte) (temp2 ^ session->decrypt_key.buf[i & 7] ^ temp);
        temp = temp2;
    }

    u32 old = (i32) session->decrypt_key.buf[0] & 0xff;
    old |= (i32) session->decrypt_key.buf[1] << 8 & 0xff00;
    old |= (i32) session->decrypt_key.buf[2] << 0x10 & 0xff0000;
    old |= (i32) session->decrypt_key.buf[3] << 0x18 & 0xff000000;

    old += body_size;

    session->decrypt_key.buf[0] = (byte) (old & 0xff);
    session->decrypt_key.buf[1] = (byte) (old >> 0x08 & 0xff);
    session->decrypt_key.buf[2] = (byte) (old >> 0x10 & 0xff);
    session->decrypt_key.buf[3] = (byte) (old >> 0x18 & 0xff);
}

struct packet *game_session_get_free_response(struct game_session *session)
{
    assert(session);
    assert(session->response_queue_count+1 < arr_len(session->response_queue));
    struct packet *response = session->response_queue+session->response_queue_count;
    session->response_queue_count++;
    return response;
}
