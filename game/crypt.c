#include <log/log.h>
#include <game/crypt.h>

void game_crypt_encrypt
(
        unsigned char* data,
        unsigned int len,
        unsigned char* key
)
{
        int temp = 0;
        int temp2 = 0;
        unsigned int old = 0;

        if (!data || !len) {
                log_warn("Trying to encrypt game server packet with no data");
                return;
        }

        if (!key) {
                log_fatal("Trying to encrypt game server packet with no key");
                return;
        }

        for (unsigned int i = 0; i < len; i++) {
               temp2 = data[i] & 0xff;
               data[i] = (unsigned char) (temp2 ^ key[i & 7] ^ temp);
               temp = data[i];
        };

        old  = ((unsigned int) key[0] &0xff);
        old |= ((unsigned int) key[1] << 8 &0xff00);
        old |= ((unsigned int) key[2] << 0x10 &0xff0000);
        old |= ((unsigned int) key[3] << 0x18 &0xff000000);

        old += len;

        key[0] = (unsigned char) (old &0xff);
        key[1] = (unsigned char) (old >> 0x08 &0xff);
        key[2] = (unsigned char) (old >> 0x10 &0xff);
        key[3] = (unsigned char) (old >> 0x18 &0xff);
};

void game_crypt_decrypt
(
        unsigned char* data,
        unsigned int len,
        unsigned char* key
)
{
        int temp = 0;
        int temp2 = 0;
        unsigned int old = 0;

        for (unsigned int i = 0; i < len; i++) {
               temp2 = data[i];
               data[i] = (unsigned char) (temp2 ^ key[i & 7] ^ temp);
               temp = temp2;
        };

        old  = (unsigned int) key[0] & 0xff;
        old |= (unsigned int) key[1] << 8 & 0xff00;
        old |= (unsigned int) key[2] << 0x10 & 0xff0000;
        old |= (unsigned int) key[3] << 0x18 & 0xff000000;

        old += len;

        key[0] = (unsigned char) (old & 0xff);
        key[1] = (unsigned char) (old >> 0x08 &0xff);
        key[2] = (unsigned char) (old >> 0x10 &0xff);
        key[3] = (unsigned char) (old >> 0x18 &0xff);
};

int game_crypt_checksum(unsigned char* data, size_t len)
{
        long chksum = 0;
        size_t count = (size_t) (len - 4);
        size_t i =0;
        long ecx;

        for (i = 0; i < count; i += 4) {
                ecx  = (long) data[i] &0xff;
                ecx |= (long) data[i+1] << 8 &0xff00;
                ecx |= (long) data[i+2] << 0x10 &0xff0000;
                ecx |= (long) data[i+3] << 0x18 &0xff000000;
                
                chksum ^= ecx;
        }

        ecx  = (long) (data[i] &0xff);
        ecx |= (long) (data[i+1] << 8 &0xff00);
        ecx |= (long) (data[i+2] << 0x10 &0xff0000);
        ecx |= (long) (data[i+3] << 0x18 &0xff000000);

        data[i] = (unsigned char) (chksum &0xff);
        data[i+1] = (unsigned char) (chksum >>0x08 &0xff);
        data[i+2] = (unsigned char) (chksum >>0x10 &0xff);
        data[i+3] = (unsigned char) (chksum >>0x18 &0xff);

        log_info("ecx: %d, chksum %d", ecx, chksum);
        if (ecx != chksum) log_fatal("checksum isn't correct");
        if (ecx == chksum) log_info("checksum succeed ----------------");

        return ecx == chksum;
}
