#include <assert.h>
#include "../../include/util.h"
#include "include/types.h"
#include "include/random.h"
#include "include/session.h"

void session_init(struct session *src)
{
	assert(src);

	struct crypt_key key = {{0x94, 0x35, 0x00, 0x00, 0xa1, 0x6c, 0x54, 0x87}};
	src->id = random_u32();
	src->encrypt_key = key;
	src->decrypt_key = key;
}

void encrypt_packet(struct session *session, struct packet *src)
{
	assert(session);
	assert(src);

	// packet_add_checksum(src);

	u16 src_size = packet_padded_size(src);
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

	// Update packet size.
	*((u16 *) src) = src_size;
}

void decrypt_packet(struct session *session, struct packet *src)
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

void session_queue_response(struct session *session, struct packet *src)
{
	session = session;
	src = src;
	// struct packet *encrypted_packet = 0;

	// assert(client);
	// assert(src);

	// if (client->response_queue_count == ARR_LEN(client->response_queue))
	// 	client->response_queue_count = 0;

	// encrypted_packet = &client->response_queue[client->response_queue_count];
	// session_encrypt(client, encrypted_packet);

	// client->response_queue_count += 1;
}
