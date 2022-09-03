#include <assert.h>
#include "../../include/util.h"
#include "../../include/packet.h"
#include "include/packet_decoder.h"

void decode_auth_login(struct auth_login *dest, struct packet *src)
{
	assert(dest);
	assert(src);

	byte *content = packet_body(src) + 1;
	byte *username = content + 0x62;
	byte *password = content + 0x70;

	CPY_STR(dest->username.buf, username);
	CPY_STR(dest->password.buf, password);
}
