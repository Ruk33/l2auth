#include "encrypt.h"

void encrypt(unsigned char *data, unsigned int len, unsigned char *key)
{
        int temp = 0;
        int temp2 = 0;
        unsigned int old = 0;

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
}
