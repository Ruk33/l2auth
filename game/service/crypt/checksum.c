#include <stdlib.h>
#include <log/log.h>
#include "checksum.h"

int game_service_crypt_checksum(unsigned char* data, size_t len)
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
